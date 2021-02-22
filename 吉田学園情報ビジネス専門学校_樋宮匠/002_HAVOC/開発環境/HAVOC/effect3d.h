//=============================================================================
//
// �G�t�F�N�g3D���� [effect3d.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "billboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �G�t�F�N�g�̋��ʐݒ�
#define EFFECT_FIRE_ALPHA_TEST_BORDER 25                                         // ���̃��l�̃{�[�_�[�l
#define EFFECT_FLOAT_FROM_SURFACE_POS 5.0f                                       // �ʂ��畂������l

// �Ԃ���
#define EFFECT_RED_FIRE_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)                     // �Ԃ����̑傫��
#define EFFECT_RED_FIRE_FIRST_COLOR D3DXCOLOR(1.0f,0.99f,0.0f,1.0f)              // �Ԃ����̐F
#define EFFECT_YELLOW_FIRE_TO_RED_RATE 0.05f                                     // �Ԃɋ߂Â��Ă�������
#define EFFECT_RED_FIRE_FADE_OUT_RATE 0.02f                                      // �Ԃ����̃t�F�[�h�A�E�g����

// AR�̃g���C��
#define EFFECT_AR_TRAIL_COLOR D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)                     // AR�̃g���C���̐F
#define EFFECT_AR_TRAIL_FADE_OUT_RATE 5.0f                                       // AR�̃g���C���̃T�C�Y�k����

// AR�̃g���C���̏���
#define EFFECT_AR_TRAIL_DECORATION_COLOR D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)          // AR�̃g���C���̏���̐F
#define EFFECT_AR_TRAIL_DECORATION_FADE_OUT_RATE 5.0f                            // AR�̃g���C���̏���̃T�C�Y�k����
#define EFFECT_AR_TRAIL_DECORATION_MAX_SPEED 1000                                // AR�̃g���C���̏���̃T�C�Y�k����

// �E�F�[�u(���g�p)
#define EFFECT_WAVE_EXTEND_SIZE 20.0f
#define EFFECT_WAVE_FADE_OUT_RATE 0.005f

// �`���[�W/���̉�
#define EFFECT_CHARGE_SIZE D3DXVECTOR3(80.0f, 80.0f, 0.0f)                       // �`���[�W�̑傫��
#define EFFECT_CHARGE_SPEED 35.0f                                                // �`���[�W�̑���(���Ƃ�30.0f)
#define EFFECT_CHARGE_CONSIDER_CLOSE_DEST 40.0f                                  // �`���[�W�̖ړI�n�ɋ߂��Ƃ݂Ȃ��l
#define EFFECT_CHARGE_CONSIDER_CLOSE_DEST_Y 40.0f                                // �`���[�W�̖ړI�n�ɋ߂��Ƃ݂Ȃ��l(�c)
#define EFFECT_CHARGE_SPREAD_MAX_X 700                                           // �`���[�W�̊g�U�̍ő�lX
#define EFFECT_CHARGE_SPREAD_MAX_Y 1000                                          // �`���[�W�̊g�U�̍ő�lY
#define EFFECT_CHARGE_SPREAD_MIN_Y 100                                           // �`���[�W�̊g�U�̍ŏ��lY
#define EFFECT_CHARGE_SPREAD_MAX_Z 700                                           // �`���[�W�̊g�U�̍ő�lZ
#define EFFECT_CHARGE_START_HOMING 90                                            // �`���[�W�̃z�[�~���O���n�܂�t���[����
#define EFFECT_CHARGE_TRAIL_COLOR D3DXCOLOR(1.0f,1.0f,0.0f,1.0f)                 // �`���[�W�̃g���C���̐F
#define EFFECT_CHARGE_TRAIL_FADE_OUT_RATE 2.5f                                   // �`���[�W�̃g���C���̃p�[�e�B�N���̃T�C�Y�k����
#define EFFECT_CHARGE_TRAIL_MOVE_CHANGE_RATE (-0.4f)                             // �`���[�W����g���C���̈ړ��ʂ�ς���Ƃ��̊���
#define EFFECT3D_PURPLE_FIRE_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)
#define EFFECT3D_PURPLE_FIRE_FIRST_COLOR D3DXCOLOR(0.835f,0.901f,1.0f,1.0f)      // ���̉��̐F
#define EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_R 0.025f                              // ���ɋ߂Â��Ă�������R
#define EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_G 0.05f                               // ���ɋ߂Â��Ă�������G
#define EFFECT3D_PURPLE_FIRE_FADE_OUT_RATE 0.01f                                 // ���̉��̃t�F�[�h�A�E�g����

// �ˌ��G�t�F�N�g
#define EFFECT_SCATTERING_BULLET_MAX_SIZE 9000                                   // �ő�T�C�Y
#define EFFECT_SCATTERING_BULLET_MIN_SIZE 5000                                   // �ŏ��T�C�Y
#define EFFECT_SCATTERING_BULLET_MAX_SPEED 900                                   // �ő呬�x
#define EFFECT_SCATTERING_BULLET_MIN_SPEED 400                                   // �ŏ����x
#define EFFECT_SCATTERING_BULLET_NUMBER_ONCE 15                                  // ��x�̌�15
#define EFFECT_SCATTERING_BULLET_COLOR D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)            // �F
#define EFFECT_SCATTERING_BULLET_FADE_OUT_RATE 5.0f                              // �p�[�e�B�N���̃T�C�Y�k����5.0f

// ���G�t�F�N�g
#define EFFECT_SMOKE_MAX_PATTERN 8                                               // ���̍ő�p�^�[��
#define EFFECT_SMOKE_UPDATE_PATTERN_SPEED 2                                      // ���̃p�^�[���X�V���x
#define EFFECT_SMOKE_COLOR_TO_BLACK_RATE 0.01f                                   // ���̍��ɋ߂Â��Ă�������
#define EFFECT_DESTROY_FADE_OUT_RATE 0.005f                                      // �j�󎞂̉��̏����Ă�������
#define EFFECT_DESTROY_MAX_SIZE 120000                                           // �j�󎞂̉��̍ő�T�C�Y
#define EFFECT_DESTROY_MIN_SIZE 60000                                            // �j�󎞂̉��̍ŏ��T�C�Y
#define EFFECT_DESTROY_NUMBER_ONCE 20                                            // �j�󎞂̉��̈�x�̌�
#define EFFECT_DESTROY_MAX_SPEED 800                                             // �j�󎞂̉��̍ő呬�x
#define EFFECT_DESTROY_MIN_SPEED 300                                             // �j�󎞂̉��̍ŏ����x

// �_�G�t�F�N�g
#define EFFECT_ACID_FIRST_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)                   // �_�̍ŏ��̑傫��
#define EFFECT_ACID_FIRST_COLOR D3DXCOLOR(0.6f, 1.0f, 0.0f, 1.0f)                // �_�̍ŏ��̐F
#define EFFECT_ACID_TO_GREEN_RATE_R 0.005f                                       // �_�̉��F�ɋ߂Â��Ă����lR
#define EFFECT_ACID_EXTEND_SIZE 10.0f                                            // �_�̊g�劄��
#define EFFECT_ACID_FADE_OUT_RATE 0.005f                                         // �_�̏����Ă�������
#define EFFECT_ACID_FLOAT_FROM_SURFACE_POS 6.0f                                  // �_�̕\�ʂ��痣���l

// �_�U���G�t�F�N�g
#define EFFECT_ACID_ATTACK_FIRST_COLOR D3DXCOLOR(0.36f, 0.282f, 0.815f, 1.0f)    // �_�U���̍ŏ��̐F
#define EFFECT_ACID_ATTACK_TO_PURPLE_RATE 0.0025f                                // �_�U���̎��F�ɋ߂Â��Ă����l

