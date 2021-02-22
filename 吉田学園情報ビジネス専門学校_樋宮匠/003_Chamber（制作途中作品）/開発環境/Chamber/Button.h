//====================================================================
//
// �{�^���̏��� (button.h)
// Author : ��{��
//
//====================================================================
#ifndef _BUTTON_H_
#define _BUTTON_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// �}�N����`
//================================================

//================================================
// �N���X�錾
//================================================

// UI�N���X
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