//=============================================================================
//
// エフェクト3D処理 [effect3d.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// エフェクトの共通設定
#define EFFECT_FIRE_ALPHA_TEST_BORDER 25                                         // 炎のα値のボーダー値
#define EFFECT_FLOAT_FROM_SURFACE_POS 5.0f                                       // 面から浮かせる値

// 赤い炎
#define EFFECT_RED_FIRE_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)                     // 赤い炎の大きさ
#define EFFECT_RED_FIRE_FIRST_COLOR D3DXCOLOR(1.0f,0.99f,0.0f,1.0f)              // 赤い炎の色
#define EFFECT_YELLOW_FIRE_TO_RED_RATE 0.05f                                     // 赤に近づけていく割合
#define EFFECT_RED_FIRE_FADE_OUT_RATE 0.02f                                      // 赤い炎のフェードアウト割合

// ARのトレイル
#define EFFECT_AR_TRAIL_COLOR D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)                     // ARのトレイルの色
#define EFFECT_AR_TRAIL_FADE_OUT_RATE 5.0f                                       // ARのトレイルのサイズ縮小量

// ARのトレイルの飾り
#define EFFECT_AR_TRAIL_DECORATION_COLOR D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)          // ARのトレイルの飾りの色
#define EFFECT_AR_TRAIL_DECORATION_FADE_OUT_RATE 5.0f                            // ARのトレイルの飾りのサイズ縮小量
#define EFFECT_AR_TRAIL_DECORATION_MAX_SPEED 1000                                // ARのトレイルの飾りのサイズ縮小量

// ウェーブ(未使用)
#define EFFECT_WAVE_EXTEND_SIZE 20.0f
#define EFFECT_WAVE_FADE_OUT_RATE 0.005f

// チャージ/紫の炎
#define EFFECT_CHARGE_SIZE D3DXVECTOR3(80.0f, 80.0f, 0.0f)                       // チャージの大きさ
#define EFFECT_CHARGE_SPEED 35.0f                                                // チャージの速さ(もとは30.0f)
#define EFFECT_CHARGE_CONSIDER_CLOSE_DEST 40.0f                                  // チャージの目的地に近いとみなす値
#define EFFECT_CHARGE_CONSIDER_CLOSE_DEST_Y 40.0f                                // チャージの目的地に近いとみなす値(縦)
#define EFFECT_CHARGE_SPREAD_MAX_X 700                                           // チャージの拡散の最大値X
#define EFFECT_CHARGE_SPREAD_MAX_Y 1000                                          // チャージの拡散の最大値Y
#define EFFECT_CHARGE_SPREAD_MIN_Y 100                                           // チャージの拡散の最小値Y
#define EFFECT_CHARGE_SPREAD_MAX_Z 700                                           // チャージの拡散の最大値Z
#define EFFECT_CHARGE_START_HOMING 90                                            // チャージのホーミングが始まるフレーム数
#define EFFECT_CHARGE_TRAIL_COLOR D3DXCOLOR(1.0f,1.0f,0.0f,1.0f)                 // チャージのトレイルの色
#define EFFECT_CHARGE_TRAIL_FADE_OUT_RATE 2.5f                                   // チャージのトレイルのパーティクルのサイズ縮小量
#define EFFECT_CHARGE_TRAIL_MOVE_CHANGE_RATE (-0.4f)                             // チャージからトレイルの移動量を変えるときの割合
#define EFFECT3D_PURPLE_FIRE_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)
#define EFFECT3D_PURPLE_FIRE_FIRST_COLOR D3DXCOLOR(0.835f,0.901f,1.0f,1.0f)      // 紫の炎の色
#define EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_R 0.025f                              // 紫に近づけていく割合R
#define EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_G 0.05f                               // 紫に近づけていく割合G
#define EFFECT3D_PURPLE_FIRE_FADE_OUT_RATE 0.01f                                 // 紫の炎のフェードアウト割合

// 射撃エフェクト
#define EFFECT_SCATTERING_BULLET_MAX_SIZE 9000                                   // 最大サイズ
#define EFFECT_SCATTERING_BULLET_MIN_SIZE 5000                                   // 最小サイズ
#define EFFECT_SCATTERING_BULLET_MAX_SPEED 900                                   // 最大速度
#define EFFECT_SCATTERING_BULLET_MIN_SPEED 400                                   // 最小速度
#define EFFECT_SCATTERING_BULLET_NUMBER_ONCE 15                                  // 一度の個数15
#define EFFECT_SCATTERING_BULLET_COLOR D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)            // 色
#define EFFECT_SCATTERING_BULLET_FADE_OUT_RATE 5.0f                              // パーティクルのサイズ縮小量5.0f

