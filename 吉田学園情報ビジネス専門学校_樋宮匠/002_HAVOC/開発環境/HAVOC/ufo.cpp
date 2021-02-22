//================================================================================
//
// UFO処理 (ufo.cpp)
// Author : 後藤慎之助(行動AI、エフェクト) / 樋宮匠(階層構造、アニメーション制御)
//
//================================================================================

//========================
// インクルードファイル
//========================
#include "ufo.h"
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

//========================================
// 静的メンバ変数宣言
//========================================
LPD3DXMESH CUfo::m_pMesh[PARTS_MAX] = {};
LPD3DXBUFFER CUfo::m_pBuffMat[PARTS_MAX] = {};
DWORD CUfo::m_nNumMat[PARTS_MAX] = {};

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CUfo::CUfo() :CCharacter(OBJTYPE::OBJTYPE_ENEMY)
{
    m_move = DEFAULT_VECTOR;
    m_nScore = 0;
    m_nOrbs = 0;

    m_patrolCentralPos = DEFAULT_VECTOR;
    m_patrolDestPos = DEFAULT_VECTOR;
    m_nPatrolDistance = 0;
    m_nCntTime = 0;

    m_fSpeed = 0.0f;
    m_rotDest = DEFAULT_VECTOR;
    m_rotMin = DEFAULT_VECTOR;

    m_bWarning = false;
    m_fDiscoveryDistance = 0.0f;

    m_attackAI = ATTACK_AI_WAIT;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CUfo::~CUfo()
{
}

//=============================================================================
// 初期化処理
// Author : 樋宮匠
//=============================================================================
HRESULT CUfo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // パーツ数を設定
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_UFO, nCount, m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
    }

    // 座標・親子関係の読み込み
    LoadModelData("./data/ANIMATION/motion_UFO.txt");

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CUfo::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CUfo::Update(void)
{
    // 行動
    Movement();

    // アニメーションの更新
    CCharacter::Update();

    // 体力が0以下なら、スコア加算し、未使用に
    if (GetLife() <= 0)
    {
        // 位置と向きを取得
        D3DXVECTOR3 pos = DEFAULT_VECTOR;
        D3DXVECTOR3 rot = DEFAULT_VECTOR;
        pos = GetPos();
        rot = GetRot();

        // 粉砕
        DeathCrush(pos, rot);

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
            if (nRandNum <= UFO_DROP_HEAL_ITEM)
            {
                CItem::Create(pos, DEFAULT_VECTOR, CItem::TYPE_HEAL);
            }
        }

        // 終了処理
        Uninit();
    }
}

//=============================================================================
// 行動処理
// Author : 後藤慎之助
//=============================================================================
void CUfo::Movement(void)
{
    // 位置と向きを取得
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    D3DXVECTOR3 rot = DEFAULT_VECTOR;
    pos = GetPos();
    rot = GetRot();

    // 警戒していないなら
    if (m_bWarning == false)
    {
        // モーションを待機にする
        GetAnimation()->SetAnimation(ANIM_IDLE);

        // 回転制御
        rot = RotControl(rot);
    }
    // 警戒しているなら
    else
    {
        // モーションを警戒にする
        GetAnimation()->SetAnimation(ANIM_WARN);
    }

    // 1F前の位置を記憶
    SetPosOld(pos);

    // 巡回AI処理
    pos = PatrolAI(pos);

    // 警戒AI処理
    rot = WarningAI(pos, rot);

    // 攻撃AI処理
    if (CGame::GetState() == CGame::STATE_NORMAL)
    {
        pos = AttackAI(pos, rot);
    }

    // 移動量と位置を結びつける
    pos += m_move;

    // マップ制限
    CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
    if (pos.x - GetActiveCollisionSize().x < mapLimit.fWest)
    {
        pos.x = mapLimit.fWest + GetActiveCollisionSize().x;
    }
    if (pos.x + GetActiveCollisionSize().x > mapLimit.fEast)
    {
        pos.x = mapLimit.fEast - GetActiveCollisionSize().x;
    }
    if (pos.z - GetActiveCollisionSize().z < mapLimit.fSouth)
    {
        pos.z = mapLimit.fSouth + GetActiveCollisionSize().z;
    }
    if (pos.z + GetActiveCollisionSize().z > mapLimit.fNorth)
    {
        pos.z = mapLimit.fNorth - GetActiveCollisionSize().z;
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
#endif // COLLISION_TEST
}

//=============================================================================
// 当たり判定の位置を更新
// Author : 後藤慎之助
//=============================================================================
void CUfo::UpdateCollisionPos(void)
{
    SetCollisionPos(0, GetPos() + UFO_UP_COLLISION_POS);
}

//=============================================================================
// 回転制御
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CUfo::RotControl(D3DXVECTOR3 rot)
{
    // 返す回転量
    D3DXVECTOR3 returnRot = rot;

    // 1F前と座標が違うなら
    if (GetPosOld() != GetPos())
    {
        // 1F前の位置と、現在の位置を比較し、角度を求める
        float fAngle = atan2((GetPosOld().x - GetPos().x), (GetPosOld().z - GetPos().z));

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
            returnRot.y += (m_rotDest.y - returnRot.y) * UFO_TURN_SPEED;
        }
    }

    return returnRot;
}

