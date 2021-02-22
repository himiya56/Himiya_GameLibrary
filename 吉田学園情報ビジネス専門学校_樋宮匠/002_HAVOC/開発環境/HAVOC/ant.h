//===================================================================================
//
// �A���̏��� (ant.h)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//===================================================================================
#ifndef _ANT_H_
#define _ANT_H_

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

// �A���̑��̖{��
#define ANT_MAX_LEG 6

// �A�����ړI�n�ɋ߂��Ƃ݂Ȃ��l
#define ANT_CONSIDER_CLOSE_DEST 20.0f

// �A�����p�g���[�����ɕ����]������܂ł̃J�E���^
#define ANT_CHANGE_MOVE_IN_PATROL 30

// �A�������͂̃A���Ɍx��������Ƃ��̋���
#define ANT_WARN_ANT_DISTANCE 2200.0f

// �A�����Z�����Ƃ݂Ȃ��l
#define ANT_CONSIDER_AS_SHORT 2000.0f
#define ANT_CONSIDER_AS_MEDIUM 4000.0f
#define ANT_CONSIDER_AS_LONG 5500.0f

// ���A��
#define ANT_WHITE_ACTIVE_COLLISION_SIZE D3DXVECTOR3(200.0f, 150.0f, 200.0f)         // �\���I�ȓ����蔻��̑傫��
#define ANT_WHITE_LIFE 50                                                           // HP
#define ANT_WHITE_SCORE 3000                                                        // �X�R�A
#define ANT_WHITE_ORBS 15                                                           // �I�[�u��
#define ANT_WHITE_SPEED 12.5f                                                       // ����
#define ANT_WHITE_PATROL_DISTANCE 3500                                              // ���񋗗�
#define ANT_WHITE_DISCOVERY_DISTANCE 2250.0f                                        // ��������
#define ANT_WHITE_COLLISION_MAX 1                                                   // �����蔻��̍ő吔
#define ANT_WHITE_BODY_COLLISION_SIZE D3DXVECTOR3(350.0f, 200.0f, 350.0f)           // �̂̓����蔻��̑傫��

// ���单�A��
#define ANT_BLACK_SCALE D3DXVECTOR3(2.0f, 2.0f, 2.0f)                               // �X�P�[��
#define ANT_BLACK_ACTIVE_COLLISION_SIZE  D3DXVECTOR3(475.0f, 500.0f, 475.0f)        // �\���I�ȓ����蔻��̑傫��
#define ANT_BLACK_LIFE 100                                                          // HP
#define ANT_BLACK_SCORE 4500                                                        // �X�R�A
#define ANT_BLACK_ORBS 25                                                           // �I�[�u��
#define ANT_BLACK_SPEED 12.0f                                                       // ����
#define ANT_BLACK_PATROL_DISTANCE 4000                                              // ���񋗗�
#define ANT_BLACK_DISCOVERY_DISTANCE 2750.0f                                        // ��������
#define ANT_BLACK_COLLISION_MAX 6                                                   // �����蔻��̍ő吔
#define ANT_BLACK_BODY_COLLISION_SIZE D3DXVECTOR3(350.0f, 350.0f, 350.0f)           // �̂̓����蔻��̑傫��
#define ANT_BLACK_STOMACH_COLLISION_SIZE D3DXVECTOR3(400.0f, 400.0f, 400.0f)        // ���̓����蔻��̑傫��
#define ANT_BLACK_TACTILE_COLLISION_SIZE D3DXVECTOR3(200.0f, 200.0f, 200.0f)        // �G�p�̓����蔻��̑傫��
#define ANT_BLACK_FANG_COLLISION_SIZE D3DXVECTOR3(300.0f, 300.0f, 300.0f)           // ��̓����蔻��̑傫��
#define ANT_BLACK_ACID_NUMBER_ONCE 2                                                // ��x�Ɏ_�𐶐����鐔(for����nCnt�ŕ����𕪂��Ă���̂Œ���)

// ��]����
#define ANT_TURN_SPEED D3DXToRadian(1.25f)       // �U��������x
#define ANT_TILT_SPEED D3DXToRadian(1.25f)       // �X�����x
#define ANT_TILT_LIMIT D3DXToRadian(90.0f)       // �X���̌��E�l

