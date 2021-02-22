//================================================================================
//
// ���C�o�[���̏��� (wyvern.cpp)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//================================================================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "wyvern.h"
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
#include "camera.h"
#include "sound.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
LPD3DXMESH CWyvern::m_pMesh[PARTS_MAX] = {};
LPD3DXBUFFER CWyvern::m_pBuffMat[PARTS_MAX] = {};
DWORD CWyvern::m_nNumMat[PARTS_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CWyvern::CWyvern() :CCharacter(OBJTYPE::OBJTYPE_ENEMY)
{
    m_move = DEFAULT_VECTOR;
    m_nScore = 0;
    m_nOrbs = 0;

    m_nCntTime = 0;

    m_fSpeed = 0.0f;
    m_rotDest = DEFAULT_VECTOR;
    m_rotMin = DEFAULT_VECTOR;

    m_attackAI = ATTACK_AI_ROAR;

    m_bUseRotControl = true;

    m_bStartBattle = false;

    m_nRightWingDurabirity = WYVERN_WING_DURABILITY;
    m_nLeftWingDurabirity = WYVERN_WING_DURABILITY;
    m_nTailDurabirity = WYVERN_TAIL_DURABILITY;
    m_bDestroyRightWing = false;
    m_bDestroyLeftWing = false;
    m_bDestroyTail = false;

    m_bFlySomersault = false;
    m_bPrevRoar = false;

    m_bStun = false;
    m_nStunValue = 0;
    m_nCntFlinch = 0;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CWyvern::~CWyvern()
{
}

//=============================================================================
// ����������
// Author : ��{��
//=============================================================================
HRESULT CWyvern::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �p�[�c����ݒ�
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_WYVERN, nCount, m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
    }

    // ���W�E�e�q�֌W�̓ǂݍ���
    LoadModelData("./data/ANIMATION/motion_wyvern.txt");

    // �{�X�d�l��
    CCharacter::SetBossSetting();

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::Update(void)
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

    	// �I�[�u���o���܂�
    	ScatterOrbs(m_nOrbs);

    	// �X�R�A���Z
    	CScore *pScore = CGame::GetScore();
    	pScore->AddScore(m_nScore);

    	// ���m���ŁA�񕜃A�C�e���𗎂Ƃ�
    	int nRandNum = GetRandNum(100, 1);
    	if (nRandNum <= WYVERN_DROP_HEAL_ITEM)
    	{
    		CItem::Create(pos, DEFAULT_VECTOR, CItem::TYPE_HEAL);
    	}

    	// �I������
    	Uninit();
    }
}

