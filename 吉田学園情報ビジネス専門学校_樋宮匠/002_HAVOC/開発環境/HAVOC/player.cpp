//======================================================================================
//
// �v���C���[���� (player.cpp)
// Author : �㓡�T�V��(�s���A���[�V��������) / ��{��(�K�w�\���A���[�V�����쐬)
//
//======================================================================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "library.h"
#include "block.h"
#include "bullet.h"
#include "camera.h"
#include "effect3d.h"
#include "character.h"
#include "debug.h"
#include "animation.h"
#include "item.h"
#include "game.h"
#include "bulletRemain.h"
#include "modelEffect.h"
#include "timer.h"
#include "sound.h"

//========================
// �ÓI�����o�ϐ��錾
//========================
LPD3DXMESH CPlayer::m_pMesh[PLAYABLE_MAX][PARTS_MAX] = {};
LPD3DXBUFFER CPlayer::m_pBuffMat[PLAYABLE_MAX][PARTS_MAX] = {};
DWORD CPlayer::m_nNumMat[PLAYABLE_MAX][PARTS_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CPlayer::CPlayer() :CCharacter(OBJTYPE::OBJTYPE_PLAYER)
{
    m_playable = PLAYABLE_001;
    m_move = DEFAULT_VECTOR;

    m_nAttackCoolTime = 0;
    m_bJump = true;
    m_rotDest = DEFAULT_VECTOR;
    m_rotMin = DEFAULT_VECTOR;

    m_nCntDeath = 0;

    m_bDisp = true;
    m_nCntInvincibleFrame = 0;
    m_bBlinking = false;

    m_bWallRun = false;

    m_nRemainBullets = PLAYER_REMAIN_BULLETS_MAX;
    m_bReloading = false;

    m_nCntLandingTime = 0;
    m_bJumpOld = true;

    m_nCntWallKickTime = 0;
    m_bWallKickType = false;

    m_nLeftHandOrbs = PLAYER_LEFT_HAND_ORBS_START;
    m_leftHandAttack = LEFT_HAND_ATTACK_NONE;

    m_nCntFootsteps = 0;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// ����������
// Author : ��{��
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �p�[�c����ݒ�
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_PLAYER, nCount, m_pMesh[m_playable][nCount], m_pBuffMat[m_playable][nCount], m_nNumMat[m_playable][nCount]);
    }

    // ���W�E�e�q�֌W�̓ǂݍ���
    LoadModelData("./data/ANIMATION/motion_player.txt");

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Update(void)
{
    // �^�C���A�b�v���Ă��Ȃ��Ȃ�
    if (CTimer::GetTimeUp() == false)
    {
        // �\�����Ă���Ȃ�
        if (m_bDisp == true)
        {
            // ���G���Ԃ�����Ȃ�A�J�E���^��������
            if (m_nCntInvincibleFrame > 0)
            {
                m_nCntInvincibleFrame--;

                // �_��
                m_bBlinking = !m_bBlinking;
            }
            else
            {
                // �_�ł��Ȃ�
                m_bBlinking = false;
            }

            // �U���̃N�[���^�C��
            if (m_nAttackCoolTime > 0)
            {
                m_nAttackCoolTime--;

                // �e�s���̏I����ݒ�
                if (m_nAttackCoolTime == 0)
                {
                    // �����[�h���Ȃ�
                    if (m_bReloading == true)
                    {
                        // �c�e�����ő��
                        m_nRemainBullets = PLAYER_REMAIN_BULLETS_MAX;

                        // �c�e���\�����X�V
                        CGame::GetBulletRemain()->Disp(m_nRemainBullets);

                        m_bReloading = false;
                    }

                    // ����U�����Ȃ�
                    if (m_leftHandAttack != LEFT_HAND_ATTACK_NONE)
                    {
                        // ����U���I��
                        m_leftHandAttack = LEFT_HAND_ATTACK_NONE;
                    }
                }
            }

            // �s��
            float fSpeed = PLAYER_BASE_SPEED;    // ��{�̑���
            if (m_bWallRun == false && m_bJump == false)
            {
                // �󒆂̐��䑬�x���A�ǃL�b�N�̗L���Ō���
                if (m_nCntWallKickTime > 0)
                {
                    // �ǃL�b�N��́A��⑁��
                    fSpeed = PLAYER_CONTROL_SPEED_IN_AIR_AFTER_WALL_KICK;
                }
                else
                {
                    // �ʏ�
                    fSpeed = PLAYER_CONTROL_SPEED_IN_AIR;
                }
            }
            Movement(m_playable, fSpeed);

            // �����[�h���̃G�t�F�N�g����
            if (m_bReloading == true)
            {
                SetReloadingEffect();
            }

            // �A�j���[�V����������
            CCharacter::Update();

            // ���C�t���Ȃ��Ȃ�����A��\����
            if (GetLife() <= 0)
            {
                m_bDisp = false;
            }
        }
        // �̗͂��Ȃ��Ȃ�
        else
        {
            // �Q�[�����ʏ�Ȃ�
            if (CGame::GetState() == CGame::STATE_NORMAL)
            {
                // ���ꂽ���̃G�t�F�N�g
                DeathCrush(GetPos(), GetRot());

                // �Q�[���̏�Ԃ��I���ɂ���
                CGame::SetState(CGame::STATE_FINISH);
            }
        }
    }
    // �^�C���A�b�v���Ă���Ȃ�
    else
    {
        // �\��
        m_bDisp = true;
        m_bBlinking = false;

        // �f�X�J�E���^
        m_nCntDeath++;

        // ���J�E���^�Ş�����
        if (m_nCntDeath % MODEL_EFFECT_SPIKE_SPAWN_RATE == 0)
        {
            // �̂̈ʒu�擾
            D3DXVECTOR3 bodyPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._41,
                GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._42,
                GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._43);

            // �����擾
            float fAngleX = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            float fAngleY = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            float fAngleZ = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            D3DXVECTOR3 rot = D3DXVECTOR3(fAngleX, fAngleY, fAngleZ);

            // �G�t�F�N�g����
            CModelEffect::Create(bodyPos, rot, DEFAULT_VECTOR, DEFAULT_VECTOR, CModelEffect::TYPE_SPIKE);
        }

        // ���[�V�������f�X�ɂ���
        GetAnimation()->SetAnimation(ANIM_DEATH);

        // �A�j���[�V����������
        CCharacter::Update();
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Draw(void)
{
    // �\������Ȃ�A�`��
    if (m_bDisp == true)
    {
        // �_�ł��Ă��Ȃ��Ȃ�`��
        if (m_bBlinking == false)
        {
            CCharacter::Draw();
        }
    }
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : ��{��
//=============================================================================
HRESULT CPlayer::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �v���C���[1
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/00_bodyD.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_HIP], NULL, &m_nNumMat[PLAYABLE_001][PARTS_HIP], &m_pMesh[PLAYABLE_001][PARTS_HIP]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/01_bodyU.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_BODY], NULL, &m_nNumMat[PLAYABLE_001][PARTS_BODY], &m_pMesh[PLAYABLE_001][PARTS_BODY]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/02_head.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_HEAD], NULL, &m_nNumMat[PLAYABLE_001][PARTS_HEAD], &m_pMesh[PLAYABLE_001][PARTS_HEAD]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/03_armUR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RSHOULDER], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RSHOULDER], &m_pMesh[PLAYABLE_001][PARTS_RSHOULDER]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/04_armDR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RARM], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RARM], &m_pMesh[PLAYABLE_001][PARTS_RARM]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/05_handR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RHAND], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RHAND], &m_pMesh[PLAYABLE_001][PARTS_RHAND]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/06_armUL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LSHOULDER], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LSHOULDER], &m_pMesh[PLAYABLE_001][PARTS_LSHOULDER]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/07_armDL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LARM], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LARM], &m_pMesh[PLAYABLE_001][PARTS_LARM]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/08_handL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LHAND], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LHAND], &m_pMesh[PLAYABLE_001][PARTS_LHAND]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/09_legUR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RTHIGH], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RTHIGH], &m_pMesh[PLAYABLE_001][PARTS_RTHIGH]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/10_legDR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RLEG], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RLEG], &m_pMesh[PLAYABLE_001][PARTS_RLEG]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/11_footR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_RFOOT], NULL, &m_nNumMat[PLAYABLE_001][PARTS_RFOOT], &m_pMesh[PLAYABLE_001][PARTS_RFOOT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/12_legUL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LTHIGH], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LTHIGH], &m_pMesh[PLAYABLE_001][PARTS_LTHIGH]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/13_legDL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LLEG], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LLEG], &m_pMesh[PLAYABLE_001][PARTS_LLEG]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/player/14_footL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PLAYABLE_001][PARTS_LFOOT], NULL, &m_nNumMat[PLAYABLE_001][PARTS_LFOOT], &m_pMesh[PLAYABLE_001][PARTS_LFOOT]);

    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : ��{��
//=============================================================================
void CPlayer::Unload(void)
{
    for (int nCountType = 0; nCountType < PLAYABLE_MAX; nCountType++)
    {
        for (int nCount = 0; nCount < PARTS_MAX; nCount++)
        {
            if (m_pBuffMat[nCountType][nCount] != NULL)
            {
                m_pBuffMat[nCountType][nCount] = NULL;
            }

            if (m_nNumMat[nCountType][nCount] != NULL)
            {
                m_nNumMat[nCountType][nCount] = NULL;
            }

            if (m_pMesh[nCountType][nCount] != NULL)
            {
                m_pMesh[nCountType][nCount] = NULL;
            }
        }
    }
}

//=============================================================================
// �C���X�^���X����
// Author : �㓡�T�V��
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PLAYABLE playable)
{
    // �������m��
    CPlayer *pPlayer = NULL;
    pPlayer = new CPlayer;

    // �e���̏���ݒ�
    pPlayer->SetPos(pos);
    pPlayer->SetRot(rot);
    pPlayer->SetTribe(CCharacter::TRIBE_PLAYER);
    pPlayer->SetActiveCollisionSize(PLAYER_COLLISION_SIZE);
    pPlayer->SetLife(PLAYER_MAX_LIFE);

    // �ǂݍ��ގ�ނ̐ݒ�
    pPlayer->m_playable = playable;

    // ������
    pPlayer->Init(pos, DEFAULT_SCALE);

    return pPlayer;
}

