//================================================================================
//
// UFO���� (ufo.cpp)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//================================================================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "ufo.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "animation.h"
#include "block.h"
#include "effect3d.h"
#include "bullet.h"
#include "library.h"
#include "debug.h"
#include "score.h"
#include "game.h"
#include "modelEffect.h"
#include "player.h"
#include "bullet.h"
#include "item.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
LPD3DXMESH CUfo::m_pMesh[PARTS_MAX] = {};
LPD3DXBUFFER CUfo::m_pBuffMat[PARTS_MAX] = {};
DWORD CUfo::m_nNumMat[PARTS_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CUfo::CUfo() :CCharacter(OBJTYPE::OBJTYPE_ENEMY)
{
    m_move = DEFAULT_VECTOR;
    m_nScore = 0;
    m_nOrbs = 0;

    m_patrolCentralPos = DEFAULT_VECTOR;
    m_patrolDestPos = DEFAULT_VECTOR;
    m_nPatrolDistance = 0;
    m_nCntTime = 0;

    m_fSpeed = 0.0f;
    m_rotDest = DEFAULT_VECTOR;
    m_rotMin = DEFAULT_VECTOR;

    m_bWarning = false;
    m_fDiscoveryDistance = 0.0f;

    m_attackAI = ATTACK_AI_WAIT;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CUfo::~CUfo()
{
}

//=============================================================================
// ����������
// Author : ��{��
//=============================================================================
HRESULT CUfo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �p�[�c����ݒ�
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_UFO, nCount, m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
    }

    // ���W�E�e�q�֌W�̓ǂݍ���
    LoadModelData("./data/ANIMATION/motion_UFO.txt");

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CUfo::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CUfo::Update(void)
{
    // �s��
    Movement();

    // �A�j���[�V�����̍X�V
    CCharacter::Update();

    // �̗͂�0�ȉ��Ȃ�A�X�R�A���Z���A���g�p��
    if (GetLife() <= 0)
    {
        // �ʒu�ƌ������擾
        D3DXVECTOR3 pos = DEFAULT_VECTOR;
        D3DXVECTOR3 rot = DEFAULT_VECTOR;
        pos = GetPos();
        rot = GetRot();

        // ����
        DeathCrush(pos, rot);

        // �~�b�V�����N���A���ɉ��Z����
        AddMissionClear();

        // ���X�g�q�b�g���v���C���[�Ȃ�
        if (GetLastHit() == LAST_HIT_PLAYER)
        {
            // �I�[�u���o���܂�
            ScatterOrbs();

            // �X�R�A���Z
            CScore *pScore = CGame::GetScore();
            pScore->AddScore(m_nScore);

            // ���m���ŁA�񕜃A�C�e���𗎂Ƃ�
            int nRandNum = GetRandNum(100, 1);
            if (nRandNum <= UFO_DROP_HEAL_ITEM)
            {
                CItem::Create(pos, DEFAULT_VECTOR, CItem::TYPE_HEAL);
            }
        }

        // �I������
        Uninit();
    }
}

//=============================================================================
// �s������
// Author : �㓡�T�V��
//=============================================================================
void CUfo::Movement(void)
{
    // �ʒu�ƌ������擾
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    D3DXVECTOR3 rot = DEFAULT_VECTOR;
    pos = GetPos();
    rot = GetRot();

    // �x�����Ă��Ȃ��Ȃ�
    if (m_bWarning == false)
    {
        // ���[�V������ҋ@�ɂ���
        GetAnimation()->SetAnimation(ANIM_IDLE);

        // ��]����
        rot = RotControl(rot);
    }
    // �x�����Ă���Ȃ�
    else
    {
        // ���[�V�������x���ɂ���
        GetAnimation()->SetAnimation(ANIM_WARN);
    }

    // 1F�O�̈ʒu���L��
    SetPosOld(pos);

    // ����AI����
    pos = PatrolAI(pos);

    // �x��AI����
    rot = WarningAI(pos, rot);

    // �U��AI����
    if (CGame::GetState() == CGame::STATE_NORMAL)
    {
        pos = AttackAI(pos, rot);
    }

    // �ړ��ʂƈʒu�����т���
    pos += m_move;

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

    // �ʒu�ƌ����𔽉f
    SetPos(pos);
    SetRot(rot);

    // �����蔻��̈ʒu���X�V
    UpdateCollisionPos();

#ifdef COLLISION_TEST
    for (int nCnt = 0; nCnt < GetCntCollision(); nCnt++)
    {
        COLLISION collision = GetCollision(nCnt);
        CDebug::Create(collision.pos, collision.size, CDebug::TYPE_MOMENT);
    }
#endif // COLLISION_TEST
}

//=============================================================================
// �����蔻��̈ʒu���X�V
// Author : �㓡�T�V��
//=============================================================================
void CUfo::UpdateCollisionPos(void)
{
    SetCollisionPos(0, GetPos() + UFO_UP_COLLISION_POS);
}

//=============================================================================
// ��]����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CUfo::RotControl(D3DXVECTOR3 rot)
{
    // �Ԃ���]��
    D3DXVECTOR3 returnRot = rot;

    // 1F�O�ƍ��W���Ⴄ�Ȃ�
    if (GetPosOld() != GetPos())
    {
        // 1F�O�̈ʒu�ƁA���݂̈ʒu���r���A�p�x�����߂�
        float fAngle = atan2((GetPosOld().x - GetPos().x), (GetPosOld().z - GetPos().z));

        // ���߂��p�x�������悤�ɂ���
        m_rotDest.y = fAngle;

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
            returnRot.y += (m_rotDest.y - returnRot.y) * UFO_TURN_SPEED;
        }
    }

    return returnRot;
}

//=============================================================================
// ����AI����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CUfo::PatrolAI(D3DXVECTOR3 pos)
{
    // �Ԃ��ʒu�����߂�
    D3DXVECTOR3 returnPos = pos;

    // �x�����łȂ��Ȃ�
    if (m_bWarning == false)
    {
        // �ړI�̈ʒu�܂ł̋��������߂�
        float fDistance = sqrtf(
            powf((m_patrolDestPos.x - pos.x), 2) +
            powf((m_patrolDestPos.z - pos.z), 2));

        // �������ړI�̈ʒu�ɋ߂��Ȃ�
        if (fDistance <= UFO_CONSIDER_CLOSE_DEST)
        {
            // �J�E���^�����Z
            m_nCntTime++;

            // ���J�E���^�ŁA���̖ړI�n�����߂�
            if (m_nCntTime >= UFO_CHANGE_MOVE_IN_PATROL)
            {
                // �J�E���^�����Z�b�g
                m_nCntTime = 0;

                // ���̖ړI�n�����߂�
                float fPosX = float(rand() % m_nPatrolDistance) - float(rand() % m_nPatrolDistance) + m_patrolCentralPos.x;
                float fPosZ = float(rand() % m_nPatrolDistance) - float(rand() % m_nPatrolDistance) + m_patrolCentralPos.z;

                // �}�b�v����
                CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
                if (fPosX - GetActiveCollisionSize().x < mapLimit.fWest)
                {
                    fPosX = mapLimit.fWest + GetActiveCollisionSize().x;
                }
                if (fPosX + GetActiveCollisionSize().x > mapLimit.fEast)
                {
                    fPosX = mapLimit.fEast - GetActiveCollisionSize().x;
                }
                if (fPosZ - GetActiveCollisionSize().z < mapLimit.fSouth)
                {
                    fPosZ = mapLimit.fSouth + GetActiveCollisionSize().z;
                }
                if (fPosZ + GetActiveCollisionSize().z > mapLimit.fNorth)
                {
                    fPosZ = mapLimit.fNorth - GetActiveCollisionSize().z;
                }

                // ���̖ړI�n���A�����o�ϐ��Ɍ��т���
                m_patrolDestPos = D3DXVECTOR3(fPosX, 0.0f, fPosZ);
            }
        }
        // �������ړI�̈ʒu��艓���Ȃ�
        else
        {
            // �J�E���^�����Z�b�g
            m_nCntTime = 0;

            // ���݂̈ʒu�ƁA�ړI�n�܂ł̊p�x�����߂�
            float fAngle = atan2((GetPos().x - m_patrolDestPos.x), (GetPos().z - m_patrolDestPos.z));

            // �ړ��ʂ����߂�
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle)*m_fSpeed, 0.0f, -cosf(fAngle)*m_fSpeed);

            // �ʒu�Ɉړ��ʂ����т���
            returnPos += move;
        }
    }

    return returnPos;
}

