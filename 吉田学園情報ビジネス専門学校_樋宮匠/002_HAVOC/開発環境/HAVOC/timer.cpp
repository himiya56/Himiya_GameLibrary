//========================================================
//
// 制限時間の処理 [timer.cpp]
// Author : 後藤慎之助
//
//========================================================

//========================================================
// インクルードファイル
//========================================================
#include "timer.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "number.h"

//========================================================
// 静的メンバ変数宣言
//========================================================
bool CTimer::m_bTimeUp = false;

//========================================================
// コンストラクタ
// Author : 後藤慎之助
//========================================================
CTimer::CTimer() :CScene(CScene::OBJTYPE_UI)
{
    //メンバ変数の初期化
    m_nSeconds = 0;
    m_nMinutes = 0;
    m_nCntFrame = 0;

    for (int nCnt = 0; nCnt < MAX_SECONDS_DIGIT; nCnt++)
    {
        m_apSecondsNumber[nCnt] = NULL;
    }

    m_pMinutesNumber = NULL;

    m_bTimeUp = false;
}

//========================================================
// デストラクタ
// Author : 後藤慎之助
//========================================================
CTimer::~CTimer(void)
{

}

//========================================================
// メモリ生成
// Author : 後藤慎之助
//========================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    //変数宣言
    CTimer *pTimer;

    //メモリ生成
    pTimer = new CTimer;

    //初期化処理
    pTimer->Init(pos, size);

    return pTimer;
}

//========================================================
// 初期化
// Author : 後藤慎之助
//========================================================
HRESULT CTimer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 分の時間
    m_nMinutes = MINUTES_TIMER;

    // 秒の時間
    m_nSeconds = START_TIMER_SECONDS;

    // 分の生成
    m_pMinutesNumber = CNumber::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), size, DEFAULT_COLOR, false);

    for (int nCnt = 0; nCnt < MAX_SECONDS_DIGIT; nCnt++)
    {
        // 秒の生成
        m_apSecondsNumber[nCnt] = CNumber::Create(D3DXVECTOR3(pos.x + (nCnt * size.x) + 75.0f, pos.y, 0.0f), size, DEFAULT_COLOR, false);
    }

    return S_OK;
}

//========================================================
// 終了処理
// Author : 後藤慎之助
//========================================================
void CTimer::Uninit(void)
{
    // 秒の破棄
    for (int nCnt = 0; nCnt < MAX_SECONDS_DIGIT; nCnt++)
    {
        if (m_apSecondsNumber[nCnt] != NULL)
        {
            m_apSecondsNumber[nCnt]->Uninit();
            delete m_apSecondsNumber[nCnt];
            m_apSecondsNumber[nCnt] = NULL;
        }

    }

    // 分の破棄
    if (m_pMinutesNumber != NULL)
    {
        m_pMinutesNumber->Uninit();
        delete m_pMinutesNumber;
        m_pMinutesNumber = NULL;
    }

    // オブジェクトの破棄
    Release();
}

//========================================================
// 更新処理
// Author : 後藤慎之助
//========================================================
void CTimer::Update(void)
{
    // ゲーム中なら
    if (CGame::GetState() == CGame::STATE_NORMAL)
    {
        // フレームをカウント
        m_nCntFrame++;

        if (m_nCntFrame % 60 == 0)
        {
            //タイマーを下げる
            m_nSeconds--;
        }

        // 秒が0以下になったら
        if (m_nSeconds <= -1)
        {
            if (m_nMinutes >= 1)
            {
                // 秒を59に戻す
                m_nSeconds = 59;

                // 分を下げる
                m_nMinutes--;
            }
        }
    }

    // ゲームの終了
    if (m_nMinutes <= 0 && m_nSeconds <= -1)
    {
        m_nCntFrame = 0;
        m_nMinutes = 0;
        m_nSeconds = 0;

        m_bTimeUp = true;

        // ゲームが通常なら
        if (CGame::GetState() == CGame::STATE_NORMAL)
        {
            // ゲームの状態を終わりにする
            CGame::SetState(CGame::STATE_FINISH);
        }
    }

    // 数字の変更
    // 変数宣言
    float fRadix = 10.0f; //基数　(スコア表示用)

    for (int nCnt = 0; nCnt < MAX_SECONDS_DIGIT; nCnt++)
    {
        int nScore = (int)powf(fRadix, MAX_SECONDS_DIGIT - (float)nCnt);
        int nScore2 = (int)powf(fRadix, MAX_SECONDS_DIGIT - (float)nCnt - 1);
        int nAnswer = m_nSeconds % nScore / nScore2;

        //秒のテクスチャの座標
        m_apSecondsNumber[nCnt]->SetNumber(nAnswer);

    }

    // 分のテクスチャ座標
    m_pMinutesNumber->SetNumber(m_nMinutes);
}

//========================================================
// 描画処理
// Author : 後藤慎之助
//========================================================
void CTimer::Draw(void)
{
    for (int nCnt = 0; nCnt < MAX_SECONDS_DIGIT; nCnt++)
    {
        // 秒の描画
        m_apSecondsNumber[nCnt]->Draw();
    }

    // 分の描画
    m_pMinutesNumber->Draw();
}