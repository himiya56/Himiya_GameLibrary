//=============================================================================
//
// �������� [equipment.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _EQUIPMENT_H_
#define _EQUIPMENT_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "polygon2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �E�F�|���N���X��`
//*****************************************************************************
class CEquipment
{
public:
	CEquipment();
	~CEquipment();

	// �񋓌^�錾
	typedef enum
	{
		EQUIPMENT_LASERSIGHT = 0,
		EQUIPMENT_OPTION,
		EQUIPMENT_AMPRIFICATER,
		EQUIPMENT_GENERATOR,
		EQUIPMENT_MAX,
	}EQUIPMENT;

	HRESULT Init(void);
	void Uninit(void);
	static void SetEquipment(const int equipment);
	static void ClearEquipment(void);
	static void CheckEquipment(void);
	static bool IsUse(const int equipment) { return m_bEquipment[equipment]; }
private:
	static bool m_bEquipment[EQUIPMENT_MAX];
};
#endif