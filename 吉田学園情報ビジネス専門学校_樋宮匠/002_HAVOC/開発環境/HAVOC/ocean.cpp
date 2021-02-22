//====================================================================
//
// 海の処理 (ocean.cpp)
// Author : 樋宮匠
//
//====================================================================

//================================================
// インクルードファイル
//================================================
#include "ocean.h"
#include "library.h"
#include "manager.h"
#include "renderer.h"

//========================
// 静的メンバ変数宣言
//========================
LPDIRECT3DTEXTURE9 COcean::m_apTexture[TYPE_MAX] = {};

//========================================================
// コンストラクタ
// Author : 樋宮匠
//========================================================
COcean::COcean() :CBillboard(OBJTYPE_WORLD)
{
    m_type = TYPE_SURFACE_0;
    m_fTexX = 0.0f;
    m_fTexY = 0.0f;
    m_nAlpha = 255;
    m_bAddAlpha = true;
}

//========================================================
// デストラクタ
// Author : 樋宮匠
//========================================================
COcean::~COcean()
{
}

//========================================================
// 初期化処理
// Author : 樋宮匠
//========================================================
HRESULT COcean::Init(void)
{
    // 海の初期化
    CBillboard::Init(DEFAULT_VECTOR, D3DXVECTOR3(OCEAN_SIZE, OCEAN_SIZE, 0.0f));
    SetRot(HIT_TOP);
    BindTexture(m_apTexture[m_type]);
    m_fTexX = 0.0f;
    m_fTexY = 0.0f;
    m_nAlpha = 255;
    m_bAddAlpha = true;

    // アルファテストの設定値を変更
    SetAlphaTestBorder(0);

    // 加算合成を利用する
    SetAdditiveSynthesis();

    return S_OK;
}

//========================================================
// 終了処理
// Author : 樋宮匠
//========================================================
void COcean::Uninit(void)
{
    CBillboard::Uninit();
}

//========================================================
// 更新処理
// Author : 樋宮匠
//========================================================
void COcean::Update(void)
{
    // アルファ値を変更
    switch (m_type)
    {
    case TYPE_SURFACE_1:
        // アルファ値の計算
        CalcAlpha(MIDDLE_ALPHA_RATE);
        // テクスチャ座標の加算
        m_fTexX += MIDDLE_MOVE_TEX;
        m_fTexY += MIDDLE_MOVE_TEX;
        break;
    case TYPE_SURFACE_2:
        // アルファ値の計算
        CalcAlpha(TOP_ALPHA_RATE);
        // テクスチャ座標の加算
        m_fTexX += TOP_MOVE_TEX;
        m_fTexY += TOP_MOVE_TEX;
        break;
    default:
        break;
    }

    // 情報の更新
    SetTexUV(m_fTexX, 1.0f + m_fTexX, m_fTexY, 1.0f + m_fTexY);
    CBillboard::Update();
}

//========================================================
// 描画処理
// Author : 樋宮匠
//========================================================
void COcean::Draw(void)
{
    CBillboard::Draw();
}

//========================================================
// アルファ値計算
// Author : 樋宮匠
//========================================================
void COcean::CalcAlpha(int nAlphaRate)
{
    if (m_bAddAlpha)
    {
        m_nAlpha += nAlphaRate;
        SetCol(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
        if (m_nAlpha > ALPHA_MAX)
        {
            m_bAddAlpha = false;
        }
    }
    else
    {
        m_nAlpha -= nAlphaRate;
        SetCol(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
        if (m_nAlpha < ALPHA_MIN)
        {
            m_bAddAlpha = true;
        }
    }
}

//========================================================
// テクスチャ読み込み処理
// Author : 樋宮匠
//========================================================
HRESULT COcean::Load(void)
{
    D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data/TEXTURE/ocean_deep.png", &m_apTexture[TYPE_SURFACE_0]);
    D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data/TEXTURE/ocean_wave.png", &m_apTexture[TYPE_SURFACE_1]);
    D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data/TEXTURE/ocean_bubble.png", &m_apTexture[TYPE_SURFACE_2]);

    return S_OK;
}

//========================================================
// テクスチャ破棄処理
// Author : 樋宮匠
//========================================================
void COcean::Unload(void)
{
    for (int nCount = 0; nCount < TYPE_MAX; nCount++)
    {
        if (m_apTexture[nCount] != NULL)
        {
            m_apTexture[nCount]->Release();
            m_apTexture[nCount] = NULL;
        }
    }
}

//========================================================
// インスタンス生成処理
// Author : 樋宮匠
//========================================================
COcean * COcean::Create(TYPE type)
{
    COcean *pOcean;
    pOcean = new COcean;
    pOcean->m_type = type;
    pOcean->Init();

    switch (type)
    {
    case TYPE_SURFACE_0:
        pOcean->SetCol(D3DCOLOR_RGBA(255, 255, 255, 255));
        pOcean->SetPos(SURFACE_0_POS);
        pOcean->m_nAlpha = 255;
        break;
    case TYPE_SURFACE_1:
        pOcean->SetCol(D3DCOLOR_RGBA(255, 255, 255, 255));
        pOcean->SetPos(SURFACE_1_POS);
        pOcean->m_nAlpha = 100;
        break;
    case TYPE_SURFACE_2:
        pOcean->SetCol(D3DCOLOR_RGBA(255, 255, 255, 0));
        pOcean->SetPos(SURFACE_2_POS);
        break;
    default:
        break;
    }
    return pOcean;
}