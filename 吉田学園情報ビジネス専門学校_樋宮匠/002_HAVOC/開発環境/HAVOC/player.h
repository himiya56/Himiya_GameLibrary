//======================================================================================
//
// プレイヤー処理 (player.h)
// Author : 後藤慎之助(行動、モーション制御) / 樋宮匠(階層構造、モーション作成)
//
//======================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "character.h"
#include "model.h"
#include "modelEffect.h"

//================================================
// マクロ定義
//================================================

// プレイヤーの当たり判定の大きさ
#define PLAYER_COLLISION_SIZE D3DXVECTOR3(75.0f, 275.0f, 75.0f)

// プレイヤーの移動周り
#define PLAYER_BASE_SPEED 15.0f                                         // 基本の速度
#define PLAYER_CONTROL_SPEED_IN_AIR 0.8f                                // 空中で制御する時の速度
#define PLAYER_CONTROL_SPEED_IN_AIR_AFTER_WALL_KICK 0.875f              // 壁キック後に、空中で制御する時の速度
#define PLAYER_WALL_RAN_SPEED 15.0f                                     // 壁走り速度
#define PLAYER_WALL_KICK_VALUE 38.5f                                    // 壁キックの強さ
#define PLAYER_WALL_KICK_JUMP_VALUE 30.0f                               // 壁キックジャンプの強さ
#define PLAYER_CONTROL_MOVE 0.85f                                       // 移動量制御
#define PLAYER_CONTROL_MOVE_IN_AIR 0.95f                                // 空中での移動量制御
#define PLAYER_TURN_SPEED D3DXToRadian(10.0f)                           // 振り向き速度
#define PLAYER_TILT_SPEED D3DXToRadian(1.0f)                            // 傾き速度
#define PLAYER_TILT_LIMIT D3DXToRadian(45.0f)                           // 傾きの限界値
#define PLAYER_JUMP_VALUE 35.0f                                         // ジャンプの強さ
#define PLAYER_FOOTSTEPS_COOL_TIME 28                                   // 足音のクールタイム

// プレイヤーの右手攻撃回り
#define PLAYER_SHOT_POS_ADJUSTMENT (D3DXVECTOR3(0.0f, 175.0f, 0.0f))    // 撃つ高さ調整(もとは150)
#define PLAYER_REMAIN_BULLETS_MAX 200                                   // 残弾の最大数
#define PLAYER_REMAIN_BULLETS_WARNING 50                                // 残弾の警告開始数
#define PLAYER_RELOAD_COOL_TIME 180                                     // リロードのクールタイム

// プレイヤーのノックバック量
#define PLAYER_KNOCKBACK_VALUE 75.0f

// プレイヤーのHP周り
#define PLAYER_MAX_LIFE 3000                                            // 最大HP(3000?)
#define PLAYER_INVINCIBLE_FRAME 180                                     // プレイヤーの無敵時間
#define PLAYER_START_MOVE_IN_INVINCIBLE 90                              // 無敵中、動き出せるまでのカウンタ

// プレイヤーの初期位置、初期向き
#define PLAYER_START_POS_STAGE1 (D3DXVECTOR3(2000.0f, 0.0f, 0.0f))
#define PLAYER_START_ROT_STAGE1 D3DXToRadian(90.0f)
#define PLAYER_START_POS_STAGE2 (D3DXVECTOR3(400.0f, 0.0f, 6400.0f))
#define PLAYER_START_ROT_STAGE2 D3DXToRadian(0.0f)
#define PLAYER_START_POS_STAGE_FINAL (D3DXVECTOR3(6300.0f, 0.0f, 300.0f))
#define PLAYER_START_ROT_STAGE_FINAL D3DXToRadian(90.0f)

// プレイヤーの着地モーション周り
#define PLAYER_LANDING_TIME 15                                          // 着地フレーム
#define PLAYER_LANDING_POS_OLD_Y_HEIGHT 10.0f                           // 1F前と現在の位置のY座標が一定距離離れていると、着地を採用

// プレイヤーの壁キックモーション周り
#define PLAYER_WALL_KICK_TIME 50                                        // 壁キックフレーム

// プレイヤーの左手攻撃周り
#define PLAYER_LEFT_HAND_ORBS_START 0                                   // 開始時のオーブ数
#define PLAYER_LEFT_HAND_ORBS_MAX 200                                   // 最大オーブ数(lv4)
#define PLAYER_LEFT_HAND_ORBS_LV1 50                                    // オーブ数lv1
#define PLAYER_LEFT_HAND_ORBS_LV2 100                                   // オーブ数lv2
#define PLAYER_LEFT_HAND_ORBS_LV3 150                                   // オーブ数lv3
#define PLAYER_LEFT_HAND_ATTACK_WEAK_TIME 80                            // 弱必殺の攻撃全体時間
#define PLAYER_LEFT_HAND_ATTACK_WEAK_CREATE_FRAME 40                    // 弱必殺の攻撃生成フレーム
#define PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE 60.0f              // 弱必殺のノックバック量
#define PLAYER_LEFT_HAND_ATTACK_STRONG_TIME 125                         // 強必殺の攻撃全体時間
#define PLAYER_LEFT_HAND_ATTACK_STRONG_CREATE_FRAME 75                  // 強必殺の攻撃生成フレーム
#define PLAYER_LEFT_HAND_ATTACK_STRONG_KNOCKBACK_VALUE 120.0f           // 強必殺のノックバック量

//================================================
// 前方宣言
//================================================
class CAnimation;

//================================================
// クラス宣言
//================================================

// プレイヤークラス
class CPlayer : public CCharacter
{
public:
    CPlayer();
    ~CPlayer();

