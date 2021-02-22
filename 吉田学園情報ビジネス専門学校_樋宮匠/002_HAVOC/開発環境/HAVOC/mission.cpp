//===============================================
//
// ミッションの処理 (mission.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "mission.h"
#include "number.h"

//========================================
// マクロ定義
//========================================

// ミッション同士の間隔
#define MISSIONS_SPACE 25.0f

//========================================
// 静的メンバ変数宣言
//========================================

//========================================
// ミッションのコンストラクタ
// Author : 後藤慎之助
//========================================
CMission::CMission() :CScene(OBJTYPE::OBJTYPE_UI)
{
    for (int nCnt = 0; nCnt < MAX_MISSION_NUMBER; nCnt++)
    {
        m_apFirstNumber[nCnt] = NULL;
    }

    for (int nCnt = 0; nCnt < MAX_MISSION_NUMBER; nCnt++)
    {
        m_apSecondNumber[nCnt] = NULL;
    }
}

//========================================
// ミッションのデストラクタ
// Author : 後藤慎之助
//========================================
CMission::~CMission()
{

}

//========================================
// ミッションの生成
// Author : 後藤慎之助
//========================================
CMission* CMission::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CMission *pMission = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pMission = new CMission;

    // 初期化
    pMission->Init(pos, size);

    return pMission;
}

//========================================
// ミッションの初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CMission::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 大きい位から、それぞれCreate
    m_apFirstNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_MISSION_NUMBER + 1) / MAX_MISSION_NUMBER / 2, pos.y - MISSIONS_SPACE, 0.0f),
        D3DXVECTOR3(size.x / MAX_MISSION_NUMBER, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apFirstNumber[1] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_MISSION_NUMBER - 1) / MAX_MISSION_NUMBER / 2, pos.y - MISSIONS_SPACE, 0.0f),
        D3DXVECTOR3(size.x / MAX_MISSION_NUMBER, size.y, 0.0f), DEFAULT_COLOR, false);

    m_apSecondNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_MISSION_NUMBER + 1) / MAX_MISSION_NUMBER / 2, pos.y + MISSIONS_SPACE, 0.0f),
        D3DXVECTOR3(size.x / MAX_MISSION_NUMBER, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apSecondNumber[1] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_MISSION_NUMBER - 1) / MAX_MISSION_NUMBER / 2, pos.y + MISSIONS_SPACE, 0.0f),
        D3DXVECTOR3(size.x / MAX_MISSION_NUMBER, size.y, 0.0f), DEFAULT_COLOR, false);

    return S_OK;
}

//========================================
// ミッションの終了処理
// Author : 後藤慎之助
//========================================
void CMission::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_MISSION_NUMBER; nCntNumber++)
    {
        // 中身があるなら
        if (m_apFirstNumber[nCntNumber] != NULL)
        {
            // 数字情報の終了処理
            m_apFirstNumber[nCntNumber]->Uninit();

            // メモリの開放
            delete m_apFirstNumber[nCntNumber];
            m_apFirstNumber[nCntNumber] = NULL;
        }
    }

    for (int nCntNumber = 0; nCntNumber < MAX_MISSION_NUMBER; nCntNumber++)
    {
        // 中身があるなら
        if (m_apSecondNumber[nCntNumber] != NULL)
        {
            // 数字情報の終了処理
            m_apSecondNumber[nCntNumber]->Uninit();

            // メモリの開放
            delete m_apSecondNumber[nCntNumber];
            m_apSecondNumber[nCntNumber] = NULL;
        }
    }

    // オブジェクトの破棄
    Release();
}

//========================================
// ミッションの更新処理
// Author : 後藤慎之助
//========================================
void CMission::Update(void)
{

}

