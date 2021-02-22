//===============================================
//
// フィールド処理 (field.cpp)
// Author : 樋宮匠
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "field.h"
#include "manager.h"
#include "renderer.h"

//========================================
// 静的メンバ変数宣言
//========================================

//=============================================================================
// コンストラクタ
// Author : 樋宮匠
//=============================================================================
CField::CField()
{
	for (int nCountHeight = 0; nCountHeight < MAX_TILE_LINE; nCountHeight++)
	{
		for (int nCountWidth = 0; nCountWidth < MAX_TILE_LINE; nCountWidth++)
		{
			m_apTile[nCountHeight][nCountWidth] = NULL;
			m_aGround[nCountHeight][nCountWidth] = 0;
		}
	}
}

//=============================================================================
// デストラクタ
// Author : 樋宮匠
//=============================================================================
CField::~CField()
{
}

//=============================================================================
// 初期化処理
// Author : 樋宮匠
//=============================================================================
HRESULT CField::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
// Author : 樋宮匠
//=============================================================================
void CField::Uninit(void)
{
	for (int nCountHeight = 0; nCountHeight < MAX_TILE_LINE; nCountHeight++)
	{
		for (int nCountWidth = 0; nCountWidth < MAX_TILE_LINE; nCountWidth++)
		{
			// 中身があるなら
			if (m_apTile[nCountHeight][nCountWidth] != NULL)
			{
				// 終了処理
				m_apTile[nCountHeight][nCountWidth]->Uninit();

				// メモリ開放
				delete m_apTile[nCountHeight][nCountWidth];
				m_apTile[nCountHeight][nCountWidth] = NULL;
			}
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
// Author : 樋宮匠
//=============================================================================
void CField::Update(void)
{
}

//=============================================================================
// 描画処理
// Author : 樋宮匠
//=============================================================================
void CField::Draw(void)
{
	for (int nCountHeight = 0; nCountHeight < MAX_TILE_LINE; nCountHeight++)
	{
		for (int nCountWidth = 0; nCountWidth < MAX_TILE_LINE; nCountWidth++)
		{
			// 中身があるなら
			if (m_apTile[nCountHeight][nCountWidth] != NULL)
			{
				m_apTile[nCountHeight][nCountWidth]->Draw();
			}
		}
	}
}

//=============================================================================
// マップデータ読み込み処理
// Author : 樋宮匠
//=============================================================================
void CField::LoadMapData(TYPE type, int nWidth, int nDepth)
{
	FILE *pFile = NULL;
	int nLoadHeight = 0;
	int nLoadWidth = 0;

	// ステージによって、ファイルを開く
	switch (type)
	{
	case FLOOR:
		pFile = fopen("./data/MAPS/Map_01.csv", "r");
		break;
	case CEILING:
		pFile = fopen("./data/MAPS/Map_02.csv", "r");
		break;
	}

	// 開けた
	if (pFile != NULL)
	{
		for (int nCountMap = 0; nCountMap < nWidth * nDepth; nCountMap++)
		{
			while (fscanf(pFile, "%d,", &m_aGround[nLoadHeight][nLoadWidth]) != EOF)
			{
				nLoadWidth++;
				if (nLoadWidth >= nDepth)
				{
					nLoadWidth = 0;
					nLoadHeight++;
				}
			}
		}

		fclose(pFile);
	}
	// 開けない
	else
	{
		printf("開けれませんでした\n");
	}
}

//=============================================================================
// インスタンス生成処理
// Author : 樋宮匠
//=============================================================================
CField * CField::Create(TYPE type, int nWidth, int nDepth)
{
	CField *m_pField;

	m_pField = new CField;
	m_pField->Init();

	// フィールド読み込み
	m_pField->LoadMapData(type, nWidth, nDepth);

	// フィールド設置
	m_pField->SetField(nWidth, nDepth);

	return m_pField;
}

//=============================================================================
// フィールド生成処理
// Author : 樋宮匠
//=============================================================================
void CField::SetField(int nWidth, int nDepth)
{
	int nHeight = 0;

	for (int nCountHeight = nDepth - 1; nCountHeight > -1; nCountHeight--)
	{
		for (int nCountWidth = 0; nCountWidth < nWidth; nCountWidth++)
		{
			switch (m_aGround[nHeight][nCountWidth])
			{
			case CTile::TILE_FLOOR:
				m_apTile[nCountHeight][nCountWidth] = new CTile;
				m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_FLOOR);
				m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
					nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
				m_apTile[nCountHeight][nCountWidth]->Update();
				break;
			case CTile::TILE_CEILING:
				m_apTile[nCountHeight][nCountWidth] = new CTile;
				m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_CEILING);
				m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
					nCountWidth * TILE_SIZE / 2, 500.0f, nCountHeight * TILE_SIZE / 2));
				m_apTile[nCountHeight][nCountWidth]->Update();
				break;
			case CTile::TILE_LINE_BLUE:
				m_apTile[nCountHeight][nCountWidth] = new CTile;
				m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_LINE_BLUE);
				m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
					nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
				m_apTile[nCountHeight][nCountWidth]->Update();
				break;
			case CTile::TILE_LINE_ORANGE:
				m_apTile[nCountHeight][nCountWidth] = new CTile;
				m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_LINE_ORANGE);
				m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
					nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
				m_apTile[nCountHeight][nCountWidth]->Update();
				break;
			default:
				break;
			}
		}

		nHeight++;
	}
}