// �_�U���̃g���C���̏���
#define EFFECT_ACID_ATTACK_TRAIL_DECORATION_COLOR D3DXCOLOR(0.811f,0.415f,1.0f,1.0f) // �_�U���̃g���C���̏���̐F
#define EFFECT_ACID_ATTACK_TRAIL_DECORATION_FADE_OUT_RATE 5.0f                       // �_�U���̃g���C���̏���̃T�C�Y�k����
#define EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED 750                            // �_�U���̃g���C���̏���̍ő呬�x

// ���̃g���C��
#define EFFECT_BLOOD_TRAIL_COLOR D3DXCOLOR(0.0f, 0.627f, 0.505f, 1.0f)           // ���̃g���C���̐F
#define EFFECT_BLOOD_TRAIL_FADE_OUT_RATE 2.5f                                    // ���̃g���C���̃T�C�Y�k����

// ���̃g���C���̏���
#define EFFECT_BLOOD_TRAIL_DECORATION_COLOR D3DXCOLOR(0.0f,1.0f,0.0f,1.0f)       // ���̃g���C���̏���̐F
#define EFFECT_BLOOD_TRAIL_DECORATION_FADE_OUT_RATE 2.5f                         // ���̃g���C���̏���̃T�C�Y�k����
#define EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED 500                              // ���̃g���C���̏���̍ő呬�x

// �����G�t�F�N�g(���g�p)
#define EFFECT_SPRAY_FIRST_SIZE D3DXVECTOR3(300.0f, 300.0f, 0.0f)                // �����̍ŏ��̑傫��
#define EFFECT_SPRAY_FIRST_COLOR D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)               // �����̍ŏ��̐F
#define EFFECT_SPRAY_TO_YELLOW_RATE_R 0.01f                                      // �����̉��F�ɋ߂Â��Ă����lR
#define EFFECT_SPRAY_TO_YELLOW_RATE_B 0.005f                                     // �����̉��F�ɋ߂Â��Ă����lB
#define EFFECT_SPRAY_EXTEND_SIZE 5.0f                                            // �����̊g�劄��
#define EFFECT_SPRAY_FADE_OUT_RATE 0.01f                                         // �����̏����Ă�������
#define EFFECT_SPRAY_ROT_MAX 1500                                                // ��������]����ō����x
#define EFFECT_SPRAY_ROT_MIN 500                                                 // ��������]����Œᑬ�x
#define EFFECT_SPRAY_CONTROL 0.95f                                               // �����̓����𐧌�
#define EFFECT_SPRAY_MAX_SPEED 7000                                              // �����̍ő呬�x
#define EFFECT_SPRAY_MIN_SPEED 3000                                              // �����̍ŏ����x

// �����ւ̃q�b�g�G�t�F�N�g
#define EFFECT_HIT_LIVING_MAX_SIZE 22500                                         // �ő�T�C�Y
#define EFFECT_HIT_LIVING_MIN_SIZE 12500                                         // �ŏ��T�C�Y
#define EFFECT_HIT_LIVING_NUMBER_ONCE 15                                         // ��x�ɐ������鐔
#define EFFECT_HIT_LIVING_MAX_SPEED 1250                                         // �ő呬�x
#define EFFECT_HIT_LIVING_MIN_SPEED 500                                          // �ŏ����x
#define EFFECT_HIT_LIVING_COLOR D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)                // �F
#define EFFECT_HIT_LIVING_FADE_OUT_RATE 7.5f                                     // �t�F�[�h�A�E�g����

// �񐶕��ւ̃q�b�g�G�t�F�N�g
#define EFFECT_HIT_NON_LIVING_MAX_SIZE 15000                                     // �ő�T�C�Y
#define EFFECT_HIT_NON_LIVING_MIN_SIZE 7500                                      // �ŏ��T�C�Y
#define EFFECT_HIT_NON_LIVING_NUMBER_ONCE 15                                     // ��x�ɐ������鐔
#define EFFECT_HIT_NON_LIVING_MAX_SPEED 1250                                     // �ő呬�x
#define EFFECT_HIT_NON_LIVING_MIN_SPEED 500                                      // �ŏ����x
#define EFFECT_HIT_NON_LIVING_COLOR D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)            // �F
#define EFFECT_HIT_NON_LIVING_FADE_OUT_RATE 5.0f                                 // �t�F�[�h�A�E�g����

// �񕜃A�C�e���̃G�t�F�N�g
#define EFFECT_HEAL_ITEM_MAX_SPEED 800                                           // �ő呬�x
#define EFFECT_HEAL_ITEM_MIN_SPEED 300                                           // �ŏ����x
#define EFFECT_HEAL_ITEM_MAX_SIZE 12000                                          // �ő�T�C�Y
#define EFFECT_HEAL_ITEM_MIN_SIZE 8000                                           // �ŏ��T�C�Y
#define EFFECT_HEAL_ITEM_MAX_RADIUS 22500                                        // �ő唼�a
#define EFFECT_HEAL_ITEM_FADE_OUT_RATE 1.0f                                      // �t�F�[�h�A�E�g����
#define EFFECT_HEAL_ITEM_COLOR D3DXCOLOR(0.745f, 1.0f, 0.682f, 1.0f)             // �F

// �񕜃G�t�F�N�g
#define EFFECT_HEALING_NUMBER_ONCE 35                                            // ��x�ɐ������鐔
#define EFFECT_HEALING_MAX_SPEED 1700                                            // �ő呬�x
#define EFFECT_HEALING_MIN_SPEED 1200                                            // �ŏ����x
#define EFFECT_HEALING_MAX_SPEED_Y 1000                                          // �ő呬�xY
#define EFFECT_HEALING_MIN_SPEED_Y 700                                           // �ŏ����xY
#define EFFECT_HEALING_MAX_SIZE 20000                                            // �ő�T�C�Y
#define EFFECT_HEALING_MIN_SIZE 15000                                            // �ŏ��T�C�Y
#define EFFECT_HEALING_FADE_OUT_RATE 2.0f                                        // �t�F�[�h�A�E�g����
#define EFFECT_HEALING_COLOR D3DXCOLOR(0.407f, 0.956f, 0.878f, 1.0f)             // �F

// �����[�h�G�t�F�N�g
#define EFFECT_RELOADING_COLOR D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f)                 // �F
#define EFFECT_RELOADING_CENTER_COLOR D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.7f)          // ���S�F
#define EFFECT_RELOADING_CENTER_REDUCE_SIZE_RATE 0.8f                            // ���S�T�C�Y�k������
#define EFFECT_RELOADING_FADE_OUT_RATE 4.0f                                      // �t�F�[�h�A�E�g����
#define EFFECT_RELOADING_MAX_SIZE 11000                                          // �ő�T�C�Y
#define EFFECT_RELOADING_MIN_SIZE 8500                                           // �ŏ��T�C�Y
#define EFFECT_RELOADING_RADIUS 300.0f                                           // ���a
#define EFFECT_RELOADING_SPEED 27.5f                                             // ����

// �n����G�t�F�N�g
#define EFFECT_CRACK_SIZE D3DXVECTOR3(350.0f, 350.0f, 0.0f)                      // �傫��
#define EFFECT_CRACK_FADE_OUT_RATE 0.01f                                         // �t�F�[�h�A�E�g����

// ����̔��e�G�t�F�N�g
#define EFFECT_BOMB_CENTER_COLOR D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)               // ���S�F
#define EFFECT_BOMB_AROUND_COLOR D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f)               // ���ӐF

