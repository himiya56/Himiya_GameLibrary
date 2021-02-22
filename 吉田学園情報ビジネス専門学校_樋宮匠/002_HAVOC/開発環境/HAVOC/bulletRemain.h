//====================================================================
//
// �c�e���� (bulletRemain.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _BULLET_REMAIN_H_
#define _BULLET_REMAIN_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene.h"

//================================================
// �O���錾(�s���S�ȃN���X�́Acpp�̕��Ńw�b�_���Ăяo��)
//================================================
class CNumber;

//================================================
// �}�N����`
//================================================
#define MAX_BULLET_REMAIN 3     // �c�e�̌���

#define BULLET_REMAIN_POS D3DXVECTOR3(1054.5f, 612.5f, 0.0f)    // �c�e�̈ʒu
#define BULLET_REMAIN_SIZE D3DXVECTOR3(80.0f, 40.0f, 0.0f)      // �c�e�̑傫��

//================================================
// �N���X�錾
//================================================

// �c�e�̃N���X
class CBulletRemain : public CScene
{
public:
    CBulletRemain();
    ~CBulletRemain();
    static CBulletRemain *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);             // �������J��
    void Update(void);
    void Draw(void);
    void Disp(const int nNumber);                   // �c�e��\��

private:
    CNumber *m_apNumber[MAX_BULLET_REMAIN];  // �������
};

#endif