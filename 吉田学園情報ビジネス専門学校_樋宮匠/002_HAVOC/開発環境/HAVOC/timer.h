//=============================================================================
//
// �������Ԃ̏��� [timer.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//================================================
// �O���錾
//================================================
class CNumber;

//=============================================================================
//�}�N����`
//=============================================================================

#define TIMER_NUMBER_SIZE_X 40   // �������Ԃ̐����̏c�̃T�C�Y
#define TIMER_NUMBER_SIZE_Y 50   // �������Ԃ̐����̉��̃T�C�Y

// �^�C�}�[�̈ʒu�ƃT�C�Y
#define TIMER_POS D3DXVECTOR3(SCREEN_WIDTH / 2 + 55,SCREEN_HEIGHT - 45.0f,0.0f)
#define TIMER_SIZE D3DXVECTOR3(TIMER_NUMBER_SIZE_X,TIMER_NUMBER_SIZE_Y,0.0f)

#define MAX_SECONDS_DIGIT 2 // �^�C�}�[�̕b�̌��̐�
#define MAX_MINUTES_DIGIT 1 // �^�C�}�[�̕��̌��̐�

// �J�n���̕b
#define START_TIMER_SECONDS 59

// �J�n���̕�
#define MINUTES_TIMER 9

//=============================================================================
//�N���X��`
//=============================================================================
class CTimer :public CScene
{
public:

    CTimer();  
    ~CTimer();

    static CTimer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size); 
    void Update(void);                               
    void Uninit(void);                               
    void Draw(void);                                

    static bool GetTimeUp(void) { return m_bTimeUp; }    // �^�C���A�b�v���擾

private:
    static bool m_bTimeUp;                               // �^�C���A�b�v�������ǂ���
    CNumber *m_pMinutesNumber;                           // ���̏��
    CNumber *m_apSecondsNumber[MAX_SECONDS_DIGIT];       // �b�̏��
    int m_nSeconds;                                      // �b�̃J�E���^
    int m_nMinutes;                                      // ���̃J�E���^
    int m_nCntFrame;                                     // �t���[�����J�E���g
};
#endif