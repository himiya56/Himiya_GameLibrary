//===============================================
//
// �����̊Ǘ����� (manager.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "manager.h"
#include "scene2d.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "mode.h"
#include "title.h"
#include "game.h"
#include "ui.h"
#include <typeinfo.h>
#include "fade.h"
#include "light.h"
#include "camera.h"
#include "character.h"
#include "block.h"
#include "effect3d.h"
#include "tile.h"
#include "effect.h"
#include "number.h"
#include "result.h"
#include "manual.h"
#include "debug.h"
#include "bullet.h"
#include "player.h"
#include "modelEffect.h"
#include "item.h"
#include "sky.h"
#include "spider.h"
#include "ant.h"
#include "wall.h"
#include "force.h"
#include "ocean.h"
#include "whiteFade.h"
#include "ufo.h"
#include "tower.h"
#include "wyvern.h"
#include "pause.h"

//========================================
// �}�N����`
//========================================

//========================================
// �ÓI�����o�ϐ��錾
//========================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CMouse *CManager::m_pMouse = NULL;
CSound *CManager::m_pSound = NULL;
CMode *CManager::m_pMode = NULL;
CFade *CManager::m_pFade = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;

int CManager::m_nScore = 0;

//========================================
// �����̊Ǘ��̃f�t�H���g�R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CManager::CManager()
{

}

//========================================
// �����̊Ǘ��̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CManager::~CManager()
{

}

//========================================
// �����̊Ǘ��̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
    // �����_���[�̐���(��񂵂��������Ȃ����̂́ACreate��p�ӂ��Ă��Ȃ�)
    m_pRenderer = new CRenderer;

    // �����_���[����������
    if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
    {
        return E_FAIL;
    }

    // �L�[�{�[�h�̐���(new��Init�͘A�������ď���)
    m_pInputKeyboard = new CInputKeyboard;

    // �L�[�{�[�h�̏���������
    if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
    {
        return E_FAIL;
    }

    // �R���g���[���̐���
    m_pInputJoypad = new CInputJoypad;

    // �R���g���[���̏���������
    if (FAILED(m_pInputJoypad->Init(hInstance, hWnd)))
    {
        return E_FAIL;
    }

    // �}�E�X�̐���
    m_pMouse = new CMouse;

    // �}�E�X�̏���������
    if (FAILED(m_pMouse->Init(hInstance, hWnd))) 
    {
        return E_FAIL;
    }

    // �T�E���h�̐���
    m_pSound = new CSound;
    if (FAILED(m_pSound->Init(hWnd)))
    {
        return E_FAIL;
    }

    // ���C�g�̐���
    m_pLight = new CLight;
    if (FAILED(m_pLight->Init()))
    {
        return E_FAIL;
    }

    // �J�����̐���
    m_pCamera = new CCamera;
    if (FAILED(m_pCamera->Init()))
    {
        return E_FAIL;
    }

    // �t�F�[�h�̐���
    m_pFade = CFade::Create(MODE_TITLE);    // �ŏ��̓^�C�g��

    // ���f���A�e�N�X�`���̃��[�h
    CUI::Load();
    CNumber::Load();
    CPlayer::Load();
    CBlock::Load();
    CEffect::Load();
    CEffect3D::Load();
    CTile::Load();
    CBullet::Load();
    CDebug::Load();
    CModelEffect::Load();
    CItem::Load();
    CSky::Load();
    COcean::Load();
    CWall::Load();
    CWhiteFade::Load();
    CSpider::Load();
    CAnt::Load();
    CForce::Load();
    CUfo::Load();
    CTower::Load();
    CWyvern::Load();
    CPause::Load();

    return S_OK;
}

//========================================
// �����̊Ǘ��̏I������
// Author : �㓡�T�V��
//========================================
void CManager::Uninit(void)
{
    // �S�ẴI�u�W�F�N�g��j��
    CScene::ReleaseAll();

    // ���f���A�e�N�X�`���̃A�����[�h
    CUI::Unload();
    CNumber::Unload();
    CPlayer::Unload();
    CBlock::Unload();
    CEffect::Unload();
    CEffect3D::Unload();
    CTile::Unload();
    CBullet::Unload();
    CDebug::Unload();
    CModelEffect::Unload();
    CItem::Unload();
    CSky::Unload();
    COcean::Unload();
    CWall::Unload();
    CWhiteFade::Unload();
    CSpider::Unload();
    CAnt::Unload();
    CForce::Unload();
    CUfo::Unload();
    CTower::Unload();
    CWyvern::Unload();
    CPause::Unload();

    // ���[�h�j��
    if (m_pMode != NULL)
    {
        // ���[�h�̏I������
        m_pMode->Uninit();

        // ���[�h�̃������̊J��
        delete m_pMode;
        m_pMode = NULL;
    }

    // �t�F�[�h�j��
    if (m_pFade != NULL)
    {
        // �t�F�[�h�̏I������
        m_pFade->Uninit();

        // �t�F�[�h�̃������̊J��
        delete m_pFade;
        m_pFade = NULL;
    }

    // �T�E���h�j��
    if (m_pSound != NULL)
    {
        // �T�E���h�I������
        m_pSound->Uninit();

        // �T�E���h�̃������̊J��
        delete m_pSound;
        m_pSound = NULL;
    }

    // �}�E�X�̔j��
    if (m_pMouse != NULL)
    {
        // �}�E�X�I������
        m_pMouse->Uninit();

        // �}�E�X�̃������̊J��
        delete m_pMouse;
        m_pMouse = NULL;
    }

    // �R���g���[���j��
    if (m_pInputJoypad != NULL)
    {
        // �R���g���[���I������
        m_pInputJoypad->Uninit();

        // �R���g���[���̃������̊J��
        delete m_pInputJoypad;
        m_pInputJoypad = NULL;
    }

    // �L�[�{�[�h�j���iInit�Ő��������t���Ŕj������j
    if (m_pInputKeyboard != NULL)
    {
        // �L�[�{�[�h�I������
        m_pInputKeyboard->Uninit();

        // �L�[�{�[�h�̃������̊J��
        delete m_pInputKeyboard;
        m_pInputKeyboard = NULL;
    }

    // �����_���[�j��
    if (m_pRenderer != NULL)
    {
        // �����_���[�I������
        m_pRenderer->Uninit();

        // �����_���[�̃������̊J��
        delete m_pRenderer;
        m_pRenderer = NULL;
    }
}

