//===============================================
//
// �J�������� (camera.cpp)
// Author : ��{��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "camera.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// ����������
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
// �I������
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCamera::Draw(void)
{
}

//=============================================================================
// �J�����X�V����(�������e)
//=============================================================================
void CCamera::SetCamera(CAMERA cam)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMatrixLookAtLH(&m_mtxView[cam], &m_posV, &m_posR, &m_vecU);
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView[cam]);

	D3DXMatrixPerspectiveFovLH(&m_mtxProjection[cam], D3DXToRadian(m_fFOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 10000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection[cam]);
}