    // アニメーションの種類
    typedef enum
    {
        ANIM_IDLE = 0,	              // 待機
        ANIM_MOVE,                    // 移動
        ANIM_LEFT_ATTACK_WEAK,        // 左手(弱)
        ANIM_JUMP,                    // ジャンプ
        ANIM_LANDING,                 // 着地
        ANIM_LEFT_ATTACK_STRONG,      // 左手(強)
        ANIM_RIGHT_ATTCK_STOP,        // 右手(停止)
        ANIM_RIGHT_ATTACK_WALK_AHEAD, // 右手(前)
        ANIM_RIGHT_ATTACK_WALK_RIGHT, // 右手(右)
        ANIM_RIGHT_ATTACK_WALK_LEFT,  // 右手(左)
        ANIM_TAKE_DAMAGE,             // やられ
        ANIM_BACK_MOVE,               // 後ろ歩き
        ANIM_BACK_MOVE_ATTACK,        // 後ろ歩き撃ち
        ANIM_RELOAD,                  // リロード
        ANIM_WALL_KICK,               // 壁キック
        ANIM_WALL_KICK2,              // 壁キック2
        ANIM_DEATH,                   // 死亡
        ANIM_MAX,
    }ANIMATION;

    // プレイアブルキャラの人数
    typedef enum
    {
        PLAYABLE_001 = 0,
        PLAYABLE_MAX,
    }PLAYABLE;

    // モデルの各パーツ
    typedef enum
    {
        PARTS_HIP = 0,
        PARTS_BODY,
        PARTS_HEAD,
        PARTS_RSHOULDER,
        PARTS_RARM,
        PARTS_RHAND,
        PARTS_LSHOULDER,
        PARTS_LARM,
        PARTS_LHAND,
        PARTS_RTHIGH,
        PARTS_RLEG,
        PARTS_RFOOT,
        PARTS_LTHIGH,
        PARTS_LLEG,
        PARTS_LFOOT,
        PARTS_MAX,
    }PARTS;

    // 左手必殺の種類
    typedef enum
    {
        LEFT_HAND_ATTACK_NONE = 0,  // なし
        LEFT_HAND_ATTACK_WEAK,      // 弱必殺
        LEFT_HAND_ATTACK_STRONG,    // 強必殺
    }LEFT_HAND_ATTACK;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PLAYABLE playable);

    void SetReloadingEffect(void);                                                      // リロード中の吸収エフェクト
    void SetScatteringBullet(void);                                                     // 射撃時の飛散
    void SetLandingCrack(D3DXVECTOR3 pos);                                              // 着地時の地割れエフェクト
    void SetWallKickEffect(D3DXVECTOR3 pos);                                            // 壁キック時のエフェクト
    void Movement(int nCnt, float fSpeed);                                              // 行動
    void Control(int nCnt, float fSpeed);                                               // 制御
    void KeyboardOperation(float fSpeed);                                               // キーボード操作
    void Attack(int nCnt, D3DXVECTOR3 pos, D3DXVECTOR3 rot);                            // 攻撃周りの処理
    void SetLeftHandAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                           // 左手攻撃を生成
    void TakeDamage(int nDamage, D3DXVECTOR3 posOld);                                   // ダメージを受けたとき
    void Healing(int nHeal);                                                            // 回復
    D3DXVECTOR3 RotControl(D3DXVECTOR3 rot);                                            // 回転制御
    void PreventReloading(void);                                                        // リロードを妨げる

    void DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                                  // やられた時の粉砕
    void SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type);// やられた時の粉砕エフェクトの発生

    // セッター
    void SetRisingLeftHandOrbs(void);                                                   // オーブ数の上昇
    void SetRemainBulletsFull(void) { m_nRemainBullets = PLAYER_REMAIN_BULLETS_MAX; }   // 残弾数を最大に

    // ゲッター
    bool GetDisp(void) { return m_bDisp; }
    bool GetReloading(void) { return m_bReloading; }
    int GetRemainBullets(void) { return m_nRemainBullets; }
    int GetLeftHandOrbs(void) { return m_nLeftHandOrbs; }
    int GetInvincibleFrame(void) { return m_nCntInvincibleFrame; }

private:
    static LPD3DXMESH		m_pMesh[PLAYABLE_MAX][PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[PLAYABLE_MAX][PARTS_MAX];
    static DWORD			m_nNumMat[PLAYABLE_MAX][PARTS_MAX];

    PLAYABLE m_playable;                    // プレイアブルキャラ
    D3DXVECTOR3 m_move;                     // 移動量

    int m_nAttackCoolTime;                  // 攻撃のクールタイム
    bool m_bJump;                           // ジャンプできるかどうか
    D3DXVECTOR3 m_rotDest;                  // 向き(目的値)
    D3DXVECTOR3 m_rotMin;                   // 向きを変えるときに、最短で振り向くため使用

    int m_nCntDeath;                        // デス時のカウンタ

    bool m_bDisp;                           // 表示フラグ
    int m_nCntInvincibleFrame;              // 無敵時間
    bool m_bBlinking;                       // 点滅中かどうか

    bool m_bWallRun;                        // 壁登り中かどうか

    int m_nRemainBullets;                   // 残弾数
    bool m_bReloading;                      // リロード中かどうか

    int m_nCntLandingTime;                  // 着地時間
    bool m_bJumpOld;                        // 1F前、ジャンプできたかどうか

    int m_nCntWallKickTime;                 // 壁キックの経過時間を数える
    bool m_bWallKickType;                   // 壁キックの種類のフラグ

    int m_nLeftHandOrbs;                    // 左手のオーブ数
    LEFT_HAND_ATTACK m_leftHandAttack;      // 左手攻撃の種類

    int m_nCntFootsteps;                    // 足音のカウンタ
};

#endif