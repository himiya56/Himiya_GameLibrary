//======================================================================================
//
// プレイヤー処理 (player.cpp)
// Author : 後藤慎之助(行動、モーション制御) / 樋宮匠(階層構造、モーション作成)
//
//======================================================================================

//========================
// インクルードファイル
//========================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "library.h"
#include "block.h"
#include "bullet.h"
#include "camera.h"
#include "effect3d.h"
#include "character.h"
#include "debug.h"
#include "animation.h"
#include "item.h"
#include "game.h"
#include "bulletRemain.h"
#include "modelEffect.h"
#include "timer.h"
#include "sound.h"

//========================
// 静的メンバ変数宣言
//========================
LPD3DXMESH CPlayer::m_pMesh[PLAYABLE_MAX][PARTS_MAX] = {};
LPD3DXBUFFER CPlayer::m_pBuffMat[PLAYABLE_MAX][PARTS_MAX] = {};
DWORD CPlayer::m_nNumMat[PLAYABLE_MAX][PARTS_MAX] = {};

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CPlayer::CPlayer() :CCharacter(OBJTYPE::OBJTYPE_PLAYER)
{
    m_playable = PLAYABLE_001;
    m_move = DEFAULT_VECTOR;

    m_nAttackCoolTime = 0;
    m_bJump = true;
    m_rotDest = DEFAULT_VECTOR;
    m_rotMin = DEFAULT_VECTOR;

    m_nCntDeath = 0;

    m_bDisp = true;
    m_nCntInvincibleFrame = 0;
    m_bBlinking = false;

    m_bWallRun = false;

    m_nRemainBullets = PLAYER_REMAIN_BULLETS_MAX;
    m_bReloading = false;

    m_nCntLandingTime = 0;
    m_bJumpOld = true;

    m_nCntWallKickTime = 0;
    m_bWallKickType = false;

    m_nLeftHandOrbs = PLAYER_LEFT_HAND_ORBS_START;
    m_leftHandAttack = LEFT_HAND_ATTACK_NONE;

    m_nCntFootsteps = 0;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// 初期化処理
// Author : 樋宮匠
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // パーツ数を設定
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_PLAYER, nCount, m_pMesh[m_playable][nCount], m_pBuffMat[m_playable][nCount], m_nNumMat[m_playable][nCount]);
    }

    // 座標・親子関係の読み込み
    LoadModelData("./data/ANIMATION/motion_player.txt");

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Update(void)
{
    // タイムアップしていないなら
    if (CTimer::GetTimeUp() == false)
    {
        // 表示しているなら
        if (m_bDisp == true)
        {
            // 無敵時間があるなら、カウンタを下げる
            if (m_nCntInvincibleFrame > 0)
            {
                m_nCntInvincibleFrame--;

                // 点滅
                m_bBlinking = !m_bBlinking;
            }
            else
            {
                // 点滅しない
                m_bBlinking = false;
            }

            // 攻撃のクールタイム
            if (m_nAttackCoolTime > 0)
            {
                m_nAttackCoolTime--;

                // 各行動の終わりを設定
                if (m_nAttackCoolTime == 0)
                {
                    // リロード中なら
                    if (m_bReloading == true)
                    {
                        // 残弾数を最大に
                        m_nRemainBullets = PLAYER_REMAIN_BULLETS_MAX;

                        // 残弾数表示を更新
                        CGame::GetBulletRemain()->Disp(m_nRemainBullets);

                        m_bReloading = false;
                    }

                    // 左手攻撃中なら
                    if (m_leftHandAttack != LEFT_HAND_ATTACK_NONE)
                    {
                        // 左手攻撃終了
                        m_leftHandAttack = LEFT_HAND_ATTACK_NONE;
                    }
                }
            }

            // 行動
            float fSpeed = PLAYER_BASE_SPEED;    // 基本の速さ
            if (m_bWallRun == false && m_bJump == false)
            {
                // 空中の制御速度を、壁キックの有無で決定
                if (m_nCntWallKickTime > 0)
                {
                    // 壁キック後は、やや早い
                    fSpeed = PLAYER_CONTROL_SPEED_IN_AIR_AFTER_WALL_KICK;
                }
                else
                {
                    // 通常
                    fSpeed = PLAYER_CONTROL_SPEED_IN_AIR;
                }
            }
            Movement(m_playable, fSpeed);

            // リロード中のエフェクト発生
            if (m_bReloading == true)
            {
                SetReloadingEffect();
            }

            // アニメーションさせる
            CCharacter::Update();

            // ライフがなくなったら、非表示に
            if (GetLife() <= 0)
            {
                m_bDisp = false;
            }
        }
        // 体力がないなら
        else
        {
            // ゲームが通常なら
            if (CGame::GetState() == CGame::STATE_NORMAL)
            {
                // やられた時のエフェクト
                DeathCrush(GetPos(), GetRot());

                // ゲームの状態を終わりにする
                CGame::SetState(CGame::STATE_FINISH);
            }
        }
    }
    // タイムアップしているなら
    else
    {
        // 表示
        m_bDisp = true;
        m_bBlinking = false;

        // デスカウンタ
        m_nCntDeath++;

        // 一定カウンタで棘生成
        if (m_nCntDeath % MODEL_EFFECT_SPIKE_SPAWN_RATE == 0)
        {
            // 体の位置取得
            D3DXVECTOR3 bodyPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._41,
                GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._42,
                GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._43);

            // 向き取得
            float fAngleX = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            float fAngleY = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            float fAngleZ = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            D3DXVECTOR3 rot = D3DXVECTOR3(fAngleX, fAngleY, fAngleZ);

            // エフェクト発生
            CModelEffect::Create(bodyPos, rot, DEFAULT_VECTOR, DEFAULT_VECTOR, CModelEffect::TYPE_SPIKE);
        }

        // モーションをデスにする
        GetAnimation()->SetAnimation(ANIM_DEATH);

        // アニメーションさせる
        CCharacter::Update();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Draw(void)
{
    // 表示するなら、描画
    if (m_bDisp == true)
    {
        // 点滅していないなら描画
        if (m_bBlinking == false)
        {
            CCharacter::Draw();
        }
    }
}

//=============================================================================
// モデルデータ読み込み処理
// Author : 樋宮匠
//=============================================================================
HRESULT CPlayer::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // プレイヤー1
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/00_bodyD.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_HIP], NULL, &m_nNumMat[PLAYABLE_001][PARTS_HIP], &m_pMesh[PLAYABLE_001][PARTS_HIP]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/01_bodyU.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_BODY], NULL, &m_nNumMat[PLAYABLE_001][PARTS_BODY], &m_pMesh[PLAYABLE_001][PARTS_BODY]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/02_head.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_HEAD], NULL, &m_nNumMat[PLAYABLE_001][PARTS_HEAD], &m_pMesh[PLAYABLE_001][PARTS_HEAD]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/03_armUR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RSHOULDER], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RSHOULDER], &m_pMesh[PLAYABLE_001][PARTS_RSHOULDER]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/04_armDR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RARM], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RARM], &m_pMesh[PLAYABLE_001][PARTS_RARM]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/05_handR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RHAND], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RHAND], &m_pMesh[PLAYABLE_001][PARTS_RHAND]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/06_armUL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LSHOULDER], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LSHOULDER], &m_pMesh[PLAYABLE_001][PARTS_LSHOULDER]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/07_armDL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LARM], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LARM], &m_pMesh[PLAYABLE_001][PARTS_LARM]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/08_handL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LHAND], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LHAND], &m_pMesh[PLAYABLE_001][PARTS_LHAND]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/09_legUR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RTHIGH], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RTHIGH], &m_pMesh[PLAYABLE_001][PARTS_RTHIGH]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/10_legDR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RLEG], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RLEG], &m_pMesh[PLAYABLE_001][PARTS_RLEG]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/11_footR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RFOOT], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RFOOT], &m_pMesh[PLAYABLE_001][PARTS_RFOOT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/12_legUL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LTHIGH], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LTHIGH], &m_pMesh[PLAYABLE_001][PARTS_LTHIGH]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/13_legDL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LLEG], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LLEG], &m_pMesh[PLAYABLE_001][PARTS_LLEG]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/14_footL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LFOOT], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LFOOT], &m_pMesh[PLAYABLE_001][PARTS_LFOOT]);

    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 樋宮匠
