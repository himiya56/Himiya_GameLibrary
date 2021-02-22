//===============================================
//
// ブロック処理 (block.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "debug.h"
#include "game.h"
#include "score.h"
#include "effect3d.h"
#include "library.h"
#include "modelEffect.h"
#include "bullet.h"
#include "item.h"
#include "sound.h"

//========================================
// 静的メンバ変数宣言
//========================================
LPD3DXMESH	CBlock::m_apMesh[CBlock::TYPE_MAX] = {};
LPD3DXBUFFER CBlock::m_apBuffMat[CBlock::TYPE_MAX] = {};
DWORD CBlock::m_aNumMat[CBlock::TYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CBlock::m_apTexture[CBlock::TYPE_MAX] = {};

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CBlock::CBlock() :CScene3D(CScene::OBJTYPE_BLOCK)
{
    m_collisionSize = DEFAULT_VECTOR;
    m_nType = TYPE_BUILDING001;
    m_bWallRun = false;
    m_bEnemyWallRan = false;

    m_nLife = 0;
    m_nScore = 0;
    m_nOrbs = 0;
    m_bUseDestroyEffect = false;

    m_bUseExplosion = false;

    m_lastHit = LAST_HIT_PLAYER;

    m_pMinimap = NULL;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CBlock::~CBlock()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CBlock::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene3D::Init(pos, size);

    // スケールを設定
    SetScale(size);

    // メッシュ、テクスチャをバインド
    BindMesh(m_apMesh[m_nType], m_apBuffMat[m_nType], m_aNumMat[m_nType]);
    BindTexture(m_apTexture[m_nType]);

    // クリスタルなら
    if (m_nType == TYPE_CRYSTAL)
    {
        // ミニマップ上にターゲットアイコンを生成
        m_pMinimap = CMinimap::Create(CMinimap::ICON_TARGET);
    }

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CBlock::Uninit(void)
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
void CBlock::Update(void)
{
    // ミニマップアイコンの情報更新
    if (m_pMinimap != NULL)
    {
        m_pMinimap->SetFieldPos(GetPos());
    }

    // 体力が0以下なら、スコア加算し、未使用に
    if (m_nLife <= 0)
    {
        // 倒壊エフェクト
        if (m_bUseDestroyEffect == true)
        {
            DestroyEffect();
        }

        // 爆発エフェクト
        if (m_bUseExplosion == true)
        {
            ExplosionEffect();
        }

        // 横に倒れるエフェクト
        FallDownModel();

        // ミッションクリア数に加算する
        AddMissionClear();

        // ラストヒットがプレイヤーなら
        if (m_lastHit == LAST_HIT_PLAYER)
        {
            // オーブをバラまく
            ScatterOrbs();

            // スコア加算
            CScore *pScore = CGame::GetScore();
            pScore->AddScore(m_nScore);
        }

        // クリスタルなら
        if (m_nType == TYPE_CRYSTAL)
        {
            // 回復アイテム生成
            CItem::Create(GetPos(), DEFAULT_VECTOR, CItem::TYPE_HEAL);
        }

        // 終了処理
        Uninit();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CBlock::Draw(void)
{
    CScene3D::Draw();
}

//=============================================================================
// モデルデータ読み込み処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CBlock::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //==============================
    // ビル001
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, 
        &m_apBuffMat[TYPE_BUILDING001], NULL, &m_aNumMat[TYPE_BUILDING001], &m_apMesh[TYPE_BUILDING001]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building001.jpg", &m_apTexture[TYPE_BUILDING001]);

    //==============================
    // ビル101
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building101.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BUILDING101], NULL, &m_aNumMat[TYPE_BUILDING101], &m_apMesh[TYPE_BUILDING101]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building101.jpg", &m_apTexture[TYPE_BUILDING101]);

    //==============================
    // ビル201
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building201.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BUILDING201], NULL, &m_aNumMat[TYPE_BUILDING201], &m_apMesh[TYPE_BUILDING201]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building201.jpg", &m_apTexture[TYPE_BUILDING201]);

    //==============================
    // ビル301
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building301.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BUILDING301], NULL, &m_aNumMat[TYPE_BUILDING301], &m_apMesh[TYPE_BUILDING301]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building301.jpg", &m_apTexture[TYPE_BUILDING301]);

    //==============================
    // ビル401
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building401.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BUILDING401], NULL, &m_aNumMat[TYPE_BUILDING401], &m_apMesh[TYPE_BUILDING401]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building401.jpg", &m_apTexture[TYPE_BUILDING401]);

    //==============================
    // ビル501
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building501.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BUILDING501], NULL, &m_aNumMat[TYPE_BUILDING501], &m_apMesh[TYPE_BUILDING501]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building501.jpg", &m_apTexture[TYPE_BUILDING501]);

    //==============================
    // 信号機
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Signal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGNAL], NULL, &m_aNumMat[TYPE_SIGNAL], &m_apMesh[TYPE_SIGNAL]);

    //==============================
    // 街灯
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/light.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LIGHT], NULL, &m_aNumMat[TYPE_LIGHT], &m_apMesh[TYPE_LIGHT]);

    //==============================
    // 街灯2
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/light2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LIGHT2], NULL, &m_aNumMat[TYPE_LIGHT2], &m_apMesh[TYPE_LIGHT2]);

    //==============================
    // ゴミ箱
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/DustBox.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DUSTBOX], NULL, &m_aNumMat[TYPE_DUSTBOX], &m_apMesh[TYPE_DUSTBOX]);

    //==============================
    // 木1
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood1.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD1], NULL, &m_aNumMat[TYPE_WOOD1], &m_apMesh[TYPE_WOOD1]);

    //==============================
    // 木2
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD2], NULL, &m_aNumMat[TYPE_WOOD2], &m_apMesh[TYPE_WOOD2]);

    //==============================
    // 木3
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood3.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD3], NULL, &m_aNumMat[TYPE_WOOD3], &m_apMesh[TYPE_WOOD3]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ki.jpg", &m_apTexture[TYPE_WOOD3]);

    //==============================
    // ベンチ
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/bench.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BENCH], NULL, &m_aNumMat[TYPE_BENCH], &m_apMesh[TYPE_BENCH]);

    //==============================
    // 机
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/tukue.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_TUKUE], NULL, &m_aNumMat[TYPE_TUKUE], &m_apMesh[TYPE_TUKUE]);

    //==============================
    // ガードレール
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/guardrail.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_GUARDRAIL], NULL, &m_aNumMat[TYPE_GUARDRAIL], &m_apMesh[TYPE_GUARDRAIL]);

    //==============================
    // ジャングルジム
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/playground.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_PLAYGROUND], NULL, &m_aNumMat[TYPE_PLAYGROUND], &m_apMesh[TYPE_PLAYGROUND]);

    //==============================
    // 注意サイン
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signEmar.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_EMAR], NULL, &m_aNumMat[TYPE_SIGN_EMAR], &m_apMesh[TYPE_SIGN_EMAR]);

    //==============================
    // 十字路サイン
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signInter.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_INTER], NULL, &m_aNumMat[TYPE_SIGN_INTER], &m_apMesh[TYPE_SIGN_INTER]);

    //==============================
    // 駐車禁止サイン
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signNoPark.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_NO_PARK], NULL, &m_aNumMat[TYPE_SIGN_NO_PARK], &m_apMesh[TYPE_SIGN_NO_PARK]);

    //==============================
    // 車
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/car.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CAR], NULL, &m_aNumMat[TYPE_CAR], &m_apMesh[TYPE_CAR]);

    //==============================
    // コンビニ
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/convini.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CONVINI], NULL, &m_aNumMat[TYPE_CONVINI], &m_apMesh[TYPE_CONVINI]);

    //==============================
    // コンテナ
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/container.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CONTAINER], NULL, &m_aNumMat[TYPE_CONTAINER], &m_apMesh[TYPE_CONTAINER]);

    //==============================
    // ガスタンク
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/gas.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_GAS], NULL, &m_aNumMat[TYPE_GAS], &m_apMesh[TYPE_GAS]);

    //==============================
    // 倉庫
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/souko.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SOUKO], NULL, &m_aNumMat[TYPE_SOUKO], &m_apMesh[TYPE_SOUKO]);

    //==============================
    // 鉄塔
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/metal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_METAL], NULL, &m_aNumMat[TYPE_METAL], &m_apMesh[TYPE_METAL]);

    //==============================
    // ソーラーパネル
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/solar.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SOLAR], NULL, &m_aNumMat[TYPE_SOLAR], &m_apMesh[TYPE_SOLAR]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/SolarPanel.jpg", &m_apTexture[TYPE_SOLAR]);

    //==============================
    // タワー
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/tower.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_TOWER], NULL, &m_aNumMat[TYPE_TOWER], &m_apMesh[TYPE_TOWER]);

    //==============================
    // クリスタル
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/crystal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CRYSTAL], NULL, &m_aNumMat[TYPE_CRYSTAL], &m_apMesh[TYPE_CRYSTAL]);

    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 後藤慎之助
