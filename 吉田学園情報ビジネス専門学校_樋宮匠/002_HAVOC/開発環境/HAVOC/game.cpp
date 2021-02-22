//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : �㓡�T�V��
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "game.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "animation.h"
#include "camera.h"
#include "fade.h"
#include "block.h"
#include "character.h"
#include "input.h"
#include "scene.h"
#include "library.h"
#include "effect3d.h"
#include "field.h"
#include "timer.h"
#include "camera.h"
#include "debug.h"
#include "bullet.h"
#include "score.h"
#include "player.h"
#include "spider.h"
#include "ant.h"
#include "item.h"
#include "bulletRemain.h"
#include "mission.h"
#include "sky.h"
#include "wall.h"
#include "ocean.h"
#include "force.h"
#include "whiteFade.h"
#include "ufo.h"
#include "modelEffect.h"
#include "tower.h"
#include "wyvern.h"
#include "pause.h"

//========================================
// �}�N����`
//========================================

// �Q�[����Ԃ̊Ǘ��t���[��
#define START_MISSION 30    // �~�b�V�����̊J�n

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CPlayer *CGame::m_pPlayer = NULL;
CScore *CGame::m_pScore = NULL;
CBulletRemain *CGame::m_pBulletRemain = NULL;
CMission *CGame::m_pMission = NULL;
CScene2D *CGame::m_apMissionUI[MISSION_UI_MAX] = {};
CPause *CGame::m_pPause = NULL;

CGame::STATE CGame::m_state = STATE_NORMAL;
CGame::STAGE CGame::m_stage = STAGE_NONE;
CGame::MAP_LIMIT CGame::m_mapLimit = {};
bool CGame::m_bMissionClear = false;

int CGame::m_nClearNum_Misson1 = 0;
int CGame::m_nClearNum_Misson2 = 0;
CGame::CURRENT_MISSION CGame::m_currentMission = CURRENT_MISSION_1;

//=============================================================================
// �Q�[���̃R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CGame::CGame()
{
    m_pPlayer = NULL;
    m_pScore = NULL;
    m_pBulletRemain = NULL;
    m_pMission = NULL;
    m_pPause = NULL;

    memset(m_apMissionUI, 0, sizeof(m_apMissionUI));
    m_nCntGameTime = 0;

    // �ÓI�����o�ϐ���������
    m_state = STATE_START;
    m_stage = STAGE_NONE;
    m_mapLimit = {};
    m_bMissionClear = false;

    m_nClearNum_Misson1 = 0;
    m_nClearNum_Misson2 = 0;
    m_currentMission = CURRENT_MISSION_1;
}

