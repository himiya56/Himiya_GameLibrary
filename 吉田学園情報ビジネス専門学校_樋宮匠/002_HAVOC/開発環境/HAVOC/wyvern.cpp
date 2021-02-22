//================================================================================
//
// ワイバーンの処理 (wyvern.cpp)
// Author : 後藤慎之助(行動AI、エフェクト) / 樋宮匠(階層構造、アニメーション制御)
//
//================================================================================

//========================
// インクルードファイル
//========================
#include "wyvern.h"
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
#include "camera.h"
#include "sound.h"

//========================================
// 静的メンバ変数宣言
//========================================
LPD3DXMESH CWyvern::m_pMesh[PARTS_MAX] = {};
LPD3DXBUFFER CWyvern::m_pBuffMat[PARTS_MAX] = {};
DWORD CWyvern::m_nNumMat[PARTS_MAX] = {};

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CWyvern::CWyvern() :CCharacter(OBJTYPE::OBJTYPE_ENEMY)
{
    m_move = DEFAULT_VECTOR;
    m_nScore = 0;
    m_nOrbs = 0;

    m_nCntTime = 0;

    m_fSpeed = 0.0f;
    m_rotDest = DEFAULT_VECTOR;
    m_rotMin = DEFAULT_VECTOR;

    m_attackAI = ATTACK_AI_ROAR;

    m_bUseRotControl = true;

    m_bStartBattle = false;

    m_nRightWingDurabirity = WYVERN_WING_DURABILITY;
    m_nLeftWingDurabirity = WYVERN_WING_DURABILITY;
    m_nTailDurabirity = WYVERN_TAIL_DURABILITY;
    m_bDestroyRightWing = false;
    m_bDestroyLeftWing = false;
    m_bDestroyTail = false;

    m_bFlySomersault = false;
    m_bPrevRoar = false;

    m_bStun = false;
    m_nStunValue = 0;
    m_nCntFlinch = 0;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CWyvern::~CWyvern()
{
}

//=============================================================================
// 初期化処理
// Author : 樋宮匠
//=============================================================================
HRESULT CWyvern::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // パーツ数を設定
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_WYVERN, nCount, m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
    }

    // 座標・親子関係の読み込み
    LoadModelData("./data/ANIMATION/motion_wyvern.txt");

    // ボス仕様に
    CCharacter::SetBossSetting();

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CWyvern::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CWyvern::Update(void)
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

    	// オーブをバラまく
    	ScatterOrbs(m_nOrbs);

    	// スコア加算
    	CScore *pScore = CGame::GetScore();
    	pScore->AddScore(m_nScore);

    	// 一定確率で、回復アイテムを落とす
    	int nRandNum = GetRandNum(100, 1);
    	if (nRandNum <= WYVERN_DROP_HEAL_ITEM)
    	{
    		CItem::Create(pos, DEFAULT_VECTOR, CItem::TYPE_HEAL);
    	}

    	// 終了処理
    	Uninit();
    }
}

