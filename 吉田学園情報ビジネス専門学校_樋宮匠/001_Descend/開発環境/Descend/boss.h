//=============================================================================
//
// ボス処理 [boss.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _BOSS_H_
#define _BOSS_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	BOSS_LIFE 100
#define BOSS_HEIGHT 8
#define BOSS_ANIM_SPEED 8
#define BOSS_ANIM_KEY 8

//*****************************************************************************
// プレイヤークラス定義
//*****************************************************************************
class CBoss :public CPolygon3D
{
public:
	CBoss();
	~CBoss();
	HRESULT Init(D3DXVECTOR3 pos);
	void Update();
	void Draw();
	void Uninit();
	static HRESULT Load(void);
	static void Unload(void);
	static CBoss * Create(D3DXVECTOR3 pos);
	static void Damage(void);
private:
	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
	static D3DXVECTOR3				m_pos;				// ポリゴンの座標
	static D3DXVECTOR3				m_move;				// ポリゴンの移動量
	static int						m_nLife;
	float							m_fTex;
	int								m_nAnimCount;
};
#endif