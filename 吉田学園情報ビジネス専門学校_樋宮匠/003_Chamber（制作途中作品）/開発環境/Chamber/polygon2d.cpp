//=============================================================================
//
// ポリゴン2D処理 [polygon2d.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "polygon2d.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPolygon2D::CPolygon2D()
{
	// インスタンスクリア
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
// デストラクタ
//=============================================================================
CPolygon2D::~CPolygon2D()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPolygon2D::Init(void)
{
	// インスタンス生成
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	//m_pPlayer = CManager::GetPlayer();

	pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;

	// 頂点バッファの生成
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
// 終了処理
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
// 更新処理
//=============================================================================
void CPolygon2D::Update(void)
{
	VERTEX_2D *pVtx;
	float fAngle;

	fAngle = atan2f(m_fSizeX / 2, m_fSizeY / 2);

	// 頂点バッファの生成
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を更新
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
// 描画処理
//=============================================================================
void CPolygon2D::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();

	// 頂点バッファをデータストリームに設定
	pRenderer->GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pRenderer->GetDevice()->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pRenderer->GetDevice()->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pRenderer->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CPolygon2D * CPolygon2D::Create(void)
{
	CPolygon2D *pPolygon2D;
	pPolygon2D = new CPolygon2D;
	pPolygon2D->Init();
	return pPolygon2D;
}

//=============================================================================
// テクスチャ作成処理
//=============================================================================
void CPolygon2D::BindTexture(const LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}

//=============================================================================
// ポリゴン色設定処理
//=============================================================================
void CPolygon2D::SetCol(const D3DXCOLOR col)
{
	m_col = col;
}


//=============================================================================
// ポリゴン位置設定処理
//=============================================================================
void CPolygon2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// ポリゴン回転設定処理
//=============================================================================
void CPolygon2D::SetRot(const float rot)
{
	m_fRot = rot;
}

//=============================================================================
// ポリゴンサイズX設定処理
//=============================================================================
void CPolygon2D::SetSize(const float sizey, const float sizex)
{
	m_fSizeY = sizey;
	m_fSizeX = sizex;
}

//=============================================================================
// テクスチャUV座標設定処理
//=============================================================================
void CPolygon2D::SetTexUV(const float leftX, const float rightX,const float topY, const float bottomY)
{
	m_TexLeftX = leftX;
	m_TexRightX = rightX;
	m_TexTopY = topY;
	m_TexBottomY = bottomY;
}

//=============================================================================
// 座標取得用
//=============================================================================
D3DXVECTOR3 CPolygon2D::GetPosition(void) const
{
	return m_pos;
}

//=============================================================================
// テクスチャUV座標取得用
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