//=============================================================================
// 巡回AI処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CUfo::PatrolAI(D3DXVECTOR3 pos)
{
    // 返す位置を決める
    D3DXVECTOR3 returnPos = pos;

    // 警戒中でないなら
    if (m_bWarning == false)
    {
        // 目的の位置までの距離を求める
        float fDistance = sqrtf(
            powf((m_patrolDestPos.x - pos.x), 2) +
            powf((m_patrolDestPos.z - pos.z), 2));

        // 距離が目的の位置に近いなら
        if (fDistance <= UFO_CONSIDER_CLOSE_DEST)
        {
            // カウンタを加算
            m_nCntTime++;

            // 一定カウンタで、次の目的地を決める
            if (m_nCntTime >= UFO_CHANGE_MOVE_IN_PATROL)
            {
                // カウンタをリセット
                m_nCntTime = 0;

                // 次の目的地を決める
                float fPosX = float(rand() % m_nPatrolDistance) - float(rand() % m_nPatrolDistance) + m_patrolCentralPos.x;
                float fPosZ = float(rand() % m_nPatrolDistance) - float(rand() % m_nPatrolDistance) + m_patrolCentralPos.z;

                // マップ制限
                CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
                if (fPosX - GetActiveCollisionSize().x < mapLimit.fWest)
                {
                    fPosX = mapLimit.fWest + GetActiveCollisionSize().x;
                }
                if (fPosX + GetActiveCollisionSize().x > mapLimit.fEast)
                {
                    fPosX = mapLimit.fEast - GetActiveCollisionSize().x;
                }
                if (fPosZ - GetActiveCollisionSize().z < mapLimit.fSouth)
                {
                    fPosZ = mapLimit.fSouth + GetActiveCollisionSize().z;
                }
                if (fPosZ + GetActiveCollisionSize().z > mapLimit.fNorth)
                {
                    fPosZ = mapLimit.fNorth - GetActiveCollisionSize().z;
                }

                // 次の目的地を、メンバ変数に結びつける
                m_patrolDestPos = D3DXVECTOR3(fPosX, 0.0f, fPosZ);
            }
        }
        // 距離が目的の位置より遠いなら
        else
        {
            // カウンタをリセット
            m_nCntTime = 0;

            // 現在の位置と、目的地までの角度を求める
            float fAngle = atan2((GetPos().x - m_patrolDestPos.x), (GetPos().z - m_patrolDestPos.z));

            // 移動量を決める
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle)*m_fSpeed, 0.0f, -cosf(fAngle)*m_fSpeed);

            // 位置に移動量を結びつける
            returnPos += move;
        }
    }

    return returnPos;
}