//=============================================================================
// 行動処理
// Author : 後藤慎之助
//=============================================================================
void CWyvern::Movement(void)
{
	// 位置と向きを取得
	D3DXVECTOR3 pos = DEFAULT_VECTOR;
	D3DXVECTOR3 rot = DEFAULT_VECTOR;
	pos = GetPos();
	rot = GetRot();

	// モーションをまずは待機にする
	GetAnimation()->SetAnimation(ANIM_IDLE);

    // 回転制御するなら
    if (m_bUseRotControl == true)
    {
        // 回転制御
        rot = RotControl(rot);
    }

	// 1F前の位置を記憶
	SetPosOld(pos);

    // 怒りなら
    if (GetLife() <= WYVERN_START_ANGRY_LIFE)
    {
        // 速さアップ
        m_fSpeed = WYVERN_SPEED_ANGRY;
    }

    // スタン値処理
    if (m_nStunValue >= WYVERN_STUN_VALUE_MAX)
    {
        // 叫び声
        CSound*pSound = CManager::GetSound();
        pSound->Play(CSound::LABEL_SE_BOSS_DEATH);

        // スタン値リセット
        m_nStunValue = 0;

        // カウンタリセット
        m_nCntTime = 0;

        // スタン
        m_bStun = true;

        // 怯み回数上昇
        m_nCntFlinch++;
    }

    // スタンしていないなら
    if (m_bStun == false)
    {
        // 攻撃AI処理
        if (m_bStartBattle == true)
        {
            pos = AttackAI(pos, rot);
        }
    }
    // スタンしているなら
    else
    {
        // スタンか怯みかの処理
        pos = StunOrFlinch(pos, rot);
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

        // バトル開始
        if (m_bStartBattle == false)
        {
            SetLandingEffect(pos);
            m_bStartBattle = true;
        }
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

            // 当たり判定の数分回す
            for (int nCntCollision = 0; nCntCollision < GetCntCollision(); nCntCollision++)
            {
                // パーツの当たり判定を取得
                COLLISION partsCollision = GetCollision(nCntCollision);

                // 当たっているなら
                if (RectangleCollision3D(&partsCollision.pos, &blockPos,
                    &partsCollision.size, &blockSize) == true)
                {
                    // ブロックを破壊
                    pBlock->SetTakeDamage(WYVERN_CRUSH_BLOCK, CBlock::LAST_HIT_ENEMY);
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
    //CDebug::Create(pos, GetActiveCollisionSize(), CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// 当たり判定の位置を更新
// Author : 後藤慎之助
//=============================================================================
void CWyvern::UpdateCollisionPos(void)
{
	// 頭の箱の位置取得
	D3DXVECTOR3 headCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_HEAD)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_CUBE_HEAD)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_CUBE_HEAD)->GetWorldMtx()._43);

	// 頭の位置取得
	D3DXVECTOR3 headPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._43);

	// 体の位置取得
	D3DXVECTOR3 bodyPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._43);

	// 右手の位置取得
	D3DXVECTOR3 rightHandPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_ARM_RIGHT)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_CUBE_ARM_RIGHT)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_CUBE_ARM_RIGHT)->GetWorldMtx()._43);

	// 左手の位置取得
	D3DXVECTOR3 leftHandPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_ARM_LEFT)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_CUBE_ARM_LEFT)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_CUBE_ARM_LEFT)->GetWorldMtx()._43);

    // 右足の位置取得
    D3DXVECTOR3 footRightPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_FOOT_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_FOOT_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_FOOT_RIGHT)->GetWorldMtx()._43);

    // 左足の位置取得
    D3DXVECTOR3 footLeftPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_FOOT_LEFT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_FOOT_LEFT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_FOOT_LEFT)->GetWorldMtx()._43);

    // 右翼の箱の位置取得
    D3DXVECTOR3 wingRightCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_WING_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_WING_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_WING_RIGHT)->GetWorldMtx()._43);

    // 左翼の位置取得
    D3DXVECTOR3 wingLeftPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._43);

    // 右翼の位置取得
    D3DXVECTOR3 wingRightPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._43);

    // 左翼の箱の位置取得
    D3DXVECTOR3 wingLeftCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_WING_LEFT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_WING_LEFT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_WING_LEFT)->GetWorldMtx()._43);

    // 尻尾の根の位置取得
    D3DXVECTOR3 tailRootPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_TAIL_ROOT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_TAIL_ROOT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_TAIL_ROOT)->GetWorldMtx()._43);

    // 尻尾の中間の位置取得
    D3DXVECTOR3 tailTipPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._43);

    // 尻尾の箱の位置取得
    D3DXVECTOR3 tailCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_TAIL)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_TAIL)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_TAIL)->GetWorldMtx()._43);

    // 右肘の位置取得
    D3DXVECTOR3 rightElbowPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_ELBOW_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_ELBOW_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_ELBOW_RIGHT)->GetWorldMtx()._43);

    // 左肘の位置取得
    D3DXVECTOR3 leftElbowPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_ELBOW_LEFT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_ELBOW_LEFT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_ELBOW_LEFT)->GetWorldMtx()._43);

    // 右足の箱の位置取得
    D3DXVECTOR3 footRightCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_FOOT_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_FOOT_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_FOOT_RIGHT)->GetWorldMtx()._43);

    // 左足の箱の位置取得
    D3DXVECTOR3 footLeftCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_FOOT_LEFT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_FOOT_LEFT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_FOOT_LEFT)->GetWorldMtx()._43);

	// 当たり判定の位置を更新
    SetCollisionPos(COLLISION_PARTS_HEAD_CUBE, headCubePos);
    SetCollisionPos(COLLISION_PARTS_HEAD, headPos);
    SetCollisionPos(COLLISION_PARTS_BODY, bodyPos);
    SetCollisionPos(COLLISION_PARTS_ARM_RIGHT, rightHandPos);
    SetCollisionPos(COLLISION_PARTS_ARM_LEFT, leftHandPos);
    SetCollisionPos(COLLISION_PARTS_FOOT_RIGHT, footRightPos);
    SetCollisionPos(COLLISION_PARTS_FOOT_LEFT, footLeftPos);
    SetCollisionPos(COLLISION_PARTS_WING_RIGHT, wingRightCubePos);
    SetCollisionPos(COLLISION_PARTS_WING_LEFT, wingLeftCubePos);
    SetCollisionPos(COLLISION_PARTS_TAIL_ROOT, tailRootPos);
    SetCollisionPos(COLLISION_PARTS_TAIL_TIP, tailTipPos);
    SetCollisionPos(COLLISION_PARTS_TAIL_CUBE, tailCubePos);

    SetCollisionPos(COLLISION_PARTS_ELBOW_RIGHT, rightElbowPos);
    SetCollisionPos(COLLISION_PARTS_ELBOW_LEFT, leftElbowPos);
    SetCollisionPos(COLLISION_PARTS_FOOT_CUBE_RIGHT, footRightCubePos);
    SetCollisionPos(COLLISION_PARTS_FOOT_CUBE_LEFT, footLeftCubePos);

    // 部位破壊処理
    CScore *pScore = CGame::GetScore();

    // 尻尾
    if (m_bDestroyTail == false)
    {
        // 耐久値がないなら
        if (m_nTailDurabirity <= 0)
        {
            // 破壊のフラグをtrueに
            m_bDestroyTail = true;

            // スコア加算
            pScore->AddScore(WYVERN_TAIL_SCORE);

            // スケールと当たり判定をなくす
            GetParts(CWyvern::PARTS_TAIL_TIP)->SetScale(DEFAULT_VECTOR);
            SetCollisionSize(COLLISION_PARTS_TAIL_TIP, DEFAULT_VECTOR);

            // エフェクトの発生
            SetDeathCrushEffect(tailTipPos, GetRot(), CModelEffect::TYPE_TAIL);

            // オーブをバラまく
            ScatterOrbs(WYVERN_TAIL_ORBS);

            // スタンしていないなら
            if (m_bStun == false)
            {
                // 確定スタン
                m_nStunValue += WYVERN_STUN_VALUE_MAX;
            }
        }
    }
    // 左翼
    if (m_bDestroyLeftWing == false)
    {
        // 耐久値がないなら
        if (m_nLeftWingDurabirity <= 0)
        {
            // 破壊のフラグをtrueに
            m_bDestroyLeftWing = true;

            // スコア加算
            pScore->AddScore(WYVERN_WING_SCORE);

            // スケールと当たり判定をなくす
            GetParts(CWyvern::PARTS_WING_LEFT)->SetScale(DEFAULT_VECTOR);
            SetCollisionSize(COLLISION_PARTS_WING_LEFT, DEFAULT_VECTOR);

            // エフェクトの発生
            SetDeathCrushEffect(wingLeftPos, GetRot(), CModelEffect::TYPE_LEFT_WING);

            // オーブをバラまく
            ScatterOrbs(WYVERN_WING_ORBS);

            // スタンしていないなら
            if (m_bStun == false)
            {
                // 確定スタン
                m_nStunValue += WYVERN_STUN_VALUE_MAX;
            }
        }
    }
    // 右翼
    if (m_bDestroyRightWing == false)
    {
        // 耐久値がないなら
        if (m_nRightWingDurabirity <= 0)
        {
            // 破壊のフラグをtrueに
            m_bDestroyRightWing = true;

            // スコア加算
            pScore->AddScore(WYVERN_WING_SCORE);

            // スケールと当たり判定をなくす
            GetParts(CWyvern::PARTS_WING_RIGHT)->SetScale(DEFAULT_VECTOR);
            SetCollisionSize(COLLISION_PARTS_WING_RIGHT, DEFAULT_VECTOR);

            // エフェクトの発生
            SetDeathCrushEffect(wingRightPos, GetRot(), CModelEffect::TYPE_RIGHT_WING);

            // オーブをバラまく
            ScatterOrbs(WYVERN_WING_ORBS);

            // スタンしていないなら
            if (m_bStun == false)
            {
                // 確定スタン
                m_nStunValue += WYVERN_STUN_VALUE_MAX;
            }
        }
    }
}

