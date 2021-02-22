//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : 後藤慎之助
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "game.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "animation.h"
#include "camera.h"
#include "fade.h"
#include "block.h"
#include "character.h"
#include "input.h"
#include "scene.h"
#include "library.h"
#include "effect3d.h"
#include "field.h"
#include "timer.h"
#include "camera.h"
#include "debug.h"
#include "bullet.h"
#include "score.h"
#include "player.h"
#include "spider.h"
#include "ant.h"
#include "item.h"
#include "bulletRemain.h"
#include "mission.h"
#include "sky.h"
#include "wall.h"
#include "ocean.h"
#include "force.h"
#include "whiteFade.h"
#include "ufo.h"
#include "modelEffect.h"
#include "tower.h"
#include "wyvern.h"
#include "pause.h"

//========================================
// マクロ定義
//========================================

// ゲーム状態の管理フレーム
#define START_MISSION 30    // ミッションの開始

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CPlayer *CGame::m_pPlayer = NULL;
CScore *CGame::m_pScore = NULL;
CBulletRemain *CGame::m_pBulletRemain = NULL;
CMission *CGame::m_pMission = NULL;
CScene2D *CGame::m_apMissionUI[MISSION_UI_MAX] = {};
CPause *CGame::m_pPause = NULL;

CGame::STATE CGame::m_state = STATE_NORMAL;
CGame::STAGE CGame::m_stage = STAGE_NONE;
CGame::MAP_LIMIT CGame::m_mapLimit = {};
bool CGame::m_bMissionClear = false;

int CGame::m_nClearNum_Misson1 = 0;
int CGame::m_nClearNum_Misson2 = 0;
CGame::CURRENT_MISSION CGame::m_currentMission = CURRENT_MISSION_1;

//=============================================================================
// ゲームのコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CGame::CGame()
{
    m_pPlayer = NULL;
    m_pScore = NULL;
    m_pBulletRemain = NULL;
    m_pMission = NULL;
    m_pPause = NULL;

    memset(m_apMissionUI, 0, sizeof(m_apMissionUI));
    m_nCntGameTime = 0;

    // 静的メンバ変数を初期化
    m_state = STATE_START;
    m_stage = STAGE_NONE;
    m_mapLimit = {};
    m_bMissionClear = false;

    m_nClearNum_Misson1 = 0;
    m_nClearNum_Misson2 = 0;
    m_currentMission = CURRENT_MISSION_1;
}

