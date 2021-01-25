//=============================================================================
//
// 敵処理 [enemy.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _ENEMY_H_
#define _ENEMY_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_SEG_X 1.0f   // プレイヤーテクスチャの横分割数
#define ENEMY_SEG_Y 1.0f   // プレイヤーテクスチャの縦分割数
#define SLIME_LIFE 10
#define BAT_LIFE 10
#define ENEMY_SPEED 1

//*****************************************************************************
// バレットクラス定義
//*****************************************************************************
class CEnemy :public CPolygon3D
{
public:
	CEnemy();
	~CEnemy();
	typedef enum
	{
		TYPE_SLIME = 0,
		TYPE_BAT,
		TYPE_EYE,
		TYPE_MAX
	}ENEMY_TYPE;

	HRESULT Init(D3DXVECTOR3 pos, int life, ENEMY_TYPE type);
	void Update();
	void Draw();
	void Uninit();
	void Collision(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	static CEnemy * Create(D3DXVECTOR3 pos, ENEMY_TYPE type);
	ENEMY_TYPE GetEnemyType(void) { return m_enemyType; }
private:
	static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];	    // テクスチャへのポインタ
	ENEMY_TYPE						m_enemyType;	// エネミータイプ
	int								m_nLife;
	int								m_nCountMove;
	D3DXVECTOR3						m_pos;
	D3DXVECTOR3						m_move;			// ポリゴンの移動量
	D3DXVECTOR3						m_rot;
	bool							m_bChase;
};
#endif