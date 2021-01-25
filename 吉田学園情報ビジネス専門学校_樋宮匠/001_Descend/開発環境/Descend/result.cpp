//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "result.h"
#include "keyboard.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CResult::m_pTexture = NULL;
bool CResult::m_bChageMode = false;
int CResult::m_nScore[MAX_RANKING] = {};
CNumber * CResult::m_apNumber[MAX_RANKING][NUM_SCORE_POLYGON] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{
	m_pPolygon = NULL;
	m_nRank = MAX_RANKING;
	m_nCountFlash = 60;
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	// 変数宣言
	int nNum = 0;

	m_nRank = MAX_RANKING;
	m_nCountFlash = 60;

	// ランキング更新場所の確認
	for (int nCountRank = 0; nCountRank < MAX_RANKING; nCountRank++)
	{
		if (m_nScore[5] > m_nScore[nCountRank])
		{
			// 以降のデータを後ろに移動
			for (int nRank = MAX_RANKING - 2; nRank > nCountRank; nRank--)
			{
				m_nScore[nRank] = m_nScore[nRank - 1];
			}
			m_nScore[nCountRank] = m_nScore[5];
			m_nRank = nCountRank;
			break;
		}
		else
		{
			m_nRank = 5;
		}
	}

	for (int nCountRanking = 0; nCountRanking < MAX_RANKING; nCountRanking++)
	{
		for (int nCountNumber = 0; nCountNumber < NUM_SCORE_POLYGON; nCountNumber++)
		{
			if (nCountRanking == 5)
			{
				nNum = (m_nScore[nCountRanking] % (int)pow(10.0f, (float)(NUM_SCORE_POLYGON - nCountNumber))) / (int)pow(10.0f, (float)(NUM_SCORE_POLYGON - nCountNumber - 1));
				m_apNumber[nCountRanking][nCountNumber] = CNumber::Create(D3DXVECTOR3((SCREEN_WIDTH - POLYGON_SIZE  * NUM_SCORE_POLYGON) / 2 + nCountNumber * POLYGON_SIZE, 750, 0.0f));
				m_apNumber[nCountRanking][nCountNumber]->SetNumber(nNum);
			}
			else
			{
				nNum = (m_nScore[nCountRanking] % (int)pow(10.0f, (float)(NUM_SCORE_POLYGON - nCountNumber))) / (int)pow(10.0f, (float)(NUM_SCORE_POLYGON - nCountNumber - 1));
				m_apNumber[nCountRanking][nCountNumber] = CNumber::Create(D3DXVECTOR3((SCREEN_WIDTH - POLYGON_SIZE  * NUM_SCORE_POLYGON) / 2 + nCountNumber * POLYGON_SIZE, (nCountRanking + 2) * 100, 0.0f));
				m_apNumber[nCountRanking][nCountNumber]->SetNumber(nNum);
			}
		}
	}

	m_bChageMode = false;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	CInputKeyboard *pInput = CManager::GetInput();

	m_nCountFlash--;

	if (!m_bChageMode && (pInput->GetKeyboardTrigger(DIK_SPACE) || pInput->GetKeyboardTrigger(DIK_RSHIFT)))
	{
		m_bChageMode = true;
		CFade::SetFade(CManager::MODE_TITLE);
	}
	else
	{
		if (m_nCountFlash < 0)
		{
			m_nCountFlash = 60;
		}

		if (m_nCountFlash < 30)
		{
			for (int nCountNumber = 0; nCountNumber < NUM_SCORE_POLYGON; nCountNumber++)
			{
				m_apNumber[m_nRank][nCountNumber]->SetCol(D3DCOLOR_RGBA(255, 255, 255, 125));
				m_apNumber[5][nCountNumber]->SetCol(D3DCOLOR_RGBA(255, 255, 255, 125));
			}
		}
		else
		{
			for (int nCountNumber = 0; nCountNumber < NUM_SCORE_POLYGON; nCountNumber++)
			{
				m_apNumber[m_nRank][nCountNumber]->SetCol(D3DCOLOR_RGBA(255, 255, 255, 255));
				m_apNumber[5][nCountNumber]->SetCol(D3DCOLOR_RGBA(255, 255, 255, 255));
			}
		}
		

		for (int nCountRanking = 0; nCountRanking < MAX_RANKING; nCountRanking++)
		{
			for (int nCountNumber = 0; nCountNumber < NUM_SCORE_POLYGON; nCountNumber++)
			{
				m_apNumber[nCountRanking][nCountNumber]->Update();
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
	for (int nCountRanking = 0; nCountRanking < MAX_RANKING; nCountRanking++)
	{
		for (int nCountNumber = 0; nCountNumber < NUM_SCORE_POLYGON; nCountNumber++)
		{
			m_apNumber[nCountRanking][nCountNumber]->Draw();
		}
	}
}

//=============================================================================
// スコア読み込み処理
//=============================================================================
void CResult::ReadScore(void)
{
	FILE *pFile;

	pFile = fopen("./data/score.txt", "r");

	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			fscanf(pFile, "%d", &m_nScore[nCount]);
		}
	}

	fclose(pFile);
}

//=============================================================================
// スコア書き込み処理
//=============================================================================
void CResult::WriteScore(void)
{
	FILE *pFile;

	pFile = fopen("./data/score.txt", "w");

	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			fprintf(pFile, "%d\n", m_nScore[nCount]);
		}
	}

	fclose(pFile);
}

//=============================================================================
// コンストラクタ
//=============================================================================
CResult * CResult::Create(int nScore)
{
	CResult *pResult;
	pResult = new CResult;
	pResult->ReadScore();
	pResult->m_nScore[5] = nScore;
	pResult->Init();
	pResult->WriteScore();

	return pResult;
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CResult::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/gameover.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void CResult::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}