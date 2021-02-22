//===============================================
//
// モデルエフェクト処理 (modelEffect.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "modelEffect.h"
#include "manager.h"
#include "renderer.h"
#include "debug.h"
#include "game.h"
#include "score.h"
#include "effect3d.h"
#include "library.h"
#include "spider.h"
#include "bullet.h"
#include "ant.h"

//========================================
// 静的メンバ変数宣言
//========================================
LPD3DXMESH	CModelEffect::m_apMesh[CModelEffect::TYPE_MAX] = {};
LPD3DXBUFFER CModelEffect::m_apBuffMat[CModelEffect::TYPE_MAX] = {};
DWORD CModelEffect::m_aNumMat[CModelEffect::TYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CModelEffect::m_apTexture[CModelEffect::TYPE_MAX] = {};

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CModelEffect::CModelEffect() :CScene3D(CScene::OBJTYPE_MODEL_EFFECT)
{
    m_nLife = 0;
    m_type = TYPE_YELLOW_SPIDER_BODY;

    m_move = DEFAULT_VECTOR;
    m_rotMove = DEFAULT_VECTOR;
    m_bShootUp = false;
    m_bFallDown = false;

    m_bUseExplosion = false;
    m_bUseLanding = false;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CModelEffect::~CModelEffect()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CModelEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene3D::Init(pos, size);

    // スケールを設定
    SetScale(size);

    // メッシュ、テクスチャをバインド
    BindMesh(m_apMesh[m_type], m_apBuffMat[m_type], m_aNumMat[m_type]);
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CModelEffect::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CModelEffect::Update(void)
{
    // 位置、向き取得
    D3DXVECTOR3 pos = GetPos();
    D3DXVECTOR3 rot = GetRot();

    // 位置、向きに移動量を加算
    pos += m_move;
    rot += m_rotMove;

    // 敵を生成する雷なら
    if (m_type == TYPE_KAMI)
    {
        SpawnEnemy();
    }
    // 棘なら
    else if (m_type == TYPE_SPIKE)
    {
        // スケールを取得
        D3DXVECTOR3 scale = GetScale();

        // 縦のみスケールを拡大
        scale.y += MODEL_EFFECT_SPIKE_EXTEND_RATE;

        // 拡大の上限
        if (scale.y > MODEL_EFFECT_SPIKE_EXTEND_MAX)
        {
            scale.y = MODEL_EFFECT_SPIKE_EXTEND_MAX;
        }

        // スケールを設定
        SetScale(scale);
    }

    // 打ち上げるエフェクトなら
    if (m_bShootUp == true)
    {
        // 重力
        m_move.y -= GRAVITY;

        // 地面についたら
        if (pos.y <= 0.0f)
        {
            pos.y = 0.0f;
            m_move.y = 0.0f;

            pos.x -= m_move.x;
            pos.z -= m_move.z;
            rot -= m_rotMove;

            // 爆発するエフェクトなら
            if (m_bUseExplosion == true)
            {
                // 爆発するフラグをfalseに
                m_bUseExplosion = false;

                // 爆発エフェクト
                ExplosionEffect();
            }

            // 着地するエフェクトなら
            if (m_bUseLanding == true)
            {
                //rot.x = MODEL_EFFECT_TILT_RATE_X;
                //rot.z = MODEL_EFFECT_TILT_RATE_Z;
            }
        }
    }

    // 倒れていくエフェクトなら
    if (m_bFallDown == true)
    {
        if (m_rotMove.x > 0.0f)
        {
            // 加速
            m_rotMove.x += MODEL_EFFECT_FALL_DOWN_ACCEL_SPEED;

            // 回転の制限
            if (rot.x > D3DXToRadian(90.0f))
            {
                rot.x = D3DXToRadian(90.0f);
                m_rotMove.x = 0.0f;
            }
        }
        else if (m_rotMove.x < 0.0f)
        {
            // 加速
            m_rotMove.x -= MODEL_EFFECT_FALL_DOWN_ACCEL_SPEED;

            // 回転の制限
            if (rot.x < -D3DXToRadian(90.0f))
            {
                rot.x = -D3DXToRadian(90.0f);
                m_rotMove.x = 0.0f;
            }
        }

        if (m_rotMove.z > 0.0f)
        {
            // 加速
            m_rotMove.z += MODEL_EFFECT_FALL_DOWN_ACCEL_SPEED;

            // 回転の制限
            if (rot.z > D3DXToRadian(90.0f))
            {
                rot.z = D3DXToRadian(90.0f);
                m_rotMove.z = 0.0f;
            }
        }
        else if (m_rotMove.z < 0.0f)
        {
            // 加速
            m_rotMove.z -= MODEL_EFFECT_FALL_DOWN_ACCEL_SPEED;

            // 回転の制限
            if (rot.z < -D3DXToRadian(90.0f))
            {
                rot.z = -D3DXToRadian(90.0f);
                m_rotMove.z = 0.0f;
            }
        }
    }

    // 位置、向きを反映
    SetPos(pos);
    SetRot(rot);

    // 着地するエフェクト以外は
    if (m_bUseLanding == false)
    {
        // ライフを減らす
        m_nLife--;
    }

    // ライフが0以下なら、未使用に
    if (m_nLife <= 0)
    {
        // 終了処理
        Uninit();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CModelEffect::Draw(void)
{
    // 敵を生成する雷だけはライトオフ
    if (m_type == TYPE_KAMI)
    {
        LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

        pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);

        CScene3D::Draw();

        // 反転合成を戻す
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    }
    else
    {
        CScene3D::Draw();
    }
}

//=============================================================================
// モデルデータ読み込み処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CModelEffect::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //==============================
    // 黄蜘蛛の体
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_000_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_YELLOW_SPIDER_BODY], NULL, &m_aNumMat[TYPE_YELLOW_SPIDER_BODY], &m_apMesh[TYPE_YELLOW_SPIDER_BODY]);

    //==============================
    // 黄蜘蛛の腹
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_001_stomach.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_YELLOW_SPIDER_STOMACH], NULL, &m_aNumMat[TYPE_YELLOW_SPIDER_STOMACH], &m_apMesh[TYPE_YELLOW_SPIDER_STOMACH]);

    //==============================
    // 黄蜘蛛の足
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_005_leg_right_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_YELLOW_SPIDER_LEG], NULL, &m_aNumMat[TYPE_YELLOW_SPIDER_LEG], &m_apMesh[TYPE_YELLOW_SPIDER_LEG]);

    //==============================
    // 巨大黒蜘蛛の体
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_000_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_SPIDER_BODY], NULL, &m_aNumMat[TYPE_BLACK_SPIDER_BODY], &m_apMesh[TYPE_BLACK_SPIDER_BODY]);

    //==============================
    // 巨大黒蜘蛛の腹
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_001_stomach.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_SPIDER_STOMACH], NULL, &m_aNumMat[TYPE_BLACK_SPIDER_STOMACH], &m_apMesh[TYPE_BLACK_SPIDER_STOMACH]);

    //==============================
    // 巨大黒蜘蛛の足
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/spider/spider_005_leg_right_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_SPIDER_LEG], NULL, &m_aNumMat[TYPE_BLACK_SPIDER_LEG], &m_apMesh[TYPE_BLACK_SPIDER_LEG]);

    //==============================
    // 信号機
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Signal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGNAL], NULL, &m_aNumMat[TYPE_SIGNAL], &m_apMesh[TYPE_SIGNAL]);

    //==============================
    // 街灯
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/light.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LIGHT], NULL, &m_aNumMat[TYPE_LIGHT], &m_apMesh[TYPE_LIGHT]);

    //==============================
    // 街灯2
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/light2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LIGHT2], NULL, &m_aNumMat[TYPE_LIGHT2], &m_apMesh[TYPE_LIGHT2]);

    //==============================
    // ガレキ大1
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/large001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_LARGE_001], NULL, &m_aNumMat[TYPE_DEBRI_LARGE_001], &m_apMesh[TYPE_DEBRI_LARGE_001]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_LARGE_001]);

    //==============================
    // ガレキ大2
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/large002.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_LARGE_002], NULL, &m_aNumMat[TYPE_DEBRI_LARGE_002], &m_apMesh[TYPE_DEBRI_LARGE_002]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_LARGE_002]);

    //==============================
    // ガレキ大3
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/large003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_LARGE_003], NULL, &m_aNumMat[TYPE_DEBRI_LARGE_003], &m_apMesh[TYPE_DEBRI_LARGE_003]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_LARGE_003]);

    //==============================
    // ガレキ小1
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/small003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_SMALL_001], NULL, &m_aNumMat[TYPE_DEBRI_SMALL_001], &m_apMesh[TYPE_DEBRI_SMALL_001]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_SMALL_001]);

    //==============================
    // ガレキ小2
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/small003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_SMALL_002], NULL, &m_aNumMat[TYPE_DEBRI_SMALL_002], &m_apMesh[TYPE_DEBRI_SMALL_002]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_SMALL_002]);

    //==============================
    // ガレキ小3
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Debri/small003.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DEBRI_SMALL_003], NULL, &m_aNumMat[TYPE_DEBRI_SMALL_003], &m_apMesh[TYPE_DEBRI_SMALL_003]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stone.jpg", &m_apTexture[TYPE_DEBRI_SMALL_003]);

    //==============================
    // 白アリの体
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WHITE_ANT_BODY], NULL, &m_aNumMat[TYPE_WHITE_ANT_BODY], &m_apMesh[TYPE_WHITE_ANT_BODY]);

    //==============================
    // 白アリの腹
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/ant_002_stomach.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WHITE_ANT_STOMACH], NULL, &m_aNumMat[TYPE_WHITE_ANT_STOMACH], &m_apMesh[TYPE_WHITE_ANT_STOMACH]);

    //==============================
    // 白アリの足
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/ant_008_leg_right_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WHITE_ANT_LEG], NULL, &m_aNumMat[TYPE_WHITE_ANT_LEG], &m_apMesh[TYPE_WHITE_ANT_LEG]);

    //==============================
    // 巨大黒アリの体
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_ANT_BODY], NULL, &m_aNumMat[TYPE_BLACK_ANT_BODY], &m_apMesh[TYPE_BLACK_ANT_BODY]);

    //==============================
    // 巨大黒アリの腹
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/ant_002_stomach.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_ANT_STOMACH], NULL, &m_aNumMat[TYPE_BLACK_ANT_STOMACH], &m_apMesh[TYPE_BLACK_ANT_STOMACH]);

    //==============================
    // 巨大黒アリの足
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/ant/ant_008_leg_right_001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLACK_ANT_LEG], NULL, &m_aNumMat[TYPE_BLACK_ANT_LEG], &m_apMesh[TYPE_BLACK_ANT_LEG]);

    //==============================
    // 木1
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood1.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD1], NULL, &m_aNumMat[TYPE_WOOD1], &m_apMesh[TYPE_WOOD1]);

    //==============================
    // 木2
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD2], NULL, &m_aNumMat[TYPE_WOOD2], &m_apMesh[TYPE_WOOD2]);

    //==============================
    // 木3
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood3.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD3], NULL, &m_aNumMat[TYPE_WOOD3], &m_apMesh[TYPE_WOOD3]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ki.jpg", &m_apTexture[TYPE_WOOD3]);

    //==============================
    // ガードレール
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/guardrail.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_GUARDRAIL], NULL, &m_aNumMat[TYPE_GUARDRAIL], &m_apMesh[TYPE_GUARDRAIL]);

    //==============================
    // 注意サイン
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signEmar.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_EMAR], NULL, &m_aNumMat[TYPE_SIGN_EMAR], &m_apMesh[TYPE_SIGN_EMAR]);

    //==============================
    // 十字路サイン
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signInter.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_INTER], NULL, &m_aNumMat[TYPE_SIGN_INTER], &m_apMesh[TYPE_SIGN_INTER]);

    //==============================
    // 駐車禁止サイン
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signNoPark.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_NO_PARK], NULL, &m_aNumMat[TYPE_SIGN_NO_PARK], &m_apMesh[TYPE_SIGN_NO_PARK]);

    //==============================
    // UFO
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/00_Fighter.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_UFO], NULL, &m_aNumMat[TYPE_UFO], &m_apMesh[TYPE_UFO]);

    //==============================
    // UFOの翼
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/UFO/02_RightWing2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, 
        &m_apBuffMat[TYPE_UFO_WING], NULL, &m_aNumMat[TYPE_UFO_WING], &m_apMesh[TYPE_UFO_WING]);

    //==============================
    // 雷
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/kami.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_KAMI], NULL, &m_aNumMat[TYPE_KAMI], &m_apMesh[TYPE_KAMI]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/magic001.png", &m_apTexture[TYPE_KAMI]);

    //==============================
    // クリスタル
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/tower/tower_crystal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CRYSTAL], NULL, &m_aNumMat[TYPE_CRYSTAL], &m_apMesh[TYPE_CRYSTAL]);

    //==============================
    // 鉄塔
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/metal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_METAL], NULL, &m_aNumMat[TYPE_METAL], &m_apMesh[TYPE_METAL]);

    //==============================
    // ミニクリスタル
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/mini.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_MINI_CRYSTAL], NULL, &m_aNumMat[TYPE_MINI_CRYSTAL], &m_apMesh[TYPE_MINI_CRYSTAL]);

    //==============================
    // ロングクリスタル
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/long.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LONG_CRYSTAL], NULL, &m_aNumMat[TYPE_LONG_CRYSTAL], &m_apMesh[TYPE_LONG_CRYSTAL]);

    //==============================
    // X体
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Xbody.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_X_BODY], NULL, &m_aNumMat[TYPE_X_BODY], &m_apMesh[TYPE_X_BODY]);

    //==============================
    // X足
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Xfoot.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_X_FOOT], NULL, &m_aNumMat[TYPE_X_FOOT], &m_apMesh[TYPE_X_FOOT]);

    //==============================
    // X左手
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/XleftHand.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_X_LHAND], NULL, &m_aNumMat[TYPE_X_LHAND], &m_apMesh[TYPE_X_LHAND]);

    //==============================
    // X右手
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/XrightHand.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_X_RHAND], NULL, &m_aNumMat[TYPE_X_RHAND], &m_apMesh[TYPE_X_RHAND]);

    //==============================
    // 棘
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/spike.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SPIKE], NULL, &m_aNumMat[TYPE_SPIKE], &m_apMesh[TYPE_SPIKE]);
    // テクスチャ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/black.png", &m_apTexture[TYPE_SPIKE]);

    //==============================
    // 尻尾
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/05_tail_tip.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_TAIL], NULL, &m_aNumMat[TYPE_TAIL], &m_apMesh[TYPE_TAIL]);

    //==============================
    // 右翼
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/08_wing_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_RIGHT_WING], NULL, &m_aNumMat[TYPE_RIGHT_WING], &m_apMesh[TYPE_RIGHT_WING]);

    //==============================
    // 左翼
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/09_wing_left.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LEFT_WING], NULL, &m_aNumMat[TYPE_LEFT_WING], &m_apMesh[TYPE_LEFT_WING]);

    //==============================
    // ワイバーン体
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/01_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WYVERN_BODY], NULL, &m_aNumMat[TYPE_WYVERN_BODY], &m_apMesh[TYPE_WYVERN_BODY]);

    //==============================
    // ワイバーン頭
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/03_head.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WYVERN_HEAD], NULL, &m_aNumMat[TYPE_WYVERN_HEAD], &m_apMesh[TYPE_WYVERN_HEAD]);

    //==============================
    // ワイバーン首
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/02_neck.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WYVERN_NECK], NULL, &m_aNumMat[TYPE_WYVERN_NECK], &m_apMesh[TYPE_WYVERN_NECK]);

    //==============================
    // ワイバーン腕
    //==============================
    D3DXLoadMeshFromX(LPCSTR("./data/MODEL/wyvern/06_arm_right.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WYVERN_ARM], NULL, &m_aNumMat[TYPE_WYVERN_ARM], &m_apMesh[TYPE_WYVERN_ARM]);

    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 後藤慎之助
//=============================================================================
void CModelEffect::Unload(void)
{
    for (int nCnt = 0; nCnt < CModelEffect::TYPE_MAX; nCnt++)
    {
        if (m_apMesh[nCnt] != NULL)
        {
            m_apMesh[nCnt]->Release();
            m_apMesh[nCnt] = NULL;
        }
        if (m_apBuffMat[nCnt] != NULL)
        {
            m_apBuffMat[nCnt]->Release();
            m_apBuffMat[nCnt] = NULL;
        }
        if (m_apTexture[nCnt] != NULL)
        {
            m_apTexture[nCnt]->Release();
            m_apTexture[nCnt] = NULL;
        }
    }
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CModelEffect * CModelEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 rotMove, TYPE type)
{
    // ポインタを用意
    CModelEffect *pModelEffect = NULL;

    // メモリ確保
    pModelEffect = new CModelEffect;

    // タイプを紐づけ(初期化の中でBindするものがあるため、最初に紐づけ)
    pModelEffect->m_type = type;

    // 初期化(黒系はスケール2倍)
    D3DXVECTOR3 scale = DEFAULT_SCALE;
    if (type == TYPE_BLACK_SPIDER_BODY || type == TYPE_BLACK_SPIDER_LEG || type == TYPE_BLACK_SPIDER_STOMACH ||
        type == TYPE_BLACK_ANT_BODY || type == TYPE_BLACK_ANT_LEG || type == TYPE_BLACK_ANT_STOMACH)
    {
        scale = SPIDER_BLACK_SCALE;
    }
    pModelEffect->Init(pos, scale);

    // 回転を結びつける
    pModelEffect->SetRot(rot);

    // メンバ変数に、引数を結びつける
    pModelEffect->m_move = move;
    pModelEffect->m_rotMove = rotMove;

    // 残存時間を設定
    pModelEffect->m_nLife = MODEL_EFFECT_MAX_LIFE;

    // 種類によって異なる設定を決める
    switch (type)
    {
    case TYPE_YELLOW_SPIDER_BODY:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_YELLOW_SPIDER_STOMACH:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_YELLOW_SPIDER_LEG:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_SPIDER_BODY:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_SPIDER_STOMACH:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_SPIDER_LEG:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_SIGNAL:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_LIGHT:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_LIGHT2:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_DEBRI_LARGE_001:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_DEBRI_LARGE_002:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_DEBRI_LARGE_003:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_DEBRI_SMALL_001:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_nLife = MODEL_EFFECT_SMALL_COLLAPSE_LIFE;
        break;
    case TYPE_DEBRI_SMALL_002:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_nLife = MODEL_EFFECT_SMALL_COLLAPSE_LIFE;
        break;
    case TYPE_DEBRI_SMALL_003:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_nLife = MODEL_EFFECT_SMALL_COLLAPSE_LIFE;
        break;
    case TYPE_WHITE_ANT_BODY:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_WHITE_ANT_STOMACH:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_WHITE_ANT_LEG:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_ANT_BODY:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_ANT_STOMACH:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_BLACK_ANT_LEG:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_WOOD1:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_WOOD2:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_WOOD3:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_GUARDRAIL:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_SIGN_EMAR:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_SIGN_INTER:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_SIGN_NO_PARK:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_UFO:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseExplosion = true;
        break;
    case TYPE_UFO_WING:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_KAMI:
        pModelEffect->m_nLife = MODEL_EFFECT_KAMI_WHOLE_FRAME;
        break;
    case TYPE_CRYSTAL:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseExplosion = true;
        break;
    case TYPE_METAL:
        pModelEffect->m_bFallDown = true;
        break;
    case TYPE_LONG_CRYSTAL:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_X_BODY:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_X_FOOT:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_X_LHAND:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_X_RHAND:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_TAIL:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseLanding = true;
        break;
    case TYPE_RIGHT_WING:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseLanding = true;
        break;
    case TYPE_LEFT_WING:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseLanding = true;
        break;
    case TYPE_WYVERN_BODY:
        pModelEffect->m_bShootUp = true;
        pModelEffect->m_bUseExplosion = true;
        break;
    case TYPE_WYVERN_HEAD:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_WYVERN_NECK:
        pModelEffect->m_bShootUp = true;
        break;
    case TYPE_WYVERN_ARM:
        pModelEffect->m_bShootUp = true;
        break;
    }

    return pModelEffect;
}

//=============================================================================
// 爆発エフェクト
// Author : 後藤慎之助
//=============================================================================
void CModelEffect::ExplosionEffect(void)
{
    // 位置を取得
    D3DXVECTOR3 pos = GetPos();

    // 小さいガレキを飛ばす
    for (int nCnt = 0; nCnt < MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV2; nCnt++)
    {
        // 変数宣言
        float fSpeedY = float(GetRandNum(MODEL_EFFECT_EXPLOSION_SPEED_Y_MAX, MODEL_EFFECT_EXPLOSION_SPEED_Y_MIN)) / 100.0f;
        float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

        // 移動量を決める
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f,
            fSpeedY, 0.0f);

        // 回転量を決める
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        // 0～3.14を出す*(0 or 1)*-1で正負を決める
        float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // 位置を決める
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*MODEL_EFFECT_UFO_EXPLOSION_RADIUS,
            0.0f, -cosf(fAngle)*MODEL_EFFECT_UFO_EXPLOSION_RADIUS) + pos;

        // 種類を決める
        int nRandNum = GetRandNum(2, 0);
        switch (nRandNum)
        {
        case 0:
            modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;
            break;
        case 1:
            modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_002;
            break;
        case 2:
            modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_003;
            break;
        }

        // エフェクト発生
        CModelEffect::Create(effectPos, rotMove, move, rotMove, modelEffectType);
    }

    // 爆発発生(プレイヤーが爆発させた判定)
    D3DXVECTOR3 size = D3DXVECTOR3(BULLET_EXPLOSION_COLLISION_SIZE_LV3, BULLET_EXPLOSION_COLLISION_SIZE_LV3, BULLET_EXPLOSION_COLLISION_SIZE_LV3);
    CBullet::Create(pos, size, DEFAULT_VECTOR,
        CBullet::TYPE_EXPLOSION, CBullet::TRIBE_PLAYER);
}

