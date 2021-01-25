//=============================================================================
//
// �}�l�[�W���[ [manager.h]
// Author : ��{ ��
//
//=============================================================================

// ��d�C���N���[�h�h�~�p
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �O���錾
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
// �}�l�[�W���[�N���X��`
//*****************************************************************************
class CManager
{
public:
	// �����o�֐�
	CManager();													 // �R���X�g���N�^
	~CManager();												 // �f�X�g���N�^

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

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);  // ����������
	void Uninit(void);											 // �I������
	void Update(void);											 // �X�V����
	void Draw(void);											 // �`�揈��
	static void SetMode(const MODE mode);
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetInput(void) { return m_pInput; }
	static CSound *GetSound(void) { return m_pSound; }
	static CFade *GetFade(void) { return m_pFade; }
	static MODE GetCurrentMode(void) { return m_CurrentMode; }
	static MODE GetNextStage(void) { return m_NextStage; }
private:
	// �����o�ϐ�
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