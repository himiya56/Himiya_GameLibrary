//=============================================================================
//
// �L�[�{�[�h���� [keyboard.h]
// Author : ��{ ��
//
//=============================================================================

// ��d�C���N���[�h�h�~�p
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_KEY_MAX 256 // �L�[�̍ő吔

//*****************************************************************************
// �L�[�{�[�h�N���X��`
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:
	// �����o�֐�
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update(void);
	bool GetKeyboardPress(int nKey);	// �L�[�������Ă��邩
	bool GetKeyboardTrigger(int nKey);	// �L�[����������
	bool GetKeyboardRelease(int nKey);	// �L�[�𗣂�����
private:
	// �����o�ϐ�
	BYTE m_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h��Ԋi�[�ϐ�
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�g���K�[�i�[�ϐ�
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�����[�X�i�[�ϐ�
};

#endif