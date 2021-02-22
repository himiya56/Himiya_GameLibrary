//===============================================
//
// カメラ処理 (camera.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "camera.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	for (int nCount = 0; nCount < CAM_MAX; nCount++)
	{
		D3DXMatrixIdentity(&m_mtxView[nCount]);
		D3DXMatrixIdentity(&m_mtxProjection[nCount]);
	}

	m_posV = DEFAULT_VECTOR;
	m_posR = DEFAULT_VECTOR;
	m_fFOV = 90.0f;
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

	m_posV = DEFAULT_VECTOR;
	m_posR = DEFAULT_VECTOR;
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_fFOV = 90.0f;

	for (int nCount = 0; nCount < CAM_MAX; nCount++)
	{
		D3DXMatrixLookAtLH(&m_mtxView[nCount], &m_posV, &m_posR, &m_vecU);
		pDevice->SetTransform(D3DTS_VIEW, &m_mtxView[nCount]);

		D3DXMatrixPerspectiveFovLH(&m_mtxProjection[nCount], D3DXToRadian(m_fFOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 2000.0f);
		pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection[nCount]);
	}

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
}

//=============================================================================
// 描画処理
//=============================================================================
void CCamera::Draw(void)
{
}

//=============================================================================
// カメラ更新処理(透視投影)
//=============================================================================
void CCamera::SetCamera(CAMERA cam)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMatrixLookAtLH(&m_mtxView[cam], &m_posV, &m_posR, &m_vecU);
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView[cam]);

	D3DXMatrixPerspectiveFovLH(&m_mtxProjection[cam], D3DXToRadian(m_fFOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 10000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection[cam]);
}
