//===============================================
//
// 弾処理 (bullet.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "debug.h"
#include "effect3d.h"
#include "library.h"
#include "block.h"
#include "character.h"
#include "camera.h"
#include "modelEffect.h"
#include "spider.h"
#include "ant.h"
#include "character.h"
#include "game.h"
#include "ufo.h"
#include "tower.h"
#include "wyvern.h"
#include "sound.h"

//========================================
// 静的メンバ変数宣言
//========================================
LPD3DXMESH	CBullet::m_apMesh[CBullet::TYPE_MAX] = {};
LPD3DXBUFFER CBullet::m_apBuffMat[CBullet::TYPE_MAX] = {};
DWORD CBullet::m_aNumMat[CBullet::TYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CBullet::m_apTexture[CBullet::TYPE_MAX] = {};

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CBullet::CBullet() :CScene3D(CScene::OBJTYPE_BULLET)
{
    m_nLife = 0;
    m_nDamage = 0;
    m_fSpeed = 0.0f;
    m_moveAngle = DEFAULT_VECTOR;
    m_collisionSize = DEFAULT_VECTOR;
    m_type = TYPE_AR;
    m_tribe = TRIBE_PLAYER;
    m_posOld = DEFAULT_VECTOR;

    m_nCntTime = 0;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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
void CBullet::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CBullet::Update(void)
{
    // 位置、大きさを取得
    D3DXVECTOR3 pos = GetPos();

    // 1F前の位置を結びつける
    m_posOld = pos;

    // 移動量を種類によって変える
    switch (m_type)
    {
    case TYPE_AR:
        // 移動量を位置に結びつける
        pos += m_moveAngle * m_fSpeed;
        break;
    case TYPE_BLOOD:
        // 移動量を位置に結びつける(重力有り)
        m_nCntTime++;
        pos += D3DXVECTOR3(m_moveAngle.x * m_fSpeed, m_moveAngle.y * m_fSpeed + (m_nCntTime * (-GRAVITY)), m_moveAngle.z * m_fSpeed);
        break;
    case TYPE_ACID:
        // 移動量を位置に結びつける(重力有り)
        m_nCntTime++;
        pos += D3DXVECTOR3(m_moveAngle.x * m_fSpeed, m_moveAngle.y * m_fSpeed + (m_nCntTime * (-GRAVITY)), m_moveAngle.z * m_fSpeed);
        break;
    case TYPE_BOMB:
        // 移動量を位置に結びつける(弾道落下有り)
        pos += m_moveAngle * m_fSpeed;
        m_nCntTime++;
        pos.y += m_nCntTime * BULLET_BOMB_FALL_DOWN_RATE;
        break;
    case TYPE_CLAP:
        // 移動量を位置に結びつける
        pos += m_moveAngle * m_fSpeed;
        break;
    case TYPE_SHOCK:
        // 移動量を位置に結びつける
        pos += m_moveAngle * m_fSpeed;
        break;
    }

#ifdef COLLISION_TEST
    if (m_type == TYPE_EXPLOSION)
    {
        // 爆発の当たり判定を見た目通りにするために、サイズの4分の1、下に下げる
        D3DXVECTOR3 explosionPos = pos - D3DXVECTOR3(0.0f, (m_collisionSize.y / 4), 0.0f);
        CDebug::Create(explosionPos, m_collisionSize, CDebug::TYPE_MOMENT);
    }
    else  if (m_type == TYPE_SHOCK)
    {
        // 衝撃波の当たり判定を見た目通りにするために、サイズを変える
        CDebug::Create(pos, BULLET_SHOCK_COLLISION_REAL_SIZE, CDebug::TYPE_MOMENT);
        CDebug::Create(GetMiddlePos(pos, m_posOld), BULLET_SHOCK_COLLISION_REAL_SIZE, CDebug::TYPE_MOMENT);
    }
    else
    {
        CDebug::Create(pos, m_collisionSize, CDebug::TYPE_MOMENT);
        CDebug::Create(GetMiddlePos(pos, m_posOld), m_collisionSize, CDebug::TYPE_MOMENT);
    }
#endif #endif // COLLISION_TEST

    // 位置を設定
    SetPos(pos);

    // 種類ごとに、エフェクト発生
    switch (m_type)
    {
    case TYPE_AR:
        AREffect(pos);
        break;

    case TYPE_BLOOD:
        BloodEffect(pos);
        break;

    case TYPE_ACID:
        AcidEffect(pos);
        break;

    case TYPE_BOMB:
        BombEffect(pos);
        break;

    case TYPE_EXPLOSION:
        ExplosionEffect(pos);
        break;

    case TYPE_CLAP:
        ClapEffect(pos);
        break;

    case TYPE_SHOCK:
        ShockEffect(pos);
        break;
    }

    // 当たり判定(爆発かそれ以外か)
    bool bUse = true;
    if (m_type == TYPE_EXPLOSION)
    {
        ExplosionCollision(pos);
    }
    else
    {
        bUse = Collision(pos);
    }

    // ライフ減少
    m_nLife--;

    // ライフがなくなった、または使用フラグがなくなったら、消滅
    if (m_nLife <= 0 || bUse == false)
    {
        Uninit();
    }
}

//=============================================================================
// 衝突判定処理
// Author : 後藤慎之助
//=============================================================================
bool CBullet::Collision(D3DXVECTOR3 pos)
{
    // 使うフラグ
    bool bUse = true;

    // 当たり判定の大きさ
    D3DXVECTOR3 bulletCollisionSize = m_collisionSize;

    // 衝撃波は当たり判定の大きさを削る
    if (m_type == TYPE_SHOCK)
    {
        bulletCollisionSize = BULLET_SHOCK_COLLISION_REAL_SIZE;
    }

    // 地面より下に行ったら
    if (pos.y <= 0.0f)
    {
        // アサルトライフルの弾なら
        if (m_type == TYPE_AR)
        {
            // 銃痕発生
            CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_RED_FIRE_SIZE,
                DEFAULT_VECTOR, EFFECT_RED_FIRE_FIRST_COLOR, HIT_TOP, EFFECT_RED_FIRE_FADE_OUT_RATE, CEffect3D::TYPE_GUN_MARKS);
        }
        // 血なら
        else if (m_type == TYPE_BLOOD)
        {
            // 酸発生
            CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_ACID_FIRST_SIZE,
                DEFAULT_VECTOR, EFFECT_ACID_FIRST_COLOR, HIT_TOP, EFFECT_ACID_FADE_OUT_RATE, CEffect3D::TYPE_ACID);
        }
        // 酸なら
        else if (m_type == TYPE_ACID)
        {
            // 攻撃酸発生
            CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_ACID_FIRST_SIZE,
                DEFAULT_VECTOR, EFFECT_ACID_ATTACK_FIRST_COLOR, HIT_TOP, EFFECT_ACID_FADE_OUT_RATE, CEffect3D::TYPE_ACID_ATTACK);
        }
        // 防衛軍の弾なら
        else if (m_type == TYPE_CLAP)
        {
            // 銃痕発生
            CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_RED_FIRE_SIZE,
                DEFAULT_VECTOR, EFFECT_RED_FIRE_FIRST_COLOR, HIT_TOP, EFFECT_RED_FIRE_FADE_OUT_RATE, CEffect3D::TYPE_GUN_MARKS);
        }
        // 衝撃波なら
        else if (m_type == TYPE_SHOCK)
        {
            // 衝撃波攻撃のエフェクト発生
            CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_ACID_FIRST_SIZE,
                DEFAULT_VECTOR, EFFECT_BLUE_FIRE_FIRST_COLOR, HIT_TOP, EFFECT_BLUE_FIRE_FADE_OUT_RATE, CEffect3D::TYPE_WAVE_ATTACK);
        }

        // 衝撃波以外は未使用に
        if (m_type != TYPE_SHOCK)
        {
            bUse = false;
        }
    }

    // ブロックへの当たり判定
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BLOCK);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BLOCK); nCntScene++)
    {
        // 中身があるなら
        if (pScene != NULL)
        {
            // 次のシーンを記憶
            CScene*pNextScene = pScene->GetNextScene();

            // ブロックにキャスト
            CBlock *pBlock = (CBlock*)pScene;

            // ブロックの位置と大きさを取得
            D3DXVECTOR3 blockPos = pBlock->GetPos();
            D3DXVECTOR3 blockSize = pBlock->GetCollisionSize();

            // ブロックとの当たり判定を計算
            BLOCK_HIT_INFO blockHitInfo;
            blockHitInfo.hitSurface = HIT_SURFACE_NONE;
            blockHitInfo.pos = pos;

            // 弾の位置
            D3DXVECTOR3 bulletPos = pos;

            // 現在の位置、1F前と現在の位置の間の2つで当たり判定を取る
            for (int nCnt = 0; nCnt < 2; nCnt++)
            {
                // 2回目は、中間の位置に変える
                if (nCnt == 1)
                {
                    bulletPos = GetMiddlePos(pos, m_posOld);
                }

                // 当たり判定計算
                blockHitInfo = (BlockCollision3D(&bulletPos, &m_posOld, &blockPos,
                    &bulletCollisionSize, &blockSize));

                // 当たったなら、forを抜ける
                if (blockHitInfo.hitSurface != HIT_SURFACE_NONE)
                {
                    // 弾がプレイヤーのものなら
                    if (m_tribe == TRIBE_PLAYER)
                    {
                        // ラストヒットがプレイヤーでダメージ
                        pBlock->SetTakeDamage(m_nDamage, CBlock::LAST_HIT_PLAYER);

                        // クリスタルなら
                        if (pBlock->GetType() == CBlock::TYPE_CRYSTAL)
                        {
                            HitEffect(bulletPos, HIT_CRYSTAL);
                        }
                        // それ以外は汎用エフェクト
                        else
                        {
                            HitEffect(bulletPos, HIT_NON_LIVING);
                        }
                    }
                    // 弾が中立、または衝撃波なら
                    else if (m_tribe == TRIBE_NEUTRAL || m_type == TYPE_SHOCK)
                    {
                        // ラストヒットが敵でダメージを与える(中立のみ、ダメージ減衰有り)
                        float fDamage = (float)m_nDamage;
                        if (m_tribe == TRIBE_NEUTRAL)
                        {
                            float fDamage = (float)m_nDamage * BULLET_NEUTRAL_ATTENUATION_FOR_BLOCK_RATE;
                        }
                        pBlock->SetTakeDamage((int)fDamage, CBlock::LAST_HIT_ENEMY);

                        // 非生物へのヒットエフェクト
                        HitEffect(bulletPos, HIT_NON_LIVING);
                    }

                    // 衝撃波以外は
                    if (m_type != TYPE_SHOCK)
                    {
                        // 未使用に
                        bUse = false;
                    }

                    break;
                }
            }

            // ヒット面に液体発生
            D3DXCOLOR color = DEFAULT_COLOR;
            CEffect3D::TYPE effectType = CEffect3D::TYPE_ACID;
            if (m_type == TYPE_BLOOD)
            {
                color = EFFECT_ACID_FIRST_COLOR;
                effectType = CEffect3D::TYPE_ACID;
            }
            else if (m_type == TYPE_ACID)
            {
                color = EFFECT_ACID_ATTACK_FIRST_COLOR;
                effectType = CEffect3D::TYPE_ACID_ATTACK;
            }
            //=====================================================================================
            switch (blockHitInfo.hitSurface)
            {
            case HIT_SURFACE_TOP:

                // 液体なら
                if (m_tribe == TRIBE_LIQUID)
                {
                    // 酸発生
                    CEffect3D::Create(D3DXVECTOR3(pos.x, blockPos.y + blockSize.y + EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_ACID_FIRST_SIZE,
                        DEFAULT_VECTOR, color, HIT_TOP, EFFECT_ACID_FADE_OUT_RATE, effectType);
                }

                break;

            case HIT_SURFACE_LEFT:

                // 液体なら
                if (m_tribe == TRIBE_LIQUID)
                {
                    // 酸発生
                    CEffect3D::Create(D3DXVECTOR3(blockPos.x - (blockSize.x / 2) - EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.y, pos.z),
                        EFFECT_ACID_FIRST_SIZE, DEFAULT_VECTOR, color, HIT_LEFT, EFFECT_ACID_FADE_OUT_RATE, effectType);
                }

                break;

            case HIT_SURFACE_BACK:

                // 液体なら
                if (m_tribe == TRIBE_LIQUID)
                {
                    // 酸発生
                    CEffect3D::Create(D3DXVECTOR3(pos.x, pos.y, blockPos.z + (blockSize.z / 2) + EFFECT_ACID_FLOAT_FROM_SURFACE_POS),
                        EFFECT_ACID_FIRST_SIZE, DEFAULT_VECTOR, color, HIT_BACK, EFFECT_ACID_FADE_OUT_RATE, effectType);
                }

                break;

            case HIT_SURFACE_RIGHT:

                // 液体なら
                if (m_tribe == TRIBE_LIQUID)
                {
                    // 酸発生
                    CEffect3D::Create(D3DXVECTOR3(blockPos.x + (blockSize.x / 2) + EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.y, pos.z),
                        EFFECT_ACID_FIRST_SIZE, DEFAULT_VECTOR, color, HIT_RIGHT, EFFECT_ACID_FADE_OUT_RATE, effectType);
                }

                break;

            case HIT_SURFACE_FRONT:

                // 液体なら
                if (m_tribe == TRIBE_LIQUID)
                {
                    // 酸発生
                    CEffect3D::Create(D3DXVECTOR3(pos.x, pos.y, blockPos.z - (blockSize.z / 2) - EFFECT_ACID_FLOAT_FROM_SURFACE_POS),
                        EFFECT_ACID_FIRST_SIZE, DEFAULT_VECTOR, color, HIT_FRONT, EFFECT_ACID_FADE_OUT_RATE, effectType);
                }

                break;
            }
            //=====================================================================================

            // 当たったなら、for分を抜ける
            if (blockHitInfo.hitSurface != HIT_SURFACE_NONE)
            {
                break;
            }

            // 次のシーンにする
            pScene = pNextScene;
        }
        else
        {
            // 中身がないなら、そこで処理を終える
            break;
        }
    }

    // 弾がプレイヤーか中立、または衝撃波なら
    if (m_tribe == TRIBE_PLAYER || m_tribe == TRIBE_NEUTRAL || m_type == TYPE_SHOCK)
    {
        // 敵への当たり判定
        pScene = CScene::GetTopScene(CScene::OBJTYPE_ENEMY);
        for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_ENEMY); nCntScene++)
        {
            // 中身があるなら
            if (pScene != NULL)
            {
                // 次のシーンを記憶
                CScene*pNextScene = pScene->GetNextScene();

                // キャラクターにキャスト
                CCharacter *pEnemy = (CCharacter*)pScene;

                // 当たり判定の数分回す
                for (int nCntCollision = 0; nCntCollision < pEnemy->GetCntCollision(); nCntCollision++)
                {
                    // エネミーの位置と大きさを取得
                    CCharacter::COLLISION enemyCollision = pEnemy->GetCollision(nCntCollision);

                    // 弾の位置
                    D3DXVECTOR3 bulletPos = pos;

                    // 現在の位置、1F前と現在の位置の間の2つで当たり判定を取る
                    for (int nCnt = 0; nCnt < 2; nCnt++)
                    {
                        // 2回目は、中間の位置に変える
                        if (nCnt == 1)
                        {
                            bulletPos = GetMiddlePos(pos, m_posOld);
                        }

                        // 当たっているなら
                        if (RectangleCollision3D(&bulletPos, &enemyCollision.pos,
                            &bulletCollisionSize, &enemyCollision.size) == true)
                        {
                            // ボス仕様でないなら
                            if (pEnemy->GetBossSetting() == false)
                            {
                                // 種族によって、ダメージを与えるときのラストヒットを変える
                                if (m_tribe == TRIBE_PLAYER)
                                {
                                    pEnemy->SetTakeDamage(m_nDamage, CCharacter::LAST_HIT_PLAYER);
                                }
                                else
                                {
                                    // 敵にダメージ(ダメージ減衰有り)
                                    float fDamage = (float)m_nDamage * BULLET_NEUTRAL_ATTENUATION_FOR_ENEMY_RATE;
                                    pEnemy->SetTakeDamage((int)fDamage, CCharacter::LAST_HIT_ENEMY);
                                }
                            }

                            // 衝撃波でないなら
                            if (m_type != TYPE_SHOCK)
                            {
                                // 種族ごとに場合分け
                                CSpider*pSpider = NULL;
                                CAnt*pAnt = NULL;
                                CUfo*pUfo = NULL;
                                CTower*pTower = NULL;
                                CWyvern*pWyvern = NULL;
                                switch (pEnemy->GetTribe())
                                {
                                case CCharacter::TRIBE_SPIDER:

                                    // スパイダー型にキャスト
                                    pSpider = (CSpider*)pEnemy;

                                    // プレイヤーの弾なら、警告
                                    if (m_tribe == TRIBE_PLAYER)
                                    {
                                        // 警戒中に
                                        pSpider->SetWarning(true);

                                        // 周囲の仲間に警告する
                                        pSpider->WarnFellow();
                                    }

                                    // 生物へのヒットエフェクト
                                    HitEffect(bulletPos, HIT_LIVING);

                                    break;

                                case CCharacter::TRIBE_ANT:

                                    // アリ型にキャスト
                                    pAnt = (CAnt*)pEnemy;

                                    // プレイヤーの弾なら、警告
                                    if (m_tribe == TRIBE_PLAYER)
                                    {
                                        // 警戒中に
                                        pAnt->SetWarning(true);

                                        // 周囲の仲間に警告する
                                        pAnt->WarnFellow();
                                    }

                                    // 生物へのヒットエフェクト
                                    HitEffect(bulletPos, HIT_LIVING);

                                    break;

                                case CCharacter::TRIBE_FORCE:

                                    // 人間へのヒットエフェクト
                                    HitEffect(bulletPos, HIT_HUMAN);

                                    break;

                                case CCharacter::TRIBE_UFO:

                                    // UFO型にキャスト
                                    pUfo = (CUfo*)pEnemy;

                                    // プレイヤーの弾なら、警告
                                    if (m_tribe == TRIBE_PLAYER)
                                    {
                                        // 警戒中に
                                        pUfo->SetWarning(true);

                                        // 周囲の仲間に警告する
                                        pUfo->WarnFellow();
                                    }

                                    // 非生物へのヒットエフェクト
                                    HitEffect(bulletPos, HIT_NON_LIVING);

                                    break;

                                case CCharacter::TRIBE_TOWER:

                                    // タワー型にキャスト
                                    pTower = (CTower*)pEnemy;

                                    // ダメージを当たった場所に応じて計算
                                    pTower->TakeDamage(bulletPos, m_nDamage, (CTower::PARTS)nCntCollision);

                                    // 非生物へのヒットエフェクト
                                    HitEffect(bulletPos, HIT_NON_LIVING);

                                    break;

                                case CCharacter::TRIBE_WYVERN:

                                    // ワイバーン型にキャスト
                                    pWyvern = (CWyvern*)pEnemy;

                                    // ダメージを当たった場所に応じて計算
                                    pWyvern->TakeDamage(bulletPos, m_nDamage, nCntCollision);

                                    // 非生物へのヒットエフェクト
                                    HitEffect(bulletPos, HIT_NON_LIVING);

                                    break;
                                }
                            }

                            // 弾を未使用に
                            bUse = false;
                        }

                        // 当たったならforを抜ける
                        if (bUse == false)
                        {
                            break;
                        }
                    }

                    // 当たったならforを抜ける
                    if (bUse == false)
                    {
                        break;
                    }
                }

                // 次のシーンにする
                pScene = pNextScene;
            }
            else
            {
                // 中身がないなら、そこで処理を終える
                break;
            }
        }
    }

    // 弾が敵か中立のものなら
    if(m_tribe == TRIBE_ENEMY || m_tribe == TRIBE_NEUTRAL)
    {
        // 自機の位置を取得
        D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

        // 弾の位置
        D3DXVECTOR3 bulletPos = pos;

        // 現在の位置、1F前と現在の位置の間の2つで当たり判定を取る
        for (int nCnt = 0; nCnt < 2; nCnt++)
        {
            // 2回目は、中間の位置に変える
            if (nCnt == 1)
            {
                bulletPos = GetMiddlePos(pos, m_posOld);
            }

            // 当たっているなら
            if (RectangleCollision3D(&bulletPos, &playerPos,
                &bulletCollisionSize, &PLAYER_COLLISION_SIZE) == true)
            {
                // クラップは、無敵時間やノックバックを設けず、直接体力を減らす
                if (m_type == TYPE_CLAP)
                {  
                    // ゲームのステートが通常なら
                    if (CGame::GetState() == CGame::STATE_NORMAL)
                    {
                        // 無敵時間がないなら
                        if (CGame::GetPlayer()->GetInvincibleFrame() <= 0)
                        {
                            // 自機に直接ダメージ
                            CGame::GetPlayer()->SetTakeDamage(m_nDamage, CCharacter::LAST_HIT_ENEMY);

                            // 生物へのヒットエフェクト
                            HitEffect(bulletPos, HIT_LIVING);
                        }
                    }
                }
                else
                {
                    // 自機にダメージ
                    CGame::GetPlayer()->TakeDamage(m_nDamage, m_posOld);

                    // 生物へのヒットエフェクト
                    HitEffect(bulletPos, HIT_LIVING);
                }

                // 弾を未使用に
                bUse = false;
            }

            // 当たったならforを抜ける
            if (bUse == false)
            {
                break;
            }
        }
    }

    // 爆弾なら
    if (m_type == TYPE_BOMB)
    {
        // 未使用になった際に爆発生成
        if (bUse == false)
        {
            SetExplosion(pos);
        }
    }

    // 衝撃波は未使用にならない
    if (m_type == TYPE_SHOCK)
    {
        bUse = true;
    }

    return bUse;
}

