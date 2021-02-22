//===============================================
//
// �J�������� (camera.cpp)
// Author : ��{��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "camera_player.h"
#include "player.h"
#include "keyboard.h"
#include "manager.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCameraPlayer::CCameraPlayer()
{
	m_posV = DEFAULT_VECTOR;
	m_posR = DEFAULT_VECTOR;
	m_posVDest = DEFAULT_VECTOR;
	m_posRDest = DEFAULT_VECTOR;
	m_fVerticalAngle = D3DXToRadian(1);
	m_fHorizontalAngle = D3DXToRadian(1);
	m_fOffset = INIT_OFFSET;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCameraPlayer::~CCameraPlayer()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCameraPlayer::Init(void)
{
	CCamera::Init();
	m_posV = DEFAULT_VECTOR;
	m_posR = DEFAULT_VECTOR;
	m_posVDest = DEFAULT_VECTOR;
	m_posRDest = DEFAULT_VECTOR;
	m_fVerticalAngle = D3DXToRadian(270);
	m_fHorizontalAngle = D3DXToRadian(90);
	m_fOffset = INIT_OFFSET;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCameraPlayer::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCameraPlayer::Update(void)
{
	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		// �I�u�W�F�N�g�擾
		CObject *pObject = CObject::GetObj(nCount);

		if (pObject != NULL)
		{
			if (pObject->GetObjType() == CObject::OBJ_TYPE_PLAYER)
			{
				m_posVDest = ((CCharacter*)pObject)->GetPos();
				m_posVDest.y += 300.0f;
				// �J�����̊p�x���v���C���[�Ɠ���������
				m_fVerticalAngle = CAMERA_FIT_ANGLE - ((CCharacter*)pObject)->GetRot().y;
				m_FollowPos = m_posVDest;
			}
		}
	}

	// �J��������
	FollowMove();

	// �ړI�n�̌v�Z(���ʍ��W)
	m_posRDest.x = m_fOffset * (sinf(m_fHorizontalAngle) * cosf(m_fVerticalAngle));
	m_posRDest.y = m_fOffset * cosf(m_fHorizontalAngle);
	m_posRDest.z = m_fOffset * (sinf(m_fHorizontalAngle) * sinf(m_fVerticalAngle));
	// �Ώۂ̍��W�Ɉړ�
	m_posRDest += m_FollowPos;
	// �ړI�n�ɋ߂Â���
	m_posR += (m_posRDest - m_posR) * 0.9f;
	m_posV += (m_posVDest - m_posV) * 0.9f;

	// ���W�X�V
	SetTargetPos(m_posR);
	SetPos(m_posV);

	// �J�����A�b�v�f�[�g
	CCamera::Update();
	SetCamera(CCamera::CAM_PLAYER);
}

//=============================================================================
// �Ǐ]�ړ����쏈��
//=============================================================================
void CCameraPlayer::FollowMove(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetInput();

	// W�L�[�ŏ��]
	if (pKeyboard->GetKeyboardPress(DIK_W))
	{
		m_fHorizontalAngle -= D3DXToRadian(1.0f);
		// 0�x�ȉ��ɂȂ�Ȃ��悤��
		if (m_fHorizontalAngle <= 0.0f)
		{
			m_fHorizontalAngle = D3DXToRadian(360.0f);
		}
	}
	// S�L�[�ŉ���]
	if (pKeyboard->GetKeyboardPress(DIK_S))
	{
		m_fHorizontalAngle += D3DXToRadian(1.0f);
		// 360�x�ȏ�ɂȂ�Ȃ��悤��
		if (m_fHorizontalAngle >= D3DXToRadian(360.0f))
		{
			m_fHorizontalAngle = D3DXToRadian(0.0f);
		}
	}
	// A�L�[�ō���]
	if (pKeyboard->GetKeyboardPress(DIK_D))
	{
		m_fVerticalAngle -= D3DXToRadian(1.0f);
		// 0�x�ȉ��ɂȂ�Ȃ��悤��
		if (m_fVerticalAngle <= 0.0f)
		{
			m_fVerticalAngle = D3DXToRadian(360.0f);
		}
	}
	// D�L�[�ŉE��]
	if (pKeyboard->GetKeyboardPress(DIK_A))
	{
		m_fVerticalAngle += D3DXToRadian(1.0f);
		// 360�x�ȏ�ɂȂ�Ȃ��悤��
		if (m_fVerticalAngle >= D3DXToRadian(360.0f))
		{
			m_fVerticalAngle = D3DXToRadian(0.0f);
		}
	}
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CCameraPlayer * CCameraPlayer::Create(void)
{
	CCameraPlayer *pCameraPlayer = NULL;
	pCameraPlayer = new CCameraPlayer;
	pCameraPlayer->Init();
	pCameraPlayer->SetObjType(OBJ_TYPE_PLAYER_CAM);
	return pCameraPlayer;
}
