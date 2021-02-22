//===================================================================================
//
// �^���[�̏��� (tower.h)
// Author : �㓡�T�V��(�s��AI�A�G�t�F�N�g) / ��{��(�K�w�\���A�A�j���[�V��������)
//
//===================================================================================
#ifndef _TOWER_H_
#define _TOWER_H_

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

// �^���[
#define TOWER_SPAWN_POS D3DXVECTOR3(10445.0f, 5000.0f, 6400.0f)                 // �o���ʒu
#define TOWER_ACTIVE_COLLISION_SIZE D3DXVECTOR3(500.0f, 3400.0f, 500.0f)        // �\���I�ȓ����蔻��̑傫��
#define TOWER_LIFE 6000                                                         // HP
#define TOWER_SCORE 100000                                                      // �X�R�A
#define TOWER_ORBS 30                                                           // �I�[�u��
#define TOWER_COLLISION_MAX 2                                                   // �����蔻��̍ő吔
#define TOWER_PILLER_COLLISION_SIZE D3DXVECTOR3(600.0f, 2200.0f, 600.0f)        // ���̓����蔻��̑傫��
#define TOWER_CRYSTAL_COLLISION_SIZE D3DXVECTOR3(550.0f, 1200.0f, 550.0f)       // �N���X�^���̓����蔻��̑傫��
#define TOWER_CRYSTAL_ROT_SPEED D3DXToRadian(1.0f)                              // �N���X�^���̉�]���x
#define TOWER_LANDING_CRACK_RADIUS 1000.0f                                      // ���n���ɃK���L����Ԕ��a
#define TOWER_LANDING_SMALL_COLLAPSE_NUMBER_ONCE 50                             // ���n���ɃK���L���΂���

// �U�����
#define TOWER_ENEMY_SPAWN_POS_Y 3500.0f                                         // �X�|�[���n�_�̍���
#define TOWER_ATTACK_RADIUS 1200.0f                                             // �U���𐶐����锼�a
#define TOWER_SET_ENEMY_SPAWN_COUNT 180                                         // �G�𐶐�����܂ł̃J�E���^
#define TOWER_SET_ENEMY_COUNT_BORDER 60                                         // �G�����v���̈ȉ��Ȃ琶�����邩

// �h����
#define TOWER_PILLER_REDUCTION_RATE 0.5f                                        // ���ւ̃_���[�W�y������
#define TOWER_CRYSTAL_DAMAGE_UP_RATE 3.0f                                       // �N���X�^���ւ̃_���[�W�A�b�v�{��

// �񕜃A�C�e���𗎂Ƃ��m��(100���̂Ȃ�ڂ�)
#define TOWER_DROP_HEAL_ITEM 100

//================================================
// �O���錾
//================================================
class CAnimation;

//================================================
// �N���X�錾
//================================================

// �^���[�N���X
class CTower : public CCharacter
{
public:
    CTower();
    ~CTower();

    // �񋓌^�錾(���[�V����)
    typedef enum
    {
        ANIM_IDLE = 0,      // �ҋ@
        ANIM_MAX,           // �ő吔
    }ANIMATION;

    // �񋓌^�錾
    typedef enum
    {
        PARTS_PILLER = 0,
        PARTS_CRYSTAL,
        PARTS_MAX,
    }PARTS;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Movement(void);
    void SetEnemySpawn(void);                                            // �G�����n�_��ݒ�
    void UpdateCollisionPos(void);                                       // �����蔻��̈ʒu�̍X�V

    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CTower *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

    void SetLandingEffect(D3DXVECTOR3 pos);                                                 // ���n�G�t�F�N�g
    void ScatterOrbs(void);                                                                 // �I�[�u���o���܂�
    void DeathCrush(D3DXVECTOR3 pos, D3DXVECTOR3 rot);                                      // ���ꂽ���̕���
    void CollapseEffect(void);                                                              // ����G�t�F�N�g
    void SetDeathCrushEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelEffect::TYPE type);    // ���ꂽ���̕��ӃG�t�F�N�g�̔���
    void TakeDamage(D3DXVECTOR3 pos, int nDamage, PARTS parts);                             // �_���[�W���󂯂��Ƃ��̏���
    void AddMissionClear(void);                                                             // �~�b�V�����N���A����ǉ�����

private:
    static LPD3DXMESH		m_pMesh[PARTS_MAX];
    static LPD3DXBUFFER		m_pBuffMat[PARTS_MAX];
    static DWORD			m_nNumMat[PARTS_MAX];

    D3DXVECTOR3 m_move;                        // �ړ���
    int m_nScore;                              // �X�R�A
    int m_nOrbs;                               // �I�[�u

    int m_nCntTime;                            // ���Ԃ𐔂���

    D3DXVECTOR3 m_rotCrystal;				   // �N���X�^����rot

    bool m_bLanding;                           // ���n�������ǂ���
};

#endif