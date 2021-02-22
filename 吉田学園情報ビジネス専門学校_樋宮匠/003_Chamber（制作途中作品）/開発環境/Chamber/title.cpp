//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "game.h"
#include "effect.h"

//=============================================================================
// タイトルのコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CTitle::CTitle()
{
    m_nCntTime = 0;
    m_bNextScene = false;
}

//=============================================================================
// タイトルのデストラクタ
// Author : 後藤慎之助
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CTitle::Init(void)
{
    // マウスカーソルの表示
    ShowCursor(TRUE);

    // 背景を生成
    CUI::Create(TITLE_POS, TITLE_SIZE, DEFAULT_COLOR, CUI::TYPE_TITLE_BG);

    // ロゴを生成
    CUI::Create(TITLE_LOGO_FIRST_POS, TITLE_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_TITLE_LOGO);

    // バージョンを生成
    CUI::Create(VERSION_POS, VERSION_SIZE, DEFAULT_COLOR, CUI::TYPE_VERSION);

    // プレススタート
    CUI::Create(PRESS_START_POS, PRESS_START_SIZE, DEFAULT_COLOR, CUI::TYPE_PRESS_START);

    // プレイヤーのイラストを生成
    CEffect::Create(EFFECT_2D_PLAYER_FIRST_POS, EFFECT_2D_PLAYER_SIZE, EFFECT_2D_PLAYER_FIRST_MOVE_VALUE, 0.0f, 0.0f, DEFAULT_COLOR, CEffect::TYPE_PLAYER);

    // BGMを再生
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_BGM_TITLE);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CTitle::Uninit(void)
{

}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CTitle::Update(void)
{
    // タイトルの時間をカウント
    m_nCntTime++;

    // タイトルの最大時間
    if (m_nCntTime > TITLE_MAX_TIME)
    {
        m_nCntTime = TITLE_MAX_TIME;
    }

    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 遷移のフラグがtrueなら
    if (m_bNextScene == true)
    {
        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // フェードしていないなら
            if (CFade::GetFade() == CFade::FADE_NONE)
            {
                // 決定音再生
                CSound *pSound = CManager::GetSound();
                pSound->Play(CSound::LABEL_SE_SYSTEM_MODE_CHANGE);

                // 遊び方モードに移行
                CFade::SetFade(CManager::MODE_MANUAL);
            }
        }
    }
    // 遷移のフラグがfalseなら
    else
    {
        // 一定時間経過で遷移可能に
        if (m_nCntTime >= TITLE_NEXT_SCENE)
        {
            m_bNextScene = true;
        }
        // 一定時間経過していなくても
        else
        {
            // エンター、またはスタートボタンを押したら
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
            {
                // 遷移可能に
                m_bNextScene = true;
            }
        }
    }
}