//=============================================================================
// �Q�[���̃f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CGame::Init(void)
{
    // �eUI���N���G�C�g�i�`�揇���Ӂj
    //====================================================================================
    //�X�R�A
    CUI::Create(UI_SCORE_POS, UI_SCORE_SIZE, DEFAULT_COLOR, CUI::TYPE_SCORE);

    // HP�Q�[�W
    CUI::Create(UI_STAMINA_POS, UI_STAMINA_SIZE, DEFAULT_COLOR, CUI::TYPE_HP_FRAME);
    CUI::Create(UI_GAUGE_POS, UI_GAUGE_SIZE, DEFAULT_COLOR, CUI::TYPE_HP_GAUGE);

    //HP���S
    CUI::Create(UI_HP_LOGO_POS, UI_HP_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_HP_LOGO);

    //����t���[��
    CUI::Create(UI_LEFTHAND_FRAME_POS, UI_LEFTHAND_FRAME_SIZE, DEFAULT_COLOR, CUI::TYPE_LEFTHAND_FRAME);

    //����Q�[�W�w�i
    CUI::Create(UI_LEFTHAND_GAUGE_BG_POS, UI_LEFTHAND_GAUGE_BG_SIZE, DEFAULT_COLOR, CUI::TYPE_LEFTHAND_GAUGEBG);

    //����Q�[�W
    CUI::Create(UI_LEFTHAND_GAUGE_POS, UI_LEFTHAND_GAUGE_SIZE, DEFAULT_COLOR, CUI::TYPE_LEFTHAND_GAUGE);

    //����Q�[�W�t���[��
    CUI::Create(UI_LEFTHAND_GAUGE_FRAME_POS, UI_LEFTHAND_GAUGE_FRAME_SIZE, DEFAULT_COLOR, CUI::TYPE_LEFTHAND_GAUGEFRAME);

    //����Q�[�W�d�؂�
    CUI::Create(UI_LEFTHAND_PARTITION_POS, UI_LEFTHAND_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_LEFTHAND_PARTITION);

    //�E�胍�S
    CUI::Create(UI_RIGHTHAND_LOGO_POS, UI_RIGHTHAND_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_RIGHTHAND_LOGO);

    //����c�e
    CUI::Create(UI_RIGHTHAND_STAMINA_POS, UI_RIGHTHAND_STAMINA_SIZE, DEFAULT_COLOR, CUI::TYPE_RIGHTHAND_BULLET_UI);

    //�}�b�v
    CUI::Create(UI_MINIMAP_POS, UI_MINIMAP_SIZE, DEFAULT_COLOR, CUI::TYPE_MINIMAP);

    // ���b�N�I��
    CUI::Create(LOCK_ON_POS, LOCK_ON_SIZE, DEFAULT_COLOR, CUI::TYPE_LOCK_ON);

    // �זE�\���܂�
    CUI::Create(UI_LIMIT_POS, UI_LIMIT_SIZE, DEFAULT_COLOR, CUI::TYPE_LIMIT);

    // �~�b�V�����̃��S
    CUI::Create(UI_MISSION_LOGO_POS, UI_MISSION_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_MISSION_LOGO);
    //====================================================================================

    // �^�C�}�[�̐���
    CTimer::Create(TIMER_POS, TIMER_SIZE);

    // �X�R�A�̐���
    m_pScore = CScore::Create(SCORE_POS, SCORE_SIZE);

    // �L�����N�^�[�̐���
    m_pPlayer = CPlayer::Create(PLAYER_START_POS_STAGE1, DEFAULT_VECTOR, CPlayer::PLAYABLE_001);

    // �c�e���\��
    m_pBulletRemain = CBulletRemain::Create(BULLET_REMAIN_POS, BULLET_REMAIN_SIZE);
    m_pBulletRemain->Disp(PLAYER_REMAIN_BULLETS_MAX);

    // �~�b�V�����\��
    m_pMission = CMission::Create(MISSION_POS, MISSION_SIZE);
    m_pMission->DispFirst(0);
    m_pMission->DispSecond(0);

    // �����[�h�\�L�̐���
    CUI::Create(RELOAD_POS, RELOAD_SIZE, DEFAULT_COLOR, CUI::TYPE_RELOAD);

    // �X�J�C�{�b�N�X�̐���
    CSky::Create();

    // �C�̐���
    COcean::Create(COcean::TYPE_SURFACE_0);
    COcean::Create(COcean::TYPE_SURFACE_1);
    COcean::Create(COcean::TYPE_SURFACE_2);

    // �|�[�Y�̐���
    m_pPause = CPause::Create();

    // �X�e�[�W�̐ݒ�
    SetStage(STAGE_1);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CGame::Uninit(void)
{
    // �m�ۂ����X�R�A�ƃv���C���[�Ɗe����UI�̃������́ACScene��ReleaseAll�ŊJ������Ă���

    // �S�Ẳ����~
    CSound *pSound = CManager::GetSound();
    pSound->StopAll();

    // �|�[�Y��j��
    if (m_pPause != NULL)
    {
        m_pPause->Uninit();
        delete m_pPause;
        m_pPause = NULL;
    }
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CGame::Update(void)
{
    // �Q�[����Ԃ̊Ǘ�
    ManageState();

#ifdef _DEBUG
    CDebug::Command();
#endif
}

//=============================================================================
// �X�e�[�W�̓ǂݍ���
// Author : �㓡�T�V��
//=============================================================================
void CGame::SetStage(STAGE stage)
{
    // �����������o�ϐ��Ɍ��т���
    m_stage = stage;

    // �v���C���[�̎c�e�����ő�ɂ���
    m_pPlayer->SetRemainBulletsFull();

    // �c�e���\�����ő�ɍX�V
    m_pBulletRemain->Disp(PLAYER_REMAIN_BULLETS_MAX);

    // �~�b�V�����N���A�̃t���O��false�ɖ߂�
    m_bMissionClear = false;

    // ���݂̃~�b�V������1�ɖ߂�
    m_currentMission = CURRENT_MISSION_1;

    // �~�b�V�����̒B���������Z�b�g
    m_nClearNum_Misson1 = 0;
    m_nClearNum_Misson2 = 0;
    m_pMission->DispFirst(m_nClearNum_Misson1);
    m_pMission->DispSecond(m_nClearNum_Misson2);

    // �~�b�V������UI������Ȃ�A�폜
    if (m_apMissionUI[0] != NULL)
    {
        m_apMissionUI[0]->Uninit();
        m_apMissionUI[0] = NULL;
    }
    if (m_apMissionUI[1] != NULL)
    {
        m_apMissionUI[1]->Uninit();
        m_apMissionUI[1] = NULL;
    }

    // �X�e�[�W�ɂ���āA�}�b�v�̐�����~�b�V����UI��ς���
    CSound *pSound = CManager::GetSound();
    switch (stage)
    {
    case STAGE_1:
        m_mapLimit.fWest = STAGE1_WEST_LIMIT;
        m_mapLimit.fSouth = STAGE1_SOUTH_LIMIT;
        m_mapLimit.fNorth = STAGE1_NORTH_LIMIT;
        m_mapLimit.fEast = STAGE1_EAST_LIMIT;
        CWall::SetWall(STAGE1_MAP_LIMIT);

        // �~�b�V����1
        if (m_apMissionUI[0] == NULL)
        {
            m_apMissionUI[0] = CUI::Create(UI_MISSION_POS_1, UI_MISSION_SIZE_1, DEFAULT_COLOR, CUI::TYPE_MISSION_1);
            m_apMissionUI[0]->SetTexturePlace(2, 2);
        }

        // �~�b�V����2
        if (m_apMissionUI[1] == NULL)
        {
            m_apMissionUI[1] = CUI::Create(UI_MISSION_POS_2, UI_MISSION_SIZE_2, DEFAULT_COLOR, CUI::TYPE_MISSION_2);
            m_apMissionUI[1]->SetTexturePlace(1, 2);
        }

        // �v���C���[�̏ꏊ��ς���
        m_pPlayer->SetPos(PLAYER_START_POS_STAGE1);

        // �J�����̃��b�N�I���ꏊ��ς���
        CManager::GetCamera()->CCamera::ResetGameCamera(PLAYER_START_POS_STAGE1, PLAYER_START_ROT_STAGE1);

        // BGM���Đ�
        pSound->Play(CSound::LABEL_BGM_STAGE_01_NORMAL);

        break;

    case STAGE_2:
        m_mapLimit.fWest = STAGE2_WEST_LIMIT;
        m_mapLimit.fSouth = STAGE2_SOUTH_LIMIT;
        m_mapLimit.fNorth = STAGE2_NORTH_LIMIT;
        m_mapLimit.fEast = STAGE2_EAST_LIMIT;
        CWall::SetWall(STAGE2_MAP_LIMIT);

        // �~�b�V����3
        if (m_apMissionUI[0] == NULL)
        {
            m_apMissionUI[0] = CUI::Create(UI_MISSION_POS_1, UI_MISSION_SIZE_1, DEFAULT_COLOR, CUI::TYPE_MISSION_3);
            m_apMissionUI[0]->SetTexturePlace(2, 2);
        }

        // �~�b�V����4
        if (m_apMissionUI[1] == NULL)
        {
            m_apMissionUI[1] = CUI::Create(UI_MISSION_POS_2, UI_MISSION_SIZE_2, DEFAULT_COLOR, CUI::TYPE_MISSION_4);
            m_apMissionUI[1]->SetTexturePlace(1, 2);
        }

        // �v���C���[�̏ꏊ��ς���
        m_pPlayer->SetPos(PLAYER_START_POS_STAGE2);

        // �J�����̃��b�N�I���ꏊ��ς���
        CManager::GetCamera()->CCamera::ResetGameCamera(PLAYER_START_POS_STAGE2, PLAYER_START_ROT_STAGE2);

        // BGM���Đ�
        pSound->Play(CSound::LABEL_BGM_STAGE_02_NORMAL);

        break;

    case STAGE_FINAL:
        m_mapLimit.fWest = STAGE_FINAL_WEST_LIMIT;
        m_mapLimit.fSouth = STAGE_FINAL_SOUTH_LIMIT;
        m_mapLimit.fNorth = STAGE_FINAL_NORTH_LIMIT;
        m_mapLimit.fEast = STAGE_FINAL_EAST_LIMIT;
        CWall::SetWall(STAGE_FINAL_MAP_LIMIT);

        // �~�b�V����5
        if (m_apMissionUI[0] == NULL)
        {
            m_apMissionUI[0] = CUI::Create(UI_MISSION_POS_1, UI_MISSION_SIZE_1, DEFAULT_COLOR, CUI::TYPE_MISSION_5);
            m_apMissionUI[0]->SetTexturePlace(2, 2);
        }

        // �~�b�V����6
        if (m_apMissionUI[1] == NULL)
        {
            m_apMissionUI[1] = CUI::Create(UI_MISSION_POS_2, UI_MISSION_SIZE_2, DEFAULT_COLOR, CUI::TYPE_MISSION_6);
            m_apMissionUI[1]->SetTexturePlace(1, 2);
        }

        // �v���C���[�̏ꏊ��ς���
        m_pPlayer->SetPos(PLAYER_START_POS_STAGE_FINAL);

        // �J�����̃��b�N�I���ꏊ��ς���
        CManager::GetCamera()->CCamera::ResetGameCamera(PLAYER_START_POS_STAGE_FINAL, PLAYER_START_ROT_STAGE_FINAL);

        // BGM���Đ�
        pSound->Play(CSound::LABEL_BGM_STAGE_03_NORMAL);

        break;
    }

    // �G�̔z�u�ǂݍ���
    LoadEnemyData(stage);

    // �}�b�v�̓ǂݍ���
    LoadMapData(stage);
}

//=============================================================================
// �Q�[����Ԃ̊Ǘ�
// Author : �㓡�T�V��
//=============================================================================
void CGame::ManageState(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // �����擾
    CSound *pSound = CManager::GetSound();

    // �J�����擾
    CCamera*pCamera = CManager::GetCamera();

    switch (m_state)
    {
    case STATE_START:

        // �}�E�X�J�[�\���̔�\��
        ShowCursor(FALSE);

        // �J�����Ǐ]
        pCamera->CCamera::SetState(CCamera::STATE_LOCK_ON);

        // �J�E���^�����Z
        m_nCntGameTime++;

        // ���t���[���ŁA�Q�[������
        if (m_nCntGameTime == START_MISSION)
        {
            // �J�E���^���Z�b�g
            m_nCntGameTime = 0;

            // �~�b�V�����X�^�[�g�\��
            CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_START);

            // �Q�[������
            m_state = STATE_NORMAL;
        }

        break;

    case STATE_NORMAL:

        // �}�E�X�J�[�\���̔�\��
        ShowCursor(FALSE);

        // �~�b�V�����̊Ǘ�
        ManageMission();

        // �J�����Ǐ]
        pCamera->CCamera::SetState(CCamera::STATE_LOCK_ON);

        // �X�^�[�g�{�^���������ꂽ��
        if (pInputKeyboard->GetKeyboardTrigger(DIK_P) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // �|�[�Y��
            pSound->Play(CSound::LABEL_SE_SYSTEM_PAUSE);

            // �|�[�Y��Ԃɂ���
            CPause::SetState(CPause::STATE_PAUSE);
            CGame::SetState(CGame::STATE_PAUSE);
            pCamera->SetState(CCamera::STATE_NONE);
        }

        break;

    case STATE_FINISH:

        // �}�E�X�J�[�\���̔�\��
        ShowCursor(FALSE);

        // �~�b�V�����N���A���Ă���Ȃ�
        if (m_bMissionClear == true)
        {
            // �J�����Ǐ]
            pCamera->CCamera::SetState(CCamera::STATE_LOCK_ON);
        }
        // �~�b�V�������s���Ă���Ȃ�
        else
        {
            // ���s���̃J������
            pCamera->CCamera::SetState(CCamera::STATE_FAILD);
        }

        // ���s����
        Judgment();

        break;

    case STATE_IN_FADE:

        // �}�E�X�J�[�\���̔�\��
        ShowCursor(FALSE);

        // �J�E���^���Z�b�g
        m_nCntGameTime = 0;

        // �J�������t�F�[�h���ɐݒ�
        pCamera->CCamera::SetState(CCamera::STATE_IN_FADE);

        break;

    case STATE_PAUSE:

        // �}�E�X�J�[�\���̕\��
        ShowCursor(TRUE);

        // �|�[�Y�̍X�V
        m_pPause->Update();

        break;
    }
}

//=============================================================================
// �~�b�V�����̊Ǘ�
// Author : �㓡�T�V��
//=============================================================================
void CGame::ManageMission(void)
{
    // �X�e�[�W�ƌ��݂̃~�b�V�����ɉ����āA�N���A���̏�������߂�
    int nMaxClearMission1 = 0;
    int nMaxClearMission2 = 0;
    switch (m_stage)
    {
    case CGame::STAGE_1:

        nMaxClearMission1 = STAGE1_MISSION1_MAX;
        nMaxClearMission2 = STAGE1_MISSION2_MAX;

        break;

    case CGame::STAGE_2:

        nMaxClearMission1 = STAGE2_MISSION1_MAX;
        nMaxClearMission2 = STAGE2_MISSION2_MAX;

        break;

    case CGame::STAGE_FINAL:

        nMaxClearMission1 = STAGE_FINAL_MISSION1_MAX;
        nMaxClearMission2 = STAGE_FINAL_MISSION2_MAX;

        break;
    }

    // ����𒴂��Ă�����A�����𒲐�
    // �܂��A����Ɠ����Ȃ�A���̃~�b�V�����ɂ���
    // �~�b�V����1
    if (m_nClearNum_Misson1 > nMaxClearMission1)
    {
        m_nClearNum_Misson1 = nMaxClearMission1;
    }
    else if (m_nClearNum_Misson1 == nMaxClearMission1)
    {
        // ���̃~�b�V������
        if (m_currentMission == CURRENT_MISSION_1)
        {
            // ���݂̃~�b�V������2��
            m_currentMission = CURRENT_MISSION_2;

            // �~�b�V����2�̓��e���X�V
            m_apMissionUI[1]->SetTexturePlace(2, 2);

            // �l�N�X�g�~�b�V�����\��
            CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_NEXT_MISSION);

            // �X�e�[�W���ƂɋN����d�|��
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_NEXT);
            switch (m_stage)
            {
            case CGame::STAGE_1:

                // BGM���~�߂�
                pSound->Stop(CSound::LABEL_BGM_STAGE_01_NORMAL);

                // BGM���Đ�
                pSound->Play(CSound::LABEL_BGM_STAGE_01_BOSS);

                // �h�q�R�𐶐�
                CForce::Create(D3DXVECTOR3(-100.0f, 0.0f, 2400.0f), D3DXVECTOR3(0.0f, D3DXToRadian(-90), 0.0f));
                CForce::Create(D3DXVECTOR3(-100.0f, 0.0f, 10400.0f), D3DXVECTOR3(0.0f, D3DXToRadian(-90), 0.0f));
                CForce::Create(D3DXVECTOR3(12900.0f, 0.0f, 10400.0f), D3DXVECTOR3(0.0f, D3DXToRadian(90), 0.0f));
                CForce::Create(D3DXVECTOR3(12900.0f, 0.0f, 2400.0f), D3DXVECTOR3(0.0f, D3DXToRadian(90), 0.0f));

                break;

            case CGame::STAGE_2:

                // BGM���~�߂�
                pSound->Stop(CSound::LABEL_BGM_STAGE_02_NORMAL);

                // BGM���Đ�
                pSound->Play(CSound::LABEL_BGM_STAGE_02_BOSS);

                // �^���[�𐶐�
                CTower::Create(TOWER_SPAWN_POS, DEFAULT_VECTOR);

                break;

            case CGame::STAGE_FINAL:

                // BGM���~�߂�
                pSound->Stop(CSound::LABEL_BGM_STAGE_03_NORMAL);

                // BGM���Đ�
                pSound->Play(CSound::LABEL_BGM_STAGE_03_BOSS);

                // ���C�o�[���𐶐�
                CWyvern::Create(WYVERN_SPAWN_POS, DEFAULT_VECTOR);

                break;
            }
        }
    }

    // �~�b�V����2
    if (m_nClearNum_Misson2 > nMaxClearMission2)
    {
        m_nClearNum_Misson2 = nMaxClearMission2;
    }
    else if (m_nClearNum_Misson2 == nMaxClearMission2)
    {
        // �Q�[���̏�Ԃ�FINISH��
        m_state = STATE_FINISH;

        // �~�b�V�����N���A�̃t���O��true��
        m_bMissionClear = true;
    }

    // �~�b�V�����N���A����\��
    m_pMission->DispFirst(m_nClearNum_Misson1);
    m_pMission->DispSecond(m_nClearNum_Misson2);
}