//=============================================================================
void CPlayer::Unload(void)
{
    for (int nCountType = 0; nCountType < PLAYABLE_MAX; nCountType++)
    {
        for (int nCount = 0; nCount < PARTS_MAX; nCount++)
        {
            if (m_pBuffMat[nCountType][nCount] != NULL)
            {
                m_pBuffMat[nCountType][nCount] = NULL;
            }

            if (m_nNumMat[nCountType][nCount] != NULL)
            {
                m_nNumMat[nCountType][nCount] = NULL;
            }

            if (m_pMesh[nCountType][nCount] != NULL)
            {
                m_pMesh[nCountType][nCount] = NULL;
            }
        }
    }
}

//=============================================================================
// インスタンス生成
// Author : 後藤慎之助
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PLAYABLE playable)
{
    // メモリ確保
    CPlayer *pPlayer = NULL;
    pPlayer = new CPlayer;

    // 親元の情報を設定
    pPlayer->SetPos(pos);
    pPlayer->SetRot(rot);
    pPlayer->SetTribe(CCharacter::TRIBE_PLAYER);
    pPlayer->SetActiveCollisionSize(PLAYER_COLLISION_SIZE);
    pPlayer->SetLife(PLAYER_MAX_LIFE);

    // 読み込む種類の設定
    pPlayer->m_playable = playable;

    // 初期化
    pPlayer->Init(pos, DEFAULT_SCALE);

    return pPlayer;
}

//=============================================================================
// リロード中の吸収エフェクト
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SetReloadingEffect(void)
{
    // 右手の位置取得
    D3DXVECTOR3 rightHandPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._41,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._42,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._43);

    // リロードエフェクト用の変数宣言
    float fAngle = 0.0f;                       // 角度
    float fSize = 0.0f;                        // 大きさ
    CCamera *pCamera = CManager::GetCamera();  // カメラを取得

    // 0～3.14を出す*(0 or 1)*-1で正負を決める
    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

    // 位置を決める
    D3DXVECTOR3 pos = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*EFFECT_RELOADING_RADIUS,
        0.0f, -cosf(fAngle + pCamera->GetAngle())*EFFECT_RELOADING_RADIUS) + rightHandPos;

    // 大きさを出す
    fSize = float(GetRandNum(EFFECT_RELOADING_MAX_SIZE, EFFECT_RELOADING_MIN_SIZE)) / 100.0f;

    // 大きさを決める
    D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

    // エフェクト発生
    CEffect3D::Create(pos, size, DEFAULT_VECTOR, EFFECT_RELOADING_COLOR,
        DEFAULT_VECTOR, EFFECT_RELOADING_FADE_OUT_RATE, CEffect3D::TYPE_RELOADING);

    // エフェクト発生
    CEffect3D::Create(pos, size * EFFECT_RELOADING_CENTER_REDUCE_SIZE_RATE, DEFAULT_VECTOR, EFFECT_RELOADING_CENTER_COLOR,
        DEFAULT_VECTOR, EFFECT_RELOADING_FADE_OUT_RATE, CEffect3D::TYPE_RELOADING);
}

//=============================================================================
// 射撃時の飛散エフェクト
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SetScatteringBullet(void)
{
    // 右手の位置取得
    D3DXVECTOR3 rightHandPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._41,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._42,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._43);

    // 射撃エフェクト用の変数宣言
    float fAngle = 0.0f;                       // 角度
    float fSpeed = 0.0f;                       // 速さ
    float fSize = 0.0f;                        // 大きさ
    CCamera *pCamera = CManager::GetCamera();  // カメラを取得
    for (int nCnt = 0; nCnt < EFFECT_SCATTERING_BULLET_NUMBER_ONCE; nCnt++)
    {
        // 0～3.14を出す*(0 or 1)*-1で正負を決める
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // 速さを出す
        fSpeed = float(GetRandNum(EFFECT_SCATTERING_BULLET_MAX_SPEED, EFFECT_SCATTERING_BULLET_MIN_SPEED)) / 100.0f;

        // 移動量を決める
        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed, 
            cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

        // 大きさを出す
        fSize = float(GetRandNum(EFFECT_SCATTERING_BULLET_MAX_SIZE, EFFECT_SCATTERING_BULLET_MIN_SIZE)) / 100.0f;

        // 大きさを決める
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // エフェクト発生
        CEffect3D::Create(rightHandPos, size, move, EFFECT_SCATTERING_BULLET_COLOR,
            DEFAULT_VECTOR, EFFECT_SCATTERING_BULLET_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
    }
}

