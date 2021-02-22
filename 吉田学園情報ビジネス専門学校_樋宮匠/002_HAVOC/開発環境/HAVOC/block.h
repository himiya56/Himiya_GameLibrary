//====================================================================
//
// �u���b�N�̏��� (block.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene3d.h"
#include "minimap.h"

//================================================
// �}�N����`
//================================================

// �ǃL�b�N�Ŏ󂯂�_���[�W
#define BLOCK_TAKE_DAMAGE_BY_WALL_KICK 50

// �����ɂ���ē����蔻�肪�ς��
#define BLOCK_HORIZONTAL_COLLISION(angle) angle <= D3DXToRadian(45.0f) && angle >= D3DXToRadian(-45.0f) || angle >= D3DXToRadian(135.0f) && angle <= D3DXToRadian(225.0f) || angle >= D3DXToRadian(-225.0f) && angle <= D3DXToRadian(-135.0f)
#define BLOCK_VERTICAL_COLLISION(angle) angle < D3DXToRadian(135.0f) && angle > D3DXToRadian(45.0f) || angle > D3DXToRadian(225.0f) && angle < D3DXToRadian(315.0f) || angle > D3DXToRadian(-135.0f) && angle < D3DXToRadian(-45.0f)|| angle < D3DXToRadian(-225.0f) && angle > D3DXToRadian(-315.0f)

// �r��
#define BLOCK_BUILDING_COLLISION_SIZE D3DXVECTOR3(630.0f, 1650.0f, 630.0f)
#define BLOCK_BUILDING_LIFE 200
#define BLOCK_BUILDING_SCORE 5000
#define BLOCK_BUILDING_ORBS 22

// �M���@
#define BLOCK_SIGNAL_COLLISION_SIZE D3DXVECTOR3(100.0f, 1100.0f, 100.0f)
#define BLOCK_SIGNAL_LIFE 50
#define BLOCK_SIGNAL_SCORE 4500
#define BLOCK_SIGNAL_ORBS 13

// �X��
#define BLOCK_LIGHT_COLLISION_SIZE D3DXVECTOR3(100.0f, 1050.0f, 100.0f)
#define BLOCK_LIGHT_LIFE 50
#define BLOCK_LIGHT_SCORE 4500
#define BLOCK_LIGHT_ORBS 13

// �S�~��
#define BLOCK_DUSTBOX_COLLISION_SIZE D3DXVECTOR3(80.0f, 120.0f, 80.0f)
#define BLOCK_DUSTBOX_LIFE 20
#define BLOCK_DUSTBOX_SCORE 20000
#define BLOCK_DUSTBOX_ORBS 7

// ��
#define BLOCK_WOOD_COLLISION_SIZE D3DXVECTOR3(100.0f, 900.0f, 100.0f)
#define BLOCK_WOOD2_COLLISION_SIZE D3DXVECTOR3(150.0f, 1100.0f, 150.0f)
#define BLOCK_WOOD_LIFE 50
#define BLOCK_WOOD_SCORE 4500
#define BLOCK_WOOD_ORBS 13

// �x���`
#define BLOCK_BENCH_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(330.0f, 100.0f, 170.0f)
#define BLOCK_BENCH_VERTICAL_COLLISION_SIZE D3DXVECTOR3(170.0f, 100.0f, 330.0f)
#define BLOCK_BENCH_LIFE 30
#define BLOCK_BENCH_SCORE 2000
#define BLOCK_BENCH_ORBS 8

// ��
#define BLOCK_TUKUE_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(440.0f, 180.0f, 260.0f)
#define BLOCK_TUKUE_VERTICAL_COLLISION_SIZE D3DXVECTOR3(260.0f, 180.0f, 440.0f)
#define BLOCK_TUKUE_LIFE 50
#define BLOCK_TUKUE_SCORE 2500
#define BLOCK_TUKUE_ORBS 9

// �K�[�h���[��
#define BLOCK_GUARDRAIL_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(600.0f, 150.0f, 100.0f)
#define BLOCK_GUARDRAIL_VERTICAL_COLLISION_SIZE D3DXVECTOR3(100.0f, 150.0f, 600.0f)
#define BLOCK_GUARDRAIL_LIFE 50
#define BLOCK_GUARDRAIL_SCORE 3500
#define BLOCK_GUARDRAIL_ORBS 11
#define BLOCK_GUARDRAIL_ROT_MOVE D3DXVECTOR3(D3DXToRadian(1.0f), 0.0f, 0.0f)

// �W�����O���W��
#define BLOCK_PLAYGROUND_COLLISION_SIZE D3DXVECTOR3(500.0f, 500.0f, 500.0f)
#define BLOCK_PLAYGROUND_LIFE 100
#define BLOCK_PLAYGROUND_SCORE 4500
#define BLOCK_PLAYGROUND_ORBS 16

