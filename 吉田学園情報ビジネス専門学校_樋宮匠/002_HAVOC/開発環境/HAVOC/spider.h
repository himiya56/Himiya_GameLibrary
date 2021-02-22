//===================================================================================
//
// 蜘蛛の処理 (spider.h)
// Author : 後藤慎之助(行動AI、エフェクト) / 樋宮匠(階層構造、アニメーション制御)
//
//===================================================================================
#ifndef _SPIDER_H_
#define _SPIDER_H_

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

// 蜘蛛の足の本数
#define SPIDER_MAX_LEG 8

// 蜘蛛が目的地に近いとみなす値
#define SPIDER_CONSIDER_CLOSE_DEST 20.0f

// 蜘蛛がパトロール中に方向転換するまでのカウンタ
#define SPIDER_CHANGE_MOVE_IN_PATROL 45

// 蜘蛛が周囲の蜘蛛に警告をするときの距離
#define SPIDER_WARN_SPIDER_DISTANCE 1850.0f

// 蜘蛛が〇距離とみなす値
#define SPIDER_CONSIDER_AS_SHORT 2000.0f
#define SPIDER_CONSIDER_AS_MEDIUM 4000.0f
#define SPIDER_CONSIDER_AS_LONG 5500.0f

// 黄蜘蛛
#define SPIDER_YELLOW_ACTIVE_COLLISION_SIZE D3DXVECTOR3(300.0f, 150.0f, 300.0f)         // 能動的な当たり判定の大きさ
#define SPIDER_YELLOW_LIFE 90                                                           // HP
#define SPIDER_YELLOW_SCORE 3500                                                        // スコア
#define SPIDER_YELLOW_ORBS 15                                                           // オーブ数
#define SPIDER_YELLOW_SPEED 7.5f                                                        // 速さ
#define SPIDER_YELLOW_PATROL_DISTANCE 2500                                              // 巡回距離
#define SPIDER_YELLOW_DISCOVERY_DISTANCE 1750.0f                                        // 発見距離
#define SPIDER_YELLOW_COLLISION_MAX 1                                                   // 当たり判定の最大数
#define SPIDER_YELLOW_BODY_COLLISION_SIZE D3DXVECTOR3(450.0f, 300.0f, 450.0f)           // 体の当たり判定の大きさ

// 巨大黒蜘蛛
#define SPIDER_BLACK_SCALE D3DXVECTOR3(2.0f, 2.0f, 2.0f)                                // スケール
#define SPIDER_BLACK_ACTIVE_COLLISION_SIZE  D3DXVECTOR3(475.0f, 400.0f, 475.0f)         // 能動的な当たり判定の大きさ
#define SPIDER_BLACK_LIFE 140                                                           // HP
#define SPIDER_BLACK_SCORE 5000                                                         // スコア
#define SPIDER_BLACK_ORBS 25                                                            // オーブ数
#define SPIDER_BLACK_SPEED 10.0f                                                        // 速さ
#define SPIDER_BLACK_PATROL_DISTANCE 3000                                               // 巡回距離
#define SPIDER_BLACK_DISCOVERY_DISTANCE 2250.0f                                         // 発見距離
#define SPIDER_BLACK_COLLISION_MAX 4                                                    // 当たり判定の最大数
#define SPIDER_BLACK_BODY_COLLISION_SIZE D3DXVECTOR3(250.0f, 250.0f, 250.0f)            // 体の当たり判定の大きさ
#define SPIDER_BLACK_STOMACH_COLLISION_SIZE D3DXVECTOR3(500.0f, 500.0f, 500.0f)         // 腹の当たり判定の大きさ
#define SPIDER_BLACK_FANG_COLLISION_SIZE D3DXVECTOR3(300.0f, 300.0f, 300.0f)            // 牙の当たり判定の大きさ
#define SPIDER_BLACK_ACID_NUMBER_ONCE 3                                                 // 一度に酸を生成する数(for文のnCntで方向を分けているので注意)

// 回転周り
#define SPIDER_TURN_SPEED D3DXToRadian(1.5f)        // 振り向き速度
#define SPIDER_TILT_SPEED D3DXToRadian(1.25f)       // 傾き速度
#define SPIDER_TILT_LIMIT D3DXToRadian(90.0f)       // 傾きの限界値