//=============================================================================
// 着地時の地割れエフェクト
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SetLandingCrack(D3DXVECTOR3 pos)
{
    // 地面がひび割れる音
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_CRACK);

    // 4種類の地割れのうち、どれか一つを使う
    int nRandNum = GetRandNum(4, 1);
    switch (nRandNum)
    {
    case 1:
        // 地割れ1発生
        CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS + pos.y, pos.z), EFFECT_CRACK_SIZE,
            DEFAULT_VECTOR, DEFAULT_COLOR, HIT_TOP, EFFECT_CRACK_FADE_OUT_RATE, CEffect3D::TYPE_CRACK1);
        break;
    case 2:
        // 地割れ2発生
        CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS + pos.y, pos.z), EFFECT_CRACK_SIZE,
            DEFAULT_VECTOR, DEFAULT_COLOR, HIT_TOP, EFFECT_CRACK_FADE_OUT_RATE, CEffect3D::TYPE_CRACK2);
        break;
    case 3:
        // 地割れ3発生
        CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS + pos.y, pos.z), EFFECT_CRACK_SIZE,
            DEFAULT_VECTOR, DEFAULT_COLOR, HIT_TOP, EFFECT_CRACK_FADE_OUT_RATE, CEffect3D::TYPE_CRACK3);
        break;
    case 4:
        // 地割れ4発生
        CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS + pos.y, pos.z), EFFECT_CRACK_SIZE,
            DEFAULT_VECTOR, DEFAULT_COLOR, HIT_TOP, EFFECT_CRACK_FADE_OUT_RATE, CEffect3D::TYPE_CRACK4);
        break;
    }

    // 小さいガレキを飛ばす
    for (int nCnt = 0; nCnt < MODEL_EFFECT_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
    {
        // 変数宣言
        float fSpeedY = float(GetRandNum(MODEL_EFFECT_LANDING_CRACK_SPEED_Y_MAX, MODEL_EFFECT_LANDING_CRACK_SPEED_Y_MIN)) / 100.0f;
        float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

        // 移動量を決める
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f,
            fSpeedY, 0.0f);

        // 回転量を決める
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        float fAngle = 0.0f;                       // 角度

        // 0～3.14を出す*(0 or 1)*-1で正負を決める
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // 位置を決める
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*MODEL_EFFECT_LANDING_CRACK_RADIUS,
            0.0f, -cosf(fAngle)*MODEL_EFFECT_LANDING_CRACK_RADIUS) + pos;

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
// 壁キックエフェクト
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SetWallKickEffect(D3DXVECTOR3 pos)
{
    // 壁キック音
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_KICK);

    for (int nCnt = 0; nCnt < MODEL_EFFECT_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
    {
        // 変数宣言
        float fSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
        float fSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
        float fSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
        float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

        // 移動量を決める(カメラのほうに飛んできているイメージ)
        CCamera *pCamera = CManager::GetCamera();
        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(pCamera->GetAngle())*fSpeedX,
            fSpeedY, -cosf(pCamera->GetAngle())*fSpeedZ);

        // 回転量を決める
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

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
        CModelEffect::Create(pos, rotMove, move, rotMove, modelEffectType);
    }

    // 無生物へのパーティクルを用いたヒットエフェクト用の変数宣言
    float fAngle = 0.0f;                       // 角度
    float fSpeed = 0.0f;                       // 速さ
    float fSize = 0.0f;                        // 大きさ
    CCamera *pCamera = CManager::GetCamera();  // カメラを取得
    for (int nCnt = 0; nCnt < EFFECT_HIT_NON_LIVING_NUMBER_ONCE; nCnt++)
    {
        // 0～3.14を出す*(0 or 1)*-1で正負を決める
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // 速さを出す
        fSpeed = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SPEED, EFFECT_HIT_NON_LIVING_MIN_SPEED)) / 100.0f;

        // 移動量を決める
        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed,
            cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

        // 大きさを出す
        fSize = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SIZE, EFFECT_HIT_NON_LIVING_MIN_SIZE)) / 100.0f;

        // 大きさを決める
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // エフェクト発生
        CEffect3D::Create(pos, size, move, EFFECT_HIT_NON_LIVING_COLOR,
            DEFAULT_VECTOR, EFFECT_HIT_NON_LIVING_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
    }
}

