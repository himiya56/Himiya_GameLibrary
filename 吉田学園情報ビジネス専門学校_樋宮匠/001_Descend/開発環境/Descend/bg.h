//=============================================================================
//
// 背景処理 [bg.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _BG_H_
#define _BG_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"
#include "map.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_ANIM_SPEED   30
#define MAX_BG_POLYGON  3   // ポリゴンの数

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CBg :public CPolygon3D
{
public:
	CBg();
	~CBg();

	// 列挙型宣言
	typedef enum
	{
		TEX_BG_0 = 0,
		TEX_BG_1,
		TEX_BG_2,
		TEX_BG_MAX
	}TEX_BG;

	HRESULT Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CBg * Create(void);
private:
	static LPDIRECT3DTEXTURE9		m_apTexture[TEX_BG_MAX];	 // テクスチャへのポインタ
	float							m_fTexUVY;					     // テクスチャUV移動用変数
};
#endif