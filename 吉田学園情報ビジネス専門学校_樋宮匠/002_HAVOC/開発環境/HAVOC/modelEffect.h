//====================================================================
//
// ���f���G�t�F�N�g�̏��� (modelEffect.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _MODELEFFECT_H_
#define _MODELEFFECT_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// �}�N����`
//================================================

// �c������
#define MODEL_EFFECT_MAX_LIFE 600

// ��]�ʂ̍ő�l/�ŏ��l
#define MODEL_EFFECT_ROT_MAX 5000
#define MODEL_EFFECT_ROT_MIN 2000

// �w�
#define MODEL_EFFECT_SPIDER_SPLATTER_FORCE_MAX 10000                // �ł��グ��ő�l
#define MODEL_EFFECT_SPIDER_SPLATTER_FORCE_MIN 5000                 // �ł��グ��ŏ��l

// �A��
#define MODEL_EFFECT_ANT_SPLATTER_FORCE_MAX 7500                    // �ł��グ��ő�l
#define MODEL_EFFECT_ANT_SPLATTER_FORCE_MIN 4500                    // �ł��グ��ŏ��l

// �v���C���[
#define MODEL_EFFECT_X_SPLATTER_FORCE_MAX 4500                      // �ł��グ��ő�l
#define MODEL_EFFECT_X_SPLATTER_FORCE_MIN 3500                      // �ł��グ��ŏ��l

// UFO
#define MODEL_EFFECT_UFO_EXPLOSION_RADIUS 750.0f                    // �����̔��a
#define MODEL_EFFECT_UFO_ROT_MAX 1500                               // ��]�̍ő�l
#define MODEL_EFFECT_UFO_ROT_MIN 500                                // ��]�̍ŏ��l

// ���C�o�[��
#define MODEL_EFFECT_WYVERN_SPEED_Y_MAX 7000                        // ���n���ɃK���L�����ł����ő呬�xY
#define MODEL_EFFECT_WYVERN_SPEED_Y_MIN 5000                        // ���n���ɃK���L�����ł����ŏ����xY
#define MODEL_EFFECT_WYVERN_ROT_MAX 500                             // ��]�̍ő�l
#define MODEL_EFFECT_WYVERN_ROT_MIN 250                             // ��]�̍ŏ��l

// �|������
#define MODEL_EFFECT_FALL_DOWN_FIRST_SPEED D3DXToRadian(0.001f)     // �ŏ��̑��x
#define MODEL_EFFECT_FALL_DOWN_ACCEL_SPEED D3DXToRadian(0.03f)      // �|��Ă������x

// ���󂷂����
#define MODEL_EFFECT_COLLAPSE_NUMBER_ONCE 5                         // �K���L����x�ɐ������鐔
#define MODEL_EFFECT_SMALL_COLLAPSE_NUMBER_ONCE 3                   // �������K���L����x�ɐ������鐔
#define MODEL_EFFECT_SMALL_COLLAPSE_LIFE 180                        // �������K���L�̎c������
#define MODEL_EFFECT_BIG_COLLAPSE_RADIUS 150                        // �傫���K���L�̔������a
#define MODEL_EFFECT_COLLAPSE_ROT_MAX 1500                          // �K���L����]����ō����x
#define MODEL_EFFECT_COLLAPSE_ROT_MIN 500                           // �K���L����]����Œᑬ�x
#define MODEL_EFFECT_COLLAPSE_SPEED_MAX 2500                        // �K���L�����ł����ő呬�x

// ���n���̒n����G�t�F�N�g(���󂷂���̂̔h��)
#define MODEL_EFFECT_LANDING_CRACK_SPEED_Y_MAX 3500                 // ���n���ɃK���L�����ł����ő呬�xY
#define MODEL_EFFECT_LANDING_CRACK_SPEED_Y_MIN 1500                 // ���n���ɃK���L�����ł����ŏ����xY
#define MODEL_EFFECT_LANDING_CRACK_RADIUS 200.0f                    // ���n���ɃK���L����Ԕ��a

// �����G�t�F�N�g(���󂷂���̂̔h��)
#define MODEL_EFFECT_BLUE_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE 1    // ���������ɏ������K���L����x�ɐ������鐔
#define MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV1 15    // �������ɏ������K���L����x�ɐ������鐔lv1
#define MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV2 20    // �������ɏ������K���L����x�ɐ������鐔lv2
#define MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV3 25    // �������ɏ������K���L����x�ɐ������鐔lv3
#define MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV4 30    // �������ɏ������K���L����x�ɐ������鐔lv4
#define MODEL_EFFECT_EXPLOSION_SPEED_Y_MAX 7000                     // �������ɃK���L�����ł����ő呬�xY
#define MODEL_EFFECT_EXPLOSION_SPEED_Y_MIN 4000                     // �������ɃK���L�����ł����ŏ����xY

// �G�𐶐����闋
#define MODEL_EFFECT_KAMI_EXTEND_RATE 0.2f                          // �g�傷�銄��
#define MODEL_EFFECT_KAMI_SCALE_DOWN_RATE 0.6f                      // �k�����銄��
#define MODEL_EFFECT_KAMI_ROT_MAX 300                               // ��]���x�ő�
#define MODEL_EFFECT_KAMI_ROT_MIN 100                               // ��]���x�ŏ�
#define MODEL_EFFECT_KAMI_WHOLE_FRAME 240                           // �S�̃t���[��
#define MODEL_EFFECT_KAMI_CHANGE_BORDER_FRAME 60                    // ���ڂ̃t���[��

// ��
#define MODEL_EFFECT_SPIKE_EXTEND_RATE 0.2f                         // �g�傷�銄��
#define MODEL_EFFECT_SPIKE_EXTEND_MAX 10.0f                         // �g��̍ő�l
#define MODEL_EFFECT_SPIKE_SPAWN_RATE 5                             // �������郌�[�g

// ���n����G�t�F�N�g�́A���n���̌X����
#define MODEL_EFFECT_TILT_RATE_X D3DXToRadian(-45.0f)
#define MODEL_EFFECT_TILT_RATE_Z D3DXToRadian(-30.0f)

//================================================
// �N���X�錾
//================================================

// ���f���G�t�F�N�g�N���X
class CModelEffect : public CScene3D
{
public:

    // ���f���G�t�F�N�g�̎��
    typedef enum
    {
        TYPE_YELLOW_SPIDER_BODY,      // ���w偂̑�
        TYPE_YELLOW_SPIDER_STOMACH,   // ���w偂̕�
        TYPE_YELLOW_SPIDER_LEG,       // ���w偂̑�
        TYPE_BLACK_SPIDER_BODY,       // ���单�w偂̑�
        TYPE_BLACK_SPIDER_STOMACH,    // ���单�w偂̕�
        TYPE_BLACK_SPIDER_LEG,        // ���单�w偂̑�
        TYPE_SIGNAL,                  // �M���@
        TYPE_LIGHT,                   // �X��
        TYPE_LIGHT2,                  // �X��2
        TYPE_DEBRI_LARGE_001,         // �K���L��1
        TYPE_DEBRI_LARGE_002,         // �K���L��2
        TYPE_DEBRI_LARGE_003,         // �K���L��3
        TYPE_DEBRI_SMALL_001,         // �K���L��1
        TYPE_DEBRI_SMALL_002,         // �K���L��2
        TYPE_DEBRI_SMALL_003,         // �K���L��3
        TYPE_WHITE_ANT_BODY,          // ���A���̑�
        TYPE_WHITE_ANT_STOMACH,       // ���A���̕�
        TYPE_WHITE_ANT_LEG,           // ���A���̑�
        TYPE_BLACK_ANT_BODY,          // ���单�A���̑�
        TYPE_BLACK_ANT_STOMACH,       // ���单�A���̕�
        TYPE_BLACK_ANT_LEG,           // ���单�A���̑�
        TYPE_WOOD1,                   // ��1
        TYPE_WOOD2,                   // ��2
        TYPE_WOOD3,                   // ��3
        TYPE_GUARDRAIL,               // �K�[�h���[��
        TYPE_SIGN_EMAR,               // ���ӃT�C��
        TYPE_SIGN_INTER,              // �\���H�T�C��
        TYPE_SIGN_NO_PARK,            // ���ԋ֎~�T�C�� 
        TYPE_UFO,                     // UFO
        TYPE_UFO_WING,                // UFO�̗�
        TYPE_KAMI,                    // ��
        TYPE_CRYSTAL,                 // �N���X�^��
        TYPE_METAL,                   // �S��
        TYPE_MINI_CRYSTAL,            // �~�j�N���X�^��
        TYPE_LONG_CRYSTAL,            // �����O�N���X�^��
        TYPE_X_BODY,                  // X��
        TYPE_X_FOOT,                  // X��
        TYPE_X_LHAND,                 // X����
        TYPE_X_RHAND,                 // X�E��
        TYPE_SPIKE,                   // ��
        TYPE_TAIL,                    // �K��
        TYPE_RIGHT_WING,              // �E��
        TYPE_LEFT_WING,               // ����
        TYPE_WYVERN_BODY,             // ���C�o�[����
        TYPE_WYVERN_HEAD,             // ���C�o�[����
        TYPE_WYVERN_NECK,             // ���C�o�[����
        TYPE_WYVERN_ARM,              // ���C�o�[���r
        TYPE_MAX                      // ��ނ̍ő吔
    }TYPE;

    CModelEffect();
    ~CModelEffect();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CModelEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 rotMove, TYPE type);

    void ExplosionEffect(void);                             // �����G�t�F�N�g
    void SpawnEnemy(void);                                  // �G���X�|�[��

private:
    static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // �ǂݍ��ރe�N�X�`��
    static LPD3DXMESH				m_apMesh[TYPE_MAX];     // �ǂݍ��ރ��b�V��
    static LPD3DXBUFFER				m_apBuffMat[TYPE_MAX];  // �ǂݍ��ރ}�e���A���̃o�b�t�@
    static DWORD					m_aNumMat[TYPE_MAX];    // �ǂݍ��ރ}�e���A����

    TYPE m_type;                                            // ���

    int m_nLife;                                            // ����
    D3DXVECTOR3 m_move;                                     // �ړ���
    D3DXVECTOR3 m_rotMove;                                  // ��]�ړ���
    bool m_bShootUp;                                        // �ł��グ�邩�ǂ���
    bool m_bFallDown;                                       // �|��Ă������ǂ���

    bool m_bUseExplosion;                                   // �������g�����ǂ���
    bool m_bUseLanding;                                     // ���n�������ǂ���
};

#endif