// 煙エフェクト
#define EFFECT_SMOKE_MAX_PATTERN 8                                               // 煙の最大パターン
#define EFFECT_SMOKE_UPDATE_PATTERN_SPEED 2                                      // 煙のパターン更新速度
#define EFFECT_SMOKE_COLOR_TO_BLACK_RATE 0.01f                                   // 煙の黒に近づけていく割合
#define EFFECT_DESTROY_FADE_OUT_RATE 0.005f                                      // 破壊時の煙の消えていく割合
#define EFFECT_DESTROY_MAX_SIZE 120000                                           // 破壊時の煙の最大サイズ
#define EFFECT_DESTROY_MIN_SIZE 60000                                            // 破壊時の煙の最小サイズ
#define EFFECT_DESTROY_NUMBER_ONCE 20                                            // 破壊時の煙の一度の個数
#define EFFECT_DESTROY_MAX_SPEED 800                                             // 破壊時の煙の最大速度
#define EFFECT_DESTROY_MIN_SPEED 300                                             // 破壊時の煙の最小速度

// 酸エフェクト
#define EFFECT_ACID_FIRST_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)                   // 酸の最初の大きさ
#define EFFECT_ACID_FIRST_COLOR D3DXCOLOR(0.6f, 1.0f, 0.0f, 1.0f)                // 酸の最初の色
#define EFFECT_ACID_TO_GREEN_RATE_R 0.005f                                       // 酸の黄色に近づけていく値R
#define EFFECT_ACID_EXTEND_SIZE 10.0f                                            // 酸の拡大割合
#define EFFECT_ACID_FADE_OUT_RATE 0.005f                                         // 酸の消えていく割合
#define EFFECT_ACID_FLOAT_FROM_SURFACE_POS 6.0f                                  // 酸の表面から離す値

// 酸攻撃エフェクト
#define EFFECT_ACID_ATTACK_FIRST_COLOR D3DXCOLOR(0.36f, 0.282f, 0.815f, 1.0f)    // 酸攻撃の最初の色
#define EFFECT_ACID_ATTACK_TO_PURPLE_RATE 0.0025f                                // 酸攻撃の紫色に近づけていく値

// 酸攻撃のトレイルの飾り
#define EFFECT_ACID_ATTACK_TRAIL_DECORATION_COLOR D3DXCOLOR(0.811f,0.415f,1.0f,1.0f) // 酸攻撃のトレイルの飾りの色
#define EFFECT_ACID_ATTACK_TRAIL_DECORATION_FADE_OUT_RATE 5.0f                       // 酸攻撃のトレイルの飾りのサイズ縮小量
#define EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED 750                            // 酸攻撃のトレイルの飾りの最大速度

// 血のトレイル
#define EFFECT_BLOOD_TRAIL_COLOR D3DXCOLOR(0.0f, 0.627f, 0.505f, 1.0f)           // 血のトレイルの色
#define EFFECT_BLOOD_TRAIL_FADE_OUT_RATE 2.5f                                    // 血のトレイルのサイズ縮小量

// 血のトレイルの飾り
#define EFFECT_BLOOD_TRAIL_DECORATION_COLOR D3DXCOLOR(0.0f,1.0f,0.0f,1.0f)       // 血のトレイルの飾りの色
#define EFFECT_BLOOD_TRAIL_DECORATION_FADE_OUT_RATE 2.5f                         // 血のトレイルの飾りのサイズ縮小量
#define EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED 500                              // 血のトレイルの飾りの最大速度