//=============================================================================
void CBlock::Unload(void)
{
    for (int nCnt = 0; nCnt < CBlock::TYPE_MAX; nCnt++)
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
CBlock * CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
    // ポインタを用意
    CBlock *pBlock = NULL;

    // メモリ確保
    pBlock = new CBlock;

    // タイプを紐づけ(初期化の中でBindするものがあるため、最初に紐づけ)
    pBlock->m_nType = nType;

    // 初期化
    pBlock->Init(pos, DEFAULT_SCALE);

    // 回転を結びつける
    pBlock->SetRot(rot);

    // 種類ごとに、結びつける値を変える
    switch (nType)
    {
    case TYPE_BUILDING001:
        
        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;
        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_BUILDING101:

        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;
        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_BUILDING201:

        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;
        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_BUILDING301:

        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;
        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_BUILDING401:

        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;
        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_BUILDING501:

        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;

        break;

    case TYPE_SIGNAL:

        pBlock->m_collisionSize = BLOCK_SIGNAL_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_SIGNAL_LIFE;
        pBlock->m_nScore = BLOCK_SIGNAL_SCORE;
        pBlock->m_nOrbs = BLOCK_SIGNAL_ORBS;

        break;

    case TYPE_LIGHT:

        pBlock->m_collisionSize = BLOCK_LIGHT_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_LIGHT_LIFE;
        pBlock->m_nScore = BLOCK_LIGHT_SCORE;
        pBlock->m_nOrbs = BLOCK_LIGHT_ORBS;

        break;

    case TYPE_LIGHT2:

        pBlock->m_collisionSize = BLOCK_LIGHT_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_LIGHT_LIFE;
        pBlock->m_nScore = BLOCK_LIGHT_SCORE;
        pBlock->m_nOrbs = BLOCK_LIGHT_ORBS;

        break;

    case TYPE_DUSTBOX:

        pBlock->m_collisionSize = BLOCK_DUSTBOX_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_DUSTBOX_LIFE;
        pBlock->m_nScore = BLOCK_DUSTBOX_SCORE;
        pBlock->m_nOrbs = BLOCK_DUSTBOX_ORBS;

        break;

    case TYPE_WOOD1:

        pBlock->m_collisionSize = BLOCK_WOOD_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_WOOD_LIFE;
        pBlock->m_nScore = BLOCK_WOOD_SCORE;
        pBlock->m_nOrbs = BLOCK_WOOD_ORBS;

        break;

    case TYPE_WOOD2:

        pBlock->m_collisionSize = BLOCK_WOOD2_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_WOOD_LIFE;
        pBlock->m_nScore = BLOCK_WOOD_SCORE;
        pBlock->m_nOrbs = BLOCK_WOOD_ORBS;

        break;

    case TYPE_WOOD3:

        pBlock->m_collisionSize = BLOCK_WOOD_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_WOOD_LIFE;
        pBlock->m_nScore = BLOCK_WOOD_SCORE;
        pBlock->m_nOrbs = BLOCK_WOOD_ORBS;

        break;

    case TYPE_BENCH:

        // 縦向きか横向きかで、当たり判定を変える
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_BENCH_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_BENCH_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_BENCH_LIFE;
        pBlock->m_nScore = BLOCK_BENCH_SCORE;
        pBlock->m_nOrbs = BLOCK_BENCH_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_TUKUE:

        // 縦向きか横向きかで、当たり判定を変える
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_TUKUE_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_TUKUE_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_TUKUE_LIFE;
        pBlock->m_nScore = BLOCK_TUKUE_SCORE;
        pBlock->m_nOrbs = BLOCK_TUKUE_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_GUARDRAIL:

        // 縦向きか横向きかで、当たり判定を変える
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_GUARDRAIL_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_GUARDRAIL_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_GUARDRAIL_LIFE;
        pBlock->m_nScore = BLOCK_GUARDRAIL_SCORE;
        pBlock->m_nOrbs = BLOCK_GUARDRAIL_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_PLAYGROUND:

        pBlock->m_collisionSize = BLOCK_PLAYGROUND_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;
        pBlock->m_nLife = BLOCK_PLAYGROUND_LIFE;
        pBlock->m_nScore = BLOCK_PLAYGROUND_SCORE;
        pBlock->m_nOrbs = BLOCK_PLAYGROUND_ORBS;

        break;

    case TYPE_SIGN_EMAR:

        pBlock->m_collisionSize = BLOCK_SIGN_EMAR_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_SIGN_EMAR_LIFE;
        pBlock->m_nScore = BLOCK_SIGN_EMAR_SCORE;
        pBlock->m_nOrbs = BLOCK_SIGN_EMAR_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_SIGN_INTER:

        pBlock->m_collisionSize = BLOCK_SIGN_INTER_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_SIGN_INTER_LIFE;
        pBlock->m_nScore = BLOCK_SIGN_INTER_SCORE;
        pBlock->m_nOrbs = BLOCK_SIGN_INTER_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_SIGN_NO_PARK:

        pBlock->m_collisionSize = BLOCK_SIGN_NO_PARK_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_SIGN_NO_PARK_LIFE;
        pBlock->m_nScore = BLOCK_SIGN_NO_PARK_SCORE;
        pBlock->m_nOrbs = BLOCK_SIGN_NO_PARK_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_CAR:

        // 縦向きか横向きかで、当たり判定を変える
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CAR_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CAR_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_CAR_LIFE;
        pBlock->m_nScore = BLOCK_CAR_SCORE;
        pBlock->m_nOrbs = BLOCK_CAR_ORBS;

        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;

        pBlock->m_bUseExplosion = true;

        break;

    case TYPE_CONVINI:

        // 縦向きか横向きかで、当たり判定を変える
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CONVINI_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CONVINI_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_CONVINI_LIFE;
        pBlock->m_nScore = BLOCK_CONVINI_SCORE;
        pBlock->m_nOrbs = BLOCK_CONVINI_ORBS;

        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;

        pBlock->m_bUseDestroyEffect = true;

        break;

    case TYPE_CONTAINER:

        // 縦向きか横向きかで、当たり判定を変える
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CONTAINER_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CONTAINER_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_CONTAINER_LIFE;
        pBlock->m_nScore = BLOCK_CONTAINER_SCORE;
        pBlock->m_nOrbs = BLOCK_CONTAINER_ORBS;

        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;

        pBlock->m_bUseDestroyEffect = true;

        break;

    case TYPE_GAS:

        pBlock->m_collisionSize = BLOCK_GAS_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;
        pBlock->m_nLife = BLOCK_GAS_LIFE;
        pBlock->m_nScore = BLOCK_GAS_SCORE;
        pBlock->m_nOrbs = BLOCK_GAS_ORBS;
        pBlock->m_bUseDestroyEffect = true;

        pBlock->m_bUseExplosion = true;

        break;

    case TYPE_SOUKO:

        // 縦向きか横向きかで、当たり判定を変える
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_SOUKO_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_SOUKO_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_SOUKO_LIFE;
        pBlock->m_nScore = BLOCK_SOUKO_SCORE;
        pBlock->m_nOrbs = BLOCK_SOUKO_ORBS;

        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;

        pBlock->m_bUseDestroyEffect = true;

        break;

    case TYPE_METAL:

        pBlock->m_collisionSize = BLOCK_METAL_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_METAL_LIFE;
        pBlock->m_nScore = BLOCK_METAL_SCORE;
        pBlock->m_nOrbs = BLOCK_METAL_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_SOLAR:

        // 縦向きか横向きかで、当たり判定を変える
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_SOLAR_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_SOLAR_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_SOLAR_LIFE;
        pBlock->m_nScore = BLOCK_SOLAR_SCORE;
        pBlock->m_nOrbs = BLOCK_SOLAR_ORBS;

        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;

        pBlock->m_bUseDestroyEffect = true;

        break;

    case TYPE_TOWER:

        pBlock->m_collisionSize = BLOCK_TOWER_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_TOWER_LIFE;
        pBlock->m_nScore = BLOCK_TOWER_SCORE;
        pBlock->m_nOrbs = BLOCK_TOWER_ORBS;

        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_CRYSTAL:

        pBlock->m_collisionSize = BLOCK_CRYSTAL_COLLISION_SIZE;
        pBlock->m_bEnemyWallRan = true;
        pBlock->m_nLife = BLOCK_CRYSTAL_LIFE;
        pBlock->m_nScore = BLOCK_CRYSTAL_SCORE;
        pBlock->m_nOrbs = BLOCK_CRYSTAL_ORBS;

        break;
    }

#ifdef COLLISION_TEST
    CDebug::Create(pos, pBlock->m_collisionSize, CDebug::TYPE_PERMANENT);
#endif #endif // COLLISION_TEST

    return pBlock;
}