//=============================================================================
// 爆発の当たり判定
// Author : 後藤慎之助
//=============================================================================
void CBullet::ExplosionCollision(D3DXVECTOR3 pos)
{
    // 当たり判定が発生するまでの時間を数える
    m_nCntTime++;
    if (m_nCntTime >= BULLET_EXPLOSION_START_COLLISION)
    {
        // 爆発の当たり判定を見た目通りにするために、サイズの4分の1、下に下げる
        D3DXVECTOR3 explosionPos = pos - D3DXVECTOR3(0.0f, (m_collisionSize.y / 4), 0.0f);

        // ブロックへの当たり判定
        CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BLOCK);
        for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BLOCK); nCntScene++)
        {
            // 中身があるなら
            if (pScene != NULL)
            {
                // 次のシーンを記憶
                CScene*pNextScene = pScene->GetNextScene();

                // ブロックにキャスト
                CBlock *pBlock = (CBlock*)pScene;

                // ブロックの位置と大きさを取得
                D3DXVECTOR3 blockPos = pBlock->GetPos();
                D3DXVECTOR3 blockSize = pBlock->GetCollisionSize();

                // ブロックとの当たり判定を計算
                if (RectangleCollision3D(&explosionPos, &blockPos,
                    &m_collisionSize, &blockSize) == true)
                {
                    // 種族によって、ダメージを与えるときのラストヒットを変える
                    if (m_tribe == TRIBE_PLAYER)
                    {
                        pBlock->SetTakeDamage(m_nDamage, CBlock::LAST_HIT_PLAYER);
                    }
                    else
                    {
                        pBlock->SetTakeDamage(m_nDamage, CBlock::LAST_HIT_ENEMY);
                    }
                }

                // 次のシーンにする
                pScene = pNextScene;
            }
            else
            {
                // 中身がないなら、そこで処理を終える
                break;
            }
        }

        // 敵への当たり判定
        pScene = CScene::GetTopScene(CScene::OBJTYPE_ENEMY);
        for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_ENEMY); nCntScene++)
        {
            // 中身があるなら
            if (pScene != NULL)
            {
                // 次のシーンを記憶
                CScene*pNextScene = pScene->GetNextScene();

                // キャラクターにキャスト
                CCharacter *pEnemy = (CCharacter*)pScene;

                // 当たり判定の数分回す
                bool bHitWyvern = false;    // ワイバーンに当たったかどうか
                float fDistance1 = 0.0f;    // 距離1
                float fDistance2 = 0.0f;    // 距離2
                int nCntHit = 0;            // 当たった個所
                for (int nCntCollision = 0; nCntCollision < pEnemy->GetCntCollision(); nCntCollision++)
                {
                    // エネミーの位置と大きさを取得
                    CCharacter::COLLISION enemyCollision = pEnemy->GetCollision(nCntCollision);

                    // 当たっているなら
                    if (RectangleCollision3D(&explosionPos, &enemyCollision.pos,
                        &m_collisionSize, &enemyCollision.size) == true)
                    {
                        // ボス仕様でないなら
                        if (pEnemy->GetBossSetting() == false)
                        {
                            // 種族によって、ダメージを与えるときのラストヒットを変える
                            if (m_tribe == TRIBE_PLAYER)
                            {
                                pEnemy->SetTakeDamage(m_nDamage, CCharacter::LAST_HIT_PLAYER);
                            }
                            else
                            {
                                pEnemy->SetTakeDamage(m_nDamage, CCharacter::LAST_HIT_ENEMY);
                            }
                        }
                        // ボス仕様なら、多段ヒット回避のため、1Fだけ爆発のダメージを通す
                        else
                        {
                            // ワイバーンなら
                            if (pEnemy->GetTribe() == CCharacter::TRIBE_WYVERN)
                            {
                                // ボスに対しては、どの爆発でもプレイヤーのものとして扱う
                                if (m_nCntTime == BULLET_EXPLOSION_USE_COLLISION_FRAME_FOR_BOSS)
                                {
                                    // 当たった後に、距離1と距離2を比較する
                                    if (bHitWyvern == true)
                                    {
                                        // 距離2を決める
                                        fDistance2 = sqrtf(powf((enemyCollision.pos.x - pos.x), 2) + powf((enemyCollision.pos.z - pos.z), 2));

                                        // 距離を比較して、距離2のほうが小さいなら
                                        if (fDistance1 > fDistance2)
                                        {
                                            // 距離1を更新
                                            fDistance1 = fDistance2;

                                            // 当たった個所を結びつける
                                            nCntHit = nCntCollision;
                                        }
                                    }

                                    // 当たってないなら、最初の距離を結びつける
                                    if (bHitWyvern == false)
                                    {
                                        // 距離1を決める
                                        fDistance1 = sqrtf(powf((enemyCollision.pos.x - pos.x), 2) + powf((enemyCollision.pos.z - pos.z), 2));

                                        // 当たったフラグをtrueに
                                        bHitWyvern = true;

                                        // 当たった個所を結びつける
                                        nCntHit = nCntCollision;
                                    }
                                }
                            }
                            else
                            {
                                // ワイバーン以外のボスに対しては、どの爆発でもプレイヤーのものとして扱う
                                if (m_nCntTime == BULLET_EXPLOSION_USE_COLLISION_FRAME_FOR_BOSS)
                                {
                                    pEnemy->SetTakeDamage(m_nDamage, CCharacter::LAST_HIT_PLAYER);
                                }
                            }
                        }

                        // ワイバーンでないなら
                        if (pEnemy->GetTribe() != CCharacter::TRIBE_WYVERN)
                        {
                            // 多段ヒットを避けるため、forを抜ける
                            break;
                        }
                    }
                }

                // ワイバーンに当たっているなら
                if (bHitWyvern == true)
                {
                    // ワイバーン型にキャスト
                    CWyvern *pWyvern = (CWyvern*)pEnemy;

                    // 種族によって、ダメージが変わる
                    float fHitDamage = (float)m_nDamage;
                    if (m_tribe == TRIBE_ENEMY)
                    {
                        // 敵の爆発ならダメージ減衰
                        fHitDamage *= WYVERN_ENEMY_EXPLOSION_ATTENUATION_RATE;
                    }

                    // ダメージを当たった場所に応じて計算
                    pWyvern->TakeDamage(pos, (int)fHitDamage, nCntHit);
                }

                // 次のシーンにする
                pScene = pNextScene;
            }
            else
            {
                // 中身がないなら、そこで処理を終える
                break;
            }
        }

        // 自機の位置を取得
        D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

        // 爆発のサイズを設定
        D3DXVECTOR3 collisionSize = m_collisionSize;

        // 自分の爆発なら
        if (m_tribe == TRIBE_PLAYER)
        {
            // サイズ減衰
            collisionSize *= BULLET_EXPLOSION_SIZE_ATTENUATION_FOR_PLAYER_RATE;
        }

        // 当たっているなら
        if (RectangleCollision3D(&explosionPos, &playerPos,
            &collisionSize, &PLAYER_COLLISION_SIZE) == true)
        {
            // 自機にダメージ(ダメージ減衰有り)
            float fDamage = (float)m_nDamage * BULLET_EXPLOSION_ATTENUATION_FOR_PLAYER_RATE;
            CGame::GetPlayer()->TakeDamage((int)fDamage, m_posOld);
        }
    }
}