//=============================================================================
// �����[�h���̋z���G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SetReloadingEffect(void)
{
    // �E��̈ʒu�擾
    D3DXVECTOR3 rightHandPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._41,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._42,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._43);

    // �����[�h�G�t�F�N�g�p�̕ϐ��錾
    float fAngle = 0.0f;                       // �p�x
    float fSize = 0.0f;                        // �傫��
    CCamera *pCamera = CManager::GetCamera();  // �J�������擾

    // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

    // �ʒu�����߂�
    D3DXVECTOR3 pos = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*EFFECT_RELOADING_RADIUS,
        0.0f, -cosf(fAngle + pCamera->GetAngle())*EFFECT_RELOADING_RADIUS) + rightHandPos;

    // �傫�����o��
    fSize = float(GetRandNum(EFFECT_RELOADING_MAX_SIZE, EFFECT_RELOADING_MIN_SIZE)) / 100.0f;

    // �傫�������߂�
    D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

    // �G�t�F�N�g����
    CEffect3D::Create(pos, size, DEFAULT_VECTOR, EFFECT_RELOADING_COLOR,
        DEFAULT_VECTOR, EFFECT_RELOADING_FADE_OUT_RATE, CEffect3D::TYPE_RELOADING);

    // �G�t�F�N�g����
    CEffect3D::Create(pos, size * EFFECT_RELOADING_CENTER_REDUCE_SIZE_RATE, DEFAULT_VECTOR, EFFECT_RELOADING_CENTER_COLOR,
        DEFAULT_VECTOR, EFFECT_RELOADING_FADE_OUT_RATE, CEffect3D::TYPE_RELOADING);
}

//=============================================================================
// �ˌ����̔�U�G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SetScatteringBullet(void)
{
    // �E��̈ʒu�擾
    D3DXVECTOR3 rightHandPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._41,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._42,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._43);

    // �ˌ��G�t�F�N�g�p�̕ϐ��錾
    float fAngle = 0.0f;                       // �p�x
    float fSpeed = 0.0f;                       // ����
    float fSize = 0.0f;                        // �傫��
    CCamera *pCamera = CManager::GetCamera();  // �J�������擾
    for (int nCnt = 0; nCnt < EFFECT_SCATTERING_BULLET_NUMBER_ONCE; nCnt++)
    {
        // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // �������o��
        fSpeed = float(GetRandNum(EFFECT_SCATTERING_BULLET_MAX_SPEED, EFFECT_SCATTERING_BULLET_MIN_SPEED)) / 100.0f;

        // �ړ��ʂ����߂�
        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed, 
            cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

        // �傫�����o��
        fSize = float(GetRandNum(EFFECT_SCATTERING_BULLET_MAX_SIZE, EFFECT_SCATTERING_BULLET_MIN_SIZE)) / 100.0f;

        // �傫�������߂�
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // �G�t�F�N�g����
        CEffect3D::Create(rightHandPos, size, move, EFFECT_SCATTERING_BULLET_COLOR,
            DEFAULT_VECTOR, EFFECT_SCATTERING_BULLET_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
    }
}

//=============================================================================
// ���n���̒n����G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SetLandingCrack(D3DXVECTOR3 pos)
{
    // �n�ʂ��Ђъ���鉹
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_CRACK);

    // 4��ނ̒n����̂����A�ǂꂩ����g��
    int nRandNum = GetRandNum(4, 1);
    switch (nRandNum)
    {
    case 1:
        // �n����1����
        CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS + pos.y, pos.z), EFFECT_CRACK_SIZE,
            DEFAULT_VECTOR, DEFAULT_COLOR, HIT_TOP, EFFECT_CRACK_FADE_OUT_RATE, CEffect3D::TYPE_CRACK1);
        break;
    case 2:
        // �n����2����
        CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS + pos.y, pos.z), EFFECT_CRACK_SIZE,
            DEFAULT_VECTOR, DEFAULT_COLOR, HIT_TOP, EFFECT_CRACK_FADE_OUT_RATE, CEffect3D::TYPE_CRACK2);
        break;
    case 3:
        // �n����3����
        CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS + pos.y, pos.z), EFFECT_CRACK_SIZE,
            DEFAULT_VECTOR, DEFAULT_COLOR, HIT_TOP, EFFECT_CRACK_FADE_OUT_RATE, CEffect3D::TYPE_CRACK3);
        break;
    case 4:
        // �n����4����
        CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS + pos.y, pos.z), EFFECT_CRACK_SIZE,
            DEFAULT_VECTOR, DEFAULT_COLOR, HIT_TOP, EFFECT_CRACK_FADE_OUT_RATE, CEffect3D::TYPE_CRACK4);
        break;
    }

    // �������K���L���΂�
    for (int nCnt = 0; nCnt < MODEL_EFFECT_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
    {
        // �ϐ��錾
        float fSpeedY = float(GetRandNum(MODEL_EFFECT_LANDING_CRACK_SPEED_Y_MAX, MODEL_EFFECT_LANDING_CRACK_SPEED_Y_MIN)) / 100.0f;
        float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

        // �ړ��ʂ����߂�
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f,
            fSpeedY, 0.0f);

        // ��]�ʂ����߂�
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        float fAngle = 0.0f;                       // �p�x

        // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // �ʒu�����߂�
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*MODEL_EFFECT_LANDING_CRACK_RADIUS,
            0.0f, -cosf(fAngle)*MODEL_EFFECT_LANDING_CRACK_RADIUS) + pos;

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
}

