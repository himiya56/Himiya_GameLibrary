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
class CGame;
class CCamera;
class CPlayer;
class CLight;

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
		MODE_GAME,
		MODE_EQUPMENT,
		MODE_RESULT,
	}MODE;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);  // ����������
	void Uninit(void);											 // �I������
	void Update(void);											 // �X�V����
	void Draw(void);											 // �`�揈��
	void Load(void);											 // ���f���E�e�N�X�`���ǂݍ���
	void Unload(void);											 // ���f���E�e�N�X�`���j��
	static void SetMode(const MODE mode);
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetInput(void) { return m_pInput; }
	static CPlayer *GetPlayer(void) { return m_pPlayer; }
	static MODE GetCurrentMode(void) { return m_CurrentMode; }
	static MODE GetNextStage(void) { return m_NextStage; }
private:
	// �����o�ϐ�
	static CRenderer		*m_pRenderer;
	static CInputKeyboard   *m_pInput;
	static CGame			*m_pGame;
	static CCamera			*m_pCamera;
	static MODE				m_CurrentMode;
	static MODE				m_NextStage;
	static CPlayer			*m_pPlayer;					  // �v���C���[�̃|�C���^
	static CLight			*m_pLight;                    // ���C�g�̃|�C���^
};

#endif