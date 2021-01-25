//===============================================
//
// 光源処理 (light.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "polygon3d.h"
#include "player.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
bool CCamera::m_bShake = false;
int CCamera::m_nOffset = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProjection);
	m_bShake = false;
	m_nShakeCount = 0;
	m_nOffset = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_posV = D3DXVECTOR3(0.0f, 0.0f, -700.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 2000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	m_bShake = false;
	m_nShakeCount = 0;
	m_nOffset = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
	//  カメラ位置修正処理
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 pos;

	if (m_bShake)
	{
		m_nShakeCount++;

		if (m_nShakeCount >= SHAKE_LENGTH)
		{
			m_nShakeCount = 0;
			m_bShake = false;
			m_nOffset = 0;
		}
		
		if (m_nShakeCount % 4 == 0)
		{
			m_nOffset -= SHAKE_SIZE / (SHAKE_LENGTH / 4);
			m_nOffset *= -1;
		}
	}

	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		// オブジェクト取得
		CObject *pObject = CObject::GetObj(nCount);

		if (pObject != NULL)
		{
			if (pObject->GetObjType() == CObject::OBJ_TYPE_PLAYER)
			{
				pos = ((CPolygon3D*)pObject)->GetPosition();
			}
		}
	}

	if (CPlayer::IsCameraFollowing())
	{
		m_posR.y = pos.y * 2 + m_nOffset;
		m_posV.y = pos.y * 2 + m_nOffset;
	}

	m_posV.x = m_nOffset;

	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(90), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 10000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

CCamera * CCamera::Create(void)
{
	CCamera *pCamera = NULL;
	pCamera = new CCamera;
	pCamera->Init();
	return pCamera;
}

void CCamera::SetShake(void)
{
	if (!m_bShake)
	{
		m_bShake = true;
		m_nOffset = SHAKE_SIZE;
	}
}
