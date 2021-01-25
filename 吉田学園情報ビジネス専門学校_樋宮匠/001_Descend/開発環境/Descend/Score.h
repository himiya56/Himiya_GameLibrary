//=============================================================================
//
// �X�R�A���� [score.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _SCORE_H_
#define _SCORE_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "number.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �w�i�N���X��`
//*****************************************************************************
class CScore :public CObject
{
public:
	CScore();
	~CScore();
	HRESULT Init(void);
	void Update();
	void Draw();
	void Uninit();
	static CScore * Create(D3DXVECTOR3 pos,int nValue = 0);
	static void AddScore(const int nValue) { m_nScore += nValue; }
	static int GetScore(void) { return m_nScore; }
private:
	static CNumber					*m_apNumber[NUM_SCORE_POLYGON];
	static int						m_nNum;
	static int						m_nScore;
	D3DXVECTOR3						m_pos;
	float							m_fRot = D3DXToRadian(0.0f);	 // ��]����p�x
	float							m_fSizeX = SCREEN_WIDTH;
	float							m_fSizeY = SCREEN_HEIGHT;
	float							m_fTexUVY;					     // �e�N�X�`��UV�ړ��p�ϐ�
};
#endif