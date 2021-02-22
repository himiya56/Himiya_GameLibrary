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
CField::CField() :CScene(CScene::OBJTYPE_FIELD)
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
HRESULT CField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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
void CField::LoadMapData(CGame::STAGE stage, int nWidth, int nDepth)
{
    FILE *pFile = NULL;
    int nLoadHeight = 0;
    int nLoadWidth = 0;

    // ステージによって、ファイルを開く
    switch (stage)
    {
    case CGame::STAGE_1:
        pFile = fopen("./data/MAPS/Map_001.csv", "r");
        break;
    case CGame::STAGE_2:
        pFile = fopen("./data/MAPS/Map_002.csv", "r");
        break;
    case CGame::STAGE_FINAL:
        pFile = fopen("./data/MAPS/Map_003.csv", "r");
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
CField * CField::Create(CGame::STAGE stage, int nWidth, int nDepth)
{
    CField *m_pField;

    m_pField = new CField;
    m_pField->Init(DEFAULT_VECTOR, DEFAULT_SCALE);

    // フィールド読み込み
    m_pField->LoadMapData(stage, nWidth, nDepth);

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
            case CTile::TILE_BLACK:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_BLACK);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            case CTile::TILE_WHITE:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_WHITE);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            case CTile::TILE_RED:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_RED);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            case CTile::TILE_GREENERY:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_GREENERY);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            case CTile::TILE_SIDEWALK:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_SIDEWALK);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            case CTile::TILE_CHUOLINE:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_CHUOLINE);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            default:
                break;
            }
        }

        nHeight++;
    }
}
