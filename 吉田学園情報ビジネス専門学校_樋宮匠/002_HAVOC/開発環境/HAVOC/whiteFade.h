//====================================================================
//
// ���t�F�[�h�̏��� (whiteFade.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _WHITEFADE_H_
#define _WHITEFADE_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// �}�N����`
//================================================
#define WHITE_FADE_RATE (0.005f)                                                // ���t�F�[�h����
#define WHITE_FADE_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)) // ���t�F�[�h�̈ʒu
#define WHITE_FADE_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))        // ���t�F�[�h�̃T�C�Y
#define WHITE_FADE_END_TIME (240)                                               // ���t�F�[�h���I��鎞��
#define WHITE_FADE_CONSIDER_TRANSPARENT (0.3f)                                  // �����Ƃ݂Ȃ����l�̒l
#define WHITE_FADE_START_COLOR  D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f)               // ���t�F�[�h�̊J�n�F

//================================================
// �N���X�錾
//================================================

// ���t�F�[�h�N���X
class CWhiteFade : public CScene2D
{
public:
    // ���t�F�[�h�̎��
    typedef enum
    {
        TYPE_STAGE1 = 0,	// �X�e�[�W1
        TYPE_STAGE2,	    // �X�e�[�W2
        TYPE_MAX		    // ��ނ̍ő吔
    }TYPE;

    CWhiteFade();
    ~CWhiteFade();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CWhiteFade *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, D3DXCOLOR col, TYPE type);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // �ǂݍ��񂾃e�N�X�`����ۑ�����
    D3DXCOLOR m_col;    // �F
    int m_nTimer;       // �J�E���^
    TYPE m_type;        // ���t�F�[�h�̎��

    bool m_bAlphaDown;  // �����x��������t���O
};

#endif