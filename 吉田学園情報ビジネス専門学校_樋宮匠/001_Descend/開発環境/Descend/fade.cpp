//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "fade.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
D3DXCOLOR CFade::m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
CFade::FADE	CFade::m_fade = FADE_NONE;
CManager::MODE	CFade::m_modeNext = CManager::MODE_GAME_STAGE_01;

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade()
{
	m_pVtxBuff = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFade::Init(void)
{
	// インスタンス生成
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;

	// 頂点バッファの生成
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].rhw = 1.0f;
		pVtx[nCount].col = D3DCOLOR_RGBA(0, 0, 0, 0);
	}

	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
	VERTEX_2D* pVtx;

	switch (m_fade)
	{
	case FADE_IN:
		m_col.a += FADE_RATE;
		if (m_col.a >=1.0f)
		{
			m_col.a = 1.0f;
			CManager::SetMode(m_modeNext);
			m_fade = FADE_OUT;
		}
		break;
	case FADE_OUT:
		m_col.a -= FADE_RATE;
		if (m_col.a <= 0.0f)
		{
			m_col.a = 0.0f;
			m_fade = FADE_NONE;
		}
		break;
	default:
		break;
	}
	// 頂点バッファの更新
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].col = m_col;
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();

	// 頂点バッファをデータストリームに設定
	pRenderer->GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pRenderer->GetDevice()->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pRenderer->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// フェードイン処理
//=============================================================================
void CFade::SetFade(CManager::MODE mode)
{
	m_fade = FADE_IN;
	m_col.a = 1.0f;

	m_modeNext = mode;
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CFade * CFade::Create(void)
{
	CFade *pFade;
	pFade = new CFade;
	pFade->Init();
	return pFade;
}