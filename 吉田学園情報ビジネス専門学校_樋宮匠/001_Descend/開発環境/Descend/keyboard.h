//=============================================================================
//
// キーボード処理 [keyboard.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX 256 // キーの最大数

//*****************************************************************************
// キーボードクラス定義
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:
	// メンバ関数
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update(void);
	bool GetKeyboardPress(int nKey);	// キーを押しているか
	bool GetKeyboardTrigger(int nKey);	// キーを押したか
	bool GetKeyboardRelease(int nKey);	// キーを離したか
private:
	// メンバ変数
	BYTE m_aKeyState[NUM_KEY_MAX];			// キーボード状態格納変数
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードトリガー格納変数
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// キーボードリリース格納変数
};

#endif