//=============================================================================
// ゲームのデストラクタ
// Author : 後藤慎之助
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CGame::Init(void)
{
    // 各UIをクリエイト（描画順注意）
    //====================================================================================
    //スコア
    CUI::Create(UI_SCORE_POS, UI_SCORE_SIZE, DEFAULT_COLOR, CUI::TYPE_SCORE);

    // HPゲージ
    CUI::Create(UI_STAMINA_POS, UI_STAMINA_SIZE, DEFAULT_COLOR, CUI::TYPE_HP_FRAME);
    CUI::Create(UI_GAUGE_POS, UI_GAUGE_SIZE, DEFAULT_COLOR, CUI::TYPE_HP_GAUGE);

    //HPロゴ
    CUI::Create(UI_HP_LOGO_POS, UI_HP_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_HP_LOGO);

    //左手フレーム
    CUI::Create(UI_LEFTHAND_FRAME_POS, UI_LEFTHAND_FRAME_SIZE, DEFAULT_COLOR, CUI::TYPE_LEFTHAND_FRAME);

    //左手ゲージ背景
    CUI::Create(UI_LEFTHAND_GAUGE_BG_POS, UI_LEFTHAND_GAUGE_BG_SIZE, DEFAULT_COLOR, CUI::TYPE_LEFTHAND_GAUGEBG);

    //左手ゲージ
    CUI::Create(UI_LEFTHAND_GAUGE_POS, UI_LEFTHAND_GAUGE_SIZE, DEFAULT_COLOR, CUI::TYPE_LEFTHAND_GAUGE);

    //左手ゲージフレーム
    CUI::Create(UI_LEFTHAND_GAUGE_FRAME_POS, UI_LEFTHAND_GAUGE_FRAME_SIZE, DEFAULT_COLOR, CUI::TYPE_LEFTHAND_GAUGEFRAME);

    //左手ゲージ仕切り
    CUI::Create(UI_LEFTHAND_PARTITION_POS, UI_LEFTHAND_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_LEFTHAND_PARTITION);

    //右手ロゴ
    CUI::Create(UI_RIGHTHAND_LOGO_POS, UI_RIGHTHAND_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_RIGHTHAND_LOGO);

    //左手残弾
    CUI::Create(UI_RIGHTHAND_STAMINA_POS, UI_RIGHTHAND_STAMINA_SIZE, DEFAULT_COLOR, CUI::TYPE_RIGHTHAND_BULLET_UI);

    //マップ
    CUI::Create(UI_MINIMAP_POS, UI_MINIMAP_SIZE, DEFAULT_COLOR, CUI::TYPE_MINIMAP);

    // ロックオン
    CUI::Create(LOCK_ON_POS, LOCK_ON_SIZE, DEFAULT_COLOR, CUI::TYPE_LOCK_ON);

    // 細胞暴走まで
    CUI::Create(UI_LIMIT_POS, UI_LIMIT_SIZE, DEFAULT_COLOR, CUI::TYPE_LIMIT);

    // ミッションのロゴ
    CUI::Create(UI_MISSION_LOGO_POS, UI_MISSION_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_MISSION_LOGO);
    //====================================================================================

    // タイマーの生成
    CTimer::Create(TIMER_POS, TIMER_SIZE);

    // スコアの生成
    m_pScore = CScore::Create(SCORE_POS, SCORE_SIZE);

    // キャラクターの生成
    m_pPlayer = CPlayer::Create(PLAYER_START_POS_STAGE1, DEFAULT_VECTOR, CPlayer::PLAYABLE_001);

    // 残弾数表示
    m_pBulletRemain = CBulletRemain::Create(BULLET_REMAIN_POS, BULLET_REMAIN_SIZE);
    m_pBulletRemain->Disp(PLAYER_REMAIN_BULLETS_MAX);

    // ミッション表示
    m_pMission = CMission::Create(MISSION_POS, MISSION_SIZE);
    m_pMission->DispFirst(0);
    m_pMission->DispSecond(0);

    // リロード表記の生成
    CUI::Create(RELOAD_POS, RELOAD_SIZE, DEFAULT_COLOR, CUI::TYPE_RELOAD);

    // スカイボックスの生成
    CSky::Create();

    // 海の生成
    COcean::Create(COcean::TYPE_SURFACE_0);
    COcean::Create(COcean::TYPE_SURFACE_1);
    COcean::Create(COcean::TYPE_SURFACE_2);

    // ポーズの生成
    m_pPause = CPause::Create();

    // ステージの設定
    SetStage(STAGE_1);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CGame::Uninit(void)
{
    // 確保したスコアとプレイヤーと各数字UIのメモリは、CSceneのReleaseAllで開放されている

    // 全ての音を停止
    CSound *pSound = CManager::GetSound();
    pSound->StopAll();

    // ポーズを破棄
    if (m_pPause != NULL)
    {
        m_pPause->Uninit();
        delete m_pPause;
        m_pPause = NULL;
    }
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CGame::Update(void)
{
    // ゲーム状態の管理
    ManageState();

#ifdef _DEBUG
    CDebug::Command();
#endif
}

//=============================================================================
// ステージの読み込み
// Author : 後藤慎之助
//=============================================================================
void CGame::SetStage(STAGE stage)
{
    // 引数をメンバ変数に結びつける
    m_stage = stage;

    // プレイヤーの残弾数を最大にする
    m_pPlayer->SetRemainBulletsFull();

    // 残弾数表示を最大に更新
    m_pBulletRemain->Disp(PLAYER_REMAIN_BULLETS_MAX);

    // ミッションクリアのフラグをfalseに戻す
    m_bMissionClear = false;

    // 現在のミッションを1に戻す
    m_currentMission = CURRENT_MISSION_1;

    // ミッションの達成数をリセット
    m_nClearNum_Misson1 = 0;
    m_nClearNum_Misson2 = 0;
    m_pMission->DispFirst(m_nClearNum_Misson1);
    m_pMission->DispSecond(m_nClearNum_Misson2);

    // ミッションのUIがあるなら、削除
    if (m_apMissionUI[0] != NULL)
    {
        m_apMissionUI[0]->Uninit();
        m_apMissionUI[0] = NULL;
    }
    if (m_apMissionUI[1] != NULL)
    {
        m_apMissionUI[1]->Uninit();
        m_apMissionUI[1] = NULL;
    }

    // ステージによって、マップの制限やミッションUIを変える
    CSound *pSound = CManager::GetSound();
    switch (stage)
    {
    case STAGE_1:
        m_mapLimit.fWest = STAGE1_WEST_LIMIT;
        m_mapLimit.fSouth = STAGE1_SOUTH_LIMIT;
        m_mapLimit.fNorth = STAGE1_NORTH_LIMIT;
        m_mapLimit.fEast = STAGE1_EAST_LIMIT;
        CWall::SetWall(STAGE1_MAP_LIMIT);

        // ミッション1
        if (m_apMissionUI[0] == NULL)
        {
            m_apMissionUI[0] = CUI::Create(UI_MISSION_POS_1, UI_MISSION_SIZE_1, DEFAULT_COLOR, CUI::TYPE_MISSION_1);
            m_apMissionUI[0]->SetTexturePlace(2, 2);
        }

        // ミッション2
        if (m_apMissionUI[1] == NULL)
        {
            m_apMissionUI[1] = CUI::Create(UI_MISSION_POS_2, UI_MISSION_SIZE_2, DEFAULT_COLOR, CUI::TYPE_MISSION_2);
            m_apMissionUI[1]->SetTexturePlace(1, 2);
        }

        // プレイヤーの場所を変える
        m_pPlayer->SetPos(PLAYER_START_POS_STAGE1);

        // カメラのロックオン場所を変える
        CManager::GetCamera()->CCamera::ResetGameCamera(PLAYER_START_POS_STAGE1, PLAYER_START_ROT_STAGE1);

        // BGMを再生
        pSound->Play(CSound::LABEL_BGM_STAGE_01_NORMAL);

        break;

    case STAGE_2:
        m_mapLimit.fWest = STAGE2_WEST_LIMIT;
        m_mapLimit.fSouth = STAGE2_SOUTH_LIMIT;
        m_mapLimit.fNorth = STAGE2_NORTH_LIMIT;
        m_mapLimit.fEast = STAGE2_EAST_LIMIT;
        CWall::SetWall(STAGE2_MAP_LIMIT);

        // ミッション3
        if (m_apMissionUI[0] == NULL)
        {
            m_apMissionUI[0] = CUI::Create(UI_MISSION_POS_1, UI_MISSION_SIZE_1, DEFAULT_COLOR, CUI::TYPE_MISSION_3);
            m_apMissionUI[0]->SetTexturePlace(2, 2);
        }

        // ミッション4
        if (m_apMissionUI[1] == NULL)
        {
            m_apMissionUI[1] = CUI::Create(UI_MISSION_POS_2, UI_MISSION_SIZE_2, DEFAULT_COLOR, CUI::TYPE_MISSION_4);
            m_apMissionUI[1]->SetTexturePlace(1, 2);
        }

        // プレイヤーの場所を変える
        m_pPlayer->SetPos(PLAYER_START_POS_STAGE2);

        // カメラのロックオン場所を変える
        CManager::GetCamera()->CCamera::ResetGameCamera(PLAYER_START_POS_STAGE2, PLAYER_START_ROT_STAGE2);

        // BGMを再生
        pSound->Play(CSound::LABEL_BGM_STAGE_02_NORMAL);

        break;

    case STAGE_FINAL:
        m_mapLimit.fWest = STAGE_FINAL_WEST_LIMIT;
        m_mapLimit.fSouth = STAGE_FINAL_SOUTH_LIMIT;
        m_mapLimit.fNorth = STAGE_FINAL_NORTH_LIMIT;
        m_mapLimit.fEast = STAGE_FINAL_EAST_LIMIT;
        CWall::SetWall(STAGE_FINAL_MAP_LIMIT);

        // ミッション5
        if (m_apMissionUI[0] == NULL)
        {
            m_apMissionUI[0] = CUI::Create(UI_MISSION_POS_1, UI_MISSION_SIZE_1, DEFAULT_COLOR, CUI::TYPE_MISSION_5);
            m_apMissionUI[0]->SetTexturePlace(2, 2);
        }

        // ミッション6
        if (m_apMissionUI[1] == NULL)
        {
            m_apMissionUI[1] = CUI::Create(UI_MISSION_POS_2, UI_MISSION_SIZE_2, DEFAULT_COLOR, CUI::TYPE_MISSION_6);
            m_apMissionUI[1]->SetTexturePlace(1, 2);
        }

        // プレイヤーの場所を変える
        m_pPlayer->SetPos(PLAYER_START_POS_STAGE_FINAL);

        // カメラのロックオン場所を変える
        CManager::GetCamera()->CCamera::ResetGameCamera(PLAYER_START_POS_STAGE_FINAL, PLAYER_START_ROT_STAGE_FINAL);

        // BGMを再生
        pSound->Play(CSound::LABEL_BGM_STAGE_03_NORMAL);

        break;
    }

    // 敵の配置読み込み
    LoadEnemyData(stage);

    // マップの読み込み
    LoadMapData(stage);
}

//=============================================================================
// ゲーム状態の管理
// Author : 後藤慎之助
//=============================================================================
void CGame::ManageState(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 音を取得
    CSound *pSound = CManager::GetSound();

    // カメラ取得
    CCamera*pCamera = CManager::GetCamera();

    switch (m_state)
    {
    case STATE_START:

        // マウスカーソルの非表示
        ShowCursor(FALSE);

        // カメラ追従
        pCamera->CCamera::SetState(CCamera::STATE_LOCK_ON);

        // カウンタを加算
        m_nCntGameTime++;

        // 一定フレームで、ゲーム中に
        if (m_nCntGameTime == START_MISSION)
        {
            // カウンタリセット
            m_nCntGameTime = 0;

            // ミッションスタート表示
            CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_START);

            // ゲーム中に
            m_state = STATE_NORMAL;
        }

        break;

    case STATE_NORMAL:

        // マウスカーソルの非表示
        ShowCursor(FALSE);

        // ミッションの管理
        ManageMission();

        // カメラ追従
        pCamera->CCamera::SetState(CCamera::STATE_LOCK_ON);

        // スタートボタンが押されたら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_P) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // ポーズ音
            pSound->Play(CSound::LABEL_SE_SYSTEM_PAUSE);

            // ポーズ状態にする
            CPause::SetState(CPause::STATE_PAUSE);
            CGame::SetState(CGame::STATE_PAUSE);
            pCamera->SetState(CCamera::STATE_NONE);
        }

        break;

    case STATE_FINISH:

        // マウスカーソルの非表示
        ShowCursor(FALSE);

        // ミッションクリアしているなら
        if (m_bMissionClear == true)
        {
            // カメラ追従
            pCamera->CCamera::SetState(CCamera::STATE_LOCK_ON);
        }
        // ミッション失敗しているなら
        else
        {
            // 失敗時のカメラに
            pCamera->CCamera::SetState(CCamera::STATE_FAILD);
        }

        // 勝敗判定
        Judgment();

        break;

    case STATE_IN_FADE:

        // マウスカーソルの非表示
        ShowCursor(FALSE);

        // カウンタリセット
        m_nCntGameTime = 0;

        // カメラをフェード中に設定
        pCamera->CCamera::SetState(CCamera::STATE_IN_FADE);

        break;

    case STATE_PAUSE:

        // マウスカーソルの表示
        ShowCursor(TRUE);

        // ポーズの更新
        m_pPause->Update();

        break;
    }
}

