#pragma once
//====================================================================
//
// キューブの処理 (cube.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _CUBE_H_
#define _CUBE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// マクロ定義
//================================================
#define COLLISION_SIZE D3DXVECTOR3(100.0f, 100.0f, 100.0f)

//================================================
// クラス宣言
//================================================

// UIクラス
class CCube : public CScene3D
{
public:
	CCube();
	~CCube();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CCube *Create(D3DXVECTOR3 pos);
private:
	static LPDIRECT3DTEXTURE9		m_pTexture;
	static LPD3DXMESH				m_pMesh;
	static LPD3DXBUFFER				m_pBuffMat;
	static DWORD					m_nNumMat;
	D3DXVECTOR3						m_rot;
};

#endif