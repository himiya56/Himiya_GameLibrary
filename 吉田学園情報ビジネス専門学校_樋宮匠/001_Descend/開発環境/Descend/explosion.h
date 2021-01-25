//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EXPLOSION_HEIGHT 50		// 弾の高さ
#define EXPLOSION_WIDTH  50		// 弾の幅
#define EXPLOSION_SEG_X 8.0f	// プレイヤーテクスチャの横分割数
#define EXPLOSION_SEG_Y 1.0f	// プレイヤーテクスチャの縦分割数
#define ANIM_SPEED		6		// アニメーション速度

//*****************************************************************************
// バレットクラス定義
//*****************************************************************************
class CExplosion :public CPolygon3D
{
public:
	CExplosion();
	~CExplosion();
	HRESULT Init(D3DXVECTOR3 pos);
	void Update();
	void Draw();
	void Uninit();
	static HRESULT Load(void);
	static void Unload(void);
	static CExplosion * Create(D3DXVECTOR3 pos);
private:
	static LPDIRECT3DTEXTURE9		m_pTexture;					 // テクスチャへのポインタ
	int								m_nCountAnim = 0;			 // アニメーションカウンタ
};
#endif