//=============================================================================
// �ǃL�b�N�G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SetWallKickEffect(D3DXVECTOR3 pos)
{
    // �ǃL�b�N��
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_KICK);

    for (int nCnt = 0; nCnt < MODEL_EFFECT_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
    {
        // �ϐ��錾
        float fSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
        float fSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
        float fSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
        float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

        // �ړ��ʂ����߂�(�J�����̂ق��ɔ��ł��Ă���C���[�W)
        CCamera *pCamera = CManager::GetCamera();
        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(pCamera->GetAngle())*fSpeedX,
            fSpeedY, -cosf(pCamera->GetAngle())*fSpeedZ);

        // ��]�ʂ����߂�
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

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
        CModelEffect::Create(pos, rotMove, move, rotMove, modelEffectType);
    }

    // �������ւ̃p�[�e�B�N����p�����q�b�g�G�t�F�N�g�p�̕ϐ��錾
    float fAngle = 0.0f;                       // �p�x
    float fSpeed = 0.0f;                       // ����
    float fSize = 0.0f;                        // �傫��
    CCamera *pCamera = CManager::GetCamera();  // �J�������擾
    for (int nCnt = 0; nCnt < EFFECT_HIT_NON_LIVING_NUMBER_ONCE; nCnt++)
    {
        // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // �������o��
        fSpeed = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SPEED, EFFECT_HIT_NON_LIVING_MIN_SPEED)) / 100.0f;

        // �ړ��ʂ����߂�
        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed,
            cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

        // �傫�����o��
        fSize = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SIZE, EFFECT_HIT_NON_LIVING_MIN_SIZE)) / 100.0f;

        // �傫�������߂�
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // �G�t�F�N�g����
        CEffect3D::Create(pos, size, move, EFFECT_HIT_NON_LIVING_COLOR,
            DEFAULT_VECTOR, EFFECT_HIT_NON_LIVING_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
    }
}

