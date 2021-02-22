//===============================================
//
// 白フェード処理 (whiteFade.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "whiteFade.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "effect.h"
#include "sound.h"
#include "scene.h"
#include "ui.h"

//=======================================
// マクロ定義
//=======================================

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CWhiteFade::m_apTexture[CWhiteFade::TYPE_MAX] = {};

//========================================
// 白フェードのコンストラクタ
// Author : 後藤慎之助
//========================================
CWhiteFade::CWhiteFade() :CScene2D(OBJTYPE::OBJTYPE_WHITEFADE)
{
    m_col = DEFAULT_COLOR;
    m_nTimer = 0;
    m_type = CWhiteFade::TYPE_MAX;
    m_bAlphaDown = false;
}

//========================================
// 白フェードのデストラクタ
// Author : 後藤慎之助
//========================================
CWhiteFade::~CWhiteFade()
{

}

//========================================
// 白フェードの初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CWhiteFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// 白フェードの終了処理
// Author : 後藤慎之助
//========================================
void CWhiteFade::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// 白フェードの更新処理
// Author : 後藤慎之助
//========================================
void CWhiteFade::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得

    // 種類に応じた場合分け
    switch (m_type)
    {
        // ステージ1
    case CWhiteFade::TYPE_STAGE1:

        if (m_nTimer > 0)
        {
            // タイマー減算
            m_nTimer--;

            // 透明度を下げるフラグをtrueに(1Fのみ通る)
            if (m_nTimer <= 0)
            {
                m_bAlphaDown = true;

                // ステージを開放
                CScene::ReleaseStage();

                // ステージを2に
                CGame::SetStage(CGame::STAGE_2);
            }
        }

        if (m_bAlphaDown == true)
        {
            // だんだん透明に
            m_col.a -= WHITE_FADE_RATE;

            // 透明とみなす値以下になったら
            if (m_col.a <= WHITE_FADE_CONSIDER_TRANSPARENT)
            {
                // 透明に
                m_col.a = -0.1f;
            }

            // 透明になったら、プレイヤーを動けるようにして、ステージ開始のUI
            if (m_col.a < 0)
            {
                // ゲームの状態を開始に戻す
                CGame::SetState(CGame::STATE_START);
            }
        }
        else
        {
            // だんだん不透明に
            m_col.a += WHITE_FADE_RATE;
        }

        break;

        // ステージ2
    case CWhiteFade::TYPE_STAGE2:

        if (m_nTimer > 0)
        {
            // タイマー減算
            m_nTimer--;

            // 透明度を下げるフラグをtrueに(1Fのみ通る)
            if (m_nTimer <= 0)
            {
                m_bAlphaDown = true;

                // ステージを開放
                CScene::ReleaseStage();

                // ステージをFINALに
                CGame::SetStage(CGame::STAGE_FINAL);
            }
        }

        if (m_bAlphaDown == true)
        {
            // だんだん透明に
            m_col.a -= WHITE_FADE_RATE;

            // 透明とみなす値以下になったら
            if (m_col.a <= WHITE_FADE_CONSIDER_TRANSPARENT)
            {
                // 透明に
                m_col.a = -0.1f;
            }

            // 透明になったら、プレイヤーを動けるようにして、ステージ開始のUI
            if (m_col.a < 0)
            {
                // ゲームの状態を開始に戻す
                CGame::SetState(CGame::STATE_START);
            }
        }
        else
        {
            // だんだん不透明に
            m_col.a += WHITE_FADE_RATE;
        }

        break;
    }

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // 色を変更
    CScene2D::SetColor(m_col);

    // 頂点座標を変更
    SetVertex();

    // 透明なら消滅
    if (m_col.a < 0)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//========================================
// 白フェードの描画処理
// Author : 後藤慎之助
//========================================
void CWhiteFade::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// 白フェードの画像ロード処理
// Author : 後藤慎之助
//========================================
HRESULT CWhiteFade::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/whiteFade000.png", &m_apTexture[CWhiteFade::TYPE_STAGE1]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/whiteFade000.png", &m_apTexture[CWhiteFade::TYPE_STAGE2]);

    return S_OK;
}

//========================================
// 白フェードの画像破棄処理
// Author : 後藤慎之助
//========================================
void CWhiteFade::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CWhiteFade::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// 白フェードの作成
// Author : 後藤慎之助
//================================
CWhiteFade *CWhiteFade::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, D3DXCOLOR col, CWhiteFade::TYPE type)
{
    CWhiteFade *pWhiteFade = NULL;

    // メモリを確保
    pWhiteFade = new CWhiteFade;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pWhiteFade->m_type = type;

    // 初期化
    pWhiteFade->Init(pos, size);

    // メンバ変数を結びつける
    pWhiteFade->m_col = col;
    pWhiteFade->m_nTimer = nTime;

    return pWhiteFade;
}