//=============================================================================
// ���s����
// Author : �㓡�T�V��
//=============================================================================
void CGame::Judgment(void)
{
    // �J�E���^�����Z
    m_nCntGameTime++;

    // �ŏ���1F�Ŕ��f���āAUI���o��
    if (m_nCntGameTime == 1)
    {
        // ���擾
        CSound *pSound = CManager::GetSound();

        // �X�e�[�W��BGM������
        switch (m_stage)
        {
        case STAGE_1:
            pSound->Stop(CSound::LABEL_BGM_STAGE_01_NORMAL);
            pSound->Stop(CSound::LABEL_BGM_STAGE_01_BOSS);
            break;
        case STAGE_2:
            pSound->Stop(CSound::LABEL_BGM_STAGE_02_NORMAL);
            pSound->Stop(CSound::LABEL_BGM_STAGE_02_BOSS);
            break;
        case STAGE_FINAL:
            pSound->Stop(CSound::LABEL_BGM_STAGE_03_NORMAL);
            pSound->Stop(CSound::LABEL_BGM_STAGE_03_BOSS);
            break;
        }

        // �~�b�V�����N���A�����Ȃ�
        if (m_bMissionClear == true)
        {
            // �N���A��
            pSound->Play(CSound::LABEL_SE_CLEAR);

            // �~�b�V�����N���A��UI
            CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_CLEAR);
        }
        // �~�b�V�������s�����Ȃ�
        else
        {
            // ���s��
            pSound->Play(CSound::LABEL_SE_FAIL);

            // �~�b�V�������s��UI
            CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_FAILD);
        }
    }

    // ���̉�ʂɍs���܂ł̃J�E���^
    if (m_nCntGameTime == COUNT_WAIT_FINISH)
    {
        // �J�E���^�����Z�b�g
        m_nCntGameTime = 0;

        // ��Ԃ��A�t�F�[�h����
        m_state = STATE_IN_FADE;

        // �~�b�V�����Ɏ��s�����A�܂��͍ŏI�X�e�[�W���I�����Ȃ�
        if (m_bMissionClear == false || m_stage == STAGE_FINAL)
        {
            // �X�R�A���}�l�[�W���ɓn��
            CManager::SetScore(m_pScore->GetScore());

            // ���U���g�Ɉڍs
            CFade::SetFade(CManager::MODE_RESULT);
        }
        // ���̃X�e�[�W��
        else
        {
            // �z���C�g�t�F�[�h
            switch (m_stage)
            {
            case STAGE_1:
                CWhiteFade::Create(WHITE_FADE_POS, WHITE_FADE_SIZE, WHITE_FADE_END_TIME, WHITE_FADE_START_COLOR, CWhiteFade::TYPE_STAGE1);
                break;

            case STAGE_2:
                CWhiteFade::Create(WHITE_FADE_POS, WHITE_FADE_SIZE, WHITE_FADE_END_TIME, WHITE_FADE_START_COLOR, CWhiteFade::TYPE_STAGE2);
                break;
            }
        }
    }
}