//=============================================================================
// �s��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Movement(int nCnt, float fSpeed)
{
    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(nCnt);

    // �X�e�B�b�N���|��Ă���Ȃ�X�e�B�b�N�̊p�x���L��
    float fAngleStick = STICK_NEUTRAL;
    if (Controller.lY != 0 || Controller.lX != 0)
    {
        fAngleStick = atan2(Controller.lX, Controller.lY*-1);
    }

    // �J�������擾
    CCamera *pCamera = CManager::GetCamera();

    // �L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �����擾
    CSound *pSound = CManager::GetSound();

    // �ʒu�ƌ������擾
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    D3DXVECTOR3 rot = DEFAULT_VECTOR;
    pos = GetPos();
    rot = GetRot();

    // 1F�O�̈ʒu���L��
    SetPosOld(pos);

    // ���̈ړ��ʐ���
    if (m_move.x != 0.0f || m_move.z != 0.0f)
    {
        // �W�����v�ł���Ȃ�
        if (m_bJump == true)
        {
            m_move.x *= PLAYER_CONTROL_MOVE;
            m_move.z *= PLAYER_CONTROL_MOVE;
        }
        // �W�����v�ł��Ȃ��Ȃ�A�󒆂ł̐���
        else
        {
            m_move.x *= PLAYER_CONTROL_MOVE_IN_AIR;
            m_move.z *= PLAYER_CONTROL_MOVE_IN_AIR;
        }
    }

    // ���[�V�������܂��͑ҋ@�ɂ���
    GetAnimation()->SetAnimation(ANIM_IDLE);

    // �v���C���[�ړ�����
    // �Q�[�����t�F�[�h���ȊO�Ȃ�
    if (CGame::GetState() != CGame::STATE_IN_FADE)
    {
        Control(nCnt, fSpeed);
    }

    // ��]����
    rot = RotControl(rot);

    // �ړ��ʂƈʒu�����т���
    pos += m_move;

    // �n�ʂ���Ȃ�
    if (pos.y > 0.0f)
    {
        // �d��
        m_move.y -= GRAVITY;

        // �W�����v�s��
        m_bJump = false;
    }
    else
    {
        // �n�ʂɂ���Ȃ�A�d�͂������Ȃ��ŃW�����v�\
        pos.y = 0.0f;
        m_move.y = 0.0f;
        m_bJump = true;
    }

    // �u���b�N�Ƃ̓����蔻��
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BLOCK);
    m_bWallRun = false; // ��x�t���O�����Z�b�g
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BLOCK); nCntScene++)
    {
        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // ���̃V�[�����L��
            CScene*pNextScene = pScene->GetNextScene();

            // �u���b�N�ɃL���X�g
            CBlock *pBlock = (CBlock*)pScene;

            // �u���b�N�Ƃ̓����蔻����v�Z
            BLOCK_HIT_INFO blockHitInfo;
            blockHitInfo.hitSurface = HIT_SURFACE_NONE;
            blockHitInfo.pos = pos;
            blockHitInfo = (BlockCollision3D(&pos, &GetPosOld(), &pBlock->GetPos(),
                &GetActiveCollisionSize(), &pBlock->GetCollisionSize()));

            // �u���b�N�̓����蔻��Ōv�Z�����ʒu���A���т���
            pos = blockHitInfo.pos;

            // �q�b�g�ʂ���āA�Ǒ����W�����v�̎d�l��ς���
            //=====================================================================================
            switch (blockHitInfo.hitSurface)
            {
            case HIT_SURFACE_TOP:

                m_move.y = 0.0f;
                m_bJump = true;

                break;

            case HIT_SURFACE_LEFT:

                // �Ǒ���ł���Ȃ�
                if (pBlock->GetWallRun() == true)
                {
                    // �����[�h�ƍ���U�������Ă��Ȃ��Ȃ�
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // �O�ɐi��ł���Ȃ�
                        if (STICK_UP(fAngleStick) || pInputKeyboard->GetKeyboardPress(DIK_W))
                        {
                            // �Ǒ���̃t���O��true
                            m_bWallRun = true;

                            // �Ǒ���
                            pos.y += PLAYER_WALL_RAN_SPEED;

                            // �d�͖���
                            m_move.y = 0.0f;

                            // �X��
                            rot.x += PLAYER_TILT_SPEED;
                        }
                    }
                }

                // �ǂɂ߂荞�܂Ȃ�
                m_move.x = 0.0f;
                m_move.z = 0.0f;

                // �W�����v�{�^���������ꂽ��
                if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_L2) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))
                {
                    // �����[�h�ƍ���U�������Ă��Ȃ��Ȃ�
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // �L�b�N�W�����v
                        m_move.y = PLAYER_WALL_KICK_JUMP_VALUE;

                        // �ǃL�b�N
                        pos.x -= PLAYER_WALL_KICK_VALUE;
                        m_move.x = -PLAYER_WALL_KICK_VALUE;

                        // �ǂɃ_���[�W
                        pBlock->SetTakeDamage(BLOCK_TAKE_DAMAGE_BY_WALL_KICK, CBlock::LAST_HIT_PLAYER);

                        // �ǃL�b�N�G�t�F�N�g
                        SetWallKickEffect(pos);

                        // �ǃL�b�N�J�E���^
                        m_nCntWallKickTime = PLAYER_WALL_KICK_TIME;

                        // �ǃL�b�N�̎�ނ𔽓]
                        m_bWallKickType = !m_bWallKickType;
                    }
                }

                break;

            case HIT_SURFACE_BACK:

                // �Ǒ���ł���Ȃ�
                if (pBlock->GetWallRun() == true)
                {
                    // �����[�h�ƍ���U�������Ă��Ȃ��Ȃ�
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // �O�ɐi��ł���Ȃ�
                        if (STICK_UP(fAngleStick) || pInputKeyboard->GetKeyboardPress(DIK_W))
                        {
                            // �Ǒ���̃t���O��true
                            m_bWallRun = true;

                            // �Ǒ���
                            pos.y += PLAYER_WALL_RAN_SPEED;

                            // �d�͖���
                            m_move.y = 0.0f;

                            // �X��
                            rot.x += PLAYER_TILT_SPEED;
                        }
                    }
                }

                // �ǂɂ߂荞�܂Ȃ�
                m_move.x = 0.0f;
                m_move.z = 0.0f;

                // �W�����v�{�^���������ꂽ��
                if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_L2) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))
                {
                    // �����[�h�ƍ���U�������Ă��Ȃ��Ȃ�
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // �L�b�N�W�����v
                        m_move.y = PLAYER_WALL_KICK_JUMP_VALUE;

                        // �ǃL�b�N
                        pos.z += PLAYER_WALL_KICK_VALUE;
                        m_move.z = PLAYER_WALL_KICK_VALUE;

                        // �ǂɃ_���[�W
                        pBlock->SetTakeDamage(BLOCK_TAKE_DAMAGE_BY_WALL_KICK, CBlock::LAST_HIT_PLAYER);

                        // �ǃL�b�N�G�t�F�N�g
                        SetWallKickEffect(pos);

                        // �ǃL�b�N�J�E���^
                        m_nCntWallKickTime = PLAYER_WALL_KICK_TIME;

                        // �ǃL�b�N�̎�ނ𔽓]
                        m_bWallKickType = !m_bWallKickType;
                    }
                }

                break;

            case HIT_SURFACE_RIGHT:

                // �Ǒ���ł���Ȃ�
                if (pBlock->GetWallRun() == true)
                {
                    // �����[�h�ƍ���U�������Ă��Ȃ��Ȃ�
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // �O�ɐi��ł���Ȃ�
                        if (STICK_UP(fAngleStick) || pInputKeyboard->GetKeyboardPress(DIK_W))
                        {
                            // �Ǒ���̃t���O��true
                            m_bWallRun = true;

                            // �Ǒ���
                            pos.y += PLAYER_WALL_RAN_SPEED;

                            // �d�͖���
                            m_move.y = 0.0f;

                            // �X��
                            rot.x += PLAYER_TILT_SPEED;
                        }
                    }
                }

                // �ǂɂ߂荞�܂Ȃ�
                m_move.x = 0.0f;
                m_move.z = 0.0f;

                // �W�����v�{�^���������ꂽ��
                if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_L2) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))
                {
                    // �����[�h�ƍ���U�������Ă��Ȃ��Ȃ�
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // �L�b�N�W�����v
                        m_move.y = PLAYER_WALL_KICK_JUMP_VALUE;

                        // �ǃL�b�N
                        pos.x += PLAYER_WALL_KICK_VALUE;
                        m_move.x = PLAYER_WALL_KICK_VALUE;

                        // �ǂɃ_���[�W
                        pBlock->SetTakeDamage(BLOCK_TAKE_DAMAGE_BY_WALL_KICK, CBlock::LAST_HIT_PLAYER);

                        // �ǃL�b�N�G�t�F�N�g
                        SetWallKickEffect(pos);

                        // �ǃL�b�N�J�E���^
                        m_nCntWallKickTime = PLAYER_WALL_KICK_TIME;

                        // �ǃL�b�N�̎�ނ𔽓]
                        m_bWallKickType = !m_bWallKickType;
                    }
                }

                break;

            case HIT_SURFACE_FRONT:

                // �Ǒ���ł���Ȃ�
                if (pBlock->GetWallRun() == true)
                {
                    // �����[�h�ƍ���U�������Ă��Ȃ��Ȃ�
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // �O�ɐi��ł���Ȃ�
                        if (STICK_UP(fAngleStick) || pInputKeyboard->GetKeyboardPress(DIK_W))
                        {
                            // �Ǒ���̃t���O��true
                            m_bWallRun = true;

                            // �Ǒ���
                            pos.y += PLAYER_WALL_RAN_SPEED;

                            // �d�͖���
                            m_move.y = 0.0f;

                            // �X��
                            rot.x += PLAYER_TILT_SPEED;
                        }
                    }
                }

                // �ǂɂ߂荞�܂Ȃ�
                m_move.x = 0.0f;
                m_move.z = 0.0f;

                // �W�����v�{�^���������ꂽ��
                if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_L2) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))
                {
                    // �����[�h�ƍ���U�������Ă��Ȃ��Ȃ�
                    if (m_bReloading == false && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                    {
                        // �L�b�N�W�����v
                        m_move.y = PLAYER_WALL_KICK_JUMP_VALUE;

                        // �ǃL�b�N
                        pos.z -= PLAYER_WALL_KICK_VALUE;
                        m_move.z = -PLAYER_WALL_KICK_VALUE;

                        // �ǂɃ_���[�W
                        pBlock->SetTakeDamage(BLOCK_TAKE_DAMAGE_BY_WALL_KICK, CBlock::LAST_HIT_PLAYER);

                        // �ǃL�b�N�G�t�F�N�g
                        SetWallKickEffect(pos);

                        // �ǃL�b�N�J�E���^
                        m_nCntWallKickTime = PLAYER_WALL_KICK_TIME;

                        // �ǃL�b�N�̎�ނ𔽓]
                        m_bWallKickType = !m_bWallKickType;
                    }
                }

                break;
            }
            //=====================================================================================

            // ���̃V�[���ɂ���
            pScene = pNextScene;
        }
        else
        {
            // ���g���Ȃ��Ȃ�A�����ŏ������I����
            break;
        }
    }

    // �G�Ƃ̓����蔻��
    pScene = CScene::GetTopScene(CScene::OBJTYPE_ENEMY);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_ENEMY); nCntScene++)
    {
        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // ���̃V�[�����L��
            CScene*pNextScene = pScene->GetNextScene();

            // �L�����N�^�[�ɃL���X�g
            CCharacter *pEnemy = (CCharacter*)pScene;

            // �����蔻��̐�����
            for (int nCntCollision = 0; nCntCollision < pEnemy->GetCntCollision(); nCntCollision++)
            {
                // �G�l�~�[�̈ʒu�Ƒ傫�����擾
                CCharacter::COLLISION enemyCollision = pEnemy->GetCollision(nCntCollision);

                // �������Ă���Ȃ�
                if (RectangleCollision3D(&pos, &enemyCollision.pos,
                    &GetActiveCollisionSize(), &enemyCollision.size) == true)
                {
                    // �푰�ɉ����āA�_���[�W���󂯂�
                    switch (pEnemy->GetTribe())
                    {
                    case TRIBE_ANT:
                        TakeDamage(PLAYER_TOUCH_ANT, pEnemy->GetPosOld());
                        break;

                    case TRIBE_SPIDER:
                        TakeDamage(PLAYER_TOUCH_SPIDER, pEnemy->GetPosOld());
                        break;

                    case TRIBE_UFO:
                        TakeDamage(PLAYER_TOUCH_UFO, pEnemy->GetPosOld());
                        break;

                    case TRIBE_TOWER:
                        TakeDamage(PLAYER_TOUCH_TOWER, pEnemy->GetPosOld());
                        break;

                    case TRIBE_WYVERN:
                        TakeDamage(PLAYER_TOUCH_WYVERN, pEnemy->GetPosOld());
                        break;
                    }
                }
            }

            // ���̃V�[���ɂ���
            pScene = pNextScene;
        }
        else
        {
            // ���g���Ȃ��Ȃ�A�����ŏ������I����
            break;
        }
    }

    // �}�b�v����
    CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
    if (pos.x - GetActiveCollisionSize().x < mapLimit.fWest)
    {
        pos.x = mapLimit.fWest + GetActiveCollisionSize().x;
    }
    if (pos.x + GetActiveCollisionSize().x > mapLimit.fEast)
    {
        pos.x = mapLimit.fEast - GetActiveCollisionSize().x;
    }
    if (pos.z - GetActiveCollisionSize().z < mapLimit.fSouth)
    {
        pos.z = mapLimit.fSouth + GetActiveCollisionSize().z;
    }
    if (pos.z + GetActiveCollisionSize().z > mapLimit.fNorth)
    {
        pos.z = mapLimit.fNorth - GetActiveCollisionSize().z;
    }

    // �W�����v�{�^���������ꂽ��
    // �Q�[�����t�F�[�h���ȊO�Ȃ�
    if (CGame::GetState() != CGame::STATE_IN_FADE)
    {
        if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_L2) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))
        {
            // ������Ȃ�
            if (m_nCntInvincibleFrame <= PLAYER_START_MOVE_IN_INVINCIBLE)
            {
                // �����[�h���⍶��U�����łȂ��A�W�����v�ł���Ȃ�
                if (m_bReloading == false && m_bJump == true && m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
                {
                    // �W�����v��
                    pSound->Play(CSound::LABEL_SE_KICK);

                    // �W�����v
                    m_move.y = PLAYER_JUMP_VALUE;

                    // �W�����v�s��
                    m_bJump = false;

                    // �X�e�B�b�N���|��Ă���Ȃ牡�ړ�
                    if (Controller.lY != 0 || Controller.lX != 0)
                    {
                        // �W�����v���̉��ړ�����������
                        m_move.x = sinf(pCamera->GetAngle() + fAngleStick) * fSpeed;
                        m_move.z = cosf(pCamera->GetAngle() + fAngleStick) * fSpeed;
                    }
                }
            }
        }
    }

    // �ǃL�b�N���[�V�����ɂ��邩�ǂ���
    if (m_nCntWallKickTime > 0)
    {
        // �J�E���g�_�E��
        m_nCntWallKickTime--;

        // �ǃL�b�N�̓r���ł���ɕǃL�b�N�������Ƃ��ɁA������x��]�����邽�߂̃t���O
        if (m_bWallKickType == false)
        {
            // ���[�V������ǃL�b�N�ɂ���
            GetAnimation()->SetAnimation(ANIM_WALL_KICK);
        }
        else
        {
            // ���[�V������ǃL�b�N�ɂ���
            GetAnimation()->SetAnimation(ANIM_WALL_KICK2);
        }
    }

    // ���n���[�V�����ɂ��邩�ǂ���
    if (GetPosOld().y - PLAYER_LANDING_POS_OLD_Y_HEIGHT > pos.y && m_bJumpOld == false && m_bJump == true)
    {
        // ���n���Ԃ�ݒ�
        m_nCntLandingTime = PLAYER_LANDING_TIME;

        // ���n���̒n����G�t�F�N�g
        SetLandingCrack(pos);

        //// �����[�h��W����
        //PreventReloading();
    }
    if (m_nCntLandingTime > 0)
    {
        // �J�E���g�_�E��
        m_nCntLandingTime--;

        // ���[�V�����𒅒n�ɂ���
        GetAnimation()->SetAnimation(ANIM_LANDING);
    }

    // �W�����v���[�V�����ɂ��邩�ǂ���
    if (m_nCntWallKickTime <= 0 && m_bJump == false && m_bWallRun == false)
    {
        // ���[�V�������W�����v�ɂ���
        GetAnimation()->SetAnimation(ANIM_JUMP);
    }

    // �U������
    // �Q�[�����t�F�[�h���ȊO�Ȃ�
    if (CGame::GetState() != CGame::STATE_IN_FADE)
    {
        Attack(nCnt, pos, rot);
    }

    // �����[�h���[�V�����ɂ��邩�ǂ���
    if (m_bReloading == true)
    {
        // ���[�V�����𗧂������[�h�ɂ���
        GetAnimation()->SetAnimation(ANIM_RELOAD);
    }

    // ���ꃂ�[�V�����ɂ��邩�ǂ���
    if (m_nCntInvincibleFrame <= PLAYER_INVINCIBLE_FRAME &&
        m_nCntInvincibleFrame >= PLAYER_START_MOVE_IN_INVINCIBLE)
    {
        // ���[�V����������ɂ���
        GetAnimation()->SetAnimation(ANIM_TAKE_DAMAGE);

        // �����[�h��W����
        PreventReloading();
    }

    // �Ǒ��肵�Ă��Ȃ��Ȃ�
    if (m_bWallRun == false)
    {
        // �X���𒼂�
        rot.x -= PLAYER_TILT_SPEED;
    }

    // �X������
    if (rot.x > PLAYER_TILT_LIMIT)
    {
        rot.x = PLAYER_TILT_LIMIT;
    }
    else if (rot.x < 0.0f)
    {
        rot.x = 0.0f;
    }

    // �ʒu�ƌ����𔽉f
    SetPos(pos);
    SetRot(rot);

    // 1F�O�A�W�����v�ł������ǂ������L�^
    m_bJumpOld = m_bJump;

    // �������Đ�
    if (m_nCntFootsteps >= PLAYER_FOOTSTEPS_COOL_TIME)
    {
        m_nCntFootsteps = 0;

        // �����[�h���Ă��Ȃ��Ȃ�Đ�
        if (m_bReloading == false)
        {
            pSound->Play(CSound::LABEL_SE_FOOT);
        }
    }

