//====================================================================
//
// ボタンの処理 (button.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _BUTTON_H_
#define _BUTTON_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// マクロ定義
//================================================

//================================================
// クラス宣言
//================================================

// UIクラス
class CButton : public CScene3D
{
public:
	CButton();
	~CButton();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CButton *Create(void);
private:
	static LPDIRECT3DTEXTURE9		m_pTexture;
	static LPD3DXMESH				m_pMesh;
	static LPD3DXBUFFER				m_pBuffMat;
	static DWORD					m_nNumMat;
	D3DXVECTOR3						m_rot;
};

#endif