// �U�����
// �ҋ@��(�����ɂ���č̗p�̊m���͕ς��ĂȂ�)
#define ANT_NEXT_ATTACK_COUNT 100                // ���̍U���܂ł̃J�E���^
#define ANT_TAUNT_PERCENT 25                     // �������̗p�����m��
#define ANT_ACID_PERCENT 40                      // �_���̗p�����m��
#define ANT_TACKLE_PERCENT 35                    // �^�b�N�����̗p�����m��
#define ANT_NEXT_ATTACK_PERCENT_MAX (ANT_TAUNT_PERCENT + ANT_ACID_PERCENT + ANT_TACKLE_PERCENT) // �U���̗p�̍ő�p�[�Z���g

// ����
#define ANT_TAUNT_WHOLE_FRAME 120                // �S�̃t���[��

// �_��΂�
#define ANT_ACID_WHOLE_FRAME 50                                  // �S�̃t���[��
#define ANT_ACID_CREATE_FRAME 30                                 // �N���G�C�g�t���[��
#define ANT_ACID_ADD_SET_POS_Y D3DXVECTOR3(0.0f, 100.0f, 0.0f)   // ���ˎ��ɁA��ɂ��炷�l
#define ANT_ACID_SHORT_MAX_ANGLE_Y 2000                          // �ߋ����̍ő�p�xY
#define ANT_ACID_SHORT_MIN_ANGLE_Y 1500                          // �ߋ����̍ŏ��p�xY
#define ANT_ACID_MEDIUM_MAX_ANGLE_Y 3500                         // �������̍ő�p�xY
#define ANT_ACID_MEDIUM_MIN_ANGLE_Y 2500                         // �������̍ŏ��p�xY
#define ANT_ACID_LONG_MAX_ANGLE_Y 6000                           // �������̍ő�p�xY
#define ANT_ACID_LONG_MIN_ANGLE_Y 4500                           // �������̍ŏ��p�xY
#define ANT_ACID_MAX_ANGLE_TO_SHIFT_FROM_CENTER 2000             // ���S���炸�炷�ő�p�x

// �^�b�N��
#define ANT_TACKLE_WHOLE_FRAME 180                // �S�̃t���[��
#define ANT_TACKLE_SPEED_RATE 1.275f              // �X�s�[�h�{��

// �񕜃A�C�e���𗎂Ƃ��m��(100���̂Ȃ�ڂ�)
#define ANT_DROP_HEAL_ITEM 20

//================================================
// �O���錾
//================================================
class CAnimation;

//================================================
// �N���X�錾
//================================================

// �A���N���X
class CAnt : public CCharacter
{
public:
    CAnt();
    ~CAnt();

    // �񋓌^�錾(���[�V����)
    typedef enum
    {
        ANIM_IDLE = 0,      // �ҋ@
        ANIM_WALK,          // ����
        ANIM_TAUNT,         // ����
        ANIM_ATTACK,        // �U��
        ANIM_MAX,           // �ő吔
    }ANIMATION;

    // �񋓌^�錾
    typedef enum
    {
        PARTS_BODY = 0,
        PARTS_HEAD,
        PARTS_STOMACH,
        PARTS_TACTILE_RIGHT,
        PARTS_TACTILE_LEFT,
        PARTS_FANG_RIGHT,
        PARTS_FANG_LEFT,
        PARTS_LEG_RIGHT_0,
        PARTS_LEG_RIGHT_1,
        PARTS_LEG_RIGHT_2,
        PARTS_LEG_LEFT_0,
        PARTS_LEG_LEFT_1,
        PARTS_LEG_LEFT_2,
        PARTS_MAX,
    }PARTS;

    // �񋓌^�錾(�A���̃^�C�v)
    typedef enum
    {
        TYPE_NONE = 0,
        TYPE_WHITE,      // ���A��
        TYPE_BLACK,      // ���A��
        TYPE_MAX,
    }TYPE;

    // �U��AI
    typedef enum
    {
        ATTACK_AI_WAIT = 0,   // �ҋ@
        ATTACK_AI_TAUNT,      // ����
        ATTACK_AI_ACID,       // �_��΂�
        ATTACK_AI_TACKLE,     // �^�b�N��
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
    
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CAnt *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bWarning, TYPE type);

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
