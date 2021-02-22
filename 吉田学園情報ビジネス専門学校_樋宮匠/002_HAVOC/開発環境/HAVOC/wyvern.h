//===================================================================================
//
// ワイバーンの処理 (wyvern.h)
// Author : 後藤慎之助(行動AI、エフェクト) / 樋宮匠(階層構造、アニメーション制御)
//
//===================================================================================
#ifndef _WYVERN_H_
#define _WYVERN_H_

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

// ワイバーンのスコアや当たり判定の情報
#define WYVERN_SPAWN_POS D3DXVECTOR3(7500.0f, 5000.0f, 9600.0f)                  // 出現位置
#define WYVERN_ACTIVE_COLLISION_SIZE D3DXVECTOR3(1500.0f, 1500.0f, 1500.0f)      // 能動的な当たり判定の大きさ
#define WYVERN_SCORE 300000                                                      // 撃破スコア
#define WYVERN_WING_SCORE 75000                                                  // 羽スコア
#define WYVERN_TAIL_SCORE 100000                                                 // 尻尾スコア
#define WYVERN_ORBS 50                                                           // オーブ数
#define WYVERN_WING_ORBS 75                                                      // 羽オーブ数
#define WYVERN_TAIL_ORBS 100                                                     // 尻尾オーブ数
#define WYVERN_CRUSH_BLOCK 500                                                   // ブロックを破壊するダメージ
#define WYVERN_HEAD_CUBE_COLLISION_SIZE D3DXVECTOR3(575.0f, 300.0f, 575.0f)      // 頭の箱の当たり判定の大きさ
#define WYVERN_HEAD_COLLISION_SIZE D3DXVECTOR3(575.0f, 300.0f, 575.0f)           // 頭の付け根当たり判定の大きさ
#define WYVERN_BODY_COLLISION_SIZE D3DXVECTOR3(1000.0f, 500.0f, 1000.0f)         // 体の当たり判定の大きさ
#define WYVERN_ARM_RIGHT_COLLISION_SIZE D3DXVECTOR3(300.0f, 200.0f, 300.0f)      // 右手の当たり判定の大きさ
#define WYVERN_ARM_LEFT_COLLISION_SIZE D3DXVECTOR3(300.0f, 200.0f, 300.0f)       // 左手の当たり判定の大きさ
#define WYVERN_FOOT_RIGHT_COLLISION_SIZE D3DXVECTOR3(250.0f, 500.0f, 250.0f)     // 右足の当たり判定の大きさ
#define WYVERN_FOOT_LEFT_COLLISION_SIZE D3DXVECTOR3(250.0f, 500.0f, 250.0f)      // 左足の当たり判定の大きさ
#define WYVERN_WING_RIGHT_COLLISION_SIZE D3DXVECTOR3(800.0f, 150.0f, 800.0f)     // 右翼の当たり判定の大きさ
#define WYVERN_WING_LEFT_COLLISION_SIZE D3DXVECTOR3(800.0f, 150.0f, 800.0f)      // 左翼の当たり判定の大きさ
#define WYVERN_TAIL_ROOT_COLLISION_SIZE D3DXVECTOR3(700.0f, 600.0f, 700.0f)      // 尻尾の根の当たり判定の大きさ
#define WYVERN_TAIL_TIP_COLLISION_SIZE D3DXVECTOR3(700.0f, 500.0f, 700.0f)       // 尻尾の中間の当たり判定の大きさ
#define WYVERN_TAIL_CUBE_COLLISION_SIZE D3DXVECTOR3(500.0f, 400.0f, 500.0f)      // 尻尾の箱当たり判定の大きさ
#define WYVERN_ELBOW_RIGHT_COLLISION_SIZE D3DXVECTOR3(400.0f, 200.0f, 400.0f)    // 右肘の当たり判定の大きさ
#define WYVERN_ELBOW_LEFT_COLLISION_SIZE D3DXVECTOR3(400.0f, 200.0f, 400.0f)     // 左肘の当たり判定の大きさ
#define WYVERN_FOOT_CUBE_RIGHT_COLLISION_SIZE D3DXVECTOR3(250.0f, 600.0f, 250.0f)// 右足の箱の当たり判定の大きさ
#define WYVERN_FOOT_CUBE_LEFT_COLLISION_SIZE D3DXVECTOR3(250.0f, 600.0f, 250.0f) // 左足の箱の当たり判定の大きさ
#define WYVERN_LANDING_CRACK_RADIUS 1500.0f                                      // 着地時にガレキが飛ぶ半径
#define WYVERN_LANDING_SMALL_COLLAPSE_NUMBER_ONCE 75                             // 着地時にガレキを飛ばす数