//=============================================================================
// 回転制御
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CWyvern::RotControl(D3DXVECTOR3 rot)
{
    // 返す回転量
    D3DXVECTOR3 returnRot = rot;

    // プレイヤーの位置を求める
    D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

    // プレイヤーへの角度を求める
    float fAngle = atan2((GetPos().x - playerPos.x), (GetPos().z - playerPos.z));

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
        returnRot.y += (m_rotDest.y - returnRot.y) * WYVERN_TURN_SPEED;
    }

    return returnRot;
}

//=============================================================================
// スタンか怯みかの処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CWyvern::StunOrFlinch(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{        
    // 返す位置を決める
    D3DXVECTOR3 returnPos = pos;

    // 回転制御しない
    m_bUseRotControl = false;

    // カウンタアップ
    m_nCntTime++;

    // 怯み回数が一定以上でスタン
    if (m_nCntFlinch >= WYVERN_FLINCH_MAX)
    {
        // アニメーションをスタンに
        GetAnimation()->SetAnimation(ANIM_STUN);

        // 一定カウンタで
        if (m_nCntTime >= WYVERN_STUN_WHOLE_FRAME)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 待機中にする
            m_attackAI = ATTACK_AI_WAIT;

            // スタン解除
            m_bStun = false;

            // 怯み回数リセット
            m_nCntFlinch = 0;
        }
        else if (m_nCntTime == WYVERN_STUN_CREATE_FRAME)
        {
            // スタン時にエフェクト発生
            SetLandingEffect(pos);
        }
    }
    else
    {
        // アニメーションを怯みに
        GetAnimation()->SetAnimation(ANIM_FLINCH);

        // 一定カウンタで
        if (m_nCntTime >= WYVERN_FLINCH_WHOLE_FRAME)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 待機中にする
            m_attackAI = ATTACK_AI_WAIT;

            // スタン解除
            m_bStun = false;
        }
        else if (m_nCntTime <= WYVERN_FLINCH_KNOCKBACK_END_FRAME)
        {
            // 向ている方の逆に進む
            D3DXVECTOR3 move = D3DXVECTOR3(sinf(rot.y) * WYVERN_FLINCH_KNOCKBACK_VALUE, 0.0f, cosf(rot.y) * WYVERN_FLINCH_KNOCKBACK_VALUE);
            returnPos += move;
        }
    }

    return returnPos;
}

