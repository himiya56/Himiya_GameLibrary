//=============================================================================
//
// ブロック処理 [block.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon3d.h"

//*****************************************************************************
// ブロッククラス定義
//*****************************************************************************
class CBlock :public CPolygon3D
{
public:
	CBlock();
	~CBlock();

	// 列挙型宣言
	typedef enum
	{
		TYPE_BLOCK_NONE = 0,
		TYPE_BLOCK_01,
		TYPE_BLOCK_02,
		TYPE_BLOCK_PLAYER,
		TYPE_BLOCK_LOGO,
		TYPE_BLOCK_HOW_1,
		TYPE_BLOCK_HOW_2,
		TYPE_BLOCK_HOW_3,
		TYPE_BLOCK_BASE = 9,
		TYPE_BLOCK_CLEAR,
		TYPE_BLOCK_ENEMY_SLIME,
		TYPE_BLOCK_ENEMY_BAT,
		TYPE_BLOCK_ENEMY_EYE,
		TYPE_BLOCK_MAX
	}TYPE_BLOCK;

	HRESULT Init(D3DXVECTOR3 pos, TYPE_BLOCK type);
	void Update(void);
	void Draw(void);
	void Uninit(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CBlock * Create(D3DXVECTOR3 pos, TYPE_BLOCK type);
	static TYPE_BLOCK GetTypeBlock(void) { return m_type; }
private:
	static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_BLOCK_MAX];	 // テクスチャへのポインタ
	static TYPE_BLOCK m_type;
};

#endif

