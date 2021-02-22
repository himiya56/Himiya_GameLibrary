//======================================================================================
//
// �v���C���[���� (player.h)
// Author : �㓡�T�V��(�s���A���[�V��������) / ��{��(�K�w�\���A���[�V�����쐬)
//
//======================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

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

// �v���C���[�̓����蔻��̑傫��
#define PLAYER_COLLISION_SIZE D3DXVECTOR3(75.0f, 275.0f, 75.0f)

// �v���C���[�̈ړ�����
#define PLAYER_BASE_SPEED 15.0f                                         // ��{�̑��x
#define PLAYER_CONTROL_SPEED_IN_AIR 0.8f                                // �󒆂Ő��䂷�鎞�̑��x
#define PLAYER_CONTROL_SPEED_IN_AIR_AFTER_WALL_KICK 0.875f              // �ǃL�b�N��ɁA�󒆂Ő��䂷�鎞�̑��x
#define PLAYER_WALL_RAN_SPEED 15.0f                                     // �Ǒ��葬�x
#define PLAYER_WALL_KICK_VALUE 38.5f                                    // �ǃL�b�N�̋���
#define PLAYER_WALL_KICK_JUMP_VALUE 30.0f                               // �ǃL�b�N�W�����v�̋���
#define PLAYER_CONTROL_MOVE 0.85f                                       // �ړ��ʐ���
#define PLAYER_CONTROL_MOVE_IN_AIR 0.95f                                // �󒆂ł̈ړ��ʐ���
#define PLAYER_TURN_SPEED D3DXToRadian(10.0f)                           // �U��������x
#define PLAYER_TILT_SPEED D3DXToRadian(1.0f)                            // �X�����x
#define PLAYER_TILT_LIMIT D3DXToRadian(45.0f)                           // �X���̌��E�l
#define PLAYER_JUMP_VALUE 35.0f                                         // �W�����v�̋���
#define PLAYER_FOOTSTEPS_COOL_TIME 28                                   // �����̃N�[���^�C��

// �v���C���[�̉E��U�����
#define PLAYER_SHOT_POS_ADJUSTMENT (D3DXVECTOR3(0.0f, 175.0f, 0.0f))    // ����������(���Ƃ�150)
#define PLAYER_REMAIN_BULLETS_MAX 200                                   // �c�e�̍ő吔
#define PLAYER_REMAIN_BULLETS_WARNING 50                                // �c�e�̌x���J�n��
#define PLAYER_RELOAD_COOL_TIME 180                                     // �����[�h�̃N�[���^�C��

// �v���C���[�̃m�b�N�o�b�N��
#define PLAYER_KNOCKBACK_VALUE 75.0f

// �v���C���[��HP����
#define PLAYER_MAX_LIFE 3000                                            // �ő�HP(3000?)
#define PLAYER_INVINCIBLE_FRAME 180                                     // �v���C���[�̖��G����
#define PLAYER_START_MOVE_IN_INVINCIBLE 90                              // ���G���A�����o����܂ł̃J�E���^

// �v���C���[�̏����ʒu�A��������
#define PLAYER_START_POS_STAGE1 (D3DXVECTOR3(2000.0f, 0.0f, 0.0f))
#define PLAYER_START_ROT_STAGE1 D3DXToRadian(90.0f)
#define PLAYER_START_POS_STAGE2 (D3DXVECTOR3(400.0f, 0.0f, 6400.0f))
#define PLAYER_START_ROT_STAGE2 D3DXToRadian(0.0f)
#define PLAYER_START_POS_STAGE_FINAL (D3DXVECTOR3(6300.0f, 0.0f, 300.0f))
#define PLAYER_START_ROT_STAGE_FINAL D3DXToRadian(90.0f)

// �v���C���[�̒��n���[�V��������
#define PLAYER_LANDING_TIME 15                                          // ���n�t���[��
#define PLAYER_LANDING_POS_OLD_Y_HEIGHT 10.0f                           // 1F�O�ƌ��݂̈ʒu��Y���W����苗������Ă���ƁA���n���̗p

