//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "result.h"
#include "keyboard.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CResult::m_pTexture = NULL;
bool CResult::m_bChageMode = false;
int CResult::m_nScore[MAX_RANKING] = {};
CNumber * CResult::m_apNumber[MAX_RANKING][NUM_SCORE_POLYGON] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{
	m_pPolygon = NULL;
	m_nRank = MAX_RANKING;
	m_nCountFlash = 60;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	// �ϐ��錾
	int nNum = 0;

	m_nRank = MAX_RANKING;
	m_nCountFlash = 60;

	// �����L���O�X�V�ꏊ�̊m�F
	for (int nCountRank = 0; nCountRank < MAX_RANKING; nCountRank++)
	{
		if (m_nScore[5] > m_nScore[nCountRank])
		{
			// �ȍ~�̃f�[�^�����Ɉړ�
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
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	Release();
}

//=============================================================================
// �X�V����
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
// �`�揈��
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
// �X�R�A�ǂݍ��ݏ���
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
// �X�R�A�������ݏ���
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
// �R���X�g���N�^
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
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CResult::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/gameover.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CResult::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}