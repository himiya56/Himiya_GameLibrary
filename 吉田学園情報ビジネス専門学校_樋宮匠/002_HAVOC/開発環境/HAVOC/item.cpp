//===============================================
//
// アイテム処理 (item.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "debug.h"
#include "game.h"
#include "score.h"
#include "effect3d.h"
#include "library.h"
#include "modelEffect.h"
#include "block.h"
#include "player.h"
#include "sound.h"

//========================================
// 静的メンバ変数宣言
//========================================
LPD3DXMESH	CItem::m_apMesh[CItem::TYPE_MAX] = {};
LPD3DXBUFFER CItem::m_apBuffMat[CItem::TYPE_MAX] = {};
DWORD CItem::m_aNumMat[CItem::TYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CItem::m_apTexture[CItem::TYPE_MAX] = {};

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CItem::CItem() :CScene3D(CScene::OBJTYPE_ITEM)
{
    m_type = TYPE_HEAL;
    m_collisionSize = DEFAULT_VECTOR;
    m_posOld = DEFAULT_VECTOR;
    m_move = DEFAULT_VECTOR;

    m_nLife = 0;
    m_bUse = true;
    m_bBlinking = false;

    m_pMinimap = NULL;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CItem::~CItem()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene3D::Init(pos, size);

    // スケールを設定
    SetScale(size);

    // メッシュ、テクスチャをバインド
    BindMesh(m_apMesh[m_type], m_apBuffMat[m_type], m_aNumMat[m_type]);
    BindTexture(m_apTexture[m_type]);

    // ミニマップ上にアイコンを生成
    m_pMinimap = CMinimap::Create(CMinimap::ICON_LIFE);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CItem::Uninit(void)
{
    // ミニマップアイコンを使用しない
    if (m_pMinimap != NULL)
    {
        m_pMinimap->UnableIcon();
    }

    CScene3D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CItem::Update(void)
{
    // 位置を取得
    D3DXVECTOR3 pos = GetPos();

    // 1F前の位置を結びつける
    m_posOld = pos;

    // 移動量と位置を結びつける
    pos += m_move;

    // 地面より上なら
    if (pos.y > 0.0f)
    {
        // 重力
        m_move.y -= GRAVITY;
    }
    else
    {
        // 地面にいる
        pos.y = 0.0f;
        m_move.y = 0.0f;
    }

    // ブロックへの当たり判定
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BLOCK);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BLOCK); nCntScene++)
    {
        // 中身があるなら
        if (pScene != NULL)
        {
            // 次のシーンを記憶
            CScene*pNextScene = pScene->GetNextScene();

            // ブロックにキャスト
            CBlock *pBlock = (CBlock*)pScene;

            // ブロックの位置と大きさを取得
            D3DXVECTOR3 blockPos = pBlock->GetPos();
            D3DXVECTOR3 blockSize = pBlock->GetCollisionSize();

            // ブロックとの当たり判定を計算
            BLOCK_HIT_INFO blockHitInfo;
            blockHitInfo.hitSurface = HIT_SURFACE_NONE;
            blockHitInfo.pos = pos;
            blockHitInfo = (BlockCollision3D(&pos, &m_posOld, &blockPos,
                &m_collisionSize, &blockSize));

            // ブロックの当たり判定で計算した位置を、結びつける
            pos = blockHitInfo.pos;

            // 上の面に当たっているなら、縦の移動量を0にする
            if (blockHitInfo.hitSurface == HIT_SURFACE_TOP)
            {
                m_move.y = 0.0f;
            }

            // 次のシーンにする
            pScene = pNextScene;
        }
        else
        {
            // 中身がないなら、そこで処理を終える
            break;
        }
    }

    // プレイヤーとの当たり判定
    CPlayer*pPlayer = NULL;
    pPlayer = CGame::GetPlayer();

    // 自機の位置を取得
    D3DXVECTOR3 playerPos = pPlayer->GetPos();

    // プレイヤーが表示されているなら
    if (pPlayer->GetDisp() == true)
    {
        // 当たっているなら
        if (RectangleCollision3D(&pos, &playerPos,
            &m_collisionSize, &PLAYER_COLLISION_SIZE) == true)
        {
            // SEを再生
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_HEAL);

            // 自機が回復
            CGame::GetPlayer()->Healing(ITEM_HEAL_VALUE);

            // このアイテムを未使用に
            m_bUse = false;
        }
    }

    // 位置を設定
    SetPos(pos);

    // 種類ごとのエフェクト
    switch (m_type)
    {
    case TYPE_HEAL:
        HealEffect();
        break;
    }

    // ミニマップアイコンの情報更新
    if (m_pMinimap != NULL)
    {
        m_pMinimap->SetFieldPos(pos);
    }