//=============================================================================
// 警戒AI処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CUfo::WarningAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // 返す向き
    D3DXVECTOR3 returnRot = rot;

    // プレイヤーの位置を取得
    D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

    // 警戒していないなら
    if (m_bWarning == false)
    {
        // プレイヤーとの距離を測る
        float fDistance = sqrtf(
            powf((playerPos.x - pos.x), 2) +
            powf((playerPos.z - pos.z), 2));

        // プレイヤー発見距離に達したら
        if (fDistance <= m_fDiscoveryDistance)
        {
            // 警戒中に(他に警戒中になるパターンは、攻撃をくらう、近くの仲間が攻撃をくらうがある)
            m_bWarning = true;
        }
    }
    // 警戒中なら
    else
    {
        // プレイヤーの位置と、自身の位置を比較し、角度を求める
        float fAngle = atan2((GetPos().x - playerPos.x), (GetPos().z - playerPos.z));

        // プレイヤーとの距離を測る
        float fDistance = sqrtf(
            powf((playerPos.x - pos.x), 2) +
            powf((playerPos.z - pos.z), 2));

        // プレイヤーとの高さの差を測る
        float fHeight = fabsf((playerPos.y) - pos.y);

        // 縦の角度を決める
        float fAngleY = atan2(fDistance, fHeight);

        // 求めた角度を向くようにする
        m_rotDest.x = cosf(fAngleY);
        m_rotDest.y = fAngle;

        // 標的の位置のほうが、武器の位置より低いなら
        if (playerPos.y < pos.y)
        {
            // 符号を逆にする
            m_rotDest.x *= -1;
        }

        // 回転の制限
        if (returnRot.y > D3DX_PI)
        {
            returnRot.y -= D3DX_PI * 2;
        }
        else if (returnRot.y < -D3DX_PI)
        {
            returnRot.y += D3DX_PI * 2;
        }
        if (returnRot.x > D3DX_PI)
        {
            returnRot.x -= D3DX_PI * 2;
        }
        else if (returnRot.x < -D3DX_PI)
        {
            returnRot.x += D3DX_PI * 2;
        }

        m_rotMin.x = m_rotDest.x - returnRot.x;
        m_rotMin.y = m_rotDest.y - returnRot.y;

        // 目的の角度の回転を制限
        if (m_rotMin.y > D3DX_PI)
        {
            m_rotDest.y -= D3DX_PI * 2;
        }
        else if (m_rotMin.y < -D3DX_PI)
        {
            m_rotDest.y += D3DX_PI * 2;
        }
        if (m_rotMin.x > D3DX_PI)
        {
            m_rotDest.x -= D3DX_PI * 2;
        }
        else if (m_rotMin.x < -D3DX_PI)
        {
            m_rotDest.x += D3DX_PI * 2;
        }

        // 回転の制限
        if (returnRot.y > D3DX_PI)
        {
            returnRot.y -= D3DX_PI * 2;
        }
        else if (returnRot.y < -D3DX_PI)
        {
            returnRot.y += D3DX_PI * 2;
        }
        if (returnRot.x > D3DX_PI)
        {
            returnRot.x -= D3DX_PI * 2;
        }
        else if (returnRot.x < -D3DX_PI)
        {
            returnRot.x += D3DX_PI * 2;
        }

        if (fabsf(m_rotMin.y) < 0)
        {
            m_rotMin.y = m_rotDest.y;
        }
        else
        {
            returnRot.y += (m_rotDest.y - returnRot.y) * UFO_TURN_SPEED;
        }

        if (fabsf(m_rotMin.x) < 0)
        {
            m_rotMin.x = m_rotDest.x;
        }
        else
        {
            returnRot.x += (m_rotDest.x - returnRot.x) * UFO_TURN_SPEED;
        }

        // 遠距離を超えたら
        if (fDistance >= UFO_CONSIDER_AS_LONG)
        {
            // 警戒中を解除
            m_bWarning = false;
        }
    }

    return returnRot;
}

//=============================================================================
// 攻撃AI処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CUfo::AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // 返す位置、移動量を決める
    D3DXVECTOR3 returnPos = pos;
    D3DXVECTOR3 move = DEFAULT_VECTOR;

    // プレイヤーまでの距離
    D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
    float fDistance = 0.0f;

    // 警戒中なら
    if (m_bWarning == true)
    {
        switch (m_attackAI)
        {
        case ATTACK_AI_WAIT:

            // カウンタアップ
            m_nCntTime++;

            // プレイヤーまでの距離を求める
            fDistance = sqrtf(
                powf((playerPos.x - pos.x), 2) +
                powf((playerPos.z - pos.z), 2));

            // 距離が遠いなら
            if (fDistance > UFO_CONSIDER_CLOSE_DEST)
            {
                // 追跡
                returnPos += D3DXVECTOR3(-sinf(rot.y) * UFO_TRACKING_SPEED, 0.0f, -cosf(rot.y) * UFO_TRACKING_SPEED);

                // 巡回の中心位置を変える
                m_patrolCentralPos = returnPos;
            }

            // 一定カウンタで
            if (m_nCntTime >= UFO_NEXT_ATTACK_COUNT)
            {
                // カウンタリセット
                m_nCntTime = 0;

                // 射撃攻撃へ
                m_attackAI = ATTACK_AI_SHOT;
            }

            break;

        case ATTACK_AI_SHOT:

            // カウンタアップ
            m_nCntTime++;

            // クリエイトフレーム
            if (m_nCntTime == UFO_SHOT_CREATE_FRAME_1)
            {
                SetShotAttack(pos, rot);
            }
            else if (m_nCntTime == UFO_SHOT_CREATE_FRAME_2)
            {
                SetShotAttack(pos, rot);
            }
            else if (m_nCntTime == UFO_SHOT_CREATE_FRAME_3)
            {
                SetShotAttack(pos, rot);
            }

            // 一定カウンタで
            if (m_nCntTime >= UFO_SHOT_WHOLE_FRAME)
            {
                // カウンタリセット
                m_nCntTime = 0;

                // 待機中にする
                m_attackAI = ATTACK_AI_WAIT;
            }

            break;
        }
    }

    return returnPos;
}