// 回転周り
#define WYVERN_TURN_SPEED D3DXToRadian(1.25f)                                    // 振り向き速度

// ワイバーンが〇距離とみなす値
#define WYVERN_CONSIDER_AS_SHORT 2000.0f                                         // 近距離
#define WYVERN_CONSIDER_AS_MEDIUM 4000.0f                                        // 中距離
#define WYVERN_CONSIDER_AS_LONG 5500.0f                                          // 遠距離

// 防御回り
#define WYVERM_HEAD_DAMAGE_UP_RATE 2.5f                                          // ヘッドショットダメージ上昇割合
#define WYVERN_HEAD_DAMAGE_UP_BORDER 100.0f                                      // ヘッドショットのダメージアップをする境界値
#define WYVERN_ENEMY_EXPLOSION_ATTENUATION_RATE 0.1f                             // 敵の爆発へのダメージ軽減割合
#define WYVERN_LIFE_MAX 15000                                                    // 最大HP
#define WYVERN_WING_DURABILITY 1200                                              // 羽の耐久値
#define WYVERN_TAIL_DURABILITY 3000                                              // 尻尾の耐久値
#define WYVERN_STUN_WHOLE_FRAME 360                                              // スタンの全体フレーム
#define WYVERN_STUN_CREATE_FRAME 120                                             // スタンの生成フレーム
#define WYVERN_STUN_VALUE_MAX 1000                                               // 最大スタン値
#define WYVERN_STUN_ADD_VALUE_HEAD 10                                            // 頭のスタン値上昇量
#define WYVERN_STUN_ADD_VALUE_FOOT 20                                            // 足のスタン値上昇量
#define WYVERN_STUN_VALUE_UP_BORDER 1000.0f                                      // スタン値アップの境界値
#define WYVERN_FLINCH_MAX 3                                                      // 最大怯み回数
#define WYVERN_FLINCH_WHOLE_FRAME 160                                            // 怯みの全体フレーム
#define WYVERN_FLINCH_KNOCKBACK_VALUE 10.0f                                      // 怯みのノックバック量
#define WYVERN_FLINCH_KNOCKBACK_END_FRAME 100                                    // 怯みのノックバック終了フレーム

// 攻撃回り
#define WYVERN_START_ANGRY_LIFE (WYVERN_LIFE_MAX / 4)                            // 怒りに派生する体力
#define WYVERN_SPEED 20.0f                                                       // 速さ
#define WYVERN_SPEED_ANGRY 27.5f                                                 // 怒り中の速さ

// 待機中(距離によって採用の確率を変える)
#define WYVERN_NEXT_ATTACK_COUNT 100                                             // 次の攻撃までのカウンタ
#define WYVERN_FIRST_PRIORITY 40                                                 // 一番目に使われやすい技(100分のなんぼか)
#define WYVERN_SECOND_PRIORITY 30                                                // 二番目に使われやすい技
#define WYVERN_THIRD_PRIORITY 20                                                 // 三番目に使われやすい技
#define WYVERN_FOURTH_PRIORITY 10                                                // 四番目に使われやすい技

// タックル
#define WYVERN_TACKLE_WHOLE_FRAME 180                                            // 全体フレーム

// 飛行
#define WYVERN_FLY_WHOLE_FRAME 240                                               // 全体フレーム
#define WYVERN_FLY_DOWN_SPEED_RATE 0.8f                                          // 速さが下がる割合
#define WYVERN_FLY_START_SOMERSAULT_FRAME 120                                    // サマーソルトまでのクールタイム

// 強襲
#define WYVERN_ASSULT_WHOLE_FRAME 120                                            // 全体フレーム
#define WYVERN_ASSULT_UP_SPEED_FRAME 80                                          // 加速フレーム
#define WYVERN_ASSULT_UP_SPEED_RATE 3.0f                                         // 速さが上がる割合
#define WYVERN_ASSULT_CREATE_FRAME_1 60                                          // クリエイトフレーム1
#define WYVERN_ASSULT_CREATE_FRAME_2 80                                          // クリエイトフレーム2
#define WYVERN_ASSULT_CREATE_FRAME_3 100                                         // クリエイトフレーム3