#ifdef COLLISION_TEST
    CDebug::Create(pos, GetActiveCollisionSize(), CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// �v���C���[�̈ړ�����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Control(int nCnt, float fSpeed)
{
    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(nCnt);
    float fAngleStick = atan2(Controller.lX, Controller.lY*-1);

    // �J�������擾
    CCamera *pCamera = CManager::GetCamera();

    // ����U�����ł��Ȃ��A������Ȃ�
    if (m_leftHandAttack == LEFT_HAND_ATTACK_NONE && m_nCntInvincibleFrame <= PLAYER_START_MOVE_IN_INVINCIBLE)
    {
        // �X�e�B�b�N���|��Ă���Ȃ�ړ�
        if (Controller.lY != 0 || Controller.lX != 0)
        {
            // �X�e�B�b�N�̊p�x�����߂�
            fAngleStick = atan2(Controller.lX, Controller.lY*-1);

            // �W�����v�ł���Ȃ�
            if (m_bJump == true)
            {
                // �����̃J�E���g�A�b�v
                m_nCntFootsteps++;

                // �ړ��ʂɑ��
                m_move.x = sinf(pCamera->GetAngle() + fAngleStick)*fSpeed;
                m_move.z = cosf(pCamera->GetAngle() + fAngleStick)*fSpeed;

                if (STICK_DOWN(fAngleStick))
                {
                    // ���[�V�������������ɂ���
                    GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
                }
                else
                {
                    // ���[�V����������ɂ���
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
            }
            // �W�����v�ł��Ȃ��Ȃ�
            else
            {
                // �Ǒ��蒆�Ȃ�
                if (m_bWallRun == true)
                {
                    // �����̃J�E���g�A�b�v
                    m_nCntFootsteps++;

                    // �󒆐���̂��߁A�ړ��ʂɉ��Z
                    m_move.x += sinf(pCamera->GetAngle() + fAngleStick)*fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + fAngleStick)*fSpeed;

                    // ���[�V����������ɂ���
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
                // �󒆂Ȃ�
                else
                {
                    // �󒆐���̂��߁A�ړ��ʂɉ��Z
                    m_move.x += sinf(pCamera->GetAngle() + fAngleStick)*fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + fAngleStick)*fSpeed;
                }
            }
        }
        // �X�e�B�b�N���|��Ă��Ȃ��Ȃ�A�L�[�{�[�h����\
        else
        {
            // �L�[�{�[�h����
            KeyboardOperation(fSpeed);
        }
    }
}

//=============================================================================
// �L�[�{�[�h����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::KeyboardOperation(float fSpeed)
{
    //�L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �J�������擾
    CCamera *pCamera = CManager::GetCamera();

    // �W�����v�ł���Ȃ�
    if (m_bJump == true)
    {
        // A�L�[�������ꂽ(���ړ�)
        if (pInputKeyboard->GetKeyboardPress(DIK_A))
        {
            // �����̃J�E���g�A�b�v
            m_nCntFootsteps++;

            if (pInputKeyboard->GetKeyboardPress(DIK_W))// A��W�������ɉ����ꂽ
            {
                m_move.x = sinf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;

                // ���[�V����������ɂ���
                GetAnimation()->SetAnimation(ANIM_MOVE);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))// A��S�������ɉ����ꂽ
            {
                m_move.x = sinf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;

                // ���[�V�������������ɂ���
                GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
            }
            else				// ������������
            {
                m_move.x = sinf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;

                // ���[�V����������ɂ���
                GetAnimation()->SetAnimation(ANIM_MOVE);
            }
        }
        // D�L�[�������ꂽ(�E�ړ�)
        else if (pInputKeyboard->GetKeyboardPress(DIK_D))
        {
            // �����̃J�E���g�A�b�v
            m_nCntFootsteps++;

            if (pInputKeyboard->GetKeyboardPress(DIK_W))// D��W�������ɉ����ꂽ
            {
                m_move.x = sinf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;

                // ���[�V����������ɂ���
                GetAnimation()->SetAnimation(ANIM_MOVE);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))// D��S�������ɉ����ꂽ
            {
                m_move.x = sinf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;

                // ���[�V�������������ɂ���
                GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
            }
            else				// �E����������
            {
                m_move.x = sinf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;
                m_move.z = cosf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;

                // ���[�V����������ɂ���
                GetAnimation()->SetAnimation(ANIM_MOVE);
            }
        }
        // W�L�[�������ꂽ(��ړ�)
        else if (pInputKeyboard->GetKeyboardPress(DIK_W))
        {
            // �����̃J�E���g�A�b�v
            m_nCntFootsteps++;

            m_move.x = sinf(pCamera->GetAngle()) * fSpeed;
            m_move.z = cosf(pCamera->GetAngle()) * fSpeed;

            // ���[�V����������ɂ���
            GetAnimation()->SetAnimation(ANIM_MOVE);
        }
        // S�L�[�������ꂽ(���ړ�)
        else if (pInputKeyboard->GetKeyboardPress(DIK_S))
        {
            // �����̃J�E���g�A�b�v
            m_nCntFootsteps++;

            m_move.x = sinf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;
            m_move.z = cosf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;

            // ���[�V�������������ɂ���
            GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
        }
    }
    // �W�����v�ł��Ȃ��Ȃ�
    else
    {
        // �Ǒ��蒆�Ȃ�
        if (m_bWallRun == true)
        {
            // �����̃J�E���g�A�b�v
            m_nCntFootsteps++;

            // A�L�[�������ꂽ(���ړ�)
            if (pInputKeyboard->GetKeyboardPress(DIK_A))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))// A��W�������ɉ����ꂽ
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;

                    // ���[�V����������ɂ���
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))// A��S�������ɉ����ꂽ
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;

                    // ���[�V�������������ɂ���
                    GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
                }
                else				// ������������
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;

                    // ���[�V����������ɂ���
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
            }
            // D�L�[�������ꂽ(�E�ړ�)
            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))// D��W�������ɉ����ꂽ
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;

                    // ���[�V����������ɂ���
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))// D��S�������ɉ����ꂽ
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;

                    // ���[�V�������������ɂ���
                    GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
                }
                else				// �E����������
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;

                    // ���[�V����������ɂ���
                    GetAnimation()->SetAnimation(ANIM_MOVE);
                }
            }
            // W�L�[�������ꂽ(��ړ�)
            else if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                m_move.x += sinf(pCamera->GetAngle()) * fSpeed;
                m_move.z += cosf(pCamera->GetAngle()) * fSpeed;

                // ���[�V����������ɂ���
                GetAnimation()->SetAnimation(ANIM_MOVE);
            }
            // S�L�[�������ꂽ(���ړ�)
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;
                m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;

                // ���[�V�������������ɂ���
                GetAnimation()->SetAnimation(ANIM_BACK_MOVE);
            }
        }
        // �󒆂Ȃ�
        else
        {
            // A�L�[�������ꂽ(���ړ�)
            if (pInputKeyboard->GetKeyboardPress(DIK_A))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))// A��W�������ɉ����ꂽ
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(45.0f)) * fSpeed;
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))// A��S�������ɉ����ꂽ
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(135.0f)) * fSpeed;
                }
                else				// ������������
                {
                    m_move.x += sinf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() - D3DXToRadian(90.0f)) * fSpeed;
                }
            }
            // D�L�[�������ꂽ(�E�ړ�)
            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))// D��W�������ɉ����ꂽ
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(45.0f)) * fSpeed;
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))// D��S�������ɉ����ꂽ
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(135.0f)) * fSpeed;
                }
                else				// �E����������
                {
                    m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;
                    m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(90.0f)) * fSpeed;
                }
            }
            // W�L�[�������ꂽ(��ړ�)
            else if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                m_move.x += sinf(pCamera->GetAngle()) * fSpeed;
                m_move.z += cosf(pCamera->GetAngle()) * fSpeed;
            }
            // S�L�[�������ꂽ(���ړ�)
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                m_move.x += sinf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;
                m_move.z += cosf(pCamera->GetAngle() + D3DXToRadian(180.0f)) * fSpeed;
            }
        }
    }
}

