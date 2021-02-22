//====================================================================
//
// アイテムの処理 (item.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"
#include "minimap.h"

//================================================
// マクロ定義
//================================================

// ヒール
#define ITEM_HEAL_COLLISION_SIZE D3DXVECTOR3(200.0f, 200.0f, 200.0f)    // 当たり判定の大きさ
#define ITEM_HEAL_LIFE 900                                              // 存在時間
#define ITEM_HEAL_START_BLINKING 180                                    // 点滅開始時間
#define ITEM_HEAL_ROT_SPEED D3DXToRadian(1.0f)                          // 回転速度
#define ITEM_HEAL_SET_EFFECT_INTERVAL 3                                 // エフェクト発生間隔
#define ITEM_HEAL_VALUE 600                                             // 回復量
#define ITEM_HEAL_APPEAR_LAUNCH_VALUE 40.0f                             // 出現時に、打ち上げる量

//================================================
// クラス宣言
//================================================

// アイテムクラス
class CItem : public CScene3D
{
public:

    // アイテムの種類
    typedef enum
    {
        TYPE_HEAL,   // ヒール
        TYPE_MAX     // 種類の最大数
    }TYPE;

    CItem();
    ~CItem();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);

    TYPE GetType(void) { return m_type; }                                   // 種類を返す
    D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }          // 衝突の大きさを返す
    void SetDontUse(void) { m_bUse = false; }                               // 使用しないようにする

    void HealEffect(void);                                                  // 回復アイテムのエフェクト

private:
    static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // 読み込むテクスチャ
    static LPD3DXMESH				m_apMesh[TYPE_MAX];     // 読み込むメッシュ
    static LPD3DXBUFFER				m_apBuffMat[TYPE_MAX];  // 読み込むマテリアルのバッファ
    static DWORD					m_aNumMat[TYPE_MAX];    // 読み込むマテリアル数

    TYPE m_type;                                            // 種類
    D3DXVECTOR3 m_collisionSize;                            // 当たり判定の大きさ
    D3DXVECTOR3 m_posOld;                                   // 1F前の位置
    D3DXVECTOR3 m_move;                                     // 移動量

    int m_nLife;                                            // ライフ
    bool m_bUse;                                            // 使用するかどうか
    bool m_bBlinking;                                       // 点滅中かどうか

    CMinimap *m_pMinimap;									// ミニマップ上のアイコンへのポインタ
};

#endif