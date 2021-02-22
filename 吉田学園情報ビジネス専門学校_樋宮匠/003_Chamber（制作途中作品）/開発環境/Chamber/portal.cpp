//=============================================================================
//
// 背景処理 [wall.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "portal.h"
#include "keyboard.h"
#include "manager.h"
#include "player.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPortal::m_pTexture[PORTAL_POLYGON_NUM] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CPortal::CPortal()
{
	// 初期化
	memset(m_pCamera, 0, sizeof(m_pCamera));
	memset(m_apPolygon3D, 0, sizeof(m_apPolygon3D));
	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		m_bUse[nCount] = false;
	}

	m_nRenderToTexture = 0;
	m_nCountUsePortal = 0;
	m_bAbleToPortal = true;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPortal::~CPortal()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPortal::Init()
{
	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		// ポリゴン インスタンス生成・初期化
		m_apPolygon3D[nCount] = new CPolygon3D;
		m_apPolygon3D[nCount]->Init();
		m_apPolygon3D[nCount]->SetSize(PORTAL_HEIGHT, PORTAL_WIDTH);
		m_apPolygon3D[nCount]->SetTexUV(PORTAL_TEXTURE_UV);
		m_apPolygon3D[nCount]->SetPos(PORTAL_INIT_POS);

		// カメラ インスタンス生成・初期化
		m_pCamera[nCount] = CCameraPortal::Create((CCameraPortal::COLOR)nCount);

		// portal初期化
		m_bUse[nCount] = false;
	}

	for (int nCount = CCameraPortal::COL_MAX; nCount < PORTAL_POLYGON_NUM; nCount++)
	{
		// ポリゴン インスタンス生成・初期化
		m_apPolygon3D[nCount] = new CPolygon3D;
		m_apPolygon3D[nCount]->Init();
		m_apPolygon3D[nCount]->SetSize(PORTAL_HEIGHT, PORTAL_WIDTH);
		m_apPolygon3D[nCount]->BindTexture(m_pTexture[nCount - 2]);
		m_apPolygon3D[nCount]->SetPos(PORTAL_INIT_POS);
	}

	// OBJ_TYPEを設定
	m_apPolygon3D[CCameraPortal::COL_BLUE]->SetObjType(OBJ_TYPE_PORTAL_BLUE);
	m_apPolygon3D[CCameraPortal::COL_ORANGE]->SetObjType(OBJ_TYPE_PORTAL_ORANGE);

	m_bAbleToPortal = true;
	m_nCountUsePortal = 0;

	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void CPortal::Update()
{
	LPDIRECT3DTEXTURE9 pTexturePortal[CCameraPortal::COL_MAX] = {};	 // ポータルへのテクスチャ

	// それぞれの反対のportalからの景色をテクスチャへ割り当て
	pTexturePortal[CCameraPortal::COL_ORANGE] = m_pCamera[CCameraPortal::COL_BLUE]->GetTexture();
	pTexturePortal[CCameraPortal::COL_BLUE] = m_pCamera[CCameraPortal::COL_ORANGE]->GetTexture();

	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		// 取得したテクスチャの貼り付け
		if (m_bUse[CCameraPortal::COL_BLUE] == m_bUse[CCameraPortal::COL_ORANGE])
		{
			m_apPolygon3D[nCount]->BindTexture(pTexturePortal[nCount]);
		}
		else
		{
			m_apPolygon3D[nCount]->BindTexture(m_pTexture[nCount + CCameraPortal::COL_MAX]);
		}

		// ポリゴンの法線取得
		D3DXVECTOR3 normal = m_apPolygon3D[nCount]->GetNormal();
		m_pCamera[nCount]->SetNormal(normal);
	}

	// テレポート処理
	TeleportToPairPortal();
}

