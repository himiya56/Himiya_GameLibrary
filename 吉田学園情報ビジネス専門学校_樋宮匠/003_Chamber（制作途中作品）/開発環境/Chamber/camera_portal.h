//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 樋宮匠
//
//=============================================================================
#ifndef _CAMERA_PORTAL_H_
#define _CAMERA_PORTAL_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RENDER_RATE FPS / 60
#define PORTAL_CAM_OFFSET 50.0f

//================================================
// ライトクラス宣言
//================================================
class CCameraPortal :public CCamera
{
public:
	CCameraPortal();
	~CCameraPortal();

	typedef enum
	{
		COL_BLUE = 0,
		COL_ORANGE,
		COL_MAX
	}COLOR;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void RenderToTexture(void);
	D3DXVECTOR3 RotateCameraToPlayerGaze(D3DXVECTOR3 pos);
	static CCameraPortal *Create(COLOR col);
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	void SetNormal(D3DXVECTOR3 normal) { m_normal = normal; }
private:
	LPDIRECT3DTEXTURE9				m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DSURFACE9				m_pTextureSurface;
	LPDIRECT3DSURFACE9				m_pDepthSurface;
	LPDIRECT3DSURFACE9				m_pBackBufferSurface;
	LPDIRECT3DSURFACE9				m_pBackDepthSurface;

	int								m_nRenderCounter;
	float							m_fVerticalAngle;			// 球面座標カメラ縦回転
	float							m_fHorizontalAngle;			// 球面座標カメラ横回転
	float							m_fAngle;					// 地面とポータルの角度

	D3DXVECTOR3						m_normal;					// ポータルの法線
	COLOR							m_col;						// ポータルの色
};

#endif