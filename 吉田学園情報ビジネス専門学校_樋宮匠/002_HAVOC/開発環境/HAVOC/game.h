//=============================================================================
//
// ゲーム処理 [game.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "mode.h"
#include "tile.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;
class CScore;
class CBulletRemain;
class CMission;
class CScene2D;
class CPause;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// ミッションUIの最大数
#define MISSION_UI_MAX 2

// ゲーム状態の管理フレーム
#define COUNT_WAIT_FINISH 300	    // ゲーム終了時に、タイトル画面へ行くまでの時間

// 各ステージごとの、マップ制限、ミッションの最大数
// ステージ1
#define STAGE1_MAP_LIMIT 65.0f
#define STAGE1_WEST_LIMIT -(TILE_SIZE / 2)
#define STAGE1_SOUTH_LIMIT -(TILE_SIZE / 2)
#define STAGE1_NORTH_LIMIT STAGE1_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE1_EAST_LIMIT STAGE1_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE1_MISSION1_MAX 50
#define STAGE1_MISSION2_MAX 4

// ステージ2
#define STAGE2_MAP_LIMIT 65.0f
#define STAGE2_WEST_LIMIT -(TILE_SIZE / 2)
#define STAGE2_SOUTH_LIMIT -(TILE_SIZE / 2)
#define STAGE2_NORTH_LIMIT STAGE2_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE2_EAST_LIMIT STAGE2_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE2_MISSION1_MAX 8
#define STAGE2_MISSION2_MAX 1

// ステージファイナル
#define STAGE_FINAL_MAP_LIMIT 70.0f
#define STAGE_FINAL_WEST_LIMIT -(TILE_SIZE / 2)
#define STAGE_FINAL_SOUTH_LIMIT -(TILE_SIZE / 2)
#define STAGE_FINAL_NORTH_LIMIT STAGE_FINAL_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE_FINAL_EAST_LIMIT STAGE_FINAL_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE_FINAL_MISSION1_MAX 3
#define STAGE_FINAL_MISSION2_MAX 1

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CMode
{
public:

    // ゲームの状態
    typedef enum
    {
        STATE_START = 0,           // 開始
        STATE_NORMAL,              // 通常
        STATE_FINISH,	           // 終了
        STATE_IN_FADE,             // フェード中
        STATE_PAUSE,	           // ポーズ
        STATE_MAX			       // 状態の最大数
    }STATE;

    // 生成する敵の種類
    typedef enum
    {
        ENEMYSET_WHITE_ANT = 0,    // 白アリ
        ENEMYSET_BLACK_ANT,        // 巨大黒アリ
        ENEMYSET_YELLOW_SPIDER,	   // 黄グモ
        ENEMYSET_BLACK_SPIDER,	   // 巨大黒グモ
        ENEMYSET_UFO,              // UFO
        ENEMYSET_MAX			   // 生成する敵の種類の最大数
    }ENEMYSET;

    // ステージ
    typedef enum
    {
        STAGE_NONE = 0,            // なし
        STAGE_1,                   // ステージ1
        STAGE_2,	               // ステージ2
        STAGE_FINAL,	           // 最終ステージ
    }STAGE;

    // 現在のミッション
    typedef enum
    {
        CURRENT_MISSION_1 = 0,     // ミッション1
        CURRENT_MISSION_2,	       // ミッション2
    }CURRENT_MISSION;

    // マップ制限
    typedef struct
    {
        float fSouth;              // 南
        float fNorth;              // 北
        float fEast;               // 東
        float fWest;               // 西
    }MAP_LIMIT;

    CGame();
    ~CGame();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);

    void Judgment(void);                                                            // 勝敗判定
    void ManageMission(void);                                                       // ミッションを管理
    void ManageState(void);                                                         // ゲーム状態の管理

    static void LoadMapData(STAGE stage);                                           // ステージを読み込み
    static void LoadEnemyData(STAGE stage);                                         // エネミーを読み込み

    // セッター
    static void SetState(STATE state) { m_state = state; }                          // ゲームの状態を設定
    static void SetStage(STAGE stage);                                              // ステージを設定
    static void SetMissionJudgment(bool bJudgment) { m_bMissionClear = bJudgment; } // ミッションクリアしたかどうかを設定
    static void SetClearNumMission1(void) { m_nClearNum_Misson1++; }                // ミッション1のクリア数を1上げる
    static void SetClearNumMission2(void) { m_nClearNum_Misson2++; }                // ミッション2のクリア数を1上げる

    // ゲッター
    static STATE GetState(void) { return m_state; }                                 // ゲームの状態を取得
    static CPlayer* GetPlayer(void) { return m_pPlayer; }                           // プレイヤーを取得
    static CScore *GetScore(void) { return m_pScore; }                              // スコアを取得
    static CBulletRemain *GetBulletRemain(void) { return m_pBulletRemain; }         // 残弾を取得
    static STAGE GetStage(void) { return m_stage; }                                 // ステージを取得
    static MAP_LIMIT GetMapLimit(void) { return m_mapLimit; }                       // マップ制限を取得
    static bool GetMissionJudgment(void) { return m_bMissionClear; }                // ミッションクリアしたかどうかを取得
    static CURRENT_MISSION GetCurrentMission(void) { return m_currentMission; }     // 現在のミッションを取得

private:

    static CScore *m_pScore;                            // スコアのポインタ
    static CPlayer *m_pPlayer;                          // プレイヤーのポインタ
    static CBulletRemain *m_pBulletRemain;              // 残弾数へのポインタ
    static CMission *m_pMission;                        // ミッションへのポインタ
    static CScene2D *m_apMissionUI[MISSION_UI_MAX];     // ミッションのUI
    static CPause *m_pPause;                            // ポーズのポインタ

    int m_nCntGameTime;                                 // ゲーム時間のカウンタ

    static STATE m_state;                               // 状態
    static STAGE m_stage;                               // ステージ
    static MAP_LIMIT m_mapLimit;                        // マップ制限
    static bool m_bMissionClear;                        // ミッションクリアしたかどうか

    static int m_nClearNum_Misson1;                     // ミッション1のクリア数
    static int m_nClearNum_Misson2;                     // ミッション2のクリア数
    static CURRENT_MISSION m_currentMission;            // 現在のミッション
};

#endif