//=============================================================================
// オーブをバラまく処理
// Author : 後藤慎之助
//=============================================================================
void CBlock::ScatterOrbs(void)
{
    // 位置を取得
    D3DXVECTOR3 pos = GetPos();

    // オーブ飛散エフェクト
    float fSpeedX = 0.0f;   //速さX
    float fSpeedY = 0.0f;   //速さY
    float fSpeedZ = 0.0f;   //速さZ
    int nPlusMinusX = 0;    //符号X
    int nPlusMinusZ = 0;    //符号Z
    for (int nCnt = 0; nCnt < m_nOrbs; nCnt++)
    {
        // 速さと、符号をランダムに決める
        fSpeedX = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_X, 0)) / 100.0f;
        fSpeedY = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Y, EFFECT_CHARGE_SPREAD_MIN_Y)) / 100.0f;
        fSpeedZ = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Z, 0)) / 100.0f;
        nPlusMinusX = GetRandNum(2, 1);
        nPlusMinusZ = GetRandNum(2, 1);
        if (nPlusMinusX == 1)
        {
            nPlusMinusX = 1;
        }
        else
        {
            nPlusMinusX = -1;
        }

        if (nPlusMinusZ == 1)
        {
            nPlusMinusZ = 1;
        }
        else
        {
            nPlusMinusZ = -1;
        }

        // 移動量を決める
        D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY, fSpeedZ*nPlusMinusZ);

        // エフェクト発生(Y軸を地面で固定)
        CEffect3D::Create(D3DXVECTOR3(pos.x, 0.0f, pos.z), EFFECT_CHARGE_SIZE, move,
            EFFECT3D_PURPLE_FIRE_FIRST_COLOR, DEFAULT_VECTOR, EFFECT3D_PURPLE_FIRE_FADE_OUT_RATE, CEffect3D::TYPE_CHARGE);
    }
}

