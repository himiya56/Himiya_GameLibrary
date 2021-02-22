//=============================================================================
//
// プレイヤーカメラ処理 [camera.h]
// Author : 樋宮匠
//
//=============================================================================
#ifndef _CAMERA_PLAYER_H_
#define _CAMERA_PLAYER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//================================================
// プレイヤーカメラクラス宣言
//================================================
class CCameraPlayer :public CCamera
{
public:
	CCameraPlayer();
	~CCameraPlayer();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void FollowMove(void);

	void SetVerticalAngle(const float fAngle) { m_fVerticalAngle = fAngle; }		// カメラ縦回転の角度設定
	void SetHorizontalAngle(const float fAngle) { m_fHorizontalAngle = fAngle; }	// カメラ縦回転の角度設定

	static CCameraPlayer *Create(void);
private:
	D3DXVECTOR3 m_posV;
	D3DXVECTOR3 m_posR;
	D3DXVECTOR3 m_posVDest;								// 座標目標値
	D3DXVECTOR3 m_posRDest;								// 注視点目標値
	D3DXVECTOR3 m_FollowPos;							// カメラ追従対象の座標

	float		m_fOffset;								// 注視点からの距離
	float		m_fVerticalAngle;						// 球面座標カメラ縦回転
	float		m_fHorizontalAngle;						// 球面座標カメラ横回転
};

#endif