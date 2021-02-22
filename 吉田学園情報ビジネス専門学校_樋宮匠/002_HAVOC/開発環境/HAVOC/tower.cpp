//================================================================================
//
// タワーの処理 (tower.cpp)
// Author : 後藤慎之助(行動AI、エフェクト) / 樋宮匠(階層構造、アニメーション制御)
//
//================================================================================

//========================
// インクルードファイル
//========================
#include "tower.h"
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
LPD3DXMESH CTower::m_pMesh[PARTS_MAX] = {};
LPD3DXBUFFER CTower::m_pBuffMat[PARTS_MAX] = {};
DWORD CTower::m_nNumMat[PARTS_MAX] = {};

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CTower::CTower() :CCharacter(OBJTYPE::OBJTYPE_ENEMY)
{
    m_move = DEFAULT_VECTOR;
    m_nScore = 0;
    m_nOrbs = 0;

    m_nCntTime = 0;

    m_rotCrystal = DEFAULT_VECTOR;
    m_bLanding = false;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CTower::~CTower()
{
}

//=============================================================================
// 初期化処理
// Author : 樋宮匠
//=============================================================================
HRESULT CTower::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // パーツ数を設定
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_TOWER, nCount, m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
    }

    // 座標・親子関係の読み込み
    LoadModelData("./data/ANIMATION/motion_tower.txt");

    // アニメーションをさせない
    CCharacter::SetUnableAnimation();

    // ボス仕様に
    CCharacter::SetBossSetting();

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CTower::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CTower::Update(void)
{
    // 行動
    Movement();

    // クリスタルを回転させる
    m_rotCrystal.y += TOWER_CRYSTAL_ROT_SPEED;
    if (m_rotCrystal.y >= D3DXToRadian(360.0f))
    {
        m_rotCrystal.y = D3DXToRadian(0);
    }
    // 回転の更新
    CCharacter::GetParts(PARTS_CRYSTAL)->SetRot(m_rotCrystal);

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
    	ScatterOrbs();

    	// スコア加算
    	CScore *pScore = CGame::GetScore();
    	pScore->AddScore(m_nScore);

    	// 一定確率で、回復アイテムを落とす
    	int nRandNum = GetRandNum(100, 1);
    	if (nRandNum <= TOWER_DROP_HEAL_ITEM)
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
void CTower::Movement(void)
{
	// 位置と向きを取得
	D3DXVECTOR3 pos = DEFAULT_VECTOR;
	D3DXVECTOR3 rot = DEFAULT_VECTOR;
	pos = GetPos();
	rot = GetRot();

	// 1F前の位置を記憶
	SetPosOld(pos);

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

        // 着地
        if (m_bLanding == false)
        {
            SetLandingEffect(pos);
            m_bLanding = true;
        }
	}

    // 着地しているなら
    if (m_bLanding == true)
    {
        // カウントアップ
        m_nCntTime++;

        // 一定カウンタで、敵生成
        if (m_nCntTime >= TOWER_SET_ENEMY_SPAWN_COUNT)
        {
            // カウンタリセット
            m_nCntTime = 0;

            SetEnemySpawn();
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
#endif // COLLISION_TEST
}

//=============================================================================
// 敵生成地点を設定
// Author : 後藤慎之助
//=============================================================================
void CTower::SetEnemySpawn(void)
{
    // 敵の総数を取得
    int nNumAllEnemy = CScene::GetNumAll(CScene::OBJTYPE_ENEMY);

    // 敵を生成できる総数なら
    if (nNumAllEnemy < TOWER_SET_ENEMY_COUNT_BORDER)
    {
        // エフェクト用の変数宣言
        float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;

        // 回転量を決める
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        // 0～3.14を出す*(0 or 1)*-1で正負を決める
        float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // 位置を決める
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*TOWER_ATTACK_RADIUS,
            TOWER_ENEMY_SPAWN_POS_Y, -cosf(fAngle)*TOWER_ATTACK_RADIUS) + GetPos();

        // 敵生成のエフェクト発生
        CModelEffect::Create(effectPos, rotMove, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_KAMI);
    }
}

//=============================================================================
// 当たり判定の位置を更新
// Author : 後藤慎之助
//=============================================================================
void CTower::UpdateCollisionPos(void)
{
	// 柱の位置取得
	D3DXVECTOR3 pillerPos = D3DXVECTOR3(GetParts(CTower::PARTS_PILLER)->GetWorldMtx()._41,
		GetParts(CTower::PARTS_PILLER)->GetWorldMtx()._42,
		GetParts(CTower::PARTS_PILLER)->GetWorldMtx()._43);

	// クリスタルの位置取得
	D3DXVECTOR3 crystalPos = D3DXVECTOR3(GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._41,
		GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._42,
		GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._43);

	// 当たり判定の位置を更新
    SetCollisionPos(PARTS_PILLER, pillerPos);
    SetCollisionPos(PARTS_CRYSTAL, crystalPos);
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CTower::Draw(void)
{
    CCharacter::Draw();
}

//=============================================================================
// モデルデータ読み込み処理
// Author : 樋宮匠
//=============================================================================
HRESULT CTower::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/tower/tower_piller.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_PILLER], NULL, &m_nNumMat[PARTS_PILLER], &m_pMesh[PARTS_PILLER]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/tower/tower_crystal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CRYSTAL], NULL, &m_nNumMat[PARTS_CRYSTAL], &m_pMesh[PARTS_CRYSTAL]);

    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 樋宮匠
