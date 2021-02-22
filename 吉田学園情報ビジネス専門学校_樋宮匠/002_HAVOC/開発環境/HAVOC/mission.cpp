//===============================================
//
// �~�b�V�����̏��� (mission.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "mission.h"
#include "number.h"

//========================================
// �}�N����`
//========================================

// �~�b�V�������m�̊Ԋu
#define MISSIONS_SPACE 25.0f

//========================================
// �ÓI�����o�ϐ��錾
//========================================

//========================================
// �~�b�V�����̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CMission::CMission() :CScene(OBJTYPE::OBJTYPE_UI)
{
    for (int nCnt = 0; nCnt < MAX_MISSION_NUMBER; nCnt++)
    {
        m_apFirstNumber[nCnt] = NULL;
    }

    for (int nCnt = 0; nCnt < MAX_MISSION_NUMBER; nCnt++)
    {
        m_apSecondNumber[nCnt] = NULL;
    }
}

//========================================
// �~�b�V�����̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CMission::~CMission()
{

}

//========================================
// �~�b�V�����̐���
// Author : �㓡�T�V��
//========================================
CMission* CMission::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CMission *pMission = NULL;

    // ���������m��
    // �R���X�g���N�^�Ŋe����R�Â�
    pMission = new CMission;

    // ������
    pMission->Init(pos, size);

    return pMission;
}

//========================================
// �~�b�V�����̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CMission::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �傫���ʂ���A���ꂼ��Create
    m_apFirstNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_MISSION_NUMBER + 1) / MAX_MISSION_NUMBER / 2, pos.y - MISSIONS_SPACE, 0.0f),
        D3DXVECTOR3(size.x / MAX_MISSION_NUMBER, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apFirstNumber[1] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_MISSION_NUMBER - 1) / MAX_MISSION_NUMBER / 2, pos.y - MISSIONS_SPACE, 0.0f),
        D3DXVECTOR3(size.x / MAX_MISSION_NUMBER, size.y, 0.0f), DEFAULT_COLOR, false);

    m_apSecondNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_MISSION_NUMBER + 1) / MAX_MISSION_NUMBER / 2, pos.y + MISSIONS_SPACE, 0.0f),
        D3DXVECTOR3(size.x / MAX_MISSION_NUMBER, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apSecondNumber[1] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_MISSION_NUMBER - 1) / MAX_MISSION_NUMBER / 2, pos.y + MISSIONS_SPACE, 0.0f),
        D3DXVECTOR3(size.x / MAX_MISSION_NUMBER, size.y, 0.0f), DEFAULT_COLOR, false);

    return S_OK;
}

//========================================
// �~�b�V�����̏I������
// Author : �㓡�T�V��
//========================================
void CMission::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_MISSION_NUMBER; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apFirstNumber[nCntNumber] != NULL)
        {
            // �������̏I������
            m_apFirstNumber[nCntNumber]->Uninit();

            // �������̊J��
            delete m_apFirstNumber[nCntNumber];
            m_apFirstNumber[nCntNumber] = NULL;
        }
    }

    for (int nCntNumber = 0; nCntNumber < MAX_MISSION_NUMBER; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apSecondNumber[nCntNumber] != NULL)
        {
            // �������̏I������
            m_apSecondNumber[nCntNumber]->Uninit();

            // �������̊J��
            delete m_apSecondNumber[nCntNumber];
            m_apSecondNumber[nCntNumber] = NULL;
        }
    }

    // �I�u�W�F�N�g�̔j��
    Release();
}

//========================================
// �~�b�V�����̍X�V����
// Author : �㓡�T�V��
//========================================
void CMission::Update(void)
{

}

