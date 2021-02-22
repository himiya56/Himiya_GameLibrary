//================================================================================
//
// 防衛軍処理 (force.cpp)
// Author : 後藤慎之助(行動AI、エフェクト) / 樋宮匠(階層構造、アニメーション制御)
//
//================================================================================

//========================
// インクルードファイル
//========================
#include "force.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "animation.h"
#include "block.h"
#include "effect3d.h"
#include "bullet.h"
#include "library.h"
#include "debug.h"
#include "score.h"
#include "game.h"
#include "modelEffect.h"
#include "player.h"
#include "bullet.h"
#include "item.h"
#include "sound.h"

//========================================
// 静的メンバ変数宣言
//========================================
LPD3DXMESH CForce::m_pMesh[PARTS_MAX] = {};
LPD3DXBUFFER CForce::m_pBuffMat[PARTS_MAX] = {};
DWORD CForce::m_nNumMat[PARTS_MAX] = {};

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CForce::CForce() :CCharacter(OBJTYPE::OBJTYPE_ENEMY)
{
    m_move = DEFAULT_VECTOR;
    m_nScore = 0;
    m_nOrbs = 0;

    m_nCntTime = 0;

    m_fSpeed = 0.0f;
    m_rotDest = DEFAULT_VECTOR;
    m_rotMin = DEFAULT_VECTOR;

    m_fDiscoveryDistance = 0.0f;

    m_attackAI = ATTACK_AI_WAIT;

    m_targetPos = DEFAULT_VECTOR;
    m_bUseRotControl = true;

    m_nDeathCounter = 0;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CForce::~CForce()
{
}

//=============================================================================
// 初期化処理
// Author : 樋宮匠
//=============================================================================
HRESULT CForce::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // パーツ数を設定
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_FORCE, nCount, m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
    }

    // 座標・親子関係の読み込み
    LoadModelData("./data/ANIMATION/motion_ranger.txt");

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CForce::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CForce::Update(void)
{
    // 体力があるなら
    if (GetLife() > 0)
    {
        // 行動
        Movement();
    }

    // アニメーションの更新
    CCharacter::Update();

    // 体力が0以下なら、スコア加算し、未使用に
    if (GetLife() <= 0)
    {
        // モーションをデスにする
        GetAnimation()->SetAnimation(ANIM_DEATH);

        // デスカウンターを加算
        m_nDeathCounter++;

        // 撃破音を鳴らすフレーム数
        if (m_nDeathCounter == FORCE_DEATH_PLAY_SOUND)
        {
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_FORCE);
        }

        // 一定カウンタで、削除
        if (m_nDeathCounter >= FORCE_DEATH_COUNTER)
        {
            // 位置と向きを取得
            D3DXVECTOR3 pos = DEFAULT_VECTOR;
            D3DXVECTOR3 rot = DEFAULT_VECTOR;
            pos = GetPos();
            rot = GetRot();

            // ミッションクリア数に加算する
            AddMissionClear();

            // ラストヒットがプレイヤーなら
            if (GetLastHit() == LAST_HIT_PLAYER)
            {
                // オーブをバラまく
                ScatterOrbs();

                // スコア加算
                CScore *pScore = CGame::GetScore();
                pScore->AddScore(m_nScore);

                // 一定確率で、回復アイテムを落とす
                int nRandNum = GetRandNum(100, 1);
                if (nRandNum <= FORCE_DROP_HEAL_ITEM)
                {
                    CItem::Create(pos, DEFAULT_VECTOR, CItem::TYPE_HEAL);
                }
            }

            // 終了処理
            Uninit();
        }
    }
}

