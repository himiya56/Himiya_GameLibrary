//====================================================================
//
// �C�̏��� (ocean.cpp)
// Author : ��{��
//
//====================================================================

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "ocean.h"
#include "library.h"
#include "manager.h"
#include "renderer.h"

//========================
// �ÓI�����o�ϐ��錾
//========================
LPDIRECT3DTEXTURE9 COcean::m_apTexture[TYPE_MAX] = {};

//========================================================
// �R���X�g���N�^
// Author : ��{��
//========================================================
COcean::COcean() :CBillboard(OBJTYPE_WORLD)
{
    m_type = TYPE_SURFACE_0;
    m_fTexX = 0.0f;
    m_fTexY = 0.0f;
    m_nAlpha = 255;
    m_bAddAlpha = true;
}

//========================================================
// �f�X�g���N�^
// Author : ��{��
//========================================================
COcean::~COcean()
{
}

//========================================================
// ����������
// Author : ��{��
//========================================================
HRESULT COcean::Init(void)
{
    // �C�̏�����
    CBillboard::Init(DEFAULT_VECTOR, D3DXVECTOR3(OCEAN_SIZE, OCEAN_SIZE, 0.0f));
    SetRot(HIT_TOP);
    BindTexture(m_apTexture[m_type]);
    m_fTexX = 0.0f;
    m_fTexY = 0.0f;
    m_nAlpha = 255;
    m_bAddAlpha = true;

    // �A���t�@�e�X�g�̐ݒ�l��ύX
    SetAlphaTestBorder(0);

    // ���Z�����𗘗p����
    SetAdditiveSynthesis();

    return S_OK;
}

//========================================================
// �I������
// Author : ��{��
//========================================================
void COcean::Uninit(void)
{
    CBillboard::Uninit();
}

//========================================================
// �X�V����
// Author : ��{��
//========================================================
void COcean::Update(void)
{
    // �A���t�@�l��ύX
    switch (m_type)
    {
    case TYPE_SURFACE_1:
        // �A���t�@�l�̌v�Z
        CalcAlpha(MIDDLE_ALPHA_RATE);
        // �e�N�X�`�����W�̉��Z
        m_fTexX += MIDDLE_MOVE_TEX;
        m_fTexY += MIDDLE_MOVE_TEX;
        break;
    case TYPE_SURFACE_2:
        // �A���t�@�l�̌v�Z
        CalcAlpha(TOP_ALPHA_RATE);
        // �e�N�X�`�����W�̉��Z
        m_fTexX += TOP_MOVE_TEX;
        m_fTexY += TOP_MOVE_TEX;
        break;
    default:
        break;
    }

    // ���̍X�V
    SetTexUV(m_fTexX, 1.0f + m_fTexX, m_fTexY, 1.0f + m_fTexY);
    CBillboard::Update();
}

//========================================================
// �`�揈��
// Author : ��{��
//========================================================
void COcean::Draw(void)
{
    CBillboard::Draw();
}

//========================================================
// �A���t�@�l�v�Z
// Author : ��{��
//========================================================
void COcean::CalcAlpha(int nAlphaRate)
{
    if (m_bAddAlpha)
    {
        m_nAlpha += nAlphaRate;
        SetCol(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
        if (m_nAlpha > ALPHA_MAX)
        {
            m_bAddAlpha = false;
        }
    }
    else
    {
        m_nAlpha -= nAlphaRate;
        SetCol(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
        if (m_nAlpha < ALPHA_MIN)
        {
            m_bAddAlpha = true;
        }
    }
}

//========================================================
// �e�N�X�`���ǂݍ��ݏ���
// Author : ��{��
//========================================================
HRESULT COcean::Load(void)
{
    D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data/TEXTURE/ocean_deep.png", &m_apTexture[TYPE_SURFACE_0]);
    D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data/TEXTURE/ocean_wave.png", &m_apTexture[TYPE_SURFACE_1]);
    D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data/TEXTURE/ocean_bubble.png", &m_apTexture[TYPE_SURFACE_2]);

    return S_OK;
}

//========================================================
// �e�N�X�`���j������
// Author : ��{��
//========================================================
void COcean::Unload(void)
{
    for (int nCount = 0; nCount < TYPE_MAX; nCount++)
    {
        if (m_apTexture[nCount] != NULL)
        {
            m_apTexture[nCount]->Release();
            m_apTexture[nCount] = NULL;
        }
    }
}

//========================================================
// �C���X�^���X��������
// Author : ��{��
//========================================================
COcean * COcean::Create(TYPE type)
{
    COcean *pOcean;
    pOcean = new COcean;
    pOcean->m_type = type;
    pOcean->Init();

    switch (type)
    {
    case TYPE_SURFACE_0:
        pOcean->SetCol(D3DCOLOR_RGBA(255, 255, 255, 255));
        pOcean->SetPos(SURFACE_0_POS);
        pOcean->m_nAlpha = 255;
        break;
    case TYPE_SURFACE_1:
        pOcean->SetCol(D3DCOLOR_RGBA(255, 255, 255, 255));
        pOcean->SetPos(SURFACE_1_POS);
        pOcean->m_nAlpha = 100;
        break;
    case TYPE_SURFACE_2:
        pOcean->SetCol(D3DCOLOR_RGBA(255, 255, 255, 0));
        pOcean->SetPos(SURFACE_2_POS);
        break;
    default:
        break;
    }
    return pOcean;
}