//=============================================================================
// ミッションの管理
// Author : 後藤慎之助
//=============================================================================
void CGame::ManageMission(void)
{
    // ステージと現在のミッションに応じて、クリア数の上限を決める
    int nMaxClearMission1 = 0;
    int nMaxClearMission2 = 0;
    switch (m_stage)
    {
    case CGame::STAGE_1:

        nMaxClearMission1 = STAGE1_MISSION1_MAX;
        nMaxClearMission2 = STAGE1_MISSION2_MAX;

        break;

    case CGame::STAGE_2:

        nMaxClearMission1 = STAGE2_MISSION1_MAX;
        nMaxClearMission2 = STAGE2_MISSION2_MAX;

        break;

    case CGame::STAGE_FINAL:

        nMaxClearMission1 = STAGE_FINAL_MISSION1_MAX;
        nMaxClearMission2 = STAGE_FINAL_MISSION2_MAX;

        break;
    }

    // 上限を超えていたら、数字を調整
    // また、上限と同じなら、次のミッションにする
    // ミッション1
    if (m_nClearNum_Misson1 > nMaxClearMission1)
    {
        m_nClearNum_Misson1 = nMaxClearMission1;
    }
    else if (m_nClearNum_Misson1 == nMaxClearMission1)
    {
        // 次のミッションへ
        if (m_currentMission == CURRENT_MISSION_1)
        {
            // 現在のミッションを2へ
            m_currentMission = CURRENT_MISSION_2;

            // ミッション2の内容を更新
            m_apMissionUI[1]->SetTexturePlace(2, 2);

            // ネクストミッション表示
            CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_NEXT_MISSION);

            // ステージごとに起きる仕掛け
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_NEXT);
            switch (m_stage)
            {
            case CGame::STAGE_1:

                // BGMを止める
                pSound->Stop(CSound::LABEL_BGM_STAGE_01_NORMAL);

                // BGMを再生
                pSound->Play(CSound::LABEL_BGM_STAGE_01_BOSS);

                // 防衛軍を生成
                CForce::Create(D3DXVECTOR3(-100.0f, 0.0f, 2400.0f), D3DXVECTOR3(0.0f, D3DXToRadian(-90), 0.0f));
                CForce::Create(D3DXVECTOR3(-100.0f, 0.0f, 10400.0f), D3DXVECTOR3(0.0f, D3DXToRadian(-90), 0.0f));
                CForce::Create(D3DXVECTOR3(12900.0f, 0.0f, 10400.0f), D3DXVECTOR3(0.0f, D3DXToRadian(90), 0.0f));
                CForce::Create(D3DXVECTOR3(12900.0f, 0.0f, 2400.0f), D3DXVECTOR3(0.0f, D3DXToRadian(90), 0.0f));

                break;

            case CGame::STAGE_2:

                // BGMを止める
                pSound->Stop(CSound::LABEL_BGM_STAGE_02_NORMAL);

                // BGMを再生
                pSound->Play(CSound::LABEL_BGM_STAGE_02_BOSS);

                // タワーを生成
                CTower::Create(TOWER_SPAWN_POS, DEFAULT_VECTOR);

                break;

            case CGame::STAGE_FINAL:

                // BGMを止める
                pSound->Stop(CSound::LABEL_BGM_STAGE_03_NORMAL);

                // BGMを再生
                pSound->Play(CSound::LABEL_BGM_STAGE_03_BOSS);

                // ワイバーンを生成
                CWyvern::Create(WYVERN_SPAWN_POS, DEFAULT_VECTOR);

                break;
            }
        }
    }

    // ミッション2
    if (m_nClearNum_Misson2 > nMaxClearMission2)
    {
        m_nClearNum_Misson2 = nMaxClearMission2;
    }
    else if (m_nClearNum_Misson2 == nMaxClearMission2)
    {
        // ゲームの状態をFINISHに
        m_state = STATE_FINISH;

        // ミッションクリアのフラグをtrueに
        m_bMissionClear = true;
    }

    // ミッションクリア数を表示
    m_pMission->DispFirst(m_nClearNum_Misson1);
    m_pMission->DispSecond(m_nClearNum_Misson2);
}

