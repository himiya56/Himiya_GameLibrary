//=============================================================================
//
// �|���S������ [polygon2d.h]
// Author : ��{ ��
//
//=============================================================================

// ��d�C���N���[�h�h�~�p
#ifndef _POLYGON2D_H_
#define _POLYGON2D_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// 2D�|���S���N���X��`
//*****************************************************************************
class CPolygon2D : public CObject
{
public:
	// �����o�֐�
	CPolygon2D();									// �R���X�g���N�^
	~CPolygon2D();									// �f�X�g���N�^
	HRESULT Init(void);								// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��
	static CPolygon2D *Create(void);				// �C���X�^���X����
	void BindTexture(const LPDIRECT3DTEXTURE9 pTex); // �e�N�X�`���쐬
	void SetPos(const D3DXVECTOR3 pos);		// ���W�ݒ�p
	void SetCol(const D3DXCOLOR col);
	void SetRot(const float rot);
	void SetSize(const float sizey, const float sizex);
	void SetTexUV(const float leftX, const float rightX, const float topY, const float bottomY);
	D3DXVECTOR3 GetPosition(void) const;
	void GetTexUV(float* leftX, float* rightX, float* topY, float* bottomY);
private:
	// �����o�ϐ�
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3				m_pos;			// �|���S���̈ʒu
	D3DXCOLOR				m_col;			// �|���S���J���[
	float					m_fRot; // ��]����p�x
	float					m_fSizeY;
	float					m_fSizeX;
	float					m_TexLeftX;
	float					m_TexRightX;
	float					m_TexTopY;
	float					m_TexBottomY;
	LPD3DXFONT				m_pFont;		// �t�H���g�ւ̃|�C���^
};

#endif