//=============================================================================
// 行動
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Movement(int nCnt, float fSpeed)
{
    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(nCnt);

    // スティックが倒れているならスティックの角度を記憶
    float fAngleStick = STICK_NEUTRAL;
    if (Controller.lY != 0 || Controller.lX != 0)
    {
        fAngleStick = atan2(Controller.lX, Controller.lY*-1);
    }

    // カメラを取得
    CCamera *pCamera = CManager::GetCamera();

    // キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // 音を取得
    CSound *pSound = CManager::GetSound();

    // 位置と向きを取得
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    D3DXVECTOR3 rot = DEFAULT_VECTOR;
    pos = GetPos();
    rot = GetRot();

    // 1F前の位置を記憶
    SetPosOld(pos);

    // 横の移動量制御
    if (m_move.x != 0.0f || m_move.z != 0.0f)
    {
        // ジャンプできるなら
        if (m_bJump == true)
        {
            m_move.x *= PLAYER_CONTROL_MOVE;
            m_move.z *= PLAYER_CONTROL_MOVE;
        }
        // ジャンプできないなら、空中での制御
        else
        {
            m_move.x *= PLAYER_CONTROL_MOVE_IN_AIR;
            m_move.z *= PLAYER_CONTROL_MOVE_IN_AIR;
        }
    }

    // モーションをまずは待機にする
    GetAnimation()->SetAnimation(ANIM_IDLE);

    // プレイヤー移動処理
    // ゲームがフェード中以外なら
    if (CGame::GetState() != CGame::STATE_IN_FADE)
    {
        Control(nCnt, fSpeed);
    }

    // 回転制御
    rot = RotControl(rot);

    // 移動量と位置を結びつける
    pos += m_move;

    // 地面より上なら
    if (pos.y > 0.0f)
    {
        // 重力
        m_move.y -= GRAVITY;

        // ジャンプ不可
        m_bJump = false;
    }
    else
    {
        // 地面にいるなら、重力をかけないでジャンプ可能
        pos.y = 0.0f;
        m_move.y = 0.0f;
        m_bJump = true;
    }

    // ブロックとの当たり判定
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BLOCK);
    m_bWallRun = false; // 一度フラグをリセット
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BLOCK); nCntScene++)
    {
        // 中身があるなら
        if (pScene != NULL)
        {
            // 次のシーンを記憶
            CScene*pNextScene = pScene->GetNextScene();

            // ブロックにキャスト
            CBlock *pBlock = (CBlock*)pScene;

            // ブロックとの当たり判定を計算
            BLOCK_HIT_INFO blockHitInfo;
            blockHitInfo.hitSurface = HIT_SURFACE_NONE;
            blockHitInfo.pos = pos;
            blockHitInfo = (BlockCollision3D(&pos, &GetPosOld(), &pBlock->GetPos(),
                &GetActiveCollisionSize(), &pBlock->GetCollisionSize()));

            // ブロックの当たり判定で計算した位置を、結びつける
            pos = blockHitInfo.pos;

            // ヒット面よって、壁走りやジャンプの仕様を変える
            //=====================================================================================
            switch (blockHitInfo.hitSurface)
            {
            case HIT_SURFACE_TOP:

                m_move.y = 0.0f;
                m_bJump = true;

                break;

            case HIT_SURFACE_LEFT:

                // 壁走りできるなら
                if (pBlock->GetWallRun() == true)
                {
                    // リロードと左手攻撃をしていないなら
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // 前に進んでいるなら
                        if (STICK_UP(fAngleStick) || pInputKeyboard->GetKeyboardPress(DIK_W))
                        {
                            // 壁走りのフラグがtrue
                            m_bWallRun = true;

                            // 壁走り
                            pos.y += PLAYER_WALL_RAN_SPEED;

                            // 重力無視
                            m_move.y = 0.0f;

                            // 傾く
                            rot.x += PLAYER_TILT_SPEED;
                        }
                    }
                }

                // 壁にめり込まない
                m_move.x = 0.0f;
                m_move.z = 0.0f;

                // ジャンプボタンが押されたら
                if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_L2) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))
                {
                    // リロードと左手攻撃をしていないなら
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // キックジャンプ
                        m_move.y = PLAYER_WALL_KICK_JUMP_VALUE;

                        // 壁キック
                        pos.x -= PLAYER_WALL_KICK_VALUE;
                        m_move.x = -PLAYER_WALL_KICK_VALUE;

                        // 壁にダメージ
                        pBlock->SetTakeDamage(BLOCK_TAKE_DAMAGE_BY_WALL_KICK, CBlock::LAST_HIT_PLAYER);

                        // 壁キックエフェクト
                        SetWallKickEffect(pos);

                        // 壁キックカウンタ
                        m_nCntWallKickTime = PLAYER_WALL_KICK_TIME;

                        // 壁キックの種類を反転
                        m_bWallKickType = !m_bWallKickType;
                    }
                }

                break;

            case HIT_SURFACE_BACK:

                // 壁走りできるなら
                if (pBlock->GetWallRun() == true)
                {
                    // リロードと左手攻撃をしていないなら
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // 前に進んでいるなら
                        if (STICK_UP(fAngleStick) || pInputKeyboard->GetKeyboardPress(DIK_W))
                        {
                            // 壁走りのフラグがtrue
                            m_bWallRun = true;

                            // 壁走り
                            pos.y += PLAYER_WALL_RAN_SPEED;

                            // 重力無視
                            m_move.y = 0.0f;

                            // 傾く
                            rot.x += PLAYER_TILT_SPEED;
                        }
                    }
                }

                // 壁にめり込まない
                m_move.x = 0.0f;
                m_move.z = 0.0f;

                // ジャンプボタンが押されたら
                if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_L2) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))
                {
                    // リロードと左手攻撃をしていないなら
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // キックジャンプ
                        m_move.y = PLAYER_WALL_KICK_JUMP_VALUE;

                        // 壁キック
                        pos.z += PLAYER_WALL_KICK_VALUE;
                        m_move.z = PLAYER_WALL_KICK_VALUE;

                        // 壁にダメージ
                        pBlock->SetTakeDamage(BLOCK_TAKE_DAMAGE_BY_WALL_KICK, CBlock::LAST_HIT_PLAYER);

                        // 壁キックエフェクト
                        SetWallKickEffect(pos);

                        // 壁キックカウンタ
                        m_nCntWallKickTime = PLAYER_WALL_KICK_TIME;

                        // 壁キックの種類を反転
                        m_bWallKickType = !m_bWallKickType;
                    }
                }

                break;

            case HIT_SURFACE_RIGHT:

                // 壁走りできるなら
                if (pBlock->GetWallRun() == true)
                {
                    // リロードと左手攻撃をしていないなら
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // 前に進んでいるなら
                        if (STICK_UP(fAngleStick) || pInputKeyboard->GetKeyboardPress(DIK_W))
                        {
                            // 壁走りのフラグがtrue
                            m_bWallRun = true;

                            // 壁走り
                            pos.y += PLAYER_WALL_RAN_SPEED;

                            // 重力無視
                            m_move.y = 0.0f;

                            // 傾く
                            rot.x += PLAYER_TILT_SPEED;
                        }
                    }
                }

                // 壁にめり込まない
                m_move.x = 0.0f;
                m_move.z = 0.0f;

                // ジャンプボタンが押されたら
                if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_L2) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))
                {
                    // リロードと左手攻撃をしていないなら
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // キックジャンプ
                        m_move.y = PLAYER_WALL_KICK_JUMP_VALUE;

                        // 壁キック
                        pos.x += PLAYER_WALL_KICK_VALUE;
                        m_move.x = PLAYER_WALL_KICK_VALUE;

                        // 壁にダメージ
                        pBlock->SetTakeDamage(BLOCK_TAKE_DAMAGE_BY_WALL_KICK, CBlock::LAST_HIT_PLAYER);

                        // 壁キックエフェクト
                        SetWallKickEffect(pos);

                        // 壁キックカウンタ
                        m_nCntWallKickTime = PLAYER_WALL_KICK_TIME;

                        // 壁キックの種類を反転
                        m_bWallKickType = !m_bWallKickType;
                    }
                }

                break;

            case HIT_SURFACE_FRONT:

                // 壁走りできるなら
                if (pBlock->GetWallRun() == true)
                {
                    // リロードと左手攻撃をしていないなら
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // 前に進んでいるなら
                        if (STICK_UP(fAngleStick) || pInputKeyboard->GetKeyboardPress(DIK_W))
                        {
                            // 壁走りのフラグがtrue
                            m_bWallRun = true;

                            // 壁走り
                            pos.y += PLAYER_WALL_RAN_SPEED;

                            // 重力無視
                            m_move.y = 0.0f;

                            // 傾く
                            rot.x += PLAYER_TILT_SPEED;
                        }
                    }
                }

                // 壁にめり込まない
                m_move.x = 0.0f;
                m_move.z = 0.0f;

                // ジャンプボタンが押されたら
                if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_L2) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))
                {
                    // リロードと左手攻撃をしていないなら
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // キックジャンプ
                        m_move.y = PLAYER_WALL_KICK_JUMP_VALUE;

                        // 壁キック
                        pos.z -= PLAYER_WALL_KICK_VALUE;
                        m_move.z = -PLAYER_WALL_KICK_VALUE;

                        // 壁にダメージ
                        pBlock->SetTakeDamage(BLOCK_TAKE_DAMAGE_BY_WALL_KICK, CBlock::LAST_HIT_PLAYER);

                        // 壁キックエフェクト
                        SetWallKickEffect(pos);

                        // 壁キックカウンタ
                        m_nCntWallKickTime = PLAYER_WALL_KICK_TIME;

                        // 壁キックの種類を反転
                        m_bWallKickType = !m_bWallKickType;
                    }
                }

                break;
            }
            //=====================================================================================

            // 次のシーンにする
            pScene = pNextScene;
        }
        else
        {
            // 中身がないなら、そこで処理を終える
            break;
        }
    }

    // 敵との当たり判定
    pScene = CScene::GetTopScene(CScene::OBJTYPE_ENEMY);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_ENEMY); nCntScene++)
    {
        // 中身があるなら
        if (pScene != NULL)
        {
            // 次のシーンを記憶
            CScene*pNextScene = pScene->GetNextScene();

            // キャラクターにキャスト
            CCharacter *pEnemy = (CCharacter*)pScene;

            // 当たり判定の数分回す
            for (int nCntCollision = 0; nCntCollision < pEnemy->GetCntCollision(); nCntCollision++)
            {
                // エネミーの位置と大きさを取得
                CCharacter::COLLISION enemyCollision = pEnemy->GetCollision(nCntCollision);

                // 当たっているなら
                if (RectangleCollision3D(&pos, &enemyCollision.pos,
                    &GetActiveCollisionSize(), &enemyCollision.size) == true)
                {
                    // 種族に応じて、ダメージを受ける
                    switch (pEnemy->GetTribe())
                    {
                    case TRIBE_ANT:
                        TakeDamage(PLAYER_TOUCH_ANT, pEnemy->GetPosOld());
                        break;

                    case TRIBE_SPIDER:
                        TakeDamage(PLAYER_TOUCH_SPIDER, pEnemy->GetPosOld());
                        break;

                    case TRIBE_UFO:
                        TakeDamage(PLAYER_TOUCH_UFO, pEnemy->GetPosOld());
                        break;

                    case TRIBE_TOWER:
                        TakeDamage(PLAYER_TOUCH_TOWER, pEnemy->GetPosOld());
                        break;

                    case TRIBE_WYVERN:
                        TakeDamage(PLAYER_TOUCH_WYVERN, pEnemy->GetPosOld());
                        break;
                    }
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

    // ジャンプボタンが押されたら
    // ゲームがフェード中以外なら
    if (CGame::GetState() != CGame::STATE_IN_FADE)
    {
        if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_L2) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))
        {
            // 動けるなら
            if (m_nCntInvincibleFrame <= PLAYER_START_MOVE_IN_INVINCIBLE)
            {
                // リロード中や左手攻撃中でなく、ジャンプできるなら
                if (m_bReloading == false && m_bJump == true && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                {
                    // ジャンプ音
                    pSound->Play(CSound::LABEL_SE_KICK);

                    // ジャンプ
                    m_move.y = PLAYER_JUMP_VALUE;

                    // ジャンプ不可に
                    m_bJump = false;

                    // スティックが倒れているなら横移動
                    if (Controller.lY != 0 || Controller.lX != 0)
                    {
                        // ジャンプ時の横移動方向を決定
                        m_move.x = sinf(pCamera->GetAngle() + fAngleStick) * fSpeed;
                        m_move.z = cosf(pCamera->GetAngle() + fAngleStick) * fSpeed;
                    }
                }
            }
        }
    }

    // 壁キックモーションにするかどうか
    if (m_nCntWallKickTime > 0)
    {
        // カウントダウン
        m_nCntWallKickTime--;

        // 壁キックの途中でさらに壁キックをしたときに、もう一度回転させるためのフラグ
        if (m_bWallKickType == false)
        {
            // モーションを壁キックにする
            GetAnimation()->SetAnimation(ANIM_WALL_KICK);
        }
        else
        {
            // モーションを壁キックにする
            GetAnimation()->SetAnimation(ANIM_WALL_KICK2);
        }
    }

    // 着地モーションにするかどうか
    if (GetPosOld().y - PLAYER_LANDING_POS_OLD_Y_HEIGHT > pos.y && m_bJumpOld == false && m_bJump == true)
    {
        // 着地時間を設定
        m_nCntLandingTime = PLAYER_LANDING_TIME;

        // 着地時の地割れエフェクト
        SetLandingCrack(pos);

        //// リロードを妨げる
        //PreventReloading();
    }
    if (m_nCntLandingTime > 0)
    {
        // カウントダウン
        m_nCntLandingTime--;

        // モーションを着地にする
        GetAnimation()->SetAnimation(ANIM_LANDING);
    }

    // ジャンプモーションにするかどうか
    if (m_nCntWallKickTime <= 0 && m_bJump == false && m_bWallRun == false)
    {
        // モーションをジャンプにする
        GetAnimation()->SetAnimation(ANIM_JUMP);
    }

    // 攻撃処理
    // ゲームがフェード中以外なら
    if (CGame::GetState() != CGame::STATE_IN_FADE)
    {
        Attack(nCnt, pos, rot);
    }

    // リロードモーションにするかどうか
    if (m_bReloading == true)
    {
        // モーションを立ちリロードにする
        GetAnimation()->SetAnimation(ANIM_RELOAD);
    }

    // やられモーションにするかどうか
    if (m_nCntInvincibleFrame <= PLAYER_INVINCIBLE_FRAME &&
        m_nCntInvincibleFrame >= PLAYER_START_MOVE_IN_INVINCIBLE)
    {
        // モーションをやられにする
        GetAnimation()->SetAnimation(ANIM_TAKE_DAMAGE);

        // リロードを妨げる
        PreventReloading();
    }

    // 壁走りしていないなら
    if (m_bWallRun == false)
    {
        // 傾きを直す
        rot.x -= PLAYER_TILT_SPEED;
    }

    // 傾き制限
    if (rot.x > PLAYER_TILT_LIMIT)
    {
        rot.x = PLAYER_TILT_LIMIT;
    }
    else if (rot.x < 0.0f)
    {
        rot.x = 0.0f;
    }

    // 位置と向きを反映
    SetPos(pos);
    SetRot(rot);

    // 1F前、ジャンプできたかどうかを記録
    m_bJumpOld = m_bJump;

    // 足音を再生
    if (m_nCntFootsteps >= PLAYER_FOOTSTEPS_COOL_TIME)
    {
        m_nCntFootsteps = 0;

        // リロードしていないなら再生
        if (m_bReloading == false)
        {
            pSound->Play(CSound::LABEL_SE_FOOT);
        }
    }

#ifdef COLLISION_TEST
    CDebug::Create(pos, GetActiveCollisionSize(), CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// プレイヤーの移動制御
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Control(int nCnt, float fSpeed)
{
    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(nCnt);
    float fAngleStick = atan2(Controller.lX, Controller.lY*-1);

    // カメラを取得
    CCamera *pCamera = CManager::GetCamera();

    // 左手攻撃中でもなく、動けるなら
    if (m_leftHandAttack == LEFT_HAND_ATTACK_NONE && m_nCntInvincibleFrame <= PLAYER_START_MOVE_IN_INVINCIBLE)
    {
        // スティックが倒れているなら移動
        if (Controller.lY != 0 || Controller.lX != 0)
        {
            // スティックの角度を求める
            fAngleStick = atan2(Controller.lX, Controller.lY*-1);

            // ジャンプできるなら
            if (m_bJump == true)
            {
                // 足音のカウントアップ
                m_nCntFootsteps++;

                // 移動量に代入
                m_move.x = sinf(pCamera->GetAngle() + fAngleStick)*fSpeed;
                m_move.z = cosf(pCamera->GetAngle() + fAngleStick)*fSpeed;

                if (STICK_DOWN(fAngleStick))
                {
                    // モーションを後ろ歩きにする
                    GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
                }
                else
                {
                    // モーションを歩きにする
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
            }
            // ジャンプできないなら
            else
            {
                // 壁走り中なら
                if (m_bWallRun == true)
                {
                    // 足音のカウントアップ
                    m_nCntFootsteps++;

                    // 空中制御のため、移動量に加算
                    m_move.x += sinf(pCamera->GetAngle() + fAngleStick)*fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + fAngleStick)*fSpeed;

                    // モーションを歩きにする
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
                // 空中なら
                else
                {
                    // 空中制御のため、移動量に加算
                    m_move.x += sinf(pCamera->GetAngle() + fAngleStick)*fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + fAngleStick)*fSpeed;
                }
            }
        }
        // スティックが倒れていないなら、キーボード操作可能
        else
        {
            // キーボード操作
            KeyboardOperation(fSpeed);
        }
    }
}

//=============================================================================
// キーボード操作
// Author : 後藤慎之助
//=============================================================================
void CPlayer::KeyboardOperation(float fSpeed)
{
    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // カメラを取得
    CCamera *pCamera = CManager::GetCamera();

    // ジャンプできるなら
    if (m_bJump == true)
    {
        // Aキーが押された(左移動)
        if (pInputKeyboard->GetKeyboardPress(DIK_A))
        {
            // 足音のカウントアップ
            m_nCntFootsteps++;

            if (pInputKeyboard->GetKeyboardPress(DIK_W))// AとWが同時に押された
            {
                m_move.x = sinf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;

                // モーションを歩きにする
                GetAnimation()->SetAnimation(ANIM_MOVE);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))// AとSが同時に押された
            {
                m_move.x = sinf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;

                // モーションを後ろ歩きにする
                GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
            }
            else				// 左だけ押した
            {
                m_move.x = sinf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;

                // モーションを歩きにする
                GetAnimation()->SetAnimation(ANIM_MOVE);
            }
        }
        // Dキーが押された(右移動)
        else if (pInputKeyboard->GetKeyboardPress(DIK_D))
        {
            // 足音のカウントアップ
            m_nCntFootsteps++;

            if (pInputKeyboard->GetKeyboardPress(DIK_W))// DとWが同時に押された
            {
                m_move.x = sinf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;

                // モーションを歩きにする
                GetAnimation()->SetAnimation(ANIM_MOVE);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))// DとSが同時に押された
            {
                m_move.x = sinf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;

                // モーションを後ろ歩きにする
                GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
            }
            else				// 右だけ押した
            {
                m_move.x = sinf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;

                // モーションを歩きにする
                GetAnimation()->SetAnimation(ANIM_MOVE);
            }
        }
        // Wキーが押された(上移動)
        else if (pInputKeyboard->GetKeyboardPress(DIK_W))
        {
            // 足音のカウントアップ
            m_nCntFootsteps++;

            m_move.x = sinf(pCamera->GetAngle()) * fSpeed;
            m_move.z = cosf(pCamera->GetAngle()) * fSpeed;

            // モーションを歩きにする
            GetAnimation()->SetAnimation(ANIM_MOVE);
        }
        // Sキーが押された(下移動)
        else if (pInputKeyboard->GetKeyboardPress(DIK_S))
        {
            // 足音のカウントアップ
            m_nCntFootsteps++;

            m_move.x = sinf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;
            m_move.z = cosf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;

            // モーションを後ろ歩きにする
            GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
        }
    }
    // ジャンプできないなら
    else
    {
        // 壁走り中なら
        if (m_bWallRun == true)
        {
            // 足音のカウントアップ
            m_nCntFootsteps++;

            // Aキーが押された(左移動)
            if (pInputKeyboard->GetKeyboardPress(DIK_A))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))// AとWが同時に押された
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;

                    // モーションを歩きにする
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))// AとSが同時に押された
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;

                    // モーションを後ろ歩きにする
                    GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
                }
                else				// 左だけ押した
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;

                    // モーションを歩きにする
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
            }
            // Dキーが押された(右移動)
            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))// DとWが同時に押された
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;

                    // モーションを歩きにする
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))// DとSが同時に押された
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;

                    // モーションを後ろ歩きにする
                    GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
                }
                else				// 右だけ押した
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;

                    // モーションを歩きにする
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
            }
            // Wキーが押された(上移動)
            else if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                m_move.x += sinf(pCamera->GetAngle()) * fSpeed;
                m_move.z += cosf(pCamera->GetAngle()) * fSpeed;

                // モーションを歩きにする
                GetAnimation()->SetAnimation(ANIM_MOVE);
            }
            // Sキーが押された(下移動)
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;
                m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;

                // モーションを後ろ歩きにする
                GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
            }
        }
        // 空中なら
        else
        {
            // Aキーが押された(左移動)
            if (pInputKeyboard->GetKeyboardPress(DIK_A))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))// AとWが同時に押された
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))// AとSが同時に押された
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;
                }
                else				// 左だけ押した
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;
                }
            }
            // Dキーが押された(右移動)
            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))// DとWが同時に押された
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))// DとSが同時に押された
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;
                }
                else				// 右だけ押した
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;
                }
            }
            // Wキーが押された(上移動)
            else if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                m_move.x += sinf(pCamera->GetAngle()) * fSpeed;
                m_move.z += cosf(pCamera->GetAngle()) * fSpeed;
            }
            // Sキーが押された(下移動)
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;
                m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;
            }
        }
    }
}

