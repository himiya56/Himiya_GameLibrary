//====================================================================
//
// ブロックの処理 (block.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"
#include "minimap.h"

//================================================
// マクロ定義
//================================================

// 壁キックで受けるダメージ
#define BLOCK_TAKE_DAMAGE_BY_WALL_KICK 50

// 向きによって当たり判定が変わる
#define BLOCK_HORIZONTAL_COLLISION(angle) angle <= D3DXToRadian(45.0f) && angle >= D3DXToRadian(-45.0f) || angle >= D3DXToRadian(135.0f) && angle <= D3DXToRadian(225.0f) || angle >= D3DXToRadian(-225.0f) && angle <= D3DXToRadian(-135.0f)
#define BLOCK_VERTICAL_COLLISION(angle) angle < D3DXToRadian(135.0f) && angle > D3DXToRadian(45.0f) || angle > D3DXToRadian(225.0f) && angle < D3DXToRadian(315.0f) || angle > D3DXToRadian(-135.0f) && angle < D3DXToRadian(-45.0f)|| angle < D3DXToRadian(-225.0f) && angle > D3DXToRadian(-315.0f)

// ビル
#define BLOCK_BUILDING_COLLISION_SIZE D3DXVECTOR3(630.0f, 1650.0f, 630.0f)
#define BLOCK_BUILDING_LIFE 200
#define BLOCK_BUILDING_SCORE 5000
#define BLOCK_BUILDING_ORBS 22

// 信号機
#define BLOCK_SIGNAL_COLLISION_SIZE D3DXVECTOR3(100.0f, 1100.0f, 100.0f)
#define BLOCK_SIGNAL_LIFE 50
#define BLOCK_SIGNAL_SCORE 4500
#define BLOCK_SIGNAL_ORBS 13

// 街灯
#define BLOCK_LIGHT_COLLISION_SIZE D3DXVECTOR3(100.0f, 1050.0f, 100.0f)
#define BLOCK_LIGHT_LIFE 50
#define BLOCK_LIGHT_SCORE 4500
#define BLOCK_LIGHT_ORBS 13

// ゴミ箱
#define BLOCK_DUSTBOX_COLLISION_SIZE D3DXVECTOR3(80.0f, 120.0f, 80.0f)
#define BLOCK_DUSTBOX_LIFE 20
#define BLOCK_DUSTBOX_SCORE 20000
#define BLOCK_DUSTBOX_ORBS 7

// 木
#define BLOCK_WOOD_COLLISION_SIZE D3DXVECTOR3(100.0f, 900.0f, 100.0f)
#define BLOCK_WOOD2_COLLISION_SIZE D3DXVECTOR3(150.0f, 1100.0f, 150.0f)
#define BLOCK_WOOD_LIFE 50
#define BLOCK_WOOD_SCORE 4500
#define BLOCK_WOOD_ORBS 13

// ベンチ
#define BLOCK_BENCH_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(330.0f, 100.0f, 170.0f)
#define BLOCK_BENCH_VERTICAL_COLLISION_SIZE D3DXVECTOR3(170.0f, 100.0f, 330.0f)
#define BLOCK_BENCH_LIFE 30
#define BLOCK_BENCH_SCORE 2000
#define BLOCK_BENCH_ORBS 8

// 机
#define BLOCK_TUKUE_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(440.0f, 180.0f, 260.0f)
#define BLOCK_TUKUE_VERTICAL_COLLISION_SIZE D3DXVECTOR3(260.0f, 180.0f, 440.0f)
#define BLOCK_TUKUE_LIFE 50
#define BLOCK_TUKUE_SCORE 2500
#define BLOCK_TUKUE_ORBS 9

// ガードレール
#define BLOCK_GUARDRAIL_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(600.0f, 150.0f, 100.0f)
#define BLOCK_GUARDRAIL_VERTICAL_COLLISION_SIZE D3DXVECTOR3(100.0f, 150.0f, 600.0f)
#define BLOCK_GUARDRAIL_LIFE 50
#define BLOCK_GUARDRAIL_SCORE 3500
#define BLOCK_GUARDRAIL_ORBS 11
#define BLOCK_GUARDRAIL_ROT_MOVE D3DXVECTOR3(D3DXToRadian(1.0f), 0.0f, 0.0f)

// ジャングルジム
#define BLOCK_PLAYGROUND_COLLISION_SIZE D3DXVECTOR3(500.0f, 500.0f, 500.0f)
#define BLOCK_PLAYGROUND_LIFE 100
#define BLOCK_PLAYGROUND_SCORE 4500
#define BLOCK_PLAYGROUND_ORBS 16

