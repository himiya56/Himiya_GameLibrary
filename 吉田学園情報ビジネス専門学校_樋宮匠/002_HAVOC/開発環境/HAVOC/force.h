//===================================================================================
//
// �h�q�R�̏��� (force.h)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//===================================================================================
#ifndef _FORCE_H_
#define _FORCE_H_

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

// �h�q�R�̊e���
#define FORCE_ACTIVE_COLLISION_SIZE D3DXVECTOR3(102.5f, 250.0f, 102.5f)         // �\���I�ȓ����蔻��̑傫��
#define FORCE_LIFE 140                                                          // HP
#define FORCE_SCORE 10000                                                       // �X�R�A
#define FORCE_ORBS 20                                                           // �I�[�u��
#define FORCE_SPEED 10.0f                                                       // ����
#define FORCE_DISCOVERY_DISTANCE 1500.0f                                        // ��������
#define FORCE_COLLISION_MAX 1                                                   // �����蔻��̍ő吔

// ��]����
#define FORCE_TURN_SPEED D3DXToRadian(2.0f)                                     // �U��������x

// �U�����
#define FORCE_NEXT_ATTACK_COUNT 85                                              // ���̍U���܂ł̃J�E���^

// �_�b�V��
#define FORCE_DUSH_WHOLE_FRAME 180                                              // �S�̃t���[��

// �ˌ�
#define FORCE_SHOT_WHOLE_FRAME 150                                              // �S�̃t���[��
#define FORCE_SHOT_CREATE_FRAME 15                                              // �N���G�C�g�t���[��
#define FORCE_SHOT_AIM_UP 100.0f                                                // �G�C�����ɁA�����_��

// �񕜃A�C�e���𗎂Ƃ��m��(100���̂Ȃ�ڂ�)
#define FORCE_DROP_HEAL_ITEM 100

// �f�X���̃J�E���^
#define FORCE_DEATH_PLAY_SOUND 60                                               // ���j����炷�t���[��
#define FORCE_DEATH_COUNTER 180                                                 // ������܂�

//================================================
// �O���錾
//================================================
class CAnimation;

//================================================
// �N���X�錾
//================================================

// �h�q�R�N���X
class CForce : public CCharacter
{
public:
    CForce();
    ~CForce();

    // �񋓌^�錾(���[�V����)
    typedef enum
    {
        ANIM_IDLE = 0,      // �ҋ@
        ANIM_WALK,          // ����
        ANIM_ATTACK,        // �U��
        ANIM_DEATH,         // ����
        ANIM_MAX,           // �ő吔
    }ANIMATION;

    // �񋓌^�錾�i�e���q�����̏��Ő錾�j
    typedef enum
    {
        PARTS_BODY = 0,
        PARTS_HEAD,
        PARTS_RIGHT_ARM,
        PARTS_RIGHT_HAND,
        PARTS_LEFT_ARM,
        PARTS_LEFT_HAND,
        PARTS_RIGHT_LEG,
        PARTS_RIGHT_FOOT,
        PARTS_LEFT_LEG,
        PARTS_LEFT_FOOT,
        PARTS_WEAPON,
        PARTS_MAX,
    }PARTS;

    // �U��AI
    typedef enum
    {
        ATTACK_AI_WAIT = 0,   // �ҋ@
        ATTACK_AI_DUSH,       // ����
        ATTACK_AI_SHOT,       // �ˌ�
        ATTACK_AI_MAX,        // �U��AI�̍ő吔
    }ATTACK_AI;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Movement(void);
    void UpdateCollisionPos(void);                                       // �����蔻��̈ʒu�̍X�V
    D3DXVECTOR3 RotControl(D3DXVECTOR3 rot);                             // ��]����
    D3DXVECTOR3 AttackAI(D3DXVECTOR3 pos, D3DXVECTOR3 rot);              // �U��AI
    void JudgmentRunOrAttack(D3DXVECTOR3 pos);                           // ���邩�U�����邩�̔��f
    void DicisionTargetPos(D3DXVECTOR3 pos);                             // �W�I�̈ʒu�����߂�
    D3DXVECTOR3 Dush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                  // ����
    void SetShotAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                // �ˌ��U��

    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CForce *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

    void ScatterOrbs(void);                                              // �I�[�u���o���܂�
    void AddMissionClear(void);                                          // �~�b�V�����N���A����ǉ�����

private:
    static LPD3DXMESH		m_pMesh[PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[PARTS_MAX];
    static DWORD			m_nNumMat[PARTS_MAX];
    // ���
    D3DXVECTOR3 m_move;                        // �ړ���
    int m_nScore;                              // �X�R�A
    int m_nOrbs;                               // �I�[�u

    int m_nCntTime;                            // ���Ԃ𐔂���

    float m_fSpeed;                            // ����
    D3DXVECTOR3 m_rotDest;                     // ����(�ړI�l)
    D3DXVECTOR3 m_rotMin;                      // ������ς���Ƃ��ɁA�ŒZ�ŐU��������ߎg�p

    float m_fDiscoveryDistance;                // �v���C���[��������

    ATTACK_AI m_attackAI;                      // �U��AI

    D3DXVECTOR3 m_targetPos;                   // �^�[�Q�b�g�̈ʒu
    bool m_bUseRotControl;                     // �����̐�����g�����ǂ���

    int m_nDeathCounter;                       // �f�X���̃J�E���^�[
};

#endif
