//====================================================================
//
// 海の処理 (ocean.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _OCEAN_H_
#define _OCEAN_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "billboard.h"

//================================================
// マクロ定義
//================================================
#define OCEAN_SIZE		 30000.0f									// 海のサイズ

#define SURFACE_0_POS    D3DXVECTOR3(6500.0f, -350.0f, 6500.0f)		// 下層の座標
#define SURFACE_1_POS    D3DXVECTOR3(6500.0f, -250.0f, 6500.0f)		// 中層の座標
#define SURFACE_2_POS    D3DXVECTOR3(6500.0f, -150.0f, 6500.0f)		// 上層の座標

#define MIDDLE_ALPHA_RATE	1										// 中層のアルファ値加算レート
#define TOP_ALPHA_RATE		2										// 上層のアルファ値加算レート

#define MIDDLE_MOVE_TEX		0.0001f									// 中層のテクスチャ移動量
#define TOP_MOVE_TEX		-0.0001f								// 上層のテクスチャ移動量

#define ALPHA_MAX			250										// アルファ値上限
#define ALPHA_MIN			80										// アルファ値下限

//================================================
// クラス宣言
//================================================

// ポーズクラス
class COcean :public CBillboard
{
public:
    COcean();
    ~COcean();

    // 数字が大きいほどY座標が上
    typedef enum
    {
        TYPE_SURFACE_0 = 0,     // 下層
        TYPE_SURFACE_1,         // 中層
        TYPE_SURFACE_2,         // 上層
        TYPE_MAX
    }TYPE;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    void CalcAlpha(int nAlphaRate);
    static HRESULT Load(void);
    static void Unload(void);
    static COcean *Create(TYPE type);
private:
    static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // 読み込むテクスチャ
    TYPE							m_type;					// 海のタイプ
    float							m_fTexX;				// テクスチャX座標
    float							m_fTexY;				// テクスチャY座標
    int								m_nAlpha;				// アルファ値
    bool							m_bAddAlpha;			// アルファ値を加算するかどうか
};

#endif