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
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "input.h"
#include "Keyboard.h"
#include "sound.h"
#include "bg.h"
#include "enemy.h"
#include "number.h"
#include "Score.h"
#include "life.h"
#include "block.h"
#include "map.h"
#include "magazine.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "game.h"
#include "item.h"
#include "camera.h"
#include "select.h"
#include "equipment.h"
#include "option.h"
#include "boss.h"
#include "pause.h"
#include "logo.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CRenderer * CManager::m_pRenderer = NULL;
CInputKeyboard * CManager::m_pInput = NULL;
CSound * CManager::m_pSound = NULL;
CTitle * CManager::m_pTitle = NULL;
CResult * CManager::m_pResult = NULL;
CGame * CManager::m_pGame = NULL;
CFade * CManager::m_pFade = NULL;
CCamera *CManager::m_pCamera = NULL;
CManager::MODE CManager::m_CurrentMode = CManager::MODE_TITLE;
CManager::MODE CManager::m_NextStage = CManager::MODE_GAME_STAGE_01;
CSelect * CManager::m_pSelect = NULL;

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

	m_pSound = new CSound;
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return -1;
	}

	// �J�����̐���
	m_pCamera = new CCamera;
	if (FAILED(m_pCamera->Init()))
	{
		return E_FAIL;
	}

	m_pFade = CFade::Create();

	// �e�N�X�`���̓ǂݍ���
	CPlayer::Load();
	CBullet::Load();
	CExplosion::Load();
	CBg::Load();
	CEnemy::Load();
	CNumber::Load();
	CLife::Load();
	CBlock::Load();
	CMagazine::Load();
	CTitle::Load();
	CResult::Load();
	CItem::Load();
	CSelect::Load();
	COption::Load();
	CBoss::Load();
	CPause::Load();
	CLogo::Load();

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

	if (m_pGame != NULL)
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = NULL;
	}
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
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
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// �e�N�X�`���̔j��
	CPlayer::Unload();
	CBullet::Unload();
	CExplosion::Unload();
	CBg::Unload();
	CNumber::Unload();
	CLife::Unload();
	CBlock::Unload();
	CMagazine::Unload();
	CTitle::Unload();
	CResult::Unload();
	CItem::Unload();
	CSelect::Unload();
	COption::Unload();
	CBoss::Unload();
	CPause::Unload();
	CLogo::Unload();
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
	if (m_pFade != NULL)
	{
		m_pFade->Update();
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
// ���[�h�ύX����
//=============================================================================
void CManager::SetMode(const MODE mode)
{
	int nScore = CScore::GetScore();

	CObject::ReleaseAll();

	m_CurrentMode = mode;
	CMagazine::SetMagFull();

	switch (mode)
	{
	case MODE_TITLE:
		m_pSound->StopSound();
		m_pGame = CGame::Create(CGame::GAME_STAGE_TITLE, 0);
		CEquipment::CheckEquipment();
		m_NextStage = MODE_GAME_STAGE_01;
		m_pSound->Play(CSound::SOUND_LABEL_BGM_TITLE);
		break;
	case MODE_GAME_STAGE_01:
		m_pSound->StopSound();
		m_pGame = CGame::Create(CGame::GAME_STAGE_01, nScore);
		CEquipment::CheckEquipment();
		m_NextStage = MODE_GAME_STAGE_02;
		m_pSound->Play(CSound::SOUND_LABEL_BGM_01);
		break;
	case MODE_GAME_STAGE_02:
		m_pGame = CGame::Create(CGame::GAME_STAGE_02, nScore);
		CEquipment::CheckEquipment();
		m_NextStage = MODE_GAME_STAGE_03;
		break;
	case MODE_GAME_STAGE_03:
		m_pGame = CGame::Create(CGame::GAME_STAGE_03, nScore);
		CEquipment::CheckEquipment();
		m_NextStage = MODE_RESULT;
		break;
	case MODE_EQUPMENT:
		m_pSelect = CSelect::Create();
		break;
	case MODE_RESULT:
		m_pSound->StopSound();
		CEquipment::ClearEquipment();
		m_pResult = CResult::Create(nScore);
		m_pSound->Play(CSound::SOUND_LABEL_BGM_RESULT);
		break;
	default:
		break;
	}
}