//=============================================================================
// 射撃攻撃
// Author : 後藤慎之助
//=============================================================================
void CUfo::SetShotAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // 移動角度を決める
    D3DXVECTOR3 moveAngle = D3DXVECTOR3(-sinf(rot.y), rot.x, -cosf(rot.y));

    // 右装飾品の位置取得
    D3DXVECTOR3 rightWing = D3DXVECTOR3(GetParts(CUfo::PARTS_RIGHT_WING_2)->GetWorldMtx()._41,
        GetParts(CUfo::PARTS_RIGHT_WING_2)->GetWorldMtx()._42,
        GetParts(CUfo::PARTS_RIGHT_WING_2)->GetWorldMtx()._43);

    // 左装飾品の位置取得
    D3DXVECTOR3 leftWing = D3DXVECTOR3(GetParts(CUfo::PARTS_LEFT_WING_2)->GetWorldMtx()._41,
        GetParts(CUfo::PARTS_LEFT_WING_2)->GetWorldMtx()._42,
        GetParts(CUfo::PARTS_LEFT_WING_2)->GetWorldMtx()._43);

    // 弾をクリエイト
    CBullet::Create(pos, BULLET_AR_COLLISION_SIZE, moveAngle, CBullet::TYPE_CLAP, CBullet::TRIBE_ENEMY);

    // 弾をクリエイト
    CBullet::Create(rightWing, BULLET_AR_COLLISION_SIZE, moveAngle, CBullet::TYPE_CLAP, CBullet::TRIBE_ENEMY);

    // 弾をクリエイト
    CBullet::Create(leftWing, BULLET_AR_COLLISION_SIZE, moveAngle, CBullet::TYPE_CLAP, CBullet::TRIBE_ENEMY);
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CUfo::Draw(void)
{
    CCharacter::Draw();
}

//=============================================================================
// モデルデータ読み込み処理
// Author : 樋宮匠
//=============================================================================
HRESULT CUfo::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/00_Fighter.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_FIGHTER], NULL, &m_nNumMat[PARTS_FIGHTER], &m_pMesh[PARTS_FIGHTER]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/01_RightWing1.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_WING_0], NULL, &m_nNumMat[PARTS_RIGHT_WING_0], &m_pMesh[PARTS_RIGHT_WING_0]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/02_RightWing2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_WING_1], NULL, &m_nNumMat[PARTS_RIGHT_WING_1], &m_pMesh[PARTS_RIGHT_WING_1]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/03_RightWing3.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_WING_2], NULL, &m_nNumMat[PARTS_RIGHT_WING_2], &m_pMesh[PARTS_RIGHT_WING_2]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/04_RightWing4.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_WING_3], NULL, &m_nNumMat[PARTS_RIGHT_WING_3], &m_pMesh[PARTS_RIGHT_WING_3]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/05_LeftWing1.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_WING_0], NULL, &m_nNumMat[PARTS_LEFT_WING_0], &m_pMesh[PARTS_LEFT_WING_0]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/06_LeftWing2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_WING_1], NULL, &m_nNumMat[PARTS_LEFT_WING_1], &m_pMesh[PARTS_LEFT_WING_1]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/07_LeftWing3.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_WING_2], NULL, &m_nNumMat[PARTS_LEFT_WING_2], &m_pMesh[PARTS_LEFT_WING_2]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/08_LeftWing4.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_WING_3], NULL, &m_nNumMat[PARTS_LEFT_WING_3], &m_pMesh[PARTS_LEFT_WING_3]);
    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 樋宮匠
//=============================================================================
void CUfo::Unload(void)
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
CUfo * CUfo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // メモリ確保
    CUfo *pUfo = NULL;
    pUfo = new CUfo;

    // 親元の情報を設定
    pUfo->SetPos(pos);
    pUfo->SetRot(rot);
    pUfo->SetTribe(CCharacter::TRIBE_UFO);

    // 初期化
    pUfo->Init(pos, DEFAULT_SCALE);

    // パトロールの中心位置を記憶
    pUfo->m_patrolCentralPos = pos;

    // 目標位置を記憶
    pUfo->m_patrolDestPos = pos;

    pUfo->SetActiveCollisionSize(UFO_ACTIVE_COLLISION_SIZE);
    pUfo->SetLife(UFO_LIFE);
    pUfo->m_nScore = UFO_SCORE;
    pUfo->m_nOrbs = UFO_ORBS;
    pUfo->m_fSpeed = UFO_SPEED;
    pUfo->m_nPatrolDistance = UFO_PATROL_DISTANCE;
    pUfo->m_fDiscoveryDistance = UFO_DISCOVERY_DISTANCE;

    pUfo->SetCntCollision(UFO_COLLISION_MAX);
    pUfo->SetCollisionSize(0, UFO_ACTIVE_COLLISION_SIZE);

    // 当たり判定の位置を更新
    pUfo->UpdateCollisionPos();

    return pUfo;
}

