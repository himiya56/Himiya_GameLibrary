//=============================================================================
//
// オブジェクト管理処理 [manager.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "input.h"
#include "Keyboard.h"
#include "camera.h"
#include "player.h"
#include "field.h"
#include "camera_player.h"
#include "portal.h"
#include "Button.h"
#include "Dropper.h"
#include "Cube.h"
#include "light.h"
#include "tile.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CRenderer * CManager::m_pRenderer = NULL;
CInputKeyboard * CManager::m_pInput = NULL;
CCamera * CManager::m_pCamera = NULL;
CPlayer * CManager::m_pPlayer = NULL;
CLight *CManager::m_pLight = NULL;
CManager::MODE CManager::m_CurrentMode = CManager::MODE_GAME;
CManager::MODE CManager::m_NextStage = CManager::MODE_GAME;

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//インスタンス生成・初期化処理
	m_pRenderer = new CRenderer;
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return -1;
	}

	m_pInput = new CInputKeyboard;
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return -1;
	}

	// ライトの生成
	m_pLight = new CLight;
	if (FAILED(m_pLight->Init()))
	{
		return E_FAIL;
	}

	m_pCamera = CCameraPlayer::Create();

	// テクスチャの読み込み
	Load();

	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 250.0f, 0.0f), DEFAULT_VECTOR);
	CField::Create(CField::FLOOR, 10, 10);
	CField::Create(CField::CEILING, 10, 10);
	//CButton::Create();
	CCube::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	//CDropper::Create();

	SetMode(MODE_TITLE);

	return 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// 終了処理
	CObject::ReleaseAll();

	if (m_pInput != NULL)
	{
		m_pInput->Uninit();
		//メモリ破棄
		delete m_pInput;
		m_pInput = NULL;
	}
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		//メモリ破棄
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// テクスチャの破棄
	Unload();
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	// 更新処理
	if (m_pInput != NULL)
	{
		m_pInput->Update();
	}
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// 描画処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// モデル・テクスチャ読み込み処理
//=============================================================================
void CManager::Load(void)
{
	CPlayer::Load();
	CPortal::Load();
	CTile::Load();
	CButton::Load();
	CCube::Load();
	CDropper::Load();
	CPlayer::Load();
}

//=============================================================================
// モデル・テクスチャ破棄処理
//=============================================================================
void CManager::Unload(void)
{
	CPlayer::Unload();
	CPortal::Unload();
	CTile::Unload();
	CButton::Unload();
	CCube::Unload();
	CDropper::Unload();
	CPlayer::Unload();
}

//=============================================================================
// モード変更処理
//=============================================================================
void CManager::SetMode(const MODE mode)
{
}
