//===================================================================================
//
// �w偂̏��� (spider.h)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//===================================================================================
#ifndef _SPIDER_H_
#define _SPIDER_H_

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

// �w偂̑��̖{��
#define SPIDER_MAX_LEG 8

// �w偂��ړI�n�ɋ߂��Ƃ݂Ȃ��l
#define SPIDER_CONSIDER_CLOSE_DEST 20.0f

// �w偂��p�g���[�����ɕ����]������܂ł̃J�E���^
#define SPIDER_CHANGE_MOVE_IN_PATROL 45

// �w偂����͂̒w偂Ɍx��������Ƃ��̋���
#define SPIDER_WARN_SPIDER_DISTANCE 1850.0f

// �w偂��Z�����Ƃ݂Ȃ��l
#define SPIDER_CONSIDER_AS_SHORT 2000.0f
#define SPIDER_CONSIDER_AS_MEDIUM 4000.0f
#define SPIDER_CONSIDER_AS_LONG 5500.0f

// ���w�
#define SPIDER_YELLOW_ACTIVE_COLLISION_SIZE D3DXVECTOR3(300.0f, 150.0f, 300.0f)         // �\���I�ȓ����蔻��̑傫��
#define SPIDER_YELLOW_LIFE 90                                                           // HP
#define SPIDER_YELLOW_SCORE 3500                                                        // �X�R�A
#define SPIDER_YELLOW_ORBS 15                                                           // �I�[�u��
#define SPIDER_YELLOW_SPEED 7.5f                                                        // ����
#define SPIDER_YELLOW_PATROL_DISTANCE 2500                                              // ���񋗗�
#define SPIDER_YELLOW_DISCOVERY_DISTANCE 1750.0f                                        // ��������
#define SPIDER_YELLOW_COLLISION_MAX 1                                                   // �����蔻��̍ő吔
#define SPIDER_YELLOW_BODY_COLLISION_SIZE D3DXVECTOR3(450.0f, 300.0f, 450.0f)           // �̂̓����蔻��̑傫��

// ���单�w�
#define SPIDER_BLACK_SCALE D3DXVECTOR3(2.0f, 2.0f, 2.0f)                                // �X�P�[��
#define SPIDER_BLACK_ACTIVE_COLLISION_SIZE  D3DXVECTOR3(475.0f, 400.0f, 475.0f)         // �\���I�ȓ����蔻��̑傫��
#define SPIDER_BLACK_LIFE 140                                                           // HP
#define SPIDER_BLACK_SCORE 5000                                                         // �X�R�A
#define SPIDER_BLACK_ORBS 25                                                            // �I�[�u��
#define SPIDER_BLACK_SPEED 10.0f                                                        // ����
#define SPIDER_BLACK_PATROL_DISTANCE 3000                                               // ���񋗗�
#define SPIDER_BLACK_DISCOVERY_DISTANCE 2250.0f                                         // ��������
#define SPIDER_BLACK_COLLISION_MAX 4                                                    // �����蔻��̍ő吔
#define SPIDER_BLACK_BODY_COLLISION_SIZE D3DXVECTOR3(250.0f, 250.0f, 250.0f)            // �̂̓����蔻��̑傫��
#define SPIDER_BLACK_STOMACH_COLLISION_SIZE D3DXVECTOR3(500.0f, 500.0f, 500.0f)         // ���̓����蔻��̑傫��
#define SPIDER_BLACK_FANG_COLLISION_SIZE D3DXVECTOR3(300.0f, 300.0f, 300.0f)            // ��̓����蔻��̑傫��
#define SPIDER_BLACK_ACID_NUMBER_ONCE 3                                                 // ��x�Ɏ_�𐶐����鐔(for����nCnt�ŕ����𕪂��Ă���̂Œ���)

// ��]����
#define SPIDER_TURN_SPEED D3DXToRadian(1.5f)        // �U��������x
#define SPIDER_TILT_SPEED D3DXToRadian(1.25f)       // �X�����x
#define SPIDER_TILT_LIMIT D3DXToRadian(90.0f)       // �X���̌��E�l

// �U�����
// �ҋ@��(�����ɂ���č̗p�̊m���͕ς��ĂȂ�)
#define SPIDER_NEXT_ATTACK_COUNT 110                // ���̍U���܂ł̃J�E���^
#define SPIDER_TAUNT_PERCENT 20                     // �������̗p�����m��
#define SPIDER_ACID_PERCENT 45                      // �_���̗p�����m��
#define SPIDER_JUMP_PERCENT 35                      // �W�����v���̗p�����m��
#define SPIDER_NEXT_ATTACK_PERCENT_MAX (SPIDER_TAUNT_PERCENT + SPIDER_ACID_PERCENT + SPIDER_JUMP_PERCENT) // �U���̗p�̍ő�p�[�Z���g

// ����
#define SPIDER_TAUNT_WHOLE_FRAME 120                // �S�̃t���[��

// �_��΂�
#define SPIDER_ACID_WHOLE_FRAME 60                                  // �S�̃t���[��
#define SPIDER_ACID_CREATE_FRAME 35                                 // �N���G�C�g�t���[��
#define SPIDER_ACID_ADD_SET_POS_Y D3DXVECTOR3(0.0f, 200.0f, 0.0f)   // ���ˎ��ɁA��ɂ��炷�l
#define SPIDER_ACID_SHORT_MAX_ANGLE_Y 1900                          // �ߋ����̍ő�p�xY
#define SPIDER_ACID_SHORT_MIN_ANGLE_Y 1500                          // �ߋ����̍ŏ��p�xY
#define SPIDER_ACID_MEDIUM_MAX_ANGLE_Y 3500                         // �������̍ő�p�xY
#define SPIDER_ACID_MEDIUM_MIN_ANGLE_Y 2500                         // �������̍ŏ��p�xY
#define SPIDER_ACID_LONG_MAX_ANGLE_Y 5900                           // �������̍ő�p�xY
#define SPIDER_ACID_LONG_MIN_ANGLE_Y 4500                           // �������̍ŏ��p�xY
#define SPIDER_ACID_MAX_ANGLE_TO_SHIFT_FROM_CENTER 2250             // ���S���炸�炷�ő�p�x

