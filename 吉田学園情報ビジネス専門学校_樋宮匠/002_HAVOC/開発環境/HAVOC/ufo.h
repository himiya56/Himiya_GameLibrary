//===================================================================================
//
// UFOの処理 (ufo.h)
// Author : 後藤慎之助(行動AI、エフェクト) / 樋宮匠(階層構造、アニメーション制御)
//
//===================================================================================
#ifndef _UFO_H_
#define _UFO_H_

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

// UFOの足の本数
#define UFO_MAX_LEG 8

// UFOが目的地に近いとみなす値
#define UFO_CONSIDER_CLOSE_DEST 30.0f

// UFOがパトロール中に方向転換するまでのカウンタ
#define UFO_CHANGE_MOVE_IN_PATROL 25

// UFOが周囲のUFOに警告をするときの距離
#define UFO_WARN_UFO_DISTANCE 4500.0f

// UFOが遠距離とみなす値
#define UFO_CONSIDER_AS_LONG 6000.0f

// UFO
#define UFO_UP_COLLISION_POS  D3DXVECTOR3(0.0f, 150.0f, 0.0f)                 // 当たり判定の位置を上にずらす
#define UFO_ACTIVE_COLLISION_SIZE D3DXVECTOR3(750.0f, 750.0f, 750.0f)         // 能動的な当たり判定の大きさ
#define UFO_LIFE 120                                                          // HP
#define UFO_SCORE 8000                                                        // スコア
#define UFO_ORBS 22                                                           // オーブ数
#define UFO_SPEED 17.5f                                                       // 速さ
#define UFO_PATROL_DISTANCE 4500                                              // 巡回距離
#define UFO_DISCOVERY_DISTANCE 2500.0f                                        // 発見距離
#define UFO_COLLISION_MAX 1                                                   // 当たり判定の最大数

// 回転周り
#define UFO_TURN_SPEED D3DXToRadian(2.5f)                                     // 振り向き速度

// 攻撃回り
#define UFO_NEXT_ATTACK_COUNT 120                                             // 次の攻撃までのカウンタ
#define UFO_TRACKING_SPEED 14.5f                                              // 追跡速度

// 射撃
#define UFO_SHOT_WHOLE_FRAME 190                                              // 全体フレーム
#define UFO_SHOT_CREATE_FRAME_1 60                                            // クリエイトフレーム1
#define UFO_SHOT_CREATE_FRAME_2 80                                            // クリエイトフレーム2
#define UFO_SHOT_CREATE_FRAME_3 100                                           // クリエイトフレーム3

// 回復アイテムを落とす確率(100分のなんぼか)
#define UFO_DROP_HEAL_ITEM 50

//================================================
// 前方宣言
//================================================
class CAnimation;

//================================================
// クラス宣言
//================================================

// UFOクラス
class CUfo : public CCharacter
{
public:
    CUfo();
    ~CUfo();

    // 列挙型宣言(モーション)
    typedef enum
    {
        ANIM_IDLE = 0,      // 待機
        ANIM_WARN,          // 威嚇
        ANIM_MAX,           // 最大数
    }ANIMATION;

    // 列挙型宣言
    typedef enum
    {
        PARTS_FIGHTER = 0,
        PARTS_RIGHT_WING_0,
        PARTS_RIGHT_WING_1,
        PARTS_RIGHT_WING_2,
        PARTS_RIGHT_WING_3,
        PARTS_LEFT_WING_0,
        PARTS_LEFT_WING_1,
        PARTS_LEFT_WING_2,
        PARTS_LEFT_WING_3,
        PARTS_MAX,
    }PARTS;

    // 攻撃AI
    typedef enum
    {
        ATTACK_AI_WAIT = 0,   // 待機
        ATTACK_AI_SHOT,       // 射撃
        ATTACK_AI_MAX,        // 攻撃AIの最大数
    }ATTACK_AI;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Movement(void);
    void UpdateCollisionPos(void);                                       // 当たり判定の位置の更新
    D3DXVECTOR3 RotControl(D3DXVECTOR3 rot);                             // 回転制御
    D3DXVECTOR3 PatrolAI(D3DXVECTOR3 pos);                               // パトロールAI
    D3DXVECTOR3 WarningAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot);             // 警戒中AI
    D3DXVECTOR3 AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot);              // 攻撃AI
    void SetShotAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                // 射撃攻撃

    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CUfo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

    void ScatterOrbs(void);                                                                 // オーブをバラまく
    void DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                                      // やられた時の粉砕
    void SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type);    // やられた時の粉砕エフェクトの発生
    void WarnFellow(void);                                                                  // 仲間に警告する
    void AddMissionClear(void);                                                             // ミッションクリア数を追加する

    // 警戒のセッター、ゲッター
    void SetWarning(bool bWarning) { m_bWarning = bWarning; }
    bool GetWarning(void) { return m_bWarning; }

private:
    static LPD3DXMESH		m_pMesh[PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[PARTS_MAX];
    static DWORD			m_nNumMat[PARTS_MAX];

    D3DXVECTOR3 m_move;                        // 移動量
    int m_nScore;                              // スコア
    int m_nOrbs;                               // オーブ

    D3DXVECTOR3 m_patrolCentralPos;            // 巡回の中心位置
    D3DXVECTOR3 m_patrolDestPos;               // 巡回の目的位置
    int m_nPatrolDistance;                     // 巡回距離
    int m_nCntTime;                            // 時間を数える

    float m_fSpeed;                            // 速さ
    D3DXVECTOR3 m_rotDest;                     // 向き(目的値)
    D3DXVECTOR3 m_rotMin;                      // 向きを変えるときに、最短で振り向くため使用

    bool m_bWarning;                           // 警戒中かどうか
    float m_fDiscoveryDistance;                // プレイヤー発見距離

    ATTACK_AI m_attackAI;                      // 攻撃AI
};

#endif
