//===============================================
//
// ���f���G�t�F�N�g���� (modelEffect.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "modelEffect.h"
#include "manager.h"
#include "renderer.h"
#include "debug.h"
#include "game.h"
#include "score.h"
#include "effect3d.h"
#include "library.h"
#include "spider.h"
#include "bullet.h"
#include "ant.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
LPD3DXMESH	CModelEffect::m_apMesh[CModelEffect::TYPE_MAX] = {};
LPD3DXBUFFER CModelEffect::m_apBuffMat[CModelEffect::TYPE_MAX] = {};
DWORD CModelEffect::m_aNumMat[CModelEffect::TYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CModelEffect::m_apTexture[CModelEffect::TYPE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CModelEffect::CModelEffect() :CScene3D(CScene::OBJTYPE_MODEL_EFFECT)
{
    m_nLife = 0;
    m_type = TYPE_YELLOW_SPIDER_BODY;

    m_move = DEFAULT_VECTOR;
    m_rotMove = DEFAULT_VECTOR;
    m_bShootUp = false;
    m_bFallDown = false;

    m_bUseExplosion = false;
    m_bUseLanding = false;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CModelEffect::~CModelEffect()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CModelEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene3D::Init(pos, size);

    // �X�P�[����ݒ�
    SetScale(size);

    // ���b�V���A�e�N�X�`�����o�C���h
    BindMesh(m_apMesh[m_type], m_apBuffMat[m_type], m_aNumMat[m_type]);
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CModelEffect::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CModelEffect::Update(void)
{
    // �ʒu�A�����擾
    D3DXVECTOR3 pos = GetPos();
    D3DXVECTOR3 rot = GetRot();

    // �ʒu�A�����Ɉړ��ʂ����Z
    pos += m_move;
    rot += m_rotMove;

    // �G�𐶐����闋�Ȃ�
    if (m_type == TYPE_KAMI)
    {
        SpawnEnemy();
    }
    // ���Ȃ�
    else if (m_type == TYPE_SPIKE)
    {
        // �X�P�[�����擾
        D3DXVECTOR3 scale = GetScale();

        // �c�̂݃X�P�[�����g��
        scale.y += MODEL_EFFECT_SPIKE_EXTEND_RATE;

        // �g��̏��
        if (scale.y > MODEL_EFFECT_SPIKE_EXTEND_MAX)
        {
            scale.y = MODEL_EFFECT_SPIKE_EXTEND_MAX;
        }

        // �X�P�[����ݒ�
        SetScale(scale);
    }

    // �ł��グ��G�t�F�N�g�Ȃ�
    if (m_bShootUp == true)
    {
        // �d��
        m_move.y -= GRAVITY;

        // �n�ʂɂ�����
        if (pos.y <= 0.0f)
        {
            pos.y = 0.0f;
            m_move.y = 0.0f;

            pos.x -= m_move.x;
            pos.z -= m_move.z;
            rot -= m_rotMove;

            // ��������G�t�F�N�g�Ȃ�
            if (m_bUseExplosion == true)
            {
                // ��������t���O��false��
                m_bUseExplosion = false;

                // �����G�t�F�N�g
                ExplosionEffect();
            }

            // ���n����G�t�F�N�g�Ȃ�
            if (m_bUseLanding == true)
            {
                //rot.x = MODEL_EFFECT_TILT_RATE_X;
                //rot.z = MODEL_EFFECT_TILT_RATE_Z;
            }
        }
    }

    // �|��Ă����G�t�F�N�g�Ȃ�
    if (m_bFallDown == true)
    {
        if (m_rotMove.x > 0.0f)
        {
            // ����
            m_rotMove.x += MODEL_EFFECT_FALL_DOWN_ACCEL_SPEED;

            // ��]�̐���
            if (rot.x > D3DXToRadian(90.0f))
            {
                rot.x = D3DXToRadian(90.0f);
                m_rotMove.x = 0.0f;
            }
        }
        else if (m_rotMove.x < 0.0f)
        {
            // ����
            m_rotMove.x -= MODEL_EFFECT_FALL_DOWN_ACCEL_SPEED;

            // ��]�̐���
            if (rot.x < -D3DXToRadian(90.0f))
            {
                rot.x = -D3DXToRadian(90.0f);
                m_rotMove.x = 0.0f;
            }
        }

        if (m_rotMove.z > 0.0f)
        {
            // ����
            m_rotMove.z += MODEL_EFFECT_FALL_DOWN_ACCEL_SPEED;

            // ��]�̐���
            if (rot.z > D3DXToRadian(90.0f))
            {
                rot.z = D3DXToRadian(90.0f);
                m_rotMove.z = 0.0f;
            }
        }
        else if (m_rotMove.z < 0.0f)
        {
            // ����
            m_rotMove.z -= MODEL_EFFECT_FALL_DOWN_ACCEL_SPEED;

            // ��]�̐���
            if (rot.z < -D3DXToRadian(90.0f))
            {
                rot.z = -D3DXToRadian(90.0f);
                m_rotMove.z = 0.0f;
            }
        }
    }

    // �ʒu�A�����𔽉f
    SetPos(pos);
    SetRot(rot);

    // ���n����G�t�F�N�g�ȊO��
    if (m_bUseLanding == false)
    {
        // ���C�t�����炷
        m_nLife--;
    }

    // ���C�t��0�ȉ��Ȃ�A���g�p��
    if (m_nLife <= 0)
    {
        // �I������
        Uninit();
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CModelEffect::Draw(void)
{
    // �G�𐶐����闋�����̓��C�g�I�t
    if (m_type == TYPE_KAMI)
    {
        LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

        pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);

        CScene3D::Draw();

        // ���]������߂�
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    }
    else
    {
        CScene3D::Draw();
    }
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
HRESULT CModelEffect::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //==============================
    // ���w偂̑�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_000_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_YELLOW_SPIDER_BODY], NULL, &m_aNumMat[TYPE_YELLOW_SPIDER_BODY], &m_apMesh[TYPE_YELLOW_SPIDER_BODY]);

    //==============================
    // ���w偂̕�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_001_stomach.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_YELLOW_SPIDER_STOMACH], NULL, &m_aNumMat[TYPE_YELLOW_SPIDER_STOMACH], &m_apMesh[TYPE_YELLOW_SPIDER_STOMACH]);

    //==============================
    // ���w偂̑�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_005_leg_right_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_YELLOW_SPIDER_LEG], NULL, &m_aNumMat[TYPE_YELLOW_SPIDER_LEG], &m_apMesh[TYPE_YELLOW_SPIDER_LEG]);

    //==============================
    // ���单�w偂̑�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_000_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_SPIDER_BODY], NULL, &m_aNumMat[TYPE_BLACK_SPIDER_BODY], &m_apMesh[TYPE_BLACK_SPIDER_BODY]);

    //==============================
    // ���单�w偂̕�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_001_stomach.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_SPIDER_STOMACH], NULL, &m_aNumMat[TYPE_BLACK_SPIDER_STOMACH], &m_apMesh[TYPE_BLACK_SPIDER_STOMACH]);

    //==============================
    // ���单�w偂̑�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_005_leg_right_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_SPIDER_LEG], NULL, &m_aNumMat[TYPE_BLACK_SPIDER_LEG], &m_apMesh[TYPE_BLACK_SPIDER_LEG]);

    //==============================
    // �M���@
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Signal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGNAL], NULL, &m_aNumMat[TYPE_SIGNAL], &m_apMesh[TYPE_SIGNAL]);

    //==============================
    // �X��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/light.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LIGHT], NULL, &m_aNumMat[TYPE_LIGHT], &m_apMesh[TYPE_LIGHT]);

    //==============================
    // �X��2
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/light2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LIGHT2], NULL, &m_aNumMat[TYPE_LIGHT2], &m_apMesh[TYPE_LIGHT2]);

    //==============================
    // �K���L��1
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/large001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_LARGE_001], NULL, &m_aNumMat[TYPE_DEBRI_LARGE_001], &m_apMesh[TYPE_DEBRI_LARGE_001]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_LARGE_001]);

    //==============================
    // �K���L��2
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/large002.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_LARGE_002], NULL, &m_aNumMat[TYPE_DEBRI_LARGE_002], &m_apMesh[TYPE_DEBRI_LARGE_002]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_LARGE_002]);

    //==============================
    // �K���L��3
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/large003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_LARGE_003], NULL, &m_aNumMat[TYPE_DEBRI_LARGE_003], &m_apMesh[TYPE_DEBRI_LARGE_003]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_LARGE_003]);

    //==============================
    // �K���L��1
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/small003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_SMALL_001], NULL, &m_aNumMat[TYPE_DEBRI_SMALL_001], &m_apMesh[TYPE_DEBRI_SMALL_001]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_SMALL_001]);

    //==============================
    // �K���L��2
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/small003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_SMALL_002], NULL, &m_aNumMat[TYPE_DEBRI_SMALL_002], &m_apMesh[TYPE_DEBRI_SMALL_002]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_SMALL_002]);

    //==============================
    // �K���L��3
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/small003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_SMALL_003], NULL, &m_aNumMat[TYPE_DEBRI_SMALL_003], &m_apMesh[TYPE_DEBRI_SMALL_003]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_SMALL_003]);

    //==============================
    // ���A���̑�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WHITE_ANT_BODY], NULL, &m_aNumMat[TYPE_WHITE_ANT_BODY], &m_apMesh[TYPE_WHITE_ANT_BODY]);

    //==============================
    // ���A���̕�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/ant_002_stomach.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WHITE_ANT_STOMACH], NULL, &m_aNumMat[TYPE_WHITE_ANT_STOMACH], &m_apMesh[TYPE_WHITE_ANT_STOMACH]);

    //==============================
    // ���A���̑�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/ant_008_leg_right_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WHITE_ANT_LEG], NULL, &m_aNumMat[TYPE_WHITE_ANT_LEG], &m_apMesh[TYPE_WHITE_ANT_LEG]);

    //==============================
    // ���单�A���̑�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_ANT_BODY], NULL, &m_aNumMat[TYPE_BLACK_ANT_BODY], &m_apMesh[TYPE_BLACK_ANT_BODY]);

    //==============================
    // ���单�A���̕�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/ant_002_stomach.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_ANT_STOMACH], NULL, &m_aNumMat[TYPE_BLACK_ANT_STOMACH], &m_apMesh[TYPE_BLACK_ANT_STOMACH]);

    //==============================
    // ���单�A���̑�
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/ant_008_leg_right_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_ANT_LEG], NULL, &m_aNumMat[TYPE_BLACK_ANT_LEG], &m_apMesh[TYPE_BLACK_ANT_LEG]);

    //==============================
    // ��1
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood1.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD1], NULL, &m_aNumMat[TYPE_WOOD1], &m_apMesh[TYPE_WOOD1]);

    //==============================
    // ��2
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD2], NULL, &m_aNumMat[TYPE_WOOD2], &m_apMesh[TYPE_WOOD2]);

    //==============================
    // ��3
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood3.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD3], NULL, &m_aNumMat[TYPE_WOOD3], &m_apMesh[TYPE_WOOD3]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ki.jpg", &m_apTexture[TYPE_WOOD3]);

    //==============================
    // �K�[�h���[��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/guardrail.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_GUARDRAIL], NULL, &m_aNumMat[TYPE_GUARDRAIL], &m_apMesh[TYPE_GUARDRAIL]);

    //==============================
    // ���ӃT�C��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signEmar.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_EMAR], NULL, &m_aNumMat[TYPE_SIGN_EMAR], &m_apMesh[TYPE_SIGN_EMAR]);

    //==============================
    // �\���H�T�C��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signInter.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_INTER], NULL, &m_aNumMat[TYPE_SIGN_INTER], &m_apMesh[TYPE_SIGN_INTER]);

    //==============================
    // ���ԋ֎~�T�C��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signNoPark.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_NO_PARK], NULL, &m_aNumMat[TYPE_SIGN_NO_PARK], &m_apMesh[TYPE_SIGN_NO_PARK]);

    //==============================
    // UFO
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/00_Fighter.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_UFO], NULL, &m_aNumMat[TYPE_UFO], &m_apMesh[TYPE_UFO]);

    //==============================
    // UFO�̗�
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/02_RightWing2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, 
        &m_apBuffMat[TYPE_UFO_WING], NULL, &m_aNumMat[TYPE_UFO_WING], &m_apMesh[TYPE_UFO_WING]);

    //==============================
    // ��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/kami.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_KAMI], NULL, &m_aNumMat[TYPE_KAMI], &m_apMesh[TYPE_KAMI]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/magic001.png", &m_apTexture[TYPE_KAMI]);

    //==============================
    // �N���X�^��
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/tower/tower_crystal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CRYSTAL], NULL, &m_aNumMat[TYPE_CRYSTAL], &m_apMesh[TYPE_CRYSTAL]);

    //==============================
    // �S��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/metal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_METAL], NULL, &m_aNumMat[TYPE_METAL], &m_apMesh[TYPE_METAL]);

    //==============================
    // �~�j�N���X�^��
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/mini.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_MINI_CRYSTAL], NULL, &m_aNumMat[TYPE_MINI_CRYSTAL], &m_apMesh[TYPE_MINI_CRYSTAL]);

    //==============================
    // �����O�N���X�^��
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/long.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LONG_CRYSTAL], NULL, &m_aNumMat[TYPE_LONG_CRYSTAL], &m_apMesh[TYPE_LONG_CRYSTAL]);

    //==============================
    // X��
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Xbody.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_X_BODY], NULL, &m_aNumMat[TYPE_X_BODY], &m_apMesh[TYPE_X_BODY]);

    //==============================
    // X��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Xfoot.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_X_FOOT], NULL, &m_aNumMat[TYPE_X_FOOT], &m_apMesh[TYPE_X_FOOT]);

    //==============================
    // X����
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/XleftHand.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_X_LHAND], NULL, &m_aNumMat[TYPE_X_LHAND], &m_apMesh[TYPE_X_LHAND]);

    //==============================
    // X�E��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/XrightHand.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_X_RHAND], NULL, &m_aNumMat[TYPE_X_RHAND], &m_apMesh[TYPE_X_RHAND]);

    //==============================
    // ��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/spike.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SPIKE], NULL, &m_aNumMat[TYPE_SPIKE], &m_apMesh[TYPE_SPIKE]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/black.png", &m_apTexture[TYPE_SPIKE]);

    //==============================
    // �K��
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/05_tail_tip.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_TAIL], NULL, &m_aNumMat[TYPE_TAIL], &m_apMesh[TYPE_TAIL]);

    //==============================
    // �E��
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/08_wing_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_RIGHT_WING], NULL, &m_aNumMat[TYPE_RIGHT_WING], &m_apMesh[TYPE_RIGHT_WING]);

    //==============================
    // ����
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/09_wing_left.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LEFT_WING], NULL, &m_aNumMat[TYPE_LEFT_WING], &m_apMesh[TYPE_LEFT_WING]);

    //==============================
    // ���C�o�[����
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/01_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WYVERN_BODY], NULL, &m_aNumMat[TYPE_WYVERN_BODY], &m_apMesh[TYPE_WYVERN_BODY]);

    //==============================
    // ���C�o�[����
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/03_head.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WYVERN_HEAD], NULL, &m_aNumMat[TYPE_WYVERN_HEAD], &m_apMesh[TYPE_WYVERN_HEAD]);

    //==============================
    // ���C�o�[����
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/02_neck.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WYVERN_NECK], NULL, &m_aNumMat[TYPE_WYVERN_NECK], &m_apMesh[TYPE_WYVERN_NECK]);

    //==============================
    // ���C�o�[���r
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/06_arm_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WYVERN_ARM], NULL, &m_aNumMat[TYPE_WYVERN_ARM], &m_apMesh[TYPE_WYVERN_ARM]);

    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : �㓡�T�V��
//=============================================================================
void CModelEffect::Unload(void)
{
    for (int nCnt = 0; nCnt < CModelEffect::TYPE_MAX; nCnt++)
    {
        if (m_apMesh[nCnt] != NULL)
        {
            m_apMesh[nCnt]->Release();
            m_apMesh[nCnt] = NULL;
        }
        if (m_apBuffMat[nCnt] != NULL)
        {
            m_apBuffMat[nCnt]->Release();
            m_apBuffMat[nCnt] = NULL;
        }
        if (m_apTexture[nCnt] != NULL)
        {
            m_apTexture[nCnt]->Release();
            m_apTexture[nCnt] = NULL;
        }
    }
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CModelEffect * CModelEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 rotMove, TYPE type)
{
    // �|�C���^��p��
    CModelEffect *pModelEffect = NULL;

    // �������m��
    pModelEffect = new CModelEffect;

    // �^�C�v��R�Â�(�������̒���Bind������̂����邽�߁A�ŏ��ɕR�Â�)
    pModelEffect->m_type = type;

    // ������(���n�̓X�P�[��2�{)
    D3DXVECTOR3 scale = DEFAULT_SCALE;
    if (type == TYPE_BLACK_SPIDER_BODY || type == TYPE_BLACK_SPIDER_LEG || type == TYPE_BLACK_SPIDER_STOMACH ||
        type == TYPE_BLACK_ANT_BODY || type == TYPE_BLACK_ANT_LEG || type == TYPE_BLACK_ANT_STOMACH)
    {
        scale = SPIDER_BLACK_SCALE;
    }
    pModelEffect->Init(pos, scale);

    // ��]�����т���
    pModelEffect->SetRot(rot);

    // �����o�ϐ��ɁA���������т���
    pModelEffect->m_move = move;
    pModelEffect->m_rotMove = rotMove;

    // �c�����Ԃ�ݒ�
    pModelEffect->m_nLife = MODEL_EFFECT_MAX_LIFE;

    // ��ނɂ���ĈقȂ�ݒ�����߂�
    switch (type)
    {
    case TYPE_YELLOW_SPIDER_BODY:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_YELLOW_SPIDER_STOMACH:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_YELLOW_SPIDER_LEG:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_SPIDER_BODY:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_SPIDER_STOMACH:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_SPIDER_LEG:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_SIGNAL:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_LIGHT:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_LIGHT2:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_DEBRI_LARGE_001:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_DEBRI_LARGE_002:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_DEBRI_LARGE_003:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_DEBRI_SMALL_001:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_nLife = MODEL_EFFECT_SMALL_COLLAPSE_LIFE;
        break;
    case TYPE_DEBRI_SMALL_002:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_nLife = MODEL_EFFECT_SMALL_COLLAPSE_LIFE;
        break;
    case TYPE_DEBRI_SMALL_003:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_nLife = MODEL_EFFECT_SMALL_COLLAPSE_LIFE;
        break;
    case TYPE_WHITE_ANT_BODY:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_WHITE_ANT_STOMACH:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_WHITE_ANT_LEG:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_ANT_BODY:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_ANT_STOMACH:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_ANT_LEG:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_WOOD1:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_WOOD2:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_WOOD3:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_GUARDRAIL:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_SIGN_EMAR:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_SIGN_INTER:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_SIGN_NO_PARK:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_UFO:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseExplosion = true;
        break;
    case TYPE_UFO_WING:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_KAMI:
        pModelEffect->m_nLife = MODEL_EFFECT_KAMI_WHOLE_FRAME;
        break;
    case TYPE_CRYSTAL:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseExplosion = true;
        break;
    case TYPE_METAL:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_LONG_CRYSTAL:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_X_BODY:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_X_FOOT:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_X_LHAND:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_X_RHAND:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_TAIL:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseLanding = true;
        break;
    case TYPE_RIGHT_WING:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseLanding = true;
        break;
    case TYPE_LEFT_WING:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseLanding = true;
        break;
    case TYPE_WYVERN_BODY:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseExplosion = true;
        break;
    case TYPE_WYVERN_HEAD:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_WYVERN_NECK:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_WYVERN_ARM:
        pModelEffect->m_bShootUp = true;
        break;
    }

    return pModelEffect;
}

