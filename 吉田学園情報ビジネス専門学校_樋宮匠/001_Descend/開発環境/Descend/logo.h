//=============================================================================
//
// タイトルロゴ処理 [logo.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _LOGO_H_
#define _LOGO_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon3d.h"

//*****************************************************************************
// タイトルロゴクラス定義
//*****************************************************************************
class CLogo :public CPolygon3D
{
public:
	CLogo();
	~CLogo();
	HRESULT Init(D3DXVECTOR3 pos);
	void Update(void);
	void Draw(void);
	void Uninit(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CLogo * Create(D3DXVECTOR3 pos);
	static bool GetPlaying(void) { return m_bPlay; }
	static void SetPlaying(bool bPlay) { m_bPlay = bPlay; }
private:
	static LPDIRECT3DTEXTURE9		m_pTexture;	 // テクスチャへのポインタ
	static bool m_bPlay;
};

#endif