//=============================================================================
// �U������̏���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Attack(int nCnt, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �����邩�A�����[�h���łȂ��Ȃ�
    if (m_nCntInvincibleFrame <= PLAYER_START_MOVE_IN_INVINCIBLE  && m_bReloading == false)
    {
        // �K�E���łȂ��Ȃ�
        if (m_leftHandAttack == LEFT_HAND_ATTACK_NONE)
        {
            // �R���g���[�����擾
            CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
            DIJOYSTATE2 Controller = pInputJoypad->GetController(nCnt);

            // �J�������擾
            CCamera *pCamera = CManager::GetCamera();

            //�L�[�{�[�h�̊m�ۂ������������擾
            CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

            // �}�E�X���擾
            CMouse *pMouse = CManager::GetMouse();

            // �T�E���h���擾
            CSound *pSound = CManager::GetSound();

            // �����[�h�{�^������������
            if (pInputJoypad->GetJoypadTrigger(nCnt, CInputJoypad::BUTTON_X) || pInputKeyboard->GetKeyboardTrigger(DIK_R))
            {
                // �c�e���ő吔�����Ȃ�
                if (m_nRemainBullets < PLAYER_REMAIN_BULLETS_MAX)
                {
                    // SE���Đ�
                    pSound->Play(CSound::LABEL_SE_MACHINE_GUN_RELOAD);

                    // �����[�h����
                    m_bReloading = true;

                    // �N�[���^�C����ݒ�
                    m_nAttackCoolTime = PLAYER_RELOAD_COOL_TIME;
                }
            }

            // �U���{�^������������
            if (pInputJoypad->GetJoypadPress(nCnt, CInputJoypad::BUTTON_R2) || pMouse->GetMousePressLeft())
            {
                // �c�e��������Ȃ�
                if (m_nRemainBullets > 0)
                {
                    // �Ǔo�蒆�łȂ��Ȃ�
                    if (m_bWallRun == false)
                    {
                        // �X�e�B�b�N���|��Ă���Ȃ�ړ��ˌ�
                        if (Controller.lY != 0 || Controller.lX != 0)
                        {
                            // �X�e�B�b�N�̊p�x�����߂�
                            float fAngleStick = atan2(Controller.lX, Controller.lY*-1);

                            if (STICK_UP(fAngleStick))
                            {
                                // ���[�V������O�����ˌ��ɂ���
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_AHEAD);
                            }
                            else if (STICK_LEFT(fAngleStick))
                            {
                                // ���[�V�������������ˌ��ɂ���
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_LEFT);
                            }
                            else if (STICK_RIGHT(fAngleStick))
                            {
                                // ���[�V�������E�����ˌ��ɂ���
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_RIGHT);
                            }
                            else if (STICK_DOWN(fAngleStick))
                            {
                                // ���[�V�������������ˌ��ɂ���
                                GetAnimation()->SetAnimation(ANIM_BACK_MOVE_ATTACK);
                            }
                        }
                        else
                        {
                            // A�L�[�������ꂽ(���ړ�)
                            if (pInputKeyboard->GetKeyboardPress(DIK_A))
                            {
                                // ���[�V�������������ˌ��ɂ���
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_LEFT);
                            }
                            // D�L�[�������ꂽ(�E�ړ�)
                            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
                            {
                                // ���[�V�������E�����ˌ��ɂ���
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_RIGHT);
                            }
                            // W�L�[�������ꂽ(��ړ�)
                            else if (pInputKeyboard->GetKeyboardPress(DIK_W))
                            {
                                // ���[�V������O�����ˌ��ɂ���
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTACK_WALK_AHEAD);
                            }
                            // S�L�[�������ꂽ(���ړ�)
                            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
                            {
                                // ���[�V�������������ˌ��ɂ���
                                GetAnimation()->SetAnimation(ANIM_BACK_MOVE_ATTACK);
                            }
                            else
                            {
                                // ���[�V�������~�ˌ��ɂ���
                                GetAnimation()->SetAnimation(ANIM_RIGHT_ATTCK_STOP);
                            }
                        }

                        // �N�[���^�C�����Ȃ��Ȃ�
                        if (m_nAttackCoolTime <= 0)
                        {
                            // �ړ��̊p�x�����߂�
                            D3DXVECTOR3 moveAngle = DEFAULT_VECTOR;
                            if (pos.y > 0.0f)
                            {
                                // �󒆂ł̏c�p�x����
                                moveAngle = D3DXVECTOR3(sinf(pCamera->GetShotAngle(true)), cosf(pCamera->GetShotThetaInAir(true)),
                                    cosf(pCamera->GetShotAngle(true)));
                            }
                            else
                            {
                                // �n��ł̏c�p�x����
                                moveAngle = D3DXVECTOR3(sinf(pCamera->GetShotAngle(true)), cosf(pCamera->GetShotTheta(true)),
                                    cosf(pCamera->GetShotAngle(true)));
                            }

                            // �A�T���g���C�t���̒e
                            CBullet::Create(pos + PLAYER_SHOT_POS_ADJUSTMENT, BULLET_AR_COLLISION_SIZE, moveAngle, CBullet::TYPE_AR, CBullet::TRIBE_PLAYER);

                            // �c�e��������炷
                            m_nRemainBullets--;

                            // �c�e���\�����X�V
                            CGame::GetBulletRemain()->Disp(m_nRemainBullets);

                            // �N�[���^�C����ݒ�
                            m_nAttackCoolTime = BULLET_AR_COOL_TIME;

                            // �ˌ����̔�U�G�t�F�N�g����
                            SetScatteringBullet();
                        }

                        // �U���́A�ǃL�b�N�̃J�E���^�����Z�b�g����
                        m_nCntWallKickTime = 0;
                    }
                }
                // �c�e�����Ȃ��Ȃ�A���������[�h
                else
                {
                    // SE���Đ�
                    pSound->Play(CSound::LABEL_SE_MACHINE_GUN_RELOAD);

                    // �����[�h����
                    m_bReloading = true;

                    // �N�[���^�C����ݒ�
                    m_nAttackCoolTime = PLAYER_RELOAD_COOL_TIME;
                }
            }

            // �K�E�{�^������������
            if (pInputJoypad->GetJoypadPress(nCnt, CInputJoypad::BUTTON_L1) && pInputJoypad->GetJoypadPress(nCnt, CInputJoypad::BUTTON_R1)
                || pInputKeyboard->GetKeyboardTrigger(DIK_Z))
            {
                // �Ǔo�蒆�łȂ��A�I�[�u����lv1�ȏ�Ȃ�
                if (m_bWallRun == false && m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_LV1)
                {
                    // ���K�E
                    if (m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_MAX)
                    {
                        m_nAttackCoolTime = PLAYER_LEFT_HAND_ATTACK_STRONG_TIME;
                        m_leftHandAttack = LEFT_HAND_ATTACK_STRONG;
                    }
                    // ��K�E
                    else
                    {
                        m_nAttackCoolTime = PLAYER_LEFT_HAND_ATTACK_WEAK_TIME;
                        m_leftHandAttack = LEFT_HAND_ATTACK_WEAK;
                    }
                }
            }
        }
        // ����U����(��)�Ȃ�
        else if (m_leftHandAttack == LEFT_HAND_ATTACK_WEAK)
        {
            // ���[�V����������U����ɂ���
            GetAnimation()->SetAnimation(ANIM_LEFT_ATTACK_WEAK);

            // �U���𐶐�����t���[��
            if (m_nAttackCoolTime == PLAYER_LEFT_HAND_ATTACK_WEAK_CREATE_FRAME)
            {
                SetLeftHandAttack(pos, rot);
            }
        }
        // ����U����(��)�Ȃ�
        else if (m_leftHandAttack == LEFT_HAND_ATTACK_STRONG)
        {
            // ���[�V����������U�����ɂ���
            GetAnimation()->SetAnimation(ANIM_LEFT_ATTACK_STRONG);

            // �U���𐶐�����t���[��
            if (m_nAttackCoolTime == PLAYER_LEFT_HAND_ATTACK_STRONG_CREATE_FRAME)
            {
                SetLeftHandAttack(pos, rot);
            }
        }
    }
}

