//=============================================================================
//
// �J�������� [camera.h]
// Author : ��{��
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define INIT_OFFSET			300.0f	// �����_����̋���
#define CAMERA_FIT_ANGLE D3DXToRadian(90)

//================================================
// ���C�g�N���X�錾
//================================================
class CCamera :public CObject
{
public:
	CCamera();
	~CCamera();

	typedef enum
	{
		CAM_BLUE = 0,
		CAM_ORANGE,
		CAM_PLAYER,
		CAM_MAX
	}CAMERA;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetCamera(CAMERA cam);

	void SetTargetPos(D3DXVECTOR3 pos) { m_posR = pos; }	// �����_�ݒ�
	void SetPos(D3DXVECTOR3 pos) { m_posV = pos; }			// ���W�ݒ�
	void SetFOV(float fFOV) { m_fFOV = fFOV; }				// ����p�ݒ�

	D3DXVECTOR3 GetTargtPos(void) { return m_posR; }		// �����_�擾
	D3DXVECTOR3 GetPos(void) { return m_posV; }				// ���W�擾
private:
	D3DXVECTOR3 m_posV;
	D3DXVECTOR3 m_posR;
	D3DXVECTOR3 m_vecU;

	D3DXMATRIX  m_mtxProjection[CAM_MAX];
	D3DXMATRIX  m_mtxView[CAM_MAX];

	float m_fFOV;
};

#endif