// �v���C���[�̕ǃL�b�N���[�V��������
#define PLAYER_WALL_KICK_TIME 50                                        // �ǃL�b�N�t���[��

// �v���C���[�̍���U������
#define PLAYER_LEFT_HAND_ORBS_START 0                                   // �J�n���̃I�[�u��
#define PLAYER_LEFT_HAND_ORBS_MAX 200                                   // �ő�I�[�u��(lv4)
#define PLAYER_LEFT_HAND_ORBS_LV1 50                                    // �I�[�u��lv1
#define PLAYER_LEFT_HAND_ORBS_LV2 100                                   // �I�[�u��lv2
#define PLAYER_LEFT_HAND_ORBS_LV3 150                                   // �I�[�u��lv3
#define PLAYER_LEFT_HAND_ATTACK_WEAK_TIME 80                            // ��K�E�̍U���S�̎���
#define PLAYER_LEFT_HAND_ATTACK_WEAK_CREATE_FRAME 40                    // ��K�E�̍U�������t���[��
#define PLAYER_LEFT_HAND_ATTACK_WEAK_KNOCKBACK_VALUE 60.0f              // ��K�E�̃m�b�N�o�b�N��
#define PLAYER_LEFT_HAND_ATTACK_STRONG_TIME 125                         // ���K�E�̍U���S�̎���
#define PLAYER_LEFT_HAND_ATTACK_STRONG_CREATE_FRAME 75                  // ���K�E�̍U�������t���[��
#define PLAYER_LEFT_HAND_ATTACK_STRONG_KNOCKBACK_VALUE 120.0f           // ���K�E�̃m�b�N�o�b�N��

//================================================
// �O���錾
//================================================
class CAnimation;

//================================================
// �N���X�錾
//================================================

// �v���C���[�N���X
class CPlayer : public CCharacter
{
public:
    CPlayer();
    ~CPlayer();

    // �A�j���[�V�����̎��
    typedef enum
    {
        ANIM_IDLE = 0,	              // �ҋ@
        ANIM_MOVE,                    // �ړ�
        ANIM_LEFT_ATTACK_WEAK,        // ����(��)
        ANIM_JUMP,                    // �W�����v
        ANIM_LANDING,                 // ���n
        ANIM_LEFT_ATTACK_STRONG,      // ����(��)
        ANIM_RIGHT_ATTCK_STOP,        // �E��(��~)
        ANIM_RIGHT_ATTACK_WALK_AHEAD, // �E��(�O)
        ANIM_RIGHT_ATTACK_WALK_RIGHT, // �E��(�E)
        ANIM_RIGHT_ATTACK_WALK_LEFT,  // �E��(��)
        ANIM_TAKE_DAMAGE,             // ����
        ANIM_BACK_MOVE,               // ������
        ANIM_BACK_MOVE_ATTACK,        // ����������
        ANIM_RELOAD,                  // �����[�h
        ANIM_WALL_KICK,               // �ǃL�b�N
        ANIM_WALL_KICK2,              // �ǃL�b�N2
        ANIM_DEATH,                   // ���S
        ANIM_MAX,
    }ANIMATION;

    // �v���C�A�u���L�����̐l��
    typedef enum
    {
        PLAYABLE_001 = 0,
        PLAYABLE_MAX,
    }PLAYABLE;

    // ���f���̊e�p�[�c
    typedef enum
    {
        PARTS_HIP = 0,
        PARTS_BODY,
        PARTS_HEAD,
        PARTS_RSHOULDER,
        PARTS_RARM,
        PARTS_RHAND,
        PARTS_LSHOULDER,
        PARTS_LARM,
        PARTS_LHAND,
        PARTS_RTHIGH,
        PARTS_RLEG,
        PARTS_RFOOT,
        PARTS_LTHIGH,
        PARTS_LLEG,
        PARTS_LFOOT,
        PARTS_MAX,
    }PARTS;