//=============================================================================
// 攻撃AI処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CWyvern::AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // 返す位置、移動量を決める
    D3DXVECTOR3 returnPos = pos;
    D3DXVECTOR3 move = DEFAULT_VECTOR;

    // 音を取得
    CSound*pSound = CManager::GetSound();

    switch (m_attackAI)
    {
    case ATTACK_AI_WAIT:

        // 回転制御する
        m_bUseRotControl = true;

        // 空中サマーソルトのフラグをfalseに
        m_bFlySomersault = false;

        // カウンタアップ
        m_nCntTime++;

        // 一定カウンタで
        if (m_nCntTime >= WYVERN_NEXT_ATTACK_COUNT)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 次の攻撃を決める
            NextAttackSelect(pos);
        }

        break;

    case ATTACK_AI_TACKLE:

        // 咆哮していない
        m_bPrevRoar = false;

        // 回転制御しない
        m_bUseRotControl = false;

        // カウンタアップ
        m_nCntTime++;

        // アニメーションを走りに
        GetAnimation()->SetAnimation(ANIM_WALK);

        // 走る
        returnPos = Dush(returnPos, rot);

        // 一定カウンタで
        if (m_nCntTime >= WYVERN_TACKLE_WHOLE_FRAME)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 待機中にする
            m_attackAI = ATTACK_AI_WAIT;
        }

        break;

    case ATTACK_AI_FLY:

        // 咆哮していない
        m_bPrevRoar = false;

        // 回転制御する
        m_bUseRotControl = true;

        // カウンタアップ
        m_nCntTime++;

        // アニメーションを羽ばたきに
        GetAnimation()->SetAnimation(ANIM_FLY);

        // 走る
        returnPos = Dush(returnPos, rot);

        // 一定カウンタで
        if (m_nCntTime >= WYVERN_FLY_WHOLE_FRAME)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 強襲にする
            m_attackAI = ATTACK_AI_ASSAULT;
        }

        break;

    case ATTACK_AI_ASSAULT:

        // 咆哮していない
        m_bPrevRoar = false;

        // 回転制御しない
        m_bUseRotControl = false;

        // カウンタアップ
        m_nCntTime++;

        // アニメーションを強襲に
        GetAnimation()->SetAnimation(ANIM_ASSAULT);

        // 走る
        returnPos = Dush(returnPos, rot);

        // 一定カウンタで
        if (m_nCntTime >= WYVERN_ASSULT_WHOLE_FRAME)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 待機中にする
            m_attackAI = ATTACK_AI_WAIT;
        }
        else if (m_nCntTime == WYVERN_ASSULT_CREATE_FRAME_1)
        {
            // 風切り音
            pSound->Play(CSound::LABEL_SE_BOSS_WIND);

            // 爆発生成
            SetExplosion(false, true);
        }
        else if (m_nCntTime == WYVERN_ASSULT_CREATE_FRAME_2)
        {
            // 爆発生成
            SetExplosion(false, true);
        }
        else if (m_nCntTime == WYVERN_ASSULT_CREATE_FRAME_3)
        {
            // 爆発生成
            SetExplosion(false, true);
        }

        break;

    case ATTACK_AI_SOMERSAULT:

        // 咆哮していない
        m_bPrevRoar = false;

        // 回転制御しない
        m_bUseRotControl = false;

        // カウンタアップ
        m_nCntTime++;

        // アニメーションをサマーソルトに
        GetAnimation()->SetAnimation(ANIM_SOMERSAULT);

        // 一定カウンタで
        if (m_nCntTime >= WYVERN_SOMERSAULT_WHOLE_FRAME)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 派生する体力になり
            if (GetLife() <= WYVERN_SOMERSAULT_DERIVE_FLY_LIFE)
            {
                // 羽があるなら
                if (m_bDestroyLeftWing == false || m_bDestroyRightWing == false)
                {
                    // 飛行中にする
                    m_attackAI = ATTACK_AI_FLY;
                }
                else
                {
                    // 待機中にする
                    m_attackAI = ATTACK_AI_WAIT;
                }
            }
            else
            {
                // 待機中にする
                m_attackAI = ATTACK_AI_WAIT;
            }
        }
        // クリエイトフレーム
        else if (m_nCntTime == WYVERN_SOMERSALUT_CREATE_FRAME)
        {
            // 尻尾があるなら
            if (m_bDestroyTail == false)
            {
                // 大爆発音
                pSound->Play(CSound::LABEL_SE_EXPLOSION_BIG);

                // 衝撃波攻撃
                SetShockWave(pos, rot);
            }
        }

        break;

    case ATTACK_AI_EXPLOSION:

        // 咆哮していない
        m_bPrevRoar = false;

        // 回転制御しない
        m_bUseRotControl = false;

        // カウンタアップ
        m_nCntTime++;

        // アニメーションを爆砕に
        GetAnimation()->SetAnimation(ANIM_EXPLOSION);

        // クリエイトスタートフレーム
        if (m_nCntTime == WYVERN_EXPLOSION_CREATE_START_FRAME)
        {
            // 叩きつける音
            pSound->Play(CSound::LABEL_SE_DESTROY_METAL);

            // プレイヤーの下に生成
            SetExplosion(true, false);
        }
        // スタート以降
        else if (m_nCntTime > WYVERN_EXPLOSION_CREATE_START_FRAME)
        {
            // クリエイトフレーム
            if (m_nCntTime % WYVERN_EXPLOSION_CREATE_FRAME == 0)
            {
                SetExplosion(false, false);
            }
        }

        // 一定カウンタで
        if (m_nCntTime >= WYVERN_EXPLOSION_WHOLE_FRAME)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 待機中にする
            m_attackAI = ATTACK_AI_WAIT;
        }

        break;

    case ATTACK_AI_ROAR:

        // 咆哮した
        m_bPrevRoar = true;

        // 回転制御しない
        m_bUseRotControl = false;

        // カウンタアップ
        m_nCntTime++;

        // アニメーションを咆哮に
        GetAnimation()->SetAnimation(ANIM_ROAR);

        // クリエイトフレーム
        if (m_nCntTime == WYVERN_ROAR_CREATE_FRAME)
        {
            // 咆哮音
            pSound->Play(CSound::LABEL_SE_BOSS_ROAR);

            // 敵生成
            SetEnemySpawn(pos, rot);
        }

        // 一定カウンタで
        if (m_nCntTime >= WYVERN_ROAR_WHOLE_FRAME)
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
// 次の攻撃を決める
// Author : 後藤慎之助
//=============================================================================
void CWyvern::NextAttackSelect(D3DXVECTOR3 pos)
{
    // プレイヤーの位置を取得
    D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

    // プレイヤーとの距離を測る
    float fDistance = sqrtf(
        powf((playerPos.x - pos.x), 2) +
        powf((playerPos.z - pos.z), 2));

    // ランダムに数字を決める
    int nRandNum = GetRandNum(100, 1);

    // 何の攻撃が採用されたか、距離によって決める
    if (nRandNum >= 1 && nRandNum <= WYVERN_FIRST_PRIORITY)
    {
        // 近距離
        if (fDistance <= WYVERN_CONSIDER_AS_SHORT)
        {
            m_attackAI = ATTACK_AI_SOMERSAULT;
        }
        // 中距離
        else if (fDistance > WYVERN_CONSIDER_AS_SHORT && fDistance <= WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_EXPLOSION;
        }
        // 遠距離
        else if (fDistance > WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_SOMERSAULT;
        }
    }
    else if (nRandNum > WYVERN_FIRST_PRIORITY && nRandNum <= WYVERN_FIRST_PRIORITY + WYVERN_SECOND_PRIORITY)
    {
        // 近距離
        if (fDistance <= WYVERN_CONSIDER_AS_SHORT)
        {
            m_attackAI = ATTACK_AI_EXPLOSION;
        }
        // 中距離
        else if (fDistance > WYVERN_CONSIDER_AS_SHORT && fDistance <= WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_SOMERSAULT;
        }
        // 遠距離
        else if (fDistance > WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_TACKLE;
        }
    }
    else if (nRandNum > WYVERN_FIRST_PRIORITY + WYVERN_SECOND_PRIORITY
        && nRandNum <= WYVERN_FIRST_PRIORITY + WYVERN_SECOND_PRIORITY + WYVERN_THIRD_PRIORITY)
    {
        // 近距離
        if (fDistance <= WYVERN_CONSIDER_AS_SHORT)
        {
            m_attackAI = ATTACK_AI_TACKLE;
        }
        // 中距離
        else if (fDistance > WYVERN_CONSIDER_AS_SHORT && fDistance <= WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_TACKLE;
        }
        // 遠距離
        else if (fDistance > WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_EXPLOSION;
        }
    }
    else if (nRandNum > WYVERN_FIRST_PRIORITY + WYVERN_SECOND_PRIORITY + WYVERN_THIRD_PRIORITY
        && nRandNum <= WYVERN_FIRST_PRIORITY + WYVERN_SECOND_PRIORITY + WYVERN_THIRD_PRIORITY + +WYVERN_FOURTH_PRIORITY)
    {
        // ひとつ前の攻撃が咆哮でないなら
        if (m_bPrevRoar == false)
        {
            m_attackAI = ATTACK_AI_ROAR;
        }
        // 咆哮なら一番優先順位の高い攻撃に
        else
        {
            // 近距離
            if (fDistance <= WYVERN_CONSIDER_AS_SHORT)
            {
                m_attackAI = ATTACK_AI_SOMERSAULT;
            }
            // 中距離
            else if (fDistance > WYVERN_CONSIDER_AS_SHORT && fDistance <= WYVERN_CONSIDER_AS_MEDIUM)
            {
                m_attackAI = ATTACK_AI_EXPLOSION;
            }
            // 遠距離
            else if (fDistance > WYVERN_CONSIDER_AS_MEDIUM)
            {
                m_attackAI = ATTACK_AI_SOMERSAULT;
            }
        }
    }
}