//=============================================================================
// 倒壊エフェクト
// Author : 後藤慎之助
//=============================================================================
void CBlock::DestroyEffect(void)
{
    // 倒壊エフェクト用の変数宣言
    float fAngle = 0.0f;                       // 角度
    float fRadius = 0.0f;                      // 半径
    float fSpeed = 0.0f;                       // 速さ
    float fSize = 0.0f;                        // 大きさ
    for (int nCnt = 0; nCnt < EFFECT_DESTROY_NUMBER_ONCE; nCnt++)
    {
        // 0～3.14を出す*(0 or 1)*-1で正負を決める
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // 半径を出す
        int nAverageSize = ((int)m_collisionSize.x + (int)m_collisionSize.z) / 2;
        fRadius = float(GetRandNum(nAverageSize * MODEL_EFFECT_BIG_COLLAPSE_RADIUS, 0)) / 100.0f;

        // 速さを出す
        fSpeed = float(GetRandNum(EFFECT_DESTROY_MAX_SPEED, EFFECT_DESTROY_MIN_SPEED)) / 100.0f;

        // 移動量を決める
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeed, 0.0f);

        // 大きさを出す
        fSize = float(GetRandNum(EFFECT_DESTROY_MAX_SIZE, EFFECT_DESTROY_MIN_SIZE)) / 100.0f;

        // 大きさを決める
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // 位置を決める(Y軸を地面から半分浮かせた位置で固定)
        D3DXVECTOR3 pos = D3DXVECTOR3(-sinf(fAngle)*fRadius + GetPos().x, fSize / 2, -cosf(fAngle)*fRadius + GetPos().z);

        // エフェクト発生
        CEffect3D::Create(pos, size, move, DEFAULT_COLOR,
            DEFAULT_VECTOR, EFFECT_DESTROY_FADE_OUT_RATE, CEffect3D::TYPE_SMOKE);
    }

    // 崩壊エフェクト
    CollapseEffect();
}

