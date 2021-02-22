//===============================================
//
// ボタン処理 (Button.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "Cube.h"
#include "manager.h"
#include "renderer.h"

//========================
// 静的メンバ変数宣言
//========================
LPD3DXMESH	CCube::m_pMesh = NULL;
LPD3DXBUFFER CCube::m_pBuffMat = NULL;
DWORD CCube::m_nNumMat = NULL;
LPDIRECT3DTEXTURE9 CCube::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CCube::CCube()
{
	m_rot = DEFAULT_VECTOR;
}

//=============================================================================
// デストラクタ
// Author : 樋宮匠
//=============================================================================
CCube::~CCube()
{
}

//=============================================================================
// 初期化処理
// Author : 樋宮匠
//=============================================================================
HRESULT CCube::Init(void)
{
	CScene3D::Init();

	BindMesh(m_pMesh, m_pBuffMat, m_nNumMat);
	BindTexture(m_pTexture);

	m_rot = DEFAULT_VECTOR;
	SetCollisionSize(COLLISION_SIZE);

	return S_OK;
}

//=============================================================================
// 終了処理
// Author : 樋宮匠
//=============================================================================
void CCube::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 樋宮匠
//=============================================================================
void CCube::Update(void)
{
	CScene3D::Update();
}

//=============================================================================
// 描画処理
// Author : 樋宮匠
//=============================================================================
void CCube::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene3D::Draw();
}

//=============================================================================
// テクスチャ・モデルデータ読み込み処理
// Author : 樋宮匠
//=============================================================================
HRESULT CCube::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Cube/Cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/color.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャ・モデルデータ破棄処理
// Author : 樋宮匠
//=============================================================================
void CCube::Unload(void)
{
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// インスタンス生成処理
// Author : 樋宮匠
//=============================================================================
CCube * CCube::Create(D3DXVECTOR3 pos)
{
	CCube *pCube = NULL;
	pCube = new CCube;
	pCube->Init();
	pCube->SetPos(pos);
	pCube->SetObjType(OBJ_TYPE_CUBE);
	return pCube;
}