//=============================================================================
// 走る処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CWyvern::Dush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    D3DXVECTOR3 returnPos = pos;

    // 向ている方向に進む
    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(rot.y) * m_fSpeed, 0.0f, -cosf(rot.y) * m_fSpeed);
    if (m_attackAI == ATTACK_AI_FLY)
    {
        // 飛行中なら減速
        move *= WYVERN_FLY_DOWN_SPEED_RATE;
    }
    else if (m_attackAI == ATTACK_AI_ASSAULT)
    {
        if (m_nCntTime >= WYVERN_ASSULT_UP_SPEED_FRAME)
        {
            // 強襲中の一定フレームから加速
            move *= WYVERN_ASSULT_UP_SPEED_RATE;
        }
    }
    returnPos += move;

    // 突進中、または飛行中でサマーソルトまでのクールタイムがなく、連続でないなら
    if (m_attackAI == ATTACK_AI_TACKLE ||
        m_attackAI == ATTACK_AI_FLY && m_nCntTime >= WYVERN_FLY_START_SOMERSAULT_FRAME && m_bFlySomersault == false)
    {
        // プレイヤーの位置を求める
        D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

        // 目標までの距離
        float fDistance = sqrtf(powf((playerPos.x - returnPos.x), 2) + powf((playerPos.z - returnPos.z), 2));

        // 近距離にいるなら
        if (fDistance <= WYVERN_CONSIDER_AS_SHORT)
        {
            // カウンタリセット
            m_nCntTime = 0;

            // 飛んでいるなら
            if (m_attackAI == ATTACK_AI_FLY)
            {
                // 空中サマーソルトのフラグをtrueに
                m_bFlySomersault = true;

                // すぐにサマーソルトする
                m_attackAI = ATTACK_AI_SOMERSAULT;
            }
            // 地上なら
            else
            {
                // サマーソルトか爆砕
                int nRandNum = GetRandNum(2, 1);
                switch (nRandNum)
                {
                case 1:
                    // サマーソルト
                    m_attackAI = ATTACK_AI_SOMERSAULT;
                    break;
                case 2:
                    // 爆砕
                    m_attackAI = ATTACK_AI_EXPLOSION;
                    break;
                }
            }
        }
    }

    return returnPos;
}

//=============================================================================
// 衝撃波攻撃
// Author : 後藤慎之助
//=============================================================================
void CWyvern::SetShockWave(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // 複数個飛ばす
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR;
    float fHorizontalAngle = 0.0f;
    for (int nCnt = 0; nCnt < WYVERN_SOMERSAULT_SHOCK_NUMBER_ONCE; nCnt++)
    {
        // 横に拡散させる
        if (nCnt == 1)
        {
            fHorizontalAngle = WYVERN_SOMERSAULT_SHOCK_ANGLE_XZ;
        }
        else if (nCnt == 2)
        {
            fHorizontalAngle = -WYVERN_SOMERSAULT_SHOCK_ANGLE_XZ;
        }

        // 移動角度を決める
        moveAngle = D3DXVECTOR3(-sinf(rot.y + fHorizontalAngle), 0.0f, -cosf(rot.y + fHorizontalAngle));

        // 弾をクリエイト
        CBullet::Create(pos, BULLET_SHOCK_COLLISION_SIZE, moveAngle, CBullet::TYPE_SHOCK, CBullet::TRIBE_ENEMY);
    }
}

