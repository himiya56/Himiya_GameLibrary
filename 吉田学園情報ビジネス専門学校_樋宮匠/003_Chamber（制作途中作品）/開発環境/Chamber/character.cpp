//===============================================
//
// �L�����N�^�[���� (character.cpp)
// Author : ��{��
//
//===============================================
#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "animation.h"
#include "scene3d.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
// Author : ��{��
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

    // �A�j���[�V�����̓f�t�H���g�Ŏg�����Ƃ�ݒ�
    m_bUseAnimation = true;
}

//=============================================================================
// �f�X�g���N�^
// Author : ��{��
//=============================================================================
CCharacter::~CCharacter()
{
}

//=============================================================================
// ����������
// Author : ��{��
//=============================================================================
HRESULT CCharacter::Init(void)
{
    // ���f���̃|�C���^�z��̏�����
    memset(m_apModel, 0, sizeof(m_apModel));

    // ���f�����p�[�c���������m�ۂ��A������
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        m_apModel[nCount] = new CModel;
        m_apModel[nCount]->Init(DEFAULT_VECTOR, DEFAULT_VECTOR);
        m_apModel[nCount]->SetScale(DEFAULT_SCALE);
        m_apModel[nCount]->BindMesh(m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
        m_apModel[nCount]->SetPos(m_posDefault[nCount]);
    }

    // �A�j���[�V�����̃��[�h
    m_pAnimation = CAnimation::Create(this, m_nPartsNum, (CAnimation::ANIM_TYPE)m_AnimType, m_cFilePass);

    return S_OK;
}

//=============================================================================
// �I������
// Author : ��{��
//=============================================================================
void CCharacter::Uninit(void)
{
    // ���f�����p�[�c���A�I�����A�������J��
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        // ���g������Ȃ�
        if (m_apModel[nCount] != NULL)
        {
            // �I������
            m_apModel[nCount]->Uninit();

            // �������J��
            delete m_apModel[nCount];
            m_apModel[nCount] = NULL;
        }
    }

    // �A�j���[�V���������I�����A�������J��
	if (m_pAnimation != NULL)
	{
		// �I������
		m_pAnimation->Uninit();

		// �������J��
		delete m_pAnimation;
		m_pAnimation = NULL;
	}

    Release();
}

//=============================================================================
// �X�V����
// Author : ��{��
//=============================================================================
void CCharacter::Update(void)
{
    // �A�j���[�V�������g���t���O��true�Ȃ�A�j���[�V����������
    if (m_bUseAnimation)
    {
        m_pAnimation->Update();
    }
}

//=============================================================================
// �`�揈��
// Author : ��{��
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
// ���f���f�[�^�i���W,�e�q�֌W�j�ǂݍ��ݏ���
// Author : ��{��
//=============================================================================
HRESULT CCharacter::LoadModelData(char* cFilePass)
{
    // ���O���L��
    m_cFilePass = cFilePass;

    // �t�@�C���|�C���g
    FILE *pFile;

    // �ϐ��錾
    int  nCntLoad = 0;
    char cReedText[128];	// �����Ƃ��ēǂݎ��p
    char cHeadText[256];	//
    char cDie[128];
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // �t�@�C���J
    pFile = fopen(cFilePass, "r");

    // �J����
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
        // �t�@�C����
        fclose(pFile);
    }

    // �J���Ȃ�
    else
    {
        printf("�J����܂���ł���\n");
    }
    return E_NOTIMPL;
}

//=============================================================================
// ���b�V�����蓖�ď���
// ��1����		: �p�[�c�Ɋ���U��ꂽ�ԍ�
// ��2�����ȍ~	: ���b�V���̃f�[�^
// Author : ��{��
//=============================================================================
void CCharacter::BindMesh(CAnimation::ANIM_TYPE type, int nPartIndex, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat)
{
	m_AnimType = type;
    m_pMesh[nPartIndex] = pMesh;
    m_pBuffMat[nPartIndex] = pBuffMat;
    m_nNumMat[nPartIndex] = nNumMat;
}