//====================================================================
//
// �~�j�}�b�v�̏��� (minimap.cpp)
// Author : ��{��
//
//====================================================================

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "minimap.h"
#include "game.h"
#include "player.h"

//========================
// �ÓI�����o�ϐ��錾
//========================

//========================================================
// �R���X�g���N�^
// Author : ��{��
//========================================================
CMinimap::CMinimap():CScene2D(CScene::OBJTYPE_UI)
{
    m_fAngle = 0.0f;
    m_fDistance = 0.0f;
    m_fieldPos = DEFAULT_VECTOR;
    m_bUse = false;

    m_bDisp = false;
    m_nCntTime = 0;

    m_type = ICON_NONE;
}

//========================================================
// �f�X�g���N�^
// Author : ��{��
//========================================================
CMinimap::~CMinimap()
{
}

//========================================================
// ����������
// Author : ��{��
//========================================================
HRESULT CMinimap::Init(void)
{
    // �}�b�v�̏�����
    CScene2D::Init(D3DXVECTOR3(MINIMAP_POS_X, MINIMAP_POS_Y, 0.0f), D3DXVECTOR3(ENEMY_ICON_SIZE, ENEMY_ICON_SIZE, 0.0f));
    SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
    SetVertex();

    return S_OK;
}

//========================================================
// �I������
// Author : ��{��
//========================================================
void CMinimap::Uninit(void)
{
    CScene2D::Uninit();
}

//========================================================
// �X�V����
// Author : ��{��
//========================================================
void CMinimap::Update(void)
{
    // ���[�_�[�ɓG���\�������܂ł̃^�C�}�[
    if (!m_bDisp)
    {
        // �J�E���g�A�b�v
        m_nCntTime++;

        // ��莞�Ԍo�߂�����
        if (m_nCntTime >= MINIMAP_ICON_DISP_DELAY)
        {
            m_bDisp = true;
        }
    }

    // �}�b�v�A�C�R�����g�p���Ă���Ȃ�
    if (m_bUse)
    {
        // ���W�v�Z�p�ϐ�
        D3DXVECTOR3 pos = DEFAULT_VECTOR;

        // �p�x�𓯊�
        m_fAngle = CalculateAngle();

        // �����𓯊�
        m_fDistance = CalculateDistance();

        // �~�j�}�b�v����͂ݏo�����狗�����C��
        if (m_fDistance >= MAX_ICON_RANGE)
        {
            m_fDistance = MAX_ICON_RANGE;
        }

        // �~�j�}�b�v���S�_����̊p�x����~����̍��W���v�Z
        pos.x = m_fDistance * cosf(m_fAngle) + MINIMAP_POS_X;
        pos.y = m_fDistance * sinf(m_fAngle) + MINIMAP_POS_Y;

        // �ʒu���X�V
        SetPosition(pos);
        SetVertex();

        CScene2D::Update();
    }
    else
    {
        Uninit();
    }

}

//========================================================
// �`�揈��
// Author : ��{��
//========================================================
void CMinimap::Draw(void)
{
    // �\������Ȃ�
    if (m_bDisp)
    {
        CScene2D::Draw();
    }
}

//========================================================
// �e�N�X�`���ǂݍ��ݏ���
// Author : ��{��
//========================================================
HRESULT CMinimap::Load(void)
{
    return S_OK;
}

//========================================================
// �e�N�X�`���j������
// Author : ��{��
//========================================================
void CMinimap::Unload(void)
{
}

//========================================================
// �C���X�^���X��������
// Author : ��{��
//========================================================
CMinimap * CMinimap::Create(TYPE type)
{
    CMinimap *pMinimap;
    pMinimap = new CMinimap;
    pMinimap->Init();
    pMinimap->m_bUse = true;
    pMinimap->m_type = type;

    // ��ނ��ƂɁA�F��ς���
    switch (type)
    {
    case ICON_ENEMY:
        pMinimap->SetColor(ICON_COLOR_ENEMY);
        break;
    case ICON_LIFE:
        pMinimap->SetColor(ICON_COLOR_LIFE);
        break;
    case ICON_TARGET:
        pMinimap->SetColor(ICON_COLOR_TARGET);
        break;
    default:
        break;
    }

    return pMinimap;
}

//========================================================
// �v���C���[�ƓG�̋����v�Z����
// Author : ��{��
//========================================================
float CMinimap::CalculateDistance(void)
{
    float fDistance;

    // 2�_�Ԃ̋������v�Z
    fDistance = sqrtf(fabsf(m_fieldPos.x - CGame::GetPlayer()->GetPos().x) * fabsf(m_fieldPos.x - CGame::GetPlayer()->GetPos().x) +
        fabsf(m_fieldPos.z - CGame::GetPlayer()->GetPos().z) * fabsf(m_fieldPos.z - CGame::GetPlayer()->GetPos().z));

    // ���΋����ɕϊ�
    fDistance = fDistance * MAP_RELATIVE;

    return fDistance;
}

//========================================================
// �v���C���[�ƓG�̊p�x�v�Z����
// Author : ��{��
//========================================================
float CMinimap::CalculateAngle(void)
{
    float fAngle;

    // 2�_����p�x���v�Z
    fAngle = atan2f(m_fieldPos.z - CGame::GetPlayer()->GetPos().z, m_fieldPos.x - CGame::GetPlayer()->GetPos().x);

    // �p�x�̂�����C��
    fAngle += CGame::GetPlayer()->GetRot().y + D3DXToRadian(180.0f);

    return -fAngle;
}
