//=============================================================================
//
// ポリゴン処理 [polygon2d.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _POLYGON2D_H_
#define _POLYGON2D_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// 2Dポリゴンクラス定義
//*****************************************************************************
class CPolygon2D : public CObject
{
public:
	// メンバ関数
	CPolygon2D();									// コンストラクタ
	~CPolygon2D();									// デストラクタ
	HRESULT Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理
	static CPolygon2D *Create(void);				// インスタンス生成
	void BindTexture(const LPDIRECT3DTEXTURE9 pTex); // テクスチャ作成
	void SetPos(const D3DXVECTOR3 pos);		// 座標設定用
	void SetCol(const D3DXCOLOR col);
	void SetRot(const float rot);
	void SetSize(const float sizey, const float sizex);
	void SetTexUV(const float leftX, const float rightX, const float topY, const float bottomY);
	D3DXVECTOR3 GetPosition(void) const;
	void GetTexUV(float* leftX, float* rightX, float* topY, float* bottomY);
private:
	// メンバ変数
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	D3DXVECTOR3				m_pos;			// ポリゴンの位置
	D3DXCOLOR				m_col;			// ポリゴンカラー
	float					m_fRot; // 回転する角度
	float					m_fSizeY;
	float					m_fSizeX;
	float					m_TexLeftX;
	float					m_TexRightX;
	float					m_TexTopY;
	float					m_TexBottomY;
	LPD3DXFONT				m_pFont;		// フォントへのポインタ
};

#endif

