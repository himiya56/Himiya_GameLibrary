//====================================================================
//
// 弾の処理 (bullet.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// マクロ定義
//================================================

// 中立の弾が、敵やブロックを攻撃するときのダメージ減衰量
#define BULLET_NEUTRAL_ATTENUATION_FOR_ENEMY_RATE 0.1f                          // 敵に対してのダメージ減衰量
#define BULLET_NEUTRAL_ATTENUATION_FOR_BLOCK_RATE 0.25f                         // ブロックに対してのダメージ減衰量

// アサルトライフルの弾
#define BULLET_AR_COLLISION_SIZE D3DXVECTOR3(27.5f, 27.5f, 27.5f)               // 当たり判定の大きさ
#define BULLET_AR_SPEED 100.0f                                                  // 速さ(100?)(速さ×寿命を、5000以内に抑える)
#define BULLET_AR_COOL_TIME 6                                                   // クールタイム(6?)
#define BULLET_AR_LIFE 50                                                       // 寿命(50?)
#define BULLET_AR_DAMAGE 14                                                     // ダメージ(14?)
#define BULLET_AR_TRAIL_SPEED -10.0f                                            // 尾を引く速さ

// 血
#define BULLET_BLOOD_COLLISION_SIZE D3DXVECTOR3(27.5f, 27.5f, 27.5f)            // 当たり判定の大きさ
#define BULLET_BLOOD_SPEED 12.5f                                                // 速さ
#define BULLET_BLOOD_LIFE 300                                                   // 寿命
#define BULLET_BLOOD_TRAIL_SPEED -5.0f                                          // 尾を引く速さ

// 酸
#define BULLET_ACID_COLLISION_SIZE D3DXVECTOR3(150.0f, 150.0f, 150.0f)          // 当たり判定の大きさ
#define BULLET_ACID_SPEED 50.0f                                                 // 速さ
#define BULLET_ACID_LIFE 600                                                    // 寿命
#define BULLET_ACID_DAMAGE 150                                                  // ダメージ
#define BULLET_ACID_TRAIL_SPEED -5.0f                                           // 尾を引く速さ

// 左手の爆弾
#define BULLET_BOMB_COLLISION_SIZE_LV1 60.0f                                    // 当たり判定の大きさlv1
#define BULLET_BOMB_COLLISION_SIZE_LV2 85.0f                                    // 当たり判定の大きさlv2
#define BULLET_BOMB_COLLISION_SIZE_LV3 110.0f                                   // 当たり判定の大きさlv3
#define BULLET_BOMB_COLLISION_SIZE_LV4 135.0f                                   // 当たり判定の大きさlv4
#define BULLET_BOMB_FIRST_SPEED 100.0f                                          // 速さ
#define BULLET_BOMB_LIFE 150                                                    // 寿命
#define BULLET_BOMB_DAMAGE 1                                                    // ダメージ
#define BULLET_BOMB_TRAIL_SPEED -10.0f                                          // 尾を引く速さ
#define BULLET_BOMB_FALL_DOWN_RATE -0.325f                                      // 弾道落下割合

// 爆発(レベル別)
#define BULLET_EXPLOSION_COLLISION_SIZE_LV1 750.0f                              // 当たり判定の大きさlv1
#define BULLET_EXPLOSION_COLLISION_SIZE_LV2 1125.0f                             // 当たり判定の大きさlv2
#define BULLET_EXPLOSION_COLLISION_SIZE_LV3 1687.5f                             // 当たり判定の大きさlv3
#define BULLET_EXPLOSION_COLLISION_SIZE_LV4 2531.25f                            // 当たり判定の大きさlv4
#define BULLET_EXPLOSION_LIFE_LV1 45                                            // 寿命lv1
#define BULLET_EXPLOSION_LIFE_LV2 60                                            // 寿命lv2
#define BULLET_EXPLOSION_LIFE_LV3 75                                            // 寿命lv3
#define BULLET_EXPLOSION_LIFE_LV4 90                                            // 寿命lv4
#define BULLET_EXPLOSION_DAMAGE_LV1 300                                         // ダメージlv1
#define BULLET_EXPLOSION_DAMAGE_LV2 600                                         // ダメージlv2
#define BULLET_EXPLOSION_DAMAGE_LV3 1200                                        // ダメージlv3
#define BULLET_EXPLOSION_DAMAGE_LV4 2400                                        // ダメージlv4
#define BULLET_EXPLOSION_ATTENUATION_FOR_PLAYER_RATE 0.35f                      // プレイヤーに対してのダメージ減衰量
#define BULLET_EXPLOSION_SIZE_ATTENUATION_FOR_PLAYER_RATE 0.5f                  // プレイヤーに対してのサイズ減衰量
#define BULLET_EXPLOSION_START_COLLISION 10                                     // 当たり判定が発生するまでの時間
#define BULLET_EXPLOSION_CREATE_SMALL_COLLAPSE_FRAME 2                          // ガレキを生成する時間
#define BULLET_EXPLOSION_USE_COLLISION_FRAME_FOR_BOSS 10                        // ボスに対して、当たり判定を発生させるフレーム