//=============================================================================
void CTower::Unload(void)
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
CTower * CTower::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // メモリ確保
    CTower *pTower = NULL;
    pTower = new CTower;

    // 親元の情報を設定
    pTower->SetPos(pos);
    pTower->SetRot(rot);
    pTower->SetTribe(CCharacter::TRIBE_TOWER);

    pTower->Init(pos, DEFAULT_SCALE);

    pTower->SetActiveCollisionSize(TOWER_ACTIVE_COLLISION_SIZE);
    pTower->SetLife(TOWER_LIFE);
    pTower->m_nScore = TOWER_SCORE;
    pTower->m_nOrbs = TOWER_ORBS;

    pTower->SetCntCollision(TOWER_COLLISION_MAX);
    pTower->SetCollisionSize(PARTS_PILLER, TOWER_PILLER_COLLISION_SIZE);
    pTower->SetCollisionSize(PARTS_CRYSTAL, TOWER_CRYSTAL_COLLISION_SIZE);

    // 当たり判定の位置を更新
    pTower->UpdateCollisionPos();

    return pTower;
}

//=============================================================================
// 着地エフェクト
// Author : 後藤慎之助
//=============================================================================
void CTower::SetLandingEffect(D3DXVECTOR3 pos)
{
    // 音を取得
    CSound*pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_LAND);

    // 小さいガレキを飛ばす
    for (int nCnt = 0; nCnt < TOWER_LANDING_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
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
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*TOWER_LANDING_CRACK_RADIUS,
            0.0f, -cosf(fAngle)*TOWER_LANDING_CRACK_RADIUS) + pos;

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
void CTower::ScatterOrbs(void)
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
void CTower::DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    //クリスタル破壊音
    CSound*pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_DESTROY_CRYSTAL);

	// パーツの位置と、モデルエフェクトの種類
	CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_CRYSTAL;

    // クリスタルの位置取得
    D3DXVECTOR3 crystalPos = D3DXVECTOR3(GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._41,
        GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._42,
        GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._43);

	// エフェクトの発生
	SetDeathCrushEffect(crystalPos, rot, modelEffectType);

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
        fRadius = float(GetRandNum((int)TOWER_LANDING_CRACK_RADIUS * MODEL_EFFECT_BIG_COLLAPSE_RADIUS, 0)) / 100.0f;

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
// 崩壊エフェクト
// Author : 後藤慎之助
//=============================================================================
void CTower::CollapseEffect(void)
{
    // クリスタルの位置取得
    D3DXVECTOR3 crystalPos = D3DXVECTOR3(GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._41,
        GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._42,
        GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._43);

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
        fPosX = float(GetRandNum((int)TOWER_LANDING_CRACK_RADIUS, 0)) - float(GetRandNum((int)TOWER_LANDING_CRACK_RADIUS, 0));
        fPosZ = float(GetRandNum((int)TOWER_LANDING_CRACK_RADIUS, 0)) - float(GetRandNum((int)TOWER_LANDING_CRACK_RADIUS, 0));

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
        CModelEffect::Create(GetPos() + D3DXVECTOR3(fPosX, crystalPos.y, fPosZ), rotMove, DEFAULT_VECTOR, rotMove, modelEffectType);
    }
}

//=============================================================================
// やられた時の粉砕エフェクトの発生
// Author : 後藤慎之助
//=============================================================================
void CTower::SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type)
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
// 攻撃を受けた部位によって、受けるダメージを変える
// Author : 後藤慎之助
//=============================================================================
void CTower::TakeDamage(D3DXVECTOR3 pos, int nDamage, PARTS parts)
{
    // 実際のダメージを決める
    float fRealDamage = (float)nDamage;

    // 柱に当たったなら
    if (parts == PARTS_PILLER)
    {
        // ダメージ軽減
        fRealDamage = fRealDamage * TOWER_PILLER_REDUCTION_RATE;
    }
    // クリスタルに当たったなら
    else if (parts == PARTS_CRYSTAL)
    {
        // 音を取得
        CSound*pSound = CManager::GetSound();
        pSound->Play(CSound::LABEL_SE_DAMAGE_CRYSTAL);

        // ダメージ上昇
        fRealDamage = fRealDamage * TOWER_CRYSTAL_DAMAGE_UP_RATE;

        // エフェクト発生
        float fAngle = 0.0f;    //角度
        float fSpeed = EFFECT_HIT_CRYSTAL_SPEED;   //速さ
        CCamera*pCamera = CManager::GetCamera();
        for (int nCnt = 0; nCnt < EFFECT_HIT_CRYSTAL_NUMBER_ONCE; nCnt++)
        {
            //0～3.14を出す*(0 or 1)*-1で正負を決める
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

            // エフェクト発生
            CEffect3D::Create(pos, EFFECT_HIT_CRYSTAL_SIZE, move, EFFECT_HIT_CRYSTAL_COLOR,
                DEFAULT_VECTOR, EFFECT_HIT_CRYSTAL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }
    }

    // 実際にダメージを受ける(どのダメージも、プレイヤーのものとして扱う)
    SetTakeDamage((int)fRealDamage, CCharacter::LAST_HIT_PLAYER);
}

//=============================================================================
// ミッションクリア数に加算する
// Author : 後藤慎之助
//=============================================================================
void CTower::AddMissionClear(void)
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

            break;

        case CGame::CURRENT_MISSION_2:
            CGame::SetClearNumMission2();
            break;
        }

        break;

    case CGame::STAGE_FINAL:

        break;
    }
}