//====================================================================
//
// �C�̏��� (ocean.h)
// Author : ��{��
//
//====================================================================
#ifndef _OCEAN_H_
#define _OCEAN_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "billboard.h"

//================================================
// �}�N����`
//================================================
#define OCEAN_SIZE		 30000.0f									// �C�̃T�C�Y

#define SURFACE_0_POS    D3DXVECTOR3(6500.0f, -350.0f, 6500.0f)		// ���w�̍��W
#define SURFACE_1_POS    D3DXVECTOR3(6500.0f, -250.0f, 6500.0f)		// ���w�̍��W
#define SURFACE_2_POS    D3DXVECTOR3(6500.0f, -150.0f, 6500.0f)		// ��w�̍��W

#define MIDDLE_ALPHA_RATE	1										// ���w�̃A���t�@�l���Z���[�g
#define TOP_ALPHA_RATE		2										// ��w�̃A���t�@�l���Z���[�g

#define MIDDLE_MOVE_TEX		0.0001f									// ���w�̃e�N�X�`���ړ���
#define TOP_MOVE_TEX		-0.0001f								// ��w�̃e�N�X�`���ړ���

#define ALPHA_MAX			250										// �A���t�@�l���
#define ALPHA_MIN			80										// �A���t�@�l����

//================================================
// �N���X�錾
//================================================

// �|�[�Y�N���X
class COcean :public CBillboard
{
public:
    COcean();
    ~COcean();

    // �������傫���ق�Y���W����
    typedef enum
    {
        TYPE_SURFACE_0 = 0,     // ���w
        TYPE_SURFACE_1,         // ���w
        TYPE_SURFACE_2,         // ��w
        TYPE_MAX
    }TYPE;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    void CalcAlpha(int nAlphaRate);
    static HRESULT Load(void);
    static void Unload(void);
    static COcean *Create(TYPE type);
private:
    static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // �ǂݍ��ރe�N�X�`��
    TYPE							m_type;					// �C�̃^�C�v
    float							m_fTexX;				// �e�N�X�`��X���W
    float							m_fTexY;				// �e�N�X�`��Y���W
    int								m_nAlpha;				// �A���t�@�l
    bool							m_bAddAlpha;			// �A���t�@�l�����Z���邩�ǂ���
};

#endif