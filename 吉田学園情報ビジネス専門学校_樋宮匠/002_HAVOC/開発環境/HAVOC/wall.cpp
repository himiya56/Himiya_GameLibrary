//====================================================================
//
// �ǂ̏��� (wall.cpp)
// Author : ��{��
//
//====================================================================

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "wall.h"
#include "library.h"
#include "manager.h"
#include "renderer.h"
#include "tile.h"

//========================
// �ÓI�����o�ϐ��錾
//========================
LPDIRECT3DTEXTURE9 CWall::m_pTexture = NULL;

//========================================================
// �R���X�g���N�^
// Author : ��{��
//========================================================
CWall::CWall() :CBillboard(OBJTYPE_WALL)
{
    m_fMapWidth = 0.0f;
    m_fTex = 0.0f;
    m_type = TYPE_WALL_NORTH;
    m_bAddAlpha = false;
    m_nAlpha = 255;
}

//========================================================
// �f�X�g���N�^
// Author : ��{��
//========================================================
CWall::~CWall()
{
}

//========================================================
// ����������
// Author : ��{��
//========================================================
HRESULT CWall::Init(void)
{
    // �ǂ̏�����(���W�̓}�b�v�[�ɐݒ肵���ۂ̃^�C��1�}�X���̂����+1����)
    CBillboard::Init(DEFAULT_VECTOR, D3DXVECTOR3(m_fMapWidth * TILE_SIZE, (m_fMapWidth * TILE_SIZE) / 2.0f, 0.0f));
    BindTexture(m_pTexture);
    SetAlphaTestBorder(0);

    return S_OK;
}

//========================================================
// �I������
// Author : ��{��
//========================================================
void CWall::Uninit(void)
{
    CBillboard::Uninit();
}

//========================================================
// �X�V����
// Author : ��{��
//========================================================
void CWall::Update(void)
{
    // �e�N�X�`�����W�X�V
    m_fTex += 0.001f;
    if (m_fTex >= 1.0f)
    {
        m_fTex = 0.0f;
    }
    SetTexUV(0.0f, 1.0f, m_fTex, m_fTex + 0.7f);

    // �A���t�@�l�̏���
    switch (m_bAddAlpha)
    {
    case true:
        m_nAlpha++;

        if (m_nAlpha >= 255)
        {
            m_bAddAlpha = false;
        }

        SetCol(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
        break;
    case false:
        m_nAlpha--;

        if (m_nAlpha <= 100)
        {
            m_bAddAlpha = true;
        }

        SetCol(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
        break;
    }
    CBillboard::Update();
}

//========================================================
// �`�揈��
// Author : ��{��
//========================================================
void CWall::Draw(void)
{
    CBillboard::Draw();
}

//========================================================
// �e�N�X�`���ǂݍ��ݏ���
// Author : ��{��
//========================================================
HRESULT CWall::Load(void)
{
    D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data/TEXTURE/wall.png", &m_pTexture);

    return S_OK;
}

//========================================================
// �e�N�X�`���j������
// Author : ��{��
//========================================================
void CWall::Unload(void)
{
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//========================================================
// �C���X�^���X��������
// Author : ��{��
//========================================================
CWall * CWall::Create(TYPE type, float fMapWidth)
{
    CWall *pWall;
    pWall = new CWall;
    pWall->m_type = type;
    pWall->m_fMapWidth = fMapWidth;
    pWall->Init();

    // �ǂ̌����ɂ���ď����l��ݒ�(�ǂ̍����͕��̔����Ȃ̂ł��̕�����������)
    switch (type)
    {
    case TYPE_WALL_NORTH:
        pWall->SetPos(D3DXVECTOR3((fMapWidth * TILE_SIZE) / 2 - TILE_SIZE / 2, (fMapWidth * TILE_SIZE) / 4, fMapWidth * TILE_SIZE - TILE_SIZE / 2));
        pWall->SetRot(HIT_FRONT);
        break;
    case TYPE_WALL_SOUTH:
        pWall->SetPos(D3DXVECTOR3((fMapWidth * TILE_SIZE) / 2 - TILE_SIZE / 2, (fMapWidth * TILE_SIZE) / 4, -TILE_SIZE / 2));
        pWall->SetRot(HIT_BACK);
        break;
    case TYPE_WALL_EAST:
        pWall->SetPos(D3DXVECTOR3((fMapWidth * TILE_SIZE - TILE_SIZE / 2), (fMapWidth * TILE_SIZE) / 4, (fMapWidth * TILE_SIZE) / 2 - TILE_SIZE / 2));
        pWall->SetRot(HIT_LEFT);
        break;
    case TYPE_WALL_WEST:
        pWall->SetPos(D3DXVECTOR3(-TILE_SIZE / 2, (fMapWidth * TILE_SIZE) / 4, (fMapWidth * TILE_SIZE) / 2 - TILE_SIZE / 2));
        pWall->SetRot(HIT_RIGHT);
        break;
    default:
        break;
    }
    return pWall;
}

//========================================================
// �ǂ̐ݒu
// Author : ��{��
//========================================================
void CWall::SetWall(float fMapWidth)
{
    //�@�k,��,��,���̏��ŕǂ�ݒu
    for (int nCount = 0; nCount < TYPE_MAX; nCount++)
    {
        Create((TYPE)nCount, fMapWidth);
    }
}