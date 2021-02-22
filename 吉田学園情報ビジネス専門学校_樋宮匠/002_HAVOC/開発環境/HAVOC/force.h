//===================================================================================
//
// 防衛軍の処理 (force.h)
// Author : 後藤慎之助(行動AI、エフェクト) / 樋宮匠(階層構造、アニメーション制御)
//
//===================================================================================
#ifndef _FORCE_H_
#define _FORCE_H_

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

// 防衛軍の各情報
#define FORCE_ACTIVE_COLLISION_SIZE D3DXVECTOR3(102.5f, 250.0f, 102.5f)         // 能動的な当たり判定の大きさ
#define FORCE_LIFE 140                                                          // HP
#define FORCE_SCORE 10000                                                       // スコア
#define FORCE_ORBS 20                                                           // オーブ数
#define FORCE_SPEED 10.0f                                                       // 速さ
#define FORCE_DISCOVERY_DISTANCE 1500.0f                                        // 発見距離
#define FORCE_COLLISION_MAX 1                                                   // 当たり判定の最大数

// 回転周り
#define FORCE_TURN_SPEED D3DXToRadian(2.0f)                                     // 振り向き速度

// 攻撃回り
#define FORCE_NEXT_ATTACK_COUNT 85                                              // 次の攻撃までのカウンタ

// ダッシュ
#define FORCE_DUSH_WHOLE_FRAME 180                                              // 全体フレーム

// 射撃
#define FORCE_SHOT_WHOLE_FRAME 150                                              // 全体フレーム
#define FORCE_SHOT_CREATE_FRAME 15                                              // クリエイトフレーム
#define FORCE_SHOT_AIM_UP 100.0f                                                // エイム時に、やや上を狙う

// 回復アイテムを落とす確率(100分のなんぼか)
#define FORCE_DROP_HEAL_ITEM 100

// デス時のカウンタ
#define FORCE_DEATH_PLAY_SOUND 60                                               // 撃破音を鳴らすフレーム
#define FORCE_DEATH_COUNTER 180                                                 // 消えるまで

//================================================
// 前方宣言
//================================================
class CAnimation;

//================================================
// クラス宣言
//================================================

// 防衛軍クラス
class CForce : public CCharacter
{
public:
    CForce();
    ~CForce();

    // 列挙型宣言(モーション)
    typedef enum
    {
        ANIM_IDLE = 0,      // 待機
        ANIM_WALK,          // 歩き
        ANIM_ATTACK,        // 攻撃
        ANIM_DEATH,         // やられ
        ANIM_MAX,           // 最大数
    }ANIMATION;

    // 列挙型宣言（親→子→孫の順で宣言）
    typedef enum
    {
        PARTS_BODY = 0,
        PARTS_HEAD,
        PARTS_RIGHT_ARM,
        PARTS_RIGHT_HAND,
        PARTS_LEFT_ARM,
        PARTS_LEFT_HAND,
        PARTS_RIGHT_LEG,
        PARTS_RIGHT_FOOT,
        PARTS_LEFT_LEG,
        PARTS_LEFT_FOOT,
        PARTS_WEAPON,
        PARTS_MAX,
    }PARTS;

    // 攻撃AI
    typedef enum
    {
        ATTACK_AI_WAIT = 0,   // 待機
        ATTACK_AI_DUSH,       // 走る
        ATTACK_AI_SHOT,       // 射撃
        ATTACK_AI_MAX,        // 攻撃AIの最大数
    }ATTACK_AI;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Movement(void);
    void UpdateCollisionPos(void);                                       // 当たり判定の位置の更新
    D3DXVECTOR3 RotControl(D3DXVECTOR3 rot);                             // 回転制御
    D3DXVECTOR3 AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot);              // 攻撃AI
    void JudgmentRunOrAttack(D3DXVECTOR3 pos);                           // 走るか攻撃するかの判断
    void DicisionTargetPos(D3DXVECTOR3 pos);                             // 標的の位置を決める
    D3DXVECTOR3 Dush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                  // 走る
    void SetShotAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                // 射撃攻撃

    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CForce *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

    void ScatterOrbs(void);                                              // オーブをバラまく
    void AddMissionClear(void);                                          // ミッションクリア数を追加する

private:
    static LPD3DXMESH		m_pMesh[PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[PARTS_MAX];
    static DWORD			m_nNumMat[PARTS_MAX];
    // 種類
    D3DXVECTOR3 m_move;                        // 移動量
    int m_nScore;                              // スコア
    int m_nOrbs;                               // オーブ

    int m_nCntTime;                            // 時間を数える

    float m_fSpeed;                            // 速さ
    D3DXVECTOR3 m_rotDest;                     // 向き(目的値)
    D3DXVECTOR3 m_rotMin;                      // 向きを変えるときに、最短で振り向くため使用

    float m_fDiscoveryDistance;                // プレイヤー発見距離

    ATTACK_AI m_attackAI;                      // 攻撃AI

    D3DXVECTOR3 m_targetPos;                   // ターゲットの位置
    bool m_bUseRotControl;                     // 向きの制御を使うかどうか

    int m_nDeathCounter;                       // デス時のカウンター
};

#endif
