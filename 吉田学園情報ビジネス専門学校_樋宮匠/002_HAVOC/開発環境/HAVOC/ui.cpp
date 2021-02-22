//===============================================
//
// UI処理 (ui.cpp)
// Author : 後藤慎之助(内部処理) / 齋藤大行(配置、サイズ調整)
//
//===============================================

//========================
// インクルードファイル
//========================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "character.h"
#include "player.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CUI::m_apTexture[CUI::TYPE_MAX] = {};

//========================================
// UIのコンストラクタ
// Author : 後藤慎之助
//========================================
CUI::CUI() :CScene2D(OBJTYPE::OBJTYPE_UI)
{
    m_move = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_type = CUI::TYPE_NONE;
    m_fAngle = 0.0f;
    m_bUse = true;
    m_bBlinking = false;
    m_nCntTime = 0;
    m_bFadeOut = false;
}

//========================================
// UIのデストラクタ
// Author : 後藤慎之助
//========================================
CUI::~CUI()
{

}

//========================================
// UIの初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// UIの終了処理
// Author : 後藤慎之助
//========================================
void CUI::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// UIの更新処理
// Author : 後藤慎之助
//========================================
void CUI::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得
    int nPatternAnim = 0;                              // パターンアニメのカウンタを利用するとき

    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    switch (m_type)
    {
    case TYPE_TITLE_LOGO:

        // 流れ落ちる
        pos.y += TITLE_LOGO_FLOW_DOWN_SPEED;

        // 一定の値で止めておく
        if (pos.y >= TITLE_LOGO_TARGET_POS_Y)
        {
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // タイトルロゴをすぐに下す
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        break;

    case TYPE_PRESS_START:

        // タイトルの時間をカウント
        m_nCntTime++;

        // タイトルの最大時間
        if (m_nCntTime > TITLE_MAX_TIME)
        {
            m_nCntTime = TITLE_MAX_TIME;
        }

        // 色を変える
        if (m_bFadeOut == true)
        {
            m_col.r -= PRESS_START_FADE_RATE;
            m_col.g -= PRESS_START_FADE_RATE;
            m_col.b -= PRESS_START_FADE_RATE;
        }
        else
        {
            m_col.r += PRESS_START_FADE_RATE;
            m_col.g += PRESS_START_FADE_RATE;
            m_col.b += PRESS_START_FADE_RATE;
        }
        if (m_col.r <= PRESS_START_FADE_OUT_MIN)
        {
            m_bFadeOut = false;
        }
        else if (m_col.r >= 1.0f)
        {
            m_bFadeOut = true;
        }

        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // 一定時間経過で点滅可能に
            if (m_nCntTime >= TITLE_NEXT_SCENE)
            {
                // 点滅のフラグをtrueに
                m_bBlinking = true;
            }
            // それ以外は、カウンタを遷移可能まで一気に進める
            else
            {
                m_nCntTime = TITLE_NEXT_SCENE;
            }
        }

        // 点滅するなら
        if (m_bBlinking == true)
        {
            // カウンターを進めて、パターンを切り替える
            nPatternAnim = CountAnimation(2, 2);

            // 色を2パターンで切り替える
            if (nPatternAnim == 1)
            {
                m_col.r = PRESS_START_FADE_OUT_MIN;
                m_col.g = PRESS_START_FADE_OUT_MIN;
                m_col.b = PRESS_START_FADE_OUT_MIN;
            }
            else
            {
                m_col.r = 1.0f;
                m_col.g = 1.0f;
                m_col.b = 1.0f;
            }
        }

        break;

    case TYPE_LOCK_ON:

        // 戦闘時以外、消す
        if (CGame::GetState() == CGame::STATE_NORMAL)
        {
            m_col.a = 1.0f;
        }
        else
        {
            m_col.a = 0.0f;
        }

        break;

    case TYPE_RELOAD:

        // リロード中なら
        if (CGame::GetPlayer()->GetReloading() == true)
        {
            SetTexturePlace(2, 2);
        }
        else
        {
            // 残弾減少中なら
            if (CGame::GetPlayer()->GetRemainBullets() <= PLAYER_REMAIN_BULLETS_WARNING)
            {
                SetTexturePlace(1, 2);
            }
            else
            {
                // 残弾も減少していないなら、表示しない
                SetTexturePlace(0, 0);
            }
        }

        break;

    case TYPE_MISSION_START:

        // 拡大
        size = Extend(size);

        break;

    case TYPE_NEXT_MISSION:

        // 拡大
        size = Extend(size);

        break;

    case TYPE_MISSION_CLEAR:

        // 拡大
        size = Extend(size);

        break;

    case TYPE_MISSION_FAILD:

        // 拡大
        size = Extend(size);

        break;

    case TYPE_HP_GAUGE:

        // 左から右に伸びるゲージ
        CScene2D::SetLeftToRightGauge((float)PLAYER_MAX_LIFE, (float)(CGame::GetPlayer()->GetLife()));

        break;

    case TYPE_LEFTHAND_GAUGE:

        // 左から右に伸びるゲージ
        CScene2D::SetLeftToRightGauge((float)PLAYER_LEFT_HAND_ORBS_MAX, (float)(CGame::GetPlayer()->GetLeftHandOrbs()));

        break;

    case TYPE_LIMIT:

        break;

    case TYPE_RESULT_BG:
        // テクスチャの頂点座標を変更
        SetFlowingAnimation(1, 400, true, CScene2D::DIRECT_RIGHT_DOWN);
        break;
    }

    // 色を反映
    SetColor(m_col);

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // ゲージ系以外は、通常の頂点座標
    if (m_type != TYPE_HP_GAUGE && m_type != TYPE_LEFTHAND_GAUGE)
    {
        SetVertex();
    }

    // 使用フラグがfalseなら消す
    if (m_bUse == false)
    {
        Uninit();
    }
}

