//=============================================================================
//
// �����̕\������ [number.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�̃|�C���^
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_fRot = D3DXToRadian(0.0f); // ��]����p�x
	m_fSizeY = NUMBER_WIDTH;
	m_fSizeX = NUMBER_HEIGHT;
	m_nNumber = 0;
	m_fTexUVX = 1.0f;	// �e�N�X�`��UV x���W
	m_fTexUVY = 1.0f;	// �e�N�X�`��UV y���W
	m_fTexSegX = 1.0f;  // �e�N�X�`���������� 
	m_fTexSegY = 1.0f;  // �e�N�X�`���c������
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture, 10.0f, 1.0f);

	pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�̐���
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(pos.x - m_fSizeX / 2, pos.y - m_fSizeY / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + m_fSizeX / 2, pos.y - m_fSizeY / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - m_fSizeX / 2, pos.y + m_fSizeY / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + m_fSizeX / 2, pos.y + m_fSizeY / 2, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(m_fTexUVX - 1.0f / m_fTexSegX, m_fTexUVY);
	pVtx[1].tex = D3DXVECTOR2(m_fTexUVX, m_fTexUVY);
	pVtx[2].tex = D3DXVECTOR2(m_fTexUVX - 1.0f / m_fTexSegX, m_fTexUVY + 1.0f / m_fTexSegY);
	pVtx[3].tex = D3DXVECTOR2(m_fTexUVX, m_fTexUVY + 1.0f / m_fTexSegY);

	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].rhw = 1.0f;
		pVtx[nCount].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNumber::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CNumber::Update(void)
{
	VERTEX_2D *pVtx;
	float TexY1, TexY2;

	TexY1 = m_fTexUVY - (1.0f / m_fTexSegY);
	TexY2 = m_fTexUVY;

	// ���_�o�b�t�@�̍X�V
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(m_nNumber * 0.1f, TexY1);
	pVtx[1].tex = D3DXVECTOR2(m_nNumber * 0.1f + 0.1f, TexY1);
	pVtx[2].tex = D3DXVECTOR2(m_nNumber * 0.1f, TexY2);
	pVtx[3].tex = D3DXVECTOR2(m_nNumber * 0.1f + 0.1f, TexY2);

	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].col = m_col;
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CNumber::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pRenderer->GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pRenderer->GetDevice()->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pRenderer->GetDevice()->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pRenderer->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CNumber * CNumber::Create(D3DXVECTOR3 pos)
{
	CNumber *pNumber;
	pNumber = new CNumber;
	pNumber->Init(pos);
	return pNumber;
}

//=============================================================================
// �e�N�X�`�����蓖�ď���
//=============================================================================
void CNumber::BindTexture(const LPDIRECT3DTEXTURE9 pTex, const float segX, const float segY)
{
	m_pTexture = pTex;
	m_fTexSegX = segX;
	m_fTexSegY = segY;
	m_fTexUVX = 1.0f / segX;
	m_fTexUVY = 1.0f / segY;
}

//=============================================================================
// ���W�ݒ菈��
//=============================================================================
void CNumber::SetPolygonPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// ��]�ݒ菈��
//=============================================================================
void CNumber::SetPolygonRot(const float rot)
{
	m_fRot = rot;
}

//=============================================================================
// �g�k�ݒ菈��
//=============================================================================
void CNumber::SetPolygonSize(const float sizey, const float sizex)
{
	m_fSizeX = sizex;
	m_fSizeY = sizey;
}

//=============================================================================
// �e�N�X�`��UV���W�ݒ菈��
//=============================================================================
void CNumber::SetTexSeg(const float segx, const float segy)
{
	m_fTexUVX = segx;
	m_fTexUVY = segy;
}

//=============================================================================
// �����ݒ菈��
//=============================================================================
void CNumber::SetNumber(const int nNum)
{
	m_nNumber = nNum;
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CNumber::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/number000.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CNumber::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
