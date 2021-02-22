//====================================================================
//
// モデルエフェクトの処理 (modelEffect.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _MODELEFFECT_H_
#define _MODELEFFECT_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// マクロ定義
//================================================

// 残存時間
#define MODEL_EFFECT_MAX_LIFE 600

// 回転量の最大値/最小値
#define MODEL_EFFECT_ROT_MAX 5000
#define MODEL_EFFECT_ROT_MIN 2000

// 蜘蛛
#define MODEL_EFFECT_SPIDER_SPLATTER_FORCE_MAX 10000                // 打ち上げる最大値
#define MODEL_EFFECT_SPIDER_SPLATTER_FORCE_MIN 5000                 // 打ち上げる最小値

// アリ
#define MODEL_EFFECT_ANT_SPLATTER_FORCE_MAX 7500                    // 打ち上げる最大値
#define MODEL_EFFECT_ANT_SPLATTER_FORCE_MIN 4500                    // 打ち上げる最小値

// プレイヤー
#define MODEL_EFFECT_X_SPLATTER_FORCE_MAX 4500                      // 打ち上げる最大値
#define MODEL_EFFECT_X_SPLATTER_FORCE_MIN 3500                      // 打ち上げる最小値

// UFO
#define MODEL_EFFECT_UFO_EXPLOSION_RADIUS 750.0f                    // 爆発の半径
#define MODEL_EFFECT_UFO_ROT_MAX 1500                               // 回転の最大値
#define MODEL_EFFECT_UFO_ROT_MIN 500                                // 回転の最小値

// ワイバーン
#define MODEL_EFFECT_WYVERN_SPEED_Y_MAX 7000                        // 着地時にガレキが飛んでいく最大速度Y
#define MODEL_EFFECT_WYVERN_SPEED_Y_MIN 5000                        // 着地時にガレキが飛んでいく最小速度Y
#define MODEL_EFFECT_WYVERN_ROT_MAX 500                             // 回転の最大値
#define MODEL_EFFECT_WYVERN_ROT_MIN 250                             // 回転の最小値

// 倒れるもの
#define MODEL_EFFECT_FALL_DOWN_FIRST_SPEED D3DXToRadian(0.001f)     // 最初の速度
#define MODEL_EFFECT_FALL_DOWN_ACCEL_SPEED D3DXToRadian(0.03f)      // 倒れていく速度

// 崩壊するもの
#define MODEL_EFFECT_COLLAPSE_NUMBER_ONCE 5                         // ガレキを一度に生成する数
#define MODEL_EFFECT_SMALL_COLLAPSE_NUMBER_ONCE 3                   // 小さいガレキを一度に生成する数
#define MODEL_EFFECT_SMALL_COLLAPSE_LIFE 180                        // 小さいガレキの残存時間
#define MODEL_EFFECT_BIG_COLLAPSE_RADIUS 150                        // 大きいガレキの発生半径
#define MODEL_EFFECT_COLLAPSE_ROT_MAX 1500                          // ガレキが回転する最高速度
#define MODEL_EFFECT_COLLAPSE_ROT_MIN 500                           // ガレキが回転する最低速度
#define MODEL_EFFECT_COLLAPSE_SPEED_MAX 2500                        // ガレキが飛んでいく最大速度

// 着地時の地割れエフェクト(崩壊するものの派生)
#define MODEL_EFFECT_LANDING_CRACK_SPEED_Y_MAX 3500                 // 着地時にガレキが飛んでいく最大速度Y
#define MODEL_EFFECT_LANDING_CRACK_SPEED_Y_MIN 1500                 // 着地時にガレキが飛んでいく最小速度Y
#define MODEL_EFFECT_LANDING_CRACK_RADIUS 200.0f                    // 着地時にガレキが飛ぶ半径

// 爆発エフェクト(崩壊するものの派生)
#define MODEL_EFFECT_BLUE_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE 1    // 青い爆発時に小さいガレキを一度に生成する数
#define MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV1 15    // 爆発時に小さいガレキを一度に生成する数lv1
#define MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV2 20    // 爆発時に小さいガレキを一度に生成する数lv2
#define MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV3 25    // 爆発時に小さいガレキを一度に生成する数lv3
#define MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV4 30    // 爆発時に小さいガレキを一度に生成する数lv4
#define MODEL_EFFECT_EXPLOSION_SPEED_Y_MAX 7000                     // 爆発時にガレキが飛んでいく最大速度Y
#define MODEL_EFFECT_EXPLOSION_SPEED_Y_MIN 4000                     // 爆発時にガレキが飛んでいく最小速度Y

// 敵を生成する雷
#define MODEL_EFFECT_KAMI_EXTEND_RATE 0.2f                          // 拡大する割合
#define MODEL_EFFECT_KAMI_SCALE_DOWN_RATE 0.6f                      // 縮小する割合
#define MODEL_EFFECT_KAMI_ROT_MAX 300                               // 回転速度最大
#define MODEL_EFFECT_KAMI_ROT_MIN 100                               // 回転速度最小
#define MODEL_EFFECT_KAMI_WHOLE_FRAME 240                           // 全体フレーム
#define MODEL_EFFECT_KAMI_CHANGE_BORDER_FRAME 60                    // 境目のフレーム

