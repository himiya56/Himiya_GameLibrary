//===============================================
//
// ���t�F�[�h���� (whiteFade.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "whiteFade.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "effect.h"
#include "sound.h"
#include "scene.h"
#include "ui.h"

//=======================================
// �}�N����`
//=======================================

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 CWhiteFade::m_apTexture[CWhiteFade::TYPE_MAX] = {};

//========================================
// ���t�F�[�h�̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CWhiteFade::CWhiteFade() :CScene2D(OBJTYPE::OBJTYPE_WHITEFADE)
{
    m_col = DEFAULT_COLOR;
    m_nTimer = 0;
    m_type = CWhiteFade::TYPE_MAX;
    m_bAlphaDown = false;
}

//========================================
// ���t�F�[�h�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CWhiteFade::~CWhiteFade()
{

}

//========================================
// ���t�F�[�h�̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CWhiteFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene2D::Init(pos, size);

    // �e�N�X�`�������蓖��
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// ���t�F�[�h�̏I������
// Author : �㓡�T�V��
//========================================
void CWhiteFade::Uninit(void)
{
    // �I������
    CScene2D::Uninit();
}

//========================================
// ���t�F�[�h�̍X�V����
// Author : �㓡�T�V��
//========================================
void CWhiteFade::Update(void)
{
    // �ϐ��錾
    D3DXVECTOR3 pos = GetPosition();                   // �ʒu���擾
    D3DXVECTOR3 size = GetSize();                      // �T�C�Y���擾

    // ��ނɉ������ꍇ����
    switch (m_type)
    {
        // �X�e�[�W1
    case CWhiteFade::TYPE_STAGE1:

        if (m_nTimer > 0)
        {
            // �^�C�}�[���Z
            m_nTimer--;

            // �����x��������t���O��true��(1F�̂ݒʂ�)
            if (m_nTimer <= 0)
            {
                m_bAlphaDown = true;

                // �X�e�[�W���J��
                CScene::ReleaseStage();

                // �X�e�[�W��2��
                CGame::SetStage(CGame::STAGE_2);
            }
        }

        if (m_bAlphaDown == true)
        {
            // ���񂾂񓧖���
            m_col.a -= WHITE_FADE_RATE;

            // �����Ƃ݂Ȃ��l�ȉ��ɂȂ�����
            if (m_col.a <= WHITE_FADE_CONSIDER_TRANSPARENT)
            {
                // ������
                m_col.a = -0.1f;
            }

            // �����ɂȂ�����A�v���C���[�𓮂���悤�ɂ��āA�X�e�[�W�J�n��UI
            if (m_col.a < 0)
            {
                // �Q�[���̏�Ԃ��J�n�ɖ߂�
                CGame::SetState(CGame::STATE_START);
            }
        }
        else
        {
            // ���񂾂�s������
            m_col.a += WHITE_FADE_RATE;
        }

        break;

        // �X�e�[�W2
    case CWhiteFade::TYPE_STAGE2:

        if (m_nTimer > 0)
        {
            // �^�C�}�[���Z
            m_nTimer--;

            // �����x��������t���O��true��(1F�̂ݒʂ�)
            if (m_nTimer <= 0)
            {
                m_bAlphaDown = true;

                // �X�e�[�W���J��
                CScene::ReleaseStage();

                // �X�e�[�W��FINAL��
                CGame::SetStage(CGame::STAGE_FINAL);
            }
        }

        if (m_bAlphaDown == true)
        {
            // ���񂾂񓧖���
            m_col.a -= WHITE_FADE_RATE;

            // �����Ƃ݂Ȃ��l�ȉ��ɂȂ�����
            if (m_col.a <= WHITE_FADE_CONSIDER_TRANSPARENT)
            {
                // ������
                m_col.a = -0.1f;
            }

            // �����ɂȂ�����A�v���C���[�𓮂���悤�ɂ��āA�X�e�[�W�J�n��UI
            if (m_col.a < 0)
            {
                // �Q�[���̏�Ԃ��J�n�ɖ߂�
                CGame::SetState(CGame::STATE_START);
            }
        }
        else
        {
            // ���񂾂�s������
            m_col.a += WHITE_FADE_RATE;
        }

        break;
    }

    // �ʒu�𔽉f
    SetPosition(pos);

    // �T�C�Y�𔽉f
    SetSize(size);

    // �F��ύX
    CScene2D::SetColor(m_col);

    // ���_���W��ύX
    SetVertex();

    // �����Ȃ����
    if (m_col.a < 0)
    {
        Uninit(); // �����艺�ɃR�[�h�͏����Ȃ�
    }
}

//========================================
// ���t�F�[�h�̕`�揈��
// Author : �㓡�T�V��
//========================================
void CWhiteFade::Draw(void)
{
    // �`�揈��
    CScene2D::Draw();
}

//========================================
// ���t�F�[�h�̉摜���[�h����
// Author : �㓡�T�V��
//========================================
HRESULT CWhiteFade::Load(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/whiteFade000.png", &m_apTexture[CWhiteFade::TYPE_STAGE1]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/whiteFade000.png", &m_apTexture[CWhiteFade::TYPE_STAGE2]);

    return S_OK;
}

//========================================
// ���t�F�[�h�̉摜�j������
// Author : �㓡�T�V��
//========================================
void CWhiteFade::Unload(void)
{
    // �e�N�X�`���̔j��
    for (int nCntTexture = 0; nCntTexture < CWhiteFade::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// ���t�F�[�h�̍쐬
// Author : �㓡�T�V��
//================================
CWhiteFade *CWhiteFade::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, D3DXCOLOR col, CWhiteFade::TYPE type)
{
    CWhiteFade *pWhiteFade = NULL;

    // ���������m��
    pWhiteFade = new CWhiteFade;

    // �^�C�v�̂݁A�e�N�X�`�������蓖�Ă̂��߂Ɍ��т��Ă���
    pWhiteFade->m_type = type;

    // ������
    pWhiteFade->Init(pos, size);

    // �����o�ϐ������т���
    pWhiteFade->m_col = col;
    pWhiteFade->m_nTimer = nTime;

    return pWhiteFade;
}