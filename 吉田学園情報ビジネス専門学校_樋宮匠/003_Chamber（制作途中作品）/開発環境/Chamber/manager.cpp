//=============================================================================
//
// �I�u�W�F�N�g�Ǘ����� [manager.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "input.h"
#include "Keyboard.h"
#include "camera.h"
#include "player.h"
#include "field.h"
#include "camera_player.h"
#include "portal.h"
#include "Button.h"
#include "Dropper.h"
#include "Cube.h"
#include "light.h"
#include "tile.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CRenderer * CManager::m_pRenderer = NULL;
CInputKeyboard * CManager::m_pInput = NULL;
CCamera * CManager::m_pCamera = NULL;
CPlayer * CManager::m_pPlayer = NULL;
CLight *CManager::m_pLight = NULL;
CManager::MODE CManager::m_CurrentMode = CManager::MODE_GAME;
CManager::MODE CManager::m_NextStage = CManager::MODE_GAME;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//�C���X�^���X�����E����������
	m_pRenderer = new CRenderer;
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return -1;
	}

	m_pInput = new CInputKeyboard;
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return -1;
	}

	// ���C�g�̐���
	m_pLight = new CLight;
	if (FAILED(m_pLight->Init()))
	{
		return E_FAIL;
	}

	m_pCamera = CCameraPlayer::Create();

	// �e�N�X�`���̓ǂݍ���
	Load();

	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 250.0f, 0.0f), DEFAULT_VECTOR);
	CField::Create(CField::FLOOR, 10, 10);
	CField::Create(CField::CEILING, 10, 10);
	//CButton::Create();
	CCube::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	//CDropper::Create();

	SetMode(MODE_TITLE);

	return 0;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �I������
	CObject::ReleaseAll();

	if (m_pInput != NULL)
	{
		m_pInput->Uninit();
		//�������j��
		delete m_pInput;
		m_pInput = NULL;
	}
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		//�������j��
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// �e�N�X�`���̔j��
	Unload();
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	// �X�V����
	if (m_pInput != NULL)
	{
		m_pInput->Update();
	}
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	// �`�揈��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// ���f���E�e�N�X�`���ǂݍ��ݏ���
//=============================================================================
void CManager::Load(void)
{
	CPlayer::Load();
	CPortal::Load();
	CTile::Load();
	CButton::Load();
	CCube::Load();
	CDropper::Load();
	CPlayer::Load();
}

//=============================================================================
// ���f���E�e�N�X�`���j������
//=============================================================================
void CManager::Unload(void)
{
	CPlayer::Unload();
	CPortal::Unload();
	CTile::Unload();
	CButton::Unload();
	CCube::Unload();
	CDropper::Unload();
	CPlayer::Unload();
}

//=============================================================================
// ���[�h�ύX����
//=============================================================================
void CManager::SetMode(const MODE mode)
{
}