// �W�����v
#define SPIDER_JUMP_WHOLE_FRAME 220                // �S�̃t���[��
#define SPIDER_JUMP_SPEED_RATE 1.2f                // �X�s�[�h�{��
#define SPIDER_JUMP_VALUE 46.0f                    // �W�����v��

// �񕜃A�C�e���𗎂Ƃ��m��(100���̂Ȃ�ڂ�)
#define SPIDER_DROP_HEAL_ITEM 15

//================================================
// �O���錾
//================================================
class CAnimation;

//================================================
// �N���X�錾
//================================================

// �X�p�C�_�[�N���X
class CSpider : public CCharacter
{
public:
    CSpider();
    ~CSpider();

    // �񋓌^�錾(���[�V����)
    typedef enum
    {
        ANIM_IDLE = 0,      // �ҋ@
        ANIM_WALK,          // ����
        ANIM_TAUNT,         // ����
        ANIM_ATTACK,        // �U��
        ANIM_JUMP,          // �W�����v
        ANIM_MAX,           // �ő吔
    }ANIMATION;

    // �񋓌^�錾
    typedef enum
    {
        PARTS_BODY = 0,
        PARTS_STOMACH,
        PARTS_FANG_RIGHT,
        PARTS_FANG_LEFT,
        PARTS_LEG_RIGHT_0,
        PARTS_LEG_RIGHT_1,
        PARTS_LEG_RIGHT_2,
        PARTS_LEG_RIGHT_3,
        PARTS_LEG_LEFT_0,
        PARTS_LEG_LEFT_1,
        PARTS_LEG_LEFT_2,
        PARTS_LEG_LEFT_3,
        PARTS_STOMACH_CUBE, // �����甭�������铖���蔻��̈ʒu
        PARTS_MAX,
    }PARTS;

    // �񋓌^�錾(�w偂̃^�C�v)
    typedef enum
    {
        TYPE_NONE = 0,
        TYPE_YELLOW,    // ���w�
        TYPE_BLACK,     // ���单�w�
        TYPE_MAX,
    }TYPE;

    // �U��AI
    typedef enum
    {
        ATTACK_AI_WAIT = 0,   // �ҋ@
        ATTACK_AI_TAUNT,      // ����
        ATTACK_AI_ACID,       // �_��΂�
        ATTACK_AI_JUMP,       // �W�����v
        ATTACK_AI_MAX,        // �U��AI�̍ő吔
    }ATTACK_AI;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Movement(void);
    void UpdateCollisionPos(void);                                       // �����蔻��̈ʒu�̍X�V
    D3DXVECTOR3 RotControl(D3DXVECTOR3 rot);                             // ��]����
    D3DXVECTOR3 PatrolAI(D3DXVECTOR3 pos);                               // �p�g���[��AI
    D3DXVECTOR3 WarningAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot);             // �x����AI
    D3DXVECTOR3 AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot);              // �U��AI
    void SetAcidAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                // �_��΂��U��
    D3DXVECTOR3 SetJumpAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);         // �W�����v�U��

    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CSpider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bWarning, TYPE type);

    void ScatterOrbs(void);                                                                 // �I�[�u���o���܂�
    void DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                                      // ���ꂽ���̕���
    void SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type);    // ���ꂽ���̕��ӃG�t�F�N�g�̔���
    void WarnFellow(void);                                                                  // ���ԂɌx������
    void AddMissionClear(void);                                                             // �~�b�V�����N���A����ǉ�����

    // �x���̃Z�b�^�[�A�Q�b�^�[
    void SetWarning(bool bWarning) { m_bWarning = bWarning; }
    bool GetWarning(void) { return m_bWarning; }

    bool GetWallRun(void) { return m_bWallRun; }

private:
    static LPD3DXMESH		m_pMesh[TYPE_MAX][PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[TYPE_MAX][PARTS_MAX];
    static DWORD			m_nNumMat[TYPE_MAX][PARTS_MAX];

    TYPE m_type;		                       // ���
    D3DXVECTOR3 m_move;                        // �ړ���
    int m_nScore;                              // �X�R�A
    int m_nOrbs;                               // �I�[�u

    D3DXVECTOR3 m_patrolCentralPos;            // ����̒��S�ʒu
    D3DXVECTOR3 m_patrolDestPos;               // ����̖ړI�ʒu
    int m_nPatrolDistance;                     // ���񋗗�
    int m_nCntTime;                            // ���Ԃ𐔂���

    bool m_bWallRun;                           // �Ǒ��蒆���ǂ���
    float m_fSpeed;                            // ����
    D3DXVECTOR3 m_rotDest;                     // ����(�ړI�l)
    D3DXVECTOR3 m_rotMin;                      // ������ς���Ƃ��ɁA�ŒZ�ŐU��������ߎg�p

    bool m_bWarning;                           // �x�������ǂ���
    float m_fDiscoveryDistance;                // �v���C���[��������

    bool m_bAttack;                            // �U�����Ă��邩�ǂ���
    ATTACK_AI m_attackAI;                      // �U��AI
};

#endif