//=============================================================================
// 行動処理
// Author : 後藤慎之助
//=============================================================================
void CForce::Movement(void)
{
    // 位置と向きを取得
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    D3DXVECTOR3 rot = DEFAULT_VECTOR;
    pos = GetPos();
    rot = GetRot();

    // モーションをまずは待機にする
    GetAnimation()->SetAnimation(ANIM_IDLE);

    // 目標への位置を決める
    DicisionTargetPos(pos);

    // 回転制御するなら
    if (m_bUseRotControl == true)
    {
        // 回転制御
        rot = RotControl(rot);
    }

    // 1F前の位置を記憶
    SetPosOld(pos);

    // 攻撃AI処理
    if (CGame::GetState() == CGame::STATE_NORMAL)
    {
        pos = AttackAI(pos, rot);
    }

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
        // 地面にいるなら、重力をかけない
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

            // 敵が壁走りできるブロックなら
            if (pBlock->GetEnemyWallRun() == true)
            {
                // ブロックの位置と大きさを取得
                D3DXVECTOR3 blockPos = pBlock->GetPos();
                D3DXVECTOR3 blockSize = pBlock->GetCollisionSize();

                // ブロックとの当たり判定を計算
                BLOCK_HIT_INFO blockHitInfo;
                blockHitInfo.hitSurface = HIT_SURFACE_NONE;
                blockHitInfo.pos = pos;
                blockHitInfo = (BlockCollision3D(&pos, &GetPosOld(), &blockPos,
                    &GetActiveCollisionSize(), &blockSize));

                // ブロックの当たり判定で計算した位置を、結びつける
                pos = blockHitInfo.pos;
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

    // 位置と向きを反映
    SetPos(pos);
    SetRot(rot);

    // 当たり判定の位置を更新
    UpdateCollisionPos();

#ifdef COLLISION_TEST
    for (int nCnt = 0; nCnt < GetCntCollision(); nCnt++)
    {
        COLLISION collision = GetCollision(nCnt);
        CDebug::Create(collision.pos, collision.size, CDebug::TYPE_MOMENT);
    }
    CDebug::Create(pos, GetActiveCollisionSize(), CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// 当たり判定の位置を更新
// Author : 後藤慎之助
//=============================================================================
void CForce::UpdateCollisionPos(void)
{
    SetCollisionPos(0, GetPos());
}

//=============================================================================
// 回転制御
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CForce::RotControl(D3DXVECTOR3 rot)
{
    // 返す回転量
    D3DXVECTOR3 returnRot = rot;

    // 目標への角度を求める
    float fAngle = atan2((GetPos().x - m_targetPos.x), (GetPos().z - m_targetPos.z));

    // 求めた角度を向くようにする
    m_rotDest.y = fAngle;

    // 回転の制限
    if (returnRot.y > D3DX_PI)
    {
        returnRot.y -= D3DX_PI * 2;
    }
    if (returnRot.y < -D3DX_PI)
    {
        returnRot.y += D3DX_PI * 2;
    }

    m_rotMin.y = m_rotDest.y - returnRot.y;

    // 目的の角度の回転を制限
    if (m_rotMin.y > D3DX_PI)
    {
        m_rotDest.y -= D3DX_PI * 2;
    }
    if (m_rotMin.y < -D3DX_PI)
    {
        m_rotDest.y += D3DX_PI * 2;
    }

    // 回転の制限
    if (returnRot.y > D3DX_PI)
    {
        returnRot.y -= D3DX_PI * 2;
    }
    if (returnRot.y < -D3DX_PI)
    {
        returnRot.y += D3DX_PI * 2;
    }

    if (fabsf(m_rotMin.y) < 0)
    {
        m_rotMin.y = m_rotDest.y;
    }
    else
    {
        returnRot.y += (m_rotDest.y - returnRot.y) * FORCE_TURN_SPEED;
    }

    return returnRot;
}

//=============================================================================
// 攻撃AI処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CForce::AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // 返す位置、移動量を決める
    D3DXVECTOR3 returnPos = pos;
    D3DXVECTOR3 move = DEFAULT_VECTOR;

    switch (m_attackAI)
    {
    case ATTACK_AI_WAIT:

        // 回転制御する
        m_bUseRotControl = true;

        // カウンタアップ
        m_nCntTime++;

        // 一定カウンタで
        if (m_nCntTime >= FORCE_NEXT_ATTACK_COUNT)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 走るか攻撃するかの判断
            JudgmentRunOrAttack(pos);
        }

        break;

    case ATTACK_AI_DUSH:

        // 回転制御しない
        m_bUseRotControl = false;

        // カウンタアップ
        m_nCntTime++;

        // アニメーションを走りに
        GetAnimation()->SetAnimation(ANIM_WALK);

        // 走る
        returnPos = Dush(returnPos, rot);

        // 一定カウンタで
        if (m_nCntTime >= FORCE_DUSH_WHOLE_FRAME)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 攻撃に派生
            m_attackAI = ATTACK_AI_SHOT;
        }

        break;

    case ATTACK_AI_SHOT:

        // 回転制御する
        m_bUseRotControl = true;

        // カウンタアップ
        m_nCntTime++;

        // アニメーションを攻撃に
        GetAnimation()->SetAnimation(ANIM_ATTACK);

        // クリエイトフレーム
        if (m_nCntTime % FORCE_SHOT_CREATE_FRAME == 0)
        {
            SetShotAttack(pos, rot);
        }

        // 一定カウンタで
        if (m_nCntTime >= FORCE_SHOT_WHOLE_FRAME)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 待機中にする
            m_attackAI = ATTACK_AI_WAIT;
        }

        break;
    }

    return returnPos;
}

