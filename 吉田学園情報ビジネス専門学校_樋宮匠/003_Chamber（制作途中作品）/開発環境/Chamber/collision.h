//=============================================================================
//
// 当たり判定処理 [collision.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _COLLISION_H_
#define _COLLISION_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// オブジェクトクラス定義
//*****************************************************************************
// ブロックに当たった時の面
typedef enum
{
	HIT_SURFACE_NONE = 0,     // 当たっていない
	HIT_SURFACE_RIGHT,        // 右
	HIT_SURFACE_LEFT,         // 左
	HIT_SURFACE_FRONT,        // 手前
	HIT_SURFACE_BACK,         // 奥
	HIT_SURFACE_TOP,          // 上面
	HIT_SURFACE_BOTTOM,       // 底
}HIT_SURFACE;

// ブロックに当たった時の情報
typedef struct
{
	HIT_SURFACE hitSurface;     // 当たった面
	D3DXVECTOR3 pos;            // 位置
}BLOCK_HIT_INFO;

// メンバ関数
BLOCK_HIT_INFO BlockCollision3D(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos1Old, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
#endif