//=============================================================================
// 爆砕攻撃
// Author : 後藤慎之助
//=============================================================================
void CWyvern::SetExplosion(bool bUseTracking, bool bFall)
{
    // 体の位置取得
    D3DXVECTOR3 bodyPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._43);

    // 0～3.14を出す*(0 or 1)*-1で正負を決める
    float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

    // 設置する位置の半径を出す
    float fRadius = float(GetRandNum(WYVERN_EXPLOSION_MAX_RADIUS, WYVERN_EXPLOSION_MIN_RADIUS));

    // 直下なら、半径を0に
    if (bFall == true)
    {
        fRadius = 0.0f;
    }

    // 位置を決める(Y軸を少し浮かせた位置で固定)
    D3DXVECTOR3 effectPos = DEFAULT_VECTOR;
    if (bUseTracking == false)
    {
        // 追従無し
        effectPos = D3DXVECTOR3(-sinf(fAngle)*fRadius + bodyPos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, -cosf(fAngle)*fRadius + bodyPos.z);
    }
    else
    {
        // 追従アリ
        D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
        effectPos = D3DXVECTOR3(playerPos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, playerPos.z);
    }

    // 爆砕の中心を発生
    CEffect3D::Create(effectPos, EFFECT_BURST_CENTER_FIRST_SIZE,
        DEFAULT_VECTOR, EFFECT_BURST_CENTER_FIRST_COLOR, HIT_TOP, EFFECT_BURST_CENTER_FADE_OUT_RATE, CEffect3D::TYPE_BURST_CENTER);
}

//=============================================================================
// 敵生成攻撃
// Author : 後藤慎之助
//=============================================================================
void CWyvern::SetEnemySpawn(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // 敵の総数を取得
    int nNumAllEnemy = CScene::GetNumAll(CScene::OBJTYPE_ENEMY);

    // 敵を生成できる総数なら
    if (nNumAllEnemy <= WYVERN_ROAR_SET_ENEMY_COUNT_BORDER)
    {
        // 角度を決め、同時に複数体生成
        float fAngle = rot.y;
        for (int nCnt = 0; nCnt < WYVERN_ROAR_SET_ENEMY_ONCE; nCnt++)
        {
            // エフェクト用の変数宣言
            float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;

            // 回転量を決める
            D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

            // 位置を決める
            D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*WYVERN_ROAR_SET_ENEMY_RADIUS,
                WYVERN_ROAR_SET_ENEMY_SPAWN_POS_Y, -cosf(fAngle)*WYVERN_ROAR_SET_ENEMY_RADIUS) + GetPos();

            // 敵生成のエフェクト発生
            CModelEffect::Create(effectPos, rotMove, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_KAMI);

            // 角度を足し、円状かつ均等に配置
            fAngle += D3DXToRadian(360.0f / (float)WYVERN_ROAR_SET_ENEMY_ONCE);
        }
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CWyvern::Draw(void)
{
    CCharacter::Draw();
}

//=============================================================================
// モデルデータ読み込み処理
// Author : 樋宮匠
//=============================================================================
HRESULT CWyvern::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/00_weist.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_WEIST], NULL, &m_nNumMat[PARTS_WEIST], &m_pMesh[PARTS_WEIST]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/01_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_BODY], NULL, &m_nNumMat[PARTS_BODY], &m_pMesh[PARTS_BODY]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/02_neck.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_NECK], NULL, &m_nNumMat[PARTS_NECK], &m_pMesh[PARTS_NECK]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/03_head.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_HEAD], NULL, &m_nNumMat[PARTS_HEAD], &m_pMesh[PARTS_HEAD]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/04_tail_root.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_TAIL_ROOT], NULL, &m_nNumMat[PARTS_TAIL_ROOT], &m_pMesh[PARTS_TAIL_ROOT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/05_tail_tip.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_TAIL_TIP], NULL, &m_nNumMat[PARTS_TAIL_TIP], &m_pMesh[PARTS_TAIL_TIP]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/06_arm_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_ARM_RIGHT], NULL, &m_nNumMat[PARTS_ARM_RIGHT], &m_pMesh[PARTS_ARM_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/07_arm_left.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_ARM_LEFT], NULL, &m_nNumMat[PARTS_ARM_LEFT], &m_pMesh[PARTS_ARM_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/08_wing_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_WING_RIGHT], NULL, &m_nNumMat[PARTS_WING_RIGHT], &m_pMesh[PARTS_WING_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/09_wing_left.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_WING_LEFT], NULL, &m_nNumMat[PARTS_WING_LEFT], &m_pMesh[PARTS_WING_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/10_leg_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEG_RIGHT], NULL, &m_nNumMat[PARTS_LEG_RIGHT], &m_pMesh[PARTS_LEG_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/11_foot_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_FOOT_RIGHT], NULL, &m_nNumMat[PARTS_FOOT_RIGHT], &m_pMesh[PARTS_FOOT_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/12_leg_left.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEG_LEFT], NULL, &m_nNumMat[PARTS_LEG_LEFT], &m_pMesh[PARTS_LEG_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/13_foot_left.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_FOOT_LEFT], NULL, &m_nNumMat[PARTS_FOOT_LEFT], &m_pMesh[PARTS_FOOT_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_HEAD], NULL, &m_nNumMat[PARTS_CUBE_HEAD], &m_pMesh[PARTS_CUBE_HEAD]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_TAIL], NULL, &m_nNumMat[PARTS_CUBE_TAIL], &m_pMesh[PARTS_CUBE_TAIL]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_ARM_RIGHT], NULL, &m_nNumMat[PARTS_CUBE_ARM_RIGHT], &m_pMesh[PARTS_CUBE_ARM_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_ARM_LEFT], NULL, &m_nNumMat[PARTS_CUBE_ARM_LEFT], &m_pMesh[PARTS_CUBE_ARM_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_WING_RIGHT], NULL, &m_nNumMat[PARTS_CUBE_WING_RIGHT], &m_pMesh[PARTS_CUBE_WING_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_WING_LEFT], NULL, &m_nNumMat[PARTS_CUBE_WING_LEFT], &m_pMesh[PARTS_CUBE_WING_LEFT]);

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_ELBOW_RIGHT], NULL, &m_nNumMat[PARTS_CUBE_ELBOW_RIGHT], &m_pMesh[PARTS_CUBE_ELBOW_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_ELBOW_LEFT], NULL, &m_nNumMat[PARTS_CUBE_ELBOW_LEFT], &m_pMesh[PARTS_CUBE_ELBOW_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_FOOT_RIGHT], NULL, &m_nNumMat[PARTS_CUBE_FOOT_RIGHT], &m_pMesh[PARTS_CUBE_FOOT_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_FOOT_LEFT], NULL, &m_nNumMat[PARTS_CUBE_FOOT_LEFT], &m_pMesh[PARTS_CUBE_FOOT_LEFT]);

    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 樋宮匠
