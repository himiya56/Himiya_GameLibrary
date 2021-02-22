//=============================================================================
//
// �����蔻�菈�� [collision.h]
// Author : ��{ ��
//
//=============================================================================

// ��d�C���N���[�h�h�~�p
#ifndef _COLLISION_H_
#define _COLLISION_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �I�u�W�F�N�g�N���X��`
//*****************************************************************************
// �u���b�N�ɓ����������̖�
typedef enum
{
	HIT_SURFACE_NONE = 0,     // �������Ă��Ȃ�
	HIT_SURFACE_RIGHT,        // �E
	HIT_SURFACE_LEFT,         // ��
	HIT_SURFACE_FRONT,        // ��O
	HIT_SURFACE_BACK,         // ��
	HIT_SURFACE_TOP,          // ���
	HIT_SURFACE_BOTTOM,       // ��
}HIT_SURFACE;

// �u���b�N�ɓ����������̏��
typedef struct
{
	HIT_SURFACE hitSurface;     // ����������
	D3DXVECTOR3 pos;            // �ʒu
}BLOCK_HIT_INFO;

// �����o�֐�
BLOCK_HIT_INFO BlockCollision3D(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos1Old, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
#endif
