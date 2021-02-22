//===============================================
//
// UI���� (ui.cpp)
// Author : �㓡�T�V��(��������) / �V����s(�z�u�A�T�C�Y����)
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "character.h"
#include "player.h"

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 CUI::m_apTexture[CUI::TYPE_MAX] = {};

//========================================
// UI�̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CUI::CUI() :CScene2D(OBJTYPE::OBJTYPE_UI)
{
    m_move = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_type = CUI::TYPE_NONE;
    m_fAngle = 0.0f;
    m_bUse = true;
    m_bBlinking = false;
    m_nCntTime = 0;
    m_bFadeOut = false;
}

//========================================
// UI�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CUI::~CUI()
{

}

//========================================
// UI�̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene2D::Init(pos, size);

    // �e�N�X�`�������蓖��
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// UI�̏I������
// Author : �㓡�T�V��
//========================================
void CUI::Uninit(void)
{
    // �I������
    CScene2D::Uninit();
}

//========================================
// UI�̍X�V����
// Author : �㓡�T�V��
//========================================
void CUI::Update(void)
{
    // �ϐ��錾
    D3DXVECTOR3 pos = GetPosition();                   // �ʒu���擾
    D3DXVECTOR3 size = GetSize();                      // �T�C�Y���擾
    int nPatternAnim = 0;                              // �p�^�[���A�j���̃J�E���^�𗘗p����Ƃ�

    //�L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    switch (m_type)
    {
    case TYPE_TITLE_LOGO:

        // ���ꗎ����
        pos.y += TITLE_LOGO_FLOW_DOWN_SPEED;

        // ���̒l�Ŏ~�߂Ă���
        if (pos.y >= TITLE_LOGO_TARGET_POS_Y)
        {
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        // �G���^�[�A�܂��̓X�^�[�g�{�^������������
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // �^�C�g�����S�������ɉ���
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        break;

    case TYPE_PRESS_START:

        // �^�C�g���̎��Ԃ��J�E���g
        m_nCntTime++;

        // �^�C�g���̍ő厞��
        if (m_nCntTime > TITLE_MAX_TIME)
        {
            m_nCntTime = TITLE_MAX_TIME;
        }

        // �F��ς���
        if (m_bFadeOut == true)
        {
            m_col.r -= PRESS_START_FADE_RATE;
            m_col.g -= PRESS_START_FADE_RATE;
            m_col.b -= PRESS_START_FADE_RATE;
        }
        else
        {
            m_col.r += PRESS_START_FADE_RATE;
            m_col.g += PRESS_START_FADE_RATE;
            m_col.b += PRESS_START_FADE_RATE;
        }
        if (m_col.r <= PRESS_START_FADE_OUT_MIN)
        {
            m_bFadeOut = false;
        }
        else if (m_col.r >= 1.0f)
        {
            m_bFadeOut = true;
        }

        // �G���^�[�A�܂��̓X�^�[�g�{�^������������
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // ��莞�Ԍo�߂œ_�ŉ\��
            if (m_nCntTime >= TITLE_NEXT_SCENE)
            {
                // �_�ł̃t���O��true��
                m_bBlinking = true;
            }
            // ����ȊO�́A�J�E���^��J�ډ\�܂ň�C�ɐi�߂�
            else
            {
                m_nCntTime = TITLE_NEXT_SCENE;
            }
        }

        // �_�ł���Ȃ�
        if (m_bBlinking == true)
        {
            // �J�E���^�[��i�߂āA�p�^�[����؂�ւ���
            nPatternAnim = CountAnimation(2, 2);

            // �F��2�p�^�[���Ő؂�ւ���
            if (nPatternAnim == 1)
            {
                m_col.r = PRESS_START_FADE_OUT_MIN;
                m_col.g = PRESS_START_FADE_OUT_MIN;
                m_col.b = PRESS_START_FADE_OUT_MIN;
            }
            else
            {
                m_col.r = 1.0f;
                m_col.g = 1.0f;
                m_col.b = 1.0f;
            }
        }

        break;

    case TYPE_LOCK_ON:

        // �퓬���ȊO�A����
        if (CGame::GetState() == CGame::STATE_NORMAL)
        {
            m_col.a = 1.0f;
        }
        else
        {
            m_col.a = 0.0f;
        }

        break;

    case TYPE_RELOAD:

        // �����[�h���Ȃ�
        if (CGame::GetPlayer()->GetReloading() == true)
        {
            SetTexturePlace(2, 2);
        }
        else
        {
            // �c�e�������Ȃ�
            if (CGame::GetPlayer()->GetRemainBullets() <= PLAYER_REMAIN_BULLETS_WARNING)
            {
                SetTexturePlace(1, 2);
            }
            else
            {
                // �c�e���������Ă��Ȃ��Ȃ�A�\�����Ȃ�
                SetTexturePlace(0, 0);
            }
        }

        break;

    case TYPE_MISSION_START:

        // �g��
        size = Extend(size);

        break;

    case TYPE_NEXT_MISSION:

        // �g��
        size = Extend(size);

        break;

    case TYPE_MISSION_CLEAR:

        // �g��
        size = Extend(size);

        break;

    case TYPE_MISSION_FAILD:

        // �g��
        size = Extend(size);

        break;

    case TYPE_HP_GAUGE:

        // ������E�ɐL�т�Q�[�W
        CScene2D::SetLeftToRightGauge((float)PLAYER_MAX_LIFE, (float)(CGame::GetPlayer()->GetLife()));

        break;

    case TYPE_LEFTHAND_GAUGE:

        // ������E�ɐL�т�Q�[�W
        CScene2D::SetLeftToRightGauge((float)PLAYER_LEFT_HAND_ORBS_MAX, (float)(CGame::GetPlayer()->GetLeftHandOrbs()));

        break;

    case TYPE_LIMIT:

        break;

    case TYPE_RESULT_BG:
        // �e�N�X�`���̒��_���W��ύX
        SetFlowingAnimation(1, 400, true, CScene2D::DIRECT_RIGHT_DOWN);
        break;
    }

    // �F�𔽉f
    SetColor(m_col);

    // �ʒu�𔽉f
    SetPosition(pos);

    // �T�C�Y�𔽉f
    SetSize(size);

    // �Q�[�W�n�ȊO�́A�ʏ�̒��_���W
    if (m_type != TYPE_HP_GAUGE && m_type != TYPE_LEFTHAND_GAUGE)
    {
        SetVertex();
    }

    // �g�p�t���O��false�Ȃ����
    if (m_bUse == false)
    {
        Uninit();
    }
}