//=============================================================================
// 攻撃周りの処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Attack(int nCnt, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // 動けるかつ、リロード中でないなら
    if (m_nCntInvincibleFrame <= PLAYER_START_MOVE_IN_INVINCIBLE  && m_bReloading == false)
    {
        // 必殺中でないなら
        if (m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
        {
            // コントローラを取得
            CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
            DIJOYSTATE2 Controller = pInputJoypad->GetController(nCnt);

            // カメラを取得
            CCamera *pCamera = CManager::GetCamera();

            //キーボードの確保したメモリを取得
            CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

            // マウスを取得
            CMouse *pMouse = CManager::GetMouse();

            // サウンドを取得
            CSound *pSound = CManager::GetSound();

            // リロードボタンを押したら
            if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_X) || pInputKeyboard->GetKeyboardTrigger(DIK_R))
            {
                // 残弾が最大数未満なら
                if (m_nRemainBullets < PLAYER_REMAIN_BULLETS_MAX)
                {
                    // SEを再生
                    pSound->Play(CSound::LABEL_SE_MACHINE_GUN_RELOAD);

                    // リロード中に
                    m_bReloading = true;

                    // クールタイムを設定
                    m_nAttackCoolTime = PLAYER_RELOAD_COOL_TIME;
                }
            }

            // 攻撃ボタンを押したら
            if (pInputJoypad->GetJoypadPress(nCnt, CInputJoypad::BUTTON_R2) || pMouse->GetMousePressLeft())
            {
                // 残弾数があるなら
                if (m_nRemainBullets > 0)
                {
                    // 壁登り中でないなら
                    if (m_bWallRun == false)
                    {
                        // スティックが倒れているなら移動射撃
                        if (Controller.lY != 0 || Controller.lX != 0)
                        {
                            // スティックの角度を求める
                            float fAngleStick = atan2(Controller.lX, Controller.lY*-1);

                            if (STICK_UP(fAngleStick))
                            {
                                // モーションを前歩き射撃にする
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_AHEAD);
                            }
                            else if (STICK_LEFT(fAngleStick))
                            {
                                // モーションを左歩き射撃にする
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_LEFT);
                            }
                            else if (STICK_RIGHT(fAngleStick))
                            {
                                // モーションを右歩き射撃にする
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_RIGHT);
                            }
                            else if (STICK_DOWN(fAngleStick))
                            {
                                // モーションを後ろ歩き射撃にする
                                GetAnimation()->SetAnimation(ANIM_BACK_MOVE_ATTACK);
                            }
                        }
                        else
                        {
                            // Aキーが押された(左移動)
                            if (pInputKeyboard->GetKeyboardPress(DIK_A))
                            {
                                // モーションを左歩き射撃にする
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_LEFT);
                            }
                            // Dキーが押された(右移動)
                            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
                            {
                                // モーションを右歩き射撃にする
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_RIGHT);
                            }
                            // Wキーが押された(上移動)
                            else if (pInputKeyboard->GetKeyboardPress(DIK_W))
                            {
                                // モーションを前歩き射撃にする
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_AHEAD);
                            }
                            // Sキーが押された(下移動)
                            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
                            {
                                // モーションを後ろ歩き射撃にする
                                GetAnimation()->SetAnimation(ANIM_BACK_MOVE_ATTACK);
                            }
                            else
                            {
                                // モーションを停止射撃にする
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTCK_STOP);
                            }
                        }

                        // クールタイムがないなら
                        if (m_nAttackCoolTime <= 0)
                        {
                            // 移動の角度を決める
                            D3DXVECTOR3 moveAngle = DEFAULT_VECTOR;
                            if (pos.y > 0.0f)
                            {
                                // 空中での縦角度調整
                                moveAngle = D3DXVECTOR3(sinf(pCamera->GetShotAngle(true)), cosf(pCamera->GetShotThetaInAir(true)),
                                    cosf(pCamera->GetShotAngle(true)));
                            }
                            else
                            {
                                // 地上での縦角度調整
                                moveAngle = D3DXVECTOR3(sinf(pCamera->GetShotAngle(true)), cosf(pCamera->GetShotTheta(true)),
                                    cosf(pCamera->GetShotAngle(true)));
                            }

                            // アサルトライフルの弾
                            CBullet::Create(pos + PLAYER_SHOT_POS_ADJUSTMENT, BULLET_AR_COLLISION_SIZE, moveAngle, CBullet::TYPE_AR, CBullet::TRIBE_PLAYER);

                            // 残弾数を一つ減らす
                            m_nRemainBullets--;

                            // 残弾数表示を更新
                            CGame::GetBulletRemain()->Disp(m_nRemainBullets);

                            // クールタイムを設定
                            m_nAttackCoolTime = BULLET_AR_COOL_TIME;

                            // 射撃時の飛散エフェクト発生
                            SetScatteringBullet();
                        }

                        // 攻撃は、壁キックのカウンタをリセットする
                        m_nCntWallKickTime = 0;
                    }
                }
                // 残弾数がないなら、自動リロード
                else
                {
                    // SEを再生
                    pSound->Play(CSound::LABEL_SE_MACHINE_GUN_RELOAD);

                    // リロード中に
                    m_bReloading = true;

                    // クールタイムを設定
                    m_nAttackCoolTime = PLAYER_RELOAD_COOL_TIME;
                }
            }

            // 必殺ボタンを押したら
            if (pInputJoypad->GetJoypadPress(nCnt, CInputJoypad::BUTTON_L1) && pInputJoypad->GetJoypadPress(nCnt, CInputJoypad::BUTTON_R1)
                || pInputKeyboard->GetKeyboardTrigger(DIK_Z))
            {
                // 壁登り中でなく、オーブ数がlv1以上なら
                if (m_bWallRun == false && m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_LV1)
                {
                    // 強必殺
                    if (m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_MAX)
                    {
                        m_nAttackCoolTime = PLAYER_LEFT_HAND_ATTACK_STRONG_TIME;
                        m_leftHandAttack = LEFT_HAND_ATTACK_STRONG;
                    }
                    // 弱必殺
                    else
                    {
                        m_nAttackCoolTime = PLAYER_LEFT_HAND_ATTACK_WEAK_TIME;
                        m_leftHandAttack = LEFT_HAND_ATTACK_WEAK;
                    }
                }
            }
        }
        // 左手攻撃中(弱)なら
        else if (m_leftHandAttack == LEFT_HAND_ATTACK_WEAK)
        {
            // モーションを左手攻撃弱にする
            GetAnimation()->SetAnimation(ANIM_LEFT_ATTACK_WEAK);

            // 攻撃を生成するフレーム
            if (m_nAttackCoolTime == PLAYER_LEFT_HAND_ATTACK_WEAK_CREATE_FRAME)
            {
                SetLeftHandAttack(pos, rot);
            }
        }
        // 左手攻撃中(強)なら
        else if (m_leftHandAttack == LEFT_HAND_ATTACK_STRONG)
        {
            // モーションを左手攻撃強にする
            GetAnimation()->SetAnimation(ANIM_LEFT_ATTACK_STRONG);

            // 攻撃を生成するフレーム
            if (m_nAttackCoolTime == PLAYER_LEFT_HAND_ATTACK_STRONG_CREATE_FRAME)
            {
                SetLeftHandAttack(pos, rot);
            }
        }
    }
}