//========================================
// UIの描画処理
// Author : 後藤慎之助
//========================================
void CUI::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// UIの画像ロード処理
// Author : 齋藤大行
//========================================
HRESULT CUI::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ver.png", &m_apTexture[CUI::TYPE_VERSION]);	                        // バージョン
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title000.png", &m_apTexture[CUI::TYPE_TITLE_BG]);	                    // タイトルの背景
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui014.png", &m_apTexture[CUI::TYPE_TITLE_LOGO]);	                    // タイトルロゴ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui015.png", &m_apTexture[CUI::TYPE_PRESS_START]);	                    // スタートボタンを押してね
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LockOn.png", &m_apTexture[CUI::TYPE_LOCK_ON]);	                        // ロックオン
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/time001.png", &m_apTexture[CUI::TYPE_LIMIT]);                          // 制限時間
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui001.png", &m_apTexture[CUI::TYPE_MISSION_START]);	                // ミッション開始
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui002.png", &m_apTexture[CUI::TYPE_NEXT_MISSION]);	                    // 次のミッション
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui003.png", &m_apTexture[CUI::TYPE_MISSION_CLEAR]);                    // ミッションクリア
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui004.png", &m_apTexture[CUI::TYPE_MISSION_FAILD]);                    // ミッション失敗
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Result_BgUI.png", &m_apTexture[CUI::TYPE_RESULT_BG]);                  // リザルト背景
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Result_ScreenUI.png", &m_apTexture[CUI::TYPE_RESULT_SCREEN]);          // リザルト表示
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/manual.jpg", &m_apTexture[CUI::TYPE_MANUAL]);	                        // 遊び方
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION.png", &m_apTexture[CUI::TYPE_MISSION_LOGO]);	                // ミッションロゴ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_1.png", &m_apTexture[CUI::TYPE_MISSION_1]);	                // ミッション1
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_2.png", &m_apTexture[CUI::TYPE_MISSION_2]);	                // ミッション2
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_3.png", &m_apTexture[CUI::TYPE_MISSION_3]);                    // ミッション3
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_4.png", &m_apTexture[CUI::TYPE_MISSION_4]);                    // ミッション4
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_5.png", &m_apTexture[CUI::TYPE_MISSION_5]);	                // ミッション5
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MISSION_6.png", &m_apTexture[CUI::TYPE_MISSION_6]);	                // ミッション6
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/HP.png", &m_apTexture[CUI::TYPE_HP_LOGO]);								// スタミナロゴ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Righthand.png", &m_apTexture[CUI::TYPE_RIGHTHAND_LOGO]);				// 右手ロゴ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lefthandpartition.png", &m_apTexture[CUI::TYPE_LEFTHAND_PARTITION]);	// 左手ゲージ仕切り
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lefthandframe.png", &m_apTexture[CUI::TYPE_LEFTHAND_FRAME]);			// 左手スタミナ(枠)
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lefthandgauge.png", &m_apTexture[CUI::TYPE_LEFTHAND_GAUGE]);			// 左手ゲージ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lefthandgaugeframe.png", &m_apTexture[CUI::TYPE_LEFTHAND_GAUGEFRAME]); // 左手ゲージフレーム
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lefthandgaugebg.png", &m_apTexture[CUI::TYPE_LEFTHAND_GAUGEBG]);       // 左手ゲージ背景
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/BulletUI.png", &m_apTexture[CUI::TYPE_RIGHTHAND_BULLET_UI]);			// 右手残弾
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/minimap.png", &m_apTexture[CUI::TYPE_MINIMAP]);						// マップ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Score.png", &m_apTexture[CUI::TYPE_SCORE]);							// スコア
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/HPframe.png", &m_apTexture[CUI::TYPE_HP_FRAME]);						// HP(枠)
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/HPGauge.png", &m_apTexture[CUI::TYPE_HP_GAUGE]);						// HPゲージ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui000.png", &m_apTexture[CUI::TYPE_RELOAD]);	                        // リロード

    return S_OK;
}