//========================================
// UI�̕`�揈��
// Author : �㓡�T�V��
//========================================
void CUI::Draw(void)
{
    // �`�揈��
    CScene2D::Draw();
}

//========================================
// UI�̉摜���[�h����
// Author : �V����s
//========================================
HRESULT CUI::Load(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ver.png", &m_apTexture[CUI::TYPE_VERSION]);	                        // �o�[�W����
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title000.png", &m_apTexture[CUI::TYPE_TITLE_BG]);	                    // �^�C�g���̔w�i
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui014.png", &m_apTexture[CUI::TYPE_TITLE_LOGO]);	                    // �^�C�g�����S
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui015.png", &m_apTexture[CUI::TYPE_PRESS_START]);	                    // �X�^�[�g�{�^���������Ă�
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LockOn.png", &m_apTexture[CUI::TYPE_LOCK_ON]);	                        // ���b�N�I��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/time001.png", &m_apTexture[CUI::TYPE_LIMIT]);                          // ��������
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui001.png", &m_apTexture[CUI::TYPE_MISSION_START]);	                // �~�b�V�����J�n
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui002.png", &m_apTexture[CUI::TYPE_NEXT_MISSION]);	                    // ���̃~�b�V����
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui003.png", &m_apTexture[CUI::TYPE_MISSION_CLEAR]);                    // �~�b�V�����N���A
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui004.png", &m_apTexture[CUI::TYPE_MISSION_FAILD]);                    // �~�b�V�������s
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Result_BgUI.png", &m_apTexture[CUI::TYPE_RESULT_BG]);                  // ���U���g�w�i
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Result_ScreenUI.png", &m_apTexture[CUI::TYPE_RESULT_SCREEN]);          // ���U���g�\��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/manual.jpg", &m_apTexture[CUI::TYPE_MANUAL]);	                        // �V�ѕ�
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION.png", &m_apTexture[CUI::TYPE_MISSION_LOGO]);	                // �~�b�V�������S
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_1.png", &m_apTexture[CUI::TYPE_MISSION_1]);	                // �~�b�V����1
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_2.png", &m_apTexture[CUI::TYPE_MISSION_2]);	                // �~�b�V����2
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_3.png", &m_apTexture[CUI::TYPE_MISSION_3]);                    // �~�b�V����3
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_4.png", &m_apTexture[CUI::TYPE_MISSION_4]);                    // �~�b�V����4
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_5.png", &m_apTexture[CUI::TYPE_MISSION_5]);	                // �~�b�V����5
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_6.png", &m_apTexture[CUI::TYPE_MISSION_6]);	                // �~�b�V����6
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/HP.png", &m_apTexture[CUI::TYPE_HP_LOGO]);								// �X�^�~�i���S
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Righthand.png", &m_apTexture[CUI::TYPE_RIGHTHAND_LOGO]);				// �E�胍�S
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lefthandpartition.png", &m_apTexture[CUI::TYPE_LEFTHAND_PARTITION]);	// ����Q�[�W�d�؂�
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lefthandframe.png", &m_apTexture[CUI::TYPE_LEFTHAND_FRAME]);			// ����X�^�~�i(�g)
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lefthandgauge.png", &m_apTexture[CUI::TYPE_LEFTHAND_GAUGE]);			// ����Q�[�W
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lefthandgaugeframe.png", &m_apTexture[CUI::TYPE_LEFTHAND_GAUGEFRAME]); // ����Q�[�W�t���[��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lefthandgaugebg.png", &m_apTexture[CUI::TYPE_LEFTHAND_GAUGEBG]);       // ����Q�[�W�w�i
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/BulletUI.png", &m_apTexture[CUI::TYPE_RIGHTHAND_BULLET_UI]);			// �E��c�e
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/minimap.png", &m_apTexture[CUI::TYPE_MINIMAP]);						// �}�b�v
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Score.png", &m_apTexture[CUI::TYPE_SCORE]);							// �X�R�A
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/HPframe.png", &m_apTexture[CUI::TYPE_HP_FRAME]);						// HP(�g)
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/HPGauge.png", &m_apTexture[CUI::TYPE_HP_GAUGE]);						// HP�Q�[�W
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui000.png", &m_apTexture[CUI::TYPE_RELOAD]);	                        // �����[�h

    return S_OK;
}

