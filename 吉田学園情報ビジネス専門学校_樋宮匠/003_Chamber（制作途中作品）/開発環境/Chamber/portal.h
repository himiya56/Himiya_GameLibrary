//=============================================================================
//
// ポータル処理 [portal.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _PORTAL_H_
#define _PORTAL_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"
#include "camera_portal.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PORTAL_WIDTH 200.0f
#define PORTAL_HEIGHT 400.0f
#define RENDER_TO_TEXTURE_FREQUENCY FPS
#define PORTAL_TEXTURE_UV 0.375f, 0.625f, 0.0f, 1.0f
#define PORTAL_RANGE 50.0f
#define PORTAL_COOL_DOWN 60
#define PORTAL_POLYGON_NUM 2 * CCameraPortal::COL_MAX
#define PORTAL_INIT_POS D3DXVECTOR3(0.0f, -PORTAL_HEIGHT, 0.0f)

//*****************************************************************************
// プレイヤークラス定義
//*****************************************************************************
class CPortal :public CObject
{
public:
	CPortal();
	~CPortal();
	HRESULT Init(void);
	void Update(void);
	void TeleportToPairPortal(void);
	void ResetPortal(void);
	void Draw(void);
	void Uninit(void);
	static CPortal *Create(void);
	void SetPortal(D3DXVECTOR3 pos, D3DXVECTOR3 angle, CCameraPortal::COLOR col);
	void SetCollisionSize(const D3DXVECTOR3 size) { m_collisionSize = size; }
	D3DXVECTOR3 GetPos(CCameraPortal::COLOR col);
	CCameraPortal *GetPortalCam(CCameraPortal::COLOR col) { return m_pCamera[col]; }
	CPolygon3D *GetPortalPolygon(CCameraPortal::COLOR col) { return m_apPolygon3D[col]; }
	D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }      // 衝突の大きさを返す

	static HRESULT Load(void);
	static void Unload(void);
private:
	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9		m_pTexture[PORTAL_POLYGON_NUM];		// テクスチャへのポインタ

	// メンバ変数
	CPolygon3D *m_apPolygon3D[PORTAL_POLYGON_NUM];						// 3Dポリゴンへのポインタ
	CCameraPortal *m_pCamera[CCameraPortal::COL_MAX];					// portal用のカメラ
	bool m_bUse[CCameraPortal::COL_MAX];								// portalを出しているかどうか
	bool m_bAbleToPortal;												// portalを使用できるか
	int  m_nRenderToTexture;											// テクスチャへ描画する時用のフラグ
	int  m_nCountUsePortal;												// portalのクールダウン
	D3DXVECTOR3 m_collisionSize;										// 当たり判定の大きさ
};
#endif