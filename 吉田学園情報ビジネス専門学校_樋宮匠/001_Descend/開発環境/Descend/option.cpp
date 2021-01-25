//=============================================================================
//
// オプション処理 [option.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "manager.h"
#include "object.h"
#include "player.h"
#include "option.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 COption::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
COption::COption()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
COption::~COption()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT COption::Init(void)
{
	// 初期化
	CPolygon3D::Init();
	D3DXVECTOR3 playerPos = CPlayer::GetPlayerPos();

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 角度
	m_pos = D3DXVECTOR3(playerPos.x, playerPos.y + POLYGON_SIZE, 0.0f);

	SetPos(m_pos);
	SetRot(m_rot);
	SetSize(POLYGON_SIZE, POLYGON_SIZE);

	// テクスチャの作成
	BindTexture(m_pTexture);
	SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void COption::Update()
{
	// プレイヤーの頭上へホーミング
	D3DXVECTOR3 playerPos = CPlayer::GetPlayerPos();

	float distX = playerPos.x - m_pos.x;
	float distY = playerPos.y + POLYGON_SIZE - m_pos.y;

	float fAngle = atan2f(distY, distX);

	if (playerPos.x + 10 > m_pos.x &&
		playerPos.x - 10 < m_pos.x &&
		playerPos.y + POLYGON_SIZE + 10 > m_pos.y &&
		playerPos.y + POLYGON_SIZE - 10 < m_pos.y)
	{
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_move = D3DXVECTOR3(cos(fAngle)*DRONE_SPEED, sin(fAngle)*DRONE_SPEED, 0.0f);
	}

	m_pos += m_move;

	SetPos(m_pos);

	CPolygon3D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void COption::Draw()
{
	CPolygon3D::Draw();
}

//=============================================================================
// 終了処理
//=============================================================================
void COption::Uninit()
{
	CPolygon3D::Uninit();
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT COption::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Drone.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void COption::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
COption * COption::Create(void)
{
	COption *pOption;
	pOption = new COption;
	pOption->SetObjType(OBJ_TYPE_BULLET);
	pOption->Init();
	return pOption;
}