//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 樋宮匠
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INIT_OFFSET			300.0f	// 注視点からの距離
#define CAMERA_FIT_ANGLE D3DXToRadian(90)

//================================================
// ライトクラス宣言
//================================================
class CCamera :public CObject
{
public:
	CCamera();
	~CCamera();

	typedef enum
	{
		CAM_BLUE = 0,
		CAM_ORANGE,
		CAM_PLAYER,
		CAM_MAX
	}CAMERA;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetCamera(CAMERA cam);

	void SetTargetPos(D3DXVECTOR3 pos) { m_posR = pos; }	// 注視点設定
	void SetPos(D3DXVECTOR3 pos) { m_posV = pos; }			// 座標設定
	void SetFOV(float fFOV) { m_fFOV = fFOV; }				// 視野角設定

	D3DXVECTOR3 GetTargtPos(void) { return m_posR; }		// 注視点取得
	D3DXVECTOR3 GetPos(void) { return m_posV; }				// 座標取得
private:
	D3DXVECTOR3 m_posV;
	D3DXVECTOR3 m_posR;
	D3DXVECTOR3 m_vecU;

	D3DXMATRIX  m_mtxProjection[CAM_MAX];
	D3DXMATRIX  m_mtxView[CAM_MAX];

	float m_fFOV;
};

#endif