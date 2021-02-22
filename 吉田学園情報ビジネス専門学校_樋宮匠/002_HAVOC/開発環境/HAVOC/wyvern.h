//===================================================================================
//
// ���C�o�[���̏��� (wyvern.h)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//===================================================================================
#ifndef _WYVERN_H_
#define _WYVERN_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "character.h"
#include "model.h"
#include "modelEffect.h"

//================================================
// �}�N����`
//================================================

// ���C�o�[���̃X�R�A�ⓖ���蔻��̏��
#define WYVERN_SPAWN_POS D3DXVECTOR3(7500.0f, 5000.0f, 9600.0f)                  // �o���ʒu
#define WYVERN_ACTIVE_COLLISION_SIZE D3DXVECTOR3(1500.0f, 1500.0f, 1500.0f)      // �\���I�ȓ����蔻��̑傫��
#define WYVERN_SCORE 300000                                                      // ���j�X�R�A
#define WYVERN_WING_SCORE 75000                                                  // �H�X�R�A
#define WYVERN_TAIL_SCORE 100000                                                 // �K���X�R�A
#define WYVERN_ORBS 50                                                           // �I�[�u��
#define WYVERN_WING_ORBS 75                                                      // �H�I�[�u��
#define WYVERN_TAIL_ORBS 100                                                     // �K���I�[�u��
#define WYVERN_CRUSH_BLOCK 500                                                   // �u���b�N��j�󂷂�_���[�W
#define WYVERN_HEAD_CUBE_COLLISION_SIZE D3DXVECTOR3(575.0f, 300.0f, 575.0f)      // ���̔��̓����蔻��̑傫��
#define WYVERN_HEAD_COLLISION_SIZE D3DXVECTOR3(575.0f, 300.0f, 575.0f)           // ���̕t���������蔻��̑傫��
#define WYVERN_BODY_COLLISION_SIZE D3DXVECTOR3(1000.0f, 500.0f, 1000.0f)         // �̂̓����蔻��̑傫��
#define WYVERN_ARM_RIGHT_COLLISION_SIZE D3DXVECTOR3(300.0f, 200.0f, 300.0f)      // �E��̓����蔻��̑傫��
#define WYVERN_ARM_LEFT_COLLISION_SIZE D3DXVECTOR3(300.0f, 200.0f, 300.0f)       // ����̓����蔻��̑傫��
#define WYVERN_FOOT_RIGHT_COLLISION_SIZE D3DXVECTOR3(250.0f, 500.0f, 250.0f)     // �E���̓����蔻��̑傫��
#define WYVERN_FOOT_LEFT_COLLISION_SIZE D3DXVECTOR3(250.0f, 500.0f, 250.0f)      // �����̓����蔻��̑傫��
#define WYVERN_WING_RIGHT_COLLISION_SIZE D3DXVECTOR3(800.0f, 150.0f, 800.0f)     // �E���̓����蔻��̑傫��
#define WYVERN_WING_LEFT_COLLISION_SIZE D3DXVECTOR3(800.0f, 150.0f, 800.0f)      // �����̓����蔻��̑傫��
#define WYVERN_TAIL_ROOT_COLLISION_SIZE D3DXVECTOR3(700.0f, 600.0f, 700.0f)      // �K���̍��̓����蔻��̑傫��
#define WYVERN_TAIL_TIP_COLLISION_SIZE D3DXVECTOR3(700.0f, 500.0f, 700.0f)       // �K���̒��Ԃ̓����蔻��̑傫��
#define WYVERN_TAIL_CUBE_COLLISION_SIZE D3DXVECTOR3(500.0f, 400.0f, 500.0f)      // �K���̔������蔻��̑傫��
#define WYVERN_ELBOW_RIGHT_COLLISION_SIZE D3DXVECTOR3(400.0f, 200.0f, 400.0f)    // �E�I�̓����蔻��̑傫��
#define WYVERN_ELBOW_LEFT_COLLISION_SIZE D3DXVECTOR3(400.0f, 200.0f, 400.0f)     // ���I�̓����蔻��̑傫��
#define WYVERN_FOOT_CUBE_RIGHT_COLLISION_SIZE D3DXVECTOR3(250.0f, 600.0f, 250.0f)// �E���̔��̓����蔻��̑傫��
#define WYVERN_FOOT_CUBE_LEFT_COLLISION_SIZE D3DXVECTOR3(250.0f, 600.0f, 250.0f) // �����̔��̓����蔻��̑傫��
#define WYVERN_LANDING_CRACK_RADIUS 1500.0f                                      // ���n���ɃK���L����Ԕ��a
#define WYVERN_LANDING_SMALL_COLLAPSE_NUMBER_ONCE 75                             // ���n���ɃK���L���΂���

