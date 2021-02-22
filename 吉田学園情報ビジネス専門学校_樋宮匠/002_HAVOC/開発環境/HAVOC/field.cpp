//===============================================
//
// �t�B�[���h���� (field.cpp)
// Author : ��{��
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "field.h"
#include "manager.h"
#include "renderer.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================

//=============================================================================
// �R���X�g���N�^
// Author : ��{��
//=============================================================================
CField::CField() :CScene(CScene::OBJTYPE_FIELD)
{
    for (int nCountHeight = 0; nCountHeight < MAX_TILE_LINE; nCountHeight++)
    {
        for (int nCountWidth = 0; nCountWidth < MAX_TILE_LINE; nCountWidth++)
        {
            m_apTile[nCountHeight][nCountWidth] = NULL;
            m_aGround[nCountHeight][nCountWidth] = 0;
        }
    }
}

//=============================================================================
// �f�X�g���N�^
// Author : ��{��
//=============================================================================
CField::~CField()
{
}

//=============================================================================
// ����������
// Author : ��{��
//=============================================================================
HRESULT CField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    return S_OK;
}

//=============================================================================
// �I������
// Author : ��{��
//=============================================================================
void CField::Uninit(void)
{
    for (int nCountHeight = 0; nCountHeight < MAX_TILE_LINE; nCountHeight++)
    {
        for (int nCountWidth = 0; nCountWidth < MAX_TILE_LINE; nCountWidth++)
        {
            // ���g������Ȃ�
            if (m_apTile[nCountHeight][nCountWidth] != NULL)
            {
                // �I������
                m_apTile[nCountHeight][nCountWidth]->Uninit();

                // �������J��
                delete m_apTile[nCountHeight][nCountWidth];
                m_apTile[nCountHeight][nCountWidth] = NULL;
            }
        }
    }

    // �I�u�W�F�N�g�̔j��
    Release();
}

//=============================================================================
// �X�V����
// Author : ��{��
//=============================================================================
void CField::Update(void)
{
}

//=============================================================================
// �`�揈��
// Author : ��{��
//=============================================================================
void CField::Draw(void)
{
    for (int nCountHeight = 0; nCountHeight < MAX_TILE_LINE; nCountHeight++)
    {
        for (int nCountWidth = 0; nCountWidth < MAX_TILE_LINE; nCountWidth++)
        {
            // ���g������Ȃ�
            if (m_apTile[nCountHeight][nCountWidth] != NULL)
            {
                m_apTile[nCountHeight][nCountWidth]->Draw();
            }
        }
    }
}

//=============================================================================
// �}�b�v�f�[�^�ǂݍ��ݏ���
// Author : ��{��
//=============================================================================
void CField::LoadMapData(CGame::STAGE stage, int nWidth, int nDepth)
{
    FILE *pFile = NULL;
    int nLoadHeight = 0;
    int nLoadWidth = 0;

    // �X�e�[�W�ɂ���āA�t�@�C�����J��
    switch (stage)
    {
    case CGame::STAGE_1:
        pFile = fopen("./data/MAPS/Map_001.csv", "r");
        break;
    case CGame::STAGE_2:
        pFile = fopen("./data/MAPS/Map_002.csv", "r");
        break;
    case CGame::STAGE_FINAL:
        pFile = fopen("./data/MAPS/Map_003.csv", "r");
        break;
    }

    // �J����
    if (pFile != NULL)
    {
        for (int nCountMap = 0; nCountMap < nWidth * nDepth; nCountMap++)
        {
            while (fscanf(pFile, "%d,", &m_aGround[nLoadHeight][nLoadWidth]) != EOF)
            {
                nLoadWidth++;
                if (nLoadWidth >= nDepth)
                {
                    nLoadWidth = 0;
                    nLoadHeight++;
                }
            }
        }

        fclose(pFile);
    }
    // �J���Ȃ�
    else
    {
        printf("�J����܂���ł���\n");
    }
}

//=============================================================================
// �C���X�^���X��������
// Author : ��{��
//=============================================================================
CField * CField::Create(CGame::STAGE stage, int nWidth, int nDepth)
{
    CField *m_pField;

    m_pField = new CField;
    m_pField->Init(DEFAULT_VECTOR, DEFAULT_SCALE);

    // �t�B�[���h�ǂݍ���
    m_pField->LoadMapData(stage, nWidth, nDepth);

    // �t�B�[���h�ݒu
    m_pField->SetField(nWidth, nDepth);

    return m_pField;
}

//=============================================================================
// �t�B�[���h��������
// Author : ��{��
//=============================================================================
void CField::SetField(int nWidth, int nDepth)
{
    int nHeight = 0;

    for (int nCountHeight = nDepth - 1; nCountHeight > -1; nCountHeight--)
    {
        for (int nCountWidth = 0; nCountWidth < nWidth; nCountWidth++)
        {
            switch (m_aGround[nHeight][nCountWidth])
            {
            case CTile::TILE_BLACK:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_BLACK);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            case CTile::TILE_WHITE:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_WHITE);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            case CTile::TILE_RED:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_RED);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            case CTile::TILE_GREENERY:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_GREENERY);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            case CTile::TILE_SIDEWALK:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_SIDEWALK);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            case CTile::TILE_CHUOLINE:
                m_apTile[nCountHeight][nCountWidth] = new CTile;
                m_apTile[nCountHeight][nCountWidth]->Init(CTile::TILE_CHUOLINE);
                m_apTile[nCountHeight][nCountWidth]->SetPos(D3DXVECTOR3(
                    nCountWidth * TILE_SIZE / 2, 0.0f, nCountHeight * TILE_SIZE / 2));
                m_apTile[nCountHeight][nCountWidth]->SetCol(
                    D3DCOLOR_RGBA(255, 255, 255, 255));
                m_apTile[nCountHeight][nCountWidth]->Update();
                break;
            default:
                break;
            }
        }

        nHeight++;
    }
}
