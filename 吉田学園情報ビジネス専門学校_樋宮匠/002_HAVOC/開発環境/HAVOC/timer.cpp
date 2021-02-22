//========================================================
//
// �������Ԃ̏��� [timer.cpp]
// Author : �㓡�T�V��
//
//========================================================

//========================================================
// �C���N���[�h�t�@�C��
//========================================================
#include "timer.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "number.h"

//========================================================
// �ÓI�����o�ϐ��錾
//========================================================
bool CTimer::m_bTimeUp = false;

//========================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//========================================================
CTimer::CTimer() :CScene(CScene::OBJTYPE_UI)
{
    //�����o�ϐ��̏�����
    m_nSeconds = 0;
    m_nMinutes = 0;
    m_nCntFrame = 0;

    for (int nCnt = 0; nCnt < MAX_SECONDS_DIGIT; nCnt++)
    {
        m_apSecondsNumber[nCnt] = NULL;
    }

    m_pMinutesNumber = NULL;

    m_bTimeUp = false;
}

//========================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//========================================================
CTimer::~CTimer(void)
{

}

//========================================================
// ����������
// Author : �㓡�T�V��
//========================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    //�ϐ��錾
    CTimer *pTimer;

    //����������
    pTimer = new CTimer;

    //����������
    pTimer->Init(pos, size);

    return pTimer;
}

//========================================================
// ������
// Author : �㓡�T�V��
//========================================================
HRESULT CTimer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ���̎���
    m_nMinutes = MINUTES_TIMER;

    // �b�̎���
    m_nSeconds = START_TIMER_SECONDS;

    // ���̐���
    m_pMinutesNumber = CNumber::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), size, DEFAULT_COLOR, false);

    for (int nCnt = 0; nCnt < MAX_SECONDS_DIGIT; nCnt++)
    {
        // �b�̐���
        m_apSecondsNumber[nCnt] = CNumber::Create(D3DXVECTOR3(pos.x + (nCnt * size.x) + 75.0f, pos.y, 0.0f), size, DEFAULT_COLOR, false);
    }

    return S_OK;
}

//========================================================
// �I������
// Author : �㓡�T�V��
//========================================================
void CTimer::Uninit(void)
{
    // �b�̔j��
    for (int nCnt = 0; nCnt < MAX_SECONDS_DIGIT; nCnt++)
    {
        if (m_apSecondsNumber[nCnt] != NULL)
        {
            m_apSecondsNumber[nCnt]->Uninit();
            delete m_apSecondsNumber[nCnt];
            m_apSecondsNumber[nCnt] = NULL;
        }

    }

    // ���̔j��
    if (m_pMinutesNumber != NULL)
    {
        m_pMinutesNumber->Uninit();
        delete m_pMinutesNumber;
        m_pMinutesNumber = NULL;
    }

    // �I�u�W�F�N�g�̔j��
    Release();
}

//========================================================
// �X�V����
// Author : �㓡�T�V��
//========================================================
void CTimer::Update(void)
{
    // �Q�[�����Ȃ�
    if (CGame::GetState() == CGame::STATE_NORMAL)
    {
        // �t���[�����J�E���g
        m_nCntFrame++;

        if (m_nCntFrame % 60 == 0)
        {
            //�^�C�}�[��������
            m_nSeconds--;
        }

        // �b��0�ȉ��ɂȂ�����
        if (m_nSeconds <= -1)
        {
            if (m_nMinutes >= 1)
            {
                // �b��59�ɖ߂�
                m_nSeconds = 59;

                // ����������
                m_nMinutes--;
            }
        }
    }

    // �Q�[���̏I��
    if (m_nMinutes <= 0 && m_nSeconds <= -1)
    {
        m_nCntFrame = 0;
        m_nMinutes = 0;
        m_nSeconds = 0;

        m_bTimeUp = true;

        // �Q�[�����ʏ�Ȃ�
        if (CGame::GetState() == CGame::STATE_NORMAL)
        {
            // �Q�[���̏�Ԃ��I���ɂ���
            CGame::SetState(CGame::STATE_FINISH);
        }
    }

    // �����̕ύX
    // �ϐ��錾
    float fRadix = 10.0f; //��@(�X�R�A�\���p)

    for (int nCnt = 0; nCnt < MAX_SECONDS_DIGIT; nCnt++)
    {
        int nScore = (int)powf(fRadix, MAX_SECONDS_DIGIT - (float)nCnt);
        int nScore2 = (int)powf(fRadix, MAX_SECONDS_DIGIT - (float)nCnt - 1);
        int nAnswer = m_nSeconds % nScore / nScore2;

        //�b�̃e�N�X�`���̍��W
        m_apSecondsNumber[nCnt]->SetNumber(nAnswer);

    }

    // ���̃e�N�X�`�����W
    m_pMinutesNumber->SetNumber(m_nMinutes);
}

//========================================================
// �`�揈��
// Author : �㓡�T�V��
//========================================================
void CTimer::Draw(void)
{
    for (int nCnt = 0; nCnt < MAX_SECONDS_DIGIT; nCnt++)
    {
        // �b�̕`��
        m_apSecondsNumber[nCnt]->Draw();
    }

    // ���̕`��
    m_pMinutesNumber->Draw();
}