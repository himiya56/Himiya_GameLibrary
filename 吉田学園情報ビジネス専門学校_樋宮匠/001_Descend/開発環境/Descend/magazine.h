//=============================================================================
//
// マガジン処理 [magazine.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _MAGAZINE_H_
#define _MAGAZINE_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon2d.h"
#include "map.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MAGAZINE_POLYGON  2   // ポリゴンの数
#define MAX_MAG				  10  // マガジンの最大装填数

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CMagazine :public CObject
{
public:
	CMagazine();
	~CMagazine();

	// 列挙型宣言
	typedef enum
	{
		TEX_MAG_0 = 0,
		TEX_MAG_01,
		TEX_MAG_MAX
	}TEX_MAG;

	HRESULT Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CMagazine * Create(void);
	static void SubtractMag(const int Value);
	static void SetMagFull(void) { m_nMag = MAX_MAG; }
	static void AddMag(void);
	static int	GetMag(void) { return m_nMag; }
private:
	static LPDIRECT3DTEXTURE9		m_apTexture[TEX_MAG_MAX];	 // テクスチャへのポインタ
	static int						m_nMag;
	CPolygon2D						*m_apPolygon[MAX_MAGAZINE_POLYGON];
	float							m_fRot = D3DXToRadian(0.0f);	 // 回転する角度
	float							m_fSizeX = POLYGON_SIZE * MAP_WIDTH;
	float							m_fSizeY = SCREEN_HEIGHT;
	float							m_fTexUVY;					     // テクスチャUV移動用変数
};
#endif