//=============================================================================
void CWyvern::Unload(void)
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
CWyvern * CWyvern::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // メモリ確保
    CWyvern *pWyvern = NULL;
    pWyvern = new CWyvern;

    // 親元の情報を設定
    pWyvern->SetPos(pos);
    pWyvern->SetRot(rot);
    pWyvern->SetTribe(CCharacter::TRIBE_WYVERN);

    // 初期化
    pWyvern->Init(pos, DEFAULT_SCALE);

    pWyvern->SetActiveCollisionSize(WYVERN_ACTIVE_COLLISION_SIZE);
    pWyvern->SetLife(WYVERN_LIFE_MAX);
    pWyvern->m_nScore = WYVERN_SCORE;
    pWyvern->m_nOrbs = WYVERN_ORBS;
    pWyvern->m_fSpeed = WYVERN_SPEED;

    pWyvern->SetCntCollision(COLLISION_PARTS_MAX);
    pWyvern->SetCollisionSize(COLLISION_PARTS_HEAD_CUBE,       WYVERN_HEAD_CUBE_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_HEAD,            WYVERN_HEAD_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_BODY,            WYVERN_BODY_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_ARM_RIGHT,       WYVERN_ARM_RIGHT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_ARM_LEFT,        WYVERN_ARM_LEFT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_FOOT_RIGHT,      WYVERN_FOOT_RIGHT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_FOOT_LEFT,       WYVERN_FOOT_LEFT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_WING_RIGHT,      WYVERN_WING_RIGHT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_WING_LEFT,       WYVERN_WING_LEFT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_TAIL_ROOT,       WYVERN_TAIL_ROOT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_TAIL_TIP,        WYVERN_TAIL_TIP_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_TAIL_CUBE,       WYVERN_TAIL_CUBE_COLLISION_SIZE);

    pWyvern->SetCollisionSize(COLLISION_PARTS_ELBOW_RIGHT,     WYVERN_ELBOW_RIGHT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_ELBOW_LEFT,      WYVERN_ELBOW_LEFT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_FOOT_CUBE_RIGHT, WYVERN_FOOT_CUBE_RIGHT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_FOOT_CUBE_LEFT,  WYVERN_FOOT_CUBE_LEFT_COLLISION_SIZE);

    // 当たり判定の位置を更新
    pWyvern->UpdateCollisionPos();

    return pWyvern;
}

//=============================================================================
// 着地エフェクト
// Author : 後藤慎之助
//=============================================================================
void CWyvern::SetLandingEffect(D3DXVECTOR3 pos)
{
    // 音を取得
    CSound*pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_LAND);

    // 小さいガレキを飛ばす
    for (int nCnt = 0; nCnt < WYVERN_LANDING_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
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

        // 位置を決める
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*WYVERN_LANDING_CRACK_RADIUS,
            0.0f, -cosf(fAngle)*WYVERN_LANDING_CRACK_RADIUS) + pos;

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
}

//=============================================================================
// オーブをバラまく処理
// Author : 後藤慎之助
//=============================================================================
void CWyvern::ScatterOrbs(int nOrbs)
{
    // 位置を取得
    D3DXVECTOR3 pos = GetPos();

    // オーブ飛散エフェクト
    float fSpeedX = 0.0f;   //速さX
    float fSpeedY = 0.0f;   //速さY
    float fSpeedZ = 0.0f;   //速さZ
    int nPlusMinusX = 0;    //符号X
    int nPlusMinusZ = 0;    //符号Z
    for (int nCnt = 0; nCnt < nOrbs; nCnt++)
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
void CWyvern::DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // 叫び声
    CSound*pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_BOSS_DEATH);

	// パーツの位置と、モデルエフェクトの種類
	D3DXVECTOR3 partsPos = DEFAULT_VECTOR;
	CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_WYVERN_BODY;

	//==============================
	// 体
	//==============================

    // エフェクトの種類
    modelEffectType = CModelEffect::TYPE_WYVERN_BODY;

	// 位置
	partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._43);

	// エフェクトの発生
	SetDeathCrushEffect(partsPos, rot, modelEffectType);

	//==============================
	// 頭
	//==============================

    // エフェクトの種類
    modelEffectType = CModelEffect::TYPE_WYVERN_HEAD;

	// 位置
	partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._43);

	// エフェクトの発生
	SetDeathCrushEffect(partsPos, rot, modelEffectType);

	//==============================
	// 首
	//==============================
    // エフェクトの種類
    modelEffectType = CModelEffect::TYPE_WYVERN_NECK;

    // 位置
    partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_NECK)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_NECK)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_NECK)->GetWorldMtx()._43);

    // エフェクトの発生
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // 腕
    //==============================
    // エフェクトの種類
    modelEffectType = CModelEffect::TYPE_WYVERN_ARM;

    // 位置
    partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_ARM_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_ARM_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_ARM_RIGHT)->GetWorldMtx()._43);

    // エフェクトの発生
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // 右翼
    //==============================
    if (m_bDestroyRightWing == false)
    {
        // エフェクトの種類
        modelEffectType = CModelEffect::TYPE_RIGHT_WING;

        // 位置
        partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._41,
            GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._42,
            GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._43);

        // エフェクトの発生
        SetDeathCrushEffect(partsPos, rot, modelEffectType);
    }

    //==============================
    // 左翼
    //==============================
    if (m_bDestroyLeftWing == false)
    {
        // エフェクトの種類
        modelEffectType = CModelEffect::TYPE_LEFT_WING;

        // 位置
        partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._41,
            GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._42,
            GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._43);

        // エフェクトの発生
        SetDeathCrushEffect(partsPos, rot, modelEffectType);
    }

    //==============================
    // 尻尾
    //==============================
    if (m_bDestroyTail == false)
    {
        // エフェクトの種類
        modelEffectType = CModelEffect::TYPE_TAIL;

        // 位置
        partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._41,
            GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._42,
            GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._43);

        // エフェクトの発生
        SetDeathCrushEffect(partsPos, rot, modelEffectType);
    }
}