// サマーソルト
#define WYVERN_SOMERSAULT_WHOLE_FRAME 145                                        // 全体フレーム
#define WYVERN_SOMERSALUT_CREATE_FRAME 85                                        // クリエイトフレーム
#define WYVERN_SOMERSAULT_SHOCK_NUMBER_ONCE 3                                    // 衝撃波を一度に生成する数
#define WYVERN_SOMERSAULT_SHOCK_ANGLE_XZ D3DXToRadian(27.5f)                     // 横の角度
#define WYVERN_SOMERSAULT_DERIVE_FLY_LIFE (WYVERN_LIFE_MAX / 2)                  // 飛行に派生する体力

// 爆砕
#define WYVERN_EXPLOSION_WHOLE_FRAME 300                                         // 全体フレーム
#define WYVERN_EXPLOSION_CREATE_START_FRAME 180                                  // クリエイトスタートフレーム
#define WYVERN_EXPLOSION_CREATE_FRAME 30                                         // クリエイトフレーム
#define WYVERN_EXPLOSION_MAX_RADIUS 3500                                         // 最大半径
#define WYVERN_EXPLOSION_MIN_RADIUS 2500                                         // 最小半径

// 咆哮
#define WYVERN_ROAR_WHOLE_FRAME 175                                              // 全体フレーム
#define WYVERN_ROAR_CREATE_FRAME 105                                             // クリエイトフレーム
#define WYVERN_ROAR_SET_ENEMY_COUNT_BORDER 45                                    // 敵が合計何体以下なら生成するか
#define WYVERN_ROAR_SET_ENEMY_ONCE 5                                             // 同時に何体の敵を生成するか
#define WYVERN_ROAR_SET_ENEMY_SPAWN_POS_Y 2500.0f                                // スポーン地点の高さ
#define WYVERN_ROAR_SET_ENEMY_RADIUS 3000.0f                                     // 攻撃を生成する半径

// 回復アイテムを落とす確率(100分のなんぼか)
#define WYVERN_DROP_HEAL_ITEM 0

//================================================
// 前方宣言
//================================================
class CAnimation;

//================================================
// クラス宣言
//================================================

// ワイバーンクラス
class CWyvern : public CCharacter
{
public:
    CWyvern();
    ~CWyvern();

    // 列挙型宣言(モーション)
    typedef enum
    {
        ANIM_IDLE = 0,      // 待機
        ANIM_WALK,          // 歩き
        ANIM_SHOT,          // 両手連射
        ANIM_ROAR,          // 咆哮
        ANIM_LAND,          // 着地
        ANIM_FLINCH,        // 怯み
        ANIM_STUN,          // ダウン
        ANIM_FLY,           // 飛行
        ANIM_SOMERSAULT,    // サマーソルト
        ANIM_ASSAULT,       // 強襲
        ANIM_EXPLOSION,     // 爆砕
        ANIM_MAX,           // 最大数
    }ANIMATION;

    // 各パーツ
    typedef enum
    {
        PARTS_WEIST = 0,
        PARTS_BODY,
        PARTS_NECK,
        PARTS_HEAD,
        PARTS_TAIL_ROOT,
        PARTS_TAIL_TIP,
        PARTS_ARM_RIGHT,
        PARTS_ARM_LEFT,
        PARTS_WING_RIGHT,
        PARTS_WING_LEFT,
        PARTS_LEG_RIGHT,
        PARTS_FOOT_RIGHT,
        PARTS_LEG_LEFT,
        PARTS_FOOT_LEFT,
        PARTS_CUBE_HEAD,
        PARTS_CUBE_TAIL,
        PARTS_CUBE_ARM_RIGHT,
        PARTS_CUBE_ARM_LEFT,
        PARTS_CUBE_WING_RIGHT,
        PARTS_CUBE_WING_LEFT,
        PARTS_CUBE_ELBOW_RIGHT,
        PARTS_CUBE_ELBOW_LEFT,
        PARTS_CUBE_FOOT_RIGHT,
        PARTS_CUBE_FOOT_LEFT,
        PARTS_MAX,
    }PARTS;

