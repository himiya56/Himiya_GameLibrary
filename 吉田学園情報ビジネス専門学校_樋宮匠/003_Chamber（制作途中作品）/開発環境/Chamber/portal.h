//=============================================================================
//
// �|�[�^������ [portal.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _PORTAL_H_
#define _PORTAL_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"
#include "camera_portal.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PORTAL_WIDTH 200.0f
#define PORTAL_HEIGHT 400.0f
#define RENDER_TO_TEXTURE_FREQUENCY FPS
#define PORTAL_TEXTURE_UV 0.375f, 0.625f, 0.0f, 1.0f
#define PORTAL_RANGE 50.0f
#define PORTAL_COOL_DOWN 60
#define PORTAL_POLYGON_NUM 2 * CCameraPortal::COL_MAX
#define PORTAL_INIT_POS D3DXVECTOR3(0.0f, -PORTAL_HEIGHT, 0.0f)

//*****************************************************************************
// �v���C���[�N���X��`
//*****************************************************************************
class CPortal :public CObject
{
public:
	CPortal();
	~CPortal();
	HRESULT Init(void);
	void Update(void);
	void TeleportToPairPortal(void);
	void ResetPortal(void);
	void Draw(void);
	void Uninit(void);
	static CPortal *Create(void);
	void SetPortal(D3DXVECTOR3 pos, D3DXVECTOR3 angle, CCameraPortal::COLOR col);
	void SetCollisionSize(const D3DXVECTOR3 size) { m_collisionSize = size; }
	D3DXVECTOR3 GetPos(CCameraPortal::COLOR col);
	CCameraPortal *GetPortalCam(CCameraPortal::COLOR col) { return m_pCamera[col]; }
	CPolygon3D *GetPortalPolygon(CCameraPortal::COLOR col) { return m_apPolygon3D[col]; }
	D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }      // �Փ˂̑傫����Ԃ�

	static HRESULT Load(void);
	static void Unload(void);
private:
	// �ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9		m_pTexture[PORTAL_POLYGON_NUM];		// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ�
	CPolygon3D *m_apPolygon3D[PORTAL_POLYGON_NUM];						// 3D�|���S���ւ̃|�C���^
	CCameraPortal *m_pCamera[CCameraPortal::COL_MAX];					// portal�p�̃J����
	bool m_bUse[CCameraPortal::COL_MAX];								// portal���o���Ă��邩�ǂ���
	bool m_bAbleToPortal;												// portal���g�p�ł��邩
	int  m_nRenderToTexture;											// �e�N�X�`���֕`�悷�鎞�p�̃t���O
	int  m_nCountUsePortal;												// portal�̃N�[���_�E��
	D3DXVECTOR3 m_collisionSize;										// �����蔻��̑傫��
};
#endif