//=============================================================================
// portal間移動処理
//=============================================================================
void CPortal::TeleportToPairPortal(void)
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 BluePos, OrangePos;
	D3DXVECTOR3 BlueRot, OrangeRot;

	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		// オブジェクト取得
		CObject *pObject = CObject::GetObj(nCount);

		if (pObject != NULL)
		{
			if (pObject->GetObjType() == CObject::OBJ_TYPE_PLAYER)
			{
				pos = ((CCharacter*)pObject)->GetPos();
			}
		}
	}

	// portalの座標・角度取得
	BluePos = m_apPolygon3D[CCameraPortal::COL_BLUE]->GetPos();
	BlueRot = m_apPolygon3D[CCameraPortal::COL_BLUE]->GetRot();

	OrangePos = m_apPolygon3D[CCameraPortal::COL_ORANGE]->GetPos();
	OrangeRot = m_apPolygon3D[CCameraPortal::COL_ORANGE]->GetRot();

	// 法線の向きをもとに範囲を計算
	D3DXVECTOR3 size[CCameraPortal::COL_MAX];
	size[CCameraPortal::COL_BLUE] = m_apPolygon3D[CCameraPortal::COL_BLUE]->GetNormal() * PORTAL_RANGE;
	size[CCameraPortal::COL_ORANGE] = m_apPolygon3D[CCameraPortal::COL_ORANGE]->GetNormal() * PORTAL_RANGE;

	// portalを使用できるなら
	if (m_bAbleToPortal)
	{
		// ポータルに重なったら対応するportalに転送
		if (fabsf(BluePos.x - pos.x) <= size[CCameraPortal::COL_BLUE].x &&
			fabsf(BluePos.z - pos.z) <= size[CCameraPortal::COL_BLUE].z &&
			fabsf(BluePos.y - pos.y) <= size[CCameraPortal::COL_BLUE].y &&
			m_bUse[CCameraPortal::COL_ORANGE])
		{
			CManager::GetPlayer()->SetPos(OrangePos);
			CManager::GetPlayer()->SetRot(OrangeRot);
			m_bAbleToPortal = false;
		}
		else if (fabsf(OrangePos.x - pos.x) <= size[CCameraPortal::COL_ORANGE].x &&
			fabsf(OrangePos.z - pos.z) <= size[CCameraPortal::COL_ORANGE].z &&
			fabsf(OrangePos.y - pos.y) <= size[CCameraPortal::COL_ORANGE].y &&
			m_bUse[CCameraPortal::COL_BLUE])
		{
			CManager::GetPlayer()->SetPos(BluePos);
			CManager::GetPlayer()->SetRot(BlueRot);
			m_bAbleToPortal = false;
		}
	}
	else
	{
		m_nCountUsePortal++;
		if (m_nCountUsePortal >= PORTAL_COOL_DOWN)
		{
			m_nCountUsePortal = 0;
			m_bAbleToPortal = true;
		}
	}
}

//=============================================================================
// ポータルリセット処理（初期化）
//=============================================================================
void CPortal::ResetPortal(void)
{
	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		m_apPolygon3D[nCount]->SetPos(PORTAL_INIT_POS);
		// portal初期化
		m_bUse[nCount] = false;
	}
	for (int nCount = CCameraPortal::COL_MAX; nCount < PORTAL_POLYGON_NUM; nCount++)
	{
		m_apPolygon3D[nCount]->SetPos(PORTAL_INIT_POS);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPortal::Draw()
{
}

//=============================================================================
// 終了処理
//=============================================================================
void CPortal::Uninit()
{
	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		// カメラ終了
		m_pCamera[nCount]->Uninit();
		// ポリゴン終了
		m_apPolygon3D[nCount]->Uninit();
	}
}

//=============================================================================
// ポータル生成処理
//=============================================================================
void CPortal::SetPortal(D3DXVECTOR3 pos, D3DXVECTOR3 angle ,CCameraPortal::COLOR col)
{
	m_apPolygon3D[col]->SetPos(pos);
	m_apPolygon3D[col + CCameraPortal::COL_MAX]->SetPos(pos);

	m_apPolygon3D[col]->SetRot(angle);
	m_apPolygon3D[col + CCameraPortal::COL_MAX]->SetRot(angle);

	// 座標設定
	m_pCamera[col]->SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));

	m_bUse[col] = true;
}

//=============================================================================
// ポータル座標取得用
//=============================================================================
D3DXVECTOR3 CPortal::GetPos(CCameraPortal::COLOR col)
{
	return m_apPolygon3D[col]->GetPos();
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CPortal::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/blue.png", &m_pTexture[CCameraPortal::COL_BLUE]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/orange.png", &m_pTexture[CCameraPortal::COL_ORANGE]);

	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/blue_close.png", &m_pTexture[CCameraPortal::COL_BLUE + CCameraPortal::COL_MAX]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/orange_close.png", &m_pTexture[CCameraPortal::COL_ORANGE + CCameraPortal::COL_MAX]);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void CPortal::Unload(void)
{
	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CPortal * CPortal::Create(void)
{
	CPortal *pPortal;
	pPortal = new CPortal;
	pPortal->Init();
	pPortal->SetObjType(OBJ_TYPE_PORTAL);
	return pPortal;
}
