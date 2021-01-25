//=============================================================================
//
// 数字表示処理 [number.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMBER_HEIGHT 80
#define NUMBER_WIDTH  80
#define NUM_SCORE_POLYGON  8   // スコアの桁数

//*****************************************************************************
// 2Dポリゴンクラス定義
//*****************************************************************************
class CNumber
{
public:
	// メンバ関数
	CNumber();										// コンストラクタ
	~CNumber();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理
	static CNumber *Create(D3DXVECTOR3 pos);					// インスタンス生成
	void BindTexture(const LPDIRECT3DTEXTURE9 pTex, const float segX, const float segY); // テクスチャ作成
	void SetPolygonPos(const D3DXVECTOR3 pos);		// 座標設定用
	void SetPolygonRot(const float rot);
	void SetPolygonSize(const float sizey, const float sizex);
	void SetTexSeg(const float segx, const float segy);
	void SetNumber(const int nNum);
	void SetCol(D3DXCOLOR col) { m_col = col; }
	static HRESULT Load(void);
	static void Unload(void);
private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff = NULL;		// 頂点バッファのポインタ
	D3DXVECTOR3						m_pos;			// ポリゴンの位置
	D3DXCOLOR						m_col;
	int								m_nNumber;
	float							m_fRot = D3DXToRadian(0.0f); // 回転する角度
	float							m_fSizeY = POLYGON_SIZE;
	float							m_fSizeX = POLYGON_SIZE;
	float							m_fTexUVX = 1.0f;	// テクスチャUV x座標
	float							m_fTexUVY = 1.0f;	// テクスチャUV y座標
	float							m_fTexSegX = 1.0f;  // テクスチャ横分割数 
	float							m_fTexSegY = 1.0f;  // テクスチャ縦分割数
};

#endif

