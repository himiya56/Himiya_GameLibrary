//====================================================================
//
// �f�o�b�O����̏��� (debug.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _DEBUG_H_
#define _DEBUG_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// �}�N����`
//================================================

#ifdef _DEBUG
// �����蔻��𐶐�����
#define COLLISION_TEST
#endif

//================================================
// �N���X�錾
//================================================

// �f�o�b�N�N���X
class CDebug : public CScene3D
{
public:

    // �f�o�b�N�̎��
    typedef enum
    {
        TYPE_PERMANENT = 0,	    // �i��
        TYPE_MOMENT,	        // �u��
        TYPE_MAX			    // ��ނ̍ő吔
    }TYPE;

    CDebug();
    ~CDebug();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CDebug *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);

    static void Command(void);  // �f�o�b�O�R�}���h

private:
    static LPDIRECT3DTEXTURE9		m_pTexture;
    static LPD3DXMESH				m_pMesh;
    static LPD3DXBUFFER				m_pBuffMat;
    static DWORD					m_nNumMat;

    int m_nLife;    // ����
    TYPE m_type;    // ���
    
};

#endif