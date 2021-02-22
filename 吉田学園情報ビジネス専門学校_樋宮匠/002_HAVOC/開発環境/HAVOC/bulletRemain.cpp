//===============================================
//
// �c�e�̏��� (bulletRemain.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "bulletRemain.h"
#include "number.h"

//========================================
// �}�N����`
//========================================

//========================================
// �ÓI�����o�ϐ��錾
//========================================

//========================================
// �c�e�̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CBulletRemain::CBulletRemain() :CScene(OBJTYPE::OBJTYPE_UI)
{
    for (int nCnt = 0; nCnt < MAX_BULLET_REMAIN; nCnt++)
    {
        m_apNumber[nCnt] = NULL;
    }
}

//========================================
// �c�e�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CBulletRemain::~CBulletRemain()
{

}

//========================================
// �c�e�̐���
// Author : �㓡�T�V��
//========================================
CBulletRemain* CBulletRemain::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CBulletRemain *pBulletRemain = NULL;

    // ���������m��
    // �R���X�g���N�^�Ŋe����R�Â�
    pBulletRemain = new CBulletRemain;

    // ������
    pBulletRemain->Init(pos, size);

    return pBulletRemain;
}

//========================================
// �c�e�̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CBulletRemain::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �傫���ʂ���A���ꂼ��Create
    m_apNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_BULLET_REMAIN + 2) / MAX_BULLET_REMAIN / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / MAX_BULLET_REMAIN, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[1] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_BULLET_REMAIN + 0) / MAX_BULLET_REMAIN / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / MAX_BULLET_REMAIN, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[2] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_BULLET_REMAIN - 2) / MAX_BULLET_REMAIN / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / MAX_BULLET_REMAIN, size.y, 0.0f), DEFAULT_COLOR, false);

    return S_OK;
}

//========================================
// �c�e�̏I������
// Author : �㓡�T�V��
//========================================
void CBulletRemain::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_BULLET_REMAIN; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            // �������̏I������
            m_apNumber[nCntNumber]->Uninit();

            // �������̃������̊J��
            ReleaseNumber(nCntNumber);
        }
    }

    // �I�u�W�F�N�g�̔j��
    Release();
}

//========================================
// �c�e�N���X�������Ă���A�����̊J������
// Author : �㓡�T�V��
//========================================
void CBulletRemain::ReleaseNumber(int nCntNumber)
{
    // �������̊J��
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// �c�e�̍X�V����
// Author : �㓡�T�V��
//========================================
void CBulletRemain::Update(void)
{

}

//========================================
// �c�e�̕`�揈��
// Author : �㓡�T�V��
//========================================
void CBulletRemain::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_BULLET_REMAIN; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// �c�e�̐ݒ菈��
// Author : �㓡�T�V��
//========================================
void CBulletRemain::Disp(const int nNumber)
{
    // �ϐ��錾
    int nIndex;		        // �w���@(�c�e�\���p)
    int nRadix = 10;	    // ��@(�c�e�\���p)

    // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_BULLET_REMAIN; nIndex++)
    {
        int nBulletRemain = (int)powf((float)nRadix, MAX_BULLET_REMAIN - (float)nIndex);
        int nBulletRemain2 = (int)powf((float)nRadix, MAX_BULLET_REMAIN - (float)nIndex - 1);

        // 
        int nAnswer = nNumber % nBulletRemain / nBulletRemain2;

        // �ԍ���ݒ�
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }
}