//=============================================================================
// �����G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CModelEffect::ExplosionEffect(void)
{
    // �ʒu���擾
    D3DXVECTOR3 pos = GetPos();

    // �������K���L���΂�
    for (int nCnt = 0; nCnt < MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV2; nCnt++)
    {
        // �ϐ��錾
        float fSpeedY = float(GetRandNum(MODEL_EFFECT_EXPLOSION_SPEED_Y_MAX, MODEL_EFFECT_EXPLOSION_SPEED_Y_MIN)) / 100.0f;
        float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

        // �ړ��ʂ����߂�
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f,
            fSpeedY, 0.0f);

        // ��]�ʂ����߂�
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
        float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // �ʒu�����߂�
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*MODEL_EFFECT_UFO_EXPLOSION_RADIUS,
            0.0f, -cosf(fAngle)*MODEL_EFFECT_UFO_EXPLOSION_RADIUS) + pos;

        // ��ނ����߂�
        int nRandNum = GetRandNum(2, 0);
        switch (nRandNum)
        {
        case 0:
            modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;
            break;
        case 1:
            modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_002;
            break;
        case 2:
            modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_003;
            break;
        }

        // �G�t�F�N�g����
        CModelEffect::Create(effectPos, rotMove, move, rotMove, modelEffectType);
    }

    // ��������(�v���C���[����������������)
    D3DXVECTOR3 size = D3DXVECTOR3(BULLET_EXPLOSION_COLLISION_SIZE_LV3, BULLET_EXPLOSION_COLLISION_SIZE_LV3, BULLET_EXPLOSION_COLLISION_SIZE_LV3);
    CBullet::Create(pos, size, DEFAULT_VECTOR,
        CBullet::TYPE_EXPLOSION, CBullet::TRIBE_PLAYER);
}

