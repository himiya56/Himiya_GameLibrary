//=============================================================================
//
// アイテム処理 [item.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _ITEM_H_
#define _ITEM_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEM_HEIGHT 30	// 弾の高さ
#define ITEM_WIDTH  30    // 弾の幅
#define ITEM_SPEED  20
#define MAGAZINE_COUNT 2

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CSound;

//*****************************************************************************
// バレットクラス定義
//*****************************************************************************
class CItem :public CPolygon3D
{
public:
	CItem();
	~CItem();

	HRESULT Init(D3DXVECTOR3 pos);
	void Update();
	void Draw();
	void Uninit();
	static HRESULT Load(void);
	static void Unload(void);
	static CItem * Create(D3DXVECTOR3 pos);
	static 	D3DXVECTOR3 SpawnItem(D3DXVECTOR3 pos);
private:
	bool ItemHit(D3DXVECTOR3 pos, CSound *pSound);
	static LPDIRECT3DTEXTURE9		m_pTexture;					 // テクスチャへのポインタ
	static int						m_nMagazineCount;
	D3DXVECTOR3						m_move;						 // ポリゴンの移動量
	D3DXVECTOR3						m_rot;						 // 回転する角度
	float							m_fWaitTimer;
	bool							m_bItemMove;
};
#endif