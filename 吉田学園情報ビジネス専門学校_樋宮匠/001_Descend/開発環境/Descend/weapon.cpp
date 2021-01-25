//=============================================================================
//
// 武器処理 [weapon.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "weapon.h"
#include "bullet.h"
#include "magazine.h"
#include "player.h"
#include "equipment.h"
#include "option.h"
#include "camera.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CWeapon::WEAPON_TYPE CWeapon::m_type = CWeapon::WEAPON_PUNCHER;

//=============================================================================
// コンストラクタ
//=============================================================================
CWeapon::CWeapon()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CWeapon::~CWeapon()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWeapon::Init(void)
{
	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void CWeapon::Update()
{
}

//=============================================================================
// 終了処理
//=============================================================================
void CWeapon::Uninit()
{
}

//=============================================================================
// 武器ごとの弾発射処理
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
		// 弾の生成
		CBullet::Create(pos, move, 0, CBullet::BULLET_PLAYER);

		// 弾を消費する
		CMagazine::SubtractMag(1);
		break;
	case WEAPON_TRIPLE:
		// 真下に飛ばす弾の角度
		fAngle = D3DXToRadian(90);

		// 弾を角度をずらしながら生成
		CBullet::Create(pos, D3DXVECTOR3(cosf(fAngle) * BULLET_SPEED, sinf(fAngle) * BULLET_SPEED, 0.0f), 0.0f, CBullet::BULLET_PLAYER);
		CBullet::Create(pos, D3DXVECTOR3(cosf(fAngle + PI * 0.25f / 3.0f) * BULLET_SPEED, sinf(fAngle + PI * 0.25f / 3.0f) * BULLET_SPEED, 0.0f), 0.0f, CBullet::BULLET_PLAYER);
		CBullet::Create(pos, D3DXVECTOR3(cosf(fAngle - PI * 0.25f / 3.0f) * BULLET_SPEED, sinf(fAngle - PI * 0.25f / 3.0f) * BULLET_SPEED, 0.0f), 0.0f, CBullet::BULLET_PLAYER);

		// 弾の消費する
		CMagazine::SubtractMag(1);
		break;
	case WEAPON_PUNCHER:
		// 弾の位置をずらしながら生成
		CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 10.0f, pos.z), move, 0, CBullet::BULLET_PLAYER);
		CBullet::Create(D3DXVECTOR3(pos.x + BULLET_WIDTH / 2, pos.y, pos.z), move, 0, CBullet::BULLET_PLAYER);
		CBullet::Create(D3DXVECTOR3(pos.x - BULLET_WIDTH / 2, pos.y, pos.z), move, 0, CBullet::BULLET_PLAYER);

		// 弾を消費する
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