//=============================================================================
// �x��AI����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CUfo::WarningAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �Ԃ�����
    D3DXVECTOR3 returnRot = rot;

    // �v���C���[�̈ʒu���擾
    D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

    // �x�����Ă��Ȃ��Ȃ�
    if (m_bWarning == false)
    {
        // �v���C���[�Ƃ̋����𑪂�
        float fDistance = sqrtf(
            powf((playerPos.x - pos.x), 2) +
            powf((playerPos.z - pos.z), 2));

        // �v���C���[���������ɒB������
        if (fDistance <= m_fDiscoveryDistance)
        {
            // �x������(���Ɍx�����ɂȂ�p�^�[���́A�U�������炤�A�߂��̒��Ԃ��U�������炤������)
            m_bWarning = true;
        }
    }
    // �x�����Ȃ�
    else
    {
        // �v���C���[�̈ʒu�ƁA���g�̈ʒu���r���A�p�x�����߂�
        float fAngle = atan2((GetPos().x - playerPos.x), (GetPos().z - playerPos.z));

        // �v���C���[�Ƃ̋����𑪂�
        float fDistance = sqrtf(
            powf((playerPos.x - pos.x), 2) +
            powf((playerPos.z - pos.z), 2));

        // �v���C���[�Ƃ̍����̍��𑪂�
        float fHeight = fabsf((playerPos.y) - pos.y);

        // �c�̊p�x�����߂�
        float fAngleY = atan2(fDistance, fHeight);

        // ���߂��p�x�������悤�ɂ���
        m_rotDest.x = cosf(fAngleY);
        m_rotDest.y = fAngle;

        // �W�I�̈ʒu�̂ق����A����̈ʒu���Ⴂ�Ȃ�
        if (playerPos.y < pos.y)
        {
            // �������t�ɂ���
            m_rotDest.x *= -1;
        }

        // ��]�̐���
        if (returnRot.y > D3DX_PI)
        {
            returnRot.y -= D3DX_PI * 2;
        }
        else if (returnRot.y < -D3DX_PI)
        {
            returnRot.y += D3DX_PI * 2;
        }
        if (returnRot.x > D3DX_PI)
        {
            returnRot.x -= D3DX_PI * 2;
        }
        else if (returnRot.x < -D3DX_PI)
        {
            returnRot.x += D3DX_PI * 2;
        }

        m_rotMin.x = m_rotDest.x - returnRot.x;
        m_rotMin.y = m_rotDest.y - returnRot.y;

        // �ړI�̊p�x�̉�]�𐧌�
        if (m_rotMin.y > D3DX_PI)
        {
            m_rotDest.y -= D3DX_PI * 2;
        }
        else if (m_rotMin.y < -D3DX_PI)
        {
            m_rotDest.y += D3DX_PI * 2;
        }
        if (m_rotMin.x > D3DX_PI)
        {
            m_rotDest.x -= D3DX_PI * 2;
        }
        else if (m_rotMin.x < -D3DX_PI)
        {
            m_rotDest.x += D3DX_PI * 2;
        }

        // ��]�̐���
        if (returnRot.y > D3DX_PI)
        {
            returnRot.y -= D3DX_PI * 2;
        }
        else if (returnRot.y < -D3DX_PI)
        {
            returnRot.y += D3DX_PI * 2;
        }
        if (returnRot.x > D3DX_PI)
        {
            returnRot.x -= D3DX_PI * 2;
        }
        else if (returnRot.x < -D3DX_PI)
        {
            returnRot.x += D3DX_PI * 2;
        }

        if (fabsf(m_rotMin.y) < 0)
        {
            m_rotMin.y = m_rotDest.y;
        }
        else
        {
            returnRot.y += (m_rotDest.y - returnRot.y) * UFO_TURN_SPEED;
        }

        if (fabsf(m_rotMin.x) < 0)
        {
            m_rotMin.x = m_rotDest.x;
        }
        else
        {
            returnRot.x += (m_rotDest.x - returnRot.x) * UFO_TURN_SPEED;
        }

        // �������𒴂�����
        if (fDistance >= UFO_CONSIDER_AS_LONG)
        {
            // �x����������
            m_bWarning = false;
        }
    }

    return returnRot;
}