// 注意サイン
#define BLOCK_SIGN_EMAR_COLLISION_SIZE D3DXVECTOR3(100.0f, 600.0f, 100.0f)
#define BLOCK_SIGN_EMAR_LIFE 50
#define BLOCK_SIGN_EMAR_SCORE 5500
#define BLOCK_SIGN_EMAR_ORBS 11

// 十字路サイン
#define BLOCK_SIGN_INTER_COLLISION_SIZE D3DXVECTOR3(100.0f, 600.0f, 100.0f)
#define BLOCK_SIGN_INTER_LIFE 50
#define BLOCK_SIGN_INTER_SCORE 4000
#define BLOCK_SIGN_INTER_ORBS 10

// 駐車禁止サイン
#define BLOCK_SIGN_NO_PARK_COLLISION_SIZE D3DXVECTOR3(100.0f, 600.0f, 100.0f)
#define BLOCK_SIGN_NO_PARK_LIFE 50
#define BLOCK_SIGN_NO_PARK_SCORE 4000
#define BLOCK_SIGN_NO_PARK_ORBS 10

// 車
#define BLOCK_CAR_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(850.0f, 250.0f, 450.0f)
#define BLOCK_CAR_VERTICAL_COLLISION_SIZE D3DXVECTOR3(450.0f, 250.0f, 850.0f)
#define BLOCK_CAR_LIFE 120
#define BLOCK_CAR_SCORE 5500
#define BLOCK_CAR_ORBS 18

// コンビニ
#define BLOCK_CONVINI_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(1500.0f, 650.0f, 800.0f)
#define BLOCK_CONVINI_VERTICAL_COLLISION_SIZE D3DXVECTOR3(800.0f, 650.0f, 1500.0f)
#define BLOCK_CONVINI_LIFE 150
#define BLOCK_CONVINI_SCORE 6500
#define BLOCK_CONVINI_ORBS 24

// コンテナ
#define BLOCK_CONTAINER_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(1700.0f, 550.0f, 600.0f)
#define BLOCK_CONTAINER_VERTICAL_COLLISION_SIZE D3DXVECTOR3(600.0f, 550.0f, 1700.0f)
#define BLOCK_CONTAINER_LIFE 120
#define BLOCK_CONTAINER_SCORE 7500
#define BLOCK_CONTAINER_ORBS 27

// ガスタンク
#define BLOCK_GAS_COLLISION_SIZE D3DXVECTOR3(550.0f, 680.0f, 550.0f)
#define BLOCK_GAS_LIFE 120
#define BLOCK_GAS_SCORE 8500
#define BLOCK_GAS_ORBS 30

// 倉庫
#define BLOCK_SOUKO_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(2000.0f, 600.0f, 1500.0f)
#define BLOCK_SOUKO_VERTICAL_COLLISION_SIZE D3DXVECTOR3(1500.0f, 600.0f, 2000.0f)
#define BLOCK_SOUKO_LIFE 250
#define BLOCK_SOUKO_SCORE 15000
#define BLOCK_SOUKO_ORBS 40

// 鉄塔
#define BLOCK_METAL_COLLISION_SIZE D3DXVECTOR3(250.0f, 2000.0f, 250.0f)
#define BLOCK_METAL_LIFE 200
#define BLOCK_METAL_SCORE 10000
#define BLOCK_METAL_ORBS 35

// ソーラーパネル
#define BLOCK_SOLAR_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(950.0f, 350.0f, 800.0f)
#define BLOCK_SOLAR_VERTICAL_COLLISION_SIZE D3DXVECTOR3(800.0f, 350.0f, 950.0f)
#define BLOCK_SOLAR_LIFE 100
#define BLOCK_SOLAR_SCORE 5000
#define BLOCK_SOLAR_ORBS 50

// タワー(未使用)
#define BLOCK_TOWER_COLLISION_SIZE D3DXVECTOR3(500.0f, 3400.0f, 500.0f)
#define BLOCK_TOWER_LIFE 50
#define BLOCK_TOWER_SCORE 4000
#define BLOCK_TOWER_ORBS 12

