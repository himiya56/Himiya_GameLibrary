//====================================================================
//
// 壁の処理 (wall.cpp)
// Author : 樋宮匠
//
//====================================================================

//================================================
// インクルードファイル
//================================================
#include "wall.h"
#include "library.h"
#include "manager.h"
#include "renderer.h"
#include "tile.h"

//========================
// 静的メンバ変数宣言
//========================
LPDIRECT3DTEXTURE9 CWall::m_pTexture = NULL;

//========================================================
// コンストラクタ
// Author : 樋宮匠
//========================================================
CWall::CWall() :CBillboard(OBJTYPE_WALL)
{
    m_fMapWidth = 0.0f;
    m_fTex = 0.0f;
    m_type = TYPE_WALL_NORTH;
    m_bAddAlpha = false;
    m_nAlpha = 255;
}

//========================================================
// デストラクタ
// Author : 樋宮匠
//========================================================
CWall::~CWall()
{
}

//========================================================
// 初期化処理
// Author : 樋宮匠
//========================================================
HRESULT CWall::Init(void)
{
    // 壁の初期化(座標はマップ端に設定した際のタイル1マス分のずれを+1する)
    CBillboard::Init(DEFAULT_VECTOR, D3DXVECTOR3(m_fMapWidth * TILE_SIZE, (m_fMapWidth * TILE_SIZE) / 2.0f, 0.0f));
    BindTexture(m_pTexture);
    SetAlphaTestBorder(0);

    return S_OK;
}

//========================================================
// 終了処理
// Author : 樋宮匠
//========================================================
void CWall::Uninit(void)
{
    CBillboard::Uninit();
}

//========================================================
// 更新処理
// Author : 樋宮匠
//========================================================
void CWall::Update(void)
{
    // テクスチャ座標更新
    m_fTex += 0.001f;
    if (m_fTex >= 1.0f)
    {
        m_fTex = 0.0f;
    }
    SetTexUV(0.0f, 1.0f, m_fTex, m_fTex + 0.7f);

    // アルファ値の処理
    switch (m_bAddAlpha)
    {
    case true:
        m_nAlpha++;

        if (m_nAlpha >= 255)
        {
            m_bAddAlpha = false;
        }

        SetCol(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
        break;
    case false:
        m_nAlpha--;

        if (m_nAlpha <= 100)
        {
            m_bAddAlpha = true;
        }

        SetCol(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
        break;
    }
    CBillboard::Update();
}

//========================================================
// 描画処理
// Author : 樋宮匠
//========================================================
void CWall::Draw(void)
{
    CBillboard::Draw();
}

//========================================================
// テクスチャ読み込み処理
// Author : 樋宮匠
//========================================================
HRESULT CWall::Load(void)
{
    D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data/TEXTURE/wall.png", &m_pTexture);

    return S_OK;
}

//========================================================
// テクスチャ破棄処理
// Author : 樋宮匠
//========================================================
void CWall::Unload(void)
{
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//========================================================
// インスタンス生成処理
// Author : 樋宮匠
//========================================================
CWall * CWall::Create(TYPE type, float fMapWidth)
{
    CWall *pWall;
    pWall = new CWall;
    pWall->m_type = type;
    pWall->m_fMapWidth = fMapWidth;
    pWall->Init();

    // 壁の向きによって初期値を設定(壁の高さは幅の半分なのでその分小さくする)
    switch (type)
    {
    case TYPE_WALL_NORTH:
        pWall->SetPos(D3DXVECTOR3((fMapWidth * TILE_SIZE) / 2 - TILE_SIZE / 2, (fMapWidth * TILE_SIZE) / 4, fMapWidth * TILE_SIZE - TILE_SIZE / 2));
        pWall->SetRot(HIT_FRONT);
        break;
    case TYPE_WALL_SOUTH:
        pWall->SetPos(D3DXVECTOR3((fMapWidth * TILE_SIZE) / 2 - TILE_SIZE / 2, (fMapWidth * TILE_SIZE) / 4, -TILE_SIZE / 2));
        pWall->SetRot(HIT_BACK);
        break;
    case TYPE_WALL_EAST:
        pWall->SetPos(D3DXVECTOR3((fMapWidth * TILE_SIZE - TILE_SIZE / 2), (fMapWidth * TILE_SIZE) / 4, (fMapWidth * TILE_SIZE) / 2 - TILE_SIZE / 2));
        pWall->SetRot(HIT_LEFT);
        break;
    case TYPE_WALL_WEST:
        pWall->SetPos(D3DXVECTOR3(-TILE_SIZE / 2, (fMapWidth * TILE_SIZE) / 4, (fMapWidth * TILE_SIZE) / 2 - TILE_SIZE / 2));
        pWall->SetRot(HIT_RIGHT);
        break;
    default:
        break;
    }
    return pWall;
}

//========================================================
// 壁の設置
// Author : 樋宮匠
//========================================================
void CWall::SetWall(float fMapWidth)
{
    //　北,南,東,西の順で壁を設置
    for (int nCount = 0; nCount < TYPE_MAX; nCount++)
    {
        Create((TYPE)nCount, fMapWidth);
    }
}