//=============================================================================
// 左手攻撃を生成する処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SetLeftHandAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // カメラを取得
    CCamera *pCamera = CManager::GetCamera();

    // 移動の角度を決める
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR;
    if (pos.y > 0.0f)
    {
        // 空中での縦角度調整
        moveAngle = D3DXVECTOR3(sinf(pCamera->GetShotAngle(false)), cosf(pCamera->GetShotThetaInAir(false)),
            cosf(pCamera->GetShotAngle(false)));
    }
    else
    {
        // 地上での縦角度調整
        moveAngle = D3DXVECTOR3(sinf(pCamera->GetShotAngle(false)), cosf(pCamera->GetShotTheta(false)),
            cosf(pCamera->GetShotAngle(false)));
    }

    // レベルごとの大きさで、爆弾を生成
    D3DXVECTOR3 size = DEFAULT_VECTOR;
    if (m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_LV1 && m_nLeftHandOrbs < PLAYER_LEFT_HAND_ORBS_LV2)
    {
        size = D3DXVECTOR3(BULLET_BOMB_COLLISION_SIZE_LV1, BULLET_BOMB_COLLISION_SIZE_LV1, BULLET_BOMB_COLLISION_SIZE_LV1);
        CBullet::Create(pos + PLAYER_SHOT_POS_ADJUSTMENT, size, moveAngle, CBullet::TYPE_BOMB, CBullet::TRIBE_PLAYER);

        // 弱ノックバック
        m_move.x = sinf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
        m_move.z = cosf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
    }
    else if (m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_LV2 && m_nLeftHandOrbs < PLAYER_LEFT_HAND_ORBS_LV3)
    {
        size = D3DXVECTOR3(BULLET_BOMB_COLLISION_SIZE_LV2, BULLET_BOMB_COLLISION_SIZE_LV2, BULLET_BOMB_COLLISION_SIZE_LV2);
        CBullet::Create(pos + PLAYER_SHOT_POS_ADJUSTMENT, size, moveAngle, CBullet::TYPE_BOMB, CBullet::TRIBE_PLAYER);

        // 弱ノックバック
        m_move.x = sinf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
        m_move.z = cosf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
    }
    else if (m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_LV3 && m_nLeftHandOrbs < PLAYER_LEFT_HAND_ORBS_MAX)
    {
        size = D3DXVECTOR3(BULLET_BOMB_COLLISION_SIZE_LV3, BULLET_BOMB_COLLISION_SIZE_LV3, BULLET_BOMB_COLLISION_SIZE_LV3);
        CBullet::Create(pos + PLAYER_SHOT_POS_ADJUSTMENT, size, moveAngle, CBullet::TYPE_BOMB, CBullet::TRIBE_PLAYER);

        // 弱ノックバック
        m_move.x = sinf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
        m_move.z = cosf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
    }
    else if (m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_MAX)
    {
        size = D3DXVECTOR3(BULLET_BOMB_COLLISION_SIZE_LV4, BULLET_BOMB_COLLISION_SIZE_LV4, BULLET_BOMB_COLLISION_SIZE_LV4);
        CBullet::Create(pos + PLAYER_SHOT_POS_ADJUSTMENT, size, moveAngle, CBullet::TYPE_BOMB, CBullet::TRIBE_PLAYER);

        // 強ノックバック
        m_move.x = sinf(rot.y) * PLAYER_LEFT_HAND_ATTACK_STRONG_KNOCKBACK_VALUE;
        m_move.z = cosf(rot.y) * PLAYER_LEFT_HAND_ATTACK_STRONG_KNOCKBACK_VALUE;
    }

    // 左手のオーブ数を0に
    m_nLeftHandOrbs = 0;
}