//========================================
// ミッションの描画処理
// Author : 後藤慎之助
//========================================
void CMission::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_MISSION_NUMBER; nCntNumber++)
    {
        // 中身があるなら
        if (m_apFirstNumber[nCntNumber] != NULL)
        {
            // 数字情報の描画
            m_apFirstNumber[nCntNumber]->Draw();
        }
    }

    for (int nCntNumber = 0; nCntNumber < MAX_MISSION_NUMBER; nCntNumber++)
    {
        // 中身があるなら
        if (m_apSecondNumber[nCntNumber] != NULL)
        {
            // 数字情報の終了処理
            m_apSecondNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// 1つ目のミッションの数字設定処理
// Author : 後藤慎之助
//========================================
void CMission::DispFirst(const int nNumber)
{
    // 変数宣言
    int nIndex;		                // 指数　(ミッション表示用)
    int nRadix = 10;	            // 基数　(ミッション表示用)
    bool bCheckStart = false;       // 数字が始まるまでのチェック(左から右へと数字を見るため)(例:05などをなくす)

    // 各桁について計算
    for (nIndex = 0; nIndex < MAX_MISSION_NUMBER; nIndex++)
    {
        int nMission = (int)powf((float)nRadix, MAX_MISSION_NUMBER - (float)nIndex);
        int nMission2 = (int)powf((float)nRadix, MAX_MISSION_NUMBER - (float)nIndex - 1);

        // 計算結果
        int nAnswer = nNumber % nMission / nMission2;

        // 0以上の数字が入ると
        if (nAnswer > 0)
        {
            // 数字開始のチェックをtrue
            bCheckStart = true;
        }

        // 設定したい数字が0かつ、最大桁数目の数字を計算しているなら
        if (nNumber == 0 && nIndex == MAX_MISSION_NUMBER - 1)
        {
            // 数字開始のチェックをtrue
            bCheckStart = true;
        }

        // 数字が開始しているなら
        if (bCheckStart == true)
        {
            // 番号を設定
            m_apFirstNumber[nIndex]->SetAlpha(1.0f);
            m_apFirstNumber[nIndex]->SetNumber(nAnswer);
        }
        else
        {
            // 開始していないなら、透明に
            m_apFirstNumber[nIndex]->SetAlpha(0.0f);
            m_apFirstNumber[nIndex]->SetNumber(nAnswer);
        }
    }
}

//========================================
// 2つ目のミッションの数字設定処理
// Author : 後藤慎之助
//========================================
void CMission::DispSecond(const int nNumber)
{
    // 変数宣言
    int nIndex;		                // 指数　(ミッション表示用)
    int nRadix = 10;	            // 基数　(ミッション表示用)
    bool bCheckStart = false;       // 数字が始まるまでのチェック(左から右へと数字を見るため)(例:05などをなくす)

    // 各桁について計算
    for (nIndex = 0; nIndex < MAX_MISSION_NUMBER; nIndex++)
    {
        int nMission = (int)powf((float)nRadix, MAX_MISSION_NUMBER - (float)nIndex);
        int nMission2 = (int)powf((float)nRadix, MAX_MISSION_NUMBER - (float)nIndex - 1);

        // 計算結果
        int nAnswer = nNumber % nMission / nMission2;

        // 0以上の数字が入ると
        if (nAnswer > 0)
        {
            // 数字開始のチェックをtrue
            bCheckStart = true;
        }

        // 設定したい数字が0かつ、最大桁数目の数字を計算しているなら
        if (nNumber == 0 && nIndex == MAX_MISSION_NUMBER - 1)
        {
            // 数字開始のチェックをtrue
            bCheckStart = true;
        }

        // 数字が開始しているなら
        if (bCheckStart == true)
        {
            // 番号を設定
            m_apSecondNumber[nIndex]->SetAlpha(1.0f);
            m_apSecondNumber[nIndex]->SetNumber(nAnswer);
        }
        else
        {
            // 開始していないなら、透明に
            m_apSecondNumber[nIndex]->SetAlpha(0.0f);
            m_apSecondNumber[nIndex]->SetNumber(nAnswer);
        }
    }
}