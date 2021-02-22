//====================================================================
//
// �ǂ̏��� (wall.h)
// Author : ��{��
//
//====================================================================
#ifndef _WALL_H_
#define _WALL_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "billboard.h"

//================================================
// �}�N����`
//================================================

//================================================
// �N���X�錾
//================================================

// �ǃN���X
class CWall :public CBillboard
{
public:
    CWall();
    ~CWall();

    // ��k�����̕�
    typedef enum
    {
        TYPE_WALL_NORTH = 0,
        TYPE_WALL_SOUTH,
        TYPE_WALL_EAST,
        TYPE_WALL_WEST,
        TYPE_MAX
    }TYPE;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CWall *Create(TYPE type, float fMapWidth);
    static void SetWall(float fMapWidth);

private:
    static LPDIRECT3DTEXTURE9		m_pTexture;				// �ǂݍ��ރe�N�X�`��
    TYPE							m_type;					// �ǂ̃^�C�v
    float							m_fMapWidth;			// �}�b�v�T�C�Y�i�[�p
    float							m_fTex;					// �e�N�X�`��UV���W�ύX�p
    int								m_nAlpha;				// �A���t�@�l���Z�p
    bool							m_bAddAlpha;			// �A���t�@�l�����Z���邩�ǂ���
};

#endif
