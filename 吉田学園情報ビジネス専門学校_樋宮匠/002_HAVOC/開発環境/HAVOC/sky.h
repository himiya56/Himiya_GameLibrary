//====================================================================
//
// �X�J�C�{�b�N�X�̏��� (character.h)
// Author : ��{��
//
//====================================================================
#ifndef _SKY_H_
#define _SKY_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// �}�N����`
//================================================
#define SKYBOX_POS D3DXVECTOR3(6500.0f, 0.0f, 6500.0f)
#define SKYBOX_SCALE D3DXVECTOR3(150.0f, 150.0f, 150.0f)
#define SKYBOX_ROT_SPEED D3DXToRadian(0.05f)

//================================================
// �N���X�錾
//================================================

// UI�N���X
class CSky : public CScene3D
{
public:
    CSky();
    ~CSky();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CSky *Create(void);
private:
    static LPDIRECT3DTEXTURE9		m_pTexture;
    static LPD3DXMESH				m_pMesh;
    static LPD3DXBUFFER				m_pBuffMat;
    static DWORD					m_nNumMat;
    D3DXVECTOR3						m_rot;
};

#endif