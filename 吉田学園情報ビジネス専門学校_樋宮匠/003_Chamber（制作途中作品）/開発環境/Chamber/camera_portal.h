//=============================================================================
//
// �J�������� [camera.h]
// Author : ��{��
//
//=============================================================================
#ifndef _CAMERA_PORTAL_H_
#define _CAMERA_PORTAL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RENDER_RATE FPS / 60
#define PORTAL_CAM_OFFSET 50.0f

//================================================
// ���C�g�N���X�錾
//================================================
class CCameraPortal :public CCamera
{
public:
	CCameraPortal();
	~CCameraPortal();

	typedef enum
	{
		COL_BLUE = 0,
		COL_ORANGE,
		COL_MAX
	}COLOR;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void RenderToTexture(void);
	D3DXVECTOR3 RotateCameraToPlayerGaze(D3DXVECTOR3 pos);
	static CCameraPortal *Create(COLOR col);
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	void SetNormal(D3DXVECTOR3 normal) { m_normal = normal; }
private:
	LPDIRECT3DTEXTURE9				m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DSURFACE9				m_pTextureSurface;
	LPDIRECT3DSURFACE9				m_pDepthSurface;
	LPDIRECT3DSURFACE9				m_pBackBufferSurface;
	LPDIRECT3DSURFACE9				m_pBackDepthSurface;

	int								m_nRenderCounter;
	float							m_fVerticalAngle;			// ���ʍ��W�J�����c��]
	float							m_fHorizontalAngle;			// ���ʍ��W�J��������]
	float							m_fAngle;					// �n�ʂƃ|�[�^���̊p�x

	D3DXVECTOR3						m_normal;					// �|�[�^���̖@��
	COLOR							m_col;						// �|�[�^���̐F
};

#endif