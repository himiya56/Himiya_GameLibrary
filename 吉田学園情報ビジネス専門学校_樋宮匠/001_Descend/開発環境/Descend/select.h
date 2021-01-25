//=============================================================================
//
// �����I������ [select.h]
// Author : ��{ ��
//
//=============================================================================

// ��d�C���N���[�h�h�~�p
#ifndef _SELECT_H_
#define _SELECT_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon2d.h"
#include "equipment.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SELECT_POLYGON 4

//*****************************************************************************
// �����i�N���X��`
//*****************************************************************************
class CSelect :public CObject
{
public:
	// �����o�֐�
	CSelect();									// �R���X�g���N�^
	~CSelect();									// �f�X�g���N�^

	typedef enum
	{
		SELECT_LEFT = 0,
		SELECT_MIDDLE,
		SELECT_RIGHT,
		SELECT_MAX,
	}SELECT;

	HRESULT Init(void);								// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��
	static CSelect *Create(void);					// �C���X�^���X����
	static HRESULT Load(void);
	static void Unload(void);
private:
	// �����o�ϐ�
	static LPDIRECT3DTEXTURE9		m_apTexture[CEquipment::EQUIPMENT_MAX + 1];		// �e�N�X�`���ւ̃|�C���^
	static bool						m_bSelect;
	int								m_nSelect;
	int								m_nEquipment[MAX_SELECT_POLYGON];
	bool							m_bSize;
	bool							m_bSelection[CEquipment::EQUIPMENT_MAX];
	float							m_fSize;
	CPolygon2D						*m_apPolygon[MAX_SELECT_POLYGON];
};

#endif