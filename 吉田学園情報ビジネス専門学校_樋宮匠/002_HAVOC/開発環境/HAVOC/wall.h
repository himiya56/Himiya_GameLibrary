//====================================================================
//
// 壁の処理 (wall.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _WALL_H_
#define _WALL_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "billboard.h"

//================================================
// マクロ定義
//================================================

//================================================
// クラス宣言
//================================================

// 壁クラス
class CWall :public CBillboard
{
public:
    CWall();
    ~CWall();

    // 南北東西の壁
    typedef enum
    {
        TYPE_WALL_NORTH = 0,
        TYPE_WALL_SOUTH,
        TYPE_WALL_EAST,
        TYPE_WALL_WEST,
        TYPE_MAX
    }TYPE;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CWall *Create(TYPE type, float fMapWidth);
    static void SetWall(float fMapWidth);

private:
    static LPDIRECT3DTEXTURE9		m_pTexture;				// 読み込むテクスチャ
    TYPE							m_type;					// 壁のタイプ
    float							m_fMapWidth;			// マップサイズ格納用
    float							m_fTex;					// テクスチャUV座標変更用
    int								m_nAlpha;				// アルファ値加算用
    bool							m_bAddAlpha;			// アルファ値を加算するかどうか
};

#endif