//=============================================================================
// 敵生成処理(主に、タワーが発する雷が使う)
// Author : 後藤慎之助
//=============================================================================
void CModelEffect::SpawnEnemy(void)
{
    // スケールを取得
    D3DXVECTOR3 scale = GetScale();

    // 境目まで拡大
    if (m_nLife > MODEL_EFFECT_KAMI_CHANGE_BORDER_FRAME)
    {
        scale.x += MODEL_EFFECT_KAMI_EXTEND_RATE;
        scale.y += MODEL_EFFECT_KAMI_EXTEND_RATE;
        scale.z += MODEL_EFFECT_KAMI_EXTEND_RATE;
    }
    // 境目
    else if (m_nLife == MODEL_EFFECT_KAMI_CHANGE_BORDER_FRAME)
    {
        int nRandNum = GetRandNum(4, 1);

        switch (nRandNum)
        {
        case 1:
            // 白アリを生成
            CAnt::Create(GetPos(), DEFAULT_VECTOR, true, CAnt::TYPE_WHITE);
            break;
        case 2:
            // 巨大黒アリを生成
            CAnt::Create(GetPos(), DEFAULT_VECTOR, true, CAnt::TYPE_BLACK);
            break;
        case 3:
            // 黄グモを生成
            CSpider::Create(GetPos(), DEFAULT_VECTOR, true, CSpider::TYPE_YELLOW);
            break;
        case 4:
            // 巨大黒グモを生成
            CSpider::Create(GetPos(), DEFAULT_VECTOR, true, CSpider::TYPE_BLACK);
            break;
        }
    }
    // 境目を超えたら縮小
    else
    {
        scale.x -= MODEL_EFFECT_KAMI_SCALE_DOWN_RATE;
        scale.y -= MODEL_EFFECT_KAMI_SCALE_DOWN_RATE;
        scale.z -= MODEL_EFFECT_KAMI_SCALE_DOWN_RATE;
    }

    // スケールを設定
    SetScale(scale);
}
