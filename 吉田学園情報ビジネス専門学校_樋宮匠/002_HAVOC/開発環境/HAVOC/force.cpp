//================================================================================
//
// �h�q�R���� (force.cpp)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//================================================================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "force.h"
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
#include "sound.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
LPD3DXMESH CForce::m_pMesh[PARTS_MAX] = {};
LPD3DXBUFFER CForce::m_pBuffMat[PARTS_MAX] = {};
DWORD CForce::m_nNumMat[PARTS_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CForce::CForce() :CCharacter(OBJTYPE::OBJTYPE_ENEMY)
{
    m_move = DEFAULT_VECTOR;
    m_nScore = 0;
    m_nOrbs = 0;

    m_nCntTime = 0;

    m_fSpeed = 0.0f;
    m_rotDest = DEFAULT_VECTOR;
    m_rotMin = DEFAULT_VECTOR;

    m_fDiscoveryDistance = 0.0f;

    m_attackAI = ATTACK_AI_WAIT;

    m_targetPos = DEFAULT_VECTOR;
    m_bUseRotControl = true;

    m_nDeathCounter = 0;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CForce::~CForce()
{
}

//=============================================================================
// ����������
// Author : ��{��
//=============================================================================
HRESULT CForce::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �p�[�c����ݒ�
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_FORCE, nCount, m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
    }

    // ���W�E�e�q�֌W�̓ǂݍ���
    LoadModelData("./data/ANIMATION/motion_ranger.txt");

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CForce::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CForce::Update(void)
{
    // �̗͂�����Ȃ�
    if (GetLife() > 0)
    {
        // �s��
        Movement();
    }

    // �A�j���[�V�����̍X�V
    CCharacter::Update();

    // �̗͂�0�ȉ��Ȃ�A�X�R�A���Z���A���g�p��
    if (GetLife() <= 0)
    {
        // ���[�V�������f�X�ɂ���
        GetAnimation()->SetAnimation(ANIM_DEATH);

        // �f�X�J�E���^�[�����Z
        m_nDeathCounter++;

        // ���j����炷�t���[����
        if (m_nDeathCounter == FORCE_DEATH_PLAY_SOUND)
        {
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_FORCE);
        }

        // ���J�E���^�ŁA�폜
        if (m_nDeathCounter >= FORCE_DEATH_COUNTER)
        {
            // �ʒu�ƌ������擾
            D3DXVECTOR3 pos = DEFAULT_VECTOR;
            D3DXVECTOR3 rot = DEFAULT_VECTOR;
            pos = GetPos();
            rot = GetRot();

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
                if (nRandNum <= FORCE_DROP_HEAL_ITEM)
                {
                    CItem::Create(pos, DEFAULT_VECTOR, CItem::TYPE_HEAL);
                }
            }

            // �I������
            Uninit();
        }
    }
}

//=============================================================================
// �s������
// Author : �㓡�T�V��
//=============================================================================
void CForce::Movement(void)
{
    // �ʒu�ƌ������擾
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    D3DXVECTOR3 rot = DEFAULT_VECTOR;
    pos = GetPos();
    rot = GetRot();

    // ���[�V�������܂��͑ҋ@�ɂ���
    GetAnimation()->SetAnimation(ANIM_IDLE);

    // �ڕW�ւ̈ʒu�����߂�
    DicisionTargetPos(pos);

    // ��]���䂷��Ȃ�
    if (m_bUseRotControl == true)
    {
        // ��]����
        rot = RotControl(rot);
    }

    // 1F�O�̈ʒu���L��
    SetPosOld(pos);

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
void CForce::UpdateCollisionPos(void)
{
    SetCollisionPos(0, GetPos());
}

//=============================================================================
// ��]����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CForce::RotControl(D3DXVECTOR3 rot)
{
    // �Ԃ���]��
    D3DXVECTOR3 returnRot = rot;

    // �ڕW�ւ̊p�x�����߂�
    float fAngle = atan2((GetPos().x - m_targetPos.x), (GetPos().z - m_targetPos.z));

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
        returnRot.y += (m_rotDest.y - returnRot.y) * FORCE_TURN_SPEED;
    }

    return returnRot;
}

