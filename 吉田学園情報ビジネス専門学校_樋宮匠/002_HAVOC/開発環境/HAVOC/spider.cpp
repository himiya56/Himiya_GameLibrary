//================================================================================
//
// �w偏��� (spider.cpp)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//================================================================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "spider.h"
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
#include "item.h"
#include "sound.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
LPD3DXMESH CSpider::m_pMesh[TYPE_MAX][PARTS_MAX] = {};
LPD3DXBUFFER CSpider::m_pBuffMat[TYPE_MAX][PARTS_MAX] = {};
DWORD CSpider::m_nNumMat[TYPE_MAX][PARTS_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CSpider::CSpider() :CCharacter(OBJTYPE::OBJTYPE_ENEMY)
{
    m_type = TYPE_YELLOW;
    m_move = DEFAULT_VECTOR;
    m_nScore = 0;
    m_nOrbs = 0;

    m_patrolCentralPos = DEFAULT_VECTOR;
    m_patrolDestPos = DEFAULT_VECTOR;
    m_nPatrolDistance = 0;
    m_nCntTime = 0;

    m_bWallRun = false;
    m_fSpeed = 0.0f;
    m_rotDest = DEFAULT_VECTOR;
    m_rotMin = DEFAULT_VECTOR;

    m_bWarning = false;
    m_fDiscoveryDistance = 0.0f;

    m_bAttack = false;
    m_attackAI = ATTACK_AI_WAIT;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CSpider::~CSpider()
{
}

//=============================================================================
// ����������
// Author : ��{��
//=============================================================================
HRESULT CSpider::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �p�[�c����ݒ�
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_SPIDER, nCount, m_pMesh[m_type][nCount], m_pBuffMat[m_type][nCount], m_nNumMat[m_type][nCount]);
    }

    // ���W�E�e�q�֌W�̓ǂݍ���
    LoadModelData("./data/ANIMATION/motion_spider.txt");

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CSpider::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CSpider::Update(void)
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
            if (nRandNum <= SPIDER_DROP_HEAL_ITEM)
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
void CSpider::Movement(void)
{
    // �ʒu�ƌ������擾
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    D3DXVECTOR3 rot = DEFAULT_VECTOR;
    pos = GetPos();
    rot = GetRot();

    // ���[�V�������܂��͑ҋ@�ɂ���
    GetAnimation()->SetAnimation(ANIM_IDLE);

    // ��]����
    rot = RotControl(rot);

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

    // �n�ʂ���Ȃ�
    if (pos.y > 0.0f)
    {
        // �d��
        m_move.y -= GRAVITY;
    }
    else
    {
        // �Ǒ���͂��Ă��Ȃ�
        m_bWallRun = false;

        // �n�ʂɂ���Ȃ�A�d�͂������Ȃ�
        pos.y = 0.0f;
        m_move.y = 0.0f;
    }

    // �u���b�N�ւ̓����蔻��
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BLOCK);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BLOCK); nCntScene++)
    {
        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // ���̃V�[�����L��
            CScene*pNextScene = pScene->GetNextScene();

            // �u���b�N�ɃL���X�g
            CBlock *pBlock = (CBlock*)pScene;

            // �G���Ǒ���ł���u���b�N�Ȃ�
            if (pBlock->GetEnemyWallRun() == true)
            {
                // �u���b�N�̈ʒu�Ƒ傫�����擾
                D3DXVECTOR3 blockPos = pBlock->GetPos();
                D3DXVECTOR3 blockSize = pBlock->GetCollisionSize();

                // �u���b�N�Ƃ̓����蔻����v�Z
                BLOCK_HIT_INFO blockHitInfo;
                blockHitInfo.hitSurface = HIT_SURFACE_NONE;
                blockHitInfo.pos = pos;
                blockHitInfo = (BlockCollision3D(&pos, &GetPosOld(), &blockPos,
                    &GetActiveCollisionSize(), &blockSize));

                // �u���b�N�̓����蔻��Ōv�Z�����ʒu���A���т���
                pos = blockHitInfo.pos;

                // �q�b�g�ʂ���āA�Ǒ���̎d�l��ς���
                //=====================================================================================
                switch (blockHitInfo.hitSurface)
                {
                case HIT_SURFACE_TOP:

                    // �Ǒ���͂��Ă��Ȃ�
                    m_bWallRun = false;

                    // �d�͂��l�����Ȃ�
                    m_move.y = 0.0f;

                    break;

                case HIT_SURFACE_LEFT:

                    // �Ǒ���
                    pos.y += m_fSpeed;
                    m_bWallRun = true;
                    m_move.y = 0.0f;

                    // �X��
                    rot.x += SPIDER_TILT_SPEED;

                    // ����������ς���
                    m_rotDest.y = D3DXToRadian(270.0f);

                    break;

                case HIT_SURFACE_BACK:

                    // �Ǒ���
                    pos.y += m_fSpeed;
                    m_bWallRun = true;
                    m_move.y = 0.0f;

                    // �X��
                    rot.x += SPIDER_TILT_SPEED;

                    // ����������ς���
                    m_rotDest.y = D3DXToRadian(0.0f);

                    break;

                case HIT_SURFACE_RIGHT:

                    // �Ǒ���
                    pos.y += m_fSpeed;
                    m_bWallRun = true;
                    m_move.y = 0.0f;

                    // �X��
                    rot.x += SPIDER_TILT_SPEED;

                    // ����������ς���
                    m_rotDest.y = D3DXToRadian(90.0f);

                    break;

                case HIT_SURFACE_FRONT:

                    // �Ǒ���
                    pos.y += m_fSpeed;
                    m_bWallRun = true;
                    m_move.y = 0.0f;

                    // �X��
                    rot.x += SPIDER_TILT_SPEED;

                    // ����������ς���
                    m_rotDest.y = D3DXToRadian(180.0f);

                    break;
                }
                //=====================================================================================
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

    // �Ǒ��肵�Ă��Ȃ��Ȃ�
    if (m_bWallRun == false)
    {
        // �X���𒼂�
        rot.x -= SPIDER_TILT_SPEED;
    }

    // �X������
    if (rot.x > SPIDER_TILT_LIMIT)
    {
        rot.x = SPIDER_TILT_LIMIT;
    }
    else if (rot.x < 0.0f)
    {
        rot.x = 0.0f;
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
    CDebug::Create(pos, GetActiveCollisionSize(), CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// �����蔻��̈ʒu���X�V
// Author : �㓡�T�V��
//=============================================================================
void CSpider::UpdateCollisionPos(void)
{
    // �̂̈ʒu�擾
    D3DXVECTOR3 bodyPos = D3DXVECTOR3(GetParts(CSpider::PARTS_BODY)->GetWorldMtx()._41,
        GetParts(CSpider::PARTS_BODY)->GetWorldMtx()._42,
        GetParts(CSpider::PARTS_BODY)->GetWorldMtx()._43);

    // ���̈ʒu�擾
    D3DXVECTOR3 stomachPos = D3DXVECTOR3(GetParts(CSpider::PARTS_STOMACH_CUBE)->GetWorldMtx()._41,
        GetParts(CSpider::PARTS_STOMACH_CUBE)->GetWorldMtx()._42,
        GetParts(CSpider::PARTS_STOMACH_CUBE)->GetWorldMtx()._43);

    // �E��̈ʒu�擾
    D3DXVECTOR3 rightFangPos = D3DXVECTOR3(GetParts(CSpider::PARTS_FANG_RIGHT)->GetWorldMtx()._41,
        GetParts(CSpider::PARTS_FANG_RIGHT)->GetWorldMtx()._42,
        GetParts(CSpider::PARTS_FANG_RIGHT)->GetWorldMtx()._43);

    // ����̈ʒu�擾
    D3DXVECTOR3 leftFangPos = D3DXVECTOR3(GetParts(CSpider::PARTS_FANG_LEFT)->GetWorldMtx()._41,
        GetParts(CSpider::PARTS_FANG_LEFT)->GetWorldMtx()._42,
        GetParts(CSpider::PARTS_FANG_LEFT)->GetWorldMtx()._43);

    // ��ނ��ƂɁA�����蔻��̈ʒu���X�V
    switch (m_type)
    {
    case TYPE_YELLOW:
        SetCollisionPos(0, GetPos());
        break;
    case TYPE_BLACK:
        SetCollisionPos(0, bodyPos);
        SetCollisionPos(1, stomachPos);
        SetCollisionPos(2, rightFangPos);
        SetCollisionPos(3, leftFangPos);
        break;
    }
}

//=============================================================================
// ��]����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CSpider::RotControl(D3DXVECTOR3 rot)
{
    // �Ԃ���]��
    D3DXVECTOR3 returnRot = rot;

    // 1F�O�ƍ��W���Ⴄ�Ȃ�
    if (GetPosOld() != GetPos())
    {
        // �����ɂ���
        GetAnimation()->SetAnimation(ANIM_WALK);

        // 1F�O�̈ʒu�ƁA���݂̈ʒu���r���A�p�x�����߂�
        float fAngle = atan2((GetPosOld().x - GetPos().x), (GetPosOld().z - GetPos().z));

        // 1F�O��X���W�܂���Z���W���Ⴄ�Ȃ�
        if (GetPosOld().x != GetPos().x || GetPosOld().z != GetPos().z)
        {
            // �Ǒ��肵�Ă��Ȃ��Ȃ�
            if (m_bWallRun == false)
            {
                // ���߂��p�x�������悤�ɂ���
                m_rotDest.y = fAngle;
            }
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
            returnRot.y += (m_rotDest.y - returnRot.y) * SPIDER_TURN_SPEED;
        }
    }

    return returnRot;
}

//=============================================================================
// ����AI����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CSpider::PatrolAI(D3DXVECTOR3 pos)
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
        if (fDistance <= SPIDER_CONSIDER_CLOSE_DEST)
        {
            // �J�E���^�����Z
            m_nCntTime++;

            // ���J�E���^�ŁA���̖ړI�n�����߂�
            if (m_nCntTime >= SPIDER_CHANGE_MOVE_IN_PATROL)
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
D3DXVECTOR3 CSpider::WarningAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �Ԃ�����
    D3DXVECTOR3 returnRot = rot;

    // �v���C���[�̈ʒu���擾
    D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

    // �U�����Ă��Ȃ����A�Ǒ��肵�Ă��Ȃ��Ȃ�
    if (m_bAttack == false && m_bWallRun == false)
    {
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
        // �x�����Ȃ�A
        else
        {
            // �v���C���[�̈ʒu�ƁA���g�̈ʒu���r���A�p�x�����߂�
            float fAngle = atan2((GetPos().x - playerPos.x), (GetPos().z - playerPos.z));

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
                returnRot.y += (m_rotDest.y - returnRot.y) * SPIDER_TURN_SPEED;
            }

            // �v���C���[�Ƃ̋����𑪂�
            float fDistance = sqrtf(
                powf((playerPos.x - pos.x), 2) +
                powf((playerPos.z - pos.z), 2));

            // �������𒴂�����
            if (fDistance >= SPIDER_CONSIDER_AS_LONG)
            {
                // �x����������
                m_bWarning = false;
            }
        }
    }

    return returnRot;
}

//=============================================================================
// �U��AI����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CSpider::AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �Ԃ��ʒu�A�ړ��ʂ����߂�
    D3DXVECTOR3 returnPos = pos;
    D3DXVECTOR3 move = DEFAULT_VECTOR;

    // �x�����Ȃ�
    if (m_bWarning == true)
    {
        switch (m_attackAI)
        {
        case ATTACK_AI_WAIT:

            // �U�����Ă��Ȃ�
            m_bAttack = false;

            // �J�E���^�A�b�v
            m_nCntTime++;

            // ���J�E���^��
            if (m_nCntTime >= SPIDER_NEXT_ATTACK_COUNT)
            {
                // �J�E���^���Z�b�g
                m_nCntTime = 0;

                // �����_���ɐ��������߂�
                int nRandNum = GetRandNum(SPIDER_NEXT_ATTACK_PERCENT_MAX, 1);

                // ���̍U�����̗p���ꂽ�����߂�
                if (nRandNum >= 1 && nRandNum <= SPIDER_TAUNT_PERCENT)
                {
                    m_attackAI = ATTACK_AI_TAUNT;
                }
                else if (nRandNum > SPIDER_TAUNT_PERCENT && nRandNum <= SPIDER_TAUNT_PERCENT + SPIDER_ACID_PERCENT)
                {
                    m_attackAI = ATTACK_AI_ACID;
                }
                else if (nRandNum > SPIDER_TAUNT_PERCENT + SPIDER_ACID_PERCENT && nRandNum <= SPIDER_JUMP_PERCENT + SPIDER_TAUNT_PERCENT + SPIDER_ACID_PERCENT)
                {
                    m_attackAI = ATTACK_AI_JUMP;
                }
            }

            break;

        case ATTACK_AI_TAUNT:

            // �������U���Ƃ݂Ȃ�
            m_bAttack = true;

            // �J�E���^�A�b�v
            m_nCntTime++;

            // �A�j���[�V�����𒧔���
            GetAnimation()->SetAnimation(ANIM_TAUNT);

            // ���J�E���^��
            if (m_nCntTime >= SPIDER_TAUNT_WHOLE_FRAME)
            {
                // �J�E���^���Z�b�g
                m_nCntTime = 0;

                // �ҋ@���ɂ���
                m_attackAI = ATTACK_AI_WAIT;
            }

            break;

        case ATTACK_AI_ACID:

            // �_��΂��͍U�����Ă���
            m_bAttack = true;

            // �J�E���^�A�b�v
            m_nCntTime++;

            // �A�j���[�V�������U����
            GetAnimation()->SetAnimation(ANIM_ATTACK);

            // �N���G�C�g�t���[��
            if (m_nCntTime == SPIDER_ACID_CREATE_FRAME)
            {
                SetAcidAttack(pos, rot);
            }

            // ���J�E���^��
            if (m_nCntTime >= SPIDER_ACID_WHOLE_FRAME)
            {
                // �J�E���^���Z�b�g
                m_nCntTime = 0;

                // �ҋ@���ɂ���
                m_attackAI = ATTACK_AI_WAIT;
            }

            break;

        case ATTACK_AI_JUMP:

            // �W�����v�͍U�����Ă���
            m_bAttack = true;

            // �J�E���^�A�b�v
            m_nCntTime++;

            // �Ǔo�蒆�łȂ��Ȃ�
            if (m_bWallRun == false)
            {
                // �A�j���[�V�������W�����v��
                GetAnimation()->SetAnimation(ANIM_JUMP);
            }

            // �W�����v�U��
            returnPos = SetJumpAttack(pos, rot);

            // ����̒��S�ʒu��ς���
            m_patrolCentralPos = returnPos;

            // ���J�E���^��
            if (m_nCntTime >= SPIDER_JUMP_WHOLE_FRAME)
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
// �_��΂��U��
// Author : �㓡�T�V��
//=============================================================================
void CSpider::SetAcidAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �v���C���[�̈ʒu���擾
    D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

    // �v���C���[�Ƃ̋����𑪂�
    float fDistance = sqrtf(
        powf((playerPos.x - pos.x), 2) +
        powf((playerPos.z - pos.z), 2));
    float fAngleY = 0.0f;

    // ��΂��p�x�␔���A��ނɂ���ĕς���
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR;
    float fHorizontalAngle = 0.0f;
    switch (m_type)
    {
    case TYPE_YELLOW:

        // �����ɉ����āA�����_���ɏ�ɔ�΂�
        // �ߋ���
        if (fDistance <= SPIDER_CONSIDER_AS_SHORT)
        {
            fAngleY = D3DXToRadian(float(GetRandNum(SPIDER_ACID_SHORT_MAX_ANGLE_Y, SPIDER_ACID_SHORT_MIN_ANGLE_Y)) / 100.0f);
        }
        // ������
        else if (fDistance > SPIDER_CONSIDER_AS_SHORT && fDistance <= SPIDER_CONSIDER_AS_MEDIUM)
        {
            fAngleY = D3DXToRadian(float(GetRandNum(SPIDER_ACID_MEDIUM_MAX_ANGLE_Y, SPIDER_ACID_MEDIUM_MIN_ANGLE_Y)) / 100.0f);
        }
        // ������
        else if (fDistance > SPIDER_CONSIDER_AS_MEDIUM)
        {
            fAngleY = D3DXToRadian(float(GetRandNum(SPIDER_ACID_LONG_MAX_ANGLE_Y, SPIDER_ACID_LONG_MIN_ANGLE_Y)) / 100.0f);
        }

        // �ړ��p�x�����߂�
        moveAngle = D3DXVECTOR3(-sinf(rot.y), fAngleY, -cosf(rot.y));

        // �e���N���G�C�g
        CBullet::Create(pos + SPIDER_ACID_ADD_SET_POS_Y, BULLET_ACID_COLLISION_SIZE, moveAngle, CBullet::TYPE_ACID, CBullet::TRIBE_ENEMY);

        break;

    case TYPE_BLACK:

        // ������΂�
        for (int nCnt = 0; nCnt < SPIDER_BLACK_ACID_NUMBER_ONCE; nCnt++)
        {
            // �����ɉ����āA�����_���ɏ�ɔ�΂�
            // �ߋ���
            if (fDistance <= SPIDER_CONSIDER_AS_SHORT)
            {
                fAngleY = D3DXToRadian(float(GetRandNum(SPIDER_ACID_SHORT_MAX_ANGLE_Y, SPIDER_ACID_SHORT_MIN_ANGLE_Y)) / 100.0f);
            }
            // ������
            else if (fDistance > SPIDER_CONSIDER_AS_SHORT && fDistance <= SPIDER_CONSIDER_AS_MEDIUM)
            {
                fAngleY = D3DXToRadian(float(GetRandNum(SPIDER_ACID_MEDIUM_MAX_ANGLE_Y, SPIDER_ACID_MEDIUM_MIN_ANGLE_Y)) / 100.0f);
            }
            // ������
            else if (fDistance > SPIDER_CONSIDER_AS_MEDIUM)
            {
                fAngleY = D3DXToRadian(float(GetRandNum(SPIDER_ACID_LONG_MAX_ANGLE_Y, SPIDER_ACID_LONG_MIN_ANGLE_Y)) / 100.0f);
            }

            // ���Ɋg�U������
            if (nCnt == 1)
            {
                fHorizontalAngle = D3DXToRadian(float(GetRandNum(SPIDER_ACID_MAX_ANGLE_TO_SHIFT_FROM_CENTER, 0)) / 100.0f);
            }
            else if (nCnt == 2)
            {
                fHorizontalAngle = -D3DXToRadian(float(GetRandNum(SPIDER_ACID_MAX_ANGLE_TO_SHIFT_FROM_CENTER, 0)) / 100.0f);
            }

            // �ړ��p�x�����߂�
            moveAngle = D3DXVECTOR3(-sinf(rot.y + fHorizontalAngle), fAngleY, -cosf(rot.y + fHorizontalAngle));

            // �e���N���G�C�g
            CBullet::Create(pos + SPIDER_ACID_ADD_SET_POS_Y, BULLET_ACID_COLLISION_SIZE, moveAngle, CBullet::TYPE_ACID, CBullet::TRIBE_ENEMY);
        }

        break;
    }
}

//=============================================================================
// �W�����v�U��
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CSpider::SetJumpAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �Ԃ��ʒu�����߂�
    D3DXVECTOR3 returnPos = pos;

    // �c�̈ړ��ʂ��Ȃ��Ȃ�����
    if (m_move.y == 0.0f)
    {
        // �Ǔo�蒆�łȂ��Ȃ�
        if (m_bWallRun == false)
        {
            // �W�����v
            m_move.y += SPIDER_JUMP_VALUE;
        }
    }

    // ���Ă�������ɐi��
    returnPos += D3DXVECTOR3(-sinf(rot.y) * m_fSpeed * SPIDER_JUMP_SPEED_RATE, 0.0f, -cosf(rot.y) * m_fSpeed* SPIDER_JUMP_SPEED_RATE);

    return returnPos;
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CSpider::Draw(void)
{
    CCharacter::Draw();
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : ��{��
//=============================================================================
HRESULT CSpider::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_000_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_BODY], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_BODY], &m_pMesh[TYPE_YELLOW][PARTS_BODY]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_001_stomach.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_STOMACH], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_STOMACH], &m_pMesh[TYPE_YELLOW][PARTS_STOMACH]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_002_fang_000.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_FANG_RIGHT], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_FANG_RIGHT], &m_pMesh[TYPE_YELLOW][PARTS_FANG_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_003_fang_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_FANG_LEFT], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_FANG_LEFT], &m_pMesh[TYPE_YELLOW][PARTS_FANG_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_004_leg_right_000.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_LEG_RIGHT_0], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_LEG_RIGHT_0], &m_pMesh[TYPE_YELLOW][PARTS_LEG_RIGHT_0]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_005_leg_right_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_LEG_RIGHT_1], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_LEG_RIGHT_1], &m_pMesh[TYPE_YELLOW][PARTS_LEG_RIGHT_1]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_006_leg_right_002.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_LEG_RIGHT_2], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_LEG_RIGHT_2], &m_pMesh[TYPE_YELLOW][PARTS_LEG_RIGHT_2]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_007_leg_right_003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_LEG_RIGHT_3], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_LEG_RIGHT_3], &m_pMesh[TYPE_YELLOW][PARTS_LEG_RIGHT_3]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_008_leg_left_000.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_LEG_LEFT_0], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_LEG_LEFT_0], &m_pMesh[TYPE_YELLOW][PARTS_LEG_LEFT_0]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_009_leg_left_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_LEG_LEFT_1], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_LEG_LEFT_1], &m_pMesh[TYPE_YELLOW][PARTS_LEG_LEFT_1]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_010_leg_left_002.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_LEG_LEFT_2], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_LEG_LEFT_2], &m_pMesh[TYPE_YELLOW][PARTS_LEG_LEFT_2]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_011_leg_left_003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_LEG_LEFT_3], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_LEG_LEFT_3], &m_pMesh[TYPE_YELLOW][PARTS_LEG_LEFT_3]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_012_cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_YELLOW][PARTS_STOMACH_CUBE], NULL, &m_nNumMat[TYPE_YELLOW][PARTS_STOMACH_CUBE], &m_pMesh[TYPE_YELLOW][PARTS_STOMACH_CUBE]);

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_000_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_BODY], NULL, &m_nNumMat[TYPE_BLACK][PARTS_BODY], &m_pMesh[TYPE_BLACK][PARTS_BODY]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_001_stomach.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_STOMACH], NULL, &m_nNumMat[TYPE_BLACK][PARTS_STOMACH], &m_pMesh[TYPE_BLACK][PARTS_STOMACH]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_002_fang_000.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_FANG_RIGHT], NULL, &m_nNumMat[TYPE_BLACK][PARTS_FANG_RIGHT], &m_pMesh[TYPE_BLACK][PARTS_FANG_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_003_fang_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_FANG_LEFT], NULL, &m_nNumMat[TYPE_BLACK][PARTS_FANG_LEFT], &m_pMesh[TYPE_BLACK][PARTS_FANG_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_004_leg_right_000.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_LEG_RIGHT_0], NULL, &m_nNumMat[TYPE_BLACK][PARTS_LEG_RIGHT_0], &m_pMesh[TYPE_BLACK][PARTS_LEG_RIGHT_0]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_005_leg_right_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_LEG_RIGHT_1], NULL, &m_nNumMat[TYPE_BLACK][PARTS_LEG_RIGHT_1], &m_pMesh[TYPE_BLACK][PARTS_LEG_RIGHT_1]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_006_leg_right_002.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_LEG_RIGHT_2], NULL, &m_nNumMat[TYPE_BLACK][PARTS_LEG_RIGHT_2], &m_pMesh[TYPE_BLACK][PARTS_LEG_RIGHT_2]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_007_leg_right_003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_LEG_RIGHT_3], NULL, &m_nNumMat[TYPE_BLACK][PARTS_LEG_RIGHT_3], &m_pMesh[TYPE_BLACK][PARTS_LEG_RIGHT_3]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_008_leg_left_000.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_LEG_LEFT_0], NULL, &m_nNumMat[TYPE_BLACK][PARTS_LEG_LEFT_0], &m_pMesh[TYPE_BLACK][PARTS_LEG_LEFT_0]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_009_leg_left_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_LEG_LEFT_1], NULL, &m_nNumMat[TYPE_BLACK][PARTS_LEG_LEFT_1], &m_pMesh[TYPE_BLACK][PARTS_LEG_LEFT_1]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_010_leg_left_002.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_LEG_LEFT_2], NULL, &m_nNumMat[TYPE_BLACK][PARTS_LEG_LEFT_2], &m_pMesh[TYPE_BLACK][PARTS_LEG_LEFT_2]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_011_leg_left_003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_LEG_LEFT_3], NULL, &m_nNumMat[TYPE_BLACK][PARTS_LEG_LEFT_3], &m_pMesh[TYPE_BLACK][PARTS_LEG_LEFT_3]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_012_cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[TYPE_BLACK][PARTS_STOMACH_CUBE], NULL, &m_nNumMat[TYPE_BLACK][PARTS_STOMACH_CUBE], &m_pMesh[TYPE_BLACK][PARTS_STOMACH_CUBE]);

    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : ��{��
//=============================================================================
void CSpider::Unload(void)
{
    for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
    {
        for (int nCount = 0; nCount < PARTS_MAX; nCount++)
        {
            if (m_pBuffMat[nCntType][nCount] != NULL)
            {
                m_pBuffMat[nCntType][nCount] = NULL;
            }

            if (m_nNumMat[nCntType][nCount] != NULL)
            {
                m_nNumMat[nCntType][nCount] = NULL;
            }

            if (m_pMesh[nCntType][nCount] != NULL)
            {
                m_pMesh[nCntType][nCount] = NULL;
            }
        }
    }
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CSpider * CSpider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bWarning, TYPE type)
{
    // �������m��
    CSpider *pSpider = NULL;
    pSpider = new CSpider;

    // �e���̏���ݒ�
    pSpider->SetPos(pos);
    pSpider->SetRot(rot);
    pSpider->SetTribe(CCharacter::TRIBE_SPIDER);

    // �ǂݍ��ގ�ނ̐ݒ�
    pSpider->m_type = type;

    // ������(���w偂̓X�P�[��2�{)
    D3DXVECTOR3 scale = DEFAULT_SCALE;
    if (type == TYPE_BLACK)
    {
        scale = SPIDER_BLACK_SCALE;
    }
    pSpider->Init(pos, scale);

    // �p�g���[���̒��S�ʒu���L��
    pSpider->m_patrolCentralPos = pos;

    // �ڕW�ʒu���L��
    pSpider->m_patrolDestPos = pos;

    // �x�������ǂ������������猋�т���
    pSpider->m_bWarning = bWarning;

    // �^�C�v���ƂɁA�Œ�̒l��R�Â�
    switch (type)
    {
    case TYPE_YELLOW:

        pSpider->SetActiveCollisionSize(SPIDER_YELLOW_ACTIVE_COLLISION_SIZE);
        pSpider->SetLife(SPIDER_YELLOW_LIFE);
        pSpider->m_nScore = SPIDER_YELLOW_SCORE;
        pSpider->m_nOrbs = SPIDER_YELLOW_ORBS;
        pSpider->m_fSpeed = SPIDER_YELLOW_SPEED;
        pSpider->m_nPatrolDistance = SPIDER_YELLOW_PATROL_DISTANCE;
        pSpider->m_fDiscoveryDistance = SPIDER_YELLOW_DISCOVERY_DISTANCE;

        pSpider->SetCntCollision(SPIDER_YELLOW_COLLISION_MAX);
        pSpider->SetCollisionSize(0, SPIDER_YELLOW_BODY_COLLISION_SIZE);

        break;

    case TYPE_BLACK:

        pSpider->SetActiveCollisionSize(SPIDER_BLACK_ACTIVE_COLLISION_SIZE);
        pSpider->SetLife(SPIDER_BLACK_LIFE);
        pSpider->m_nScore = SPIDER_BLACK_SCORE;
        pSpider->m_nOrbs = SPIDER_BLACK_ORBS;
        pSpider->m_fSpeed = SPIDER_BLACK_SPEED;
        pSpider->m_nPatrolDistance = SPIDER_BLACK_PATROL_DISTANCE;
        pSpider->m_fDiscoveryDistance = SPIDER_BLACK_DISCOVERY_DISTANCE;

        pSpider->SetCntCollision(SPIDER_BLACK_COLLISION_MAX);
        pSpider->SetCollisionSize(0, SPIDER_BLACK_BODY_COLLISION_SIZE);
        pSpider->SetCollisionSize(1, SPIDER_BLACK_STOMACH_COLLISION_SIZE);
        pSpider->SetCollisionSize(2, SPIDER_BLACK_FANG_COLLISION_SIZE);
        pSpider->SetCollisionSize(3, SPIDER_BLACK_FANG_COLLISION_SIZE);

        break;
    }

    // �����蔻��̈ʒu���X�V
    pSpider->UpdateCollisionPos();

    return pSpider;
}

//=============================================================================
// �I�[�u���o���܂�����
// Author : �㓡�T�V��
//=============================================================================
void CSpider::ScatterOrbs(void)
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
void CSpider::DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �����j��
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_BUG);

    // �p�[�c�̈ʒu�ƁA���f���G�t�F�N�g�̎��
    D3DXVECTOR3 partsPos = DEFAULT_VECTOR;
    CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_YELLOW_SPIDER_BODY;

    //==============================
    // ��
    //==============================
    // �ʒu
    partsPos = D3DXVECTOR3(GetParts(CSpider::PARTS_BODY)->GetWorldMtx()._41,
        GetParts(CSpider::PARTS_BODY)->GetWorldMtx()._42,
        GetParts(CSpider::PARTS_BODY)->GetWorldMtx()._43);

    // ���
    switch (m_type)
    {
    case TYPE_YELLOW:
        modelEffectType = CModelEffect::TYPE_YELLOW_SPIDER_BODY;
        break;
    case TYPE_BLACK:
        modelEffectType = CModelEffect::TYPE_BLACK_SPIDER_BODY;
        break;
    }

    // �G�t�F�N�g�̔���
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // ��
    //==============================
    // �ʒu
    partsPos = D3DXVECTOR3(GetParts(CSpider::PARTS_STOMACH)->GetWorldMtx()._41,
        GetParts(CSpider::PARTS_STOMACH)->GetWorldMtx()._42,
        GetParts(CSpider::PARTS_STOMACH)->GetWorldMtx()._43);

    // ���
    switch (m_type)
    {
    case TYPE_YELLOW:
        modelEffectType = CModelEffect::TYPE_YELLOW_SPIDER_STOMACH;
        break;
    case TYPE_BLACK:
        modelEffectType = CModelEffect::TYPE_BLACK_SPIDER_STOMACH;
        break;
    }

    // �G�t�F�N�g�̔���
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // ��
    //==============================
    // ���
    switch (m_type)
    {
    case TYPE_YELLOW:
        modelEffectType = CModelEffect::TYPE_YELLOW_SPIDER_LEG;
        break;
    case TYPE_BLACK:
        modelEffectType = CModelEffect::TYPE_BLACK_SPIDER_LEG;
        break;
    }

    // ���̖{�����o���܂�
    for (int nCnt = 0; nCnt < SPIDER_MAX_LEG; nCnt++)
    {
        // �G�t�F�N�g�̔���(���͒��S����o��)
        SetDeathCrushEffect(pos, rot, modelEffectType);
    }
}