//=============================================================================
// 走るか攻撃するかの判断
// Author : 後藤慎之助
//=============================================================================
void CForce::JudgmentRunOrAttack(D3DXVECTOR3 pos)
{
    // 目標までの距離
    float fDistance = sqrtf(powf((m_targetPos.x - pos.x), 2) + powf((m_targetPos.z - pos.z), 2));

    // 発見距離にいるなら
    if (fDistance <= m_fDiscoveryDistance)
    {
        // 攻撃にする
        m_attackAI = ATTACK_AI_SHOT;
    }
    // 発見距離にいないなら
    else
    {
        // 走る
        m_attackAI = ATTACK_AI_DUSH;
    }
}

//=============================================================================
// ターゲットの位置を決める
// Author : 後藤慎之助
//=============================================================================
void CForce::DicisionTargetPos(D3DXVECTOR3 pos)
{
    // ターゲットの位置に応じて、優先順位をつける処理
    m_targetPos = CGame::GetPlayer()->GetPos();                                                              // 目標の位置(一度プレイヤーに)
    float fFirstTargetDistance = sqrtf(powf((m_targetPos.x - pos.x), 2) + powf((m_targetPos.z - pos.z), 2)); // 目標までの距離1
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_ENEMY);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_ENEMY); nCntScene++)
    {
        // 中身があるなら
        if (pScene != NULL)
        {
            // 次のシーンを記憶
            CScene*pNextScene = pScene->GetNextScene();

            // キャラクターにキャスト
            CCharacter *pEnemy = (CCharacter*)pScene;

            // 敵の種族が防衛軍以外なら
            if (pEnemy->GetTribe() != CCharacter::TRIBE_FORCE)
            {
                // 対象の位置
                D3DXVECTOR3 targetPos = pEnemy->GetPos();

                // 目標までの距離2
                float fSecondTargetDistance = sqrtf(
                    powf((targetPos.x - pos.x), 2) +
                    powf((targetPos.z - pos.z), 2));

                // 目標までの距離2が、目標までの距離1よりも近いなら
                if (fSecondTargetDistance < fFirstTargetDistance)
                {
                    // ターゲットの位置を更新
                    m_targetPos = targetPos;

                    // 目標までの距離1を更新
                    fFirstTargetDistance = fSecondTargetDistance;
                }
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
}

//=============================================================================
// 走る処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CForce::Dush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    D3DXVECTOR3 returnPos = pos;

    // 向ている方向に進む
    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(rot.y) * m_fSpeed, 0.0f, -cosf(rot.y) * m_fSpeed);
    returnPos += move;

    // 目標までの距離
    float fDistance = sqrtf(powf((m_targetPos.x - returnPos.x), 2) + powf((m_targetPos.z - returnPos.z), 2));

    // 発見距離にいるなら
    if (fDistance <= m_fDiscoveryDistance)
    {
        // カウンタリセット
        m_nCntTime = 0;

        // すぐに攻撃する
        m_attackAI = ATTACK_AI_SHOT;
    }

    return returnPos;
}

//=============================================================================
// 射撃攻撃
// Author : 後藤慎之助
//=============================================================================
void CForce::SetShotAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // 武器の位置取得
    D3DXVECTOR3 weaponPos = D3DXVECTOR3(GetParts(CForce::PARTS_WEAPON)->GetWorldMtx()._41,
        GetParts(CForce::PARTS_WEAPON)->GetWorldMtx()._42,
        GetParts(CForce::PARTS_WEAPON)->GetWorldMtx()._43);

    // プレイヤーとの距離を測る
    float fDistance = sqrtf(
    	powf((m_targetPos.x - weaponPos.x), 2) +
    	powf((m_targetPos.z - weaponPos.z), 2));

    // プレイヤーとの高さの差を測る
    float fHeight = fabsf((m_targetPos.y + FORCE_SHOT_AIM_UP) - weaponPos.y);

    // 角度を決める
    float fAngleY = atan2(fDistance, fHeight);

    // 移動角度を決める
    D3DXVECTOR3 moveAngle = D3DXVECTOR3(-sinf(rot.y), cosf(fAngleY), -cosf(rot.y));

    // 標的の位置のほうが、武器の位置より低いなら
    if (m_targetPos.y + FORCE_SHOT_AIM_UP < weaponPos.y)
    {
        // Y軸の移動角度を逆にする
        moveAngle.y *= -1;
    }

    // 弾をクリエイト
    CBullet::Create(weaponPos, BULLET_CLAP_COLLISION_SIZE, moveAngle, CBullet::TYPE_CLAP, CBullet::TRIBE_NEUTRAL);
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CForce::Draw(void)
{
    CCharacter::Draw();
}