// クリスタル
#define BLOCK_CRYSTAL_COLLISION_SIZE D3DXVECTOR3(300.0f, 350.0f, 300.0f)
#define BLOCK_CRYSTAL_LIFE 350
#define BLOCK_CRYSTAL_SCORE 50000
#define BLOCK_CRYSTAL_ORBS 35
#define BLOCK_CRYSTAL_SHOOT_UP_MOVE D3DXVECTOR3(0.0f, 50.0f, 0.0f)                                              // 破壊時に打ち上げる移動量
#define BLOCK_CRYSTAL_SHOOT_UP_ROT_MOVE D3DXVECTOR3(D3DXToRadian(5.0f), D3DXToRadian(1.5f), D3DXToRadian(1.5f)) // 破壊時に打ち上げる回転量

//================================================
// クラス宣言
//================================================

// ブロッククラス
class CBlock : public CScene3D
{
public:

    // ブロックの種類
    typedef enum
    {
        TYPE_BUILDING001,   // ビル1
        TYPE_BUILDING101,   // ビル2
        TYPE_BUILDING201,   // ビル3
        TYPE_BUILDING301,   // ビル4
        TYPE_BUILDING401,   // ビル5
        TYPE_BUILDING501,   // ビル6
        TYPE_SIGNAL,        // 信号機
        TYPE_LIGHT,         // 街灯
        TYPE_DUSTBOX,       // ゴミ箱
        TYPE_LIGHT2,        // 街灯2
        TYPE_WOOD1,         // 木1
        TYPE_WOOD2,         // 木2
        TYPE_WOOD3,         // 木3
        TYPE_BENCH,         // ベンチ
        TYPE_TUKUE,         // 机
        TYPE_GUARDRAIL,     // ガードレール
        TYPE_PLAYGROUND,    // ジャングルジム
        TYPE_SIGN_EMAR,     // 注意サイン
        TYPE_SIGN_INTER,    // 十字路サイン
        TYPE_SIGN_NO_PARK,  // 駐車禁止サイン 
        TYPE_CAR,           // 車
        TYPE_CONVINI,       // コンビニ
        TYPE_CONTAINER,     // コンテナ
        TYPE_GAS,           // ガスタンク
        TYPE_SOUKO,         // 倉庫
        TYPE_METAL,         // 鉄塔
        TYPE_SOLAR,         // ソーラーパネル
        TYPE_TOWER,         // タワー
        TYPE_CRYSTAL,       // クリスタル
        TYPE_MAX            // 種類の最大数
    }TYPE;

    // ラストヒット
    typedef enum
    {
        LAST_HIT_PLAYER = 0,	// プレイヤーの攻撃
        LAST_HIT_ENEMY,	        // 敵の攻撃
    }LAST_HIT;

    CBlock();
    ~CBlock();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

    D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }          // 衝突の大きさを返す
    bool GetWallRun(void) { return m_bWallRun; }                            // 壁走りできるかどうかを返す
    bool GetEnemyWallRun(void) { return m_bEnemyWallRan; }                  // 敵が壁走りできるかどうかを返す
    int GetType(void) { return m_nType; }                                   // 種類を返す

    void SetTakeDamage(const int nDamage, const LAST_HIT lastHit) { m_nLife = m_nLife - nDamage; m_lastHit = lastHit; }
    void ScatterOrbs(void);                                                 // オーブをバラまく
    void DestroyEffect(void);                                               // 倒壊エフェクト
    void ExplosionEffect(void);                                             // 爆発エフェクト
    void CollapseEffect(void);                                              // 崩壊エフェクト
    void FallDownModel(void);                                               // 横に倒れるモデル
    void AddMissionClear(void);                                             // ミッションクリア数を追加する

private:
    static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // 読み込むテクスチャ
    static LPD3DXMESH				m_apMesh[TYPE_MAX];     // 読み込むメッシュ
    static LPD3DXBUFFER				m_apBuffMat[TYPE_MAX];  // 読み込むマテリアルのバッファ
    static DWORD					m_aNumMat[TYPE_MAX];    // 読み込むマテリアル数

    int m_nType;                                            // 種類
    D3DXVECTOR3 m_collisionSize;                            // 当たり判定の大きさ
    bool m_bWallRun;                                        // 壁走りできるかどうか
    bool m_bEnemyWallRan;                                   // 敵が壁走りをできるかどうか

    int m_nLife;                                            // ライフ
    int m_nScore;                                           // スコア
    int m_nOrbs;                                            // オーブ
    bool m_bUseDestroyEffect;                               // 倒壊エフェクトを使うかどうか

    bool m_bUseExplosion;                                   // 爆発を使うかどうか

    LAST_HIT m_lastHit;                                     // 最後に攻撃してきたもの

    CMinimap *m_pMinimap;									// ミニマップ上のアイコンへのポインタ
};

#endif