//=============================================================================
// �U��AI����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CForce::AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �Ԃ��ʒu�A�ړ��ʂ����߂�
    D3DXVECTOR3 returnPos = pos;
    D3DXVECTOR3 move = DEFAULT_VECTOR;

    switch (m_attackAI)
    {
    case ATTACK_AI_WAIT:

        // ��]���䂷��
        m_bUseRotControl = true;

        // �J�E���^�A�b�v
        m_nCntTime++;

        // ���J�E���^��
        if (m_nCntTime >= FORCE_NEXT_ATTACK_COUNT)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // ���邩�U�����邩�̔��f
            JudgmentRunOrAttack(pos);
        }

        break;

    case ATTACK_AI_DUSH:

        // ��]���䂵�Ȃ�
        m_bUseRotControl = false;

        // �J�E���^�A�b�v
        m_nCntTime++;

        // �A�j���[�V�����𑖂��
        GetAnimation()->SetAnimation(ANIM_WALK);

        // ����
        returnPos = Dush(returnPos, rot);

        // ���J�E���^��
        if (m_nCntTime >= FORCE_DUSH_WHOLE_FRAME)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // �U���ɔh��
            m_attackAI = ATTACK_AI_SHOT;
        }

        break;

    case ATTACK_AI_SHOT:

        // ��]���䂷��
        m_bUseRotControl = true;

        // �J�E���^�A�b�v
        m_nCntTime++;

        // �A�j���[�V�������U����
        GetAnimation()->SetAnimation(ANIM_ATTACK);

        // �N���G�C�g�t���[��
        if (m_nCntTime % FORCE_SHOT_CREATE_FRAME == 0)
        {
            SetShotAttack(pos, rot);
        }

        // ���J�E���^��
        if (m_nCntTime >= FORCE_SHOT_WHOLE_FRAME)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // �ҋ@���ɂ���
            m_attackAI = ATTACK_AI_WAIT;
        }

        break;
    }

    return returnPos;
}

//=============================================================================
// ���邩�U�����邩�̔��f
// Author : �㓡�T�V��
//=============================================================================
void CForce::JudgmentRunOrAttack(D3DXVECTOR3 pos)
{
    // �ڕW�܂ł̋���
    float fDistance = sqrtf(powf((m_targetPos.x - pos.x), 2) + powf((m_targetPos.z - pos.z), 2));

    // ���������ɂ���Ȃ�
    if (fDistance <= m_fDiscoveryDistance)
    {
        // �U���ɂ���
        m_attackAI = ATTACK_AI_SHOT;
    }
    // ���������ɂ��Ȃ��Ȃ�
    else
    {
        // ����
        m_attackAI = ATTACK_AI_DUSH;
    }
}