//=============================================================================
// �G��������(��ɁA�^���[�������闋���g��)
// Author : �㓡�T�V��
//=============================================================================
void CModelEffect::SpawnEnemy(void)
{
    // �X�P�[�����擾
    D3DXVECTOR3 scale = GetScale();

    // ���ڂ܂Ŋg��
    if (m_nLife > MODEL_EFFECT_KAMI_CHANGE_BORDER_FRAME)
    {
        scale.x += MODEL_EFFECT_KAMI_EXTEND_RATE;
        scale.y += MODEL_EFFECT_KAMI_EXTEND_RATE;
        scale.z += MODEL_EFFECT_KAMI_EXTEND_RATE;
    }
    // ����
    else if (m_nLife == MODEL_EFFECT_KAMI_CHANGE_BORDER_FRAME)
    {
        int nRandNum = GetRandNum(4, 1);

        switch (nRandNum)
        {
        case 1:
            // ���A���𐶐�
            CAnt::Create(GetPos(), DEFAULT_VECTOR, true, CAnt::TYPE_WHITE);
            break;
        case 2:
            // ���单�A���𐶐�
            CAnt::Create(GetPos(), DEFAULT_VECTOR, true, CAnt::TYPE_BLACK);
            break;
        case 3:
            // ���O���𐶐�
            CSpider::Create(GetPos(), DEFAULT_VECTOR, true, CSpider::TYPE_YELLOW);
            break;
        case 4:
            // ���单�O���𐶐�
            CSpider::Create(GetPos(), DEFAULT_VECTOR, true, CSpider::TYPE_BLACK);
            break;
        }
    }
    // ���ڂ𒴂�����k��
    else
    {
        scale.x -= MODEL_EFFECT_KAMI_SCALE_DOWN_RATE;
        scale.y -= MODEL_EFFECT_KAMI_SCALE_DOWN_RATE;
        scale.z -= MODEL_EFFECT_KAMI_SCALE_DOWN_RATE;
    }

    // �X�P�[����ݒ�
    SetScale(scale);
}