//=============================================================================
// �U��AI����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CUfo::AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �Ԃ��ʒu�A�ړ��ʂ����߂�
    D3DXVECTOR3 returnPos = pos;
    D3DXVECTOR3 move = DEFAULT_VECTOR;

    // �v���C���[�܂ł̋���
    D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
    float fDistance = 0.0f;

    // �x�����Ȃ�
    if (m_bWarning == true)
    {
        switch (m_attackAI)
        {
        case ATTACK_AI_WAIT:

            // �J�E���^�A�b�v
            m_nCntTime++;

            // �v���C���[�܂ł̋��������߂�
            fDistance = sqrtf(
                powf((playerPos.x - pos.x), 2) +
                powf((playerPos.z - pos.z), 2));

            // �����������Ȃ�
            if (fDistance > UFO_CONSIDER_CLOSE_DEST)
            {
                // �ǐ�
                returnPos += D3DXVECTOR3(-sinf(rot.y) * UFO_TRACKING_SPEED, 0.0f, -cosf(rot.y) * UFO_TRACKING_SPEED);

                // ����̒��S�ʒu��ς���
                m_patrolCentralPos = returnPos;
            }

            // ���J�E���^��
            if (m_nCntTime >= UFO_NEXT_ATTACK_COUNT)
            {
                // �J�E���^���Z�b�g
                m_nCntTime = 0;

                // �ˌ��U����
                m_attackAI = ATTACK_AI_SHOT;
            }

            break;

        case ATTACK_AI_SHOT:

            // �J�E���^�A�b�v
            m_nCntTime++;

            // �N���G�C�g�t���[��
            if (m_nCntTime == UFO_SHOT_CREATE_FRAME_1)
            {
                SetShotAttack(pos, rot);
            }
            else if (m_nCntTime == UFO_SHOT_CREATE_FRAME_2)
            {
                SetShotAttack(pos, rot);
            }
            else if (m_nCntTime == UFO_SHOT_CREATE_FRAME_3)
            {
                SetShotAttack(pos, rot);
            }

            // ���J�E���^��
            if (m_nCntTime >= UFO_SHOT_WHOLE_FRAME)
            {
                // �J�E���^���Z�b�g
                m_nCntTime = 0;

                // �ҋ@���ɂ���
                m_attackAI = ATTACK_AI_WAIT;
            }

            break;
        }
    }

    return returnPos;
}

