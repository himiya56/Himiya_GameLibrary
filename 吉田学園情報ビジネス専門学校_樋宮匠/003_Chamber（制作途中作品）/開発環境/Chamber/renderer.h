//=============================================================================
//
// 描画処理 [renderer.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_POLYGON 1024

//*****************************************************************************
// レンダラークラス定義
//*****************************************************************************
class CRenderer
{
public:
	// メンバ関数
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	static void ResumeGame(void) { m_bPuase = false; }
private:
	// メンバ関数
	void DrawFPS(void);

	// メンバ変数
	LPDIRECT3D9				m_pD3D = NULL;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice = NULL;	        // Deviceオブジェクト(描画に必要)
	LPD3DXFONT				m_pFont = NULL;			// フォントへのポインタ
	static bool				m_bPuase;
};

#endif

