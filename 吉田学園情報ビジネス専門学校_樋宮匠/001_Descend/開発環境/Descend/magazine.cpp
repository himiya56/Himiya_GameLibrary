//=============================================================================
//
// マガジン処理 [magazine.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "magazine.h"
#include "manager.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CMagazine::m_apTexture[TEX_MAG_MAX] = {};
int CMagazine::m_nMag = MAX_MAG;

//=============================================================================
// コンストラクタ
//=============================================================================
CMagazine::CMagazine()
{
	m_fRot = D3DXToRadian(0.0f);
	m_fSizeX = POLYGON_SIZE;
	m_fSizeY = SCREEN_HEIGHT / 3;
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
	m_fTexUVY = 0.5f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMagazine::~CMagazine()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMagazine::Init(void)
{
	for (int nCount = 0; nCount < MAX_MAGAZINE_POLYGON; nCount++)
	{
		// 初期化処理
		m_apPolygon[nCount] = new CPolygon2D;
		m_apPolygon[nCount]->Init();
		// 座標設定
		m_apPolygon[nCount]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2 - POLYGON_SIZE * (MAP_WIDTH / 2)) + POLYGON_SIZE / 2 + (MAP_WIDTH * POLYGON_SIZE), SCREEN_HEIGHT / 2, 0.0f));
		m_apPolygon[nCount]->SetSize(m_fSizeX, m_fSizeY);
		// テクスチャ割り当て
		m_apPolygon[nCount]->BindTexture(m_apTexture[nCount]);
		m_apPolygon[nCount]->SetTexUV(0.0f, 1.0f, 0.5f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void CMagazine::Update()
{
	for (int nCount = 0; nCount < MAX_MAGAZINE_POLYGON; nCount++)
	{
		// テクスチャのUV座標を取得
		//m_fTexUVY = m_apPolygon[nCount]->GetTexSegY();

		m_apPolygon[nCount]->SetRot(m_fRot);
		m_apPolygon[nCount]->SetSize(m_fSizeX, m_fSizeY);
		m_apPolygon[nCount]->SetTexUV(0.0f, 1.0f, 0.25f * ((float)m_nMag / MAX_MAG), 0.5f + 0.25f * ((float)m_nMag / MAX_MAG));
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMagazine::Draw()
{
}

//=============================================================================
// 終了処理
//=============================================================================
void CMagazine::Uninit()
{
	Release();
}

//=============================================================================
// テクスチャ読み込み
//=============================================================================
HRESULT CMagazine::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Magazine_Bullet.png", &m_apTexture[TEX_MAG_01]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Block_02.png", &m_apTexture[TEX_MAG_0]);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄
//=============================================================================
void CMagazine::Unload(void)
{
	for (int nCount = 0; nCount < TEX_MAG_MAX; nCount++)
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
CMagazine * CMagazine::Create(void)
{
	CMagazine *pMag;
	pMag = new CMagazine;
	pMag->Init();
	pMag->SetObjType(OBJ_TYPE_HUD);
	return pMag;
}

//=============================================================================
// 残弾数を減らす
//=============================================================================
void CMagazine::SubtractMag(const int Value)
{
	m_nMag -= Value;
}

void CMagazine::AddMag(void)
{
	m_nMag += 1;
	if (m_nMag >= MAX_MAG)
	{
		m_nMag = MAX_MAG;
	}
}
