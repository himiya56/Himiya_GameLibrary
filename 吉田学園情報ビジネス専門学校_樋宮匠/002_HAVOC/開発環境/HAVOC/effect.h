//====================================================================
//
// �G�t�F�N�g�̏��� (effect.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// �}�N����`
//================================================

// �v���C���[
#define EFFECT_2D_PLAYER_FIRST_POS D3DXVECTOR3(-200.0f, SCREEN_HEIGHT / 2, 0.0f)
#define EFFECT_2D_PLAYER_SIZE D3DXVECTOR3(450.0f, 450.0f, 0.0f)
#define EFFECT_2D_PLAYER_FIRST_MOVE_VALUE D3DXVECTOR3(50.0f, 0.0f, 0.0f)
#define EFFECT_2D_PLAYER_MOVE_DOWN_RATE 0.96f
#define EFFECT_2D_PLAYER_STOP_MOVE_VALUE 0.1f

// �v���C���[�̉e
#define EFFECT_2D_PLAYER_SHADOW_FADE_RATE 0.1f

//================================================
// �N���X�錾
//================================================

// �G�t�F�N�g�N���X
class CEffect : public CScene2D
{
public:
    // �G�t�F�N�g�̎��
    typedef enum
    {
        TYPE_PLAYER = 0,	// �v���C���[
        TYPE_PLAYER_SHADOW,	// �v���C���[�̉e
        TYPE_MAX			// ��ނ̍ő吔
    }TYPE;
    CEffect();
    ~CEffect();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, float fAngle, float fRotSpeed, D3DXCOLOR col, TYPE type);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // �ǂݍ��񂾃e�N�X�`����ۑ�����
    D3DXVECTOR3 m_move;                                // �ړ���
    D3DXCOLOR m_col;                                   // �F
    int m_nLife;                                       // ���C�t
    TYPE m_type;                                       // �G�t�F�N�g�̎��

    bool m_bAlphaDown;                                 // �����x��������t���O

    float m_fRotSpeed;                                 // ��]���x
    float m_fAngle;                                    // �p�x
};

#endif