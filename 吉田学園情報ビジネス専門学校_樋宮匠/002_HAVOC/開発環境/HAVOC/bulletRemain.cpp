//===============================================
//
// 残弾の処理 (bulletRemain.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "bulletRemain.h"
#include "number.h"

//========================================
// マクロ定義
//========================================

//========================================
// 静的メンバ変数宣言
//========================================

//========================================
// 残弾のコンストラクタ
// Author : 後藤慎之助
//========================================
CBulletRemain::CBulletRemain() :CScene(OBJTYPE::OBJTYPE_UI)
{
    for (int nCnt = 0; nCnt < MAX_BULLET_REMAIN; nCnt++)
    {
        m_apNumber[nCnt] = NULL;
    }
}

//========================================
// 残弾のデストラクタ
// Author : 後藤慎之助
//========================================
CBulletRemain::~CBulletRemain()
{

}

//========================================
// 残弾の生成
// Author : 後藤慎之助
//========================================
CBulletRemain* CBulletRemain::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CBulletRemain *pBulletRemain = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pBulletRemain = new CBulletRemain;

    // 初期化
    pBulletRemain->Init(pos, size);

    return pBulletRemain;
}

//========================================
// 残弾の初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CBulletRemain::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 大きい位から、それぞれCreate
    m_apNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_BULLET_REMAIN + 2) / MAX_BULLET_REMAIN / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / MAX_BULLET_REMAIN, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[1] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_BULLET_REMAIN + 0) / MAX_BULLET_REMAIN / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / MAX_BULLET_REMAIN, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[2] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_BULLET_REMAIN - 2) / MAX_BULLET_REMAIN / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / MAX_BULLET_REMAIN, size.y, 0.0f), DEFAULT_COLOR, false);

    return S_OK;
}

//========================================
// 残弾の終了処理
// Author : 後藤慎之助
//========================================
void CBulletRemain::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_BULLET_REMAIN; nCntNumber++)
    {
        // 中身があるなら
        if (m_apNumber[nCntNumber] != NULL)
        {
            // 数字情報の終了処理
            m_apNumber[nCntNumber]->Uninit();

            // 数字情報のメモリの開放
            ReleaseNumber(nCntNumber);
        }
    }

    // オブジェクトの破棄
    Release();
}

//========================================
// 残弾クラスが持っている、数字の開放処理
// Author : 後藤慎之助
//========================================
void CBulletRemain::ReleaseNumber(int nCntNumber)
{
    // メモリの開放
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// 残弾の更新処理
// Author : 後藤慎之助
//========================================
void CBulletRemain::Update(void)
{

}

//========================================
// 残弾の描画処理
// Author : 後藤慎之助
//========================================
void CBulletRemain::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_BULLET_REMAIN; nCntNumber++)
    {
        // 中身があるなら
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// 残弾の設定処理
// Author : 後藤慎之助
//========================================
void CBulletRemain::Disp(const int nNumber)
{
    // 変数宣言
    int nIndex;		        // 指数　(残弾表示用)
    int nRadix = 10;	    // 基数　(残弾表示用)

    // 各桁について計算
    for (nIndex = 0; nIndex < MAX_BULLET_REMAIN; nIndex++)
    {
        int nBulletRemain = (int)powf((float)nRadix, MAX_BULLET_REMAIN - (float)nIndex);
        int nBulletRemain2 = (int)powf((float)nRadix, MAX_BULLET_REMAIN - (float)nIndex - 1);

        // 
        int nAnswer = nNumber % nBulletRemain / nBulletRemain2;

        // 番号を設定
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }
}