//=============================================================================
// �}�b�v�̓ǂݍ���
// Author : �㓡�T�V��
//=============================================================================
void CGame::LoadMapData(STAGE stage)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    int  nCntLoad = 0;
    char cReedText[128];	// �����Ƃ��ēǂݎ��p
    char cHeadText[256];	//
    char cDie[128];

    // �f�o�b�O�p�̕ϐ�
    int nType[256];
    int nWidth = 0; // ��
    int nDepth = 0; // ���s
    D3DXVECTOR3 pos[256];
    D3DXVECTOR3 rot[256];

    // �X�e�[�W�ɂ���āA�t�@�C�����J��
    switch (stage)
    {
    case STAGE_1:
        pFile = fopen("data/TXT/stage1.txt", "r");
        break;
    case STAGE_2:
        pFile = fopen("data/TXT/stage2.txt", "r");
        break;
    case STAGE_FINAL:
        pFile = fopen("data/TXT/stage3.txt", "r");
        break;
    }

    // �J����
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "MODELSET") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);

            if (strcmp(cHeadText, "MAP_SIZ") == 0) 
            {
                sscanf(cReedText, "%s %s %d %d", &cDie, &cDie, &nWidth, &nDepth);
            }
        }

        if (strcmp(cHeadText, "MODELSET") == 0)
        {
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {
                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "\n") == 0)
                {
                }
                else if (strcmp(cHeadText, "END_MODELSET") != 0)
                {
                    if (strcmp(cHeadText, "TYPE") == 0)
                    {
                        sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nType[nCntLoad]);
                    }

                    if (strcmp(cHeadText, "POS") == 0)
                    {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos[nCntLoad].x, &pos[nCntLoad].y, &pos[nCntLoad].z);
                    }

                    if (strcmp(cHeadText, "ROT") == 0)
                    {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot[nCntLoad].x, &rot[nCntLoad].y, &rot[nCntLoad].z);

                        // �u���b�N�𐶐�
                        CBlock::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), nType[nCntLoad]);

                        nCntLoad++;
                    }
                }
            }

        }

        // �t�B�[���h����
        CField::Create(stage, nWidth, nDepth);

        // �t�@�C����
        fclose(pFile);
    }

    // �J���Ȃ�
    else
    {
        printf("�J����܂���ł���\n");
    }
}

