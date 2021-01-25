//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// ポーズクラス定義
//*****************************************************************************
class CPause
{
public:
	// メンバ関数
	CPause();									// コンストラクタ
	~CPause();									// デストラクタ

	typedef enum
	{
		PAUSE_RETURN = 0,
		PAUSE_RETRY,
		PAUSE_TITLE,
		PAUSE_END,
		PAUSE_MAX
	}PAUSE;

	HRESULT Init(void);								// 初期化処理
	static void Uninit(void);								// 終了処理
	static void Update(void);								// 更新処理
	static void Draw(void);								// 描画処理
	static CPause *Create(void);					// インスタンス生成
	static HRESULT Load(void);
	static void Unload(void);
private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9				m_pTexture;		// テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9		    m_pVtxBuff;		// 頂点バッファのポインタ
	static int								m_nSelect;
};

#endif