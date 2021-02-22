//===============================================
//
// �{�^������ (Button.cpp)
// Author : ��{��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "Cube.h"
#include "manager.h"
#include "renderer.h"

//========================
// �ÓI�����o�ϐ��錾
//========================
LPD3DXMESH	CCube::m_pMesh = NULL;
LPD3DXBUFFER CCube::m_pBuffMat = NULL;
DWORD CCube::m_nNumMat = NULL;
LPDIRECT3DTEXTURE9 CCube::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCube::CCube()
{
	m_rot = DEFAULT_VECTOR;
}

//=============================================================================
// �f�X�g���N�^
// Author : ��{��
//=============================================================================
CCube::~CCube()
{
}

//=============================================================================
// ����������
// Author : ��{��
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
// �I������
// Author : ��{��
//=============================================================================
void CCube::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
// Author : ��{��
//=============================================================================
void CCube::Update(void)
{
	CScene3D::Update();
}

//=============================================================================
// �`�揈��
// Author : ��{��
//=============================================================================
void CCube::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene3D::Draw();
}

//=============================================================================
// �e�N�X�`���E���f���f�[�^�ǂݍ��ݏ���
// Author : ��{��
//=============================================================================
HRESULT CCube::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Cube/Cube.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/color.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���E���f���f�[�^�j������
// Author : ��{��
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
// �C���X�^���X��������
// Author : ��{��
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