//=============================================================================
// ���ꂽ���̕��ӃG�t�F�N�g�̔���
// Author : �㓡�T�V��
//=============================================================================
void CSpider::SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type)
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
    fSpeedY = float(GetRandNum(MODEL_EFFECT_SPIDER_SPLATTER_FORCE_MAX, MODEL_EFFECT_SPIDER_SPLATTER_FORCE_MIN)) / 100.0f;
    fSpeedZ = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Z, 0)) / 100.0f;
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

//=============================================================================
// ���ԂɌx�����鏈��
// Author : �㓡�T�V��
//=============================================================================
void CSpider::WarnFellow(void)
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

            // �G�̎푰���w偂Ȃ�
            if (pEnemy->GetTribe() == CCharacter::TRIBE_SPIDER)
            {
                // �Ώۂ̈ʒu
                D3DXVECTOR3 targetPos = pEnemy->GetPos();

                // �ΏۂƂ̋���
                float fDistance = sqrtf(
                    powf((targetPos.x - GetPos().x), 2) +
                    powf((targetPos.z - GetPos().z), 2));

                // �ΏۂƂ̋������A���ԂɌx������Ƃ��͈͓̔��Ȃ�
                if (fDistance <= SPIDER_WARN_SPIDER_DISTANCE)
                {
                    // �X�p�C�_�[�^�ɃL���X�g
                    CSpider*pSpider = (CSpider*)pEnemy;

                    // �Ǒ��肵�Ă��Ȃ��Ȃ�
                    if (pSpider->GetWallRun() == false)
                    {
                        // �x������
                        pSpider->SetWarning(true);
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
}

//=============================================================================
// �~�b�V�����N���A���ɉ��Z����
// Author : �㓡�T�V��
//=============================================================================
void CSpider::AddMissionClear(void)
{
    // �X�e�[�W�ƌ��݂̃~�b�V�����ɉ����āA�N���A���𑝉�
    CGame::STAGE stage = CGame::GetStage();
    CGame::CURRENT_MISSION currentMission = CGame::GetCurrentMission();
    switch (stage)
    {
    case CGame::STAGE_1:

        switch (currentMission)
        {
        case CGame::CURRENT_MISSION_1:
            CGame::SetClearNumMission1();
            break;

        case CGame::CURRENT_MISSION_2:

            break;
        }

        break;

    case CGame::STAGE_2:

        break;

    case CGame::STAGE_FINAL:

        break;
    }
}