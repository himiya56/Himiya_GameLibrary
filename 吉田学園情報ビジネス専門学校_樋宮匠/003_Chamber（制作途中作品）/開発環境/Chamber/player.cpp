//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "player.h"
#include "keyboard.h"
#include "manager.h"
#include "collision.h"
#include "scene3d.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
LPD3DXMESH CPlayer::m_pMesh[PARTS_MAX] = {};
LPD3DXBUFFER CPlayer::m_pBuffMat[PARTS_MAX] = {};
DWORD CPlayer::m_nNumMat[PARTS_MAX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer()
{	
	// �����o�ϐ�
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�̈ړ���
	m_fGravDest = PLAYER_GRAVITY;
	m_bJump = false;
	m_bShot = false;
	m_bHit = false;
	m_pPortal = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// �p�[�c����ݒ�
	SetPartNum(PARTS_MAX);

	for (int nCount = 0; nCount < PARTS_MAX; nCount++)
	{
		BindMesh(CAnimation::TYPE_HUMAN , nCount, m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
	}

	// ���W�E�e�q�֌W�̓ǂݍ���
	LoadModelData("./data/ANIMATION/motion_player.txt");

	CCharacter::Init();

	m_bJump = false;
	m_bShot = false;
	m_bHit = false;

	SetPos(DEFAULT_VECTOR);

	// portal�C���X�^���X����
	m_pPortal = CPortal::Create();

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update()
{
	// ���쏈��
	Move();

	CCharacter::Update();
}

//=============================================================================
// �v���C���[���쏈��
//=============================================================================
void CPlayer::Move(void)
{
	// �v���C���[�����擾
	D3DXVECTOR3 pos, rot;

	pos = GetPos();
	rot = GetRot();

	// 1F�O�̈ʒu���L��
	SetPosOld(pos);

	CInputKeyboard *pInput = CManager::GetInput();

	// ���ֈړ�
	if (pInput->GetKeyboardPress(DIK_LEFT))
	{
		m_move.x = sinf(rot.y - D3DXToRadian(90)) * PLAYER_SPEED;
		m_move.z = cosf(rot.y - D3DXToRadian(90)) * PLAYER_SPEED;
	}
	// �E�ֈړ�
	if (pInput->GetKeyboardPress(DIK_RIGHT))
	{
		m_move.x = sinf(rot.y + D3DXToRadian(90)) * PLAYER_SPEED;
		m_move.z = cosf(rot.y + D3DXToRadian(90)) * PLAYER_SPEED;
	}
	// �O�ֈړ�
	if (pInput->GetKeyboardPress(DIK_UP))
	{
		m_move.x = sinf(rot.y) * PLAYER_SPEED;
		m_move.z = cosf(rot.y) * PLAYER_SPEED;
	}
	// ���ֈړ�
	if (pInput->GetKeyboardPress(DIK_DOWN))
	{
		m_move.x = sinf(rot.y) * -PLAYER_SPEED;
		m_move.z = cosf(rot.y) * -PLAYER_SPEED;
	}
	// �E�։�]
	if (pInput->GetKeyboardPress(DIK_E))
	{
		rot.y += D3DXToRadian(2);
	}
	// �E�։�]
	if (pInput->GetKeyboardPress(DIK_Q))
	{
		rot.y -= D3DXToRadian(2);
	}
	// �v���C���[�̑O�Ƀ|�[�^���ݒu
	// �|�[�^��
	if (pInput->GetKeyboardTrigger(DIK_Z))
	{
		m_pPortal->SetPortal(D3DXVECTOR3(pos.x, pos.y + 300.0f, pos.z), rot,CCameraPortal::COL_BLUE);
	}
	// �I�����W�|�[�^��
	if (pInput->GetKeyboardTrigger(DIK_X))
	{
		m_pPortal->SetPortal(D3DXVECTOR3(pos.x, pos.y + 300.0f, pos.z), rot,CCameraPortal::COL_ORANGE);
	}
	// portal���Z�b�g
	if (pInput->GetKeyboardTrigger(DIK_C))
	{
		m_pPortal->ResetPortal();
	}

	// �����̌v�Z
	m_move.x += (0.0f - m_move.x) * FRICTION;
	m_move.y += (0.0f - m_move.y) * FRICTION;
	m_move.z += (0.0f - m_move.z) * FRICTION;

	// �ʒu�̈ړ�
	pos += m_move;

	// �����蔻��
	CScene3D *pScene3D = NULL;
	CPortal  *pPortal = NULL;

	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		// �I�u�W�F�N�g�擾
		CObject *pObject = CObject::GetObj(nCount);

		if (pObject != NULL)
		{
			// �����蔻��p�̕ϐ�
			BLOCK_HIT_INFO blockHitInfo;
			blockHitInfo.hitSurface = HIT_SURFACE_NONE;
			blockHitInfo.pos = pos;

			if (pObject->GetObjType() == CObject::OBJ_TYPE_CUBE)
			{
				// character�ɃL���X�g
				pScene3D = (CScene3D*)pObject;
				// �����蔻��̌v�Z
				blockHitInfo = BlockCollision3D(&pos, &GetPosOld(), &pScene3D->GetPos(),
					&COLLLISION_SIZE, &pScene3D->GetCollisionSize());
			}

			// �u���b�N�̓����蔻��Ōv�Z�����ʒu���A���т���
			pos = blockHitInfo.pos;

			// �q�b�g�ʂ���ď���
			//=====================================================================================
			switch (blockHitInfo.hitSurface)
			{
			case HIT_SURFACE_TOP:
			case HIT_SURFACE_BOTTOM:
				// �ǂɂ߂荞�܂Ȃ�
				m_move.y = 0.0f;
				break;
			case HIT_SURFACE_LEFT:
			case HIT_SURFACE_RIGHT:
			case HIT_SURFACE_FRONT:
			case HIT_SURFACE_BACK:
				// �ǂɂ߂荞�܂Ȃ�
				m_move.x = 0.0f;
				m_move.z = 0.0f;
				break;
			default:
				break;
			}
		}
	}

	SetPos(pos);
	SetRot(rot);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	CCharacter::Draw();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit()
{
	CCharacter::Uninit();
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CPlayer::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v���C���[1
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/00_bodyD.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_HIP], NULL, &m_nNumMat[PARTS_HIP], &m_pMesh[PARTS_HIP]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/01_bodyU.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_BODY], NULL, &m_nNumMat[PARTS_BODY], &m_pMesh[PARTS_BODY]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/02_head.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_HEAD], NULL, &m_nNumMat[PARTS_HEAD], &m_pMesh[PARTS_HEAD]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/03_armUR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RSHOULDER], NULL, &m_nNumMat[PARTS_RSHOULDER], &m_pMesh[PARTS_RSHOULDER]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/04_armDR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RARM], NULL, &m_nNumMat[PARTS_RARM], &m_pMesh[PARTS_RARM]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/05_handR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RHAND], NULL, &m_nNumMat[PARTS_RHAND], &m_pMesh[PARTS_RHAND]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/06_armUL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LSHOULDER], NULL, &m_nNumMat[PARTS_LSHOULDER], &m_pMesh[PARTS_LSHOULDER]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/07_armDL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LARM], NULL, &m_nNumMat[PARTS_LARM], &m_pMesh[PARTS_LARM]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/08_handL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LHAND], NULL, &m_nNumMat[PARTS_LHAND], &m_pMesh[PARTS_LHAND]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/09_legUR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RTHIGH], NULL, &m_nNumMat[PARTS_RTHIGH], &m_pMesh[PARTS_RTHIGH]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/10_legDR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RLEG], NULL, &m_nNumMat[PARTS_RLEG], &m_pMesh[PARTS_RLEG]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/11_footR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RFOOT], NULL, &m_nNumMat[PARTS_RFOOT], &m_pMesh[PARTS_RFOOT]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/12_legUL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LTHIGH], NULL, &m_nNumMat[PARTS_LTHIGH], &m_pMesh[PARTS_LTHIGH]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/13_legDL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LLEG], NULL, &m_nNumMat[PARTS_LLEG], &m_pMesh[PARTS_LLEG]);
	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Player/14_footL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LFOOT], NULL, &m_nNumMat[PARTS_LFOOT], &m_pMesh[PARTS_LFOOT]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CPlayer::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer;
	pPlayer->SetObjType(OBJ_TYPE_PLAYER);
	pPlayer->Init();
	return pPlayer;
}