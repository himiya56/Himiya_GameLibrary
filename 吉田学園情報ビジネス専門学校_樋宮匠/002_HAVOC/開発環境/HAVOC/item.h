//====================================================================
//
// �A�C�e���̏��� (item.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene3d.h"
#include "minimap.h"

//================================================
// �}�N����`
//================================================

// �q�[��
#define ITEM_HEAL_COLLISION_SIZE D3DXVECTOR3(200.0f, 200.0f, 200.0f)    // �����蔻��̑傫��
#define ITEM_HEAL_LIFE 900                                              // ���ݎ���
#define ITEM_HEAL_START_BLINKING 180                                    // �_�ŊJ�n����
#define ITEM_HEAL_ROT_SPEED D3DXToRadian(1.0f)                          // ��]���x
#define ITEM_HEAL_SET_EFFECT_INTERVAL 3                                 // �G�t�F�N�g�����Ԋu
#define ITEM_HEAL_VALUE 600                                             // �񕜗�
#define ITEM_HEAL_APPEAR_LAUNCH_VALUE 40.0f                             // �o�����ɁA�ł��グ���

//================================================
// �N���X�錾
//================================================

// �A�C�e���N���X
class CItem : public CScene3D
{
public:

    // �A�C�e���̎��
    typedef enum
    {
        TYPE_HEAL,   // �q�[��
        TYPE_MAX     // ��ނ̍ő吔
    }TYPE;

    CItem();
    ~CItem();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);

    TYPE GetType(void) { return m_type; }                                   // ��ނ�Ԃ�
    D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }          // �Փ˂̑傫����Ԃ�
    void SetDontUse(void) { m_bUse = false; }                               // �g�p���Ȃ��悤�ɂ���

    void HealEffect(void);                                                  // �񕜃A�C�e���̃G�t�F�N�g

private:
    static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // �ǂݍ��ރe�N�X�`��
    static LPD3DXMESH				m_apMesh[TYPE_MAX];     // �ǂݍ��ރ��b�V��
    static LPD3DXBUFFER				m_apBuffMat[TYPE_MAX];  // �ǂݍ��ރ}�e���A���̃o�b�t�@
    static DWORD					m_aNumMat[TYPE_MAX];    // �ǂݍ��ރ}�e���A����

    TYPE m_type;                                            // ���
    D3DXVECTOR3 m_collisionSize;                            // �����蔻��̑傫��
    D3DXVECTOR3 m_posOld;                                   // 1F�O�̈ʒu
    D3DXVECTOR3 m_move;                                     // �ړ���

    int m_nLife;                                            // ���C�t
    bool m_bUse;                                            // �g�p���邩�ǂ���
    bool m_bBlinking;                                       // �_�Œ����ǂ���

    CMinimap *m_pMinimap;									// �~�j�}�b�v��̃A�C�R���ւ̃|�C���^
};

#endif