//=============================================================================
// 爆発生成処理
// Author : 後藤慎之助
//=============================================================================
void CBullet::SetExplosion(D3DXVECTOR3 pos)
{
    // コリジョンサイズによって、生成する爆発の大きさを変える
    D3DXVECTOR3 size = DEFAULT_VECTOR;
    if (m_collisionSize.x >= BULLET_BOMB_COLLISION_SIZE_LV1 && m_collisionSize.x < BULLET_BOMB_COLLISION_SIZE_LV2)
    {
        size = D3DXVECTOR3(BULLET_EXPLOSION_COLLISION_SIZE_LV1, BULLET_EXPLOSION_COLLISION_SIZE_LV1, BULLET_EXPLOSION_COLLISION_SIZE_LV1);
        CBullet::Create(pos, size, DEFAULT_VECTOR,
            CBullet::TYPE_EXPLOSION, m_tribe);
    }
    else if (m_collisionSize.x >= BULLET_BOMB_COLLISION_SIZE_LV2 && m_collisionSize.x < BULLET_BOMB_COLLISION_SIZE_LV3)
    {
        size = D3DXVECTOR3(BULLET_EXPLOSION_COLLISION_SIZE_LV2, BULLET_EXPLOSION_COLLISION_SIZE_LV2, BULLET_EXPLOSION_COLLISION_SIZE_LV2);
        CBullet::Create(pos, size, DEFAULT_VECTOR,
            CBullet::TYPE_EXPLOSION, m_tribe);
    }
    else if (m_collisionSize.x >= BULLET_BOMB_COLLISION_SIZE_LV3 && m_collisionSize.x < BULLET_BOMB_COLLISION_SIZE_LV4)
    {
        size = D3DXVECTOR3(BULLET_EXPLOSION_COLLISION_SIZE_LV3, BULLET_EXPLOSION_COLLISION_SIZE_LV3, BULLET_EXPLOSION_COLLISION_SIZE_LV3);
        CBullet::Create(pos, size, DEFAULT_VECTOR,
            CBullet::TYPE_EXPLOSION, m_tribe);
    }
    else if (m_collisionSize.x >= BULLET_BOMB_COLLISION_SIZE_LV4)
    {
        size = D3DXVECTOR3(BULLET_EXPLOSION_COLLISION_SIZE_LV4, BULLET_EXPLOSION_COLLISION_SIZE_LV4, BULLET_EXPLOSION_COLLISION_SIZE_LV4);
        CBullet::Create(pos, size, DEFAULT_VECTOR,
            CBullet::TYPE_EXPLOSION, m_tribe);
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CBullet::Draw(void)
{
    // 弾はモデルを出さないほうが見栄えが良かったので、Drawしない
}

//=============================================================================
// モデルデータ読み込み処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CBullet::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //==============================
    // アサルトライフルの弾
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bullet.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_AR], NULL, &m_aNumMat[TYPE_AR], &m_apMesh[TYPE_AR]);

    //==============================
    // 血
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bullet.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLOOD], NULL, &m_aNumMat[TYPE_BLOOD], &m_apMesh[TYPE_BLOOD]);

    //==============================
    // 酸
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Acid.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_ACID], NULL, &m_aNumMat[TYPE_ACID], &m_apMesh[TYPE_ACID]);

    //==============================
    // 左手の爆弾
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bomb.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BOMB], NULL, &m_aNumMat[TYPE_BOMB], &m_apMesh[TYPE_BOMB]);

    //==============================
    // 爆発
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bomb.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_EXPLOSION], NULL, &m_aNumMat[TYPE_EXPLOSION], &m_apMesh[TYPE_EXPLOSION]);

    //==============================
    // アサルトライフルの弾
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bullet.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CLAP], NULL, &m_aNumMat[TYPE_CLAP], &m_apMesh[TYPE_CLAP]);

    //==============================
    // 衝撃波
    //==============================
    // モデル
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bomb.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SHOCK], NULL, &m_aNumMat[TYPE_SHOCK], &m_apMesh[TYPE_SHOCK]);

    return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
