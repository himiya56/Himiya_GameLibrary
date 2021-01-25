//=============================================================================
//
// ポリゴン処理 [object.h]
// Author : 樋宮 匠
//
//=============================================================================

// 二重インクルード防止用
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// オブジェクトクラス定義
//*****************************************************************************
class CObject
{
public:
	// メンバ関数
	CObject();
	virtual ~CObject();

	// 列挙型宣言
	typedef enum
	{
		OBJ_TYPE_NONE = 0,
		OBJ_TYPE_HUD,
		OBJ_TYPE_EXPLOSION,
		OBJ_TYPE_PLAYER,
		OBJ_TYPE_ENEMY,
		OBJ_TYPE_BOSS,
		OBJ_TYPE_BULLET,
		OBJ_TYPE_ITEM,
		OBJ_TYPE_EFFECT,
		OBJ_TYPE_BLOCK,
		OBJ_TYPE_BASE,
		OBJ_TYPE_CLEAR,
		OBJ_TYPE_WALL,
		OBJ_TYPE_MAX
	}OBJ_TYPE;

	// 純粋仮想関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static  void UpdateAll(void);
	static  void DrawAll(void);
	static  void ReleaseAll(void);	// 全オブジェクト破棄関数
	static	CObject *GetObj(int nIndex);
	void SetObjType(const OBJ_TYPE type); // オブジェクトタイプを設定
	OBJ_TYPE GetObjType(void);			 // オブジェクトタイプの取得
protected:
	void Release(void);		// オブジェクト破棄関数
	OBJ_TYPE m_Type;
private:
	// メンバ変数
	static CObject *m_apObject[MAX_POLYGON]; // Objectインスタンス
	static int		m_nNumAll;				 // 生成されたObject派生クラスのインスタンスの数
	int m_nNumIndex;
};

#endif

