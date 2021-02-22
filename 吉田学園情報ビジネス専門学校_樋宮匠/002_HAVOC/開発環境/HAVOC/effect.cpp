//===============================================
//
// エフェクト処理 (effect.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "effect.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "game.h"

//=======================================
// マクロ定義
//=======================================

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[CEffect::TYPE_MAX] = {};

//========================================
// エフェクトのコンストラクタ
// Author : 後藤慎之助
//========================================
CEffect::CEffect() :CScene2D(OBJTYPE::OBJTYPE_EFFECT2D)
{
    m_move = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_nLife = 1;    // 0だと消えるため
    m_type = CEffect::TYPE_MAX;
    m_bAlphaDown = false;

    m_fAngle = 0.0f;
    m_fRotSpeed = 0.0f;
}

//========================================
// エフェクトのデストラクタ
// Author : 後藤慎之助
//========================================
CEffect::~CEffect()
{

}

//========================================
// エフェクトの初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// エフェクトの終了処理
// Author : 後藤慎之助
//========================================
void CEffect::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// エフェクトの更新処理
// Author : 後藤慎之助
//========================================
void CEffect::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得
    bool bUse = true;                                  // 使用しているかどうか

    // 種類に応じた場合分け
    switch (m_type)
    {
        // プレイヤー
    case CEffect::TYPE_PLAYER:

        // 移動量減衰
        if (m_move.x != 0.0f || m_move.y != 0.0f)
        {
            m_move *= EFFECT_2D_PLAYER_MOVE_DOWN_RATE;
        }

        // 横の移動量が一定より低くなったら
        if (m_move.x < EFFECT_2D_PLAYER_STOP_MOVE_VALUE)
        {
            m_move.x = 0.0f;
        }

        // 移動量反映
        pos += m_move;

        // プレイヤーの影を生成
        CEffect::Create(pos, EFFECT_2D_PLAYER_SIZE, DEFAULT_VECTOR, 0.0f, 0.0f, DEFAULT_COLOR, CEffect::TYPE_PLAYER_SHADOW);

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // プレイヤーの影
    case CEffect::TYPE_PLAYER_SHADOW:

        // 移動量反映
        pos += m_move;

        // だんだん透明に
        m_col.a -= EFFECT_2D_PLAYER_SHADOW_FADE_RATE;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;
    }

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // 頂点座標を変更
    SetRotVertex(m_fAngle);

    // サイズがなくなる、透明になる、ライフがなくなったら未使用に
    if (size.x < 0 || size.y < 0 || m_col.a < 0 || m_nLife <= 0)
    {
        bUse = false;
    }

    // ゲーム外なら
    if (OutGame2D(&pos, &size) == true)
    {
        // 未使用に
        bUse = false;
    }

    // 使用のフラグがfalseなら消滅
    if (bUse == false)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//========================================
// エフェクトの描画処理
// Author : 後藤慎之助
//========================================
void CEffect::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// エフェクトの画像ロード処理
// Author : 後藤慎之助
//========================================
HRESULT CEffect::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Player_Shadow.png", &m_apTexture[CEffect::TYPE_PLAYER]);          // プレイヤー
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Player_Shadow.png", &m_apTexture[CEffect::TYPE_PLAYER_SHADOW]);   // プレイヤーの影

    return S_OK;
}

//========================================
// エフェクトの画像破棄処理
// Author : 後藤慎之助
//========================================
void CEffect::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CEffect::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// エフェクトの作成
// Author : 後藤慎之助
//================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, float fAngle, float fRotSpeed, D3DXCOLOR col, CEffect::TYPE type)
{
    // 変数宣言
    CEffect *pEffect = NULL;

    // メモリを確保
    pEffect = new CEffect;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pEffect->m_type = type;

    // 初期化
    pEffect->Init(pos, size);

    // メンバ変数を結びつける
    pEffect->m_move = move;
    pEffect->m_fAngle = fAngle;
    pEffect->m_fRotSpeed = fRotSpeed;
    pEffect->m_col = col;

    return pEffect;
}