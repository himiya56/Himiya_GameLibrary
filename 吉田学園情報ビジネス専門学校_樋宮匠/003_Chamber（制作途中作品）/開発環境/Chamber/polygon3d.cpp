//===============================================
//
// 3D�|���S������ (scene3d.cpp)
// Author : ��{��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "polygon3d.h"
#include "manager.h"
#include "renderer.h"


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPolygon3D::CPolygon3D()
{
	m_pTexture = NULL;
	memset(m_pBlendingTexture, 0, TEXTURE_BLENDING_MAX);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pVtxBuff = NULL;
	m_pos = DEFAULT_VECTOR;
	m_rot = DEFAULT_VECTOR;
	m_scale = DEFAULT_SCALE;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nTexDir = DIR_RIGHT;
	m_fWidth = POLYGON_SIZE;
	m_fHeight = POLYGON_SIZE;
	m_TexLeftX = 0.0f;
	m_TexRightX = 1.0f;
	m_TexTopY = 0.0f;
	m_TexBottomY = 1.0f;
	m_bUseTextureBlend = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPolygon3D::~CPolygon3D()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPolygon3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_nTexDir = DIR_RIGHT;
	m_fWidth = POLYGON_SIZE;
	m_fHeight = POLYGON_SIZE;

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPolygon3D::Uninit(void)
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
void CPolygon3D::Update(void)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2, m_fHeight / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3( m_fWidth / 2, m_fHeight / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2,-m_fHeight / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3( m_fWidth / 2,-m_fHeight / 2, 0.0f);

	pVtx[0 + m_nTexDir].tex = D3DXVECTOR2(m_TexLeftX, m_TexTopY);
	pVtx[1 - m_nTexDir].tex = D3DXVECTOR2(m_TexRightX, m_TexTopY);
	pVtx[2 + m_nTexDir].tex = D3DXVECTOR2(m_TexLeftX, m_TexBottomY);
	pVtx[3 - m_nTexDir].tex = D3DXVECTOR2(m_TexRightX, m_TexBottomY);


	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[nCount].col = m_col;
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPolygon3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���u�����f�B���O���g�p����Ȃ�
	if (m_bUseTextureBlend)
	{
		pDevice->SetTexture(0, m_pBlendingTexture[0]);
		pDevice->SetTexture(1, m_pBlendingTexture[1]);
	}
	else
	{
		pDevice->SetTexture(0, m_pTexture);
	}

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �e�N�X�`���u�����f�B���O����
//=============================================================================
void CPolygon3D::BlendingTexture(const LPDIRECT3DTEXTURE9 pTex1, const LPDIRECT3DTEXTURE9 pTex2)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_pBlendingTexture[0] = pTex1;
	m_pBlendingTexture[1] = pTex2;

	m_bUseTextureBlend = true;
}

//=============================================================================
// UV���W�ݒ菈��
//=============================================================================
void CPolygon3D::SetTexUV(const float leftX, const float rightX, const float topY, const float bottomY)
{
	m_TexLeftX = leftX;
	m_TexRightX = rightX;
	m_TexTopY = topY;
	m_TexBottomY = bottomY;
}

//=============================================================================
// UV���W�擾�p
//=============================================================================
void CPolygon3D::GetTexUV(float * leftX, float * rightX, float * topY, float * bottomY)
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

//=============================================================================
// �@���v�Z
//=============================================================================
D3DXVECTOR3 CPolygon3D::GetNormal(void)
{
	D3DXVECTOR3 normalize, normal;
	D3DXMATRIX mtxRot;

	// �|���̖@��
	normalize = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ��]�s��̍쐬
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	// ���W��ϊ�
	D3DXVec3TransformCoord(&normalize, &normalize, &mtxRot);

	// ���K��
	D3DXVec3Normalize(&normal, &normalize);

	return normal;
}

//=============================================================================
// �@���ƒn��(zx����)�̊p�x�̌v�Z
//=============================================================================
float CPolygon3D::GetNormalandGrandAngle(void)
{
	D3DXVECTOR3 zxNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vec1, normal;
	float fAngle, fVec1, fVec2, fAbs1, fAbs2;

	// ���q�̌v�Z(�x�N�g��*�x�N�g��)
	normal = GetNormal();
	vec1.x = zxNormal.x * normal.x;
	vec1.y = zxNormal.y * normal.y;
	vec1.z = zxNormal.z * normal.z;
	fVec1 = vec1.x + vec1.y + vec1.z;

	// ����̌v�Z(�x�N�g���̐�Βl*�x�N�g���̐�Βl)
	fAbs1 = sqrtf(powf(zxNormal.x, 2.0f) + powf(zxNormal.y, 2.0f) + powf(zxNormal.z, 2.0f));
	fAbs2 = sqrtf(powf(normal.x, 2.0f) + powf(normal.y, 2.0f) + powf(normal.z, 2.0f));
	fVec2 = fAbs1 * fAbs2;

	// �p�x�̌v�Z
	fAngle = acosf(fVec1 / fVec2);
	//fAngle = D3DXToRadian(90) - fAngle;

	return fAngle;
}