//=============================================================================
// ダメージを受けたときの処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::TakeDamage(int nDamage, D3DXVECTOR3 posOld)
{
    // ゲームのステートが通常なら
    if (CGame::GetState() == CGame::STATE_NORMAL)
    {
        // 無敵時間がないなら
        if (m_nCntInvincibleFrame <= 0)
        {
            // ダメージを受けた時の声
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_DAMAGE_PLAYER);

            // ダメージを受ける
            SetTakeDamage(nDamage, LAST_HIT_ENEMY);

            // 位置を取得
            D3DXVECTOR3 pos = GetPos();

            // ノックバック量を決める
            D3DXVECTOR3 move = DEFAULT_VECTOR;
            move.x = pos.x - posOld.x;
            move.z = pos.z - posOld.z;
            D3DXVec3Normalize(&move, &move);  //ベクトルを正規化

                                              // ノックバック
            m_move.x = move.x * PLAYER_KNOCKBACK_VALUE;
            m_move.z = move.z * PLAYER_KNOCKBACK_VALUE;

            // 無敵時間の設定
            m_nCntInvincibleFrame = PLAYER_INVINCIBLE_FRAME;

            // クールタイムをリセット
            m_nAttackCoolTime = 0;

            // 左手攻撃をキャンセル
            m_leftHandAttack = LEFT_HAND_ATTACK_NONE;
        }
    }
}