//========================================
// UI�̉摜�j������
// Author : �㓡�T�V��
//========================================
void CUI::Unload(void)
{
    // �e�N�X�`���̔j��
    for (int nCntTexture = 0; nCntTexture < CUI::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// UI�̍쐬
// Author : �㓡�T�V��
//================================
CUI *CUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CUI::TYPE type)
{
    CUI *pUI = NULL;

    // ���������m��
    pUI = new CUI;

    // �^�C�v�̂݁A�e�N�X�`�������蓖�Ă̂��߂Ɍ��т��Ă���
    pUI->m_type = type;

    // ������
    pUI->Init(pos, size);

    // �����o�ϐ������т���
    pUI->m_col = col;

    return pUI;
}

//================================
// �g�傷��UI
// Author : �㓡�T�V��
//================================
D3DXVECTOR3 CUI::Extend(D3DXVECTOR3 size)
{
    // �ϐ��錾
    D3DXVECTOR3 returnSize = size;  // �Ԃ��T�C�Y
    int nCnt = 0;                   // �J�E���^

    // ���T�C�Y�g��
    returnSize.x += UI_EXTEND_MISSION_EXTEND_RATE_X;

    // �c�T�C�Y�g��
    returnSize.y += UI_EXTEND_MISSION_EXTEND_RATE_Y;

    // ���T�C�Y�̐���
    if (returnSize.x >= UI_EXTEND_MISSION_MAX_SIZE_X)
    {
        returnSize.x = UI_EXTEND_MISSION_MAX_SIZE_X;
    }

    // �c�T�C�Y�̐���
    if (returnSize.y >= UI_EXTEND_MISSION_MAX_SIZE_Y)
    {
        returnSize.y = UI_EXTEND_MISSION_MAX_SIZE_Y;
    }

    // �A�j���[�V�����̃J�E���^�𗘗p���āAUI�̃t�F�[�h�A�E�g�ɂȂ���
    nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

    // �t�F�[�h�A�E�g���n�߂�J�E���g�𒴂�����A���l�������Ă���
    if (nCnt >= UI_EXTEND_MISSION_FADE_OUT_COUNT)
    {
        m_col.a -= UI_EXTEND_MISSION_FADE_OUT_RATE;
    }

    // �����x��0�����������A����
    if (m_col.a <= 0.0f)
    {
        m_bUse = false;
    }

    return returnSize;
}