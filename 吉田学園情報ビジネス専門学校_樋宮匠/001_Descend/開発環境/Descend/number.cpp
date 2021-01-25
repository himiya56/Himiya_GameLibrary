//=============================================================================
//
// 数字の表示処理 [number.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CNumber::CNumber()
{
	m_pVtxBuff = NULL;		// 頂点バッファのポインタ
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_fRot = D3DXToRadian(0.0f); // 回転する角度
	m_fSizeY = NUMBER_WIDTH;
	m_fSizeX = NUMBER_HEIGHT;
	m_nNumber = 0;
	m_fTexUVX = 1.0f;	// テクスチャUV x座標
	m_fTexUVY = 1.0f;	// テクスチャUV y座標
	m_fTexSegX = 1.0f;  // テクスチャ横分割数 
	m_fTexSegY = 1.0f;  // テクスチャ縦分割数
}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber::~CNumber()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ割り当て
	BindTexture(m_pTexture, 10.0f, 1.0f);

	pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;

	// 頂点バッファの生成
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
// 終了処理
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
// 更新処理
//=============================================================================
void CNumber::Update(void)
{
	VERTEX_2D *pVtx;
	float TexY1, TexY2;

	TexY1 = m_fTexUVY - (1.0f / m_fTexSegY);
	TexY2 = m_fTexUVY;

	// 頂点バッファの更新
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
// 描画処理
//=============================================================================
void CNumber::Draw(void)
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
CNumber * CNumber::Create(D3DXVECTOR3 pos)
{
	CNumber *pNumber;
	pNumber = new CNumber;
	pNumber->Init(pos);
	return pNumber;
}

//=============================================================================
// テクスチャ割り当て処理
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
// 座標設定処理
//=============================================================================
void CNumber::SetPolygonPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 回転設定処理
//=============================================================================
void CNumber::SetPolygonRot(const float rot)
{
	m_fRot = rot;
}

//=============================================================================
// 拡縮設定処理
//=============================================================================
void CNumber::SetPolygonSize(const float sizey, const float sizex)
{
	m_fSizeX = sizex;
	m_fSizeY = sizey;
}

//=============================================================================
// テクスチャUV座標設定処理
//=============================================================================
void CNumber::SetTexSeg(const float segx, const float segy)
{
	m_fTexUVX = segx;
	m_fTexUVY = segy;
}

//=============================================================================
// 数字設定処理
//=============================================================================
void CNumber::SetNumber(const int nNum)
{
	m_nNumber = nNum;
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CNumber::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/number000.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void CNumber::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
