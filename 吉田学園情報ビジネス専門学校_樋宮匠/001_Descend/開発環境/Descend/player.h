//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _PLAYER_H_
#define _PLAYER_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FRICTION 0.2f		 // 慣性
#define PLAYER_SEG_X 1.0f	 // プレイヤーテクスチャの横分割数
#define PLAYER_SEG_Y 1.0f	 // プレイヤーテクスチャの縦分割数
#define PLAYER_SPEED 5.0f	 // プレイヤーの移動速度
#define PLAYER_GRAVITY -5.0f  // 重力の強さ
#define FIRE_RATE 10

//*****************************************************************************
// プレイヤークラス定義
//*****************************************************************************
class CPlayer :public CPolygon3D
{
public:
	CPlayer();
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos);
	void Update();
	void Draw();
	void Uninit();
	void Collision(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	static CPlayer * Create(D3DXVECTOR3 pos);
	static void Damage(void);
	static D3DXVECTOR3 GetPlayerPos(void) { return m_pos; }
	static D3DXVECTOR3 GetPlayerMove(void) { return m_move; }
	static void AddCombo(void) { m_nCombo++; }
	static int GetCombo(void) { return m_nCombo; }
	static bool IsCameraFollowing(void) { return m_bCameraFollowing; }
private:
	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
	static D3DXVECTOR3				m_pos;				// ポリゴンの座標
	static D3DXVECTOR3				m_move;				// ポリゴンの移動量
	static bool						m_bCameraFollowing;
	static int						m_nCombo;
	// メンバ変数
	float							m_fGravDest;
	int								m_nFireRate;
	int								m_nInvisibleTimer;
	int								m_nDir;
	int								m_nCountDeath;
	bool							m_bJump;
	bool							m_bShot;
	bool							m_bHit;
	bool							m_bDeath;
};
#endif