// 血煙エフェクト(未使用)
#define EFFECT_SPRAY_FIRST_SIZE D3DXVECTOR3(300.0f, 300.0f, 0.0f)                // 血煙の最初の大きさ
#define EFFECT_SPRAY_FIRST_COLOR D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)               // 血煙の最初の色
#define EFFECT_SPRAY_TO_YELLOW_RATE_R 0.01f                                      // 血煙の黄色に近づけていく値R
#define EFFECT_SPRAY_TO_YELLOW_RATE_B 0.005f                                     // 血煙の黄色に近づけていく値B
#define EFFECT_SPRAY_EXTEND_SIZE 5.0f                                            // 血煙の拡大割合
#define EFFECT_SPRAY_FADE_OUT_RATE 0.01f                                         // 血煙の消えていく割合
#define EFFECT_SPRAY_ROT_MAX 1500                                                // 血煙が回転する最高速度
#define EFFECT_SPRAY_ROT_MIN 500                                                 // 血煙が回転する最低速度
#define EFFECT_SPRAY_CONTROL 0.95f                                               // 血煙の動きを制御
#define EFFECT_SPRAY_MAX_SPEED 7000                                              // 血煙の最大速度
#define EFFECT_SPRAY_MIN_SPEED 3000                                              // 血煙の最小速度

// 生物へのヒットエフェクト
#define EFFECT_HIT_LIVING_MAX_SIZE 22500                                         // 最大サイズ
#define EFFECT_HIT_LIVING_MIN_SIZE 12500                                         // 最小サイズ
#define EFFECT_HIT_LIVING_NUMBER_ONCE 15                                         // 一度に生成する数
#define EFFECT_HIT_LIVING_MAX_SPEED 1250                                         // 最大速度
#define EFFECT_HIT_LIVING_MIN_SPEED 500                                          // 最小速度
#define EFFECT_HIT_LIVING_COLOR D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)                // 色
#define EFFECT_HIT_LIVING_FADE_OUT_RATE 7.5f                                     // フェードアウト割合

// 非生物へのヒットエフェクト
#define EFFECT_HIT_NON_LIVING_MAX_SIZE 15000                                     // 最大サイズ
#define EFFECT_HIT_NON_LIVING_MIN_SIZE 7500                                      // 最小サイズ
#define EFFECT_HIT_NON_LIVING_NUMBER_ONCE 15                                     // 一度に生成する数
#define EFFECT_HIT_NON_LIVING_MAX_SPEED 1250                                     // 最大速度
#define EFFECT_HIT_NON_LIVING_MIN_SPEED 500                                      // 最小速度
#define EFFECT_HIT_NON_LIVING_COLOR D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)            // 色
#define EFFECT_HIT_NON_LIVING_FADE_OUT_RATE 5.0f                                 // フェードアウト割合

// 回復アイテムのエフェクト
#define EFFECT_HEAL_ITEM_MAX_SPEED 800                                           // 最大速度
#define EFFECT_HEAL_ITEM_MIN_SPEED 300                                           // 最小速度
#define EFFECT_HEAL_ITEM_MAX_SIZE 12000                                          // 最大サイズ
#define EFFECT_HEAL_ITEM_MIN_SIZE 8000                                           // 最小サイズ
#define EFFECT_HEAL_ITEM_MAX_RADIUS 22500                                        // 最大半径
#define EFFECT_HEAL_ITEM_FADE_OUT_RATE 1.0f                                      // フェードアウト割合
#define EFFECT_HEAL_ITEM_COLOR D3DXCOLOR(0.745f, 1.0f, 0.682f, 1.0f)             // 色

// 回復エフェクト
#define EFFECT_HEALING_NUMBER_ONCE 35                                            // 一度に生成する数
#define EFFECT_HEALING_MAX_SPEED 1700                                            // 最大速度
#define EFFECT_HEALING_MIN_SPEED 1200                                            // 最小速度
#define EFFECT_HEALING_MAX_SPEED_Y 1000                                          // 最大速度Y
#define EFFECT_HEALING_MIN_SPEED_Y 700                                           // 最小速度Y
#define EFFECT_HEALING_MAX_SIZE 20000                                            // 最大サイズ
#define EFFECT_HEALING_MIN_SIZE 15000                                            // 最小サイズ
#define EFFECT_HEALING_FADE_OUT_RATE 2.0f                                        // フェードアウト割合
#define EFFECT_HEALING_COLOR D3DXCOLOR(0.407f, 0.956f, 0.878f, 1.0f)             // 色

