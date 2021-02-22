//===============================================
//
// カメラ処理 (camera.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "camera_portal.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "polygon3d.h"
#include "player.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CCameraPortal::CCameraPortal()
{
	m_pTextureSurface = NULL;
	m_pDepthSurface = NULL;
	m_pBackBufferSurface = NULL;
	m_pBackDepthSurface = NULL;
	m_nRenderCounter = 0;
	m_fVerticalAngle = D3DXToRadian(1);
	m_fHorizontalAngle = D3DXToRadian(1);
}

//=============================================================================
// デストラクタ
//=============================================================================
CCameraPortal::~CCameraPortal()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCameraPortal::Init(void)
{
	CCamera::Init();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの作成
	D3DXCreateTexture(CManager::GetRenderer()->GetDevice(), SCREEN_WIDTH, SCREEN_HEIGHT, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture);

	// テクスチャサーフェイスの保持
	m_pTexture->GetSurfaceLevel(0, &m_pTextureSurface);

	// バックバッファの保持
	CManager::GetRenderer()->GetDevice()->GetRenderTarget(0, &m_pBackBufferSurface);

	m_nRenderCounter = 0;
	m_fVerticalAngle = D3DXToRadian(270);
	m_fHorizontalAngle = D3DXToRadian(90);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCameraPortal::Uninit(void)
{
	CCamera::Uninit();

	if (m_pTextureSurface != NULL)
	{
		m_pTextureSurface->Release();
		m_pTextureSurface = NULL;
	}

	if (m_pDepthSurface != NULL)
	{
		m_pDepthSurface->Release();
		m_pDepthSurface = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CCameraPortal::Update(void)
{
	// 自身の座標を取得
	D3DXVECTOR3 posV;
	D3DXVECTOR3 posR = GetPos();

	// 法線の方向を見るようにする
	posR += m_normal;
	// カメラ角度調整
	posR = RotateCameraToPlayerGaze(posR);
	// 注視点更新
	SetTargetPos(posR);

	// カウンターが基準を超えたら
	if (m_nRenderCounter >= RENDER_RATE)
	{
		// カウンターリセット
		m_nRenderCounter = 0;
		// テクスチャへ描画
		RenderToTexture();
	}

	// レンダリングテクスチャカウントを進める
	m_nRenderCounter++;
}

//=============================================================================
// テクスチャへの描画
//=============================================================================
void CCameraPortal::RenderToTexture()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// レンダーターゲットをテクスチャサーフェイスに変更
	CManager::GetRenderer()->GetDevice()->SetRenderTarget(0, m_pTextureSurface);

	// カメラ更新
	SetCamera((CCamera::CAMERA)m_col);

	// テクスチャへ描画
	CManager::GetRenderer()->Draw();

	// レンダーターゲットを保持していたバックバッファに戻す
	CManager::GetRenderer()->GetDevice()->SetRenderTarget(0, m_pBackBufferSurface);
}

//=============================================================================
// プレイヤーとポータルの角度にカメラを合わせる処理
//=============================================================================
D3DXVECTOR3 CCameraPortal::RotateCameraToPlayerGaze(D3DXVECTOR3 pos)
{
	// ペアのポータル情報用変数
	D3DXVECTOR3 pairPortalPos, pairPortalRot, camRot;
	CPortal *pPortal[COL_MAX] = {};

	// プレイヤー情報用変数
	D3DXVECTOR3 playerPos;

	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		// オブジェクト取得
		CObject *pObject = CObject::GetObj(nCount);

		if (pObject != NULL)
		{
			if (pObject->GetObjType() == CObject::OBJ_TYPE_PORTAL)
			{
				// portalのポインタ取得
				pPortal[COL_BLUE] = ((CPortal*)pObject);
				pPortal[COL_ORANGE] = ((CPortal*)pObject);
			}
			if (pObject->GetObjType() == CObject::OBJ_TYPE_PLAYER)
			{
				// プレイヤー座標取得
				playerPos = ((CCharacter*)pObject)->GetPos();
			}
		}
	}

	// ペアのポータルの座標を取得
	if (m_col == COL_BLUE)
	{
		pairPortalPos = pPortal[COL_ORANGE]->GetPortalCam(COL_ORANGE)->GetPos();
		pairPortalRot = pPortal[COL_ORANGE]->GetPortalPolygon(COL_ORANGE)->GetRot();
	}
	else if (m_col == COL_ORANGE)
	{
		pairPortalPos = pPortal[COL_BLUE]->GetPortalCam(COL_BLUE)->GetPos();
		pairPortalRot = pPortal[COL_BLUE]->GetPortalPolygon(COL_BLUE)->GetRot();
	}

	// ペアのポータルとプレイヤーの角度を計算
	camRot.x = atan2f(pairPortalPos.y - playerPos.y, pairPortalPos.z - playerPos.z);
	camRot.y = atan2f(pairPortalPos.z - playerPos.z, pairPortalPos.x - playerPos.x);
	camRot.z = atan2f(pairPortalPos.x - playerPos.x, pairPortalPos.y - playerPos.y);

	// 180°以内に修正
	//camRot.y = fmodf(camRot.y, D3DXToRadian(180));

	// ポータルの回転分のずれを修正
	camRot.x = D3DXToRadian(0);
	camRot.y = -(CAMERA_FIT_ANGLE + camRot.y + pairPortalRot.y);
	camRot.z = D3DXToRadian(0);

	// 回転行列の作成
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, camRot.y, camRot.x, camRot.z);

	// 座標を変換
	D3DXVec3TransformCoord(&pos, &pos, &mtxRot);

	// 変換した注視点を返す
	return pos;
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CCameraPortal * CCameraPortal::Create(COLOR col)
{
	CCameraPortal *pCamera = NULL;
	pCamera = new CCameraPortal;
	pCamera->Init();
	pCamera->m_col = col;
	pCamera->SetObjType(OBJ_TYPE_PORTAL_CAM);
	return pCamera;
}