//=============================================================================
// オーブをバラまく処理
// Author : 後藤慎之助
//=============================================================================
void CUfo::ScatterOrbs(void)
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
// やられた時の粉砕処理
// Author : 後藤慎之助
//=============================================================================
void CUfo::DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // パーツの位置と、モデルエフェクトの種類
    D3DXVECTOR3 partsPos = DEFAULT_VECTOR;
    CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_UFO;

    //==============================
    // 体
    //==============================
    // 位置
    partsPos = D3DXVECTOR3(GetParts(CUfo::PARTS_FIGHTER)->GetWorldMtx()._41,
        GetParts(CUfo::PARTS_FIGHTER)->GetWorldMtx()._42,
        GetParts(CUfo::PARTS_FIGHTER)->GetWorldMtx()._43);

    // エフェクトの発生
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // 翼
    //==============================
    modelEffectType = CModelEffect::TYPE_UFO_WING;

    // 足の本数分バラまく
    for (int nCnt = 0; nCnt < UFO_MAX_LEG; nCnt++)
    {
        // エフェクトの発生(足は中心から出す)
        SetDeathCrushEffect(pos, rot, modelEffectType);
    }
}

//=============================================================================
// やられた時の粉砕エフェクトの発生
// Author : 後藤慎之助
//=============================================================================
void CUfo::SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type)
{
    // 粉砕エフェクト用の変数宣言
    float fSpeedX = 0.0f;               // 速さX
    float fSpeedY = 0.0f;               // 速さY
    float fSpeedZ = 0.0f;               // 速さZ
    float fRotSpeedX = 0.0f;            // 回転速度X
    float fRotSpeedY = 0.0f;            // 回転速度Y
    float fRotSpeedZ = 0.0f;            // 回転速度Z
    int nPlusMinusX = 0;                // 符号X
    int nPlusMinusZ = 0;                // 符号Z

    // 速さと、符号をランダムに決める
    fSpeedX = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_X, 0)) / 100.0f;
    fSpeedY = 0.0f;
    fSpeedZ = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Z, 0)) / 100.0f;
    fRotSpeedX = float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f;
    fRotSpeedY = float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f;
    fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f;
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

    // 回転量を決める
    D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

    // エフェクト発生
    CModelEffect::Create(pos, rot, move, rotMove, type);
}

//=============================================================================
// 仲間に警告する処理
// Author : 後藤慎之助
//=============================================================================
void CUfo::WarnFellow(void)
{
    // 周囲の仲間を警戒中にする処理
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

            // 敵の種族がUFOなら
            if (pEnemy->GetTribe() == CCharacter::TRIBE_UFO)
            {
                // 対象の位置
                D3DXVECTOR3 targetPos = pEnemy->GetPos();

                // 対象との距離
                float fDistance = sqrtf(
                    powf((targetPos.x - GetPos().x), 2) +
                    powf((targetPos.z - GetPos().z), 2));

                // 対象との距離が、仲間に警告するときの範囲内なら
                if (fDistance <= UFO_WARN_UFO_DISTANCE)
                {
                    // UFO型にキャスト
                    CUfo*pUfo = (CUfo*)pEnemy;

                    // 警戒中に
                    pUfo->SetWarning(true);
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
// ミッションクリア数に加算する
// Author : 後藤慎之助
//=============================================================================
void CUfo::AddMissionClear(void)
{
    // ステージと現在のミッションに応じて、クリア数を増加
    CGame::STAGE stage = CGame::GetStage();
    CGame::CURRENT_MISSION currentMission = CGame::GetCurrentMission();
    switch (stage)
    {
    case CGame::STAGE_1:

        break;

    case CGame::STAGE_2:

        switch (currentMission)
        {
        case CGame::CURRENT_MISSION_1:
            CGame::SetClearNumMission1();
            break;

        case CGame::CURRENT_MISSION_2:

            break;
        }

        break;

    case CGame::STAGE_FINAL:

        break;
    }
}