//=============================================================================
// �s������
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::Movement(void)
{
	// �ʒu�ƌ������擾
	D3DXVECTOR3 pos = DEFAULT_VECTOR;
	D3DXVECTOR3 rot = DEFAULT_VECTOR;
	pos = GetPos();
	rot = GetRot();

	// ���[�V�������܂��͑ҋ@�ɂ���
	GetAnimation()->SetAnimation(ANIM_IDLE);

    // ��]���䂷��Ȃ�
    if (m_bUseRotControl == true)
    {
        // ��]����
        rot = RotControl(rot);
    }

	// 1F�O�̈ʒu���L��
	SetPosOld(pos);

    // �{��Ȃ�
    if (GetLife() <= WYVERN_START_ANGRY_LIFE)
    {
        // �����A�b�v
        m_fSpeed = WYVERN_SPEED_ANGRY;
    }

    // �X�^���l����
    if (m_nStunValue >= WYVERN_STUN_VALUE_MAX)
    {
        // ���ѐ�
        CSound*pSound = CManager::GetSound();
        pSound->Play(CSound::LABEL_SE_BOSS_DEATH);

        // �X�^���l���Z�b�g
        m_nStunValue = 0;

        // �J�E���^���Z�b�g
        m_nCntTime = 0;

        // �X�^��
        m_bStun = true;

        // ���݉񐔏㏸
        m_nCntFlinch++;
    }

    // �X�^�����Ă��Ȃ��Ȃ�
    if (m_bStun == false)
    {
        // �U��AI����
        if (m_bStartBattle == true)
        {
            pos = AttackAI(pos, rot);
        }
    }
    // �X�^�����Ă���Ȃ�
    else
    {
        // �X�^�������݂��̏���
        pos = StunOrFlinch(pos, rot);
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

        // �o�g���J�n
        if (m_bStartBattle == false)
        {
            SetLandingEffect(pos);
            m_bStartBattle = true;
        }
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

            // �u���b�N�̈ʒu�Ƒ傫�����擾
            D3DXVECTOR3 blockPos = pBlock->GetPos();
            D3DXVECTOR3 blockSize = pBlock->GetCollisionSize();

            // �����蔻��̐�����
            for (int nCntCollision = 0; nCntCollision < GetCntCollision(); nCntCollision++)
            {
                // �p�[�c�̓����蔻����擾
                COLLISION partsCollision = GetCollision(nCntCollision);

                // �������Ă���Ȃ�
                if (RectangleCollision3D(&partsCollision.pos, &blockPos,
                    &partsCollision.size, &blockSize) == true)
                {
                    // �u���b�N��j��
                    pBlock->SetTakeDamage(WYVERN_CRUSH_BLOCK, CBlock::LAST_HIT_ENEMY);
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
    //CDebug::Create(pos, GetActiveCollisionSize(), CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// �����蔻��̈ʒu���X�V
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::UpdateCollisionPos(void)
{
	// ���̔��̈ʒu�擾
	D3DXVECTOR3 headCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_HEAD)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_CUBE_HEAD)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_CUBE_HEAD)->GetWorldMtx()._43);

	// ���̈ʒu�擾
	D3DXVECTOR3 headPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._43);

	// �̂̈ʒu�擾
	D3DXVECTOR3 bodyPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._43);

	// �E��̈ʒu�擾
	D3DXVECTOR3 rightHandPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_ARM_RIGHT)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_CUBE_ARM_RIGHT)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_CUBE_ARM_RIGHT)->GetWorldMtx()._43);

	// ����̈ʒu�擾
	D3DXVECTOR3 leftHandPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_ARM_LEFT)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_CUBE_ARM_LEFT)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_CUBE_ARM_LEFT)->GetWorldMtx()._43);

    // �E���̈ʒu�擾
    D3DXVECTOR3 footRightPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_FOOT_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_FOOT_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_FOOT_RIGHT)->GetWorldMtx()._43);

    // �����̈ʒu�擾
    D3DXVECTOR3 footLeftPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_FOOT_LEFT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_FOOT_LEFT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_FOOT_LEFT)->GetWorldMtx()._43);

    // �E���̔��̈ʒu�擾
    D3DXVECTOR3 wingRightCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_WING_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_WING_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_WING_RIGHT)->GetWorldMtx()._43);

    // �����̈ʒu�擾
    D3DXVECTOR3 wingLeftPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._43);

    // �E���̈ʒu�擾
    D3DXVECTOR3 wingRightPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._43);

    // �����̔��̈ʒu�擾
    D3DXVECTOR3 wingLeftCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_WING_LEFT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_WING_LEFT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_WING_LEFT)->GetWorldMtx()._43);

    // �K���̍��̈ʒu�擾
    D3DXVECTOR3 tailRootPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_TAIL_ROOT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_TAIL_ROOT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_TAIL_ROOT)->GetWorldMtx()._43);

    // �K���̒��Ԃ̈ʒu�擾
    D3DXVECTOR3 tailTipPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._43);

    // �K���̔��̈ʒu�擾
    D3DXVECTOR3 tailCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_TAIL)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_TAIL)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_TAIL)->GetWorldMtx()._43);

    // �E�I�̈ʒu�擾
    D3DXVECTOR3 rightElbowPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_ELBOW_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_ELBOW_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_ELBOW_RIGHT)->GetWorldMtx()._43);

    // ���I�̈ʒu�擾
    D3DXVECTOR3 leftElbowPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_ELBOW_LEFT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_ELBOW_LEFT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_ELBOW_LEFT)->GetWorldMtx()._43);

    // �E���̔��̈ʒu�擾
    D3DXVECTOR3 footRightCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_FOOT_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_FOOT_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_FOOT_RIGHT)->GetWorldMtx()._43);

    // �����̔��̈ʒu�擾
    D3DXVECTOR3 footLeftCubePos = D3DXVECTOR3(GetParts(CWyvern::PARTS_CUBE_FOOT_LEFT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_CUBE_FOOT_LEFT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_CUBE_FOOT_LEFT)->GetWorldMtx()._43);

	// �����蔻��̈ʒu���X�V
    SetCollisionPos(COLLISION_PARTS_HEAD_CUBE, headCubePos);
    SetCollisionPos(COLLISION_PARTS_HEAD, headPos);
    SetCollisionPos(COLLISION_PARTS_BODY, bodyPos);
    SetCollisionPos(COLLISION_PARTS_ARM_RIGHT, rightHandPos);
    SetCollisionPos(COLLISION_PARTS_ARM_LEFT, leftHandPos);
    SetCollisionPos(COLLISION_PARTS_FOOT_RIGHT, footRightPos);
    SetCollisionPos(COLLISION_PARTS_FOOT_LEFT, footLeftPos);
    SetCollisionPos(COLLISION_PARTS_WING_RIGHT, wingRightCubePos);
    SetCollisionPos(COLLISION_PARTS_WING_LEFT, wingLeftCubePos);
    SetCollisionPos(COLLISION_PARTS_TAIL_ROOT, tailRootPos);
    SetCollisionPos(COLLISION_PARTS_TAIL_TIP, tailTipPos);
    SetCollisionPos(COLLISION_PARTS_TAIL_CUBE, tailCubePos);

    SetCollisionPos(COLLISION_PARTS_ELBOW_RIGHT, rightElbowPos);
    SetCollisionPos(COLLISION_PARTS_ELBOW_LEFT, leftElbowPos);
    SetCollisionPos(COLLISION_PARTS_FOOT_CUBE_RIGHT, footRightCubePos);
    SetCollisionPos(COLLISION_PARTS_FOOT_CUBE_LEFT, footLeftCubePos);

    // ���ʔj�󏈗�
    CScore *pScore = CGame::GetScore();

    // �K��
    if (m_bDestroyTail == false)
    {
        // �ϋv�l���Ȃ��Ȃ�
        if (m_nTailDurabirity <= 0)
        {
            // �j��̃t���O��true��
            m_bDestroyTail = true;

            // �X�R�A���Z
            pScore->AddScore(WYVERN_TAIL_SCORE);

            // �X�P�[���Ɠ����蔻����Ȃ���
            GetParts(CWyvern::PARTS_TAIL_TIP)->SetScale(DEFAULT_VECTOR);
            SetCollisionSize(COLLISION_PARTS_TAIL_TIP, DEFAULT_VECTOR);

            // �G�t�F�N�g�̔���
            SetDeathCrushEffect(tailTipPos, GetRot(), CModelEffect::TYPE_TAIL);

            // �I�[�u���o���܂�
            ScatterOrbs(WYVERN_TAIL_ORBS);

            // �X�^�����Ă��Ȃ��Ȃ�
            if (m_bStun == false)
            {
                // �m��X�^��
                m_nStunValue += WYVERN_STUN_VALUE_MAX;
            }
        }
    }
    // ����
    if (m_bDestroyLeftWing == false)
    {
        // �ϋv�l���Ȃ��Ȃ�
        if (m_nLeftWingDurabirity <= 0)
        {
            // �j��̃t���O��true��
            m_bDestroyLeftWing = true;

            // �X�R�A���Z
            pScore->AddScore(WYVERN_WING_SCORE);

            // �X�P�[���Ɠ����蔻����Ȃ���
            GetParts(CWyvern::PARTS_WING_LEFT)->SetScale(DEFAULT_VECTOR);
            SetCollisionSize(COLLISION_PARTS_WING_LEFT, DEFAULT_VECTOR);

            // �G�t�F�N�g�̔���
            SetDeathCrushEffect(wingLeftPos, GetRot(), CModelEffect::TYPE_LEFT_WING);

            // �I�[�u���o���܂�
            ScatterOrbs(WYVERN_WING_ORBS);

            // �X�^�����Ă��Ȃ��Ȃ�
            if (m_bStun == false)
            {
                // �m��X�^��
                m_nStunValue += WYVERN_STUN_VALUE_MAX;
            }
        }
    }
    // �E��
    if (m_bDestroyRightWing == false)
    {
        // �ϋv�l���Ȃ��Ȃ�
        if (m_nRightWingDurabirity <= 0)
        {
            // �j��̃t���O��true��
            m_bDestroyRightWing = true;

            // �X�R�A���Z
            pScore->AddScore(WYVERN_WING_SCORE);

            // �X�P�[���Ɠ����蔻����Ȃ���
            GetParts(CWyvern::PARTS_WING_RIGHT)->SetScale(DEFAULT_VECTOR);
            SetCollisionSize(COLLISION_PARTS_WING_RIGHT, DEFAULT_VECTOR);

            // �G�t�F�N�g�̔���
            SetDeathCrushEffect(wingRightPos, GetRot(), CModelEffect::TYPE_RIGHT_WING);

            // �I�[�u���o���܂�
            ScatterOrbs(WYVERN_WING_ORBS);

            // �X�^�����Ă��Ȃ��Ȃ�
            if (m_bStun == false)
            {
                // �m��X�^��
                m_nStunValue += WYVERN_STUN_VALUE_MAX;
            }
        }
    }
}

