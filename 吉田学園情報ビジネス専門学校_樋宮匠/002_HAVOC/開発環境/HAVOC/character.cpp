//===============================================
//
// キャラクター処理 (character.cpp)
// Author : 樋宮匠
//
//===============================================
#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "animation.h"
#include "scene3d.h"
#include "library.h"
#include "game.h"

//========================================
// 静的メンバ変数宣言
//========================================

//=============================================================================
// デフォルトコンストラクタ
// Author : 樋宮匠
//=============================================================================
CCharacter::CCharacter()
{
    m_activeCollisionSize = DEFAULT_VECTOR;
    m_nLife = 0;
    m_posOld = DEFAULT_VECTOR;

    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_tribe = TRIBE_NONE;
    m_nPartsNum = 0;
    memset(m_apModel, 0, sizeof(m_apModel));
    memset(m_posDefault, 0, sizeof(m_posDefault));
    memset(m_nIndexParent, 0, sizeof(m_nIndexParent));

    m_pAnimation = NULL;
    m_pMinimap = NULL;
    m_cFilePass = NULL;

    for (int nCnt = 0; nCnt < CHARCTER_COLLISION_MAX; nCnt++)
    {
        m_aCollision[nCnt].pos = DEFAULT_VECTOR;
        m_aCollision[nCnt].size = DEFAULT_VECTOR;
    }
    m_nCntCollision = 0;

    m_lastHit = LAST_HIT_PLAYER;

    // アニメーションはデフォルトで使うことを設定
    m_bUseAnimation = true;

    // ボス仕様はデフォルトでfalse
    m_bBossSetting = false;
}

//=============================================================================
// オーバーライドされたコンストラクタ
// Author : 樋宮匠
//=============================================================================
CCharacter::CCharacter(OBJTYPE objtype) :CScene(objtype)
{
    m_activeCollisionSize = DEFAULT_VECTOR;
    m_nLife = 0;
    m_posOld = DEFAULT_VECTOR;

    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_tribe = TRIBE_NONE;
    m_nPartsNum = 0;
    memset(m_apModel, 0, sizeof(m_apModel));
    memset(m_posDefault, 0, sizeof(m_posDefault));
    memset(m_nIndexParent, 0, sizeof(m_nIndexParent));

    m_pAnimation = NULL;
    m_pMinimap = NULL;
    m_cFilePass = NULL;

    for (int nCnt = 0; nCnt < CHARCTER_COLLISION_MAX; nCnt++)
    {
        m_aCollision[nCnt].pos = DEFAULT_VECTOR;
        m_aCollision[nCnt].size = DEFAULT_VECTOR;
    }
    m_nCntCollision = 0;

    m_lastHit = LAST_HIT_PLAYER;

    // アニメーションはデフォルトで使うことを設定
    m_bUseAnimation = true;

    // ボス仕様はデフォルトでfalse
    m_bBossSetting = false;
}

//=============================================================================
// デストラクタ
// Author : 樋宮匠
//=============================================================================
CCharacter::~CCharacter()
{
}