//========================================
// UIの画像破棄処理
// Author : 後藤慎之助
//========================================
void CUI::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CUI::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// UIの作成
// Author : 後藤慎之助
//================================
CUI *CUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CUI::TYPE type)
{
    CUI *pUI = NULL;

    // メモリを確保
    pUI = new CUI;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pUI->m_type = type;

    // 初期化
    pUI->Init(pos, size);

    // メンバ変数を結びつける
    pUI->m_col = col;

    return pUI;
}

//================================
// 拡大するUI
// Author : 後藤慎之助
//================================
D3DXVECTOR3 CUI::Extend(D3DXVECTOR3 size)
{
    // 変数宣言
    D3DXVECTOR3 returnSize = size;  // 返すサイズ
    int nCnt = 0;                   // カウンタ

    // 横サイズ拡大
    returnSize.x += UI_EXTEND_MISSION_EXTEND_RATE_X;

    // 縦サイズ拡大
    returnSize.y += UI_EXTEND_MISSION_EXTEND_RATE_Y;

    // 横サイズの制限
    if (returnSize.x >= UI_EXTEND_MISSION_MAX_SIZE_X)
    {
        returnSize.x = UI_EXTEND_MISSION_MAX_SIZE_X;
    }

    // 縦サイズの制限
    if (returnSize.y >= UI_EXTEND_MISSION_MAX_SIZE_Y)
    {
        returnSize.y = UI_EXTEND_MISSION_MAX_SIZE_Y;
    }

    // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
    nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

    // フェードアウトを始めるカウントを超えたら、α値を下げていく
    if (nCnt >= UI_EXTEND_MISSION_FADE_OUT_COUNT)
    {
        m_col.a -= UI_EXTEND_MISSION_FADE_OUT_RATE;
    }

    // 透明度が0を下回ったら、消す
    if (m_col.a <= 0.0f)
    {
        m_bUse = false;
    }

    return returnSize;
}