//========================================
// �����̊Ǘ��̍X�V����
// Author : �㓡�T�V��
//========================================
void CManager::Update(void)
{
    // �L�[�{�[�h�X�V����(�ŏ��ɍs��)
    if (m_pInputKeyboard != NULL)
    {
        m_pInputKeyboard->Update();
    }

    // �R���g���[���X�V����
    if (m_pInputJoypad != NULL)
    {
        m_pInputJoypad->Update();
    }

    // �}�E�X�X�V����
    if (m_pMouse != NULL) 
    {
        m_pMouse->Update();
    }

    // �����_���[�X�V����
    if (m_pRenderer != NULL)
    {
        // ���[�h������Ȃ�
        if (m_pMode != NULL)
        {
            // �Q�[�����Ȃ�
            if (typeid(*m_pMode) == typeid(CGame))
            {
                // �|�[�Y�łȂ��Ȃ�
                if (CGame::GetState() != CGame::STATE_PAUSE)
                {
                    // �S�čX�V
                    m_pRenderer->Update();
                }
            }
            else
            {
                // �Q�[���ȊO�̏�ʂȂ�A��ɑS�čX�V
                m_pRenderer->Update();
            }
        }
    }

    // ���[�h�̍X�V����
    if (m_pMode != NULL)
    {
        m_pMode->Update();
    }

    // �t�F�[�h�̍X�V����
    if (m_pFade != NULL)
    {
        m_pFade->Update();
    }

    // �J�����̍X�V����
    if (m_pCamera != NULL)
    {
        // �Q�[�����Ȃ�
        if (typeid(*m_pMode) == typeid(CGame))
        {
            m_pCamera->Update();
        }
    }
}

//========================================
// �����̊Ǘ��̕`�揈��
// Author : �㓡�T�V��
//========================================
void CManager::Draw(void)
{
    // �`�揈��
    if (m_pRenderer != NULL)
    {
        m_pRenderer->Draw();
    }
}

//========================================
// ���[�h�̎擾
// Author : �㓡�T�V��
//========================================
CManager::MODE CManager::GetMode(void)
{
    // �Ԃ�l�Ɏg�����[�h
    MODE mode = MODE_NONE;

    // ���̃��[�h�������Ă��邩���āA�^�����Ƃɏꍇ����
    if (typeid(*m_pMode) == typeid(CTitle))
    {
        mode = MODE_TITLE;
    }
    else if (typeid(*m_pMode) == typeid(CManual))
    {
        mode = MODE_MANUAL;
    }
    else if (typeid(*m_pMode) == typeid(CGame))
    {
        mode = MODE_GAME;
    }
    else if (typeid(*m_pMode) == typeid(CResult))
    {
        mode = MODE_RESULT;
    }

    return mode;
}

//========================================
// ���[�h�̐ݒ�
// mode	:	�ݒ肷�郂�[�h
// Author : �㓡�T�V��
//========================================
void CManager::SetMode(MODE mode)
{
    // ���[�h�̒��g������Ȃ�
    if (m_pMode != NULL)
    {
        // �I������
        m_pMode->Uninit();

        // �������̔j��
        delete m_pMode;
        m_pMode = NULL;
    }

    // �V�[����̃I�u�W�F�N�g��S�ďI������
    CScene::ReleaseAll();

    // �󂯎�������[�h�ɉ�ʑJ��
    switch (mode)
    {
    case MODE_TITLE:
        m_pMode = new CTitle;
        break;

    case MODE_MANUAL:
        m_pMode = new CManual;
        break;

    case MODE_GAME:
        m_pMode = new CGame;
        break;

    case MODE_RESULT:
        m_pMode = new CResult;
        break;
    }

    // �m�ۂ������[�h�̏�����
    m_pMode->Init();
}