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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHAKE_LENGTH 10
#define SHAKE_SIZE 10

//================================================
// ライトクラス宣言
//================================================
class CCamera
{
public:
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	CCamera *Create(void);
	static void SetShake(void);
private:
	D3DXVECTOR3 m_posV;
	D3DXVECTOR3 m_posR;
	D3DXVECTOR3 m_posVDest;
	D3DXVECTOR3 m_posRDest;
	D3DXVECTOR3 m_vecU;
	D3DXMATRIX  m_mtxProjection;
	D3DXMATRIX  m_mtxView;
	static bool m_bShake;
	int			m_nShakeCount;
	static int	m_nOffset;
};

#endif