#ifdef COLLISION_TEST
    CDebug::Create(GetPos(), m_collisionSize, CDebug::TYPE_MOMENT);
#endif #endif // COLLISION_TEST

    // 設置されてからの時間をカウントダウン
    m_nLife--;

    // 体力が0以下なら、使用フラグをfalseに
    if (m_nLife <= 0)
    {
        m_bUse = false;
    }

    // 使用フラグがfalseなら、消去
    if (m_bUse == false)
    {
        Uninit();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CItem::Draw(void)
{
    // 点滅していないなら
    if (m_bBlinking == false)
    {
        CScene3D::Draw();
    }
}

//=============================================================================
// モデルデータ読み込み処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CItem::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //==============================
    // ヒール
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/heal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_HEAL], NULL, &m_aNumMat[TYPE_HEAL], &m_apMesh[TYPE_HEAL]);

    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 後藤慎之助
//=============================================================================
void CItem::Unload(void)
{
    for (int nCnt = 0; nCnt < CItem::TYPE_MAX; nCnt++)
    {
        if (m_apMesh[nCnt] != NULL)
        {
            m_apMesh[nCnt]->Release();
            m_apMesh[nCnt] = NULL;
        }
        if (m_apBuffMat[nCnt] != NULL)
        {
            m_apBuffMat[nCnt]->Release();
            m_apBuffMat[nCnt] = NULL;
        }
        if (m_apTexture[nCnt] != NULL)
        {
            m_apTexture[nCnt]->Release();
            m_apTexture[nCnt] = NULL;
        }
    }
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CItem * CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
    // ポインタを用意
    CItem *pItem = NULL;

    // メモリ確保
    pItem = new CItem;

    // タイプを紐づけ(初期化の中でBindするものがあるため、最初に紐づけ)
    pItem->m_type = type;

    // 初期化
    pItem->Init(pos, DEFAULT_SCALE);

    // 回転を結びつける
    pItem->SetRot(rot);

    // 1F前の位置を設定
    pItem->m_posOld = pos;

    // 種類ごとに、結びつける値を変える
    switch (type)
    {
    case TYPE_HEAL:

        pItem->m_collisionSize = ITEM_HEAL_COLLISION_SIZE;
        pItem->m_nLife = ITEM_HEAL_LIFE;
        pItem->m_move.y = ITEM_HEAL_APPEAR_LAUNCH_VALUE;

        break;
    }

    return pItem;
}

//=============================================================================
// 回復アイテムのエフェクト
// Author : 後藤慎之助
//=============================================================================
void CItem::HealEffect(void)
{
    // 位置と向きを取得
    D3DXVECTOR3 pos = GetPos();
    D3DXVECTOR3 rot = GetRot();

    // 常に回転
    rot.y += ITEM_HEAL_ROT_SPEED;

    // 向きを反映
    SetRot(rot);

    // 一定間隔でエフェクト発生
    if (m_nLife % ITEM_HEAL_SET_EFFECT_INTERVAL == 0)
    {
        // エフェクト発生用の変数宣言
        // 速さを出す
        float fSpeed = float(GetRandNum(EFFECT_HEAL_ITEM_MAX_SPEED, EFFECT_HEAL_ITEM_MIN_SPEED)) / 100.0f;

        // 移動量を決める
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeed, 0.0f);

        // 大きさを出す
        float fSize = float(GetRandNum(EFFECT_HEAL_ITEM_MAX_SIZE, EFFECT_HEAL_ITEM_MIN_SIZE)) / 100.0f;

        // 大きさを決める
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // 角度を出す
        float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // 半径を出す
        float fRadius = float(GetRandNum(EFFECT_HEAL_ITEM_MAX_RADIUS, 0)) / 100.0f;

        // 位置を決める
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*fRadius, fSize / 2, -cosf(fAngle)*fRadius) + pos;

        // エフェクト発生
        CEffect3D::Create(effectPos, size, move, EFFECT_HEAL_ITEM_COLOR,
            DEFAULT_VECTOR, EFFECT_HEAL_ITEM_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
    }

    // 一定時間で、点滅させる
    if (m_nLife <= ITEM_HEAL_START_BLINKING)
    {
        m_bBlinking = !m_bBlinking;
    }
}