// 棘
#define MODEL_EFFECT_SPIKE_EXTEND_RATE 0.2f                         // 拡大する割合
#define MODEL_EFFECT_SPIKE_EXTEND_MAX 10.0f                         // 拡大の最大値
#define MODEL_EFFECT_SPIKE_SPAWN_RATE 5                             // 生成するレート

// 着地するエフェクトの、着地時の傾き量
#define MODEL_EFFECT_TILT_RATE_X D3DXToRadian(-45.0f)
#define MODEL_EFFECT_TILT_RATE_Z D3DXToRadian(-30.0f)

//================================================
// クラス宣言
//================================================

// モデルエフェクトクラス
class CModelEffect : public CScene3D
{
public:

    // モデルエフェクトの種類
    typedef enum
    {
        TYPE_YELLOW_SPIDER_BODY,      // 黄蜘蛛の体
        TYPE_YELLOW_SPIDER_STOMACH,   // 黄蜘蛛の腹
        TYPE_YELLOW_SPIDER_LEG,       // 黄蜘蛛の足
        TYPE_BLACK_SPIDER_BODY,       // 巨大黒蜘蛛の体
        TYPE_BLACK_SPIDER_STOMACH,    // 巨大黒蜘蛛の腹
        TYPE_BLACK_SPIDER_LEG,        // 巨大黒蜘蛛の足
        TYPE_SIGNAL,                  // 信号機
        TYPE_LIGHT,                   // 街灯
        TYPE_LIGHT2,                  // 街灯2
        TYPE_DEBRI_LARGE_001,         // ガレキ大1
        TYPE_DEBRI_LARGE_002,         // ガレキ大2
        TYPE_DEBRI_LARGE_003,         // ガレキ大3
        TYPE_DEBRI_SMALL_001,         // ガレキ小1
        TYPE_DEBRI_SMALL_002,         // ガレキ小2
        TYPE_DEBRI_SMALL_003,         // ガレキ小3
        TYPE_WHITE_ANT_BODY,          // 白アリの体
        TYPE_WHITE_ANT_STOMACH,       // 白アリの腹
        TYPE_WHITE_ANT_LEG,           // 白アリの足
        TYPE_BLACK_ANT_BODY,          // 巨大黒アリの体
        TYPE_BLACK_ANT_STOMACH,       // 巨大黒アリの腹
        TYPE_BLACK_ANT_LEG,           // 巨大黒アリの足
        TYPE_WOOD1,                   // 木1
        TYPE_WOOD2,                   // 木2
        TYPE_WOOD3,                   // 木3
        TYPE_GUARDRAIL,               // ガードレール
        TYPE_SIGN_EMAR,               // 注意サイン
        TYPE_SIGN_INTER,              // 十字路サイン
        TYPE_SIGN_NO_PARK,            // 駐車禁止サイン 
        TYPE_UFO,                     // UFO
        TYPE_UFO_WING,                // UFOの翼
        TYPE_KAMI,                    // 雷
        TYPE_CRYSTAL,                 // クリスタル
        TYPE_METAL,                   // 鉄塔
        TYPE_MINI_CRYSTAL,            // ミニクリスタル
        TYPE_LONG_CRYSTAL,            // ロングクリスタル
        TYPE_X_BODY,                  // X体
        TYPE_X_FOOT,                  // X足
        TYPE_X_LHAND,                 // X左手
        TYPE_X_RHAND,                 // X右手
        TYPE_SPIKE,                   // 棘
        TYPE_TAIL,                    // 尻尾
        TYPE_RIGHT_WING,              // 右翼
        TYPE_LEFT_WING,               // 左翼
        TYPE_WYVERN_BODY,             // ワイバーン体
        TYPE_WYVERN_HEAD,             // ワイバーン頭
        TYPE_WYVERN_NECK,             // ワイバーン首
        TYPE_WYVERN_ARM,              // ワイバーン腕
        TYPE_MAX                      // 種類の最大数
    }TYPE;

    CModelEffect();
    ~CModelEffect();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CModelEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 rotMove, TYPE type);

    void ExplosionEffect(void);                             // 爆発エフェクト
    void SpawnEnemy(void);                                  // 敵をスポーン

private:
    static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // 読み込むテクスチャ
    static LPD3DXMESH				m_apMesh[TYPE_MAX];     // 読み込むメッシュ
    static LPD3DXBUFFER				m_apBuffMat[TYPE_MAX];  // 読み込むマテリアルのバッファ
    static DWORD					m_aNumMat[TYPE_MAX];    // 読み込むマテリアル数

    TYPE m_type;                                            // 種類

    int m_nLife;                                            // 寿命
    D3DXVECTOR3 m_move;                                     // 移動量
    D3DXVECTOR3 m_rotMove;                                  // 回転移動量
    bool m_bShootUp;                                        // 打ち上げるかどうか
    bool m_bFallDown;                                       // 倒れていくかどうか

    bool m_bUseExplosion;                                   // 爆発を使うかどうか
    bool m_bUseLanding;                                     // 着地したかどうか
};

#endif