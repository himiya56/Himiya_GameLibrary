//===============================================
//
// エフェクト3D処理 (effect3d.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "effect3d.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "character.h"
#include "game.h"
#include "library.h"
#include "sound.h"
#include "bullet.h"

//========================================
// 静的メンバ変数宣言
//========================================
LPDIRECT3DTEXTURE9 CEffect3D::m_apTexture[TYPE_MAX] = {};

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CEffect3D::CEffect3D(CScene::OBJTYPE objtype) :CBillboard(objtype)
{
    m_move = DEFAULT_VECTOR;
    m_fSpeed = 0.0f;
    m_col = DEFAULT_COLOR;
    m_type = TYPE_PURPLE_FIRE;
    m_fFadeOutRate = 0.0f;
    m_bStartFadeOut = false;
    m_nCntTime = 0;
    m_fRotAngle = 0.0f;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CEffect3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ビルボードの初期化
    CBillboard::Init(pos, size);

    // テクスチャをバインド
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::Uninit(void)
{
    CBillboard::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::Update(void)
{
    // 位置と大きさを取得
    D3DXVECTOR3 pos = GetPos();
    D3DXVECTOR3 size = GetSize();

    // 使用するフラグ
    bool bUse = true;

    // アニメーションが一周したかどうか
    bool bOneRound = false;

    switch (m_type)
    {
    case TYPE_PURPLE_FIRE:

        // だんだん透明に
        m_col.a -= m_fFadeOutRate;

        // 紫に近づけていく
        m_col.r -= EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_R;
        m_col.g -= EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_G;

        break;

    case TYPE_PARTICLE:

        // だんだん縮小
        size.x -= m_fFadeOutRate;
        size.y -= m_fFadeOutRate;

        // 大きさを反映
        SetSize(size);

        break;

    case TYPE_RELOADING:

        // だんだん縮小
        size.x -= m_fFadeOutRate;
        size.y -= m_fFadeOutRate;

        // 右手に移動
        GetMoveToRightHand(pos);

        // 大きさを反映
        SetSize(size);

        break;

    case TYPE_WAVE:

        // だんだん拡大
        size.x += EFFECT_WAVE_EXTEND_SIZE;
        size.y += EFFECT_WAVE_EXTEND_SIZE;

        // だんだん透明に
        m_col.a -= m_fFadeOutRate;

        // 大きさを反映
        SetSize(size);

        break;

    case TYPE_GUN_MARKS:

        // だんだん透明に
        m_col.a -= m_fFadeOutRate;

        // 赤に近づけていく
        m_col.g -= EFFECT_YELLOW_FIRE_TO_RED_RATE;

        break;


    case TYPE_CRACK1:
        // だんだん透明に
        m_col.a -= m_fFadeOutRate;
        break;

    case TYPE_CRACK2:
        // だんだん透明に
        m_col.a -= m_fFadeOutRate;
        break;

    case TYPE_CRACK3:
        // だんだん透明に
        m_col.a -= m_fFadeOutRate;
        break;

    case TYPE_CRACK4:
        // だんだん透明に
        m_col.a -= m_fFadeOutRate;
        break;

    case TYPE_CHARGE:

        // 左手に吸収
        GetAngleToLeftHand(pos);

        // 紫に近づけていく
        m_col.r -= EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_R;
        m_col.g -= EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_G;

        // フェードアウト開始のフラグがtrueなら
        if (m_bStartFadeOut == true)
        {
            // だんだん透明に
            m_col.a -= m_fFadeOutRate;
        }
        else
        {
            // 重さ回避
            //// トレイルエフェクト発生
            //CEffect3D::Create(pos, size / 2, m_move * EFFECT_CHARGE_TRAIL_MOVE_CHANGE_RATE, EFFECT_CHARGE_TRAIL_COLOR,
            //    DEFAULT_VECTOR, EFFECT_CHARGE_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }

        break;

    case TYPE_SMOKE:

        // だんだん透明に
        m_col.a -= m_fFadeOutRate;

        // 黒に近づけていく
        m_col.r -= EFFECT_SMOKE_COLOR_TO_BLACK_RATE;
        m_col.g -= EFFECT_SMOKE_COLOR_TO_BLACK_RATE;
        m_col.b -= EFFECT_SMOKE_COLOR_TO_BLACK_RATE;

        // アニメーションのカウンタを止めるフラグがfalseなら
        if (m_bStartFadeOut == false)
        {
            // カウントアップ
            m_nCntTime++;

            // テクスチャ座標を更新
            if (m_nCntTime == EFFECT_SMOKE_UPDATE_PATTERN_SPEED)
            {
                bOneRound = SetUpdateTexLeftToRight(EFFECT_SMOKE_MAX_PATTERN);

                // カウンタリセット
                m_nCntTime = 0;

                // 一周したら、アニメーションのカウンタを止める
                if (bOneRound == true)
                {
                    m_bStartFadeOut = true;
                }
            }
        }

        break;

    case TYPE_ACID:

        // だんだん拡大
        size.x += EFFECT_ACID_EXTEND_SIZE;
        size.y += EFFECT_ACID_EXTEND_SIZE;

        // だんだん透明に
        m_col.a -= m_fFadeOutRate;

        // だんだん緑に
        m_col.r -= EFFECT_ACID_TO_GREEN_RATE_R;

        // 大きさを反映
        SetSize(size);

        break;

    case TYPE_ACID_ATTACK:

        // だんだん拡大
        size.x += EFFECT_ACID_EXTEND_SIZE;
        size.y += EFFECT_ACID_EXTEND_SIZE;

        // だんだん透明に
        m_col.a -= m_fFadeOutRate;

        // だんだん紫に
        m_col.r -= EFFECT_ACID_ATTACK_TO_PURPLE_RATE;

        // 大きさを反映
        SetSize(size);

        break;

    case TYPE_WAVE_ATTACK:

        // だんだん拡大
        size.x += EFFECT_BLUE_FIRE_EXTEND_RATE;
        size.y += EFFECT_BLUE_FIRE_EXTEND_RATE;

        // だんだん透明に
        m_col.a -= m_fFadeOutRate;

        // 青に近づけていく
        m_col.g -= EFFECT_WHITE_FIRE_TO_BLUE_RATE;
        m_col.r -= EFFECT_WHITE_FIRE_TO_BLUE_RATE;

        // 大きさを反映
        SetSize(size);

        break;

    case TYPE_BURST_CENTER:

        // だんだん拡大
        size.x += EFFECT_BURST_CENTER_EXTEND_SIZE;
        size.y += EFFECT_BURST_CENTER_EXTEND_SIZE;

        // だんだん透明に
        m_col.a -= m_fFadeOutRate;

        // だんだん黄色に
        m_col.r += EFFECT_BURST_CENTER_TO_YELLOW_RATE_R;

        // 黄色に達したら
        if (m_col.r >= 1.0f)
        {
            // だんだん赤色に
            m_col.g -= EFFECT_BURST_CENTER_TO_RED_RATE_G;
        }

        // 爆砕の周辺を生成
        SetBurstAround(pos, size);

        // 一定カウンタで爆発する
        m_nCntTime++;
        if (m_nCntTime == EFFECT_BURST_CENTER_EXPLOSION_FRAME)
        {
            CBullet::Create(pos, D3DXVECTOR3(BULLET_EXPLOSION_COLLISION_SIZE_LV3, BULLET_EXPLOSION_COLLISION_SIZE_LV3, 
                BULLET_EXPLOSION_COLLISION_SIZE_LV3), DEFAULT_VECTOR,
                CBullet::TYPE_EXPLOSION, CBullet::TRIBE_ENEMY);
        }

        // 大きさを反映
        SetSize(size);

        break;

    case TYPE_BURST_AROUND:

        // だんだん拡大
        size.x += EFFECT_BURST_AROUND_EXTEND_SIZE;
        size.y += EFFECT_BURST_AROUND_EXTEND_SIZE;

        // だんだん透明に
        m_col.a -= m_fFadeOutRate;

        // 大きさを反映
        SetSize(size);

        break;

    case TYPE_SPRAY:

        // だんだん透明に
        m_col.a -= m_fFadeOutRate;

        // だんだん回転させる
        m_fRotAngle += m_fSpeed;

        // 移動量を制御
        if (m_move.x != 0.0f || m_move.z != 0.0f)
        {
            m_move.x *= EFFECT_SPRAY_CONTROL;
            m_move.z *= EFFECT_SPRAY_CONTROL;
        }
        m_move.y -= GRAVITY;

        if (m_move.y < -2.5f)
        {
            m_move.y = -2.5f;
        }

        // 大きさを反映
        SetSize(size);

        break;

    case TYPE_EXPLOSION:

        // だんだん透明に
        m_col.a -= EFFECT_EX_DOWN_ALPHA_RATE;

        // だんだん縮小
        size.x -= m_fFadeOutRate;
        size.y -= m_fFadeOutRate;

        // だんだん赤に
        m_col.g -= EFFECT_EX_TO_RED_RATE;

        // 一定カウンタで、移動量変化
        m_nCntTime++;
        if (m_nCntTime > EFFECT_EX_CHANGE_MOVE_START_COUNT)
        {
            pos.y += EFFECT_EX_CHANGE_MOVE_RATE;
            m_move.y = 0.0f;
        }

        // 回転の速度を角度に足す
        m_fRotAngle += m_fSpeed;

        // 大きさを反映
        SetSize(size);

        break;

    case TYPE_BLUE_FIRE:

        // だんだん透明に
        m_col.a -= EFFECT_EX_DOWN_ALPHA_RATE;

        // だんだん縮小
        size.x -= m_fFadeOutRate;
        size.y -= m_fFadeOutRate;

        // 青に近づけていく
        m_col.g -= EFFECT_WHITE_FIRE_TO_BLUE_RATE;
        m_col.r -= EFFECT_WHITE_FIRE_TO_BLUE_RATE;

        // 一定カウンタで、移動量変化
        m_nCntTime++;
        if (m_nCntTime > EFFECT_EX_CHANGE_MOVE_START_COUNT)
        {
            pos.y += EFFECT_EX_CHANGE_MOVE_RATE;
            m_move.y = 0.0f;
        }

        // 回転の速度を角度に足す
        m_fRotAngle += m_fSpeed;

        // 大きさを反映
        SetSize(size);

        break;
    }

    // 移動
    pos += m_move;

    // 位置、色を反映
    SetPos(pos);
    SetCol(m_col);

    // ビルボードの更新を、タイプごとで切り替える
    if (m_type == TYPE_SPRAY || m_type == TYPE_EXPLOSION || m_type == TYPE_BLUE_FIRE)
    {
        // ビルボードを更新
        CBillboard::Update(m_fRotAngle);
    }
    else
    {
        // ビルボードを更新
        CBillboard::Update();
    }

    // 透明、大きさがないなら使用フラグをfalseに
    if (m_col.a <= 0.0f || size.x < 0.0f || size.y < 0.0f)
    {
        bUse = false;
    }

    // 使用フラグがfalseなら、消す
    if (bUse == false)
    {
        Uninit();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::Draw(void)
{
    switch (m_type)
    {
    case TYPE_PURPLE_FIRE:
        CBillboard::SetAdditiveSynthesis();
        break;
    case TYPE_PARTICLE:
        CBillboard::SetAdditiveSynthesis();
        break;
    case TYPE_WAVE:
        CBillboard::SetAdditiveSynthesis();
        break;
    case TYPE_GUN_MARKS:
        CBillboard::SetAdditiveSynthesis();
        break;
    case TYPE_CHARGE:
        CBillboard::SetAdditiveSynthesis();
        break;
    case TYPE_ACID:
        CBillboard::SetAdditiveSynthesis();
        CBillboard::SetZBufferNone();
        break;
    case TYPE_ACID_ATTACK:
        CBillboard::SetAdditiveSynthesis();
        CBillboard::SetZBufferNone();
        break;
    case TYPE_WAVE_ATTACK:
        CBillboard::SetAdditiveSynthesis();
        CBillboard::SetZBufferNone();
        break;
    case TYPE_BURST_CENTER:
        CBillboard::SetAdditiveSynthesis();
        CBillboard::SetZBufferNone();
        break;
    case TYPE_BURST_AROUND:
        CBillboard::SetAdditiveSynthesis();
        CBillboard::SetZBufferNone();
        break;
    case TYPE_RELOADING:
        CBillboard::SetAdditiveSynthesis();
        break;
    case TYPE_EXPLOSION:
        CBillboard::SetAdditiveSynthesis();
        break;
    case TYPE_BLUE_FIRE:
        CBillboard::SetAdditiveSynthesis();
        break;
    }

    CBillboard::Draw();
}

//=============================================================================
// データ読み込み処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CEffect3D::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle001.png", &m_apTexture[TYPE_PURPLE_FIRE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle001.png", &m_apTexture[TYPE_PARTICLE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect101.jpg", &m_apTexture[TYPE_WAVE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[TYPE_GUN_MARKS]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle001.png", &m_apTexture[TYPE_CHARGE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion001.png", &m_apTexture[TYPE_SMOKE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[TYPE_ACID]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.png", &m_apTexture[TYPE_SPRAY]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[TYPE_ACID_ATTACK]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[TYPE_WAVE_ATTACK]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[TYPE_BURST_CENTER]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[TYPE_BURST_AROUND]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle001.png", &m_apTexture[TYPE_RELOADING]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.png", &m_apTexture[TYPE_CRACK1]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect101.png", &m_apTexture[TYPE_CRACK2]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect102.png", &m_apTexture[TYPE_CRACK3]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect103.png", &m_apTexture[TYPE_CRACK4]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/smoke.png", &m_apTexture[TYPE_EXPLOSION]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/smoke.png", &m_apTexture[TYPE_BLUE_FIRE]);

    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::Unload(void)
{
    for (int nCount = 0; nCount < TYPE_MAX; nCount++)
    {
        if (m_apTexture[nCount] != NULL)
        {
            m_apTexture[nCount]->Release();
            m_apTexture[nCount] = NULL;
        }
    }
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CEffect3D * CEffect3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 rot, float fFadeOutRate, TYPE type)
{
    // メモリの確保
    CEffect3D *pEffect3D = NULL;

    // タイプごとに、Zバッファを無効化するエフェクトかどうかを決める
    if (type == TYPE_ACID || type == TYPE_ACID_ATTACK || type == TYPE_WAVE_ATTACK
        || type == TYPE_BURST_CENTER || type == TYPE_BURST_AROUND)
    {
        pEffect3D = new CEffect3D(CScene::OBJTYPE_EFFECT3D_Z_NONE);
    }
    else
    {
        pEffect3D = new CEffect3D(CScene::OBJTYPE_EFFECT3D);
    }

    // メモリを確保出来たら
    if (pEffect3D != NULL)
    {
        // 種類を結びつけておく
        pEffect3D->m_type = type;

        // 初期化
        pEffect3D->Init(pos, size);

        // ビルボードへ設定を反映
        pEffect3D->SetRot(rot);
        pEffect3D->SetCol(col);

        // 種類ごとの設定
        switch (type)
        {
        case TYPE_PURPLE_FIRE:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_GUN_MARKS:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_CRACK1:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_CRACK2:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_CRACK3:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_CRACK4:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_CHARGE:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            pEffect3D->m_fSpeed = EFFECT_CHARGE_SPEED;
            break;

        case TYPE_SMOKE:
            pEffect3D->SetTexUV(0.0f, 1.0f / EFFECT_SMOKE_MAX_PATTERN, 0.0f, 1.0f);
            break;

        case TYPE_ACID:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_ACID_ATTACK:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_WAVE_ATTACK:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_BURST_CENTER:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_BURST_AROUND:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            break;

        case TYPE_SPRAY:
            pEffect3D->SetAlphaTestBorder(EFFECT_FIRE_ALPHA_TEST_BORDER);
            pEffect3D->m_fSpeed = pEffect3D->GetRandRotSpeed(EFFECT_SPRAY_ROT_MAX);
            break;

        case TYPE_RELOADING:
            pEffect3D->m_fSpeed = EFFECT_RELOADING_SPEED;
            break;

        case TYPE_EXPLOSION:
            pEffect3D->SetAlphaTestBorder(EFFECT_EX_ALPHA_TEST_BORDER);
            pEffect3D->m_fSpeed = pEffect3D->GetRandRotSpeed(EFFECT_EX_ROT_MAX);
            break;

        case TYPE_BLUE_FIRE:
            pEffect3D->SetAlphaTestBorder(EFFECT_EX_ALPHA_TEST_BORDER);
            pEffect3D->m_fSpeed = pEffect3D->GetRandRotSpeed(EFFECT_EX_ROT_MAX);
            break;
        }

        // 引数から、メンバ変数を結びつける
        pEffect3D->m_col = col;
        pEffect3D->m_move = move;
        pEffect3D->m_fFadeOutRate = fFadeOutRate;
    }

    return pEffect3D;
}

//=============================================================================
// ランダムに回転速度を決める
// Author : 後藤慎之助
//=============================================================================
float CEffect3D::GetRandRotSpeed(int nMax)
{
    float fReturnSpeed = 0.0f;

    // ランダムに値を取得
    fReturnSpeed = float(GetRandNum(nMax, 0)) / 100.0f - float(GetRandNum(nMax, 0)) / 100.0f;

    // 値を角度に変える
    fReturnSpeed = D3DXToRadian(fReturnSpeed);

    return fReturnSpeed;
}

//=============================================================================
// 左手への角度を決める
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::GetAngleToLeftHand(D3DXVECTOR3 pos)
{
    // カウンタ加算
    m_nCntTime++;

    // カウンタが一定以上でホーミング開始
    if (m_nCntTime >= EFFECT_CHARGE_START_HOMING)
    {
        // 自機の左手の位置を取得
        CPlayer *pPlayer = NULL;
        pPlayer = CGame::GetPlayer();
        CCharacter*pCharacter = NULL;
        pCharacter = (CCharacter*)pPlayer;
        D3DXVECTOR3 leftHandPos = D3DXVECTOR3(pCharacter->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._41,
            pCharacter->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._42,
            pCharacter->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._43);

        float fTargetPosX = 0.0f;                  // 目標のX座標
        float fTargetPosZ = 0.0f;                  // 目標のY座標
        float fMyPosX = 0.0f;                      // 自身のX座標
        float fMyPosZ = 0.0f;                      // 自身のY座標
        float fAngle = 0.0f;                       // 求める角度

        // 目標の座標と自身の座標
        fTargetPosX = leftHandPos.x, fTargetPosZ = leftHandPos.z;
        fMyPosX = pos.x, fMyPosZ = pos.z;

        // 角度を求める
        fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosZ - fTargetPosZ));

        // 横移動の値を決める
        m_move.x = -sinf(fAngle) * m_fSpeed;
        m_move.z = -cosf(fAngle) * m_fSpeed;

        // プレイヤーとの距離を測る
        float fDistance = sqrtf(
            powf((leftHandPos.x - pos.x), 2) +
            powf((leftHandPos.z - pos.z), 2));

        // プレイヤーとの高さの差を測る
        float fHeight = fabsf((leftHandPos.y) - pos.y);

        // 角度を決める
        float fAngleY = atan2(fDistance, fHeight);

        // 縦の移動量を決める
        m_move.y = cosf(fAngleY) * m_fSpeed;

        // 左手の位置のほうが、自身の位置より低いなら
        if (leftHandPos.y < pos.y)
        {
            // Y軸の移動角度を逆にする
            m_move.y *= -1;
        }

        // フェードアウトの開始フラグがfalseで
        if (m_bStartFadeOut == false)
        {
            // 目的地に近いなら
            if (fDistance <= EFFECT_CHARGE_CONSIDER_CLOSE_DEST && fHeight <= EFFECT_CHARGE_CONSIDER_CLOSE_DEST_Y)
            {
                // 生存しているなら
                if (pPlayer != NULL)
                {
                    if (pPlayer->GetDisp() == true)
                    {
                        //// 音を取得
                        //CSound*pSound = CManager::GetSound();
                        //pSound->Play(CSound::LABEL_SE_ORB);

                        // オーブ数上昇
                        pPlayer->SetRisingLeftHandOrbs();
                    }
                }

                // フェードアウト開始
                m_bStartFadeOut = true;
            }
        }
    }
}

//=============================================================================
// 右手への移動量を決める
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::GetMoveToRightHand(D3DXVECTOR3 pos)
{
    // 自機の右手の位置を取得
    CPlayer *pPlayer = NULL;
    pPlayer = CGame::GetPlayer();
    CCharacter*pCharacter = NULL;
    pCharacter = (CCharacter*)pPlayer;
    D3DXVECTOR3 rightHandPos = D3DXVECTOR3(pCharacter->GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._41,
        pCharacter->GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._42,
        pCharacter->GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._43);

    float fTargetPosX = 0.0f;                  // 目標のX座標
    float fTargetPosZ = 0.0f;                  // 目標のY座標
    float fMyPosX = 0.0f;                      // 自身のX座標
    float fMyPosZ = 0.0f;                      // 自身のY座標
    float fAngle = 0.0f;                       // 求める角度

    // 目標の座標と自身の座標
    fTargetPosX = rightHandPos.x, fTargetPosZ = rightHandPos.z;
    fMyPosX = pos.x, fMyPosZ = pos.z;

    // 角度を求める
    fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosZ - fTargetPosZ));

    // 横移動の値を決める
    m_move.x = -sinf(fAngle) * m_fSpeed;
    m_move.z = -cosf(fAngle) * m_fSpeed;

    // 高さを比べ、縦移動を調整
    float fDistanceY = 0.0f;
    float fAdjustment = 0.0f;
    if (pos.y < rightHandPos.y)
    {
        fAdjustment = 1.0f;
        fDistanceY = rightHandPos.y - pos.y;
    }
    else if (pos.y == rightHandPos.y)
    {
        fAdjustment = 0.0f;
    }
    else if (pos.y > rightHandPos.y)
    {
        fAdjustment = -1.0f;
        fDistanceY = pos.y - rightHandPos.y;
    }

    // 縦移動の値を決める
    m_move.y = (fDistanceY / m_fSpeed) * fAdjustment;
}

//=============================================================================
// 爆砕の周辺を生成
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::SetBurstAround(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 0～3.14を出す*(0 or 1)*-1で正負を決める
    float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

    // 設置する位置の半径を出す
    float fRadius = float(GetRandNum(EFFECT_BURST_CENTER_MAX_RADIUS, 0));

    // 位置を決める(Y軸を少し浮かせた位置で固定)
    D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*fRadius + pos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, -cosf(fAngle)*fRadius + pos.z);

    CEffect3D::Create(effectPos, size*EFFECT_BURST_AROUND_FIRST_SIZE_RATE,
        DEFAULT_VECTOR, D3DXCOLOR(m_col.r, m_col.g, m_col.b, 1.0f), HIT_TOP, EFFECT_BURST_AROUND_FADE_OUT_RATE, CEffect3D::TYPE_BURST_AROUND);
}
