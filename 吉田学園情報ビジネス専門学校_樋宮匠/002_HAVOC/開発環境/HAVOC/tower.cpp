//================================================================================
//
// �^���[�̏��� (tower.cpp)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//================================================================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "tower.h"
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
LPD3DXMESH CTower::m_pMesh[PARTS_MAX] = {};
LPD3DXBUFFER CTower::m_pBuffMat[PARTS_MAX] = {};
DWORD CTower::m_nNumMat[PARTS_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CTower::CTower() :CCharacter(OBJTYPE::OBJTYPE_ENEMY)
{
    m_move = DEFAULT_VECTOR;
    m_nScore = 0;
    m_nOrbs = 0;

    m_nCntTime = 0;

    m_rotCrystal = DEFAULT_VECTOR;
    m_bLanding = false;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CTower::~CTower()
{
}

//=============================================================================
// ����������
// Author : ��{��
//=============================================================================
HRESULT CTower::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �p�[�c����ݒ�
    SetPartNum(PARTS_MAX);

    for (int nCount = 0; nCount < PARTS_MAX; nCount++)
    {
        BindMesh(CCharacter::TRIBE_TOWER, nCount, m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
    }

    // ���W�E�e�q�֌W�̓ǂݍ���
    LoadModelData("./data/ANIMATION/motion_tower.txt");

    // �A�j���[�V�����������Ȃ�
    CCharacter::SetUnableAnimation();

    // �{�X�d�l��
    CCharacter::SetBossSetting();

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CTower::Uninit(void)
{
    CCharacter::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CTower::Update(void)
{
    // �s��
    Movement();

    // �N���X�^������]������
    m_rotCrystal.y += TOWER_CRYSTAL_ROT_SPEED;
    if (m_rotCrystal.y >= D3DXToRadian(360.0f))
    {
        m_rotCrystal.y = D3DXToRadian(0);
    }
    // ��]�̍X�V
    CCharacter::GetParts(PARTS_CRYSTAL)->SetRot(m_rotCrystal);

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
    	ScatterOrbs();

    	// �X�R�A���Z
    	CScore *pScore = CGame::GetScore();
    	pScore->AddScore(m_nScore);

    	// ���m���ŁA�񕜃A�C�e���𗎂Ƃ�
    	int nRandNum = GetRandNum(100, 1);
    	if (nRandNum <= TOWER_DROP_HEAL_ITEM)
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
void CTower::Movement(void)
{
	// �ʒu�ƌ������擾
	D3DXVECTOR3 pos = DEFAULT_VECTOR;
	D3DXVECTOR3 rot = DEFAULT_VECTOR;
	pos = GetPos();
	rot = GetRot();

	// 1F�O�̈ʒu���L��
	SetPosOld(pos);

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

        // ���n
        if (m_bLanding == false)
        {
            SetLandingEffect(pos);
            m_bLanding = true;
        }
	}

    // ���n���Ă���Ȃ�
    if (m_bLanding == true)
    {
        // �J�E���g�A�b�v
        m_nCntTime++;

        // ���J�E���^�ŁA�G����
        if (m_nCntTime >= TOWER_SET_ENEMY_SPAWN_COUNT)
        {
            // �J�E���^���Z�b�g
            m_nCntTime = 0;

            SetEnemySpawn();
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
#endif // COLLISION_TEST
}

//=============================================================================
// �G�����n�_��ݒ�
// Author : �㓡�T�V��
//=============================================================================
void CTower::SetEnemySpawn(void)
{
    // �G�̑������擾
    int nNumAllEnemy = CScene::GetNumAll(CScene::OBJTYPE_ENEMY);

    // �G�𐶐��ł��鑍���Ȃ�
    if (nNumAllEnemy < TOWER_SET_ENEMY_COUNT_BORDER)
    {
        // �G�t�F�N�g�p�̕ϐ��錾
        float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_KAMI_ROT_MAX, MODEL_EFFECT_KAMI_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;

        // ��]�ʂ����߂�
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
        float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // �ʒu�����߂�
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*TOWER_ATTACK_RADIUS,
            TOWER_ENEMY_SPAWN_POS_Y, -cosf(fAngle)*TOWER_ATTACK_RADIUS) + GetPos();

        // �G�����̃G�t�F�N�g����
        CModelEffect::Create(effectPos, rotMove, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_KAMI);
    }
}

//=============================================================================
// �����蔻��̈ʒu���X�V
// Author : �㓡�T�V��
//=============================================================================
void CTower::UpdateCollisionPos(void)
{
	// ���̈ʒu�擾
	D3DXVECTOR3 pillerPos = D3DXVECTOR3(GetParts(CTower::PARTS_PILLER)->GetWorldMtx()._41,
		GetParts(CTower::PARTS_PILLER)->GetWorldMtx()._42,
		GetParts(CTower::PARTS_PILLER)->GetWorldMtx()._43);

	// �N���X�^���̈ʒu�擾
	D3DXVECTOR3 crystalPos = D3DXVECTOR3(GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._41,
		GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._42,
		GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._43);

	// �����蔻��̈ʒu���X�V
    SetCollisionPos(PARTS_PILLER, pillerPos);
    SetCollisionPos(PARTS_CRYSTAL, crystalPos);
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CTower::Draw(void)
{
    CCharacter::Draw();
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : ��{��
//=============================================================================
HRESULT CTower::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/tower/tower_piller.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_PILLER], NULL, &m_nNumMat[PARTS_PILLER], &m_pMesh[PARTS_PILLER]);
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/tower/tower_crystal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_CRYSTAL], NULL, &m_nNumMat[PARTS_CRYSTAL], &m_pMesh[PARTS_CRYSTAL]);

    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : ��{��
//=============================================================================
void CTower::Unload(void)
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
CTower * CTower::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    // �������m��
    CTower *pTower = NULL;
    pTower = new CTower;

    // �e���̏���ݒ�
    pTower->SetPos(pos);
    pTower->SetRot(rot);
    pTower->SetTribe(CCharacter::TRIBE_TOWER);

    pTower->Init(pos, DEFAULT_SCALE);

    pTower->SetActiveCollisionSize(TOWER_ACTIVE_COLLISION_SIZE);
    pTower->SetLife(TOWER_LIFE);
    pTower->m_nScore = TOWER_SCORE;
    pTower->m_nOrbs = TOWER_ORBS;

    pTower->SetCntCollision(TOWER_COLLISION_MAX);
    pTower->SetCollisionSize(PARTS_PILLER, TOWER_PILLER_COLLISION_SIZE);
    pTower->SetCollisionSize(PARTS_CRYSTAL, TOWER_CRYSTAL_COLLISION_SIZE);

    // �����蔻��̈ʒu���X�V
    pTower->UpdateCollisionPos();

    return pTower;
}

//=============================================================================
// ���n�G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CTower::SetLandingEffect(D3DXVECTOR3 pos)
{
    // �����擾
    CSound*pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_LAND);

    // �������K���L���΂�
    for (int nCnt = 0; nCnt < TOWER_LANDING_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
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
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*TOWER_LANDING_CRACK_RADIUS,
            0.0f, -cosf(fAngle)*TOWER_LANDING_CRACK_RADIUS) + pos;

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
void CTower::ScatterOrbs(void)
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
void CTower::DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    //�N���X�^���j��
    CSound*pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_DESTROY_CRYSTAL);

	// �p�[�c�̈ʒu�ƁA���f���G�t�F�N�g�̎��
	CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_CRYSTAL;

    // �N���X�^���̈ʒu�擾
    D3DXVECTOR3 crystalPos = D3DXVECTOR3(GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._41,
        GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._42,
        GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._43);

	// �G�t�F�N�g�̔���
	SetDeathCrushEffect(crystalPos, rot, modelEffectType);

    // �|��G�t�F�N�g�p�̕ϐ��錾
    float fAngle = 0.0f;                       // �p�x
    float fRadius = 0.0f;                      // ���a
    float fSpeed = 0.0f;                       // ����
    float fSize = 0.0f;                        // �傫��
    for (int nCnt = 0; nCnt < EFFECT_DESTROY_NUMBER_ONCE; nCnt++)
    {
        // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // ���a���o��
        fRadius = float(GetRandNum((int)TOWER_LANDING_CRACK_RADIUS * MODEL_EFFECT_BIG_COLLAPSE_RADIUS, 0)) / 100.0f;

        // �������o��
        fSpeed = float(GetRandNum(EFFECT_DESTROY_MAX_SPEED, EFFECT_DESTROY_MIN_SPEED)) / 100.0f;

        // �ړ��ʂ����߂�
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeed, 0.0f);

        // �傫�����o��
        fSize = float(GetRandNum(EFFECT_DESTROY_MAX_SIZE, EFFECT_DESTROY_MIN_SIZE)) / 100.0f;

        // �傫�������߂�
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // �ʒu�����߂�(Y����n�ʂ��甼�����������ʒu�ŌŒ�)
        D3DXVECTOR3 pos = D3DXVECTOR3(-sinf(fAngle)*fRadius + GetPos().x, fSize / 2, -cosf(fAngle)*fRadius + GetPos().z);

        // �G�t�F�N�g����
        CEffect3D::Create(pos, size, move, DEFAULT_COLOR,
            DEFAULT_VECTOR, EFFECT_DESTROY_FADE_OUT_RATE, CEffect3D::TYPE_SMOKE);
    }

    // ����G�t�F�N�g
    CollapseEffect();
}

//=============================================================================
// ����G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CTower::CollapseEffect(void)
{
    // �N���X�^���̈ʒu�擾
    D3DXVECTOR3 crystalPos = D3DXVECTOR3(GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._41,
        GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._42,
        GetParts(CTower::PARTS_CRYSTAL)->GetWorldMtx()._43);

    for (int nCnt = 0; nCnt < MODEL_EFFECT_COLLAPSE_NUMBER_ONCE; nCnt++)
    {
        // ����G�t�F�N�g�p�̕ϐ��錾
        float fPosX = 0.0f;                 // �ʒuX
        float fPosZ = 0.0f;                 // �ʒuZ
        float fRotSpeedX = 0.0f;            // ��]���xX
        float fRotSpeedY = 0.0f;            // ��]���xY
        float fRotSpeedZ = 0.0f;            // ��]���xZ
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_001;

        // �ʒu�����炷
        fPosX = float(GetRandNum((int)TOWER_LANDING_CRACK_RADIUS, 0)) - float(GetRandNum((int)TOWER_LANDING_CRACK_RADIUS, 0));
        fPosZ = float(GetRandNum((int)TOWER_LANDING_CRACK_RADIUS, 0)) - float(GetRandNum((int)TOWER_LANDING_CRACK_RADIUS, 0));

        // ��]�̑��������߂�
        fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;

        // ��]�ʂ����߂�
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        // ��ނ����߂�
        int nRandNum = GetRandNum(2, 0);
        switch (nRandNum)
        {
        case 0:
            modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_001;
            break;
        case 1:
            modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_002;
            break;
        case 2:
            modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_003;
            break;
        }

        // �G�t�F�N�g����(Y���͏�̖ʂŌŒ�)
        CModelEffect::Create(GetPos() + D3DXVECTOR3(fPosX, crystalPos.y, fPosZ), rotMove, DEFAULT_VECTOR, rotMove, modelEffectType);
    }
}

//=============================================================================
// ���ꂽ���̕��ӃG�t�F�N�g�̔���
// Author : �㓡�T�V��
//=============================================================================
void CTower::SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type)
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
// �U�����󂯂����ʂɂ���āA�󂯂�_���[�W��ς���
// Author : �㓡�T�V��
//=============================================================================
void CTower::TakeDamage(D3DXVECTOR3 pos, int nDamage, PARTS parts)
{
    // ���ۂ̃_���[�W�����߂�
    float fRealDamage = (float)nDamage;

    // ���ɓ��������Ȃ�
    if (parts == PARTS_PILLER)
    {
        // �_���[�W�y��
        fRealDamage = fRealDamage * TOWER_PILLER_REDUCTION_RATE;
    }
    // �N���X�^���ɓ��������Ȃ�
    else if (parts == PARTS_CRYSTAL)
    {
        // �����擾
        CSound*pSound = CManager::GetSound();
        pSound->Play(CSound::LABEL_SE_DAMAGE_CRYSTAL);

        // �_���[�W�㏸
        fRealDamage = fRealDamage * TOWER_CRYSTAL_DAMAGE_UP_RATE;

        // �G�t�F�N�g����
        float fAngle = 0.0f;    //�p�x
        float fSpeed = EFFECT_HIT_CRYSTAL_SPEED;   //����
        CCamera*pCamera = CManager::GetCamera();
        for (int nCnt = 0; nCnt < EFFECT_HIT_CRYSTAL_NUMBER_ONCE; nCnt++)
        {
            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

            // �G�t�F�N�g����
            CEffect3D::Create(pos, EFFECT_HIT_CRYSTAL_SIZE, move, EFFECT_HIT_CRYSTAL_COLOR,
                DEFAULT_VECTOR, EFFECT_HIT_CRYSTAL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }
    }

    // ���ۂɃ_���[�W���󂯂�(�ǂ̃_���[�W���A�v���C���[�̂��̂Ƃ��Ĉ���)
    SetTakeDamage((int)fRealDamage, CCharacter::LAST_HIT_PLAYER);
}

//=============================================================================
// �~�b�V�����N���A���ɉ��Z����
// Author : �㓡�T�V��
//=============================================================================
void CTower::AddMissionClear(void)
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

            break;

        case CGame::CURRENT_MISSION_2:
            CGame::SetClearNumMission2();
            break;
        }

        break;

    case CGame::STAGE_FINAL:

        break;
    }
}