//=============================================================================
// ��]����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CWyvern::RotControl(D3DXVECTOR3 rot)
{
    // �Ԃ���]��
    D3DXVECTOR3 returnRot = rot;

    // �v���C���[�̈ʒu�����߂�
    D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

    // �v���C���[�ւ̊p�x�����߂�
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
        returnRot.y += (m_rotDest.y - returnRot.y) * WYVERN_TURN_SPEED;
    }

    return returnRot;
}

//=============================================================================
// �X�^�������݂��̏���
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CWyvern::StunOrFlinch(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{        
    // �Ԃ��ʒu�����߂�
    D3DXVECTOR3 returnPos = pos;

    // ��]���䂵�Ȃ�
    m_bUseRotControl = false;

    // �J�E���^�A�b�v
    m_nCntTime++;

    // ���݉񐔂����ȏ�ŃX�^��
    if (m_nCntFlinch >= WYVERN_FLINCH_MAX)
    {
        // �A�j���[�V�������X�^����
        GetAnimation()->SetAnimation(ANIM_STUN);

        // ���J�E���^��
        if (m_nCntTime >= WYVERN_STUN_WHOLE_FRAME)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // �ҋ@���ɂ���
            m_attackAI = ATTACK_AI_WAIT;

            // �X�^������
            m_bStun = false;

            // ���݉񐔃��Z�b�g
            m_nCntFlinch = 0;
        }
        else if (m_nCntTime == WYVERN_STUN_CREATE_FRAME)
        {
            // �X�^�����ɃG�t�F�N�g����
            SetLandingEffect(pos);
        }
    }
    else
    {
        // �A�j���[�V���������݂�
        GetAnimation()->SetAnimation(ANIM_FLINCH);

        // ���J�E���^��
        if (m_nCntTime >= WYVERN_FLINCH_WHOLE_FRAME)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // �ҋ@���ɂ���
            m_attackAI = ATTACK_AI_WAIT;

            // �X�^������
            m_bStun = false;
        }
        else if (m_nCntTime <= WYVERN_FLINCH_KNOCKBACK_END_FRAME)
        {
            // ���Ă�����̋t�ɐi��
            D3DXVECTOR3 move = D3DXVECTOR3(sinf(rot.y) * WYVERN_FLINCH_KNOCKBACK_VALUE, 0.0f, cosf(rot.y) * WYVERN_FLINCH_KNOCKBACK_VALUE);
            returnPos += move;
        }
    }

    return returnPos;
}

