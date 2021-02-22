//=============================================================================
//
// �v���C���[�J�������� [camera.h]
// Author : ��{��
//
//=============================================================================
#ifndef _CAMERA_PLAYER_H_
#define _CAMERA_PLAYER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//================================================
// �v���C���[�J�����N���X�錾
//================================================
class CCameraPlayer :public CCamera
{
public:
	CCameraPlayer();
	~CCameraPlayer();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void FollowMove(void);

	void SetVerticalAngle(const float fAngle) { m_fVerticalAngle = fAngle; }		// �J�����c��]�̊p�x�ݒ�
	void SetHorizontalAngle(const float fAngle) { m_fHorizontalAngle = fAngle; }	// �J�����c��]�̊p�x�ݒ�

	static CCameraPlayer *Create(void);
private:
	D3DXVECTOR3 m_posV;
	D3DXVECTOR3 m_posR;
	D3DXVECTOR3 m_posVDest;								// ���W�ڕW�l
	D3DXVECTOR3 m_posRDest;								// �����_�ڕW�l
	D3DXVECTOR3 m_FollowPos;							// �J�����Ǐ]�Ώۂ̍��W

	float		m_fOffset;								// �����_����̋���
	float		m_fVerticalAngle;						// ���ʍ��W�J�����c��]
	float		m_fHorizontalAngle;						// ���ʍ��W�J��������]
};

#endif