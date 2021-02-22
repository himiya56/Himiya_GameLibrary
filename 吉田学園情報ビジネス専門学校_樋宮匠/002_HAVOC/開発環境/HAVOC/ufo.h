//===================================================================================
//
// UFO�̏��� (ufo.h)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//===================================================================================
#ifndef _UFO_H_
#define _UFO_H_

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

// UFO�̑��̖{��
#define UFO_MAX_LEG 8

// UFO���ړI�n�ɋ߂��Ƃ݂Ȃ��l
#define UFO_CONSIDER_CLOSE_DEST 30.0f

// UFO���p�g���[�����ɕ����]������܂ł̃J�E���^
#define UFO_CHANGE_MOVE_IN_PATROL 25

// UFO�����͂�UFO�Ɍx��������Ƃ��̋���
#define UFO_WARN_UFO_DISTANCE 4500.0f

// UFO���������Ƃ݂Ȃ��l
#define UFO_CONSIDER_AS_LONG 6000.0f

// UFO
#define UFO_UP_COLLISION_POS  D3DXVECTOR3(0.0f, 150.0f, 0.0f)                 // �����蔻��̈ʒu����ɂ��炷
#define UFO_ACTIVE_COLLISION_SIZE D3DXVECTOR3(750.0f, 750.0f, 750.0f)         // �\���I�ȓ����蔻��̑傫��
#define UFO_LIFE 120                                                          // HP
#define UFO_SCORE 8000                                                        // �X�R�A
#define UFO_ORBS 22                                                           // �I�[�u��
#define UFO_SPEED 17.5f                                                       // ����
#define UFO_PATROL_DISTANCE 4500                                              // ���񋗗�
#define UFO_DISCOVERY_DISTANCE 2500.0f                                        // ��������
#define UFO_COLLISION_MAX 1                                                   // �����蔻��̍ő吔

// ��]����
#define UFO_TURN_SPEED D3DXToRadian(2.5f)                                     // �U��������x

// �U�����
#define UFO_NEXT_ATTACK_COUNT 120                                             // ���̍U���܂ł̃J�E���^
#define UFO_TRACKING_SPEED 14.5f                                              // �ǐՑ��x

// �ˌ�
#define UFO_SHOT_WHOLE_FRAME 190                                              // �S�̃t���[��
#define UFO_SHOT_CREATE_FRAME_1 60                                            // �N���G�C�g�t���[��1
#define UFO_SHOT_CREATE_FRAME_2 80                                            // �N���G�C�g�t���[��2
#define UFO_SHOT_CREATE_FRAME_3 100                                           // �N���G�C�g�t���[��3

// �񕜃A�C�e���𗎂Ƃ��m��(100���̂Ȃ�ڂ�)
#define UFO_DROP_HEAL_ITEM 50

//================================================
// �O���錾
//================================================
class CAnimation;

//================================================
// �N���X�錾
//================================================

// UFO�N���X
class CUfo : public CCharacter
{
public:
    CUfo();
    ~CUfo();

    // �񋓌^�錾(���[�V����)
    typedef enum
    {
        ANIM_IDLE = 0,      // �ҋ@
        ANIM_WARN,          // �Њd
        ANIM_MAX,           // �ő吔
    }ANIMATION;

    // �񋓌^�錾
    typedef enum
    {
        PARTS_FIGHTER = 0,
        PARTS_RIGHT_WING_0,
        PARTS_RIGHT_WING_1,
        PARTS_RIGHT_WING_2,
        PARTS_RIGHT_WING_3,
        PARTS_LEFT_WING_0,
        PARTS_LEFT_WING_1,
        PARTS_LEFT_WING_2,
        PARTS_LEFT_WING_3,
        PARTS_MAX,
    }PARTS;

    // �U��AI
    typedef enum
    {
        ATTACK_AI_WAIT = 0,   // �ҋ@
        ATTACK_AI_SHOT,       // �ˌ�
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
    void SetShotAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                // �ˌ��U��

    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CUfo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

    void ScatterOrbs(void);                                                                 // �I�[�u���o���܂�
    void DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                                      // ���ꂽ���̕���
    void SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type);    // ���ꂽ���̕��ӃG�t�F�N�g�̔���
    void WarnFellow(void);                                                                  // ���ԂɌx������
    void AddMissionClear(void);                                                             // �~�b�V�����N���A����ǉ�����

    // �x���̃Z�b�^�[�A�Q�b�^�[
    void SetWarning(bool bWarning) { m_bWarning = bWarning; }
    bool GetWarning(void) { return m_bWarning; }

private:
    static LPD3DXMESH		m_pMesh[PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[PARTS_MAX];
    static DWORD			m_nNumMat[PARTS_MAX];

    D3DXVECTOR3 m_move;                        // �ړ���
    int m_nScore;                              // �X�R�A
    int m_nOrbs;                               // �I�[�u

    D3DXVECTOR3 m_patrolCentralPos;            // ����̒��S�ʒu
    D3DXVECTOR3 m_patrolDestPos;               // ����̖ړI�ʒu
    int m_nPatrolDistance;                     // ���񋗗�
    int m_nCntTime;                            // ���Ԃ𐔂���

    float m_fSpeed;                            // ����
    D3DXVECTOR3 m_rotDest;                     // ����(�ړI�l)
    D3DXVECTOR3 m_rotMin;                      // ������ς���Ƃ��ɁA�ŒZ�ŐU��������ߎg�p

    bool m_bWarning;                           // �x�������ǂ���
    float m_fDiscoveryDistance;                // �v���C���[��������

    ATTACK_AI m_attackAI;                      // �U��AI
};

#endif