// 防衛軍の弾
#define BULLET_CLAP_COLLISION_SIZE D3DXVECTOR3(30.0f, 30.0f, 30.0f)             // 当たり判定の大きさ
#define BULLET_CLAP_SPEED 100.0f                                                // 速さ
#define BULLET_CLAP_LIFE 60                                                     // 寿命
#define BULLET_CLAP_DAMAGE 100                                                  // ダメージ
#define BULLET_CLAP_TRAIL_SPEED -10.0f                                          // 尾を引く速さ

// 衝撃波
#define BULLET_SHOCK_COLLISION_SIZE D3DXVECTOR3(2531.25f, 2531.25f, 2531.25f)   // 大きさ
#define BULLET_SHOCK_COLLISION_REAL_SIZE D3DXVECTOR3(940.0f, 1200.0f, 940.0f)   // 実際の当たり判定の大きさ
#define BULLET_SHOCK_SPEED 75.0f                                                // 速さ
#define BULLET_SHOCK_LIFE 75                                                    // 寿命
#define BULLET_SHOCK_DAMAGE 250                                                 // ダメージ

//================================================
// クラス宣言
//================================================

// 弾クラス
class CBullet : public CScene3D
{
public:

    // ヒットエフェクト
    typedef enum
    {
        HIT_LIVING,        // 生物
        HIT_NON_LIVING,    // 非生物
        HIT_HUMAN,         // 人間
        HIT_CRYSTAL,       // クリスタル
        HIT_MAX            // 種類の最大数
    }HIT;

    // 弾の種族
    typedef enum
    {
        TRIBE_PLAYER,   // プレイヤー
        TRIBE_ENEMY,    // エネミー
        TRIBE_LIQUID,   // 液体
        TRIBE_NEUTRAL,  // 中立
        TRIBE_MAX       // 種類の最大数
    }TRIBE;

    // 弾の種類
    typedef enum
    {
        TYPE_AR,        // アサルトライフル
        TYPE_BLOOD,     // 血
        TYPE_ACID,      // 酸
        TYPE_BOMB,      // 左手の爆弾
        TYPE_EXPLOSION, // 爆発
        TYPE_CLAP,      // 防衛軍の弾
        TYPE_SHOCK,     // 衝撃波
        TYPE_MAX        // 種類の最大数
    }TYPE;

    CBullet();
    ~CBullet();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    bool Collision(D3DXVECTOR3 pos);
    void ExplosionCollision(D3DXVECTOR3 pos);
    void SetExplosion(D3DXVECTOR3 pos);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 collisionSize, D3DXVECTOR3 moveAngle, TYPE type, TRIBE tribe);

    D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }                  // 衝突の大きさを返す
    D3DXVECTOR3 GetMiddlePos(D3DXVECTOR3 pos, D3DXVECTOR3 posOld);                  // 中間の位置を返す

    void HitEffect(D3DXVECTOR3 pos, HIT hit);
    void AREffect(D3DXVECTOR3 pos);
    void BloodEffect(D3DXVECTOR3 pos);
    void AcidEffect(D3DXVECTOR3 pos);
    void BombEffect(D3DXVECTOR3 pos);
    void ExplosionEffect(D3DXVECTOR3 pos);
    void ClapEffect(D3DXVECTOR3 pos);
    void ShockEffect(D3DXVECTOR3 pos);

private:
    static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // 読み込むテクスチャ
    static LPD3DXMESH				m_apMesh[TYPE_MAX];     // 読み込むメッシュ
    static LPD3DXBUFFER				m_apBuffMat[TYPE_MAX];  // 読み込むマテリアルのバッファ
    static DWORD					m_aNumMat[TYPE_MAX];    // 読み込むマテリアル数

    int m_nLife;                                            // 寿命
    int m_nDamage;                                          // ダメージ
    float m_fSpeed;                                         // 速さ
    D3DXVECTOR3 m_moveAngle;                                // 移動の角度
    TYPE m_type;                                            // 種類
    TRIBE m_tribe;                                          // 種族
    D3DXVECTOR3 m_collisionSize;                            // 当たり判定の大きさ
    D3DXVECTOR3 m_posOld;                                   // 前の位置

    int m_nCntTime;                                         // 生成されてからの時間を数える
};

#endif