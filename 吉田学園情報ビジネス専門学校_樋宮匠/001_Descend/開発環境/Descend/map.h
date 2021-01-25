//=============================================================================
//
// マップ生成処理 [map.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _MAP_H_
#define _MAP_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAP_WIDTH		12  // マップの幅
#define MAP_HEIGHT		70  // マップの高さ
#define MAX_BLOCK		MAP_WIDTH * MAP_HEIGHT

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CMap
{
public:
	CMap();
	~CMap();

	typedef enum
	{
		MAP_NONE = 0,
		MAP_TITLE,
		MAP_01,
		MAP_02,
		MAP_03,
		MAP_MAX
	}MAP_ID;

	HRESULT Init(void);
	static void GenerateMap(MAP_ID id);
	void LoadMapData(void);
	void Uninit(void);
	static CMap * Create(void);
private:
	FILE *pFile[MAP_MAX];
	static int  aMapData[MAP_MAX][MAX_BLOCK];
	static int	m_nNumBlock[MAP_MAX];
};
#endif