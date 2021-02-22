//====================================================================
//
// デバッグ周りの処理 (debug.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _DEBUG_H_
#define _DEBUG_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// マクロ定義
//================================================

#ifdef _DEBUG
// 当たり判定を生成する
#define COLLISION_TEST
#endif

//================================================
// クラス宣言
//================================================

// デバッククラス
class CDebug : public CScene3D
{
public:

    // デバックの種類
    typedef enum
    {
        TYPE_PERMANENT = 0,	    // 永続
        TYPE_MOMENT,	        // 瞬間
        TYPE_MAX			    // 種類の最大数
    }TYPE;

    CDebug();
    ~CDebug();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CDebug *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);

    static void Command(void);  // デバッグコマンド

private:
    static LPDIRECT3DTEXTURE9		m_pTexture;
    static LPD3DXMESH				m_pMesh;
    static LPD3DXBUFFER				m_pBuffMat;
    static DWORD					m_nNumMat;

    int m_nLife;    // 寿命
    TYPE m_type;    // 種類
    
};

#endif