// �����G�t�F�N�g
#define EFFECT_EX_ALPHA_TEST_BORDER 100                                          // ���l�̃{�[�_�[�l
#define EFFECT_EX_DOWN_ALPHA_RATE 0.001f                                         // ���l�������Ă�������
#define EFFECT_EX_TO_RED_RATE 0.004f                                             // �ԐF�ɋ߂Â��Ă����l
#define EFFECT_EX_CHANGE_MOVE_START_COUNT 180                                    // �ړ��ʕω��̊J�n����
#define EFFECT_EX_CHANGE_MOVE_RATE -1.35f                                        // �ړ��ʕω��̊���
#define EFFECT_EX_ROT_MAX 1000                                                   // ��]����ō����x
#define EFFECT_EX_FADE_OUT_RATE_LV1 2.0f                                         // �t�F�[�h�A�E�g����lv1
#define EFFECT_EX_FADE_OUT_RATE_LV2 3.0f                                         // �t�F�[�h�A�E�g����lv2
#define EFFECT_EX_FADE_OUT_RATE_LV3 4.5f                                         // �t�F�[�h�A�E�g����lv3
#define EFFECT_EX_FADE_OUT_RATE_LV4 6.75f                                        // �t�F�[�h�A�E�g����lv4
#define EFFECT_EX_MAX_SPEED_LV1 700                                              // �ő呬�xlv1
#define EFFECT_EX_MAX_SPEED_LV2 950                                              // �ő呬�xlv2
#define EFFECT_EX_MAX_SPEED_LV3 1200                                             // �ő呬�xlv3
#define EFFECT_EX_MAX_SPEED_LV4 1450                                             // �ő呬�xlv4
#define EFFECT_EX_MAX_SPEED_Y 1000                                               // �ő呬�xY
#define EFFECT_EX_NUMBER_ONCE 2                                                  // 1F���Ƃɐ������鐔(0�ŐԁA1�ŉ��F)
#define EFFECT_EX_RED_COLOR D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f)                    // ��
#define EFFECT_EX_YELLOW_COLOR D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)                 // ��

// ����
#define EFFECT_BLUE_FIRE_FIRST_COLOR D3DXCOLOR(0.588f,0.937f,1.0f,1.0f)          // �����̐F
#define EFFECT_BLUE_FIRE_BLUE D3DXCOLOR(0.0f,0.0f,1.0f,1.0f)                     // ��
#define EFFECT_WHITE_FIRE_TO_BLUE_RATE 0.004f                                    // �ɋ߂Â��Ă�������
#define EFFECT_BLUE_FIRE_FADE_OUT_RATE 0.008f                                    // �����Ă�������
#define EFFECT_BLUE_FIRE_EXTEND_RATE 12.5f                                       // �g�劄��

// �N���X�^���ɓ��������ۂ̃G�t�F�N�g
#define EFFECT_HIT_CRYSTAL_COLOR D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.8f)               // �s���N
#define EFFECT_HIT_CRYSTAL_NUMBER_ONCE 15                                        // ��x�ɐ������鐔
#define EFFECT_HIT_CRYSTAL_SIZE D3DXVECTOR3(400.0f, 400.0f, 0.0f)                // �傫��
#define EFFECT_HIT_CRYSTAL_FADE_OUT_RATE 10.0f                                   // �t�F�[�h�A�E�g����
#define EFFECT_HIT_CRYSTAL_SPEED 20.0f                                           // ����

// �~�j�N���X�^���ɓ��������ۂ̃G�t�F�N�g�̑傫���ƃt�F�[�h�A�E�g�����Ƒ���
#define EFFECT_HIT_MINI_CRYSTAL_SIZE D3DXVECTOR3(200.0f, 200.0f, 0.0f)           // �傫��
#define EFFECT_HIT_MINI_CRYSTAL_FADE_OUT_RATE 5.0f                               // �t�F�[�h�A�E�g����
#define EFFECT_HIT_MINI_CRYSTAL_SPEED 10.0f                                      // ����

// ���C�o�[���̓��ɓ��������ۂ̃G�t�F�N�g�̑傫���ƃt�F�[�h�A�E�g�����Ƒ���
#define EFFECT_HIT_WYVERN_HEAD_SIZE D3DXVECTOR3(300.0f, 300.0f, 0.0f)           // �傫��
#define EFFECT_HIT_WYVERN_HEAD_FADE_OUT_RATE 7.5f                               // �t�F�[�h�A�E�g����
#define EFFECT_HIT_WYVERN_HEAD_SPEED 15.0f                                      // ����

// ���ӂ̒��S�G�t�F�N�g
#define EFFECT_BURST_CENTER_FIRST_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)          // �ŏ��̑傫��
#define EFFECT_BURST_CENTER_FIRST_COLOR D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)       // �ŏ��̐F
#define EFFECT_BURST_CENTER_TO_YELLOW_RATE_R 0.02f                              // ���F�ɋ߂Â��Ă����lR
#define EFFECT_BURST_CENTER_TO_RED_RATE_G 0.02f                                 // �ԐF�ɋ߂Â��Ă����lG
#define EFFECT_BURST_CENTER_EXTEND_SIZE 6.5f                                    // �g�劄��
#define EFFECT_BURST_CENTER_FADE_OUT_RATE 0.006f                                // �t�F�[�h�A�E�g����
#define EFFECT_BURST_CENTER_MAX_RADIUS 500                                      // �ő唼�a
#define EFFECT_BURST_CENTER_EXPLOSION_FRAME 120                                 // �����𐶐�����t���[����

// ���ӂ̎��ӃG�t�F�N�g
#define EFFECT_BURST_AROUND_FIRST_SIZE_RATE 0.6f                                // �ŏ��̑傫���̊���
#define EFFECT_BURST_AROUND_EXTEND_SIZE 5.5f                                    // �g�劄��
#define EFFECT_BURST_AROUND_FADE_OUT_RATE 0.0085f                               // �t�F�[�h�A�E�g����

//*****************************************************************************
// �G�t�F�N�g3D�N���X��`
//*****************************************************************************
class CEffect3D :public CBillboard
{
public:

    // �G�t�F�N�g3D�̎��
    typedef enum
    {
        TYPE_PURPLE_FIRE = 0,	// ���̉�
        TYPE_PARTICLE,          // �p�[�e�B�N��
        TYPE_WAVE,              // �E�F�[�u
        TYPE_GUN_MARKS,         // �e��
        TYPE_CHARGE,            // �`���[�W
        TYPE_SMOKE,             // ��
        TYPE_ACID,              // �_
        TYPE_SPRAY,             // ����
        TYPE_ACID_ATTACK,       // �_�U��
        TYPE_WAVE_ATTACK,       // �Ռ��g�U��
        TYPE_RELOADING,         // �����[�h��
        TYPE_CRACK1,            // �n����1
        TYPE_CRACK2,            // �n����2
        TYPE_CRACK3,            // �n����3
        TYPE_CRACK4,            // �n����4
        TYPE_EXPLOSION,         // ����
        TYPE_BLUE_FIRE,         // ����
        TYPE_BURST_CENTER,      // ���ӂ̒��S
        TYPE_BURST_AROUND,      // ���ӂ̎���
        TYPE_MAX			    // ��ނ̍ő吔
    }TYPE;

    CEffect3D(CScene::OBJTYPE objtype);
    ~CEffect3D();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Update(void);
    void Draw(void);
    void Uninit(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CEffect3D * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 rot, float fFadeOutRate, TYPE type);

    float GetRandRotSpeed(int nMax);
    void GetAngleToLeftHand(D3DXVECTOR3 pos);
    void GetMoveToRightHand(D3DXVECTOR3 pos);
    void SetBurstAround(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];    // �ǂݍ��ރe�N�X�`��
    D3DXVECTOR3 m_move;                                 // �ړ���
    float m_fSpeed;                                     // ����
    D3DXCOLOR m_col;                                    // �F
    TYPE m_type;                                        // ���
    float m_fFadeOutRate;                               // �t�F�[�h�A�E�g����

    bool m_bStartFadeOut;                               // �t�F�[�h�A�E�g�J�n
    int m_nCntTime;                                     // �J�E���^

    float m_fRotAngle;                                  // ��]�p�x
};
#endif