//=============================================================================
// �^�[�Q�b�g�̈ʒu�����߂�
// Author : �㓡�T�V��
//=============================================================================
void CForce::DicisionTargetPos(D3DXVECTOR3 pos)
{
    // �^�[�Q�b�g�̈ʒu�ɉ����āA�D�揇�ʂ����鏈��
    m_targetPos = CGame::GetPlayer()->GetPos();                                                              // �ڕW�̈ʒu(��x�v���C���[��)
    float fFirstTargetDistance = sqrtf(powf((m_targetPos.x - pos.x), 2) + powf((m_targetPos.z - pos.z), 2)); // �ڕW�܂ł̋���1
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

            // �G�̎푰���h�q�R�ȊO�Ȃ�
            if (pEnemy->GetTribe() != CCharacter::TRIBE_FORCE)
            {
                // �Ώۂ̈ʒu
                D3DXVECTOR3 targetPos = pEnemy->GetPos();

                // �ڕW�܂ł̋���2
                float fSecondTargetDistance = sqrtf(
                    powf((targetPos.x - pos.x), 2) +
                    powf((targetPos.z - pos.z), 2));

                // �ڕW�܂ł̋���2���A�ڕW�܂ł̋���1�����߂��Ȃ�
                if (fSecondTargetDistance < fFirstTargetDistance)
                {
                    // �^�[�Q�b�g�̈ʒu���X�V
                    m_targetPos = targetPos;

                    // �ڕW�܂ł̋���1���X�V
                    fFirstTargetDistance = fSecondTargetDistance;
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
// ���鏈��
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CForce::Dush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    D3DXVECTOR3 returnPos = pos;

    // ���Ă�������ɐi��
    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(rot.y) * m_fSpeed, 0.0f, -cosf(rot.y) * m_fSpeed);
    returnPos += move;

    // �ڕW�܂ł̋���
    float fDistance = sqrtf(powf((m_targetPos.x - returnPos.x), 2) + powf((m_targetPos.z - returnPos.z), 2));

    // ���������ɂ���Ȃ�
    if (fDistance <= m_fDiscoveryDistance)
    {
        // �J�E���^���Z�b�g
        m_nCntTime = 0;

        // �����ɍU������
        m_attackAI = ATTACK_AI_SHOT;
    }

    return returnPos;
}

//=============================================================================
// �ˌ��U��
// Author : �㓡�T�V��
//=============================================================================
void CForce::SetShotAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // ����̈ʒu�擾
    D3DXVECTOR3 weaponPos = D3DXVECTOR3(GetParts(CForce::PARTS_WEAPON)->GetWorldMtx()._41,
        GetParts(CForce::PARTS_WEAPON)->GetWorldMtx()._42,
        GetParts(CForce::PARTS_WEAPON)->GetWorldMtx()._43);

    // �v���C���[�Ƃ̋����𑪂�
    float fDistance = sqrtf(
    	powf((m_targetPos.x - weaponPos.x), 2) +
    	powf((m_targetPos.z - weaponPos.z), 2));

    // �v���C���[�Ƃ̍����̍��𑪂�
    float fHeight = fabsf((m_targetPos.y + FORCE_SHOT_AIM_UP) - weaponPos.y);

    // �p�x�����߂�
    float fAngleY = atan2(fDistance, fHeight);

    // �ړ��p�x�����߂�
    D3DXVECTOR3 moveAngle = D3DXVECTOR3(-sinf(rot.y), cosf(fAngleY), -cosf(rot.y));

    // �W�I�̈ʒu�̂ق����A����̈ʒu���Ⴂ�Ȃ�
    if (m_targetPos.y + FORCE_SHOT_AIM_UP < weaponPos.y)
    {
        // Y���̈ړ��p�x���t�ɂ���
        moveAngle.y *= -1;
    }

    // �e���N���G�C�g
    CBullet::Create(weaponPos, BULLET_CLAP_COLLISION_SIZE, moveAngle, CBullet::TYPE_CLAP, CBullet::TRIBE_NEUTRAL);
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CForce::Draw(void)
{
    CCharacter::Draw();
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : ��{��
//=============================================================================
HRESULT CForce::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/00_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_BODY], NULL, &m_nNumMat[PARTS_BODY], &m_pMesh[PARTS_BODY]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/01_head.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_HEAD], NULL, &m_nNumMat[PARTS_HEAD], &m_pMesh[PARTS_HEAD]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/02_right_arm.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_ARM], NULL, &m_nNumMat[PARTS_RIGHT_ARM], &m_pMesh[PARTS_RIGHT_ARM]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/03_right_hand.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_HAND], NULL, &m_nNumMat[PARTS_RIGHT_HAND], &m_pMesh[PARTS_RIGHT_HAND]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/04_left_arm.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_ARM], NULL, &m_nNumMat[PARTS_LEFT_ARM], &m_pMesh[PARTS_LEFT_ARM]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/05_left_hand.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_HAND], NULL, &m_nNumMat[PARTS_LEFT_HAND], &m_pMesh[PARTS_LEFT_HAND]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/06_right_leg.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_LEG], NULL, &m_nNumMat[PARTS_RIGHT_LEG], &m_pMesh[PARTS_RIGHT_LEG]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/07_right_foot.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RIGHT_FOOT], NULL, &m_nNumMat[PARTS_RIGHT_FOOT], &m_pMesh[PARTS_RIGHT_FOOT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/08_left_leg.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_LEG], NULL, &m_nNumMat[PARTS_LEFT_LEG], &m_pMesh[PARTS_LEFT_LEG]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/09_left_foot.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEFT_FOOT], NULL, &m_nNumMat[PARTS_LEFT_FOOT], &m_pMesh[PARTS_LEFT_FOOT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ranger/10_weapon.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_WEAPON], NULL, &m_nNumMat[PARTS_WEAPON], &m_pMesh[PARTS_WEAPON]);

    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : ��{��
//=============================================================================
void CForce::Unload(void)
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
CForce * CForce::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �������m��
    CForce *pForce = NULL;
    pForce = new CForce;

    // �e���̏���ݒ�
    pForce->SetPos(pos);
    pForce->SetRot(rot);
    pForce->SetTribe(CCharacter::TRIBE_FORCE);

    pForce->Init(pos, DEFAULT_SCALE);

    pForce->SetActiveCollisionSize(FORCE_ACTIVE_COLLISION_SIZE);
    pForce->SetLife(FORCE_LIFE);
    pForce->m_nScore = FORCE_SCORE;
    pForce->m_nOrbs = FORCE_ORBS;
    pForce->m_fSpeed = FORCE_SPEED;
    pForce->m_fDiscoveryDistance = FORCE_DISCOVERY_DISTANCE;

    pForce->SetCntCollision(FORCE_COLLISION_MAX);
    pForce->SetCollisionSize(0, FORCE_ACTIVE_COLLISION_SIZE);

    // �����蔻��̈ʒu���X�V
    pForce->UpdateCollisionPos();

    pForce->m_targetPos = CGame::GetPlayer()->GetPos();

    return pForce;
}

//=============================================================================
// �I�[�u���o���܂�����
// Author : �㓡�T�V��
//=============================================================================
void CForce::ScatterOrbs(void)
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
// �~�b�V�����N���A���ɉ��Z����
// Author : �㓡�T�V��
//=============================================================================
void CForce::AddMissionClear(void)
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

            break;

        case CGame::CURRENT_MISSION_2:
            CGame::SetClearNumMission2();
            break;
        }

        break;

    case CGame::STAGE_2:

        break;

    case CGame::STAGE_FINAL:

        break;
    }
}