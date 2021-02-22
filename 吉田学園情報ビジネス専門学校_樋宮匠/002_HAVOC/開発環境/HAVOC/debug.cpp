//===============================================
//
// デバッグ周りの処理 (debug.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "debug.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"

//========================================
// 静的メンバ変数宣言
//========================================
LPD3DXMESH	CDebug::m_pMesh = NULL;
LPD3DXBUFFER CDebug::m_pBuffMat = NULL;
DWORD CDebug::m_nNumMat = NULL;
LPDIRECT3DTEXTURE9 CDebug::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CDebug::CDebug() :CScene3D(CScene::OBJTYPE_MODEL_EFFECT)
{
    m_nLife = 2;
    m_type = TYPE_PERMANENT;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CDebug::~CDebug()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CDebug::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CScene3D::Init(pos, size);

    SetScale(size);

    BindMesh(m_pMesh, m_pBuffMat, m_nNumMat);
    BindTexture(m_pTexture);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CDebug::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CDebug::Update(void)
{
    CScene3D::Update();

    // 1Fずつ生成するモデルなら、その都度消す(あえて少し負荷をかけ、それ以上ゲームが重くならないように作る)
    if (m_type == TYPE_MOMENT)
    {
        m_nLife--;
    }

    if (m_nLife <= 0)
    {
        Uninit();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CDebug::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    CScene3D::Draw();
}

//=============================================================================
// モデルデータ読み込み処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CDebug::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/debug.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 後藤慎之助
//=============================================================================
void CDebug::Unload(void)
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
// Author : 後藤慎之助
//=============================================================================
CDebug * CDebug::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
    CDebug *pDebug = NULL;
    pDebug = new CDebug;
    pDebug->m_type = type;
    pDebug->Init(pos, size);

    return pDebug;
}

//=============================================================================
// デバッグコマンド
// Author : 後藤慎之助
//=============================================================================
void CDebug::Command(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // エンターキーが押されたら
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
    {
        // タイトルに移行
        CFade::SetFade(CManager::MODE_TITLE);
    }
}
