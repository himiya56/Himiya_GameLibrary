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
#include "character.h"
#include "portal.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FRICTION 0.2f		 // 慣性
#define PLAYER_SEG_X 1.0f	 // プレイヤーテクスチャの横分割数
#define PLAYER_SEG_Y 1.0f	 // プレイヤーテクスチャの縦分割数
#define PLAYER_SPEED 10.0f	 // プレイヤーの移動速度
#define PLAYER_GRAVITY -5.0f  // 重力の強さ
#define FIRE_RATE 10
#define COLLLISION_SIZE D3DXVECTOR3(200.0f, 400.0f, 200.0f)

//*****************************************************************************
// プレイヤークラス定義
//*****************************************************************************
class CPlayer :public CCharacter
{
public:
	CPlayer();
	~CPlayer();

	// モデルの各パーツ
	typedef enum
	{
		PARTS_HIP = 0,
		PARTS_BODY,
		PARTS_HEAD,
		PARTS_RSHOULDER,
		PARTS_RARM,
		PARTS_RHAND,
		PARTS_LSHOULDER,
		PARTS_LARM,
		PARTS_LHAND,
		PARTS_RTHIGH,
		PARTS_RLEG,
		PARTS_RFOOT,
		PARTS_LTHIGH,
		PARTS_LLEG,
		PARTS_LFOOT,
		PARTS_MAX,
	}PARTS;

	HRESULT Init(void);
	void Update(void);
	void Move(void);
	void Draw(void);
	void Uninit(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CPlayer * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	D3DXVECTOR3 GetMove(void) { return m_move; }
private:
	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
	static LPD3DXMESH		m_pMesh[PARTS_MAX];
	static LPD3DXBUFFER		m_pBuffMat[PARTS_MAX];
	static DWORD			m_nNumMat[PARTS_MAX];

	// メンバ変数
	CPortal							*m_pPortal;			// ポータルのポインタ
	D3DXVECTOR3						m_move;				// ポリゴンの移動量

	float							m_fGravDest;
	bool							m_bJump;
	bool							m_bShot;
	bool							m_bHit;
	bool							m_bDeath;
};
#endif