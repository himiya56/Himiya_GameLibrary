//====================================================================
//
// ミニマップの処理 (minimap.cpp)
// Author : 樋宮匠
//
//====================================================================

//================================================
// インクルードファイル
//================================================
#include "minimap.h"
#include "game.h"
#include "player.h"

//========================
// 静的メンバ変数宣言
//========================

//========================================================
// コンストラクタ
// Author : 樋宮匠
//========================================================
CMinimap::CMinimap():CScene2D(CScene::OBJTYPE_UI)
{
    m_fAngle = 0.0f;
    m_fDistance = 0.0f;
    m_fieldPos = DEFAULT_VECTOR;
    m_bUse = false;

    m_bDisp = false;
    m_nCntTime = 0;

    m_type = ICON_NONE;
}

//========================================================
// デストラクタ
// Author : 樋宮匠
//========================================================
CMinimap::~CMinimap()
{
}

//========================================================
// 初期化処理
// Author : 樋宮匠
//========================================================
HRESULT CMinimap::Init(void)
{
    // マップの初期化
    CScene2D::Init(D3DXVECTOR3(MINIMAP_POS_X, MINIMAP_POS_Y, 0.0f), D3DXVECTOR3(ENEMY_ICON_SIZE, ENEMY_ICON_SIZE, 0.0f));
    SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
    SetVertex();

    return S_OK;
}

//========================================================
// 終了処理
// Author : 樋宮匠
//========================================================
void CMinimap::Uninit(void)
{
    CScene2D::Uninit();
}

//========================================================
// 更新処理
// Author : 樋宮匠
//========================================================
void CMinimap::Update(void)
{
    // レーダーに敵が表示されるまでのタイマー
    if (!m_bDisp)
    {
        // カウントアップ
        m_nCntTime++;

        // 一定時間経過したら
        if (m_nCntTime >= MINIMAP_ICON_DISP_DELAY)
        {
            m_bDisp = true;
        }
    }

    // マップアイコンを使用しているなら
    if (m_bUse)
    {
        // 座標計算用変数
        D3DXVECTOR3 pos = DEFAULT_VECTOR;

        // 角度を同期
        m_fAngle = CalculateAngle();

        // 距離を同期
        m_fDistance = CalculateDistance();

        // ミニマップからはみ出したら距離を修正
        if (m_fDistance >= MAX_ICON_RANGE)
        {
            m_fDistance = MAX_ICON_RANGE;
        }

        // ミニマップ中心点からの角度から円周上の座標を計算
        pos.x = m_fDistance * cosf(m_fAngle) + MINIMAP_POS_X;
        pos.y = m_fDistance * sinf(m_fAngle) + MINIMAP_POS_Y;

        // 位置を更新
        SetPosition(pos);
        SetVertex();

        CScene2D::Update();
    }
    else
    {
        Uninit();
    }

}

//========================================================
// 描画処理
// Author : 樋宮匠
//========================================================
void CMinimap::Draw(void)
{
    // 表示するなら
    if (m_bDisp)
    {
        CScene2D::Draw();
    }
}

//========================================================
// テクスチャ読み込み処理
// Author : 樋宮匠
//========================================================
HRESULT CMinimap::Load(void)
{
    return S_OK;
}

//========================================================
// テクスチャ破棄処理
// Author : 樋宮匠
//========================================================
void CMinimap::Unload(void)
{
}

//========================================================
// インスタンス生成処理
// Author : 樋宮匠
//========================================================
CMinimap * CMinimap::Create(TYPE type)
{
    CMinimap *pMinimap;
    pMinimap = new CMinimap;
    pMinimap->Init();
    pMinimap->m_bUse = true;
    pMinimap->m_type = type;

    // 種類ごとに、色を変える
    switch (type)
    {
    case ICON_ENEMY:
        pMinimap->SetColor(ICON_COLOR_ENEMY);
        break;
    case ICON_LIFE:
        pMinimap->SetColor(ICON_COLOR_LIFE);
        break;
    case ICON_TARGET:
        pMinimap->SetColor(ICON_COLOR_TARGET);
        break;
    default:
        break;
    }

    return pMinimap;
}

//========================================================
// プレイヤーと敵の距離計算処理
// Author : 樋宮匠
//========================================================
float CMinimap::CalculateDistance(void)
{
    float fDistance;

    // 2点間の距離を計算
    fDistance = sqrtf(fabsf(m_fieldPos.x - CGame::GetPlayer()->GetPos().x) * fabsf(m_fieldPos.x - CGame::GetPlayer()->GetPos().x) +
        fabsf(m_fieldPos.z - CGame::GetPlayer()->GetPos().z) * fabsf(m_fieldPos.z - CGame::GetPlayer()->GetPos().z));

    // 相対距離に変換
    fDistance = fDistance * MAP_RELATIVE;

    return fDistance;
}

//========================================================
// プレイヤーと敵の角度計算処理
// Author : 樋宮匠
//========================================================
float CMinimap::CalculateAngle(void)
{
    float fAngle;

    // 2点から角度を計算
    fAngle = atan2f(m_fieldPos.z - CGame::GetPlayer()->GetPos().z, m_fieldPos.x - CGame::GetPlayer()->GetPos().x);

    // 角度のずれを修正
    fAngle += CGame::GetPlayer()->GetRot().y + D3DXToRadian(180.0f);

    return -fAngle;
}
