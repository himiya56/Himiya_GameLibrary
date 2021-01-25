//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "fade.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
D3DXCOLOR CFade::m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
CFade::FADE	CFade::m_fade = FADE_NONE;
CManager::MODE	CFade::m_modeNext = CManager::MODE_GAME_STAGE_01;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFade::CFade()
{
	m_pVtxBuff = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFade::Init(void)
{
	// �C���X�^���X����
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�̐���
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].rhw = 1.0f;
		pVtx[nCount].col = D3DCOLOR_RGBA(0, 0, 0, 0);
	}

	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFade::Uninit(void)
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
void CFade::Update(void)
{
	VERTEX_2D* pVtx;

	switch (m_fade)
	{
	case FADE_IN:
		m_col.a += FADE_RATE;
		if (m_col.a >=1.0f)
		{
			m_col.a = 1.0f;
			CManager::SetMode(m_modeNext);
			m_fade = FADE_OUT;
		}
		break;
	case FADE_OUT:
		m_col.a -= FADE_RATE;
		if (m_col.a <= 0.0f)
		{
			m_col.a = 0.0f;
			m_fade = FADE_NONE;
		}
		break;
	default:
		break;
	}
	// ���_�o�b�t�@�̍X�V
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].col = m_col;
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFade::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pRenderer->GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pRenderer->GetDevice()->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pRenderer->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// �t�F�[�h�C������
//=============================================================================
void CFade::SetFade(CManager::MODE mode)
{
	m_fade = FADE_IN;
	m_col.a = 1.0f;

	m_modeNext = mode;
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CFade * CFade::Create(void)
{
	CFade *pFade;
	pFade = new CFade;
	pFade->Init();
	return pFade;
}