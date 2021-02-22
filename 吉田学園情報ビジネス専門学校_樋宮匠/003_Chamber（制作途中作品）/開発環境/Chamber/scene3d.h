//====================================================================
//
// 3D���f������ (scene3d.h)
// Author : ��{��
//
//====================================================================
#ifndef _SCENE_3D_H_
#define _SCENE_3D_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POLYGON_3D_SIZE 1000

//================================================
// �N���X�錾
//================================================

// 3D���f���N���X
class CScene3D : public CObject
{
public:
    CScene3D();
    virtual ~CScene3D();

    virtual HRESULT Init(void);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void BindMesh(const LPD3DXMESH pMesh, const  LPD3DXBUFFER pBuffMat, const DWORD nNumMat);
    void BindTexture(const LPDIRECT3DTEXTURE9 pTex);
    void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
    void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
    void SetScale(const D3DXVECTOR3 scale) { m_scale = scale; }
	void SetCollisionSize(const D3DXVECTOR3 size) { m_collisionSize = size; }

    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetRot(void) { return m_rot; }
    D3DXVECTOR3 GetScale(void) { return m_scale; }
	D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }          // �Փ˂̑傫����Ԃ�

private:
    LPDIRECT3DTEXTURE9		m_pTexture;
    LPD3DXMESH				m_pMesh;
    LPD3DXBUFFER			m_pBuffMat;
    DWORD					m_nNumMat;
    D3DXMATRIX			    m_mtxWorld;		// ���[���h�}�g���b�N�X
    D3DXVECTOR3				m_pos;			// ���W
    D3DXVECTOR3				m_rot;			// ��]
    D3DXVECTOR3				m_scale;
	D3DXVECTOR3				m_collisionSize;     // �����蔻��̑傫��
};

#endif