// 攻撃回り
// 待機中(距離によって採用の確率は変えてない)
#define SPIDER_NEXT_ATTACK_COUNT 110                // 次の攻撃までのカウンタ
#define SPIDER_TAUNT_PERCENT 20                     // 挑発が採用される確率
#define SPIDER_ACID_PERCENT 45                      // 酸が採用される確率
#define SPIDER_JUMP_PERCENT 35                      // ジャンプが採用される確率
#define SPIDER_NEXT_ATTACK_PERCENT_MAX (SPIDER_TAUNT_PERCENT + SPIDER_ACID_PERCENT + SPIDER_JUMP_PERCENT) // 攻撃採用の最大パーセント

// 挑発
#define SPIDER_TAUNT_WHOLE_FRAME 120                // 全体フレーム

// 酸飛ばし
#define SPIDER_ACID_WHOLE_FRAME 60                                  // 全体フレーム
#define SPIDER_ACID_CREATE_FRAME 35                                 // クリエイトフレーム
#define SPIDER_ACID_ADD_SET_POS_Y D3DXVECTOR3(0.0f, 200.0f, 0.0f)   // 発射時に、上にずらす値
#define SPIDER_ACID_SHORT_MAX_ANGLE_Y 1900                          // 近距離の最大角度Y
#define SPIDER_ACID_SHORT_MIN_ANGLE_Y 1500                          // 近距離の最小角度Y
#define SPIDER_ACID_MEDIUM_MAX_ANGLE_Y 3500                         // 中距離の最大角度Y
#define SPIDER_ACID_MEDIUM_MIN_ANGLE_Y 2500                         // 中距離の最小角度Y
#define SPIDER_ACID_LONG_MAX_ANGLE_Y 5900                           // 遠距離の最大角度Y
#define SPIDER_ACID_LONG_MIN_ANGLE_Y 4500                           // 遠距離の最小角度Y
#define SPIDER_ACID_MAX_ANGLE_TO_SHIFT_FROM_CENTER 2250             // 中心からずらす最大角度

// ジャンプ
#define SPIDER_JUMP_WHOLE_FRAME 220                // 全体フレーム
#define SPIDER_JUMP_SPEED_RATE 1.2f                // スピード倍率
#define SPIDER_JUMP_VALUE 46.0f                    // ジャンプ力

// 回復アイテムを落とす確率(100分のなんぼか)
#define SPIDER_DROP_HEAL_ITEM 15

//================================================
// 前方宣言
//================================================
class CAnimation;

//================================================
// クラス宣言
//================================================

// スパイダークラス
class CSpider : public CCharacter
{
public:
    CSpider();
    ~CSpider();

    // 列挙型宣言(モーション)
    typedef enum
    {
        ANIM_IDLE = 0,      // 待機
        ANIM_WALK,          // 歩き
        ANIM_TAUNT,         // 挑発
        ANIM_ATTACK,        // 攻撃
        ANIM_JUMP,          // ジャンプ
        ANIM_MAX,           // 最大数
    }ANIMATION;

    // 列挙型宣言
    typedef enum
    {
        PARTS_BODY = 0,
        PARTS_STOMACH,
        PARTS_FANG_RIGHT,
        PARTS_FANG_LEFT,
        PARTS_LEG_RIGHT_0,
        PARTS_LEG_RIGHT_1,
        PARTS_LEG_RIGHT_2,
        PARTS_LEG_RIGHT_3,
        PARTS_LEG_LEFT_0,
        PARTS_LEG_LEFT_1,
        PARTS_LEG_LEFT_2,
        PARTS_LEG_LEFT_3,
        PARTS_STOMACH_CUBE, // 腹から発生させる当たり判定の位置
        PARTS_MAX,
    }PARTS;

    // 列挙型宣言(蜘蛛のタイプ)
    typedef enum
    {
        TYPE_NONE = 0,
        TYPE_YELLOW,    // 黄蜘蛛
        TYPE_BLACK,     // 巨大黒蜘蛛
        TYPE_MAX,
    }TYPE;

    // 攻撃AI
    typedef enum
    {
        ATTACK_AI_WAIT = 0,   // 待機
        ATTACK_AI_TAUNT,      // 挑発
        ATTACK_AI_ACID,       // 酸飛ばし
        ATTACK_AI_JUMP,       // ジャンプ
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
    D3DXVECTOR3 SetJumpAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);         // ジャンプ攻撃

    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CSpider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bWarning, TYPE type);

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
