//=============================================================================
//
// プレイヤーライフ処理 [life.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "life.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLife::m_apTexture[TEX_LIFE_MAX] = {};
float CLife::m_fLife = 4.0f;

//=============================================================================
// コンストラクタ
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
// デストラクタ
//=============================================================================
CLife::~CLife()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLife::Init(void)
{
	for (int nCount = 0; nCount < NUM_LIFE_POLYGON; nCount++)
	{
		// 初期化処理
		m_apPolygon[nCount] = new CPolygon2D;
		m_apPolygon[nCount]->Init();
		// 座標設定
		m_apPolygon[nCount]->SetPos(D3DXVECTOR3(LIFE_WIDTH / 2 + 50, 50, 0.0f));
		// テクスチャ割り当て
		m_apPolygon[nCount]->BindTexture(m_apTexture[nCount]);
		m_apPolygon[nCount]->SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);
	}

	m_apPolygon[TEX_LIFE_0]->SetSize(LIFE_WIDTH + 20, LIFE_HEIGHT + 20);
	m_apPolygon[TEX_LIFE_01]->SetSize(m_fSizeX, m_fSizeY);

	return S_OK;
}

//=============================================================================
// 更新処理
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
// 描画処理
//=============================================================================
void CLife::Draw()
{
}

//=============================================================================
// 終了処理
//=============================================================================
void CLife::Uninit()
{
	Release();
}

//=============================================================================
// テクスチャ読み込み
//=============================================================================
HRESULT CLife::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/White.png", &m_apTexture[TEX_LIFE_0]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Red.jpg", &m_apTexture[TEX_LIFE_01]);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄
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
// インスタンス生成処理
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
// ライフを1減らす
//=============================================================================
void CLife::SubtractLife(void)
{
	m_fLife -= 1.0f;
	if (m_fLife <= 0)
	{
		m_fLife = 0.0f;
	}
}