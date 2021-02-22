//=============================================================================
//
// �|���S��2D���� [polygon2d.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "polygon2d.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "player.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPolygon2D::CPolygon2D()
{
	// �C���X�^���X�N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_TexLeftX = 0.0f;
	m_TexRightX = 1.0f;
	m_TexTopY = 0.0f;
	m_TexBottomY = 1.0f;
	m_pFont = NULL;
	m_pTexture = NULL;
	m_fRot = D3DXToRadian(0);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPolygon2D::~CPolygon2D()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPolygon2D::Init(void)
{
	// �C���X�^���X����
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	//m_pPlayer = CManager::GetPlayer();

	pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�̐���
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(m_TexLeftX, m_TexTopY);
	pVtx[1].tex = D3DXVECTOR2(m_TexRightX, m_TexBottomY);
	pVtx[2].tex = D3DXVECTOR2(m_TexLeftX, m_TexTopY);
	pVtx[3].tex = D3DXVECTOR2(m_TexRightX, m_TexBottomY);

	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].rhw = 1.0f;
		pVtx[nCount].col = m_col;
	}

	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPolygon2D::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPolygon2D::Update(void)
{
	VERTEX_2D *pVtx;
	float fAngle;

	fAngle = atan2f(m_fSizeX / 2, m_fSizeY / 2);

	// ���_�o�b�t�@�̐���
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�����X�V
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (cosf(fAngle + m_fRot) * sqrtf(powf(m_fSizeX / 2, 2) + powf(m_fSizeY / 2, 2))), m_pos.y - (sinf(fAngle + m_fRot) * sqrtf(powf(m_fSizeX / 2, 2) + powf(m_fSizeY / 2, 2))), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (cosf(fAngle - m_fRot) * sqrtf(powf(m_fSizeX / 2, 2) + powf(m_fSizeY / 2, 2))), m_pos.y - (sinf(fAngle - m_fRot) * sqrtf(powf(m_fSizeX / 2, 2) + powf(m_fSizeY / 2, 2))), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (cosf(fAngle - m_fRot) * sqrtf(powf(m_fSizeX / 2, 2) + powf(m_fSizeY / 2, 2))), m_pos.y + (sinf(fAngle - m_fRot) * sqrtf(powf(m_fSizeX / 2, 2) + powf(m_fSizeY / 2, 2))), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (cosf(fAngle + m_fRot) * sqrtf(powf(m_fSizeX / 2, 2) + powf(m_fSizeY / 2, 2))), m_pos.y + (sinf(fAngle + m_fRot) * sqrtf(powf(m_fSizeX / 2, 2) + powf(m_fSizeY / 2, 2))), 0.0f);

	pVtx[0].tex = D3DXVECTOR2(m_TexLeftX, m_TexTopY);
	pVtx[1].tex = D3DXVECTOR2(m_TexRightX, m_TexTopY);
	pVtx[2].tex = D3DXVECTOR2(m_TexLeftX, m_TexBottomY);
	pVtx[3].tex = D3DXVECTOR2(m_TexRightX, m_TexBottomY);

	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].col = m_col;
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPolygon2D::Draw(void)
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
CPolygon2D * CPolygon2D::Create(void)
{
	CPolygon2D *pPolygon2D;
	pPolygon2D = new CPolygon2D;
	pPolygon2D->Init();
	return pPolygon2D;
}

//=============================================================================
// �e�N�X�`���쐬����
//=============================================================================
void CPolygon2D::BindTexture(const LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}

//=============================================================================
// �|���S���F�ݒ菈��
//=============================================================================
void CPolygon2D::SetCol(const D3DXCOLOR col)
{
	m_col = col;
}


//=============================================================================
// �|���S���ʒu�ݒ菈��
//=============================================================================
void CPolygon2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �|���S����]�ݒ菈��
//=============================================================================
void CPolygon2D::SetRot(const float rot)
{
	m_fRot = rot;
}

//=============================================================================
// �|���S���T�C�YX�ݒ菈��
//=============================================================================
void CPolygon2D::SetSize(const float sizey, const float sizex)
{
	m_fSizeY = sizey;
	m_fSizeX = sizex;
}

//=============================================================================
// �e�N�X�`��UV���W�ݒ菈��
//=============================================================================
void CPolygon2D::SetTexUV(const float leftX, const float rightX,const float topY, const float bottomY)
{
	m_TexLeftX = leftX;
	m_TexRightX = rightX;
	m_TexTopY = topY;
	m_TexBottomY = bottomY;
}

//=============================================================================
// ���W�擾�p
//=============================================================================
D3DXVECTOR3 CPolygon2D::GetPosition(void) const
{
	return m_pos;
}

//=============================================================================
// �e�N�X�`��UV���W�擾�p
//=============================================================================
void CPolygon2D::GetTexUV(float* leftX,float* rightX,float* topY,float* bottomY)
{
	if (leftX != NULL)
	{
		*leftX = m_TexLeftX;
	}
	if (rightX != NULL)
	{
		*rightX = m_TexRightX;
	}
	if (topY != NULL)
	{
		*topY = m_TexTopY;
	}
	if (bottomY != NULL)
	{
		*bottomY = m_TexBottomY;
	}
}