// Author : 後藤慎之助
//=============================================================================
void CBullet::Unload(void)
{
    for (int nCnt = 0; nCnt < CBullet::TYPE_MAX; nCnt++)
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
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 collisionSize, D3DXVECTOR3 moveAngle, TYPE type, TRIBE tribe)
{
    // ポインタを用意
    CBullet *pBullet = NULL;

    // メモリ確保
    pBullet = new CBullet;

    // タイプを紐づけ(初期化の中でBindするものがあるため、最初に紐づけ)
    pBullet->m_type = type;

    // 初期化
    pBullet->Init(pos, DEFAULT_SCALE);

    // 1F前の位置を、位置と結びつける
    pBullet->m_posOld = pos;

    // 移動の角度を結びつける
    pBullet->m_moveAngle = moveAngle;

    // 種族を結びつける
    pBullet->m_tribe = tribe;

    // 当たり判定の大きさを決める
    pBullet->m_collisionSize = collisionSize;

    // 種類ごとに、結びつける値を変える
    CSound *pSound = CManager::GetSound();
    switch (type)
    {
    case TYPE_AR:

        pBullet->m_fSpeed = BULLET_AR_SPEED;
        pBullet->m_nLife = BULLET_AR_LIFE;
        pBullet->m_nDamage = BULLET_AR_DAMAGE;

        if (tribe == TRIBE_PLAYER)
        {
            // SEを再生
            pSound->Play(CSound::LABEL_SE_MACHINE_GUN_SHOT);
        }

        break;

    case TYPE_BLOOD:

        pBullet->m_fSpeed = BULLET_BLOOD_SPEED;
        pBullet->m_nLife = BULLET_BLOOD_LIFE;

        break;

    case TYPE_ACID:

        pBullet->m_fSpeed = BULLET_ACID_SPEED;
        pBullet->m_nLife = BULLET_ACID_LIFE;
        pBullet->m_nDamage = BULLET_ACID_DAMAGE;

        break;

    case TYPE_BOMB:

        pBullet->m_fSpeed = BULLET_BOMB_FIRST_SPEED;
        pBullet->m_nLife = BULLET_BOMB_LIFE;
        pBullet->m_nDamage = BULLET_BOMB_DAMAGE;

        if (tribe == TRIBE_PLAYER)
        {
            // SEを再生
            pSound->Play(CSound::LABEL_SE_CANON_SHOT);
        }

        break;

    case TYPE_EXPLOSION:

        // コリジョンサイズによって、ライフやダメージを決める
        if (collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV1 && collisionSize.x < BULLET_EXPLOSION_COLLISION_SIZE_LV2)
        {
            pBullet->m_nLife = BULLET_EXPLOSION_LIFE_LV1;
            pBullet->m_nDamage = BULLET_EXPLOSION_DAMAGE_LV1;

            // SEを再生
            pSound->Play(CSound::LABEL_SE_EXPLOSION_SMALL);
        }
        else if (collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV2 && collisionSize.x < BULLET_EXPLOSION_COLLISION_SIZE_LV3)
        {
            pBullet->m_nLife = BULLET_EXPLOSION_LIFE_LV2;
            pBullet->m_nDamage = BULLET_EXPLOSION_DAMAGE_LV2;

            // SEを再生
            pSound->Play(CSound::LABEL_SE_EXPLOSION_SMALL);
        }
        else if (collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV3 && collisionSize.x < BULLET_EXPLOSION_COLLISION_SIZE_LV4)
        {
            pBullet->m_nLife = BULLET_EXPLOSION_LIFE_LV3;
            pBullet->m_nDamage = BULLET_EXPLOSION_DAMAGE_LV3;

            // SEを再生
            pSound->Play(CSound::LABEL_SE_EXPLOSION_MIDIUM);
        }
        else if (collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV4)
        {
            pBullet->m_nLife = BULLET_EXPLOSION_LIFE_LV4;
            pBullet->m_nDamage = BULLET_EXPLOSION_DAMAGE_LV4;

            // SEを再生
            pSound->Play(CSound::LABEL_SE_EXPLOSION_BIG);
        }

        break;

    case TYPE_CLAP:

        pBullet->m_fSpeed = BULLET_CLAP_SPEED;
        pBullet->m_nLife = BULLET_CLAP_LIFE;
        pBullet->m_nDamage = BULLET_CLAP_DAMAGE;

        break;

    case TYPE_SHOCK:

        pBullet->m_fSpeed = BULLET_SHOCK_SPEED;
        pBullet->m_nLife = BULLET_SHOCK_LIFE;
        pBullet->m_nDamage = BULLET_SHOCK_DAMAGE;

        break;
    }

    return pBullet;
}

