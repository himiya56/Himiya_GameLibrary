//===============================================
//
// 3Dポリゴン処理 (scene3d.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "polygon3d.h"
#include "manager.h"
#include "renderer.h"


//=============================================================================
// コンストラクタ
//=============================================================================
CPolygon3D::CPolygon3D()
{
	m_pTexture = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nDir = DIR_RIGHT;
	m_fSizeX = POLYGON_SIZE;
	m_fSizeY = POLYGON_SIZE;
	m_TexLeftX = 0.0f;
	m_TexRightX = 1.0f;
	m_TexTopY = 0.0f;
	m_TexBottomY = 1.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPolygon3D::~CPolygon3D()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPolygon3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_nDir = DIR_RIGHT;
	m_fSizeX = POLYGON_SIZE;
	m_fSizeY = POLYGON_SIZE;

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	return S_OK;
}

//=============================================================================
// 終了処理
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
// 更新処理
//=============================================================================
void CPolygon3D::Update(void)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fSizeX / 2, m_pos.y + m_fSizeY / 2, m_Type);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX / 2, m_pos.y + m_fSizeY / 2, m_Type);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fSizeX / 2, m_pos.y - m_fSizeY / 2, m_Type);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX / 2, m_pos.y - m_fSizeY / 2, m_Type);

	pVtx[0 + m_nDir].tex = D3DXVECTOR2(m_TexLeftX, m_TexTopY);
	pVtx[1 - m_nDir].tex = D3DXVECTOR2(m_TexRightX, m_TexTopY);
	pVtx[2 + m_nDir].tex = D3DXVECTOR2(m_TexLeftX, m_TexBottomY);
	pVtx[3 - m_nDir].tex = D3DXVECTOR2(m_TexRightX, m_TexBottomY);


	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[nCount].col = m_col;
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
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

	pDevice->SetTexture(0, m_pTexture);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// UV座標設定処理
//=============================================================================
void CPolygon3D::SetTexUV(const float leftX, const float rightX, const float topY, const float bottomY)
{
	m_TexLeftX = leftX;
	m_TexRightX = rightX;
	m_TexTopY = topY;
	m_TexBottomY = bottomY;
}

//=============================================================================
// UV座標取得用
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