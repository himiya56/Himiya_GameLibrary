//=============================================================================
//
// �����\������ [number.h]
// Author : ��{ ��
//
//=============================================================================

// ��d�C���N���[�h�h�~�p
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUMBER_HEIGHT 80
#define NUMBER_WIDTH  80
#define NUM_SCORE_POLYGON  8   // �X�R�A�̌���

//*****************************************************************************
// 2D�|���S���N���X��`
//*****************************************************************************
class CNumber
{
public:
	// �����o�֐�
	CNumber();										// �R���X�g���N�^
	~CNumber();										// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);								// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��
	static CNumber *Create(D3DXVECTOR3 pos);					// �C���X�^���X����
	void BindTexture(const LPDIRECT3DTEXTURE9 pTex, const float segX, const float segY); // �e�N�X�`���쐬
	void SetPolygonPos(const D3DXVECTOR3 pos);		// ���W�ݒ�p
	void SetPolygonRot(const float rot);
	void SetPolygonSize(const float sizey, const float sizex);
	void SetTexSeg(const float segx, const float segy);
	void SetNumber(const int nNum);
	void SetCol(D3DXCOLOR col) { m_col = col; }
	static HRESULT Load(void);
	static void Unload(void);
private:
	// �����o�ϐ�
	static LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff = NULL;		// ���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3						m_pos;			// �|���S���̈ʒu
	D3DXCOLOR						m_col;
	int								m_nNumber;
	float							m_fRot = D3DXToRadian(0.0f); // ��]����p�x
	float							m_fSizeY = POLYGON_SIZE;
	float							m_fSizeX = POLYGON_SIZE;
	float							m_fTexUVX = 1.0f;	// �e�N�X�`��UV x���W
	float							m_fTexUVY = 1.0f;	// �e�N�X�`��UV y���W
	float							m_fTexSegX = 1.0f;  // �e�N�X�`���������� 
	float							m_fTexSegY = 1.0f;  // �e�N�X�`���c������
};

#endif

