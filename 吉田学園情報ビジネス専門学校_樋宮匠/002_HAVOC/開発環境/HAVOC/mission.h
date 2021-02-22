//====================================================================
//
// �~�b�V�������� (mission.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _MISSION_H_
#define _MISSION_H_

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
#define MAX_MISSION_NUMBER 2                              // �~�b�V�����̌���

#define MISSION_POS D3DXVECTOR3(300.0f, 295.5f, 0.0f)     // �~�b�V�����̐����̈ʒu
#define MISSION_SIZE D3DXVECTOR3(42.5f, 30.0f, 0.0f)      // �~�b�V�����̐����̑傫��

//================================================
// �N���X�錾
//================================================

// �~�b�V�����̃N���X
class CMission : public CScene
{
public:
    CMission();
    ~CMission();
    static CMission *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    void DispFirst(const int nNumber);                     // 1�Ԗڂ̃~�b�V������\��
    void DispSecond(const int nNumber);                    // 2�Ԗڂ̃~�b�V������\��

private:
    CNumber *m_apFirstNumber[MAX_MISSION_NUMBER];          // 1�Ԗڂ̃~�b�V�����̏��
    CNumber *m_apSecondNumber[MAX_MISSION_NUMBER];         // 2�Ԗڂ̃~�b�V�����̏��
};

#endif