//=============================================================================
// 中間の位置を取得
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CBullet::GetMiddlePos(D3DXVECTOR3 pos, D3DXVECTOR3 posOld)
{
    D3DXVECTOR3 returnPos = DEFAULT_VECTOR;

    returnPos = (pos + posOld) / 2;

    return returnPos;
}

//=============================================================================
// ヒットエフェクト
// Author : 後藤慎之助
//=============================================================================
void CBullet::HitEffect(D3DXVECTOR3 pos, HIT hit)
{
    // 音取得
    CSound *pSound = CManager::GetSound();

    // 無生物に当たったら
    if (hit == HIT_NON_LIVING)
    {
        for (int nCnt = 0; nCnt < MODEL_EFFECT_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
        {
            // 変数宣言
            float fSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

            // 移動量を決める(カメラのほうに飛んできているイメージ)
            CCamera *pCamera = CManager::GetCamera();
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(pCamera->GetAngle())*fSpeedX,
                fSpeedY, -cosf(pCamera->GetAngle())*fSpeedZ);

            // 回転量を決める
            D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

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
            CModelEffect::Create(pos, rotMove, move, rotMove, modelEffectType);
        }

        // 無生物へのパーティクルを用いたヒットエフェクト用の変数宣言
        float fAngle = 0.0f;                       // 角度
        float fSpeed = 0.0f;                       // 速さ
        float fSize = 0.0f;                        // 大きさ
        CCamera *pCamera = CManager::GetCamera();  // カメラを取得
        for (int nCnt = 0; nCnt < EFFECT_HIT_NON_LIVING_NUMBER_ONCE; nCnt++)
        {
            // 0～3.14を出す*(0 or 1)*-1で正負を決める
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

            // 速さを出す
            fSpeed = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SPEED, EFFECT_HIT_NON_LIVING_MIN_SPEED)) / 100.0f;

            // 移動量を決める
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed,
                cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

            // 大きさを出す
            fSize = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SIZE, EFFECT_HIT_NON_LIVING_MIN_SIZE)) / 100.0f;

            // 大きさを決める
            D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

            // エフェクト発生
            CEffect3D::Create(pos, size, move, EFFECT_HIT_NON_LIVING_COLOR,
                DEFAULT_VECTOR, EFFECT_HIT_NON_LIVING_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }
    }
    // 生物に当たったら
    else if (hit == HIT_LIVING)
    {
        // プレイヤーの弾なら
        if (m_tribe == TRIBE_PLAYER)
        {
            // SEを再生
            pSound->Play(CSound::LABEL_SE_DAMAGE_ENEMY);
        }

        // 0～3.14を出す*(0 or 1)*-1で正負を決める
        float fAngleX = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        float fAngleY = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        float fAngleZ = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // 血のバレットを生成
        CBullet::Create(pos, BULLET_BLOOD_COLLISION_SIZE, D3DXVECTOR3(fAngleX, fAngleY, fAngleZ), CBullet::TYPE_BLOOD, CBullet::TRIBE_LIQUID);

        // 生物へのヒットエフェクト用の変数宣言
        float fAngle = 0.0f;                       // 角度
        float fSpeed = 0.0f;                       // 速さ
        float fSize = 0.0f;                        // 大きさ
        CCamera *pCamera = CManager::GetCamera();  // カメラを取得
        for (int nCnt = 0; nCnt < EFFECT_HIT_LIVING_NUMBER_ONCE; nCnt++)
        {
            // 0～3.14を出す*(0 or 1)*-1で正負を決める
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

            // 速さを出す
            fSpeed = float(GetRandNum(EFFECT_HIT_LIVING_MAX_SPEED, EFFECT_HIT_LIVING_MIN_SPEED)) / 100.0f;

            // 移動量を決める
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed,
                cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

            // 大きさを出す
            fSize = float(GetRandNum(EFFECT_HIT_LIVING_MAX_SIZE, EFFECT_HIT_LIVING_MIN_SIZE)) / 100.0f;

            // 大きさを決める
            D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

            // エフェクト発生
            CEffect3D::Create(pos, size, move, EFFECT_HIT_LIVING_COLOR,
                DEFAULT_VECTOR, EFFECT_HIT_LIVING_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }
    }
    // 人間に当たったら
    else if (hit == HIT_HUMAN)
    {
        // プレイヤーの弾なら
        if (m_tribe == TRIBE_PLAYER)
        {
            // SEを再生
            pSound->Play(CSound::LABEL_SE_DAMAGE_ENEMY);
        }

        // パーティクルを用いたヒットエフェクト用の変数宣言
        float fAngle = 0.0f;                       // 角度
        float fSpeed = 0.0f;                       // 速さ
        float fSize = 0.0f;                        // 大きさ
        CCamera *pCamera = CManager::GetCamera();  // カメラを取得
        for (int nCnt = 0; nCnt < EFFECT_HIT_NON_LIVING_NUMBER_ONCE; nCnt++)
        {
            // 0～3.14を出す*(0 or 1)*-1で正負を決める
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

            // 速さを出す
            fSpeed = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SPEED, EFFECT_HIT_NON_LIVING_MIN_SPEED)) / 100.0f;

            // 移動量を決める
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed,
                cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

            // 大きさを出す
            fSize = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SIZE, EFFECT_HIT_NON_LIVING_MIN_SIZE)) / 100.0f;

            // 大きさを決める
            D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

            // エフェクト発生
            CEffect3D::Create(pos, size, move, EFFECT_HIT_NON_LIVING_COLOR,
                DEFAULT_VECTOR, EFFECT_HIT_NON_LIVING_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }
    }
    // クリスタルに当たったら
    else if (hit == HIT_CRYSTAL)
    {
        // プレイヤーの弾なら
        if (m_tribe == TRIBE_PLAYER)
        {
            // SEを再生
            pSound->Play(CSound::LABEL_SE_DAMAGE_CRYSTAL);
        }

        for (int nCnt = 0; nCnt < MODEL_EFFECT_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
        {
            // 変数宣言
            float fSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

            // 移動量を決める(カメラのほうに飛んできているイメージ)
            CCamera *pCamera = CManager::GetCamera();
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(pCamera->GetAngle())*fSpeedX,
                fSpeedY, -cosf(pCamera->GetAngle())*fSpeedZ);

            // 回転量を決める
            D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

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
            CModelEffect::Create(pos, rotMove, move, rotMove, modelEffectType);
        }

        // エフェクト発生
        float fAngle = 0.0f;    //角度
        float fSpeed = EFFECT_HIT_MINI_CRYSTAL_SPEED;   //速さ
        CCamera*pCamera = CManager::GetCamera();
        for (int nCnt = 0; nCnt < EFFECT_HIT_CRYSTAL_NUMBER_ONCE; nCnt++)
        {
            //0～3.14を出す*(0 or 1)*-1で正負を決める
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

            // エフェクト発生
            CEffect3D::Create(pos, EFFECT_HIT_MINI_CRYSTAL_SIZE, move, EFFECT_HIT_CRYSTAL_COLOR,
                DEFAULT_VECTOR, EFFECT_HIT_MINI_CRYSTAL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }
    }
}

