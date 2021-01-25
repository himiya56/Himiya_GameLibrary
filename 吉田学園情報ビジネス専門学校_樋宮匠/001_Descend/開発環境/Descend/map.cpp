//=============================================================================
//
// マップ生成処理 [map.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "map.h"
#include "block.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "logo.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CMap::aMapData[MAP_MAX][MAX_BLOCK] = {};
int CMap::m_nNumBlock[MAP_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CMap::CMap()
{
	memset(pFile, 0, sizeof(pFile));
}

//=============================================================================
// デストラクタ
//=============================================================================
CMap::~CMap()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMap::Init(void)
{
	memset(aMapData, 0, sizeof(aMapData));
	memset(m_nNumBlock, 0, sizeof(m_nNumBlock));

	LoadMapData();

	return S_OK;
}

//=============================================================================
// マップ生成処理
//=============================================================================
void CMap::GenerateMap(MAP_ID id)
{
	int nHorizon = 0;
	int nVert = 0;
	D3DXVECTOR3 pos;

	for (int nCount = 0; nCount < m_nNumBlock[id]; nCount++)
	{
		pos = D3DXVECTOR3(-(MAP_WIDTH / 2) * POLYGON_SIZE / 2 + POLYGON_SIZE / 2 * nHorizon, -nVert * POLYGON_SIZE / 2, 0.0f);

		switch (aMapData[id][nCount])
		{
		case CBlock::TYPE_BLOCK_NONE:
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_01:
			CBlock::Create(pos, CBlock::TYPE_BLOCK_01);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_02:
			CBlock::Create(pos, CBlock::TYPE_BLOCK_02);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_PLAYER:
			CPlayer::Create(pos);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_LOGO:
			CLogo::Create(pos);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_HOW_1:
			CBlock::Create(pos, CBlock::TYPE_BLOCK_HOW_1);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_HOW_2:
			CBlock::Create(pos, CBlock::TYPE_BLOCK_HOW_2);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_HOW_3:
			CBlock::Create(pos, CBlock::TYPE_BLOCK_HOW_3);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_BASE:
			CBlock::Create(pos, CBlock::TYPE_BLOCK_BASE);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_CLEAR:
			CBlock::Create(pos, CBlock::TYPE_BLOCK_CLEAR);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_ENEMY_SLIME:
			CEnemy::Create(pos, CEnemy::TYPE_SLIME);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_ENEMY_BAT:
			CEnemy::Create(pos, CEnemy::TYPE_BAT);
			nHorizon++;
			break;
		case CBlock::TYPE_BLOCK_ENEMY_EYE:
			CEnemy::Create(pos, CEnemy::TYPE_EYE);
			nHorizon++;
			break;
		default:
			nHorizon++;
			break;
		}

		if (nHorizon >= MAP_WIDTH)
		{
			nHorizon = 0;
			nVert++;
		}
	}
}

//=============================================================================
// マップデータ読み込み処理
//=============================================================================
void CMap::LoadMapData(void)
{
	//ファイル終わりまで繰り返す
	// ファイルオープン
	pFile[MAP_TITLE] = fopen("./data/MAPS/Map_Title.csv", "r");
	pFile[MAP_01] = fopen("./data/MAPS/Map_01.csv", "r");
	pFile[MAP_02] = fopen("./data/MAPS/Map_02.csv", "r");
	pFile[MAP_03] = fopen("./data/MAPS/Map_03.csv", "r");

	for (int nCountMap = 1; nCountMap < MAP_MAX; nCountMap++)
	{
		if (pFile[nCountMap] != NULL)
		{
			while (fscanf(pFile[nCountMap], "%d,", &aMapData[nCountMap][m_nNumBlock[nCountMap]]) != EOF)
			{
				m_nNumBlock[nCountMap]++;
			}
		}
	}

	// ファイルクローズ
	fclose(pFile[MAP_TITLE]);
	fclose(pFile[MAP_01]);
	fclose(pFile[MAP_02]);
	fclose(pFile[MAP_03]);
}

//=============================================================================
// 終了処理
//=============================================================================
void CMap::Uninit(void)
{
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CMap * CMap::Create(void)
{
	CMap *pMap;
	pMap = new CMap;
	pMap->Init();
	return pMap;
}