//=============================================================================
// �U��AI����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CWyvern::AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �Ԃ��ʒu�A�ړ��ʂ����߂�
    D3DXVECTOR3 returnPos = pos;
    D3DXVECTOR3 move = DEFAULT_VECTOR;

    // �����擾
    CSound*pSound = CManager::GetSound();

    switch (m_attackAI)
    {
    case ATTACK_AI_WAIT:

        // ��]���䂷��
        m_bUseRotControl = true;

        // �󒆃T�}�[�\���g�̃t���O��false��
        m_bFlySomersault = false;

        // �J�E���^�A�b�v
        m_nCntTime++;

        // ���J�E���^��
        if (m_nCntTime >= WYVERN_NEXT_ATTACK_COUNT)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // ���̍U�������߂�
            NextAttackSelect(pos);
        }

        break;

    case ATTACK_AI_TACKLE:

        // ���K���Ă��Ȃ�
        m_bPrevRoar = false;

        // ��]���䂵�Ȃ�
        m_bUseRotControl = false;

        // �J�E���^�A�b�v
        m_nCntTime++;

        // �A�j���[�V�����𑖂��
        GetAnimation()->SetAnimation(ANIM_WALK);

        // ����
        returnPos = Dush(returnPos, rot);

        // ���J�E���^��
        if (m_nCntTime >= WYVERN_TACKLE_WHOLE_FRAME)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // �ҋ@���ɂ���
            m_attackAI = ATTACK_AI_WAIT;
        }

        break;

    case ATTACK_AI_FLY:

        // ���K���Ă��Ȃ�
        m_bPrevRoar = false;

        // ��]���䂷��
        m_bUseRotControl = true;

        // �J�E���^�A�b�v
        m_nCntTime++;

        // �A�j���[�V�������H�΂�����
        GetAnimation()->SetAnimation(ANIM_FLY);

        // ����
        returnPos = Dush(returnPos, rot);

        // ���J�E���^��
        if (m_nCntTime >= WYVERN_FLY_WHOLE_FRAME)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // ���P�ɂ���
            m_attackAI = ATTACK_AI_ASSAULT;
        }

        break;

    case ATTACK_AI_ASSAULT:

        // ���K���Ă��Ȃ�
        m_bPrevRoar = false;

        // ��]���䂵�Ȃ�
        m_bUseRotControl = false;

        // �J�E���^�A�b�v
        m_nCntTime++;

        // �A�j���[�V���������P��
        GetAnimation()->SetAnimation(ANIM_ASSAULT);

        // ����
        returnPos = Dush(returnPos, rot);

        // ���J�E���^��
        if (m_nCntTime >= WYVERN_ASSULT_WHOLE_FRAME)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // �ҋ@���ɂ���
            m_attackAI = ATTACK_AI_WAIT;
        }
        else if (m_nCntTime == WYVERN_ASSULT_CREATE_FRAME_1)
        {
            // ���؂艹
            pSound->Play(CSound::LABEL_SE_BOSS_WIND);

            // ��������
            SetExplosion(false, true);
        }
        else if (m_nCntTime == WYVERN_ASSULT_CREATE_FRAME_2)
        {
            // ��������
            SetExplosion(false, true);
        }
        else if (m_nCntTime == WYVERN_ASSULT_CREATE_FRAME_3)
        {
            // ��������
            SetExplosion(false, true);
        }

        break;

    case ATTACK_AI_SOMERSAULT:

        // ���K���Ă��Ȃ�
        m_bPrevRoar = false;

        // ��]���䂵�Ȃ�
        m_bUseRotControl = false;

        // �J�E���^�A�b�v
        m_nCntTime++;

        // �A�j���[�V�������T�}�[�\���g��
        GetAnimation()->SetAnimation(ANIM_SOMERSAULT);

        // ���J�E���^��
        if (m_nCntTime >= WYVERN_SOMERSAULT_WHOLE_FRAME)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // �h������̗͂ɂȂ�
            if (GetLife() <= WYVERN_SOMERSAULT_DERIVE_FLY_LIFE)
            {
                // �H������Ȃ�
                if (m_bDestroyLeftWing == false || m_bDestroyRightWing == false)
                {
                    // ��s���ɂ���
                    m_attackAI = ATTACK_AI_FLY;
                }
                else
                {
                    // �ҋ@���ɂ���
                    m_attackAI = ATTACK_AI_WAIT;
                }
            }
            else
            {
                // �ҋ@���ɂ���
                m_attackAI = ATTACK_AI_WAIT;
            }
        }
        // �N���G�C�g�t���[��
        else if (m_nCntTime == WYVERN_SOMERSALUT_CREATE_FRAME)
        {
            // �K��������Ȃ�
            if (m_bDestroyTail == false)
            {
                // �唚����
                pSound->Play(CSound::LABEL_SE_EXPLOSION_BIG);

                // �Ռ��g�U��
                SetShockWave(pos, rot);
            }
        }

        break;

    case ATTACK_AI_EXPLOSION:

        // ���K���Ă��Ȃ�
        m_bPrevRoar = false;

        // ��]���䂵�Ȃ�
        m_bUseRotControl = false;

        // �J�E���^�A�b�v
        m_nCntTime++;

        // �A�j���[�V�����𔚍ӂ�
        GetAnimation()->SetAnimation(ANIM_EXPLOSION);

        // �N���G�C�g�X�^�[�g�t���[��
        if (m_nCntTime == WYVERN_EXPLOSION_CREATE_START_FRAME)
        {
            // �@�����鉹
            pSound->Play(CSound::LABEL_SE_DESTROY_METAL);

            // �v���C���[�̉��ɐ���
            SetExplosion(true, false);
        }
        // �X�^�[�g�ȍ~
        else if (m_nCntTime > WYVERN_EXPLOSION_CREATE_START_FRAME)
        {
            // �N���G�C�g�t���[��
            if (m_nCntTime % WYVERN_EXPLOSION_CREATE_FRAME == 0)
            {
                SetExplosion(false, false);
            }
        }

        // ���J�E���^��
        if (m_nCntTime >= WYVERN_EXPLOSION_WHOLE_FRAME)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // �ҋ@���ɂ���
            m_attackAI = ATTACK_AI_WAIT;
        }

        break;

    case ATTACK_AI_ROAR:

        // ���K����
        m_bPrevRoar = true;

        // ��]���䂵�Ȃ�
        m_bUseRotControl = false;

        // �J�E���^�A�b�v
        m_nCntTime++;

        // �A�j���[�V��������K��
        GetAnimation()->SetAnimation(ANIM_ROAR);

        // �N���G�C�g�t���[��
        if (m_nCntTime == WYVERN_ROAR_CREATE_FRAME)
        {
            // ���K��
            pSound->Play(CSound::LABEL_SE_BOSS_ROAR);

            // �G����
            SetEnemySpawn(pos, rot);
        }

        // ���J�E���^��
        if (m_nCntTime >= WYVERN_ROAR_WHOLE_FRAME)
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
// ���̍U�������߂�
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::NextAttackSelect(D3DXVECTOR3 pos)
{
    // �v���C���[�̈ʒu���擾
    D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

    // �v���C���[�Ƃ̋����𑪂�
    float fDistance = sqrtf(
        powf((playerPos.x - pos.x), 2) +
        powf((playerPos.z - pos.z), 2));

    // �����_���ɐ��������߂�
    int nRandNum = GetRandNum(100, 1);

    // ���̍U�����̗p���ꂽ���A�����ɂ���Č��߂�
    if (nRandNum >= 1 && nRandNum <= WYVERN_FIRST_PRIORITY)
    {
        // �ߋ���
        if (fDistance <= WYVERN_CONSIDER_AS_SHORT)
        {
            m_attackAI = ATTACK_AI_SOMERSAULT;
        }
        // ������
        else if (fDistance > WYVERN_CONSIDER_AS_SHORT && fDistance <= WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_EXPLOSION;
        }
        // ������
        else if (fDistance > WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_SOMERSAULT;
        }
    }
    else if (nRandNum > WYVERN_FIRST_PRIORITY && nRandNum <= WYVERN_FIRST_PRIORITY + WYVERN_SECOND_PRIORITY)
    {
        // �ߋ���
        if (fDistance <= WYVERN_CONSIDER_AS_SHORT)
        {
            m_attackAI = ATTACK_AI_EXPLOSION;
        }
        // ������
        else if (fDistance > WYVERN_CONSIDER_AS_SHORT && fDistance <= WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_SOMERSAULT;
        }
        // ������
        else if (fDistance > WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_TACKLE;
        }
    }
    else if (nRandNum > WYVERN_FIRST_PRIORITY + WYVERN_SECOND_PRIORITY
        && nRandNum <= WYVERN_FIRST_PRIORITY + WYVERN_SECOND_PRIORITY + WYVERN_THIRD_PRIORITY)
    {
        // �ߋ���
        if (fDistance <= WYVERN_CONSIDER_AS_SHORT)
        {
            m_attackAI = ATTACK_AI_TACKLE;
        }
        // ������
        else if (fDistance > WYVERN_CONSIDER_AS_SHORT && fDistance <= WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_TACKLE;
        }
        // ������
        else if (fDistance > WYVERN_CONSIDER_AS_MEDIUM)
        {
            m_attackAI = ATTACK_AI_EXPLOSION;
        }
    }
    else if (nRandNum > WYVERN_FIRST_PRIORITY + WYVERN_SECOND_PRIORITY + WYVERN_THIRD_PRIORITY
        && nRandNum <= WYVERN_FIRST_PRIORITY + WYVERN_SECOND_PRIORITY + WYVERN_THIRD_PRIORITY + +WYVERN_FOURTH_PRIORITY)
    {
        // �ЂƂO�̍U�������K�łȂ��Ȃ�
        if (m_bPrevRoar == false)
        {
            m_attackAI = ATTACK_AI_ROAR;
        }
        // ���K�Ȃ��ԗD�揇�ʂ̍����U����
        else
        {
            // �ߋ���
            if (fDistance <= WYVERN_CONSIDER_AS_SHORT)
            {
                m_attackAI = ATTACK_AI_SOMERSAULT;
            }
            // ������
            else if (fDistance > WYVERN_CONSIDER_AS_SHORT && fDistance <= WYVERN_CONSIDER_AS_MEDIUM)
            {
                m_attackAI = ATTACK_AI_EXPLOSION;
            }
            // ������
            else if (fDistance > WYVERN_CONSIDER_AS_MEDIUM)
            {
                m_attackAI = ATTACK_AI_SOMERSAULT;
            }
        }
    }
}