//=============================================================================
// �G�l�~�[�̔z�u�ǂݍ���
// Author : �㓡�T�V��
//=============================================================================
void CGame::LoadEnemyData(STAGE stage)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    int  nCntLoad = 0;
    char cReedText[128];	// �����Ƃ��ēǂݎ��p
    char cHeadText[256];	//
    char cDie[128];

    // �f�o�b�O�p�̕ϐ�
    int nType[256];
    D3DXVECTOR3 pos[256];
    D3DXVECTOR3 rot[256];

    // �X�e�[�W�ɂ���āA�t�@�C�����J��
    switch (stage)
    {
    case STAGE_1:
        pFile = fopen("data/TXT/enemystage1.txt", "r");
        break;
    case STAGE_2:
        pFile = fopen("data/TXT/enemystage2.txt", "r");
        break;
    case STAGE_FINAL:
        pFile = fopen("data/TXT/enemystage3.txt", "r");
        break;
    }

    // �J����
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "ENEMYSET") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "ENEMYSET") == 0)
        {
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {
                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "\n") == 0)
                {
                }
                else if (strcmp(cHeadText, "END_ENEMYSET") != 0)
                {
                    if (strcmp(cHeadText, "TYPE") == 0)
                    {
                        sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nType[nCntLoad]);
                    }

                    if (strcmp(cHeadText, "POS") == 0)
                    {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos[nCntLoad].x, &pos[nCntLoad].y, &pos[nCntLoad].z);
                    }

                    if (strcmp(cHeadText, "ROT") == 0)
                    {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot[nCntLoad].x, &rot[nCntLoad].y, &rot[nCntLoad].z);

                        switch (nType[nCntLoad])
                        {
                        case ENEMYSET_WHITE_ANT:
                            // ���A���𐶐�
                            CAnt::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CAnt::TYPE_WHITE);
                            break;
                        case ENEMYSET_BLACK_ANT:
                            // ���单�A���𐶐�
                            CAnt::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CAnt::TYPE_BLACK);
                            break;
                        case ENEMYSET_YELLOW_SPIDER:
                            // ���O���𐶐�
                            CSpider::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CSpider::TYPE_YELLOW);
                            break;
                        case ENEMYSET_BLACK_SPIDER:
                            // ���单�O���𐶐�
                            CSpider::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CSpider::TYPE_BLACK);
                            break;
                        case ENEMYSET_UFO:
                            // UFO�𐶐�
                            CUfo::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)));
                            break;
                        }

                        nCntLoad++;
                    }
                }
            }

        }
        // �t�@�C����
        fclose(pFile);
    }

    // �J���Ȃ�
    else
    {
        printf("�J����܂���ł���\n");
    }
}
