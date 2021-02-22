//====================================================================
//
// エフェクトの処理 (effect.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================

// プレイヤー
#define EFFECT_2D_PLAYER_FIRST_POS D3DXVECTOR3(-200.0f, SCREEN_HEIGHT / 2, 0.0f)
#define EFFECT_2D_PLAYER_SIZE D3DXVECTOR3(450.0f, 450.0f, 0.0f)
#define EFFECT_2D_PLAYER_FIRST_MOVE_VALUE D3DXVECTOR3(50.0f, 0.0f, 0.0f)
#define EFFECT_2D_PLAYER_MOVE_DOWN_RATE 0.96f
#define EFFECT_2D_PLAYER_STOP_MOVE_VALUE 0.1f

// プレイヤーの影
#define EFFECT_2D_PLAYER_SHADOW_FADE_RATE 0.1f

//================================================
// クラス宣言
//================================================

// エフェクトクラス
class CEffect : public CScene2D
{
public:
    // エフェクトの種類
    typedef enum
    {
        TYPE_PLAYER = 0,	// プレイヤー
        TYPE_PLAYER_SHADOW,	// プレイヤーの影
        TYPE_MAX			// 種類の最大数
    }TYPE;
    CEffect();
    ~CEffect();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, float fAngle, float fRotSpeed, D3DXCOLOR col, TYPE type);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    D3DXVECTOR3 m_move;                                // 移動量
    D3DXCOLOR m_col;                                   // 色
    int m_nLife;                                       // ライフ
    TYPE m_type;                                       // エフェクトの種類

    bool m_bAlphaDown;                                 // 透明度を下げるフラグ

    float m_fRotSpeed;                                 // 回転速度
    float m_fAngle;                                    // 角度
};

#endif