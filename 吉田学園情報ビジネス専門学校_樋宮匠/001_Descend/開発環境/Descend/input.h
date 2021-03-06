//=============================================================================
//
// 入力処理 [input.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX 256 // キーの最大数

//*****************************************************************************
// インプットクラス定義
//*****************************************************************************
class CInput
{
public:
	// メンバ関数
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;
protected:
	// メンバ変数
	static LPDIRECTINPUT8	m_pDInput;
	LPDIRECTINPUTDEVICE8	m_pDevice = NULL;
private:
};

#endif