//=============================================================================
// 初期化処理
// Author : 樋宮匠
//=============================================================================
HRESULT CCharacter::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 位置を結びつける
    m_pos = pos;
    m_posOld = pos;

    // モデルのポインタ配列の初期化
    memset(m_apModel, 0, sizeof(m_apModel));

    // モデルをパーツ分メモリ確保し、初期化
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        m_apModel[nCount] = new CModel;
        m_apModel[nCount]->Init(DEFAULT_VECTOR, DEFAULT_VECTOR);
        m_apModel[nCount]->SetScale(size);
        m_apModel[nCount]->BindMesh(m_pMesh[m_tribe][nCount], m_pBuffMat[m_tribe][nCount], m_nNumMat[m_tribe][nCount]);
        m_apModel[nCount]->SetPos(m_posDefault[nCount]);
    }

    // アニメーションのロード
    m_pAnimation = CAnimation::Create(this, m_nPartsNum, (CAnimation::ANIM_TYPE)m_tribe, m_cFilePass);

    // 敵なら、ミニマップ上にアイコンを生成
    if (m_tribe != TRIBE_PLAYER)
    {
        // 防衛軍またはワイバーンならミッションのターゲットなので黄色
        if (m_tribe == TRIBE_FORCE || m_tribe == TRIBE_WYVERN)
        {
            m_pMinimap = CMinimap::Create(CMinimap::ICON_TARGET);
        }
        else
        {
            // UFOはステージ2でミッションのターゲットなので黄色
            if (m_tribe == TRIBE_UFO && CGame::GetStage() == CGame::STAGE_2)
            {
                m_pMinimap = CMinimap::Create(CMinimap::ICON_TARGET);
            }
            else
            {
                m_pMinimap = CMinimap::Create(CMinimap::ICON_ENEMY);
            }
        }
    }

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 樋宮匠
//=============================================================================
void CCharacter::Uninit(void)
{
    // モデルをパーツ分、終了し、メモリ開放
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        // 中身があるなら
        if (m_apModel[nCount] != NULL)
        {
            // 終了処理
            m_apModel[nCount]->Uninit();

            // メモリ開放
            delete m_apModel[nCount];
            m_apModel[nCount] = NULL;
        }
    }

    // アニメーション情報を終了し、メモリ開放
    if (m_pAnimation != NULL)
    {
        // 終了処理
        m_pAnimation->Uninit();

        // メモリ開放
        delete m_pAnimation;
        m_pAnimation = NULL;
    }

    // マップアイコンを使用しない
    if (m_pMinimap != NULL)
    {
        m_pMinimap->UnableIcon();
    }

    Release();
}

//=============================================================================
// 更新処理
// Author : 樋宮匠
//=============================================================================
void CCharacter::Update(void)
{
    // アニメーションを使うフラグがtrueならアニメーションさせる
    if (m_bUseAnimation)
    {
        m_pAnimation->Update();
    }

    // ミニマップアイコンの情報更新
    if (m_pMinimap != NULL)
    {
        m_pMinimap->SetFieldPos(m_pos);
    }
}

//=============================================================================
// 描画処理
// Author : 樋宮匠
//=============================================================================
void CCharacter::Draw(void)
{
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        if (nCount == 0)
        {
            m_apModel[nCount]->Draw(m_pos, m_rot);
        }
        else
        {
            m_apModel[nCount]->Draw(m_apModel[m_nIndexParent[nCount]]);
        }
    }
}

//=============================================================================
// モデルデータ（座標,親子関係）読み込み処理
// Author : 樋宮匠
//=============================================================================
HRESULT CCharacter::LoadModelData(char* cFilePass)
{
    // 名前を記憶
    m_cFilePass = cFilePass;

    // ファイルポイント
    FILE *pFile;

    // 変数宣言
    int  nCntLoad = 0;
    char cReedText[128];	// 文字として読み取り用
    char cHeadText[256];	//
    char cDie[128];
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // ファイル開
    pFile = fopen(cFilePass, "r");

    // 開けた
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "CHARACTERSET") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "CHARACTERSET") == 0)
        {
            while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "\n") == 0)
                {
                }
                else if (strcmp(cHeadText, "PARTSSET") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTSSET") != 0)
                    {

                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "INDEX") == 0)
                        {
                            int nNum;
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nNum);
                        }

                        if (strcmp(cHeadText, "PARENT") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_nIndexParent[nCntLoad]);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos.x, &pos.y, &pos.z);

                            m_posDefault[nCntLoad] = pos;
                        }

                        if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot.x, &rot.y, &rot.z);
                        }
                    }
                    nCntLoad++;
                }
            }
        }
        // ファイル閉
        fclose(pFile);
    }

    // 開けない
    else
    {
        printf("開けれませんでした\n");
    }
    return E_NOTIMPL;
}

//=============================================================================
// メッシュ割り当て処理
// 第1引数		: パーツに割り振られた番号
// 第2引数以降	: メッシュのデータ
// Author : 樋宮匠
//=============================================================================
void CCharacter::BindMesh(TRIBE type, int nPartIndex, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat)
{
    m_tribe = type;
    m_pMesh[type][nPartIndex] = pMesh;
    m_pBuffMat[type][nPartIndex] = pBuffMat;
    m_nNumMat[type][nPartIndex] = nNumMat;
}