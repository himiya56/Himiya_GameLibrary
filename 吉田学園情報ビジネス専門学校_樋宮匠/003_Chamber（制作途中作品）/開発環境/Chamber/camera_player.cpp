//===============================================
//
// カメラ処理 (camera.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "camera_player.h"
#include "player.h"
#include "keyboard.h"
#include "manager.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// コンストラクタ
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
// デストラクタ
//=============================================================================
CCameraPlayer::~CCameraPlayer()
{
}

//=============================================================================
// 初期化処理
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
// 終了処理
//=============================================================================
void CCameraPlayer::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCameraPlayer::Update(void)
{
	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		// オブジェクト取得
		CObject *pObject = CObject::GetObj(nCount);

		if (pObject != NULL)
		{
			if (pObject->GetObjType() == CObject::OBJ_TYPE_PLAYER)
			{
				m_posVDest = ((CCharacter*)pObject)->GetPos();
				m_posVDest.y += 300.0f;
				// カメラの角度をプレイヤーと同期させる
				m_fVerticalAngle = CAMERA_FIT_ANGLE - ((CCharacter*)pObject)->GetRot().y;
				m_FollowPos = m_posVDest;
			}
		}
	}

	// カメラ操作
	FollowMove();

	// 目的地の計算(球面座標)
	m_posRDest.x = m_fOffset * (sinf(m_fHorizontalAngle) * cosf(m_fVerticalAngle));
	m_posRDest.y = m_fOffset * cosf(m_fHorizontalAngle);
	m_posRDest.z = m_fOffset * (sinf(m_fHorizontalAngle) * sinf(m_fVerticalAngle));
	// 対象の座標に移動
	m_posRDest += m_FollowPos;
	// 目的地に近づける
	m_posR += (m_posRDest - m_posR) * 0.9f;
	m_posV += (m_posVDest - m_posV) * 0.9f;

	// 座標更新
	SetTargetPos(m_posR);
	SetPos(m_posV);

	// カメラアップデート
	CCamera::Update();
	SetCamera(CCamera::CAM_PLAYER);
}

//=============================================================================
// 追従移動操作処理
//=============================================================================
void CCameraPlayer::FollowMove(void)
{
	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetInput();

	// Wキーで上回転
	if (pKeyboard->GetKeyboardPress(DIK_W))
	{
		m_fHorizontalAngle -= D3DXToRadian(1.0f);
		// 0度以下にならないように
		if (m_fHorizontalAngle <= 0.0f)
		{
			m_fHorizontalAngle = D3DXToRadian(360.0f);
		}
	}
	// Sキーで下回転
	if (pKeyboard->GetKeyboardPress(DIK_S))
	{
		m_fHorizontalAngle += D3DXToRadian(1.0f);
		// 360度以上にならないように
		if (m_fHorizontalAngle >= D3DXToRadian(360.0f))
		{
			m_fHorizontalAngle = D3DXToRadian(0.0f);
		}
	}
	// Aキーで左回転
	if (pKeyboard->GetKeyboardPress(DIK_D))
	{
		m_fVerticalAngle -= D3DXToRadian(1.0f);
		// 0度以下にならないように
		if (m_fVerticalAngle <= 0.0f)
		{
			m_fVerticalAngle = D3DXToRadian(360.0f);
		}
	}
	// Dキーで右回転
	if (pKeyboard->GetKeyboardPress(DIK_A))
	{
		m_fVerticalAngle += D3DXToRadian(1.0f);
		// 360度以上にならないように
		if (m_fVerticalAngle >= D3DXToRadian(360.0f))
		{
			m_fVerticalAngle = D3DXToRadian(0.0f);
		}
	}
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CCameraPlayer * CCameraPlayer::Create(void)
{
	CCameraPlayer *pCameraPlayer = NULL;
	pCameraPlayer = new CCameraPlayer;
	pCameraPlayer->Init();
	pCameraPlayer->SetObjType(OBJ_TYPE_PLAYER_CAM);
	return pCameraPlayer;
}
