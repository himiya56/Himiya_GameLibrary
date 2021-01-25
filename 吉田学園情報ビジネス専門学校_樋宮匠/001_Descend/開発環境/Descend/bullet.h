//=============================================================================
//
// 弾処理 [bullet.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _BULLET_H_
#define _BULLET_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_HEIGHT 30	// 弾の高さ
#define BULLET_WIDTH  30    // 弾の幅
#define BULLET_LIFE   20	// 弾の寿命

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CSound;

//*****************************************************************************
// バレットクラス定義
//*****************************************************************************
class CBullet :public CPolygon3D
{
public:
	CBullet();
	~CBullet();

	// 列挙型宣言
	typedef enum
	{
		BULLET_NONE = 0,
		BULLET_PLAYER,
		BULLET_ENEMY
	}BULLET_TYPE;

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float rot, BULLET_TYPE type);
	void Update();
	void Draw();
	void Uninit();
	static HRESULT Load(void);
	static void Unload(void);
	static CBullet * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float rot, BULLET_TYPE type);
private:
	bool BulletHit(D3DXVECTOR3 pos, CSound *pSound);
	static LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	D3DXVECTOR3						m_move;		    // ポリゴンの移動量
	int								m_nLife;		// 弾の寿命
	D3DXVECTOR3						m_rot;			// 回転する角度
	BULLET_TYPE						m_BulletType;
};
#endif