//=============================================================================
// �ˌ��U��
// Author : �㓡�T�V��
//=============================================================================
void CUfo::SetShotAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �ړ��p�x�����߂�
    D3DXVECTOR3 moveAngle = D3DXVECTOR3(-sinf(rot.y), rot.x, -cosf(rot.y));

    // �E�����i�̈ʒu�擾
    D3DXVECTOR3 rightWing = D3DXVECTOR3(GetParts(CUfo::PARTS_RIGHT_WING_2)->GetWorldMtx()._41,
        GetParts(CUfo::PARTS_RIGHT_WING_2)->GetWorldMtx()._42,
        GetParts(CUfo::PARTS_RIGHT_WING_2)->GetWorldMtx()._43);

    // �������i�̈ʒu�擾
    D3DXVECTOR3 leftWing = D3DXVECTOR3(GetParts(CUfo::PARTS_LEFT_WING_2)->GetWorldMtx()._41,
        GetParts(CUfo::PARTS_LEFT_WING_2)->GetWorldMtx()._42,
        GetParts(CUfo::PARTS_LEFT_WING_2)->GetWorldMtx()._43);

    // �e���N���G�C�g
    CBullet::Create(pos, BULLET_AR_COLLISION_SIZE, moveAngle, CBullet::TYPE_CLAP, CBullet::TRIBE_ENEMY);

    // �e���N���G�C�g
    CBullet::Create(rightWing, BULLET_AR_COLLISION_SIZE, moveAngle, CBullet::TYPE_CLAP, CBullet::TRIBE_ENEMY);

    // �e���N���G�C�g
    CBullet::Create(leftWing, BULLET_AR_COLLISION_SIZE, moveAngle, CBullet::TYPE_CLAP, CBullet::TRIBE_ENEMY);
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CUfo::Draw(void)
{
    CCharacter::Draw();
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : ��{��
//=============================================================================
HRESULT CUfo::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/00_Fighter.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_FIGHTER], NULL, &m_nNumMat[PARTS_FIGHTER], &m_pMesh[PARTS_FIGHTER]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/01_RightWing1.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_WING_0], NULL, &m_nNumMat[PARTS_RIGHT_WING_0], &m_pMesh[PARTS_RIGHT_WING_0]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/02_RightWing2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_WING_1], NULL, &m_nNumMat[PARTS_RIGHT_WING_1], &m_pMesh[PARTS_RIGHT_WING_1]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/03_RightWing3.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_WING_2], NULL, &m_nNumMat[PARTS_RIGHT_WING_2], &m_pMesh[PARTS_RIGHT_WING_2]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/04_RightWing4.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_WING_3], NULL, &m_nNumMat[PARTS_RIGHT_WING_3], &m_pMesh[PARTS_RIGHT_WING_3]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/05_LeftWing1.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_WING_0], NULL, &m_nNumMat[PARTS_LEFT_WING_0], &m_pMesh[PARTS_LEFT_WING_0]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/06_LeftWing2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_WING_1], NULL, &m_nNumMat[PARTS_LEFT_WING_1], &m_pMesh[PARTS_LEFT_WING_1]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/07_LeftWing3.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_WING_2], NULL, &m_nNumMat[PARTS_LEFT_WING_2], &m_pMesh[PARTS_LEFT_WING_2]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/08_LeftWing4.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_WING_3], NULL, &m_nNumMat[PARTS_LEFT_WING_3], &m_pMesh[PARTS_LEFT_WING_3]);
    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : ��{��
//=============================================================================
void CUfo::Unload(void)
{
    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        if (m_pBuffMat[nCount] != NULL)
        {
            m_pBuffMat[nCount] = NULL;
        }

        if (m_nNumMat[nCount] != NULL)
        {
            m_nNumMat[nCount] = NULL;
        }

        if (m_pMesh[nCount] != NULL)
        {
            m_pMesh[nCount] = NULL;
        }
    }
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CUfo * CUfo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �������m��
    CUfo *pUfo = NULL;
    pUfo = new CUfo;

    // �e���̏���ݒ�
    pUfo->SetPos(pos);
    pUfo->SetRot(rot);
    pUfo->SetTribe(CCharacter::TRIBE_UFO);

    // ������
    pUfo->Init(pos, DEFAULT_SCALE);

    // �p�g���[���̒��S�ʒu���L��
    pUfo->m_patrolCentralPos = pos;

    // �ڕW�ʒu���L��
    pUfo->m_patrolDestPos = pos;

    pUfo->SetActiveCollisionSize(UFO_ACTIVE_COLLISION_SIZE);
    pUfo->SetLife(UFO_LIFE);
    pUfo->m_nScore = UFO_SCORE;
    pUfo->m_nOrbs = UFO_ORBS;
    pUfo->m_fSpeed = UFO_SPEED;
    pUfo->m_nPatrolDistance = UFO_PATROL_DISTANCE;
    pUfo->m_fDiscoveryDistance = UFO_DISCOVERY_DISTANCE;

    pUfo->SetCntCollision(UFO_COLLISION_MAX);
    pUfo->SetCollisionSize(0, UFO_ACTIVE_COLLISION_SIZE);

    // �����蔻��̈ʒu���X�V
    pUfo->UpdateCollisionPos();

    return pUfo;
}

