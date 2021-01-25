//=============================================================================
//
// �v���C���[���C�t���� [life.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "life.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLife::m_apTexture[TEX_LIFE_MAX] = {};
float CLife::m_fLife = 4.0f;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLife::CLife()
{
	//m_pFont = NULL;
	m_fRot = D3DXToRadian(0.0f);
	m_fSizeX = LIFE_WIDTH;
	m_fSizeY = LIFE_HEIGHT;
	m_fMaxLife = 4.0f;
	m_fLife = 4.0f;
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLife::~CLife()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLife::Init(void)
{
	for (int nCount = 0; nCount < NUM_LIFE_POLYGON; nCount++)
	{
		// ����������
		m_apPolygon[nCount] = new CPolygon2D;
		m_apPolygon[nCount]->Init();
		// ���W�ݒ�
		m_apPolygon[nCount]->SetPos(D3DXVECTOR3(LIFE_WIDTH / 2 + 50, 50, 0.0f));
		// �e�N�X�`�����蓖��
		m_apPolygon[nCount]->BindTexture(m_apTexture[nCount]);
		m_apPolygon[nCount]->SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);
	}

	m_apPolygon[TEX_LIFE_0]->SetSize(LIFE_WIDTH + 20, LIFE_HEIGHT + 20);
	m_apPolygon[TEX_LIFE_01]->SetSize(m_fSizeX, m_fSizeY);

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CLife::Update()
{
	for (int nCount = 0; nCount < NUM_LIFE_POLYGON; nCount++)
	{
		m_apPolygon[nCount]->SetRot(m_fRot);
		m_apPolygon[TEX_LIFE_0]->SetSize(LIFE_WIDTH + 20, LIFE_HEIGHT + 20);

		float fSizeX = m_fSizeX * (m_fLife / m_fMaxLife);
		m_apPolygon[TEX_LIFE_01]->SetSize(fSizeX, m_fSizeY);

		m_apPolygon[TEX_LIFE_01]->SetPos(D3DXVECTOR3(LIFE_WIDTH / 2 + 50, 50.0f, 0.0f));
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLife::Draw()
{
}

//=============================================================================
// �I������
//=============================================================================
void CLife::Uninit()
{
	Release();
}

//=============================================================================
// �e�N�X�`���ǂݍ���
//=============================================================================
HRESULT CLife::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/White.png", &m_apTexture[TEX_LIFE_0]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Red.jpg", &m_apTexture[TEX_LIFE_01]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CLife::Unload(void)
{
	for (int nCount = 0; nCount < NUM_LIFE_POLYGON; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CLife * CLife::Create(void)
{
	CLife *pLife;
	pLife = new CLife;
	pLife->Init();
	pLife->SetObjType(OBJ_TYPE_HUD);
	return pLife;
}

//=============================================================================
// ���C�t��1���炷
//=============================================================================
void CLife::SubtractLife(void)
{
	m_fLife -= 1.0f;
	if (m_fLife <= 0)
	{
		m_fLife = 0.0f;
	}
}