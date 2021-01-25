//=============================================================================
//
// 装備選択処理 [select.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _SELECT_H_
#define _SELECT_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon2d.h"
#include "equipment.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SELECT_POLYGON 4

//*****************************************************************************
// 装備品クラス定義
//*****************************************************************************
class CSelect :public CObject
{
public:
	// メンバ関数
	CSelect();									// コンストラクタ
	~CSelect();									// デストラクタ

	typedef enum
	{
		SELECT_LEFT = 0,
		SELECT_MIDDLE,
		SELECT_RIGHT,
		SELECT_MAX,
	}SELECT;

	HRESULT Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理
	static CSelect *Create(void);					// インスタンス生成
	static HRESULT Load(void);
	static void Unload(void);
private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9		m_apTexture[CEquipment::EQUIPMENT_MAX + 1];		// テクスチャへのポインタ
	static bool						m_bSelect;
	int								m_nSelect;
	int								m_nEquipment[MAX_SELECT_POLYGON];
	bool							m_bSize;
	bool							m_bSelection[CEquipment::EQUIPMENT_MAX];
	float							m_fSize;
	CPolygon2D						*m_apPolygon[MAX_SELECT_POLYGON];
};

#endif