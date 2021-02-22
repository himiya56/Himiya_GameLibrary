//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "collision.h"

//=============================================================================
// 当たり判定3D
//=============================================================================
BLOCK_HIT_INFO BlockCollision3D(D3DXVECTOR3 * pPos1, D3DXVECTOR3 * pPos1Old, const D3DXVECTOR3 * pPos2, const D3DXVECTOR3 * pSize1, const D3DXVECTOR3 * pSize2)
{
	// ブロックに当たった情報
	BLOCK_HIT_INFO blockHitInfo;
	blockHitInfo.hitSurface = HIT_SURFACE_NONE;
	blockHitInfo.pos = *pPos1;

	D3DXVECTOR3 box1Max = D3DXVECTOR3(pSize1->x / 2, pSize1->y, pSize1->z / 2) + *pPos1;      //ぶつかる側の最大値
	D3DXVECTOR3 box1Min = D3DXVECTOR3(-pSize1->x / 2, 0.0f, -pSize1->z / 2) + *pPos1;         //ぶつかる側の最小値
	D3DXVECTOR3 box2Max = D3DXVECTOR3(pSize2->x / 2, pSize2->y, pSize2->z / 2) + *pPos2;      //ぶつかられる側の最大値
	D3DXVECTOR3 box2Min = D3DXVECTOR3(-pSize2->x / 2, 0.0f, -pSize2->z / 2) + *pPos2;         //ぶつかられる側の最小値

																							  // 当たり判定を計算
	if (box1Max.y > box2Min.y&&
		box1Min.y <= box2Max.y&&
		box1Max.x > box2Min.x&&
		box1Min.x < box2Max.x&&
		box1Max.z > box2Min.z&&
		box1Min.z < box2Max.z)
	{
		if (box1Max.y > box2Min.y&&
			pPos1Old->y + pSize1->y / 2 < box2Min.y)
		{// Y軸の下
			blockHitInfo.hitSurface = HIT_SURFACE_BOTTOM;
			blockHitInfo.pos.y = box2Min.y - pSize1->y;
		}
		if (box1Min.y <= box2Max.y&&
			pPos1Old->y >= box2Max.y)
		{// Y軸の上
			blockHitInfo.hitSurface = HIT_SURFACE_TOP;
			blockHitInfo.pos.y = box2Max.y;
		}
		if (box1Max.x > box2Min.x&&
			pPos1Old->x + pSize1->x / 2 <= box2Min.x)
		{// X軸の左
			blockHitInfo.hitSurface = HIT_SURFACE_LEFT;
			blockHitInfo.pos.x = box2Min.x - pSize1->x / 2;
		}
		if (box1Min.x < box2Max.x&&
			pPos1Old->x - pSize1->x / 2 >= box2Max.x)
		{// X軸の右
			blockHitInfo.hitSurface = HIT_SURFACE_RIGHT;
			blockHitInfo.pos.x = box2Max.x + pSize1->x / 2;
		}
		if (box1Max.z > box2Min.z&&
			pPos1Old->z + pSize1->z / 2 <= box2Min.z)
		{// Z軸の手前
			blockHitInfo.hitSurface = HIT_SURFACE_FRONT;
			blockHitInfo.pos.z = box2Min.z - pSize1->z / 2;
		}
		if (box1Min.z < box2Max.z&&
			pPos1Old->z - pSize1->z / 2 >= box2Max.z)
		{// Z軸の奥
			blockHitInfo.hitSurface = HIT_SURFACE_BACK;
			blockHitInfo.pos.z = box2Max.z + pSize1->z / 2;
		}
	}

	return blockHitInfo;
}