//=============================================================================
// ���鏈��
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CWyvern::Dush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    D3DXVECTOR3 returnPos = pos;

    // ���Ă�������ɐi��
    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(rot.y) * m_fSpeed, 0.0f, -cosf(rot.y) * m_fSpeed);
    if (m_attackAI == ATTACK_AI_FLY)
    {
        // ��s���Ȃ猸��
        move *= WYVERN_FLY_DOWN_SPEED_RATE;
    }
    else if (m_attackAI == ATTACK_AI_ASSAULT)
    {
        if (m_nCntTime >= WYVERN_ASSULT_UP_SPEED_FRAME)
        {
            // ���P���̈��t���[���������
            move *= WYVERN_ASSULT_UP_SPEED_RATE;
        }
    }
    returnPos += move;

    // �ːi���A�܂��͔�s���ŃT�}�[�\���g�܂ł̃N�[���^�C�����Ȃ��A�A���łȂ��Ȃ�
    if (m_attackAI == ATTACK_AI_TACKLE ||
        m_attackAI == ATTACK_AI_FLY && m_nCntTime >= WYVERN_FLY_START_SOMERSAULT_FRAME && m_bFlySomersault == false)
    {
        // �v���C���[�̈ʒu�����߂�
        D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

        // �ڕW�܂ł̋���
        float fDistance = sqrtf(powf((playerPos.x - returnPos.x), 2) + powf((playerPos.z - returnPos.z), 2));

        // �ߋ����ɂ���Ȃ�
        if (fDistance <= WYVERN_CONSIDER_AS_SHORT)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            // ���ł���Ȃ�
            if (m_attackAI == ATTACK_AI_FLY)
            {
                // �󒆃T�}�[�\���g�̃t���O��true��
                m_bFlySomersault = true;

                // �����ɃT�}�[�\���g����
                m_attackAI = ATTACK_AI_SOMERSAULT;
            }
            // �n��Ȃ�
            else
            {
                // �T�}�[�\���g������
                int nRandNum = GetRandNum(2, 1);
                switch (nRandNum)
                {
                case 1:
                    // �T�}�[�\���g
                    m_attackAI = ATTACK_AI_SOMERSAULT;
                    break;
                case 2:
                    // ����
                    m_attackAI = ATTACK_AI_EXPLOSION;
                    break;
                }
            }
        }
    }

    return returnPos;
}

//=============================================================================
// �Ռ��g�U��
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::SetShockWave(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // ������΂�
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR;
    float fHorizontalAngle = 0.0f;
    for (int nCnt = 0; nCnt < WYVERN_SOMERSAULT_SHOCK_NUMBER_ONCE; nCnt++)
    {
        // ���Ɋg�U������
        if (nCnt == 1)
        {
            fHorizontalAngle = WYVERN_SOMERSAULT_SHOCK_ANGLE_XZ;
        }
        else if (nCnt == 2)
        {
            fHorizontalAngle = -WYVERN_SOMERSAULT_SHOCK_ANGLE_XZ;
        }

        // �ړ��p�x�����߂�
        moveAngle = D3DXVECTOR3(-sinf(rot.y + fHorizontalAngle), 0.0f, -cosf(rot.y + fHorizontalAngle));

        // �e���N���G�C�g
        CBullet::Create(pos, BULLET_SHOCK_COLLISION_SIZE, moveAngle, CBullet::TYPE_SHOCK, CBullet::TRIBE_ENEMY);
    }
}