// リロードエフェクト
#define EFFECT_RELOADING_COLOR D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f)                 // 色
#define EFFECT_RELOADING_CENTER_COLOR D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.7f)          // 中心色
#define EFFECT_RELOADING_CENTER_REDUCE_SIZE_RATE 0.8f                            // 中心サイズ縮小割合
#define EFFECT_RELOADING_FADE_OUT_RATE 4.0f                                      // フェードアウト割合
#define EFFECT_RELOADING_MAX_SIZE 11000                                          // 最大サイズ
#define EFFECT_RELOADING_MIN_SIZE 8500                                           // 最小サイズ
#define EFFECT_RELOADING_RADIUS 300.0f                                           // 半径
#define EFFECT_RELOADING_SPEED 27.5f                                             // 速さ

// 地割れエフェクト
#define EFFECT_CRACK_SIZE D3DXVECTOR3(350.0f, 350.0f, 0.0f)                      // 大きさ
#define EFFECT_CRACK_FADE_OUT_RATE 0.01f                                         // フェードアウト割合

// 左手の爆弾エフェクト
#define EFFECT_BOMB_CENTER_COLOR D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)               // 中心色
#define EFFECT_BOMB_AROUND_COLOR D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f)               // 周辺色

// 爆発エフェクト
#define EFFECT_EX_ALPHA_TEST_BORDER 100                                          // α値のボーダー値
#define EFFECT_EX_DOWN_ALPHA_RATE 0.001f                                         // α値を下げていく割合
#define EFFECT_EX_TO_RED_RATE 0.004f                                             // 赤色に近づけていく値
#define EFFECT_EX_CHANGE_MOVE_START_COUNT 180                                    // 移動量変化の開始時間
#define EFFECT_EX_CHANGE_MOVE_RATE -1.35f                                        // 移動量変化の割合
#define EFFECT_EX_ROT_MAX 1000                                                   // 回転する最高速度
#define EFFECT_EX_FADE_OUT_RATE_LV1 2.0f                                         // フェードアウト割合lv1
#define EFFECT_EX_FADE_OUT_RATE_LV2 3.0f                                         // フェードアウト割合lv2
#define EFFECT_EX_FADE_OUT_RATE_LV3 4.5f                                         // フェードアウト割合lv3
#define EFFECT_EX_FADE_OUT_RATE_LV4 6.75f                                        // フェードアウト割合lv4
#define EFFECT_EX_MAX_SPEED_LV1 700                                              // 最大速度lv1
#define EFFECT_EX_MAX_SPEED_LV2 950                                              // 最大速度lv2
#define EFFECT_EX_MAX_SPEED_LV3 1200                                             // 最大速度lv3
#define EFFECT_EX_MAX_SPEED_LV4 1450                                             // 最大速度lv4
#define EFFECT_EX_MAX_SPEED_Y 1000                                               // 最大速度Y
#define EFFECT_EX_NUMBER_ONCE 2                                                  // 1Fごとに生成する数(0で赤、1で黄色)
#define EFFECT_EX_RED_COLOR D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f)                    // 赤
#define EFFECT_EX_YELLOW_COLOR D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)                 // 黄

// 青い炎
#define EFFECT_BLUE_FIRE_FIRST_COLOR D3DXCOLOR(0.588f,0.937f,1.0f,1.0f)          // 青い炎の色
#define EFFECT_BLUE_FIRE_BLUE D3DXCOLOR(0.0f,0.0f,1.0f,1.0f)                     // 青
#define EFFECT_WHITE_FIRE_TO_BLUE_RATE 0.004f                                    // 青に近づけていく割合
#define EFFECT_BLUE_FIRE_FADE_OUT_RATE 0.008f                                    // 消えていく割合
#define EFFECT_BLUE_FIRE_EXTEND_RATE 12.5f                                       // 拡大割合

// クリスタルに当たった際のエフェクト
#define EFFECT_HIT_CRYSTAL_COLOR D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.8f)               // ピンク
#define EFFECT_HIT_CRYSTAL_NUMBER_ONCE 15                                        // 一度に生成する数
#define EFFECT_HIT_CRYSTAL_SIZE D3DXVECTOR3(400.0f, 400.0f, 0.0f)                // 大きさ
#define EFFECT_HIT_CRYSTAL_FADE_OUT_RATE 10.0f                                   // フェードアウト割合
#define EFFECT_HIT_CRYSTAL_SPEED 20.0f                                           // 速さ

