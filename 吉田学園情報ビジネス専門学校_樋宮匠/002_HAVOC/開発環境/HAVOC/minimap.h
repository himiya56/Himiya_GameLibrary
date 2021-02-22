//====================================================================
//
// ミニマップの処理 (minimap.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define MINIMAP_SIZE	 350.0f											// ミニマップのサイズ
#define MINIMAP_POS_X	 1200.0f - MINIMAP_SIZE / 2						// ミニマップのX座標
#define MINIMAP_POS_Y	 155.0f											// ミニマップのY座標
#define MINIMAP_CENTER	 D3DXVECTOR3(MINIMAP_POS_X, MINIMAP_POS_Y)	    // ミニマップの中心座標
#define ENEMY_ICON_SIZE  5.0f											// ミニマップ上のアイコンサイズ
#define MAX_ICON_RANGE	 63.0f											// ミニマップアイコンが中心から離れられる最大距離
#define MAP_VISION_RANGE 5000.0f                                        // ミニマップに表示するプレイヤーからの距離
#define MAP_RELATIVE     MAX_ICON_RANGE / MAP_VISION_RANGE              // 相対座標計算用
#define MINIMAP_ICON_DISP_DELAY        3                                // ミニマップに表示されるまでの遅らせるフレーム数

#define ICON_COLOR_ENEMY D3DCOLOR_RGBA(255, 0, 0, 255)					// ミニマップアイコンの敵の色
#define ICON_COLOR_LIFE  D3DCOLOR_RGBA(0, 255, 0, 255)					// ミニマップアイコンの回復アイテムの色
#define ICON_COLOR_TARGET  D3DCOLOR_RGBA(255, 255, 0, 255)				// ミニマップアイコンのターゲットの色

//================================================
// クラス宣言
//================================================

// ポーズクラス
class CMinimap :public CScene2D
{
public:
    CMinimap();
    ~CMinimap();

    typedef enum
    {
        ICON_NONE,
        ICON_ENEMY,         // 敵
        ICON_LIFE,          // 回復アイテム
        ICON_TARGET,        // ミッションのターゲット
        ICON_MAX,
    }TYPE;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CMinimap *Create(TYPE type);
    float CalculateDistance(void);                                   // プレイヤーと敵の角度計算関数
    float CalculateAngle(void);                                      // プレイヤーと敵の距離計算関数
    void SetFieldPos(const D3DXVECTOR3 pos) { m_fieldPos = pos; }    // フィールド上の座標設定関数
    void UnableIcon(void) { m_bUse = false; }                        // マップアイコンを消す

private:
    D3DXVECTOR3					m_fieldPos;				// 表示するアイコンのフィールド上での座標
    float						m_fAngle;				// プレイヤーとの角度
    float						m_fDistance;			// プレイヤーとの距離
    bool						m_bUse;

    bool						m_bDisp;				// 表示するかどうか
    int							m_nCntTime;				// 時間をカウント

    TYPE						m_type;					// 表示されるアイコンのタイプ
};

#endif