//=============================================================================
// ���ӍU��
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::SetExplosion(bool bUseTracking, bool bFall)
{
    // �̂̈ʒu�擾
    D3DXVECTOR3 bodyPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._43);

    // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
    float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

    // �ݒu����ʒu�̔��a���o��
    float fRadius = float(GetRandNum(WYVERN_EXPLOSION_MAX_RADIUS, WYVERN_EXPLOSION_MIN_RADIUS));

    // �����Ȃ�A���a��0��
    if (bFall == true)
    {
        fRadius = 0.0f;
    }

    // �ʒu�����߂�(Y�����������������ʒu�ŌŒ�)
    D3DXVECTOR3 effectPos = DEFAULT_VECTOR;
    if (bUseTracking == false)
    {
        // �Ǐ]����
        effectPos = D3DXVECTOR3(-sinf(fAngle)*fRadius + bodyPos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, -cosf(fAngle)*fRadius + bodyPos.z);
    }
    else
    {
        // �Ǐ]�A��
        D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
        effectPos = D3DXVECTOR3(playerPos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, playerPos.z);
    }

    // ���ӂ̒��S�𔭐�
    CEffect3D::Create(effectPos, EFFECT_BURST_CENTER_FIRST_SIZE,
        DEFAULT_VECTOR, EFFECT_BURST_CENTER_FIRST_COLOR, HIT_TOP, EFFECT_BURST_CENTER_FADE_OUT_RATE, CEffect3D::TYPE_BURST_CENTER);
}

//=============================================================================
// �G�����U��
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::SetEnemySpawn(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �G�̑������擾
    int nNumAllEnemy = CScene::GetNumAll(CScene::OBJTYPE_ENEMY);

    // �G�𐶐��ł��鑍���Ȃ�
    if (nNumAllEnemy <= WYVERN_ROAR_SET_ENEMY_COUNT_BORDER)
    {
        // �p�x�����߁A�����ɕ����̐���
        float fAngle = rot.y;
        for (int nCnt = 0; nCnt < WYVERN_ROAR_SET_ENEMY_ONCE; nCnt++)
        {
            // �G�t�F�N�g�p�̕ϐ��錾
            float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;

            // ��]�ʂ����߂�
            D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

            // �ʒu�����߂�
            D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*WYVERN_ROAR_SET_ENEMY_RADIUS,
                WYVERN_ROAR_SET_ENEMY_SPAWN_POS_Y, -cosf(fAngle)*WYVERN_ROAR_SET_ENEMY_RADIUS) + GetPos();

            // �G�����̃G�t�F�N�g����
            CModelEffect::Create(effectPos, rotMove, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_KAMI);

            // �p�x�𑫂��A�~�󂩂ϓ��ɔz�u
            fAngle += D3DXToRadian(360.0f / (float)WYVERN_ROAR_SET_ENEMY_ONCE);
        }
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::Draw(void)
{
    CCharacter::Draw();
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : ��{��
//=============================================================================
HRESULT CWyvern::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/00_weist.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_WEIST], NULL, &m_nNumMat[PARTS_WEIST], &m_pMesh[PARTS_WEIST]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/01_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_BODY], NULL, &m_nNumMat[PARTS_BODY], &m_pMesh[PARTS_BODY]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/02_neck.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_NECK], NULL, &m_nNumMat[PARTS_NECK], &m_pMesh[PARTS_NECK]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/03_head.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_HEAD], NULL, &m_nNumMat[PARTS_HEAD], &m_pMesh[PARTS_HEAD]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/04_tail_root.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_TAIL_ROOT], NULL, &m_nNumMat[PARTS_TAIL_ROOT], &m_pMesh[PARTS_TAIL_ROOT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/05_tail_tip.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_TAIL_TIP], NULL, &m_nNumMat[PARTS_TAIL_TIP], &m_pMesh[PARTS_TAIL_TIP]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/06_arm_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_ARM_RIGHT], NULL, &m_nNumMat[PARTS_ARM_RIGHT], &m_pMesh[PARTS_ARM_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/07_arm_left.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_ARM_LEFT], NULL, &m_nNumMat[PARTS_ARM_LEFT], &m_pMesh[PARTS_ARM_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/08_wing_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_WING_RIGHT], NULL, &m_nNumMat[PARTS_WING_RIGHT], &m_pMesh[PARTS_WING_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/09_wing_left.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_WING_LEFT], NULL, &m_nNumMat[PARTS_WING_LEFT], &m_pMesh[PARTS_WING_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/10_leg_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEG_RIGHT], NULL, &m_nNumMat[PARTS_LEG_RIGHT], &m_pMesh[PARTS_LEG_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/11_foot_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_FOOT_RIGHT], NULL, &m_nNumMat[PARTS_FOOT_RIGHT], &m_pMesh[PARTS_FOOT_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/12_leg_left.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LEG_LEFT], NULL, &m_nNumMat[PARTS_LEG_LEFT], &m_pMesh[PARTS_LEG_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/13_foot_left.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_FOOT_LEFT], NULL, &m_nNumMat[PARTS_FOOT_LEFT], &m_pMesh[PARTS_FOOT_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_HEAD], NULL, &m_nNumMat[PARTS_CUBE_HEAD], &m_pMesh[PARTS_CUBE_HEAD]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_TAIL], NULL, &m_nNumMat[PARTS_CUBE_TAIL], &m_pMesh[PARTS_CUBE_TAIL]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_ARM_RIGHT], NULL, &m_nNumMat[PARTS_CUBE_ARM_RIGHT], &m_pMesh[PARTS_CUBE_ARM_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_ARM_LEFT], NULL, &m_nNumMat[PARTS_CUBE_ARM_LEFT], &m_pMesh[PARTS_CUBE_ARM_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_WING_RIGHT], NULL, &m_nNumMat[PARTS_CUBE_WING_RIGHT], &m_pMesh[PARTS_CUBE_WING_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_WING_LEFT], NULL, &m_nNumMat[PARTS_CUBE_WING_LEFT], &m_pMesh[PARTS_CUBE_WING_LEFT]);

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_ELBOW_RIGHT], NULL, &m_nNumMat[PARTS_CUBE_ELBOW_RIGHT], &m_pMesh[PARTS_CUBE_ELBOW_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_ELBOW_LEFT], NULL, &m_nNumMat[PARTS_CUBE_ELBOW_LEFT], &m_pMesh[PARTS_CUBE_ELBOW_LEFT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_FOOT_RIGHT], NULL, &m_nNumMat[PARTS_CUBE_FOOT_RIGHT], &m_pMesh[PARTS_CUBE_FOOT_RIGHT]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CUBE_FOOT_LEFT], NULL, &m_nNumMat[PARTS_CUBE_FOOT_LEFT], &m_pMesh[PARTS_CUBE_FOOT_LEFT]);

    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : ��{��
//=============================================================================
void CWyvern::Unload(void)
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
CWyvern * CWyvern::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �������m��
    CWyvern *pWyvern = NULL;
    pWyvern = new CWyvern;

    // �e���̏���ݒ�
    pWyvern->SetPos(pos);
    pWyvern->SetRot(rot);
    pWyvern->SetTribe(CCharacter::TRIBE_WYVERN);

    // ������
    pWyvern->Init(pos, DEFAULT_SCALE);

    pWyvern->SetActiveCollisionSize(WYVERN_ACTIVE_COLLISION_SIZE);
    pWyvern->SetLife(WYVERN_LIFE_MAX);
    pWyvern->m_nScore = WYVERN_SCORE;
    pWyvern->m_nOrbs = WYVERN_ORBS;
    pWyvern->m_fSpeed = WYVERN_SPEED;

    pWyvern->SetCntCollision(COLLISION_PARTS_MAX);
    pWyvern->SetCollisionSize(COLLISION_PARTS_HEAD_CUBE,       WYVERN_HEAD_CUBE_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_HEAD,            WYVERN_HEAD_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_BODY,            WYVERN_BODY_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_ARM_RIGHT,       WYVERN_ARM_RIGHT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_ARM_LEFT,        WYVERN_ARM_LEFT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_FOOT_RIGHT,      WYVERN_FOOT_RIGHT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_FOOT_LEFT,       WYVERN_FOOT_LEFT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_WING_RIGHT,      WYVERN_WING_RIGHT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_WING_LEFT,       WYVERN_WING_LEFT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_TAIL_ROOT,       WYVERN_TAIL_ROOT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_TAIL_TIP,        WYVERN_TAIL_TIP_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_TAIL_CUBE,       WYVERN_TAIL_CUBE_COLLISION_SIZE);

    pWyvern->SetCollisionSize(COLLISION_PARTS_ELBOW_RIGHT,     WYVERN_ELBOW_RIGHT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_ELBOW_LEFT,      WYVERN_ELBOW_LEFT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_FOOT_CUBE_RIGHT, WYVERN_FOOT_CUBE_RIGHT_COLLISION_SIZE);
    pWyvern->SetCollisionSize(COLLISION_PARTS_FOOT_CUBE_LEFT,  WYVERN_FOOT_CUBE_LEFT_COLLISION_SIZE);

    // �����蔻��̈ʒu���X�V
    pWyvern->UpdateCollisionPos();

    return pWyvern;
}