    // 各パーツの当たり判定
    typedef enum
    {
        COLLISION_PARTS_HEAD_CUBE = 0,
        COLLISION_PARTS_HEAD,
        COLLISION_PARTS_BODY,
        COLLISION_PARTS_ARM_RIGHT,
        COLLISION_PARTS_ARM_LEFT,
        COLLISION_PARTS_FOOT_RIGHT,
        COLLISION_PARTS_FOOT_LEFT,
        COLLISION_PARTS_WING_RIGHT,
        COLLISION_PARTS_WING_LEFT,
        COLLISION_PARTS_TAIL_ROOT,
        COLLISION_PARTS_TAIL_TIP,
        COLLISION_PARTS_TAIL_CUBE,
        COLLISION_PARTS_ELBOW_RIGHT,
        COLLISION_PARTS_ELBOW_LEFT,
        COLLISION_PARTS_FOOT_CUBE_RIGHT,
        COLLISION_PARTS_FOOT_CUBE_LEFT,
        COLLISION_PARTS_MAX,
    }COLLISION_PARTS;

    // 攻撃AI
    typedef enum
    {
        ATTACK_AI_WAIT = 0,   // 待機
        ATTACK_AI_TACKLE,     // タックル
        ATTACK_AI_SHOT,       // 射撃
        ATTACK_AI_SOMERSAULT, // サマーソルト
        ATTACK_AI_ROAR,       // 咆哮
        ATTACK_AI_FLY,        // はばたき
        ATTACK_AI_ASSAULT,    // 強襲
        ATTACK_AI_EXPLOSION,  // 爆砕
        ATTACK_AI_MAX,        // 攻撃AIの最大数
    }ATTACK_AI;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Movement(void);
    void UpdateCollisionPos(void);                                       // 当たり判定の位置の更新
    D3DXVECTOR3 RotControl(D3DXVECTOR3 rot);                             // 回転制御
    D3DXVECTOR3 StunOrFlinch(D3DXVECTOR3 pos, D3DXVECTOR3 rot);          // スタンか怯みかの処理
    D3DXVECTOR3 AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot);              // 攻撃AI
    void NextAttackSelect(D3DXVECTOR3 pos);                              // 次の攻撃を選択
    D3DXVECTOR3 Dush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                  // 走る
    void SetShockWave(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                 // 衝撃波攻撃
    void SetExplosion(bool bUseTracking, bool bFall);                    // 爆発攻撃
    void SetEnemySpawn(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                // 敵生成攻撃

    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CWyvern *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

    void SetLandingEffect(D3DXVECTOR3 pos);                                                 // 着地エフェクト
    void ScatterOrbs(int nOrbs);                                                            // オーブをバラまく
    void DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                                      // やられた時の粉砕
    void SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type);    // やられた時の粉砕エフェクトの発生
    void TakeDamage(D3DXVECTOR3 pos, int nDamage, int nCntCollision);                       // ダメージを受けたときの処理
    void AddMissionClear(void);                                                             // ミッションクリア数を追加する

private:
    static LPD3DXMESH		m_pMesh[PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[PARTS_MAX];
    static DWORD			m_nNumMat[PARTS_MAX];

    D3DXVECTOR3 m_move;                        // 移動量
    int m_nScore;                              // スコア
    int m_nOrbs;                               // オーブ

    int m_nCntTime;                            // 時間を数える

    float m_fSpeed;                            // 速さ
    D3DXVECTOR3 m_rotDest;                     // 向き(目的値)
    D3DXVECTOR3 m_rotMin;                      // 向きを変えるときに、最短で振り向くため使用

    ATTACK_AI m_attackAI;                      // 攻撃AI

    bool m_bUseRotControl;                     // 向きの制御を使うかどうか

    bool m_bStartBattle;                       // バトルが開始したかどうか

    int m_nRightWingDurabirity;                // 右羽耐久値
    int m_nLeftWingDurabirity;                 // 左羽耐久値
    int m_nTailDurabirity;                     // 尻尾耐久値
    bool m_bDestroyRightWing;                  // 右羽を破壊したかどうか
    bool m_bDestroyLeftWing;                   // 左羽を破壊したかどうか
    bool m_bDestroyTail;                       // 尻尾を破壊したかどうか

    bool m_bFlySomersault;                     // 飛行中にサマーソルトをしたかどうか
    bool m_bPrevRoar;                          // 前に咆哮をしたかどうか

    bool m_bStun;                              // スタンしているかどうか
    int m_nStunValue;                          // スタン値
    int m_nCntFlinch;                          // 怯み回数加算
};

#endif