//=============================================================================
// 勝敗判定
// Author : 後藤慎之助
//=============================================================================
void CGame::Judgment(void)
{
    // カウンタを加算
    m_nCntGameTime++;

    // 最初の1Fで判断して、UIを出す
    if (m_nCntGameTime == 1)
    {
        // 音取得
        CSound *pSound = CManager::GetSound();

        // ステージのBGMを消す
        switch (m_stage)
        {
        case STAGE_1:
            pSound->Stop(CSound::LABEL_BGM_STAGE_01_NORMAL);
            pSound->Stop(CSound::LABEL_BGM_STAGE_01_BOSS);
            break;
        case STAGE_2:
            pSound->Stop(CSound::LABEL_BGM_STAGE_02_NORMAL);
            pSound->Stop(CSound::LABEL_BGM_STAGE_02_BOSS);
            break;
        case STAGE_FINAL:
            pSound->Stop(CSound::LABEL_BGM_STAGE_03_NORMAL);
            pSound->Stop(CSound::LABEL_BGM_STAGE_03_BOSS);
            break;
        }

        // ミッションクリアしたなら
        if (m_bMissionClear == true)
        {
            // クリア音
            pSound->Play(CSound::LABEL_SE_CLEAR);

            // ミッションクリアのUI
            CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_CLEAR);
        }
        // ミッション失敗したなら
        else
        {
            // 失敗音
            pSound->Play(CSound::LABEL_SE_FAIL);

            // ミッション失敗のUI
            CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_FAILD);
        }
    }

    // 次の画面に行くまでのカウンタ
    if (m_nCntGameTime == COUNT_WAIT_FINISH)
    {
        // カウンタをリセット
        m_nCntGameTime = 0;

        // 状態を、フェード中に
        m_state = STATE_IN_FADE;

        // ミッションに失敗した、または最終ステージを終えたなら
        if (m_bMissionClear == false || m_stage == STAGE_FINAL)
        {
            // スコアをマネージャに渡す
            CManager::SetScore(m_pScore->GetScore());

            // リザルトに移行
            CFade::SetFade(CManager::MODE_RESULT);
        }
        // 次のステージへ
        else
        {
            // ホワイトフェード
            switch (m_stage)
            {
            case STAGE_1:
                CWhiteFade::Create(WHITE_FADE_POS, WHITE_FADE_SIZE, WHITE_FADE_END_TIME, WHITE_FADE_START_COLOR, CWhiteFade::TYPE_STAGE1);
                break;

            case STAGE_2:
                CWhiteFade::Create(WHITE_FADE_POS, WHITE_FADE_SIZE, WHITE_FADE_END_TIME, WHITE_FADE_START_COLOR, CWhiteFade::TYPE_STAGE2);
                break;
            }
        }
    }
}