//=============================================================================
// 爆発エフェクト
// Author : 後藤慎之助
//=============================================================================
void CBlock::ExplosionEffect(void)
{
    // 位置を取得
    D3DXVECTOR3 pos = GetPos();

    // 小さいガレキを飛ばす
    for (int nCnt = 0; nCnt < MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV2; nCnt++)
    {
        // 変数宣言
        float fSpeedY = float(GetRandNum(MODEL_EFFECT_EXPLOSION_SPEED_Y_MAX, MODEL_EFFECT_EXPLOSION_SPEED_Y_MIN)) / 100.0f;
        float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

        // 移動量を決める
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f,
            fSpeedY, 0.0f);

        // 回転量を決める
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        // 0～3.14を出す*(0 or 1)*-1で正負を決める
        float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // XとZの中間の大きさを得る
        float fAverageSize = (m_collisionSize.x + m_collisionSize.z) / 2;

        // 位置を決める
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*fAverageSize,
            0.0f, -cosf(fAngle)*fAverageSize) + pos;

        // 種類を決める
        int nRandNum = GetRandNum(2, 0);
        switch (nRandNum)
        {
        case 0:
            modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;
            break;
        case 1:
            modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_002;
            break;
        case 2:
            modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_003;
            break;
        }

        // エフェクト発生
        CModelEffect::Create(effectPos, rotMove, move, rotMove, modelEffectType);
    }

    // 爆発発生
    D3DXVECTOR3 size = D3DXVECTOR3(BULLET_EXPLOSION_COLLISION_SIZE_LV2, BULLET_EXPLOSION_COLLISION_SIZE_LV2, BULLET_EXPLOSION_COLLISION_SIZE_LV2);

    // ラストヒットがプレイヤーなら
    if (m_lastHit == LAST_HIT_PLAYER)
    {
        // 種族をプレイヤーに
        CBullet::Create(pos, size, DEFAULT_VECTOR,
            CBullet::TYPE_EXPLOSION, CBullet::TRIBE_PLAYER);
    }
    // それ以外は種族を敵に
    else
    {
        CBullet::Create(pos, size, DEFAULT_VECTOR,
            CBullet::TYPE_EXPLOSION, CBullet::TRIBE_ENEMY);
    }
}

