//=============================================================================
//
// マネージャー [manager.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CSound;
class CTitle;
class CGame;
class CResult;
class CFade;
class CCamera;
class CSelect;

//*****************************************************************************
// マネージャークラス定義
//*****************************************************************************
class CManager
{
public:
	// メンバ関数
	CManager();													 // コンストラクタ
	~CManager();												 // デストラクタ

	typedef enum
	{
		MODE_TITLE = 0,
		MODE_GAME_STAGE_01,
		MODE_GAME_STAGE_02,
		MODE_GAME_STAGE_03,
		MODE_GAME_STAGE_BOSS,
		MODE_EQUPMENT,
		MODE_RESULT,
	}MODE;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);  // 初期化処理
	void Uninit(void);											 // 終了処理
	void Update(void);											 // 更新処理
	void Draw(void);											 // 描画処理
	static void SetMode(const MODE mode);
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetInput(void) { return m_pInput; }
	static CSound *GetSound(void) { return m_pSound; }
	static CFade *GetFade(void) { return m_pFade; }
	static MODE GetCurrentMode(void) { return m_CurrentMode; }
	static MODE GetNextStage(void) { return m_NextStage; }
private:
	// メンバ変数
	static CRenderer		*m_pRenderer;
	static CInputKeyboard   *m_pInput;
	static CSound			*m_pSound;
	static CTitle			*m_pTitle;
	static CGame			*m_pGame;
	static CResult			*m_pResult;
	static CFade			*m_pFade;
	static CCamera			*m_pCamera;
	static MODE				m_CurrentMode;
	static MODE				m_NextStage;
	static CSelect			*m_pSelect;
};

#endif