//=============================================================================
// マップの読み込み
// Author : 後藤慎之助
//=============================================================================
void CGame::LoadMapData(STAGE stage)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    int  nCntLoad = 0;
    char cReedText[128];	// 文字として読み取り用
    char cHeadText[256];	//
    char cDie[128];

    // デバッグ用の変数
    int nType[256];
    int nWidth = 0; // 横
    int nDepth = 0; // 奥行
    D3DXVECTOR3 pos[256];
    D3DXVECTOR3 rot[256];

    // ステージによって、ファイルを開く
    switch (stage)
    {
    case STAGE_1:
        pFile = fopen("data/TXT/stage1.txt", "r");
        break;
    case STAGE_2:
        pFile = fopen("data/TXT/stage2.txt", "r");
        break;
    case STAGE_FINAL:
        pFile = fopen("data/TXT/stage3.txt", "r");
        break;
    }

    // 開けた
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "MODELSET") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);

            if (strcmp(cHeadText, "MAP_SIZ") == 0) 
            {
                sscanf(cReedText, "%s %s %d %d", &cDie, &cDie, &nWidth, &nDepth);
            }
        }

        if (strcmp(cHeadText, "MODELSET") == 0)
        {
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {
                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "\n") == 0)
                {
                }
                else if (strcmp(cHeadText, "END_MODELSET") != 0)
                {
                    if (strcmp(cHeadText, "TYPE") == 0)
                    {
                        sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nType[nCntLoad]);
                    }

                    if (strcmp(cHeadText, "POS") == 0)
                    {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos[nCntLoad].x, &pos[nCntLoad].y, &pos[nCntLoad].z);
                    }

                    if (strcmp(cHeadText, "ROT") == 0)
                    {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot[nCntLoad].x, &rot[nCntLoad].y, &rot[nCntLoad].z);

                        // ブロックを生成
                        CBlock::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), nType[nCntLoad]);

                        nCntLoad++;
                    }
                }
            }

        }

        // フィールド生成
        CField::Create(stage, nWidth, nDepth);

        // ファイル閉
        fclose(pFile);
    }

    // 開けない
    else
    {
        printf("開けれませんでした\n");
    }
}

