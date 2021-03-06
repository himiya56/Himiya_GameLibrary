//===============================================
//
// 3D|S (scene3d.cpp)
// Author : γ‘TV
//
//===============================================

//========================
// CN[ht@C
//========================
#include "scene3d.h"
#include "manager.h"
#include "renderer.h"

//========================================
// ΓIoΟιΎ
//========================================
bool CScene3D::m_bAdditiveSynthesis = false;

//=============================================================================
// RXgN^
// Author : γ‘TV
//=============================================================================
CScene3D::CScene3D()
{
    m_pTexture = NULL;
    m_pBuffMat = NULL;
    m_pMesh = NULL;
    m_nNumMat = 0;
    D3DXMatrixIdentity(&m_mtxWorld);
    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_scale = DEFAULT_SCALE;
}

//=============================================================================
// I[o[Ch³κ½RXgN^
// Author : γ‘TV
//=============================================================================
CScene3D::CScene3D(OBJTYPE objType) :CScene(objType)
{
    m_pTexture = NULL;
    m_pBuffMat = NULL;
    m_pMesh = NULL;
    m_nNumMat = 0;
    D3DXMatrixIdentity(&m_mtxWorld);
    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_scale = DEFAULT_SCALE;
}

//=============================================================================
// fXgN^
//=============================================================================
CScene3D::~CScene3D()
{
}

//=============================================================================
// ϊ»
// Author : γ‘TV
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    m_pos = pos;

    return S_OK;
}

//=============================================================================
// IΉ
// Author : γ‘TV
//=============================================================================
void CScene3D::Uninit(void)
{
    Release();
}

//=============================================================================
// XV
// Author : γ‘TV
//=============================================================================
void CScene3D::Update(void)
{
}

//=============================================================================
// `ζ
// Author : γ‘TV
//=============================================================================
void CScene3D::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;
    D3DXMATERIAL *pMat;

    // ΑZ¬
    if (m_bAdditiveSynthesis == true)
    {
        // _[Xe[g(ΑZ¬Ι·ι)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    D3DXMatrixIdentity(&m_mtxWorld);

    D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

    D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    pDevice->GetMaterial(&matDef);

    pDevice->SetTexture(0, m_pTexture);

    ZeroMemory(&matDef, sizeof(D3DMATERIAL9));

    pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

    for (int nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
    {
        pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

        m_pMesh->DrawSubset(nCntMat);
    }

    pDevice->SetMaterial(&matDef);

    pDevice->SetTexture(0, NULL);

    // ΑZ¬πί·
    if (m_bAdditiveSynthesis == true)
    {
        // _[Xe[g(ΑZ¬πί·)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        // ΑZ¬ΜtOπfalseΙί·
        m_bAdditiveSynthesis = false;
    }
}

//=============================================================================
// bVθΔ
// Author : γ‘TV
//=============================================================================
void CScene3D::BindMesh(const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat)
{
    m_pMesh = pMesh;
    m_pBuffMat = pBuffMat;
    m_nNumMat = nNumMat;
}

//=============================================================================
// eNX`θΔ
// Author : γ‘TV
//=============================================================================
void CScene3D::BindTexture(const LPDIRECT3DTEXTURE9 pTex)
{
    m_pTexture = pTex;
}