//=============================================================================
// �I�[�u���o���܂�����
// Author : �㓡�T�V��
//=============================================================================
void CUfo::ScatterOrbs(void)
{
    // �ʒu���擾
    D3DXVECTOR3 pos = GetPos();

    // �I�[�u��U�G�t�F�N�g
    float fSpeedX = 0.0f;   //����X
    float fSpeedY = 0.0f;   //����Y
    float fSpeedZ = 0.0f;   //����Z
    int nPlusMinusX = 0;    //����X
    int nPlusMinusZ = 0;    //����Z
    for (int nCnt = 0; nCnt < m_nOrbs; nCnt++)
    {
        // �����ƁA�����������_���Ɍ��߂�
        fSpeedX = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_X, 0)) / 100.0f;
        fSpeedY = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Y, EFFECT_CHARGE_SPREAD_MIN_Y)) / 100.0f;
        fSpeedZ = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Z, 0)) / 100.0f;
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

        // �G�t�F�N�g����
        CEffect3D::Create(pos, EFFECT_CHARGE_SIZE, move,
            EFFECT3D_PURPLE_FIRE_FIRST_COLOR, DEFAULT_VECTOR, EFFECT3D_PURPLE_FIRE_FADE_OUT_RATE, CEffect3D::TYPE_CHARGE);
    }
}

