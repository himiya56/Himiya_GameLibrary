//=============================================================================
//
// リザルト処理 [result.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon2d.h"
#include "number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RESULT_POLYGON 1
#define MAX_RANKING 6

//*****************************************************************************
// リザルトクラス定義
//*****************************************************************************
class CResult :public CObject
{
public:
	// メンバ関数
	CResult();										// コンストラクタ
	~CResult();										// デストラクタ
	HRESULT Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理
	static void ReadScore(void);					// スコア読み込み
	static void WriteScore(void);					// スコア書き込み
	static CResult *Create(int nScore);					// インスタンス生成
	static HRESULT Load(void);
	static void Unload(void);
private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	static bool						m_bChageMode;   // モードを変えたかのフラグ
	static int						m_nScore[MAX_RANKING];
	static CNumber					*m_apNumber[MAX_RANKING][NUM_SCORE_POLYGON];
	CPolygon2D						*m_pPolygon;
	int								m_nRank;
	int								m_nCountFlash;
};

#endif