    // ����K�E�̎��
    typedef enum
    {
        LEFT_HAND_ATTACK_NONE = 0,  // �Ȃ�
        LEFT_HAND_ATTACK_WEAK,      // ��K�E
        LEFT_HAND_ATTACK_STRONG,    // ���K�E
    }LEFT_HAND_ATTACK;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PLAYABLE playable);

    void SetReloadingEffect(void);                                                      // �����[�h���̋z���G�t�F�N�g
    void SetScatteringBullet(void);                                                     // �ˌ����̔�U
    void SetLandingCrack(D3DXVECTOR3 pos);                                              // ���n���̒n����G�t�F�N�g
    void SetWallKickEffect(D3DXVECTOR3 pos);                                            // �ǃL�b�N���̃G�t�F�N�g
    void Movement(int nCnt, float fSpeed);                                              // �s��
    void Control(int nCnt, float fSpeed);                                               // ����
    void KeyboardOperation(float fSpeed);                                               // �L�[�{�[�h����
    void Attack(int nCnt, D3DXVECTOR3 pos, D3DXVECTOR3 rot);                            // �U������̏���
    void SetLeftHandAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                           // ����U���𐶐�
    void TakeDamage(int nDamage, D3DXVECTOR3 posOld);                                   // �_���[�W���󂯂��Ƃ�
    void Healing(int nHeal);                                                            // ��
    D3DXVECTOR3 RotControl(D3DXVECTOR3 rot);                                            // ��]����
    void PreventReloading(void);                                                        // �����[�h��W����

    void DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                                  // ���ꂽ���̕���
    void SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type);// ���ꂽ���̕��ӃG�t�F�N�g�̔���

    // �Z�b�^�[
    void SetRisingLeftHandOrbs(void);                                                   // �I�[�u���̏㏸
    void SetRemainBulletsFull(void) { m_nRemainBullets = PLAYER_REMAIN_BULLETS_MAX; }   // �c�e�����ő��

    // �Q�b�^�[
    bool GetDisp(void) { return m_bDisp; }
    bool GetReloading(void) { return m_bReloading; }
    int GetRemainBullets(void) { return m_nRemainBullets; }
    int GetLeftHandOrbs(void) { return m_nLeftHandOrbs; }
    int GetInvincibleFrame(void) { return m_nCntInvincibleFrame; }

private:
    static LPD3DXMESH		m_pMesh[PLAYABLE_MAX][PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[PLAYABLE_MAX][PARTS_MAX];
    static DWORD			m_nNumMat[PLAYABLE_MAX][PARTS_MAX];

    PLAYABLE m_playable;                    // �v���C�A�u���L����
    D3DXVECTOR3 m_move;                     // �ړ���

    int m_nAttackCoolTime;                  // �U���̃N�[���^�C��
    bool m_bJump;                           // �W�����v�ł��邩�ǂ���
    D3DXVECTOR3 m_rotDest;                  // ����(�ړI�l)
    D3DXVECTOR3 m_rotMin;                   // ������ς���Ƃ��ɁA�ŒZ�ŐU��������ߎg�p

    int m_nCntDeath;                        // �f�X���̃J�E���^

    bool m_bDisp;                           // �\���t���O
    int m_nCntInvincibleFrame;              // ���G����
    bool m_bBlinking;                       // �_�Œ����ǂ���

    bool m_bWallRun;                        // �Ǔo�蒆���ǂ���

    int m_nRemainBullets;                   // �c�e��
    bool m_bReloading;                      // �����[�h�����ǂ���

    int m_nCntLandingTime;                  // ���n����
    bool m_bJumpOld;                        // 1F�O�A�W�����v�ł������ǂ���

    int m_nCntWallKickTime;                 // �ǃL�b�N�̌o�ߎ��Ԃ𐔂���
    bool m_bWallKickType;                   // �ǃL�b�N�̎�ނ̃t���O

    int m_nLeftHandOrbs;                    // ����̃I�[�u��
    LEFT_HAND_ATTACK m_leftHandAttack;      // ����U���̎��

    int m_nCntFootsteps;                    // �����̃J�E���^
};

#endif