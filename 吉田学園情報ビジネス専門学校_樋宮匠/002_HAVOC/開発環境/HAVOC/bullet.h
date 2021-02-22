//====================================================================
//
// �e�̏��� (bullet.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// �}�N����`
//================================================

// �����̒e���A�G��u���b�N���U������Ƃ��̃_���[�W������
#define BULLET_NEUTRAL_ATTENUATION_FOR_ENEMY_RATE 0.1f                          // �G�ɑ΂��Ẵ_���[�W������
#define BULLET_NEUTRAL_ATTENUATION_FOR_BLOCK_RATE 0.25f                         // �u���b�N�ɑ΂��Ẵ_���[�W������

// �A�T���g���C�t���̒e
#define BULLET_AR_COLLISION_SIZE D3DXVECTOR3(27.5f, 27.5f, 27.5f)               // �����蔻��̑傫��
#define BULLET_AR_SPEED 100.0f                                                  // ����(100?)(�����~�������A5000�ȓ��ɗ}����)
#define BULLET_AR_COOL_TIME 6                                                   // �N�[���^�C��(6?)
#define BULLET_AR_LIFE 50                                                       // ����(50?)
#define BULLET_AR_DAMAGE 14                                                     // �_���[�W(14?)
#define BULLET_AR_TRAIL_SPEED -10.0f                                            // ������������

// ��
#define BULLET_BLOOD_COLLISION_SIZE D3DXVECTOR3(27.5f, 27.5f, 27.5f)            // �����蔻��̑傫��
#define BULLET_BLOOD_SPEED 12.5f                                                // ����
#define BULLET_BLOOD_LIFE 300                                                   // ����
#define BULLET_BLOOD_TRAIL_SPEED -5.0f                                          // ������������

// �_
#define BULLET_ACID_COLLISION_SIZE D3DXVECTOR3(150.0f, 150.0f, 150.0f)          // �����蔻��̑傫��
#define BULLET_ACID_SPEED 50.0f                                                 // ����
#define BULLET_ACID_LIFE 600                                                    // ����
#define BULLET_ACID_DAMAGE 150                                                  // �_���[�W
#define BULLET_ACID_TRAIL_SPEED -5.0f                                           // ������������

// ����̔��e
#define BULLET_BOMB_COLLISION_SIZE_LV1 60.0f                                    // �����蔻��̑傫��lv1
#define BULLET_BOMB_COLLISION_SIZE_LV2 85.0f                                    // �����蔻��̑傫��lv2
#define BULLET_BOMB_COLLISION_SIZE_LV3 110.0f                                   // �����蔻��̑傫��lv3
#define BULLET_BOMB_COLLISION_SIZE_LV4 135.0f                                   // �����蔻��̑傫��lv4
#define BULLET_BOMB_FIRST_SPEED 100.0f                                          // ����
#define BULLET_BOMB_LIFE 150                                                    // ����
#define BULLET_BOMB_DAMAGE 1                                                    // �_���[�W
#define BULLET_BOMB_TRAIL_SPEED -10.0f                                          // ������������
#define BULLET_BOMB_FALL_DOWN_RATE -0.325f                                      // �e����������

// ����(���x����)
#define BULLET_EXPLOSION_COLLISION_SIZE_LV1 750.0f                              // �����蔻��̑傫��lv1
#define BULLET_EXPLOSION_COLLISION_SIZE_LV2 1125.0f                             // �����蔻��̑傫��lv2
#define BULLET_EXPLOSION_COLLISION_SIZE_LV3 1687.5f                             // �����蔻��̑傫��lv3
#define BULLET_EXPLOSION_COLLISION_SIZE_LV4 2531.25f                            // �����蔻��̑傫��lv4
#define BULLET_EXPLOSION_LIFE_LV1 45                                            // ����lv1
#define BULLET_EXPLOSION_LIFE_LV2 60                                            // ����lv2
#define BULLET_EXPLOSION_LIFE_LV3 75                                            // ����lv3
#define BULLET_EXPLOSION_LIFE_LV4 90                                            // ����lv4
#define BULLET_EXPLOSION_DAMAGE_LV1 300                                         // �_���[�Wlv1
#define BULLET_EXPLOSION_DAMAGE_LV2 600                                         // �_���[�Wlv2
#define BULLET_EXPLOSION_DAMAGE_LV3 1200                                        // �_���[�Wlv3
#define BULLET_EXPLOSION_DAMAGE_LV4 2400                                        // �_���[�Wlv4
#define BULLET_EXPLOSION_ATTENUATION_FOR_PLAYER_RATE 0.35f                      // �v���C���[�ɑ΂��Ẵ_���[�W������
#define BULLET_EXPLOSION_SIZE_ATTENUATION_FOR_PLAYER_RATE 0.5f                  // �v���C���[�ɑ΂��ẴT�C�Y������
#define BULLET_EXPLOSION_START_COLLISION 10                                     // �����蔻�肪��������܂ł̎���
#define BULLET_EXPLOSION_CREATE_SMALL_COLLAPSE_FRAME 2                          // �K���L�𐶐����鎞��
#define BULLET_EXPLOSION_USE_COLLISION_FRAME_FOR_BOSS 10                        // �{�X�ɑ΂��āA�����蔻��𔭐�������t���[��

// �h�q�R�̒e
#define BULLET_CLAP_COLLISION_SIZE D3DXVECTOR3(30.0f, 30.0f, 30.0f)             // �����蔻��̑傫��
#define BULLET_CLAP_SPEED 100.0f                                                // ����
#define BULLET_CLAP_LIFE 60                                                     // ����
#define BULLET_CLAP_DAMAGE 100                                                  // �_���[�W
#define BULLET_CLAP_TRAIL_SPEED -10.0f                                          // ������������

// �Ռ��g
#define BULLET_SHOCK_COLLISION_SIZE D3DXVECTOR3(2531.25f, 2531.25f, 2531.25f)   // �傫��
#define BULLET_SHOCK_COLLISION_REAL_SIZE D3DXVECTOR3(940.0f, 1200.0f, 940.0f)   // ���ۂ̓����蔻��̑傫��
#define BULLET_SHOCK_SPEED 75.0f                                                // ����
#define BULLET_SHOCK_LIFE 75                                                    // ����
#define BULLET_SHOCK_DAMAGE 250                                                 // �_���[�W

//================================================
// �N���X�錾
//================================================

// �e�N���X
class CBullet : public CScene3D
{
public:

    // �q�b�g�G�t�F�N�g
    typedef enum
    {
        HIT_LIVING,        // ����
        HIT_NON_LIVING,    // �񐶕�
        HIT_HUMAN,         // �l��
        HIT_CRYSTAL,       // �N���X�^��
        HIT_MAX            // ��ނ̍ő吔
    }HIT;

    // �e�̎푰
    typedef enum
    {
        TRIBE_PLAYER,   // �v���C���[
        TRIBE_ENEMY,    // �G�l�~�[
        TRIBE_LIQUID,   // �t��
        TRIBE_NEUTRAL,  // ����
        TRIBE_MAX       // ��ނ̍ő吔
    }TRIBE;

    // �e�̎��
    typedef enum
    {
        TYPE_AR,        // �A�T���g���C�t��
        TYPE_BLOOD,     // ��
        TYPE_ACID,      // �_
        TYPE_BOMB,      // ����̔��e
        TYPE_EXPLOSION, // ����
        TYPE_CLAP,      // �h�q�R�̒e
        TYPE_SHOCK,     // �Ռ��g
        TYPE_MAX        // ��ނ̍ő吔
    }TYPE;

    CBullet();
    ~CBullet();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    bool Collision(D3DXVECTOR3 pos);
    void ExplosionCollision(D3DXVECTOR3 pos);
    void SetExplosion(D3DXVECTOR3 pos);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 collisionSize, D3DXVECTOR3 moveAngle, TYPE type, TRIBE tribe);

    D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }                  // �Փ˂̑傫����Ԃ�
    D3DXVECTOR3 GetMiddlePos(D3DXVECTOR3 pos, D3DXVECTOR3 posOld);                  // ���Ԃ̈ʒu��Ԃ�

    void HitEffect(D3DXVECTOR3 pos, HIT hit);
    void AREffect(D3DXVECTOR3 pos);
    void BloodEffect(D3DXVECTOR3 pos);
    void AcidEffect(D3DXVECTOR3 pos);
    void BombEffect(D3DXVECTOR3 pos);
    void ExplosionEffect(D3DXVECTOR3 pos);
    void ClapEffect(D3DXVECTOR3 pos);
    void ShockEffect(D3DXVECTOR3 pos);

private:
    static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // �ǂݍ��ރe�N�X�`��
    static LPD3DXMESH				m_apMesh[TYPE_MAX];     // �ǂݍ��ރ��b�V��
    static LPD3DXBUFFER				m_apBuffMat[TYPE_MAX];  // �ǂݍ��ރ}�e���A���̃o�b�t�@
    static DWORD					m_aNumMat[TYPE_MAX];    // �ǂݍ��ރ}�e���A����

    int m_nLife;                                            // ����
    int m_nDamage;                                          // �_���[�W
    float m_fSpeed;                                         // ����
    D3DXVECTOR3 m_moveAngle;                                // �ړ��̊p�x
    TYPE m_type;                                            // ���
    TRIBE m_tribe;                                          // �푰
    D3DXVECTOR3 m_collisionSize;                            // �����蔻��̑傫��
    D3DXVECTOR3 m_posOld;                                   // �O�̈ʒu

    int m_nCntTime;                                         // ��������Ă���̎��Ԃ𐔂���
};

#endif