// ��]����
#define WYVERN_TURN_SPEED D3DXToRadian(1.25f)                                    // �U��������x

// ���C�o�[�����Z�����Ƃ݂Ȃ��l
#define WYVERN_CONSIDER_AS_SHORT 2000.0f                                         // �ߋ���
#define WYVERN_CONSIDER_AS_MEDIUM 4000.0f                                        // ������
#define WYVERN_CONSIDER_AS_LONG 5500.0f                                          // ������

// �h����
#define WYVERM_HEAD_DAMAGE_UP_RATE 2.5f                                          // �w�b�h�V���b�g�_���[�W�㏸����
#define WYVERN_HEAD_DAMAGE_UP_BORDER 100.0f                                      // �w�b�h�V���b�g�̃_���[�W�A�b�v�����鋫�E�l
#define WYVERN_ENEMY_EXPLOSION_ATTENUATION_RATE 0.1f                             // �G�̔����ւ̃_���[�W�y������
#define WYVERN_LIFE_MAX 15000                                                    // �ő�HP
#define WYVERN_WING_DURABILITY 1200                                              // �H�̑ϋv�l
#define WYVERN_TAIL_DURABILITY 3000                                              // �K���̑ϋv�l
#define WYVERN_STUN_WHOLE_FRAME 360                                              // �X�^���̑S�̃t���[��
#define WYVERN_STUN_CREATE_FRAME 120                                             // �X�^���̐����t���[��
#define WYVERN_STUN_VALUE_MAX 1000                                               // �ő�X�^���l
#define WYVERN_STUN_ADD_VALUE_HEAD 10                                            // ���̃X�^���l�㏸��
#define WYVERN_STUN_ADD_VALUE_FOOT 20                                            // ���̃X�^���l�㏸��
#define WYVERN_STUN_VALUE_UP_BORDER 1000.0f                                      // �X�^���l�A�b�v�̋��E�l
#define WYVERN_FLINCH_MAX 3                                                      // �ő勯�݉�
#define WYVERN_FLINCH_WHOLE_FRAME 160                                            // ���݂̑S�̃t���[��
#define WYVERN_FLINCH_KNOCKBACK_VALUE 10.0f                                      // ���݂̃m�b�N�o�b�N��
#define WYVERN_FLINCH_KNOCKBACK_END_FRAME 100                                    // ���݂̃m�b�N�o�b�N�I���t���[��

// �U�����
#define WYVERN_START_ANGRY_LIFE (WYVERN_LIFE_MAX / 4)                            // �{��ɔh������̗�
#define WYVERN_SPEED 20.0f                                                       // ����
#define WYVERN_SPEED_ANGRY 27.5f                                                 // �{�蒆�̑���

// �ҋ@��(�����ɂ���č̗p�̊m����ς���)
#define WYVERN_NEXT_ATTACK_COUNT 100                                             // ���̍U���܂ł̃J�E���^
#define WYVERN_FIRST_PRIORITY 40                                                 // ��ԖڂɎg���₷���Z(100���̂Ȃ�ڂ�)
#define WYVERN_SECOND_PRIORITY 30                                                // ��ԖڂɎg���₷���Z
#define WYVERN_THIRD_PRIORITY 20                                                 // �O�ԖڂɎg���₷���Z
#define WYVERN_FOURTH_PRIORITY 10                                                // �l�ԖڂɎg���₷���Z

// �^�b�N��
#define WYVERN_TACKLE_WHOLE_FRAME 180                                            // �S�̃t���[��

// ��s
#define WYVERN_FLY_WHOLE_FRAME 240                                               // �S�̃t���[��
#define WYVERN_FLY_DOWN_SPEED_RATE 0.8f                                          // �����������銄��
#define WYVERN_FLY_START_SOMERSAULT_FRAME 120                                    // �T�}�[�\���g�܂ł̃N�[���^�C��

// ���P
#define WYVERN_ASSULT_WHOLE_FRAME 120                                            // �S�̃t���[��
#define WYVERN_ASSULT_UP_SPEED_FRAME 80                                          // �����t���[��
#define WYVERN_ASSULT_UP_SPEED_RATE 3.0f                                         // �������オ�銄��
#define WYVERN_ASSULT_CREATE_FRAME_1 60                                          // �N���G�C�g�t���[��1
#define WYVERN_ASSULT_CREATE_FRAME_2 80                                          // �N���G�C�g�t���[��2
#define WYVERN_ASSULT_CREATE_FRAME_3 100                                         // �N���G�C�g�t���[��3

