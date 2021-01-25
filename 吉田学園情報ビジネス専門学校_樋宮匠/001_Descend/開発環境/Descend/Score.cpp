//=============================================================================
//
// スコア処理 [score.h]
// Author : 樋宮 匠
//
//=============================================================================
#include "Score.h"
#include "keyboard.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CScore::m_nNum = 0;
int CScore::m_nScore = 0;
CNumber * CScore::m_apNumber[NUM_SCORE_POLYGON] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRot = D3DXToRadian(0.0f);
	m_nNum = 0;
	m_nScore = 0;
	m_fSizeX = POLYGON_SIZE;
	m_fSizeY = POLYGON_SIZE;
	memset(m_apNumber, 0, sizeof(m_apNumber));
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(void)
{
	for (int nCount = 0; nCount < NUM_SCORE_POLYGON; nCount++)
	{
		// 初期化処理
		m_apNumber[nCount] = CNumber::Create(D3DXVECTOR3(m_pos.x + nCount * POLYGON_SIZE, m_pos.y, 0.0f));
		// 座標設定
		m_apNumber[nCount]->SetPolygonSize(m_fSizeX, m_fSizeY);

		m_nNum = (m_nScore % (int)pow(10.0f, (float)(NUM_SCORE_POLYGON - nCount))) / (int)pow(10.0f, (float)(NUM_SCORE_POLYGON - nCount - 1));
		m_apNumber[nCount]->SetNumber(m_nNum);
	}

	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update()
{
	for (int nCount = 0; nCount < NUM_SCORE_POLYGON; nCount++)
	{
		m_apNumber[nCount]->SetPolygonRot(m_fRot);
		m_apNumber[nCount]->SetPolygonSize(m_fSizeX, m_fSizeY);

		m_nNum = (m_nScore % (int)pow(10.0f, (float)(NUM_SCORE_POLYGON - nCount))) / (int)pow(10.0f, (float)(NUM_SCORE_POLYGON - nCount - 1));
		m_apNumber[nCount]->SetNumber(m_nNum);

		m_apNumber[nCount]->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw()
{
	for (int nCount = 0; nCount < NUM_SCORE_POLYGON; nCount++)
	{
		m_apNumber[nCount]->Draw();
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit()
{
	for (int nCount = 0; nCount < NUM_SCORE_POLYGON; nCount++)
	{
		m_apNumber[nCount]->Uninit();
		delete m_apNumber[nCount];
		m_apNumber[nCount] = NULL;
	}

	Release();
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CScore * CScore::Create(D3DXVECTOR3 pos,int nValue)
{
	CScore *pScore;
	pScore = new CScore;

	pScore->m_pos = pos;
	pScore->Init();
	pScore->m_nScore = nValue;

	return pScore;
}