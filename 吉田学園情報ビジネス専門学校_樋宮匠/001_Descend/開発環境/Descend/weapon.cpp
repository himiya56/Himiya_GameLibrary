//=============================================================================
//
// ���폈�� [weapon.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "weapon.h"
#include "bullet.h"
#include "magazine.h"
#include "player.h"
#include "equipment.h"
#include "option.h"
#include "camera.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CWeapon::WEAPON_TYPE CWeapon::m_type = CWeapon::WEAPON_PUNCHER;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CWeapon::CWeapon()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWeapon::~CWeapon()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CWeapon::Init(void)
{
	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CWeapon::Update()
{
}

//=============================================================================
// �I������
//=============================================================================
void CWeapon::Uninit()
{
}

//=============================================================================
// ���킲�Ƃ̒e���ˏ���
//=============================================================================
void CWeapon::CreateShot(D3DXVECTOR3 pos)
{
	D3DXMATRIX mtxRot;
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, BULLET_SPEED, 0.0f);
	float fAngleRate;
	float fAngle;

	switch (m_type)
	{
	case WEAPON_MACHINEGUN:
		// �e�̐���
		CBullet::Create(pos, move, 0, CBullet::BULLET_PLAYER);

		// �e�������
		CMagazine::SubtractMag(1);
		break;
	case WEAPON_TRIPLE:
		// �^���ɔ�΂��e�̊p�x
		fAngle = D3DXToRadian(90);

		// �e���p�x�����炵�Ȃ��琶��
		CBullet::Create(pos, D3DXVECTOR3(cosf(fAngle) * BULLET_SPEED, sinf(fAngle) * BULLET_SPEED, 0.0f), 0.0f, CBullet::BULLET_PLAYER);
		CBullet::Create(pos, D3DXVECTOR3(cosf(fAngle + PI * 0.25f / 3.0f) * BULLET_SPEED, sinf(fAngle + PI * 0.25f / 3.0f) * BULLET_SPEED, 0.0f), 0.0f, CBullet::BULLET_PLAYER);
		CBullet::Create(pos, D3DXVECTOR3(cosf(fAngle - PI * 0.25f / 3.0f) * BULLET_SPEED, sinf(fAngle - PI * 0.25f / 3.0f) * BULLET_SPEED, 0.0f), 0.0f, CBullet::BULLET_PLAYER);

		// �e�̏����
		CMagazine::SubtractMag(1);
		break;
	case WEAPON_PUNCHER:
		// �e�̈ʒu�����炵�Ȃ��琶��
		CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 10.0f, pos.z), move, 0, CBullet::BULLET_PLAYER);
		CBullet::Create(D3DXVECTOR3(pos.x + BULLET_WIDTH / 2, pos.y, pos.z), move, 0, CBullet::BULLET_PLAYER);
		CBullet::Create(D3DXVECTOR3(pos.x - BULLET_WIDTH / 2, pos.y, pos.z), move, 0, CBullet::BULLET_PLAYER);

		// �e�������
		CMagazine::SubtractMag(1);
		break;
	default:
		break;
	}

	if (CEquipment::IsUse(CEquipment::EQUIPMENT_OPTION))
	{
		CBullet::Create(D3DXVECTOR3(pos.x, pos.y + POLYGON_SIZE, 0.0f), move, 0, CBullet::BULLET_PLAYER);
	}

	CCamera::SetShake();
}