// �T�}�[�\���g
#define WYVERN_SOMERSAULT_WHOLE_FRAME 145                                        // �S�̃t���[��
#define WYVERN_SOMERSALUT_CREATE_FRAME 85                                        // �N���G�C�g�t���[��
#define WYVERN_SOMERSAULT_SHOCK_NUMBER_ONCE 3                                    // �Ռ��g����x�ɐ������鐔
#define WYVERN_SOMERSAULT_SHOCK_ANGLE_XZ D3DXToRadian(27.5f)                     // ���̊p�x
#define WYVERN_SOMERSAULT_DERIVE_FLY_LIFE (WYVERN_LIFE_MAX / 2)                  // ��s�ɔh������̗�

// ����
#define WYVERN_EXPLOSION_WHOLE_FRAME 300                                         // �S�̃t���[��
#define WYVERN_EXPLOSION_CREATE_START_FRAME 180                                  // �N���G�C�g�X�^�[�g�t���[��
#define WYVERN_EXPLOSION_CREATE_FRAME 30                                         // �N���G�C�g�t���[��
#define WYVERN_EXPLOSION_MAX_RADIUS 3500                                         // �ő唼�a
#define WYVERN_EXPLOSION_MIN_RADIUS 2500                                         // �ŏ����a

// ���K
#define WYVERN_ROAR_WHOLE_FRAME 175                                              // �S�̃t���[��
#define WYVERN_ROAR_CREATE_FRAME 105                                             // �N���G�C�g�t���[��
#define WYVERN_ROAR_SET_ENEMY_COUNT_BORDER 45                                    // �G�����v���̈ȉ��Ȃ琶�����邩
#define WYVERN_ROAR_SET_ENEMY_ONCE 5                                             // �����ɉ��̂̓G�𐶐����邩
#define WYVERN_ROAR_SET_ENEMY_SPAWN_POS_Y 2500.0f                                // �X�|�[���n�_�̍���
#define WYVERN_ROAR_SET_ENEMY_RADIUS 3000.0f                                     // �U���𐶐����锼�a

// �񕜃A�C�e���𗎂Ƃ��m��(100���̂Ȃ�ڂ�)
#define WYVERN_DROP_HEAL_ITEM 0

//================================================
// �O���錾
//================================================
class CAnimation;

//================================================
// �N���X�錾
//================================================

// ���C�o�[���N���X
class CWyvern : public CCharacter
{
public:
    CWyvern();
    ~CWyvern();

    // �񋓌^�錾(���[�V����)
    typedef enum
    {
        ANIM_IDLE = 0,      // �ҋ@
        ANIM_WALK,          // ����
        ANIM_SHOT,          // ����A��
        ANIM_ROAR,          // ���K
        ANIM_LAND,          // ���n
        ANIM_FLINCH,        // ����
        ANIM_STUN,          // �_�E��
        ANIM_FLY,           // ��s
        ANIM_SOMERSAULT,    // �T�}�[�\���g
        ANIM_ASSAULT,       // ���P
        ANIM_EXPLOSION,     // ����
        ANIM_MAX,           // �ő吔
    }ANIMATION;

    // �e�p�[�c
    typedef enum
    {
        PARTS_WEIST = 0,
        PARTS_BODY,
        PARTS_NECK,
        PARTS_HEAD,
        PARTS_TAIL_ROOT,
        PARTS_TAIL_TIP,
        PARTS_ARM_RIGHT,
        PARTS_ARM_LEFT,
        PARTS_WING_RIGHT,
        PARTS_WING_LEFT,
        PARTS_LEG_RIGHT,
        PARTS_FOOT_RIGHT,
        PARTS_LEG_LEFT,
        PARTS_FOOT_LEFT,
        PARTS_CUBE_HEAD,
        PARTS_CUBE_TAIL,
        PARTS_CUBE_ARM_RIGHT,
        PARTS_CUBE_ARM_LEFT,
        PARTS_CUBE_WING_RIGHT,
        PARTS_CUBE_WING_LEFT,
        PARTS_CUBE_ELBOW_RIGHT,
        PARTS_CUBE_ELBOW_LEFT,
        PARTS_CUBE_FOOT_RIGHT,
        PARTS_CUBE_FOOT_LEFT,
        PARTS_MAX,
    }PARTS;