//=============================================================================
// やられた時の粉砕エフェクトの発生
// Author : 後藤慎之助
//=============================================================================
void CWyvern::SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type)
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
    fSpeedY = float(GetRandNum(MODEL_EFFECT_WYVERN_SPEED_Y_MAX, MODEL_EFFECT_WYVERN_SPEED_Y_MIN)) / 100.0f;
	fSpeedZ = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Z, 0)) / 100.0f;
	fRotSpeedX = float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f;
	fRotSpeedY = float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f;
	fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f;
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
// 攻撃を受けた部位によって、受けるダメージを変える
// Author : 後藤慎之助
//=============================================================================
void CWyvern::TakeDamage(D3DXVECTOR3 pos, int nDamage, int nCntCollision)
{
    // 実際のダメージを決める
    float fRealDamage = (float)nDamage;

    // 顔面に当たったなら
    if (nCntCollision == COLLISION_PARTS_HEAD_CUBE)
    {
        // 音を取得
        CSound*pSound = CManager::GetSound();
        pSound->Play(CSound::LABEL_SE_DAMAGE_CRYSTAL);

        // 一定の値より低いなら
        if (fRealDamage < WYVERN_HEAD_DAMAGE_UP_BORDER)
        {
            // ダメージ上昇
            fRealDamage = fRealDamage * WYVERM_HEAD_DAMAGE_UP_RATE;

            // エフェクト発生
            float fAngle = 0.0f;    //角度
            float fSpeed = EFFECT_HIT_WYVERN_HEAD_SPEED;   //速さ
            CCamera*pCamera = CManager::GetCamera();
            for (int nCnt = 0; nCnt < EFFECT_HIT_CRYSTAL_NUMBER_ONCE; nCnt++)
            {
                //0～3.14を出す*(0 or 1)*-1で正負を決める
                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

                // エフェクト発生
                CEffect3D::Create(pos, EFFECT_HIT_WYVERN_HEAD_SIZE, move, EFFECT_HIT_CRYSTAL_COLOR,
                    DEFAULT_VECTOR, EFFECT_HIT_WYVERN_HEAD_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
            }
        }

        // スタンしていないなら
        if (m_bStun == false)
        {
            // ダメージが一定より高いなら
            if (fRealDamage >= WYVERN_STUN_VALUE_UP_BORDER)
            {
                // 確定スタン
                m_nStunValue += WYVERN_STUN_VALUE_MAX;
            }
            else
            {
                // スタン値上昇
                m_nStunValue += WYVERN_STUN_ADD_VALUE_HEAD;
            }
        }
    }
    // 左翼なら
    else if (nCntCollision == COLLISION_PARTS_WING_LEFT)
    {
        // 部位耐久値減少
        m_nLeftWingDurabirity -= (int)fRealDamage;
    }
    // 右翼なら
    else if (nCntCollision == COLLISION_PARTS_WING_RIGHT)
    {
        // 部位耐久値減少
        m_nRightWingDurabirity -= (int)fRealDamage;
    }
    // 尻尾なら
    else if (nCntCollision == COLLISION_PARTS_TAIL_CUBE || nCntCollision == COLLISION_PARTS_TAIL_TIP)
    {
        // 部位耐久値減少
        m_nTailDurabirity -= (int)fRealDamage;
    }
    // 足なら
    else if (nCntCollision == COLLISION_PARTS_FOOT_CUBE_LEFT || nCntCollision == COLLISION_PARTS_FOOT_LEFT ||
        nCntCollision == COLLISION_PARTS_FOOT_CUBE_RIGHT || nCntCollision == COLLISION_PARTS_FOOT_RIGHT)
    {
        // スタンしていないなら
        if (m_bStun == false)
        {
            // ダメージが一定より高いなら
            if (fRealDamage >= WYVERN_STUN_VALUE_UP_BORDER)
            {
                // 確定スタン
                m_nStunValue += WYVERN_STUN_VALUE_MAX;
            }
            else
            {
                // スタン値上昇
                m_nStunValue += WYVERN_STUN_ADD_VALUE_FOOT;
            }
        }
    }

    // 実際にダメージを受ける(どのダメージも、プレイヤーのものとして扱う)
    SetTakeDamage((int)fRealDamage, CCharacter::LAST_HIT_PLAYER);
}

//=============================================================================
// ミッションクリア数に加算する
// Author : 後藤慎之助
//=============================================================================
void CWyvern::AddMissionClear(void)
{
    // ステージと現在のミッションに応じて、クリア数を増加
    CGame::STAGE stage = CGame::GetStage();
    CGame::CURRENT_MISSION currentMission = CGame::GetCurrentMission();
    switch (stage)
    {
    case CGame::STAGE_1:

        break;

    case CGame::STAGE_2:

        break;

    case CGame::STAGE_FINAL:

        switch (currentMission)
        {
        case CGame::CURRENT_MISSION_1:

            break;

        case CGame::CURRENT_MISSION_2:
            CGame::SetClearNumMission2();
            break;
        }

        break;
    }
}