//=============================================================================
// ���ꂽ���̕��ӏ���
// Author : �㓡�T�V��
//=============================================================================
void CUfo::DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �p�[�c�̈ʒu�ƁA���f���G�t�F�N�g�̎��
    D3DXVECTOR3 partsPos = DEFAULT_VECTOR;
    CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_UFO;

    //==============================
    // ��
    //==============================
    // �ʒu
    partsPos = D3DXVECTOR3(GetParts(CUfo::PARTS_FIGHTER)->GetWorldMtx()._41,
        GetParts(CUfo::PARTS_FIGHTER)->GetWorldMtx()._42,
        GetParts(CUfo::PARTS_FIGHTER)->GetWorldMtx()._43);

    // �G�t�F�N�g�̔���
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // ��
    //==============================
    modelEffectType = CModelEffect::TYPE_UFO_WING;

    // ���̖{�����o���܂�
    for (int nCnt = 0; nCnt < UFO_MAX_LEG; nCnt++)
    {
        // �G�t�F�N�g�̔���(���͒��S����o��)
        SetDeathCrushEffect(pos, rot, modelEffectType);
    }
}

//=============================================================================
// ���ꂽ���̕��ӃG�t�F�N�g�̔���
// Author : �㓡�T�V��
//=============================================================================
void CUfo::SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type)
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
    fSpeedX = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_X, 0)) / 100.0f;
    fSpeedY = 0.0f;
    fSpeedZ = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Z, 0)) / 100.0f;
    fRotSpeedX = float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f;
    fRotSpeedY = float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f;
    fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_UFO_ROT_MAX, MODEL_EFFECT_UFO_ROT_MIN)) / 100.0f;
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

//=============================================================================
// ���ԂɌx�����鏈��
// Author : �㓡�T�V��
//=============================================================================
void CUfo::WarnFellow(void)
{
    // ���͂̒��Ԃ��x�����ɂ��鏈��
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_ENEMY);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_ENEMY); nCntScene++)
    {
        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // ���̃V�[�����L��
            CScene*pNextScene = pScene->GetNextScene();

            // �L�����N�^�[�ɃL���X�g
            CCharacter *pEnemy = (CCharacter*)pScene;

            // �G�̎푰��UFO�Ȃ�
            if (pEnemy->GetTribe() == CCharacter::TRIBE_UFO)
            {
                // �Ώۂ̈ʒu
                D3DXVECTOR3 targetPos = pEnemy->GetPos();

                // �ΏۂƂ̋���
                float fDistance = sqrtf(
                    powf((targetPos.x - GetPos().x), 2) +
                    powf((targetPos.z - GetPos().z), 2));

                // �ΏۂƂ̋������A���ԂɌx������Ƃ��͈͓̔��Ȃ�
                if (fDistance <= UFO_WARN_UFO_DISTANCE)
                {
                    // UFO�^�ɃL���X�g
                    CUfo*pUfo = (CUfo*)pEnemy;

                    // �x������
                    pUfo->SetWarning(true);
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
}

//=============================================================================
// �~�b�V�����N���A���ɉ��Z����
// Author : �㓡�T�V��
//=============================================================================
void CUfo::AddMissionClear(void)
{
    // �X�e�[�W�ƌ��݂̃~�b�V�����ɉ����āA�N���A���𑝉�
    CGame::STAGE stage = CGame::GetStage();
    CGame::CURRENT_MISSION currentMission = CGame::GetCurrentMission();
    switch (stage)
    {
    case CGame::STAGE_1:

        break;

    case CGame::STAGE_2:

        switch (currentMission)
        {
        case CGame::CURRENT_MISSION_1:
            CGame::SetClearNumMission1();
            break;

        case CGame::CURRENT_MISSION_2:

            break;
        }

        break;

    case CGame::STAGE_FINAL:

        break;
    }
}