//=============================================================================
// ����U���𐶐����鏈��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SetLeftHandAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �J�������擾
    CCamera *pCamera = CManager::GetCamera();

    // �ړ��̊p�x�����߂�
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR;
    if (pos.y > 0.0f)
    {
        // �󒆂ł̏c�p�x����
        moveAngle = D3DXVECTOR3(sinf(pCamera->GetShotAngle(false)), cosf(pCamera->GetShotThetaInAir(false)),
            cosf(pCamera->GetShotAngle(false)));
    }
    else
    {
        // �n��ł̏c�p�x����
        moveAngle = D3DXVECTOR3(sinf(pCamera->GetShotAngle(false)), cosf(pCamera->GetShotTheta(false)),
            cosf(pCamera->GetShotAngle(false)));
    }

    // ���x�����Ƃ̑傫���ŁA���e�𐶐�
    D3DXVECTOR3 size = DEFAULT_VECTOR;
    if (m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_LV1 && m_nLeftHandOrbs < PLAYER_LEFT_HAND_ORBS_LV2)
    {
        size = D3DXVECTOR3(BULLET_BOMB_COLLISION_SIZE_LV1, BULLET_BOMB_COLLISION_SIZE_LV1, BULLET_BOMB_COLLISION_SIZE_LV1);
        CBullet::Create(pos + PLAYER_SHOT_POS_ADJUSTMENT, size, moveAngle, CBullet::TYPE_BOMB, CBullet::TRIBE_PLAYER);

        // ��m�b�N�o�b�N
        m_move.x = sinf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
        m_move.z = cosf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
    }
    else if (m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_LV2 && m_nLeftHandOrbs < PLAYER_LEFT_HAND_ORBS_LV3)
    {
        size = D3DXVECTOR3(BULLET_BOMB_COLLISION_SIZE_LV2, BULLET_BOMB_COLLISION_SIZE_LV2, BULLET_BOMB_COLLISION_SIZE_LV2);
        CBullet::Create(pos + PLAYER_SHOT_POS_ADJUSTMENT, size, moveAngle, CBullet::TYPE_BOMB, CBullet::TRIBE_PLAYER);

        // ��m�b�N�o�b�N
        m_move.x = sinf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
        m_move.z = cosf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
    }
    else if (m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_LV3 && m_nLeftHandOrbs < PLAYER_LEFT_HAND_ORBS_MAX)
    {
        size = D3DXVECTOR3(BULLET_BOMB_COLLISION_SIZE_LV3, BULLET_BOMB_COLLISION_SIZE_LV3, BULLET_BOMB_COLLISION_SIZE_LV3);
        CBullet::Create(pos + PLAYER_SHOT_POS_ADJUSTMENT, size, moveAngle, CBullet::TYPE_BOMB, CBullet::TRIBE_PLAYER);

        // ��m�b�N�o�b�N
        m_move.x = sinf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
        m_move.z = cosf(rot.y) * PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE;
    }
    else if (m_nLeftHandOrbs >= PLAYER_LEFT_HAND_ORBS_MAX)
    {
        size = D3DXVECTOR3(BULLET_BOMB_COLLISION_SIZE_LV4, BULLET_BOMB_COLLISION_SIZE_LV4, BULLET_BOMB_COLLISION_SIZE_LV4);
        CBullet::Create(pos + PLAYER_SHOT_POS_ADJUSTMENT, size, moveAngle, CBullet::TYPE_BOMB, CBullet::TRIBE_PLAYER);

        // ���m�b�N�o�b�N
        m_move.x = sinf(rot.y) * PLAYER_LEFT_HAND_ATTACK_STRONG_KNOCKBACK_VALUE;
        m_move.z = cosf(rot.y) * PLAYER_LEFT_HAND_ATTACK_STRONG_KNOCKBACK_VALUE;
    }

    // ����̃I�[�u����0��
    m_nLeftHandOrbs = 0;
}

//=============================================================================
// �_���[�W���󂯂��Ƃ��̏���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::TakeDamage(int nDamage, D3DXVECTOR3 posOld)
{
    // �Q�[���̃X�e�[�g���ʏ�Ȃ�
    if (CGame::GetState() == CGame::STATE_NORMAL)
    {
        // ���G���Ԃ��Ȃ��Ȃ�
        if (m_nCntInvincibleFrame <= 0)
        {
            // �_���[�W���󂯂����̐�
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_DAMAGE_PLAYER);

            // �_���[�W���󂯂�
            SetTakeDamage(nDamage, LAST_HIT_ENEMY);

            // �ʒu���擾
            D3DXVECTOR3 pos = GetPos();

            // �m�b�N�o�b�N�ʂ����߂�
            D3DXVECTOR3 move = DEFAULT_VECTOR;
            move.x = pos.x - posOld.x;
            move.z = pos.z - posOld.z;
            D3DXVec3Normalize(&move, &move);  //�x�N�g���𐳋K��

                                              // �m�b�N�o�b�N
            m_move.x = move.x * PLAYER_KNOCKBACK_VALUE;
            m_move.z = move.z * PLAYER_KNOCKBACK_VALUE;

            // ���G���Ԃ̐ݒ�
            m_nCntInvincibleFrame = PLAYER_INVINCIBLE_FRAME;

            // �N�[���^�C�������Z�b�g
            m_nAttackCoolTime = 0;

            // ����U�����L�����Z��
            m_leftHandAttack = LEFT_HAND_ATTACK_NONE;
        }
    }
}