//=============================================================================
// ���n�G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::SetLandingEffect(D3DXVECTOR3 pos)
{
    // �����擾
    CSound*pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_LAND);

    // �������K���L���΂�
    for (int nCnt = 0; nCnt < WYVERN_LANDING_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
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
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*WYVERN_LANDING_CRACK_RADIUS,
            0.0f, -cosf(fAngle)*WYVERN_LANDING_CRACK_RADIUS) + pos;

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
// �I�[�u���o���܂�����
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::ScatterOrbs(int nOrbs)
{
    // �ʒu���擾
    D3DXVECTOR3 pos = GetPos();

    // �I�[�u��U�G�t�F�N�g
    float fSpeedX = 0.0f;   //����X
    float fSpeedY = 0.0f;   //����Y
    float fSpeedZ = 0.0f;   //����Z
    int nPlusMinusX = 0;    //����X
    int nPlusMinusZ = 0;    //����Z
    for (int nCnt = 0; nCnt < nOrbs; nCnt++)
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
void CWyvern::DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // ���ѐ�
    CSound*pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_BOSS_DEATH);

	// �p�[�c�̈ʒu�ƁA���f���G�t�F�N�g�̎��
	D3DXVECTOR3 partsPos = DEFAULT_VECTOR;
	CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_WYVERN_BODY;

	//==============================
	// ��
	//==============================

    // �G�t�F�N�g�̎��
    modelEffectType = CModelEffect::TYPE_WYVERN_BODY;

	// �ʒu
	partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_BODY)->GetWorldMtx()._43);

	// �G�t�F�N�g�̔���
	SetDeathCrushEffect(partsPos, rot, modelEffectType);

	//==============================
	// ��
	//==============================

    // �G�t�F�N�g�̎��
    modelEffectType = CModelEffect::TYPE_WYVERN_HEAD;

	// �ʒu
	partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._41,
		GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._42,
		GetParts(CWyvern::PARTS_HEAD)->GetWorldMtx()._43);

	// �G�t�F�N�g�̔���
	SetDeathCrushEffect(partsPos, rot, modelEffectType);

	//==============================
	// ��
	//==============================
    // �G�t�F�N�g�̎��
    modelEffectType = CModelEffect::TYPE_WYVERN_NECK;

    // �ʒu
    partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_NECK)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_NECK)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_NECK)->GetWorldMtx()._43);

    // �G�t�F�N�g�̔���
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // �r
    //==============================
    // �G�t�F�N�g�̎��
    modelEffectType = CModelEffect::TYPE_WYVERN_ARM;

    // �ʒu
    partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_ARM_RIGHT)->GetWorldMtx()._41,
        GetParts(CWyvern::PARTS_ARM_RIGHT)->GetWorldMtx()._42,
        GetParts(CWyvern::PARTS_ARM_RIGHT)->GetWorldMtx()._43);

    // �G�t�F�N�g�̔���
    SetDeathCrushEffect(partsPos, rot, modelEffectType);

    //==============================
    // �E��
    //==============================
    if (m_bDestroyRightWing == false)
    {
        // �G�t�F�N�g�̎��
        modelEffectType = CModelEffect::TYPE_RIGHT_WING;

        // �ʒu
        partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._41,
            GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._42,
            GetParts(CWyvern::PARTS_WING_RIGHT)->GetWorldMtx()._43);

        // �G�t�F�N�g�̔���
        SetDeathCrushEffect(partsPos, rot, modelEffectType);
    }

    //==============================
    // ����
    //==============================
    if (m_bDestroyLeftWing == false)
    {
        // �G�t�F�N�g�̎��
        modelEffectType = CModelEffect::TYPE_LEFT_WING;

        // �ʒu
        partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._41,
            GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._42,
            GetParts(CWyvern::PARTS_WING_LEFT)->GetWorldMtx()._43);

        // �G�t�F�N�g�̔���
        SetDeathCrushEffect(partsPos, rot, modelEffectType);
    }

    //==============================
    // �K��
    //==============================
    if (m_bDestroyTail == false)
    {
        // �G�t�F�N�g�̎��
        modelEffectType = CModelEffect::TYPE_TAIL;

        // �ʒu
        partsPos = D3DXVECTOR3(GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._41,
            GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._42,
            GetParts(CWyvern::PARTS_TAIL_TIP)->GetWorldMtx()._43);

        // �G�t�F�N�g�̔���
        SetDeathCrushEffect(partsPos, rot, modelEffectType);
    }
}

