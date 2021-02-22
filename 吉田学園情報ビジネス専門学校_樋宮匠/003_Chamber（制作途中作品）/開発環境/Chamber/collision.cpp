//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "collision.h"

//=============================================================================
// �����蔻��3D
//=============================================================================
BLOCK_HIT_INFO BlockCollision3D(D3DXVECTOR3 * pPos1, D3DXVECTOR3 * pPos1Old, const D3DXVECTOR3 * pPos2, const D3DXVECTOR3 * pSize1, const D3DXVECTOR3 * pSize2)
{
	// �u���b�N�ɓ����������
	BLOCK_HIT_INFO blockHitInfo;
	blockHitInfo.hitSurface = HIT_SURFACE_NONE;
	blockHitInfo.pos = *pPos1;

	D3DXVECTOR3 box1Max = D3DXVECTOR3(pSize1->x / 2, pSize1->y, pSize1->z / 2) + *pPos1;      //�Ԃ��鑤�̍ő�l
	D3DXVECTOR3 box1Min = D3DXVECTOR3(-pSize1->x / 2, 0.0f, -pSize1->z / 2) + *pPos1;         //�Ԃ��鑤�̍ŏ��l
	D3DXVECTOR3 box2Max = D3DXVECTOR3(pSize2->x / 2, pSize2->y, pSize2->z / 2) + *pPos2;      //�Ԃ����鑤�̍ő�l
	D3DXVECTOR3 box2Min = D3DXVECTOR3(-pSize2->x / 2, 0.0f, -pSize2->z / 2) + *pPos2;         //�Ԃ����鑤�̍ŏ��l

																							  // �����蔻����v�Z
	if (box1Max.y > box2Min.y&&
		box1Min.y <= box2Max.y&&
		box1Max.x > box2Min.x&&
		box1Min.x < box2Max.x&&
		box1Max.z > box2Min.z&&
		box1Min.z < box2Max.z)
	{
		if (box1Max.y > box2Min.y&&
			pPos1Old->y + pSize1->y / 2 < box2Min.y)
		{// Y���̉�
			blockHitInfo.hitSurface = HIT_SURFACE_BOTTOM;
			blockHitInfo.pos.y = box2Min.y - pSize1->y;
		}
		if (box1Min.y <= box2Max.y&&
			pPos1Old->y >= box2Max.y)
		{// Y���̏�
			blockHitInfo.hitSurface = HIT_SURFACE_TOP;
			blockHitInfo.pos.y = box2Max.y;
		}
		if (box1Max.x > box2Min.x&&
			pPos1Old->x + pSize1->x / 2 <= box2Min.x)
		{// X���̍�
			blockHitInfo.hitSurface = HIT_SURFACE_LEFT;
			blockHitInfo.pos.x = box2Min.x - pSize1->x / 2;
		}
		if (box1Min.x < box2Max.x&&
			pPos1Old->x - pSize1->x / 2 >= box2Max.x)
		{// X���̉E
			blockHitInfo.hitSurface = HIT_SURFACE_RIGHT;
			blockHitInfo.pos.x = box2Max.x + pSize1->x / 2;
		}
		if (box1Max.z > box2Min.z&&
			pPos1Old->z + pSize1->z / 2 <= box2Min.z)
		{// Z���̎�O
			blockHitInfo.hitSurface = HIT_SURFACE_FRONT;
			blockHitInfo.pos.z = box2Min.z - pSize1->z / 2;
		}
		if (box1Min.z < box2Max.z&&
			pPos1Old->z - pSize1->z / 2 >= box2Max.z)
		{// Z���̉�
			blockHitInfo.hitSurface = HIT_SURFACE_BACK;
			blockHitInfo.pos.z = box2Max.z + pSize1->z / 2;
		}
	}

	return blockHitInfo;
}
