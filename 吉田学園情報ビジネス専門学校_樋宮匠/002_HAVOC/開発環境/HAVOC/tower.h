//===================================================================================
//
// タワーの処理 (tower.h)
// Author : 後藤慎之助(行動AI、エフェクト) / 樋宮匠(階層構造、アニメーション制御)
//
//===================================================================================
#ifndef _TOWER_H_
#define _TOWER_H_

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

// タワー
#define TOWER_SPAWN_POS D3DXVECTOR3(10445.0f, 5000.0f, 6400.0f)                 // 出現位置
#define TOWER_ACTIVE_COLLISION_SIZE D3DXVECTOR3(500.0f, 3400.0f, 500.0f)        // 能動的な当たり判定の大きさ
#define TOWER_LIFE 6000                                                         // HP
#define TOWER_SCORE 100000                                                      // スコア
#define TOWER_ORBS 30                                                           // オーブ数
#define TOWER_COLLISION_MAX 2                                                   // 当たり判定の最大数
#define TOWER_PILLER_COLLISION_SIZE D3DXVECTOR3(600.0f, 2200.0f, 600.0f)        // 柱の当たり判定の大きさ
#define TOWER_CRYSTAL_COLLISION_SIZE D3DXVECTOR3(550.0f, 1200.0f, 550.0f)       // クリスタルの当たり判定の大きさ
#define TOWER_CRYSTAL_ROT_SPEED D3DXToRadian(1.0f)                              // クリスタルの回転速度
#define TOWER_LANDING_CRACK_RADIUS 1000.0f                                      // 着地時にガレキが飛ぶ半径
#define TOWER_LANDING_SMALL_COLLAPSE_NUMBER_ONCE 50                             // 着地時にガレキを飛ばす数

// 攻撃回り
#define TOWER_ENEMY_SPAWN_POS_Y 3500.0f                                         // スポーン地点の高さ
#define TOWER_ATTACK_RADIUS 1200.0f                                             // 攻撃を生成する半径
#define TOWER_SET_ENEMY_SPAWN_COUNT 180                                         // 敵を生成するまでのカウンタ
#define TOWER_SET_ENEMY_COUNT_BORDER 60                                         // 敵が合計何体以下なら生成するか

// 防御回り
#define TOWER_PILLER_REDUCTION_RATE 0.5f                                        // 柱へのダメージ軽減割合
#define TOWER_CRYSTAL_DAMAGE_UP_RATE 3.0f                                       // クリスタルへのダメージアップ倍率

// 回復アイテムを落とす確率(100分のなんぼか)
#define TOWER_DROP_HEAL_ITEM 100

//================================================
// 前方宣言
//================================================
class CAnimation;

//================================================
// クラス宣言
//================================================

// タワークラス
class CTower : public CCharacter
{
public:
    CTower();
    ~CTower();

    // 列挙型宣言(モーション)
    typedef enum
    {
        ANIM_IDLE = 0,      // 待機
        ANIM_MAX,           // 最大数
    }ANIMATION;

    // 列挙型宣言
    typedef enum
    {
        PARTS_PILLER = 0,
        PARTS_CRYSTAL,
        PARTS_MAX,
    }PARTS;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Movement(void);
    void SetEnemySpawn(void);                                            // 敵生成地点を設定
    void UpdateCollisionPos(void);                                       // 当たり判定の位置の更新

    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CTower *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

    void SetLandingEffect(D3DXVECTOR3 pos);                                                 // 着地エフェクト
    void ScatterOrbs(void);                                                                 // オーブをバラまく
    void DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                                      // やられた時の粉砕
    void CollapseEffect(void);                                                              // 崩壊エフェクト
    void SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type);    // やられた時の粉砕エフェクトの発生
    void TakeDamage(D3DXVECTOR3 pos, int nDamage, PARTS parts);                             // ダメージを受けたときの処理
    void AddMissionClear(void);                                                             // ミッションクリア数を追加する

private:
    static LPD3DXMESH		m_pMesh[PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[PARTS_MAX];
    static DWORD			m_nNumMat[PARTS_MAX];

    D3DXVECTOR3 m_move;                        // 移動量
    int m_nScore;                              // スコア
    int m_nOrbs;                               // オーブ

    int m_nCntTime;                            // 時間を数える

    D3DXVECTOR3 m_rotCrystal;				   // クリスタルのrot

    bool m_bLanding;                           // 着地したかどうか
};

#endif