// ���ӃT�C��
#define BLOCK_SIGN_EMAR_COLLISION_SIZE D3DXVECTOR3(100.0f, 600.0f, 100.0f)
#define BLOCK_SIGN_EMAR_LIFE 50
#define BLOCK_SIGN_EMAR_SCORE 5500
#define BLOCK_SIGN_EMAR_ORBS 11

// �\���H�T�C��
#define BLOCK_SIGN_INTER_COLLISION_SIZE D3DXVECTOR3(100.0f, 600.0f, 100.0f)
#define BLOCK_SIGN_INTER_LIFE 50
#define BLOCK_SIGN_INTER_SCORE 4000
#define BLOCK_SIGN_INTER_ORBS 10

// ���ԋ֎~�T�C��
#define BLOCK_SIGN_NO_PARK_COLLISION_SIZE D3DXVECTOR3(100.0f, 600.0f, 100.0f)
#define BLOCK_SIGN_NO_PARK_LIFE 50
#define BLOCK_SIGN_NO_PARK_SCORE 4000
#define BLOCK_SIGN_NO_PARK_ORBS 10

// ��
#define BLOCK_CAR_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(850.0f, 250.0f, 450.0f)
#define BLOCK_CAR_VERTICAL_COLLISION_SIZE D3DXVECTOR3(450.0f, 250.0f, 850.0f)
#define BLOCK_CAR_LIFE 120
#define BLOCK_CAR_SCORE 5500
#define BLOCK_CAR_ORBS 18

// �R���r�j
#define BLOCK_CONVINI_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(1500.0f, 650.0f, 800.0f)
#define BLOCK_CONVINI_VERTICAL_COLLISION_SIZE D3DXVECTOR3(800.0f, 650.0f, 1500.0f)
#define BLOCK_CONVINI_LIFE 150
#define BLOCK_CONVINI_SCORE 6500
#define BLOCK_CONVINI_ORBS 24

// �R���e�i
#define BLOCK_CONTAINER_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(1700.0f, 550.0f, 600.0f)
#define BLOCK_CONTAINER_VERTICAL_COLLISION_SIZE D3DXVECTOR3(600.0f, 550.0f, 1700.0f)
#define BLOCK_CONTAINER_LIFE 120
#define BLOCK_CONTAINER_SCORE 7500
#define BLOCK_CONTAINER_ORBS 27

// �K�X�^���N
#define BLOCK_GAS_COLLISION_SIZE D3DXVECTOR3(550.0f, 680.0f, 550.0f)
#define BLOCK_GAS_LIFE 120
#define BLOCK_GAS_SCORE 8500
#define BLOCK_GAS_ORBS 30

// �q��
#define BLOCK_SOUKO_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(2000.0f, 600.0f, 1500.0f)
#define BLOCK_SOUKO_VERTICAL_COLLISION_SIZE D3DXVECTOR3(1500.0f, 600.0f, 2000.0f)
#define BLOCK_SOUKO_LIFE 250
#define BLOCK_SOUKO_SCORE 15000
#define BLOCK_SOUKO_ORBS 40

// �S��
#define BLOCK_METAL_COLLISION_SIZE D3DXVECTOR3(250.0f, 2000.0f, 250.0f)
#define BLOCK_METAL_LIFE 200
#define BLOCK_METAL_SCORE 10000
#define BLOCK_METAL_ORBS 35

// �\�[���[�p�l��
#define BLOCK_SOLAR_HORIZONTAL_COLLISION_SIZE D3DXVECTOR3(950.0f, 350.0f, 800.0f)
#define BLOCK_SOLAR_VERTICAL_COLLISION_SIZE D3DXVECTOR3(800.0f, 350.0f, 950.0f)
#define BLOCK_SOLAR_LIFE 100
#define BLOCK_SOLAR_SCORE 5000
#define BLOCK_SOLAR_ORBS 50

// �^���[(���g�p)
#define BLOCK_TOWER_COLLISION_SIZE D3DXVECTOR3(500.0f, 3400.0f, 500.0f)
#define BLOCK_TOWER_LIFE 50
#define BLOCK_TOWER_SCORE 4000
#define BLOCK_TOWER_ORBS 12

// �N���X�^��
#define BLOCK_CRYSTAL_COLLISION_SIZE D3DXVECTOR3(300.0f, 350.0f, 300.0f)
#define BLOCK_CRYSTAL_LIFE 350
#define BLOCK_CRYSTAL_SCORE 50000
#define BLOCK_CRYSTAL_ORBS 35
#define BLOCK_CRYSTAL_SHOOT_UP_MOVE D3DXVECTOR3(0.0f, 50.0f, 0.0f)                                              // �j�󎞂ɑł��グ��ړ���
#define BLOCK_CRYSTAL_SHOOT_UP_ROT_MOVE D3DXVECTOR3(D3DXToRadian(5.0f), D3DXToRadian(1.5f), D3DXToRadian(1.5f)) // �j�󎞂ɑł��グ���]��

//================================================
// �N���X�錾
//================================================

// �u���b�N�N���X
class CBlock : public CScene3D
{
public:

    // �u���b�N�̎��
    typedef enum
    {
        TYPE_BUILDING001,   // �r��1
        TYPE_BUILDING101,   // �r��2
        TYPE_BUILDING201,   // �r��3
        TYPE_BUILDING301,   // �r��4
        TYPE_BUILDING401,   // �r��5
        TYPE_BUILDING501,   // �r��6
        TYPE_SIGNAL,        // �M���@
        TYPE_LIGHT,         // �X��
        TYPE_DUSTBOX,       // �S�~��
        TYPE_LIGHT2,        // �X��2
        TYPE_WOOD1,         // ��1
        TYPE_WOOD2,         // ��2
        TYPE_WOOD3,         // ��3
        TYPE_BENCH,         // �x���`
        TYPE_TUKUE,         // ��
        TYPE_GUARDRAIL,     // �K�[�h���[��
        TYPE_PLAYGROUND,    // �W�����O���W��
        TYPE_SIGN_EMAR,     // ���ӃT�C��
        TYPE_SIGN_INTER,    // �\���H�T�C��
        TYPE_SIGN_NO_PARK,  // ���ԋ֎~�T�C�� 
        TYPE_CAR,           // ��
        TYPE_CONVINI,       // �R���r�j
        TYPE_CONTAINER,     // �R���e�i
        TYPE_GAS,           // �K�X�^���N
        TYPE_SOUKO,         // �q��
        TYPE_METAL,         // �S��
        TYPE_SOLAR,         // �\�[���[�p�l��
        TYPE_TOWER,         // �^���[
        TYPE_CRYSTAL,       // �N���X�^��
        TYPE_MAX            // ��ނ̍ő吔
    }TYPE;

    // ���X�g�q�b�g
    typedef enum
    {
        LAST_HIT_PLAYER = 0,	// �v���C���[�̍U��
        LAST_HIT_ENEMY,	        // �G�̍U��
    }LAST_HIT;

    CBlock();
    ~CBlock();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

    D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }          // �Փ˂̑傫����Ԃ�
    bool GetWallRun(void) { return m_bWallRun; }                            // �Ǒ���ł��邩�ǂ�����Ԃ�
    bool GetEnemyWallRun(void) { return m_bEnemyWallRan; }                  // �G���Ǒ���ł��邩�ǂ�����Ԃ�
    int GetType(void) { return m_nType; }                                   // ��ނ�Ԃ�

    void SetTakeDamage(const int nDamage, const LAST_HIT lastHit) { m_nLife = m_nLife - nDamage; m_lastHit = lastHit; }
    void ScatterOrbs(void);                                                 // �I�[�u���o���܂�
    void DestroyEffect(void);                                               // �|��G�t�F�N�g
    void ExplosionEffect(void);                                             // �����G�t�F�N�g
    void CollapseEffect(void);                                              // ����G�t�F�N�g
    void FallDownModel(void);                                               // ���ɓ|��郂�f��
    void AddMissionClear(void);                                             // �~�b�V�����N���A����ǉ�����

private:
    static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // �ǂݍ��ރe�N�X�`��
    static LPD3DXMESH				m_apMesh[TYPE_MAX];     // �ǂݍ��ރ��b�V��
    static LPD3DXBUFFER				m_apBuffMat[TYPE_MAX];  // �ǂݍ��ރ}�e���A���̃o�b�t�@
    static DWORD					m_aNumMat[TYPE_MAX];    // �ǂݍ��ރ}�e���A����

    int m_nType;                                            // ���
    D3DXVECTOR3 m_collisionSize;                            // �����蔻��̑傫��
    bool m_bWallRun;                                        // �Ǒ���ł��邩�ǂ���
    bool m_bEnemyWallRan;                                   // �G���Ǒ�����ł��邩�ǂ���

    int m_nLife;                                            // ���C�t
    int m_nScore;                                           // �X�R�A
    int m_nOrbs;                                            // �I�[�u
    bool m_bUseDestroyEffect;                               // �|��G�t�F�N�g���g�����ǂ���

    bool m_bUseExplosion;                                   // �������g�����ǂ���

    LAST_HIT m_lastHit;                                     // �Ō�ɍU�����Ă�������

    CMinimap *m_pMinimap;									// �~�j�}�b�v��̃A�C�R���ւ̃|�C���^
};

#endif