//=============================================================================
// 崩壊エフェクト
// Author : 後藤慎之助
//=============================================================================
void CBlock::CollapseEffect(void)
{
    // ガレキが崩れる音再生
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_DESTROY_RUBBLE);

    for (int nCnt = 0; nCnt < MODEL_EFFECT_COLLAPSE_NUMBER_ONCE; nCnt++)
    {
        // 崩壊エフェクト用の変数宣言
        float fPosX = 0.0f;                 // 位置X
        float fPosZ = 0.0f;                 // 位置Z
        float fRotSpeedX = 0.0f;            // 回転速度X
        float fRotSpeedY = 0.0f;            // 回転速度Y
        float fRotSpeedZ = 0.0f;            // 回転速度Z
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_001;

        // 位置をずらす
        fPosX = float(GetRandNum((int)m_collisionSize.x, 0)) - float(GetRandNum((int)m_collisionSize.x, 0));
        fPosZ = float(GetRandNum((int)m_collisionSize.z, 0)) - float(GetRandNum((int)m_collisionSize.z, 0));

        // 回転の速さを決める
        fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;

        // 回転量を決める
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        // 種類を決める
        int nRandNum = GetRandNum(2, 0);
        switch (nRandNum)
        {
        case 0:
            modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_001;
            break;
        case 1:
            modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_002;
            break;
        case 2:
            modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_003;
            break;
        }

        // エフェクト発生(Y軸は上の面で固定)
        CModelEffect::Create(GetPos() + D3DXVECTOR3(fPosX, m_collisionSize.y, fPosZ), rotMove, DEFAULT_VECTOR, rotMove, modelEffectType);
    }
}

