//=============================================================================
//
// ���U���g���� [result.h]
// Author : ��{ ��
//
//=============================================================================

// ��d�C���N���[�h�h�~�p
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon2d.h"
#include "number.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RESULT_POLYGON 1
#define MAX_RANKING 6

//*****************************************************************************
// ���U���g�N���X��`
//*****************************************************************************
class CResult :public CObject
{
public:
	// �����o�֐�
	CResult();										// �R���X�g���N�^
	~CResult();										// �f�X�g���N�^
	HRESULT Init(void);								// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��
	static void ReadScore(void);					// �X�R�A�ǂݍ���
	static void WriteScore(void);					// �X�R�A��������
	static CResult *Create(int nScore);					// �C���X�^���X����
	static HRESULT Load(void);
	static void Unload(void);
private:
	// �����o�ϐ�
	static LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	static bool						m_bChageMode;   // ���[�h��ς������̃t���O
	static int						m_nScore[MAX_RANKING];
	static CNumber					*m_apNumber[MAX_RANKING][NUM_SCORE_POLYGON];
	CPolygon2D						*m_pPolygon;
	int								m_nRank;
	int								m_nCountFlash;
};

#endif