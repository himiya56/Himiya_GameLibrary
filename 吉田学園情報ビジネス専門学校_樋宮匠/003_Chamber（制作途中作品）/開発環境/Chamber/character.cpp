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
    m_nPartsNum = 0;
    memset(m_apModel, 0, sizeof(m_apModel));
    memset(m_posDefault, 0, sizeof(m_posDefault));
    memset(m_nIndexParent, 0, sizeof(m_nIndexParent));

    m_pAnimation = NULL;
    m_cFilePass = NULL;

    for (int nCnt = 0; nCnt < CHARCTER_COLLISION_MAX; nCnt++)
    {
        m_aCollision[nCnt].pos = DEFAULT_VECTOR;
        m_aCollision[nCnt].size = DEFAULT_VECTOR;
    }
    m_nCntCollision = 0;

    // アニメーションはデフォルトで使うことを設定
    m_bUseAnimation = true;
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
HRESULT CCharacter::Init(void)
{
    // モデルのポインタ配列の初期化
    memset(m_apModel, 0, sizeof(m_apModel));

    // モデルをパーツ分メモリ確保し、初期化
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        m_apModel[nCount] = new CModel;
        m_apModel[nCount]->Init(DEFAULT_VECTOR, DEFAULT_VECTOR);
        m_apModel[nCount]->SetScale(DEFAULT_SCALE);
        m_apModel[nCount]->BindMesh(m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
        m_apModel[nCount]->SetPos(m_posDefault[nCount]);
    }

    // アニメーションのロード
    m_pAnimation = CAnimation::Create(this, m_nPartsNum, (CAnimation::ANIM_TYPE)m_AnimType, m_cFilePass);

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
void CCharacter::BindMesh(CAnimation::ANIM_TYPE type, int nPartIndex, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat)
{
	m_AnimType = type;
    m_pMesh[nPartIndex] = pMesh;
    m_pBuffMat[nPartIndex] = pBuffMat;
    m_nNumMat[nPartIndex] = nNumMat;
}