//===================================================================================
//
// アリの処理 (ant.h)
// Author : 後藤慎之助(行動AI、エフェクト) / 樋宮匠(階層構造、アニメーション制御)
//
//===================================================================================
#ifndef _ANT_H_
#define _ANT_H_

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

// アリの足の本数
#define ANT_MAX_LEG 6

// アリが目的地に近いとみなす値
#define ANT_CONSIDER_CLOSE_DEST 20.0f

// アリがパトロール中に方向転換するまでのカウンタ
#define ANT_CHANGE_MOVE_IN_PATROL 30

// アリが周囲のアリに警告をするときの距離
#define ANT_WARN_ANT_DISTANCE 2200.0f

// アリが〇距離とみなす値
#define ANT_CONSIDER_AS_SHORT 2000.0f
#define ANT_CONSIDER_AS_MEDIUM 4000.0f
#define ANT_CONSIDER_AS_LONG 5500.0f

// 白アリ
#define ANT_WHITE_ACTIVE_COLLISION_SIZE D3DXVECTOR3(200.0f, 150.0f, 200.0f)         // 能動的な当たり判定の大きさ
#define ANT_WHITE_LIFE 50                                                           // HP
#define ANT_WHITE_SCORE 3000                                                        // スコア
#define ANT_WHITE_ORBS 15                                                           // オーブ数
#define ANT_WHITE_SPEED 12.5f                                                       // 速さ
#define ANT_WHITE_PATROL_DISTANCE 3500                                              // 巡回距離
#define ANT_WHITE_DISCOVERY_DISTANCE 2250.0f                                        // 発見距離
#define ANT_WHITE_COLLISION_MAX 1                                                   // 当たり判定の最大数
#define ANT_WHITE_BODY_COLLISION_SIZE D3DXVECTOR3(350.0f, 200.0f, 350.0f)           // 体の当たり判定の大きさ

// 巨大黒アリ
#define ANT_BLACK_SCALE D3DXVECTOR3(2.0f, 2.0f, 2.0f)                               // スケール
#define ANT_BLACK_ACTIVE_COLLISION_SIZE  D3DXVECTOR3(475.0f, 500.0f, 475.0f)        // 能動的な当たり判定の大きさ
#define ANT_BLACK_LIFE 100                                                          // HP
#define ANT_BLACK_SCORE 4500                                                        // スコア
#define ANT_BLACK_ORBS 25                                                           // オーブ数
#define ANT_BLACK_SPEED 12.0f                                                       // 速さ
#define ANT_BLACK_PATROL_DISTANCE 4000                                              // 巡回距離
#define ANT_BLACK_DISCOVERY_DISTANCE 2750.0f                                        // 発見距離
#define ANT_BLACK_COLLISION_MAX 6                                                   // 当たり判定の最大数
#define ANT_BLACK_BODY_COLLISION_SIZE D3DXVECTOR3(350.0f, 350.0f, 350.0f)           // 体の当たり判定の大きさ
#define ANT_BLACK_STOMACH_COLLISION_SIZE D3DXVECTOR3(400.0f, 400.0f, 400.0f)        // 腹の当たり判定の大きさ
#define ANT_BLACK_TACTILE_COLLISION_SIZE D3DXVECTOR3(200.0f, 200.0f, 200.0f)        // 触角の当たり判定の大きさ
#define ANT_BLACK_FANG_COLLISION_SIZE D3DXVECTOR3(300.0f, 300.0f, 300.0f)           // 牙の当たり判定の大きさ
#define ANT_BLACK_ACID_NUMBER_ONCE 2                                                // 一度に酸を生成する数(for文のnCntで方向を分けているので注意)

// 回転周り
#define ANT_TURN_SPEED D3DXToRadian(1.25f)       // 振り向き速度
#define ANT_TILT_SPEED D3DXToRadian(1.25f)       // 傾き速度
#define ANT_TILT_LIMIT D3DXToRadian(90.0f)       // 傾きの限界値

// 攻撃回り
// 待機中(距離によって採用の確率は変えてない)
#define ANT_NEXT_ATTACK_COUNT 100                // 次の攻撃までのカウンタ
#define ANT_TAUNT_PERCENT 25                     // 挑発が採用される確率
#define ANT_ACID_PERCENT 40                      // 酸が採用される確率
#define ANT_TACKLE_PERCENT 35                    // タックルが採用される確率
#define ANT_NEXT_ATTACK_PERCENT_MAX (ANT_TAUNT_PERCENT + ANT_ACID_PERCENT + ANT_TACKLE_PERCENT) // 攻撃採用の最大パーセント

// 挑発
#define ANT_TAUNT_WHOLE_FRAME 120                // 全体フレーム

