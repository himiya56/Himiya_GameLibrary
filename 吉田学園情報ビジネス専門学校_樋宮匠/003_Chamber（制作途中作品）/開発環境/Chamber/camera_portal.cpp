//===============================================
//
// �J�������� (camera.cpp)
// Author : ��{��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "camera_portal.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "polygon3d.h"
#include "player.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=============================================================================
CCameraPortal::~CCameraPortal()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCameraPortal::Init(void)
{
	CCamera::Init();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̍쐬
	D3DXCreateTexture(CManager::GetRenderer()->GetDevice(), SCREEN_WIDTH, SCREEN_HEIGHT, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture);

	// �e�N�X�`���T�[�t�F�C�X�̕ێ�
	m_pTexture->GetSurfaceLevel(0, &m_pTextureSurface);

	// �o�b�N�o�b�t�@�̕ێ�
	CManager::GetRenderer()->GetDevice()->GetRenderTarget(0, &m_pBackBufferSurface);

	m_nRenderCounter = 0;
	m_fVerticalAngle = D3DXToRadian(270);
	m_fHorizontalAngle = D3DXToRadian(90);

	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void CCameraPortal::Update(void)
{
	// ���g�̍��W���擾
	D3DXVECTOR3 posV;
	D3DXVECTOR3 posR = GetPos();

	// �@���̕���������悤�ɂ���
	posR += m_normal;
	// �J�����p�x����
	posR = RotateCameraToPlayerGaze(posR);
	// �����_�X�V
	SetTargetPos(posR);

	// �J�E���^�[����𒴂�����
	if (m_nRenderCounter >= RENDER_RATE)
	{
		// �J�E���^�[���Z�b�g
		m_nRenderCounter = 0;
		// �e�N�X�`���֕`��
		RenderToTexture();
	}

	// �����_�����O�e�N�X�`���J�E���g��i�߂�
	m_nRenderCounter++;
}

//=============================================================================
// �e�N�X�`���ւ̕`��
//=============================================================================
void CCameraPortal::RenderToTexture()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �����_�[�^�[�Q�b�g���e�N�X�`���T�[�t�F�C�X�ɕύX
	CManager::GetRenderer()->GetDevice()->SetRenderTarget(0, m_pTextureSurface);

	// �J�����X�V
	SetCamera((CCamera::CAMERA)m_col);

	// �e�N�X�`���֕`��
	CManager::GetRenderer()->Draw();

	// �����_�[�^�[�Q�b�g��ێ����Ă����o�b�N�o�b�t�@�ɖ߂�
	CManager::GetRenderer()->GetDevice()->SetRenderTarget(0, m_pBackBufferSurface);
}

//=============================================================================
// �v���C���[�ƃ|�[�^���̊p�x�ɃJ���������킹�鏈��
//=============================================================================
D3DXVECTOR3 CCameraPortal::RotateCameraToPlayerGaze(D3DXVECTOR3 pos)
{
	// �y�A�̃|�[�^�����p�ϐ�
	D3DXVECTOR3 pairPortalPos, pairPortalRot, camRot;
	CPortal *pPortal[COL_MAX] = {};

	// �v���C���[���p�ϐ�
	D3DXVECTOR3 playerPos;

	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		// �I�u�W�F�N�g�擾
		CObject *pObject = CObject::GetObj(nCount);

		if (pObject != NULL)
		{
			if (pObject->GetObjType() == CObject::OBJ_TYPE_PORTAL)
			{
				// portal�̃|�C���^�擾
				pPortal[COL_BLUE] = ((CPortal*)pObject);
				pPortal[COL_ORANGE] = ((CPortal*)pObject);
			}
			if (pObject->GetObjType() == CObject::OBJ_TYPE_PLAYER)
			{
				// �v���C���[���W�擾
				playerPos = ((CCharacter*)pObject)->GetPos();
			}
		}
	}

	// �y�A�̃|�[�^���̍��W���擾
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

	// �y�A�̃|�[�^���ƃv���C���[�̊p�x���v�Z
	camRot.x = atan2f(pairPortalPos.y - playerPos.y, pairPortalPos.z - playerPos.z);
	camRot.y = atan2f(pairPortalPos.z - playerPos.z, pairPortalPos.x - playerPos.x);
	camRot.z = atan2f(pairPortalPos.x - playerPos.x, pairPortalPos.y - playerPos.y);

	// 180���ȓ��ɏC��
	//camRot.y = fmodf(camRot.y, D3DXToRadian(180));

	// �|�[�^���̉�]���̂�����C��
	camRot.x = D3DXToRadian(0);
	camRot.y = -(CAMERA_FIT_ANGLE + camRot.y + pairPortalRot.y);
	camRot.z = D3DXToRadian(0);

	// ��]�s��̍쐬
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, camRot.y, camRot.x, camRot.z);

	// ���W��ϊ�
	D3DXVec3TransformCoord(&pos, &pos, &mtxRot);

	// �ϊ����������_��Ԃ�
	return pos;
}

//=============================================================================
// �C���X�^���X��������
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