//=============================================================================
// 横に倒れていくエフェクト
// Author : 後藤慎之助
//=============================================================================
void CBlock::FallDownModel(void)
{
    // 音を取得
    CSound*pSound = CManager::GetSound();

    // 位置と回転を取得
    D3DXVECTOR3 pos = GetPos();
    D3DXVECTOR3 rot = GetRot();

    // 倒れる方向をランダムに決める
    int nRandNum = GetRandNum(4, 1);
    D3DXVECTOR3 rotMove = DEFAULT_VECTOR;
    switch (nRandNum)
    {
    case 1:
        rotMove = D3DXVECTOR3(MODEL_EFFECT_FALL_DOWN_FIRST_SPEED, 0.0f, 0.0f);
        break;
    case 2:
        rotMove = D3DXVECTOR3(-MODEL_EFFECT_FALL_DOWN_FIRST_SPEED, 0.0f, 0.0f);
        break;
    case 3:
        rotMove = D3DXVECTOR3(0.0f, 0.0f, MODEL_EFFECT_FALL_DOWN_FIRST_SPEED);
        break;
    case 4:
        rotMove = D3DXVECTOR3(0.0f, 0.0f, -MODEL_EFFECT_FALL_DOWN_FIRST_SPEED);
        break;
    }

    // 種類ごとにエフェクト発生
    switch (m_nType)
    {
    case TYPE_SIGNAL:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_SIGNAL);
        break;

    case TYPE_LIGHT:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_LIGHT);
        break;

    case TYPE_LIGHT2:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_LIGHT2);
        break;

    case TYPE_WOOD1:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_WOOD1);
        break;

    case TYPE_WOOD2:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_WOOD2);
        break;

    case TYPE_WOOD3:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_WOOD3);
        break;

    case TYPE_GUARDRAIL:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, BLOCK_GUARDRAIL_ROT_MOVE, CModelEffect::TYPE_GUARDRAIL);
        break;

    case TYPE_SIGN_EMAR:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_SIGN_EMAR);
        break;

    case TYPE_SIGN_INTER:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_SIGN_INTER);
        break;

    case TYPE_SIGN_NO_PARK:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_SIGN_NO_PARK);
        break;

    case TYPE_METAL:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_METAL);
        break;

    case TYPE_CRYSTAL:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, DEFAULT_VECTOR, CModelEffect::TYPE_MINI_CRYSTAL);
        CModelEffect::Create(pos, rot, BLOCK_CRYSTAL_SHOOT_UP_MOVE, BLOCK_CRYSTAL_SHOOT_UP_ROT_MOVE, CModelEffect::TYPE_LONG_CRYSTAL);
        pSound->Play(CSound::LABEL_SE_DESTROY_CRYSTAL);
        break;
    }
}

//=============================================================================
// ミッションクリア数に加算する
// Author : 後藤慎之助
//=============================================================================
void CBlock::AddMissionClear(void)
{
    // ステージと現在のミッションに応じて、クリア数を増加
    CGame::STAGE stage = CGame::GetStage();
    CGame::CURRENT_MISSION currentMission = CGame::GetCurrentMission();
    switch (stage)
    {
    case CGame::STAGE_1:

        switch (currentMission)
        {
        case CGame::CURRENT_MISSION_1:
            CGame::SetClearNumMission1();
            break;

        case CGame::CURRENT_MISSION_2:

            break;
        }

        break;

    case CGame::STAGE_2:

        break;

    case CGame::STAGE_FINAL:

        switch (currentMission)
        {
        case CGame::CURRENT_MISSION_1:
            // クリスタルなら
            if (m_nType == TYPE_CRYSTAL)
            {
                CGame::SetClearNumMission1();
            }
            break;

        case CGame::CURRENT_MISSION_2:

            break;
        }

        break;
    }
}