// 酸飛ばし
#define ANT_ACID_WHOLE_FRAME 50                                  // 全体フレーム
#define ANT_ACID_CREATE_FRAME 30                                 // クリエイトフレーム
#define ANT_ACID_ADD_SET_POS_Y D3DXVECTOR3(0.0f, 100.0f, 0.0f)   // 発射時に、上にずらす値
#define ANT_ACID_SHORT_MAX_ANGLE_Y 2000                          // 近距離の最大角度Y
#define ANT_ACID_SHORT_MIN_ANGLE_Y 1500                          // 近距離の最小角度Y
#define ANT_ACID_MEDIUM_MAX_ANGLE_Y 3500                         // 中距離の最大角度Y
#define ANT_ACID_MEDIUM_MIN_ANGLE_Y 2500                         // 中距離の最小角度Y
#define ANT_ACID_LONG_MAX_ANGLE_Y 6000                           // 遠距離の最大角度Y
#define ANT_ACID_LONG_MIN_ANGLE_Y 4500                           // 遠距離の最小角度Y
#define ANT_ACID_MAX_ANGLE_TO_SHIFT_FROM_CENTER 2000             // 中心からずらす最大角度

// タックル
#define ANT_TACKLE_WHOLE_FRAME 180                // 全体フレーム
#define ANT_TACKLE_SPEED_RATE 1.275f              // スピード倍率

// 回復アイテムを落とす確率(100分のなんぼか)
#define ANT_DROP_HEAL_ITEM 20

//================================================
// 前方宣言
//================================================
class CAnimation;

//================================================
// クラス宣言
//================================================

// アリクラス
class CAnt : public CCharacter
{
public:
    CAnt();
    ~CAnt();

    // 列挙型宣言(モーション)
    typedef enum
    {
        ANIM_IDLE = 0,      // 待機
        ANIM_WALK,          // 歩き
        ANIM_TAUNT,         // 挑発
        ANIM_ATTACK,        // 攻撃
        ANIM_MAX,           // 最大数
    }ANIMATION;

    // 列挙型宣言
    typedef enum
    {
        PARTS_BODY = 0,
        PARTS_HEAD,
        PARTS_STOMACH,
        PARTS_TACTILE_RIGHT,
        PARTS_TACTILE_LEFT,
        PARTS_FANG_RIGHT,
        PARTS_FANG_LEFT,
        PARTS_LEG_RIGHT_0,
        PARTS_LEG_RIGHT_1,
        PARTS_LEG_RIGHT_2,
        PARTS_LEG_LEFT_0,
        PARTS_LEG_LEFT_1,
        PARTS_LEG_LEFT_2,
        PARTS_MAX,
    }PARTS;

    // 列挙型宣言(アリのタイプ)
    typedef enum
    {
        TYPE_NONE = 0,
        TYPE_WHITE,      // 白アリ
        TYPE_BLACK,      // 黒アリ
        TYPE_MAX,
    }TYPE;

    // 攻撃AI
    typedef enum
    {
        ATTACK_AI_WAIT = 0,   // 待機
        ATTACK_AI_TAUNT,      // 挑発
        ATTACK_AI_ACID,       // 酸飛ばし
        ATTACK_AI_TACKLE,     // タックル
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
    void SetAcidAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                // 酸飛ばし攻撃
    
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CAnt *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bWarning, TYPE type);

    void ScatterOrbs(void);                                                                 // オーブをバラまく
    void DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                                      // やられた時の粉砕
    void SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type);    // やられた時の粉砕エフェクトの発生
    void WarnFellow(void);                                                                  // 仲間に警告する
    void AddMissionClear(void);                                                             // ミッションクリア数を追加する

    // 警戒のセッター、ゲッター
    void SetWarning(bool bWarning) { m_bWarning = bWarning; }
    bool GetWarning(void) { return m_bWarning; }

    bool GetWallRun(void) { return m_bWallRun; }

private:
    static LPD3DXMESH		m_pMesh[TYPE_MAX][PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[TYPE_MAX][PARTS_MAX];
    static DWORD			m_nNumMat[TYPE_MAX][PARTS_MAX];

    TYPE m_type;		                       // 種類
    D3DXVECTOR3 m_move;                        // 移動量
    int m_nScore;                              // スコア
    int m_nOrbs;                               // オーブ

    D3DXVECTOR3 m_patrolCentralPos;            // 巡回の中心位置
    D3DXVECTOR3 m_patrolDestPos;               // 巡回の目的位置
    int m_nPatrolDistance;                     // 巡回距離
    int m_nCntTime;                            // 時間を数える

    bool m_bWallRun;                           // 壁走り中かどうか
    float m_fSpeed;                            // 速さ
    D3DXVECTOR3 m_rotDest;                     // 向き(目的値)
    D3DXVECTOR3 m_rotMin;                      // 向きを変えるときに、最短で振り向くため使用

    bool m_bWarning;                           // 警戒中かどうか
    float m_fDiscoveryDistance;                // プレイヤー発見距離

    bool m_bAttack;                            // 攻撃しているかどうか
    ATTACK_AI m_attackAI;                      // 攻撃AI
};

#endif
