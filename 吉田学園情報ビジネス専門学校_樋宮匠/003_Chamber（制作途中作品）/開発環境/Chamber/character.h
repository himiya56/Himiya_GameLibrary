//==========================================================================================
//
// �L�����N�^�[�̏��� (character.h)
// Author : ��{��(�N���X�݌v�A�K�w�\��) / �㓡�T�V��(���������蔻��AHP��_���[�W����̊Ǘ�)
//
//==========================================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "object.h"
#include "model.h"
#include "animation.h"

//================================================
// �}�N����`
//================================================
#define CHARCTER_COLLISION_MAX 18   // �L�����N�^�[�̓����蔻��̍ő吔

// �v���C���[���G�ɐG�ꂽ���ɗ^����_���[�W(��ޕ�)
#define PLAYER_TOUCH_SPIDER 375
#define PLAYER_TOUCH_ANT    250
#define PLAYER_TOUCH_UFO    300
#define PLAYER_TOUCH_TOWER  1000
#define PLAYER_TOUCH_WYVERN 500

//================================================
// �N���X�錾
//================================================
class CAnimation;

// �L�����N�^�[�N���X
class CCharacter : public CObject
{
public:
    CCharacter();
    ~CCharacter();

    // �����蔻��̏��
    typedef struct
    {
        D3DXVECTOR3 pos;        // �ʒu
        D3DXVECTOR3 size;       // ��`�̃T�C�Y
    }COLLISION;

    virtual HRESULT Init(void);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    HRESULT LoadModelData(char* cFilePass);

    // �Z�b�^�[
    void SetPartPos(int index, D3DXVECTOR3 pos) { m_apModel[index]->SetPos(pos); }
    void SetPartRot(int index, D3DXVECTOR3 rot) { m_apModel[index]->SetRot(rot); }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetPartNum(int nPartNum) { m_nPartsNum = nPartNum; }
    void SetLife(const int nLife) { m_nLife = nLife; }
    void SetPosOld(const D3DXVECTOR3 posOld) { m_posOld = posOld; }

    void BindMesh(CAnimation::ANIM_TYPE type, int nPartIndex, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat);

    // �Q�b�^�[
    D3DXVECTOR3 GetDefaultPos(int index) { return m_posDefault[index]; }
    D3DXVECTOR3 GetPartsRot(int index) { return m_apModel[index]->GetRot(); }
    CModel * GetParts(int index) { return m_apModel[index]; }
    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetRot(void) { return m_rot; }
    CAnimation * GetAnimation(void) { return m_pAnimation; }
    int GetLife(void) { return m_nLife; }
    D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

    // �����蔻��Ɋւ���Q�b�^�[��Z�b�^�[
    void SetActiveCollisionSize(const D3DXVECTOR3 collisionSize) { m_activeCollisionSize = collisionSize; }
    D3DXVECTOR3 GetActiveCollisionSize(void) { return m_activeCollisionSize; }
    COLLISION GetCollision(int nCnt) { return m_aCollision[nCnt]; }                         // �e�p�[�c�́A�Փ˂Ɋւ������Ԃ�
    void SetCollisionPos(int nCnt, D3DXVECTOR3 pos) { m_aCollision[nCnt].pos = pos; }       // �e�p�[�c�́A�Փ˂̈ʒu��ݒ�
    void SetCollisionSize(int nCnt, D3DXVECTOR3 size) { m_aCollision[nCnt].size = size; }   // �e�p�[�c�́A�Փ˂̃T�C�Y��ݒ�
    int GetCntCollision(void) { return m_nCntCollision; }                                   // �����蔻��̐���Ԃ�
    void SetCntCollision(int nCnt) { m_nCntCollision = nCnt; }                              // �����蔻��̐���ݒ�

    // �A�j���[�V�������g�p���Ȃ��ꍇ�̃Z�b�^�[
    void SetUnableAnimation(void) { m_bUseAnimation = false; }

private:
    LPD3DXMESH				m_pMesh[CHARCTER_PARTS_MAX];
    LPD3DXBUFFER			m_pBuffMat[CHARCTER_PARTS_MAX];
    DWORD					m_nNumMat[CHARCTER_PARTS_MAX];
    D3DXVECTOR3				m_posDefault[CHARCTER_PARTS_MAX];
    int						m_nIndexParent[CHARCTER_PARTS_MAX];

    D3DXVECTOR3		m_pos;                                              // �S�Ă̐e�̈ʒu                                        // 1F�O�̈ʒu
    D3DXVECTOR3		m_rot;                                              // �S�Ă̐e�̌���
    int				m_nPartsNum;                                        // �p�[�c��

    CModel			*m_apModel[CHARCTER_PARTS_MAX];                     // ���f���ւ̃|�C���^
    CAnimation		*m_pAnimation;                                      // �A�j���[�V�����ւ̃|�C���^

    D3DXVECTOR3 m_activeCollisionSize;                                  // �\���I�ȓ����蔻��̑傫��(�ǂɑ΂��Ďg�����肷��)
    int m_nLife;                                                        // HP
    D3DXVECTOR3		m_posOld;                                           // 1F�O�̈ʒu

    char* m_cFilePass;                                                  // �t�@�C���̃p�X

    COLLISION m_aCollision[CHARCTER_COLLISION_MAX];                     // �����蔻��̏��
    int m_nCntCollision;                                                // �����蔻��̐�

	CAnimation::ANIM_TYPE m_AnimType;									// �A�j���[�V�����^�C�v
    bool m_bUseAnimation;												// �A�j���[�V���������邩�ǂ���
};

#endif