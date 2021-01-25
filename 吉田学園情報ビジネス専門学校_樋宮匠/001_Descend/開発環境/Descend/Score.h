//=============================================================================
//
// スコア処理 [score.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _SCORE_H_
#define _SCORE_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CScore :public CObject
{
public:
	CScore();
	~CScore();
	HRESULT Init(void);
	void Update();
	void Draw();
	void Uninit();
	static CScore * Create(D3DXVECTOR3 pos,int nValue = 0);
	static void AddScore(const int nValue) { m_nScore += nValue; }
	static int GetScore(void) { return m_nScore; }
private:
	static CNumber					*m_apNumber[NUM_SCORE_POLYGON];
	static int						m_nNum;
	static int						m_nScore;
	D3DXVECTOR3						m_pos;
	float							m_fRot = D3DXToRadian(0.0f);	 // 回転する角度
	float							m_fSizeX = SCREEN_WIDTH;
	float							m_fSizeY = SCREEN_HEIGHT;
	float							m_fTexUVY;					     // テクスチャUV移動用変数
};
#endif