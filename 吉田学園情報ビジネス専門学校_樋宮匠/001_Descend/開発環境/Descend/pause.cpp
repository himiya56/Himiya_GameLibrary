//=============================================================================
//
// 装備処理 [equipment.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "fade.h"
#include "equipment.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPause::m_pTexture = NULL;
LPDIRECT3DVERTEXBUFFER9 CPause::m_pVtxBuff = NULL;
int CPause::m_nSelect = PAUSE_RETURN;

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause()
{
	m_nSelect = PAUSE_RETURN;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::~CPause()
{
}

//=============================================================================
// コンストラクタ
//=============================================================================
HRESULT CPause::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	VERTEX_2D*pVtx;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 6, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	m_nSelect = PAUSE_RETURN;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.001f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.001f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].rhw = 1.0f;

		pVtx[nCount].col = D3DCOLOR_RGBA(0, 0, 0, 180);

	}

	// 初期化処理
	for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
	{
		pVtx[4].pos = D3DXVECTOR3(D3DXVECTOR3((SCREEN_WIDTH / 2 - POLYGON_SIZE * 2) + nCount * 50 - POLYGON_SIZE * 2, (SCREEN_HEIGHT / 2 - POLYGON_SIZE * 2) + nCount * POLYGON_SIZE * 1.5 - POLYGON_SIZE / 2, 0.0f));
		pVtx[5].pos = D3DXVECTOR3(D3DXVECTOR3((SCREEN_WIDTH / 2 - POLYGON_SIZE * 2) + nCount * 50 + POLYGON_SIZE * 2, (SCREEN_HEIGHT / 2 - POLYGON_SIZE * 2) + nCount * POLYGON_SIZE * 1.5 - POLYGON_SIZE / 2, 0.0f));
		pVtx[6].pos = D3DXVECTOR3(D3DXVECTOR3((SCREEN_WIDTH / 2 - POLYGON_SIZE * 2) + nCount * 50 - POLYGON_SIZE * 2, (SCREEN_HEIGHT / 2 - POLYGON_SIZE * 2) + nCount * POLYGON_SIZE * 1.5 + POLYGON_SIZE / 2, 0.0f));
		pVtx[7].pos = D3DXVECTOR3(D3DXVECTOR3((SCREEN_WIDTH / 2 - POLYGON_SIZE * 2) + nCount * 50 + POLYGON_SIZE * 2, (SCREEN_HEIGHT / 2 - POLYGON_SIZE * 2) + nCount * POLYGON_SIZE * 1.5 + POLYGON_SIZE / 2, 0.0f));

		pVtx[4].tex = D3DXVECTOR2(nCount * 0.25f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(nCount * 0.25f + 0.25f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(nCount * 0.25f, 0.5f);
		pVtx[7].tex = D3DXVECTOR2(nCount * 0.25f + 0.25f, 0.5f);

		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx += 4;
	}

	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPause::Uninit(void)
{
	CRenderer::ResumeGame();

	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPause::Update(void)
{
	VERTEX_2D*pVtx;
	CInputKeyboard *pInput = CManager::GetInput();

	// 左へ移動
	if (pInput->GetKeyboardTrigger(DIK_A) || pInput->GetKeyboardTrigger(DIK_LEFT))
	{
		m_nSelect--;

		if (m_nSelect < PAUSE_RETURN)
		{
			m_nSelect = PAUSE_END;
		}
	}
	// 右へ移動
	if (pInput->GetKeyboardTrigger(DIK_D) || pInput->GetKeyboardTrigger(DIK_RIGHT))
	{
		m_nSelect++;

		if (m_nSelect > PAUSE_END)
		{
			m_nSelect = PAUSE_RETURN;
		}
	}

	// 決定処理
	if (pInput->GetKeyboardTrigger(DIK_SPACE) || pInput->GetKeyboardTrigger(DIK_RSHIFT))
	{
		switch (m_nSelect)
		{
		case PAUSE_RETURN:
			Uninit();
			break;
		case PAUSE_RETRY:
			Uninit();
			CEquipment::ClearEquipment();
			if (CManager::GetCurrentMode() == CManager::MODE_TITLE)
			{
				CFade::SetFade(CManager::GetCurrentMode());
			}
			else
			{
				CFade::SetFade(CManager::MODE_GAME_STAGE_01);
			}
			break;
		case PAUSE_TITLE:
			Uninit();
			CEquipment::ClearEquipment();
			CFade::SetFade(CManager::MODE_TITLE);
			break;
		case PAUSE_END:
			DestroyWindow(FindWindow(CLASS_NAME, NULL));
			break;
		default:
			break;
		}

	}
	else
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
		{
			if (nCount == m_nSelect)
			{
				pVtx[4].tex = D3DXVECTOR2(nCount * 0.25f, 0.5f);
				pVtx[5].tex = D3DXVECTOR2(nCount * 0.25f + 0.25f, 0.5f);
				pVtx[6].tex = D3DXVECTOR2(nCount * 0.25f, 1.0f);
				pVtx[7].tex = D3DXVECTOR2(nCount * 0.25f + 0.25f, 1.0f);
			}
			else
			{
				pVtx[4].tex = D3DXVECTOR2(nCount * 0.25f, 0.0f);
				pVtx[5].tex = D3DXVECTOR2(nCount * 0.25f + 0.25f, 0.0f);
				pVtx[6].tex = D3DXVECTOR2(nCount * 0.25f, 0.5f);
				pVtx[7].tex = D3DXVECTOR2(nCount * 0.25f + 0.25f, 0.5f);
			}

			pVtx += 4;
		}

		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPause::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();

	for (int nCount = 0; nCount < PAUSE_MAX + 1; nCount++)
	{
		// 頂点バッファをデータストリームに設定
		pRenderer->GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pRenderer->GetDevice()->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		if (nCount == 0)
		{
			pRenderer->GetDevice()->SetTexture(0, NULL);
		}
		else
		{
			pRenderer->GetDevice()->SetTexture(0, m_pTexture);
		}

		// ポリゴンの描画
		pRenderer->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, NUM_POLYGON);
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPause * CPause::Create(void)
{
	CPause *pPause;
	pPause = new CPause;
	pPause->Init();

	return pPause;
}

//=============================================================================
// コンストラクタ
//=============================================================================
HRESULT CPause::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/pause.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPause::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