//=============================================================================
// ���ꂽ���̕��ӃG�t�F�N�g�̔���
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type)
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
    fSpeedY = float(GetRandNum(MODEL_EFFECT_WYVERN_SPEED_Y_MAX, MODEL_EFFECT_WYVERN_SPEED_Y_MIN)) / 100.0f;
	fSpeedZ = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Z, 0)) / 100.0f;
	fRotSpeedX = float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f;
	fRotSpeedY = float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f;
	fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_WYVERN_ROT_MAX, MODEL_EFFECT_WYVERN_ROT_MIN)) / 100.0f;
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
// �U�����󂯂����ʂɂ���āA�󂯂�_���[�W��ς���
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::TakeDamage(D3DXVECTOR3 pos, int nDamage, int nCntCollision)
{
    // ���ۂ̃_���[�W�����߂�
    float fRealDamage = (float)nDamage;

    // ��ʂɓ��������Ȃ�
    if (nCntCollision == COLLISION_PARTS_HEAD_CUBE)
    {
        // �����擾
        CSound*pSound = CManager::GetSound();
        pSound->Play(CSound::LABEL_SE_DAMAGE_CRYSTAL);

        // ���̒l���Ⴂ�Ȃ�
        if (fRealDamage < WYVERN_HEAD_DAMAGE_UP_BORDER)
        {
            // �_���[�W�㏸
            fRealDamage = fRealDamage * WYVERM_HEAD_DAMAGE_UP_RATE;

            // �G�t�F�N�g����
            float fAngle = 0.0f;    //�p�x
            float fSpeed = EFFECT_HIT_WYVERN_HEAD_SPEED;   //����
            CCamera*pCamera = CManager::GetCamera();
            for (int nCnt = 0; nCnt < EFFECT_HIT_CRYSTAL_NUMBER_ONCE; nCnt++)
            {
                //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

                // �G�t�F�N�g����
                CEffect3D::Create(pos, EFFECT_HIT_WYVERN_HEAD_SIZE, move, EFFECT_HIT_CRYSTAL_COLOR,
                    DEFAULT_VECTOR, EFFECT_HIT_WYVERN_HEAD_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
            }
        }

        // �X�^�����Ă��Ȃ��Ȃ�
        if (m_bStun == false)
        {
            // �_���[�W������荂���Ȃ�
            if (fRealDamage >= WYVERN_STUN_VALUE_UP_BORDER)
            {
                // �m��X�^��
                m_nStunValue += WYVERN_STUN_VALUE_MAX;
            }
            else
            {
                // �X�^���l�㏸
                m_nStunValue += WYVERN_STUN_ADD_VALUE_HEAD;
            }
        }
    }
    // �����Ȃ�
    else if (nCntCollision == COLLISION_PARTS_WING_LEFT)
    {
        // ���ʑϋv�l����
        m_nLeftWingDurabirity -= (int)fRealDamage;
    }
    // �E���Ȃ�
    else if (nCntCollision == COLLISION_PARTS_WING_RIGHT)
    {
        // ���ʑϋv�l����
        m_nRightWingDurabirity -= (int)fRealDamage;
    }
    // �K���Ȃ�
    else if (nCntCollision == COLLISION_PARTS_TAIL_CUBE || nCntCollision == COLLISION_PARTS_TAIL_TIP)
    {
        // ���ʑϋv�l����
        m_nTailDurabirity -= (int)fRealDamage;
    }
    // ���Ȃ�
    else if (nCntCollision == COLLISION_PARTS_FOOT_CUBE_LEFT || nCntCollision == COLLISION_PARTS_FOOT_LEFT ||
        nCntCollision == COLLISION_PARTS_FOOT_CUBE_RIGHT || nCntCollision == COLLISION_PARTS_FOOT_RIGHT)
    {
        // �X�^�����Ă��Ȃ��Ȃ�
        if (m_bStun == false)
        {
            // �_���[�W������荂���Ȃ�
            if (fRealDamage >= WYVERN_STUN_VALUE_UP_BORDER)
            {
                // �m��X�^��
                m_nStunValue += WYVERN_STUN_VALUE_MAX;
            }
            else
            {
                // �X�^���l�㏸
                m_nStunValue += WYVERN_STUN_ADD_VALUE_FOOT;
            }
        }
    }

    // ���ۂɃ_���[�W���󂯂�(�ǂ̃_���[�W���A�v���C���[�̂��̂Ƃ��Ĉ���)
    SetTakeDamage((int)fRealDamage, CCharacter::LAST_HIT_PLAYER);
}

//=============================================================================
// �~�b�V�����N���A���ɉ��Z����
// Author : �㓡�T�V��
//=============================================================================
void CWyvern::AddMissionClear(void)
{
    // �X�e�[�W�ƌ��݂̃~�b�V�����ɉ����āA�N���A���𑝉�
    CGame::STAGE stage = CGame::GetStage();
    CGame::CURRENT_MISSION currentMission = CGame::GetCurrentMission();
    switch (stage)
    {
    case CGame::STAGE_1:

        break;

    case CGame::STAGE_2:

        break;

    case CGame::STAGE_FINAL:

        switch (currentMission)
        {
        case CGame::CURRENT_MISSION_1:

            break;

        case CGame::CURRENT_MISSION_2:
            CGame::SetClearNumMission2();
            break;
        }

        break;
    }
}