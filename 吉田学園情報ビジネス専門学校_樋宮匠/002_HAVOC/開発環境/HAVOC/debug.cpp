//===============================================
//
// �f�o�b�O����̏��� (debug.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "debug.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
LPD3DXMESH	CDebug::m_pMesh = NULL;
LPD3DXBUFFER CDebug::m_pBuffMat = NULL;
DWORD CDebug::m_nNumMat = NULL;
LPDIRECT3DTEXTURE9 CDebug::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CDebug::CDebug() :CScene3D(CScene::OBJTYPE_MODEL_EFFECT)
{
    m_nLife = 2;
    m_type = TYPE_PERMANENT;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CDebug::~CDebug()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
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
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CDebug::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CDebug::Update(void)
{
    CScene3D::Update();

    // 1F���������郂�f���Ȃ�A���̓s�x����(�����ď������ׂ������A����ȏ�Q�[�����d���Ȃ�Ȃ��悤�ɍ��)
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
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CDebug::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    CScene3D::Draw();
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
HRESULT CDebug::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/debug.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : �㓡�T�V��
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
// �C���X�^���X��������
// Author : �㓡�T�V��
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
// �f�o�b�O�R�}���h
// Author : �㓡�T�V��
//=============================================================================
void CDebug::Command(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �G���^�[�L�[�������ꂽ��
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
    {
        // �^�C�g���Ɉڍs
        CFade::SetFade(CManager::MODE_TITLE);
    }
}