//=============================================================================
// 回復処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Healing(int nHeal)
{
    // 現在のHPを取得
    int nHP = GetLife();

    // 設定する回復量
    int nAnswer = nHeal;

    // 最大値を超えたら
    if (nHP + nHeal > PLAYER_MAX_LIFE)
    {
        // 回復量を制限
        nAnswer = PLAYER_MAX_LIFE - nHP;
    }

    // 回復
    SetHealing(nAnswer);

    for (int nCnt = 0; nCnt < EFFECT_HEALING_NUMBER_ONCE; nCnt++)
    {
        // エフェクト発生用の変数宣言
        // 大きさを出す
        float fSize = float(GetRandNum(EFFECT_HEALING_MAX_SIZE, EFFECT_HEALING_MIN_SIZE)) / 100.0f;

        // 大きさを決める
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // 角度を出す
        float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // 位置を決める
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle) * GetActiveCollisionSize().x, fSize / 2, -cosf(fAngle) * GetActiveCollisionSize().x) + GetPos();

        // 速さを出す
        float fSpeed = float(GetRandNum(EFFECT_HEALING_MAX_SPEED, EFFECT_HEALING_MIN_SPEED)) / 100.0f;
        float fSpeedY = float(GetRandNum(EFFECT_HEALING_MAX_SPEED_Y, EFFECT_HEALING_MIN_SPEED_Y)) / 100.0f;

        // 移動量を決める
        float fMoveAngle = atan2((GetPos().x - effectPos.x), (GetPos().z - effectPos.z));
        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fMoveAngle) * fSpeed, fSpeedY, -cosf(fMoveAngle)* fSpeed);

        // エフェクト発生
        CEffect3D::Create(effectPos, size, move, EFFECT_HEALING_COLOR,
            DEFAULT_VECTOR, EFFECT_HEALING_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
    }

}

//=============================================================================
// 向きの制御
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CPlayer::RotControl(D3DXVECTOR3 rot)
{
    // 返す回転量
    D3DXVECTOR3 returnRot = rot;

    // カメラを取得
    CCamera *pCamera = CManager::GetCamera();

    // カメラの向いているほうを向く
    m_rotDest.y = pCamera->GetPlayerRot();

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
        returnRot.y += (m_rotDest.y - returnRot.y) * PLAYER_TURN_SPEED;
    }

    return returnRot;
}

//=============================================================================
// リロードを妨げる処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::PreventReloading(void)
{
    // リロード中なら
    if (m_bReloading == true)
    {
        // サウンドを取得
        CSound *pSound = CManager::GetSound();

        // SEを止める
        pSound->Stop(CSound::LABEL_SE_MACHINE_GUN_RELOAD);

        // リロード中ではなくす
        m_bReloading = false;

        // クールタイムをリセット
        m_nAttackCoolTime = 0;
    }
}

//=============================================================================
// オーブ数上昇処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SetRisingLeftHandOrbs(void)
{
    // 最大数以下なら
    if (m_nLeftHandOrbs < PLAYER_LEFT_HAND_ORBS_MAX)
    {
        // 上昇
        m_nLeftHandOrbs++;
    }
}

//=============================================================================
// やられた時の粉砕処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // パーツの位置と、モデルエフェクトの種類
    D3DXVECTOR3 partsPos = DEFAULT_VECTOR;
    CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_X_BODY;

    //==============================
    // 体
    //==============================
    // 位置
    partsPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._41,
        GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._42,
        GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._43);

    // エフェクトの発生
    modelEffectType = CModelEffect::TYPE_X_BODY;
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // 足
    //==============================
    // 位置
    partsPos = GetPos();

    // エフェクトの発生
    modelEffectType = CModelEffect::TYPE_X_FOOT;
    SetDeathCrushEffect(partsPos, rot, modelEffectType);
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // 左手
    //==============================
    // 位置
    partsPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._41,
        GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._42,
        GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._43);

    // エフェクトの発生
    modelEffectType = CModelEffect::TYPE_X_LHAND;
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // 右手
    //==============================
    // 位置
    partsPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._41,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._42,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._43);

    // エフェクトの発生
    modelEffectType = CModelEffect::TYPE_X_RHAND;
    SetDeathCrushEffect(partsPos, rot, modelEffectType);
}

//=============================================================================
// やられた時の粉砕エフェクトの発生
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type)
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
    fSpeedX = 0.0f;
    fSpeedY = float(GetRandNum(MODEL_EFFECT_X_SPLATTER_FORCE_MAX, MODEL_EFFECT_X_SPLATTER_FORCE_MIN)) / 100.0f;
    fSpeedZ = 0.0f;
    fRotSpeedX = float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f;
    fRotSpeedY = float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f;
    fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f;
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