//=============================================================================
// モデルデータ読み込み処理
// Author : 樋宮匠
//=============================================================================
HRESULT CForce::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/00_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_BODY], NULL, &m_nNumMat[PARTS_BODY], &m_pMesh[PARTS_BODY]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/01_head.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_HEAD], NULL, &m_nNumMat[PARTS_HEAD], &m_pMesh[PARTS_HEAD]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/02_right_arm.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_ARM], NULL, &m_nNumMat[PARTS_RIGHT_ARM], &m_pMesh[PARTS_RIGHT_ARM]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/03_right_hand.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_HAND], NULL, &m_nNumMat[PARTS_RIGHT_HAND], &m_pMesh[PARTS_RIGHT_HAND]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/04_left_arm.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_ARM], NULL, &m_nNumMat[PARTS_LEFT_ARM], &m_pMesh[PARTS_LEFT_ARM]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/05_left_hand.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_HAND], NULL, &m_nNumMat[PARTS_LEFT_HAND], &m_pMesh[PARTS_LEFT_HAND]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/06_right_leg.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_LEG], NULL, &m_nNumMat[PARTS_RIGHT_LEG], &m_pMesh[PARTS_RIGHT_LEG]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/07_right_foot.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_FOOT], NULL, &m_nNumMat[PARTS_RIGHT_FOOT], &m_pMesh[PARTS_RIGHT_FOOT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/08_left_leg.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_LEG], NULL, &m_nNumMat[PARTS_LEFT_LEG], &m_pMesh[PARTS_LEFT_LEG]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/09_left_foot.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_FOOT], NULL, &m_nNumMat[PARTS_LEFT_FOOT], &m_pMesh[PARTS_LEFT_FOOT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/10_weapon.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_WEAPON], NULL, &m_nNumMat[PARTS_WEAPON], &m_pMesh[PARTS_WEAPON]);

    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 樋宮匠
//=============================================================================
void CForce::Unload(void)
{
    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        if (m_pBuffMat[nCount] != NULL)
        {
            m_pBuffMat[nCount] = NULL;
        }

        if (m_nNumMat[nCount] != NULL)
        {
            m_nNumMat[nCount] = NULL;
        }

        if (m_pMesh[nCount] != NULL)
        {
            m_pMesh[nCount] = NULL;
        }
    }
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CForce * CForce::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // メモリ確保
    CForce *pForce = NULL;
    pForce = new CForce;

    // 親元の情報を設定
    pForce->SetPos(pos);
    pForce->SetRot(rot);
    pForce->SetTribe(CCharacter::TRIBE_FORCE);

    pForce->Init(pos, DEFAULT_SCALE);

    pForce->SetActiveCollisionSize(FORCE_ACTIVE_COLLISION_SIZE);
    pForce->SetLife(FORCE_LIFE);
    pForce->m_nScore = FORCE_SCORE;
    pForce->m_nOrbs = FORCE_ORBS;
    pForce->m_fSpeed = FORCE_SPEED;
    pForce->m_fDiscoveryDistance = FORCE_DISCOVERY_DISTANCE;

    pForce->SetCntCollision(FORCE_COLLISION_MAX);
    pForce->SetCollisionSize(0, FORCE_ACTIVE_COLLISION_SIZE);

    // 当たり判定の位置を更新
    pForce->UpdateCollisionPos();

    pForce->m_targetPos = CGame::GetPlayer()->GetPos();

    return pForce;
}

//=============================================================================
// オーブをバラまく処理
// Author : 後藤慎之助
//=============================================================================
void CForce::ScatterOrbs(void)
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

        // エフェクト発生
        CEffect3D::Create(pos, EFFECT_CHARGE_SIZE, move,
            EFFECT3D_PURPLE_FIRE_FIRST_COLOR, DEFAULT_VECTOR, EFFECT3D_PURPLE_FIRE_FADE_OUT_RATE, CEffect3D::TYPE_CHARGE);
    }
}

//=============================================================================
// ミッションクリア数に加算する
// Author : 後藤慎之助
//=============================================================================
void CForce::AddMissionClear(void)
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

            break;

        case CGame::CURRENT_MISSION_2:
            CGame::SetClearNumMission2();
            break;
        }

        break;

    case CGame::STAGE_2:

        break;

    case CGame::STAGE_FINAL:

        break;
    }
}