//========================================
// �~�b�V�����̕`�揈��
// Author : �㓡�T�V��
//========================================
void CMission::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_MISSION_NUMBER; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apFirstNumber[nCntNumber] != NULL)
        {
            // �������̕`��
            m_apFirstNumber[nCntNumber]->Draw();
        }
    }

    for (int nCntNumber = 0; nCntNumber < MAX_MISSION_NUMBER; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apSecondNumber[nCntNumber] != NULL)
        {
            // �������̏I������
            m_apSecondNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// 1�ڂ̃~�b�V�����̐����ݒ菈��
// Author : �㓡�T�V��
//========================================
void CMission::DispFirst(const int nNumber)
{
    // �ϐ��錾
    int nIndex;		                // �w���@(�~�b�V�����\���p)
    int nRadix = 10;	            // ��@(�~�b�V�����\���p)
    bool bCheckStart = false;       // �������n�܂�܂ł̃`�F�b�N(������E�ւƐ��������邽��)(��:05�Ȃǂ��Ȃ���)

    // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_MISSION_NUMBER; nIndex++)
    {
        int nMission = (int)powf((float)nRadix, MAX_MISSION_NUMBER - (float)nIndex);
        int nMission2 = (int)powf((float)nRadix, MAX_MISSION_NUMBER - (float)nIndex - 1);

        // �v�Z����
        int nAnswer = nNumber % nMission / nMission2;

        // 0�ȏ�̐����������
        if (nAnswer > 0)
        {
            // �����J�n�̃`�F�b�N��true
            bCheckStart = true;
        }

        // �ݒ肵����������0���A�ő包���ڂ̐������v�Z���Ă���Ȃ�
        if (nNumber == 0 && nIndex == MAX_MISSION_NUMBER - 1)
        {
            // �����J�n�̃`�F�b�N��true
            bCheckStart = true;
        }

        // �������J�n���Ă���Ȃ�
        if (bCheckStart == true)
        {
            // �ԍ���ݒ�
            m_apFirstNumber[nIndex]->SetAlpha(1.0f);
            m_apFirstNumber[nIndex]->SetNumber(nAnswer);
        }
        else
        {
            // �J�n���Ă��Ȃ��Ȃ�A������
            m_apFirstNumber[nIndex]->SetAlpha(0.0f);
            m_apFirstNumber[nIndex]->SetNumber(nAnswer);
        }
    }
}

//========================================
// 2�ڂ̃~�b�V�����̐����ݒ菈��
// Author : �㓡�T�V��
//========================================
void CMission::DispSecond(const int nNumber)
{
    // �ϐ��錾
    int nIndex;		                // �w���@(�~�b�V�����\���p)
    int nRadix = 10;	            // ��@(�~�b�V�����\���p)
    bool bCheckStart = false;       // �������n�܂�܂ł̃`�F�b�N(������E�ւƐ��������邽��)(��:05�Ȃǂ��Ȃ���)

    // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_MISSION_NUMBER; nIndex++)
    {
        int nMission = (int)powf((float)nRadix, MAX_MISSION_NUMBER - (float)nIndex);
        int nMission2 = (int)powf((float)nRadix, MAX_MISSION_NUMBER - (float)nIndex - 1);

        // �v�Z����
        int nAnswer = nNumber % nMission / nMission2;

        // 0�ȏ�̐����������
        if (nAnswer > 0)
        {
            // �����J�n�̃`�F�b�N��true
            bCheckStart = true;
        }

        // �ݒ肵����������0���A�ő包���ڂ̐������v�Z���Ă���Ȃ�
        if (nNumber == 0 && nIndex == MAX_MISSION_NUMBER - 1)
        {
            // �����J�n�̃`�F�b�N��true
            bCheckStart = true;
        }

        // �������J�n���Ă���Ȃ�
        if (bCheckStart == true)
        {
            // �ԍ���ݒ�
            m_apSecondNumber[nIndex]->SetAlpha(1.0f);
            m_apSecondNumber[nIndex]->SetNumber(nAnswer);
        }
        else
        {
            // �J�n���Ă��Ȃ��Ȃ�A������
            m_apSecondNumber[nIndex]->SetAlpha(0.0f);
            m_apSecondNumber[nIndex]->SetNumber(nAnswer);
        }
    }
}