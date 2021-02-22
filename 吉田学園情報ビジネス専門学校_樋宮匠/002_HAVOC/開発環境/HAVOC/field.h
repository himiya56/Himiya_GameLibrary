//====================================================================
//
// フィールドの処理 (character.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"
#include "tile.h"
#include "game.h"

//================================================
// マクロ定義
//================================================

#define MAX_TILE_LINE 80   // 一列のタイルの最大数

//================================================
// クラス宣言
//================================================

// フィールドクラス
class CField : public CScene
{
public:
    CField();
    ~CField();

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void LoadMapData(CGame::STAGE stage, int nWidth, int nDepth);

    static CField *Create(CGame::STAGE stage, int nWidth, int nDepth);

    void SetField(int nWidth, int nDepth);

private:
    CTile					*m_apTile[MAX_TILE_LINE][MAX_TILE_LINE];
    int						m_aGround[MAX_TILE_LINE][MAX_TILE_LINE];
};

#endif