//=============================================================================
// �񕜏���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Healing(int nHeal)
{
    // ���݂�HP���擾
    int nHP = GetLife();

    // �ݒ肷��񕜗�
    int nAnswer = nHeal;

    // �ő�l�𒴂�����
    if (nHP + nHeal > PLAYER_MAX_LIFE)
    {
        // �񕜗ʂ𐧌�
        nAnswer = PLAYER_MAX_LIFE - nHP;
    }

    // ��
    SetHealing(nAnswer);

    for (int nCnt = 0; nCnt < EFFECT_HEALING_NUMBER_ONCE; nCnt++)
    {
        // �G�t�F�N�g�����p�̕ϐ��錾
        // �傫�����o��
        float fSize = float(GetRandNum(EFFECT_HEALING_MAX_SIZE, EFFECT_HEALING_MIN_SIZE)) / 100.0f;

        // �傫�������߂�
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // �p�x���o��
        float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // �ʒu�����߂�
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle) * GetActiveCollisionSize().x, fSize / 2, -cosf(fAngle) * GetActiveCollisionSize().x) + GetPos();

        // �������o��
        float fSpeed = float(GetRandNum(EFFECT_HEALING_MAX_SPEED, EFFECT_HEALING_MIN_SPEED)) / 100.0f;
        float fSpeedY = float(GetRandNum(EFFECT_HEALING_MAX_SPEED_Y, EFFECT_HEALING_MIN_SPEED_Y)) / 100.0f;

        // �ړ��ʂ����߂�
        float fMoveAngle = atan2((GetPos().x - effectPos.x), (GetPos().z - effectPos.z));
        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fMoveAngle) * fSpeed, fSpeedY, -cosf(fMoveAngle)* fSpeed);

        // �G�t�F�N�g����
        CEffect3D::Create(effectPos, size, move, EFFECT_HEALING_COLOR,
            DEFAULT_VECTOR, EFFECT_HEALING_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
    }

}

//=============================================================================
// �����̐���
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CPlayer::RotControl(D3DXVECTOR3 rot)
{
    // �Ԃ���]��
    D3DXVECTOR3 returnRot = rot;

    // �J�������擾
    CCamera *pCamera = CManager::GetCamera();

    // �J�����̌����Ă���ق�������
    m_rotDest.y = pCamera->GetPlayerRot();

    // ��]�̐���
    if (returnRot.y > D3DX_PI)
    {
        returnRot.y -= D3DX_PI * 2;
    }
    if (returnRot.y < -D3DX_PI)
    {
        returnRot.y += D3DX_PI * 2;
    }

    m_rotMin.y = m_rotDest.y - returnRot.y;

    // �ړI�̊p�x�̉�]�𐧌�
    if (m_rotMin.y > D3DX_PI)
    {
        m_rotDest.y -= D3DX_PI * 2;
    }
    if (m_rotMin.y < -D3DX_PI)
    {
        m_rotDest.y += D3DX_PI * 2;
    }

    // ��]�̐���
    if (returnRot.y > D3DX_PI)
    {
        returnRot.y -= D3DX_PI * 2;
    }
    if (returnRot.y < -D3DX_PI)
    {
        returnRot.y += D3DX_PI * 2;
    }

    if (fabsf(m_rotMin.y) < 0)
    {
        m_rotMin.y = m_rotDest.y;
    }
    else
    {
        returnRot.y += (m_rotDest.y - returnRot.y) * PLAYER_TURN_SPEED;
    }

    return returnRot;
}

//=============================================================================
// �����[�h��W���鏈��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::PreventReloading(void)
{
    // �����[�h���Ȃ�
    if (m_bReloading == true)
    {
        // �T�E���h���擾
        CSound *pSound = CManager::GetSound();

        // SE���~�߂�
        pSound->Stop(CSound::LABEL_SE_MACHINE_GUN_RELOAD);

        // �����[�h���ł͂Ȃ���
        m_bReloading = false;

        // �N�[���^�C�������Z�b�g
        m_nAttackCoolTime = 0;
    }
}

//=============================================================================
// �I�[�u���㏸����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SetRisingLeftHandOrbs(void)
{
    // �ő吔�ȉ��Ȃ�
    if (m_nLeftHandOrbs < PLAYER_LEFT_HAND_ORBS_MAX)
    {
        // �㏸
        m_nLeftHandOrbs++;
    }
}

//=============================================================================
// ���ꂽ���̕��ӏ���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �p�[�c�̈ʒu�ƁA���f���G�t�F�N�g�̎��
    D3DXVECTOR3 partsPos = DEFAULT_VECTOR;
    CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_X_BODY;

    //==============================
    // ��
    //==============================
    // �ʒu
    partsPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._41,
        GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._42,
        GetParts(CPlayer::PARTS_BODY)->GetWorldMtx()._43);

    // �G�t�F�N�g�̔���
    modelEffectType = CModelEffect::TYPE_X_BODY;
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // ��
    //==============================
    // �ʒu
    partsPos = GetPos();

    // �G�t�F�N�g�̔���
    modelEffectType = CModelEffect::TYPE_X_FOOT;
    SetDeathCrushEffect(partsPos, rot, modelEffectType);
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // ����
    //==============================
    // �ʒu
    partsPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._41,
        GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._42,
        GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._43);

    // �G�t�F�N�g�̔���
    modelEffectType = CModelEffect::TYPE_X_LHAND;
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // �E��
    //==============================
    // �ʒu
    partsPos = D3DXVECTOR3(GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._41,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._42,
        GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._43);

    // �G�t�F�N�g�̔���
    modelEffectType = CModelEffect::TYPE_X_RHAND;
    SetDeathCrushEffect(partsPos, rot, modelEffectType);
}

//=============================================================================
// ���ꂽ���̕��ӃG�t�F�N�g�̔���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type)
{
    // ���ӃG�t�F�N�g�p�̕ϐ��錾
    float fSpeedX = 0.0f;               // ����X
    float fSpeedY = 0.0f;               // ����Y
    float fSpeedZ = 0.0f;               // ����Z
    float fRotSpeedX = 0.0f;            // ��]���xX
    float fRotSpeedY = 0.0f;            // ��]���xY
    float fRotSpeedZ = 0.0f;            // ��]���xZ
    int nPlusMinusX = 0;                // ����X
    int nPlusMinusZ = 0;                // ����Z

    // �����ƁA�����������_���Ɍ��߂�
    fSpeedX = 0.0f;
    fSpeedY = float(GetRandNum(MODEL_EFFECT_X_SPLATTER_FORCE_MAX, MODEL_EFFECT_X_SPLATTER_FORCE_MIN)) / 100.0f;
    fSpeedZ = 0.0f;
    fRotSpeedX = float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f;
    fRotSpeedY = float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f;
    fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_ROT_MAX, MODEL_EFFECT_ROT_MIN)) / 100.0f;
    nPlusMinusX = GetRandNum(2, 1);
    nPlusMinusZ = GetRandNum(2, 1);
    if (nPlusMinusX == 1)
    {
        nPlusMinusX = 1;
    }
    else
    {
        nPlusMinusX = -1;
    }

    if (nPlusMinusZ == 1)
    {
        nPlusMinusZ = 1;
    }
    else
    {
        nPlusMinusZ = -1;
    }

    // �ړ��ʂ����߂�
    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY, fSpeedZ*nPlusMinusZ);

    // ��]�ʂ����߂�
    D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

    // �G�t�F�N�g����
    CModelEffect::Create(pos, rot, move, rotMove, type);
}