//=============================================================================
// ARのエフェクト
// Author : 後藤慎之助
//=============================================================================
void CBullet::AREffect(D3DXVECTOR3 pos)
{
    // 1F経過した後から、トレイルを出す
    if (m_nLife <= BULLET_AR_LIFE - 1)
    {
        // トレイル発生
        float fSpeed = BULLET_AR_TRAIL_SPEED;
        CEffect3D::Create(pos, m_collisionSize * 3, m_moveAngle * fSpeed, EFFECT_AR_TRAIL_COLOR,
            DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

        // トレイル発生
        CEffect3D::Create(pos, m_collisionSize * 2, m_moveAngle * fSpeed, D3DXCOLOR(1.0f, 0.99f, 0.0f, 0.8f),
            DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

        // トレイル発生(中間位置)
        CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 3, m_moveAngle * fSpeed, EFFECT_AR_TRAIL_COLOR,
            DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

        // トレイル発生(中間位置)
        CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2, m_moveAngle * fSpeed, D3DXCOLOR(1.0f, 0.99f, 0.0f, 0.8f),
            DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
    }

    // 速さを決める
    float fSpeedX = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedY = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedZ = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // 移動量を設定
    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // トレイルデコレーションを現在の位置に発生
    CEffect3D::Create(pos, m_collisionSize * 2, move, EFFECT_AR_TRAIL_DECORATION_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // 速さを決める
    fSpeedX = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedY = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedZ = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // 移動量を設定
    move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // トレイルデコレーションを過去の位置と現在の位置の中間に発生
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2, move, EFFECT_AR_TRAIL_DECORATION_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
}

//=============================================================================
// 血の弾のエフェクト
// Author : 後藤慎之助
//=============================================================================
void CBullet::BloodEffect(D3DXVECTOR3 pos)
{
    float fSpeed = BULLET_BLOOD_TRAIL_SPEED;

    // トレイル発生
    CEffect3D::Create(pos, m_collisionSize * 2, m_moveAngle * fSpeed, EFFECT_BLOOD_TRAIL_COLOR,
        DEFAULT_VECTOR, EFFECT_BLOOD_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // 速さを決める
    float fSpeedX = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedY = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedZ = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // 移動量を設定
    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // トレイルデコレーションを現在の位置に発生
    CEffect3D::Create(pos, m_collisionSize * 2, move, EFFECT_BLOOD_TRAIL_DECORATION_COLOR,
        DEFAULT_VECTOR, EFFECT_BLOOD_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // 速さを決める
    fSpeedX = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedY = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedZ = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // 移動量を設定
    move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // トレイルデコレーションを過去の位置と現在の位置の中間に発生
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2, move, EFFECT_BLOOD_TRAIL_DECORATION_COLOR,
        DEFAULT_VECTOR, EFFECT_BLOOD_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
}

//=============================================================================
// 酸攻撃のエフェクト
// Author : 後藤慎之助
//=============================================================================
void CBullet::AcidEffect(D3DXVECTOR3 pos)
{
    // 速さを決める
    float fSpeedX = float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedY = float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedZ = float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // 移動量を設定
    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // トレイルデコレーションを現在の位置に発生
    CEffect3D::Create(pos, m_collisionSize * 2, move, EFFECT_ACID_ATTACK_TRAIL_DECORATION_COLOR,
        DEFAULT_VECTOR, EFFECT_ACID_ATTACK_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
}

//=============================================================================
// 左手の爆弾のエフェクト
// Author : 後藤慎之助
//=============================================================================
void CBullet::BombEffect(D3DXVECTOR3 pos)
{
    // トレイル発生
    float fSpeed = BULLET_BOMB_TRAIL_SPEED;
    CEffect3D::Create(pos, m_collisionSize * 2.5f, m_moveAngle * fSpeed, EFFECT_BOMB_AROUND_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // トレイル発生
    CEffect3D::Create(pos, m_collisionSize * 1.75f, m_moveAngle * fSpeed, EFFECT_BOMB_CENTER_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // トレイル発生(中間位置)
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2.5f, m_moveAngle * fSpeed, EFFECT_BOMB_AROUND_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // トレイル発生(中間位置)
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 1.75f, m_moveAngle * fSpeed, EFFECT_BOMB_CENTER_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // 速さを決める
    float fSpeedX = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedY = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedZ = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // 移動量を設定
    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // トレイルデコレーションを現在の位置に発生
    CEffect3D::Create(pos, m_collisionSize * 2, move, EFFECT_BOMB_AROUND_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // 速さを決める
    fSpeedX = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedY = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedZ = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // 移動量を設定
    move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // トレイルデコレーションを過去の位置と現在の位置の中間に発生
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2, move, EFFECT_BOMB_AROUND_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
}

//=============================================================================
// 爆発のエフェクト
// Author : 後藤慎之助
//=============================================================================
void CBullet::ExplosionEffect(D3DXVECTOR3 pos)
{
    // コリジョンサイズによって、移動量とフェードアウト割合を決める
    float fFadeOutRate = 0.0f;
    int nMaxSpeed_XZ = 0;
    int nSmallCollapseNumberOnce = 0;
    if (m_collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV1 && m_collisionSize.x < BULLET_EXPLOSION_COLLISION_SIZE_LV2)
    {
        fFadeOutRate = EFFECT_EX_FADE_OUT_RATE_LV1;
        nMaxSpeed_XZ = EFFECT_EX_MAX_SPEED_LV1;
        nSmallCollapseNumberOnce = MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV1;
    }
    else if (m_collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV2 && m_collisionSize.x < BULLET_EXPLOSION_COLLISION_SIZE_LV3)
    {
        fFadeOutRate = EFFECT_EX_FADE_OUT_RATE_LV2;
        nMaxSpeed_XZ = EFFECT_EX_MAX_SPEED_LV2;
        nSmallCollapseNumberOnce = MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV2;
    }
    else if (m_collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV3 && m_collisionSize.x < BULLET_EXPLOSION_COLLISION_SIZE_LV4)
    {
        fFadeOutRate = EFFECT_EX_FADE_OUT_RATE_LV3;
        nMaxSpeed_XZ = EFFECT_EX_MAX_SPEED_LV3;
        nSmallCollapseNumberOnce = MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV3;
    }
    else if (m_collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV4)
    {
        fFadeOutRate = EFFECT_EX_FADE_OUT_RATE_LV4;
        nMaxSpeed_XZ = EFFECT_EX_MAX_SPEED_LV4;
        nSmallCollapseNumberOnce = MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV4;
    }

    // 小さいガレキを飛ばすカウンタになったら
    if (m_nCntTime == BULLET_EXPLOSION_CREATE_SMALL_COLLAPSE_FRAME)
    {
        // 小さいガレキを飛ばす
        for (int nCnt = 0; nCnt < nSmallCollapseNumberOnce; nCnt++)
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
            D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*m_collisionSize.x / 2,
                0.0f, -cosf(fAngle)*m_collisionSize.x / 2) + pos;

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
    }

    // 爆発の炎を発生
    for (int nCnt = 0; nCnt < EFFECT_EX_NUMBER_ONCE; nCnt++)
    {
        // 速さを決める(Y軸は、上方向のみ)
        float fSpeedX = float(GetRandNum(nMaxSpeed_XZ, 0)) / 100.0f - float(GetRandNum(nMaxSpeed_XZ, 0)) / 100.0f;
        float fSpeedY = float(GetRandNum(EFFECT_EX_MAX_SPEED_Y, 0)) / 100.0f;
        float fSpeedZ = float(GetRandNum(nMaxSpeed_XZ, 0)) / 100.0f - float(GetRandNum(nMaxSpeed_XZ, 0)) / 100.0f;

        // 移動量を設定
        D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

        // 赤と黄色の2色を出す
        D3DXCOLOR color = DEFAULT_COLOR;
        if (nCnt == 0)
        {
            color = EFFECT_EX_RED_COLOR;
        }
        else
        {
            color = EFFECT_EX_YELLOW_COLOR;
        }

        // 現在の位置から発生
        CEffect3D::Create(pos, m_collisionSize, move, color,
            DEFAULT_VECTOR, fFadeOutRate, CEffect3D::TYPE_EXPLOSION);
    }
}

//=============================================================================
// クラップのエフェクト
// Author : 後藤慎之助
//=============================================================================
void CBullet::ClapEffect(D3DXVECTOR3 pos)
{
    // トレイル発生
    float fSpeed = BULLET_CLAP_TRAIL_SPEED;

    // トレイル発生
    CEffect3D::Create(pos, m_collisionSize * 3, m_moveAngle * fSpeed, D3DXCOLOR(1.0f, 0.99f, 0.0f, 1.0f),
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // トレイル発生(中間位置)
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2, m_moveAngle * fSpeed, D3DXCOLOR(1.0f, 0.99f, 0.0f, 1.0f),
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
}

//=============================================================================
// 衝撃波のエフェクト
// Author : 後藤慎之助
//=============================================================================
void CBullet::ShockEffect(D3DXVECTOR3 pos)
{
    // 小さいガレキを飛ばす
    for (int nCnt = 0; nCnt < MODEL_EFFECT_BLUE_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
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
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*m_collisionSize.x / 2 + pos.x,
            0.0f, -cosf(fAngle)*m_collisionSize.x / 2 + pos.z);

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

    // 青い炎を発生
    for (int nCnt = 0; nCnt < EFFECT_EX_NUMBER_ONCE; nCnt++)
    {
        // 速さを決める(Y軸は、上方向のみ)
        float fSpeedX = float(GetRandNum(EFFECT_EX_MAX_SPEED_LV4, 0)) / 100.0f - float(GetRandNum(EFFECT_EX_MAX_SPEED_LV4, 0)) / 100.0f;
        float fSpeedFireY = float(GetRandNum(EFFECT_EX_MAX_SPEED_Y, 0)) / 100.0f;
        float fSpeedZ = float(GetRandNum(EFFECT_EX_MAX_SPEED_LV4, 0)) / 100.0f - float(GetRandNum(EFFECT_EX_MAX_SPEED_LV4, 0)) / 100.0f;

        // 移動量を設定
        D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedFireY, fSpeedZ);

        // 赤と黄色の2色を出す
        D3DXCOLOR color = DEFAULT_COLOR;
        if (nCnt == 0)
        {
            color = EFFECT_BLUE_FIRE_FIRST_COLOR;
        }
        else
        {
            color = EFFECT_BLUE_FIRE_BLUE;
        }

        // 現在の位置から発生
        CEffect3D::Create(pos, m_collisionSize, move, color,
            DEFAULT_VECTOR, EFFECT_EX_FADE_OUT_RATE_LV4, CEffect3D::TYPE_BLUE_FIRE);
    }
}