// ミニクリスタルに当たった際のエフェクトの大きさとフェードアウト割合と速さ
#define EFFECT_HIT_MINI_CRYSTAL_SIZE D3DXVECTOR3(200.0f, 200.0f, 0.0f)           // 大きさ
#define EFFECT_HIT_MINI_CRYSTAL_FADE_OUT_RATE 5.0f                               // フェードアウト割合
#define EFFECT_HIT_MINI_CRYSTAL_SPEED 10.0f                                      // 速さ

// ワイバーンの頭に当たった際のエフェクトの大きさとフェードアウト割合と速さ
#define EFFECT_HIT_WYVERN_HEAD_SIZE D3DXVECTOR3(300.0f, 300.0f, 0.0f)           // 大きさ
#define EFFECT_HIT_WYVERN_HEAD_FADE_OUT_RATE 7.5f                               // フェードアウト割合
#define EFFECT_HIT_WYVERN_HEAD_SPEED 15.0f                                      // 速さ

// 爆砕の中心エフェクト
#define EFFECT_BURST_CENTER_FIRST_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)          // 最初の大きさ
#define EFFECT_BURST_CENTER_FIRST_COLOR D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)       // 最初の色
#define EFFECT_BURST_CENTER_TO_YELLOW_RATE_R 0.02f                              // 黄色に近づけていく値R
#define EFFECT_BURST_CENTER_TO_RED_RATE_G 0.02f                                 // 赤色に近づけていく値G
#define EFFECT_BURST_CENTER_EXTEND_SIZE 6.5f                                    // 拡大割合
#define EFFECT_BURST_CENTER_FADE_OUT_RATE 0.006f                                // フェードアウト割合
#define EFFECT_BURST_CENTER_MAX_RADIUS 500                                      // 最大半径
#define EFFECT_BURST_CENTER_EXPLOSION_FRAME 120                                 // 爆発を生成するフレーム数

// 爆砕の周辺エフェクト
#define EFFECT_BURST_AROUND_FIRST_SIZE_RATE 0.6f                                // 最初の大きさの割合
#define EFFECT_BURST_AROUND_EXTEND_SIZE 5.5f                                    // 拡大割合
#define EFFECT_BURST_AROUND_FADE_OUT_RATE 0.0085f                               // フェードアウト割合

//*****************************************************************************
// エフェクト3Dクラス定義
//*****************************************************************************
class CEffect3D :public CBillboard
{
public:

    // エフェクト3Dの種類
    typedef enum
    {
        TYPE_PURPLE_FIRE = 0,	// 紫の炎
        TYPE_PARTICLE,          // パーティクル
        TYPE_WAVE,              // ウェーブ
        TYPE_GUN_MARKS,         // 銃痕
        TYPE_CHARGE,            // チャージ
        TYPE_SMOKE,             // 煙
        TYPE_ACID,              // 酸
        TYPE_SPRAY,             // 血煙
        TYPE_ACID_ATTACK,       // 酸攻撃
        TYPE_WAVE_ATTACK,       // 衝撃波攻撃
        TYPE_RELOADING,         // リロード中
        TYPE_CRACK1,            // 地割れ1
        TYPE_CRACK2,            // 地割れ2
        TYPE_CRACK3,            // 地割れ3
        TYPE_CRACK4,            // 地割れ4
        TYPE_EXPLOSION,         // 爆発
        TYPE_BLUE_FIRE,         // 青い炎
        TYPE_BURST_CENTER,      // 爆砕の中心
        TYPE_BURST_AROUND,      // 爆砕の周辺
        TYPE_MAX			    // 種類の最大数
    }TYPE;

    CEffect3D(CScene::OBJTYPE objtype);
    ~CEffect3D();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Update(void);
    void Draw(void);
    void Uninit(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CEffect3D * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 rot, float fFadeOutRate, TYPE type);

    float GetRandRotSpeed(int nMax);
    void GetAngleToLeftHand(D3DXVECTOR3 pos);
    void GetMoveToRightHand(D3DXVECTOR3 pos);
    void SetBurstAround(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];    // 読み込むテクスチャ
    D3DXVECTOR3 m_move;                                 // 移動量
    float m_fSpeed;                                     // 速さ
    D3DXCOLOR m_col;                                    // 色
    TYPE m_type;                                        // 種類
    float m_fFadeOutRate;                               // フェードアウト割合

    bool m_bStartFadeOut;                               // フェードアウト開始
    int m_nCntTime;                                     // カウンタ

    float m_fRotAngle;                                  // 回転角度
};
#endif