//=============================================================================
// エネミーの配置読み込み
// Author : 後藤慎之助
//=============================================================================
void CGame::LoadEnemyData(STAGE stage)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    int  nCntLoad = 0;
    char cReedText[128];	// 文字として読み取り用
    char cHeadText[256];	//
    char cDie[128];

    // デバッグ用の変数
    int nType[256];
    D3DXVECTOR3 pos[256];
    D3DXVECTOR3 rot[256];

    // ステージによって、ファイルを開く
    switch (stage)
    {
    case STAGE_1:
        pFile = fopen("data/TXT/enemystage1.txt", "r");
        break;
    case STAGE_2:
        pFile = fopen("data/TXT/enemystage2.txt", "r");
        break;
    case STAGE_FINAL:
        pFile = fopen("data/TXT/enemystage3.txt", "r");
        break;
    }

    // 開けた
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "ENEMYSET") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "ENEMYSET") == 0)
        {
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {
                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "\n") == 0)
                {
                }
                else if (strcmp(cHeadText, "END_ENEMYSET") != 0)
                {
                    if (strcmp(cHeadText, "TYPE") == 0)
                    {
                        sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nType[nCntLoad]);
                    }

                    if (strcmp(cHeadText, "POS") == 0)
                    {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos[nCntLoad].x, &pos[nCntLoad].y, &pos[nCntLoad].z);
                    }

                    if (strcmp(cHeadText, "ROT") == 0)
                    {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot[nCntLoad].x, &rot[nCntLoad].y, &rot[nCntLoad].z);

                        switch (nType[nCntLoad])
                        {
                        case ENEMYSET_WHITE_ANT:
                            // 白アリを生成
                            CAnt::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CAnt::TYPE_WHITE);
                            break;
                        case ENEMYSET_BLACK_ANT:
                            // 巨大黒アリを生成
                            CAnt::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CAnt::TYPE_BLACK);
                            break;
                        case ENEMYSET_YELLOW_SPIDER:
                            // 黄グモを生成
                            CSpider::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CSpider::TYPE_YELLOW);
                            break;
                        case ENEMYSET_BLACK_SPIDER:
                            // 巨大黒グモを生成
                            CSpider::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CSpider::TYPE_BLACK);
                            break;
                        case ENEMYSET_UFO:
                            // UFOを生成
                            CUfo::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)));
                            break;
                        }

                        nCntLoad++;
                    }
                }
            }

        }
        // ファイル閉
        fclose(pFile);
    }

    // 開けない
    else
    {
        printf("開けれませんでした\n");
    }
}
