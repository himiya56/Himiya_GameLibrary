//=============================================================================
//
// 装備処理 [equipment.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _EQUIPMENT_H_
#define _EQUIPMENT_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "polygon2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// ウェポンクラス定義
//*****************************************************************************
class CEquipment
{
public:
	CEquipment();
	~CEquipment();

	// 列挙型宣言
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