    // �e�p�[�c�̓����蔻��
    typedef enum
    {
        COLLISION_PARTS_HEAD_CUBE = 0,
        COLLISION_PARTS_HEAD,
        COLLISION_PARTS_BODY,
        COLLISION_PARTS_ARM_RIGHT,
        COLLISION_PARTS_ARM_LEFT,
        COLLISION_PARTS_FOOT_RIGHT,
        COLLISION_PARTS_FOOT_LEFT,
        COLLISION_PARTS_WING_RIGHT,
        COLLISION_PARTS_WING_LEFT,
        COLLISION_PARTS_TAIL_ROOT,
        COLLISION_PARTS_TAIL_TIP,
        COLLISION_PARTS_TAIL_CUBE,
        COLLISION_PARTS_ELBOW_RIGHT,
        COLLISION_PARTS_ELBOW_LEFT,
        COLLISION_PARTS_FOOT_CUBE_RIGHT,
        COLLISION_PARTS_FOOT_CUBE_LEFT,
        COLLISION_PARTS_MAX,
    }COLLISION_PARTS;

    // �U��AI
    typedef enum
    {
        ATTACK_AI_WAIT = 0,   // �ҋ@
        ATTACK_AI_TACKLE,     // �^�b�N��
        ATTACK_AI_SHOT,       // �ˌ�
        ATTACK_AI_SOMERSAULT, // �T�}�[�\���g
        ATTACK_AI_ROAR,       // ���K
        ATTACK_AI_FLY,        // �͂΂���
        ATTACK_AI_ASSAULT,    // ���P
        ATTACK_AI_EXPLOSION,  // ����
        ATTACK_AI_MAX,        // �U��AI�̍ő吔
    }ATTACK_AI;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Movement(void);
    void UpdateCollisionPos(void);                                       // �����蔻��̈ʒu�̍X�V
    D3DXVECTOR3 RotControl(D3DXVECTOR3 rot);                             // ��]����
    D3DXVECTOR3 StunOrFlinch(D3DXVECTOR3 pos, D3DXVECTOR3 rot);          // �X�^�������݂��̏���
    D3DXVECTOR3 AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot);              // �U��AI
    void NextAttackSelect(D3DXVECTOR3 pos);                              // ���̍U����I��
    D3DXVECTOR3 Dush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                  // ����
    void SetShockWave(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                 // �Ռ��g�U��
    void SetExplosion(bool bUseTracking, bool bFall);                    // �����U��
    void SetEnemySpawn(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                // �G�����U��

    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CWyvern *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

    void SetLandingEffect(D3DXVECTOR3 pos);                                                 // ���n�G�t�F�N�g
    void ScatterOrbs(int nOrbs);                                                            // �I�[�u���o���܂�
    void DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                                      // ���ꂽ���̕���
    void SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type);    // ���ꂽ���̕��ӃG�t�F�N�g�̔���
    void TakeDamage(D3DXVECTOR3 pos, int nDamage, int nCntCollision);                       // �_���[�W���󂯂��Ƃ��̏���
    void AddMissionClear(void);                                                             // �~�b�V�����N���A����ǉ�����

private:
    static LPD3DXMESH		m_pMesh[PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[PARTS_MAX];
    static DWORD			m_nNumMat[PARTS_MAX];

    D3DXVECTOR3 m_move;                        // �ړ���
    int m_nScore;                              // �X�R�A
    int m_nOrbs;                               // �I�[�u

    int m_nCntTime;                            // ���Ԃ𐔂���

    float m_fSpeed;                            // ����
    D3DXVECTOR3 m_rotDest;                     // ����(�ړI�l)
    D3DXVECTOR3 m_rotMin;                      // ������ς���Ƃ��ɁA�ŒZ�ŐU��������ߎg�p

    ATTACK_AI m_attackAI;                      // �U��AI

    bool m_bUseRotControl;                     // �����̐�����g�����ǂ���

    bool m_bStartBattle;                       // �o�g�����J�n�������ǂ���

    int m_nRightWingDurabirity;                // �E�H�ϋv�l
    int m_nLeftWingDurabirity;                 // ���H�ϋv�l
    int m_nTailDurabirity;                     // �K���ϋv�l
    bool m_bDestroyRightWing;                  // �E�H��j�󂵂����ǂ���
    bool m_bDestroyLeftWing;                   // ���H��j�󂵂����ǂ���
    bool m_bDestroyTail;                       // �K����j�󂵂����ǂ���

    bool m_bFlySomersault;                     // ��s���ɃT�}�[�\���g���������ǂ���
    bool m_bPrevRoar;                          // �O�ə��K���������ǂ���

    bool m_bStun;                              // �X�^�����Ă��邩�ǂ���
    int m_nStunValue;                          // �X�^���l
    int m_nCntFlinch;                          // ���݉񐔉��Z
};

#endif