//===============================================
//
// �u���b�N���� (block.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "debug.h"
#include "game.h"
#include "score.h"
#include "effect3d.h"
#include "library.h"
#include "modelEffect.h"
#include "bullet.h"
#include "item.h"
#include "sound.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
LPD3DXMESH	CBlock::m_apMesh[CBlock::TYPE_MAX] = {};
LPD3DXBUFFER CBlock::m_apBuffMat[CBlock::TYPE_MAX] = {};
DWORD CBlock::m_aNumMat[CBlock::TYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CBlock::m_apTexture[CBlock::TYPE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBlock::CBlock() :CScene3D(CScene::OBJTYPE_BLOCK)
{
    m_collisionSize = DEFAULT_VECTOR;
    m_nType = TYPE_BUILDING001;
    m_bWallRun = false;
    m_bEnemyWallRan = false;

    m_nLife = 0;
    m_nScore = 0;
    m_nOrbs = 0;
    m_bUseDestroyEffect = false;

    m_bUseExplosion = false;

    m_lastHit = LAST_HIT_PLAYER;

    m_pMinimap = NULL;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBlock::~CBlock()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CBlock::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene3D::Init(pos, size);

    // �X�P�[����ݒ�
    SetScale(size);

    // ���b�V���A�e�N�X�`�����o�C���h
    BindMesh(m_apMesh[m_nType], m_apBuffMat[m_nType], m_aNumMat[m_nType]);
    BindTexture(m_apTexture[m_nType]);

    // �N���X�^���Ȃ�
    if (m_nType == TYPE_CRYSTAL)
    {
        // �~�j�}�b�v��Ƀ^�[�Q�b�g�A�C�R���𐶐�
        m_pMinimap = CMinimap::Create(CMinimap::ICON_TARGET);
    }

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CBlock::Uninit(void)
{
    // �~�j�}�b�v�A�C�R�����g�p���Ȃ�
    if (m_pMinimap != NULL)
    {
        m_pMinimap->UnableIcon();
    }

    CScene3D::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CBlock::Update(void)
{
    // �~�j�}�b�v�A�C�R���̏��X�V
    if (m_pMinimap != NULL)
    {
        m_pMinimap->SetFieldPos(GetPos());
    }

    // �̗͂�0�ȉ��Ȃ�A�X�R�A���Z���A���g�p��
    if (m_nLife <= 0)
    {
        // �|��G�t�F�N�g
        if (m_bUseDestroyEffect == true)
        {
            DestroyEffect();
        }

        // �����G�t�F�N�g
        if (m_bUseExplosion == true)
        {
            ExplosionEffect();
        }

        // ���ɓ|���G�t�F�N�g
        FallDownModel();

        // �~�b�V�����N���A���ɉ��Z����
        AddMissionClear();

        // ���X�g�q�b�g���v���C���[�Ȃ�
        if (m_lastHit == LAST_HIT_PLAYER)
        {
            // �I�[�u���o���܂�
            ScatterOrbs();

            // �X�R�A���Z
            CScore *pScore = CGame::GetScore();
            pScore->AddScore(m_nScore);
        }

        // �N���X�^���Ȃ�
        if (m_nType == TYPE_CRYSTAL)
        {
            // �񕜃A�C�e������
            CItem::Create(GetPos(), DEFAULT_VECTOR, CItem::TYPE_HEAL);
        }

        // �I������
        Uninit();
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CBlock::Draw(void)
{
    CScene3D::Draw();
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
HRESULT CBlock::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //==============================
    // �r��001
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building001.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, 
        &m_apBuffMat[TYPE_BUILDING001], NULL, &m_aNumMat[TYPE_BUILDING001], &m_apMesh[TYPE_BUILDING001]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building001.jpg", &m_apTexture[TYPE_BUILDING001]);

    //==============================
    // �r��101
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building101.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BUILDING101], NULL, &m_aNumMat[TYPE_BUILDING101], &m_apMesh[TYPE_BUILDING101]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building101.jpg", &m_apTexture[TYPE_BUILDING101]);

    //==============================
    // �r��201
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building201.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BUILDING201], NULL, &m_aNumMat[TYPE_BUILDING201], &m_apMesh[TYPE_BUILDING201]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building201.jpg", &m_apTexture[TYPE_BUILDING201]);

    //==============================
    // �r��301
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building301.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BUILDING301], NULL, &m_aNumMat[TYPE_BUILDING301], &m_apMesh[TYPE_BUILDING301]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building301.jpg", &m_apTexture[TYPE_BUILDING301]);

    //==============================
    // �r��401
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building401.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BUILDING401], NULL, &m_aNumMat[TYPE_BUILDING401], &m_apMesh[TYPE_BUILDING401]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building401.jpg", &m_apTexture[TYPE_BUILDING401]);

    //==============================
    // �r��501
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/building/building501.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BUILDING501], NULL, &m_aNumMat[TYPE_BUILDING501], &m_apMesh[TYPE_BUILDING501]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building501.jpg", &m_apTexture[TYPE_BUILDING501]);

    //==============================
    // �M���@
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Signal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGNAL], NULL, &m_aNumMat[TYPE_SIGNAL], &m_apMesh[TYPE_SIGNAL]);

    //==============================
    // �X��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/light.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LIGHT], NULL, &m_aNumMat[TYPE_LIGHT], &m_apMesh[TYPE_LIGHT]);

    //==============================
    // �X��2
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/light2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_LIGHT2], NULL, &m_aNumMat[TYPE_LIGHT2], &m_apMesh[TYPE_LIGHT2]);

    //==============================
    // �S�~��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/DustBox.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_DUSTBOX], NULL, &m_aNumMat[TYPE_DUSTBOX], &m_apMesh[TYPE_DUSTBOX]);

    //==============================
    // ��1
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood1.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD1], NULL, &m_aNumMat[TYPE_WOOD1], &m_apMesh[TYPE_WOOD1]);

    //==============================
    // ��2
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood2.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD2], NULL, &m_aNumMat[TYPE_WOOD2], &m_apMesh[TYPE_WOOD2]);

    //==============================
    // ��3
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/wood3.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_WOOD3], NULL, &m_aNumMat[TYPE_WOOD3], &m_apMesh[TYPE_WOOD3]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ki.jpg", &m_apTexture[TYPE_WOOD3]);

    //==============================
    // �x���`
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/bench.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BENCH], NULL, &m_aNumMat[TYPE_BENCH], &m_apMesh[TYPE_BENCH]);

    //==============================
    // ��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/tukue.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_TUKUE], NULL, &m_aNumMat[TYPE_TUKUE], &m_apMesh[TYPE_TUKUE]);

    //==============================
    // �K�[�h���[��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/guardrail.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_GUARDRAIL], NULL, &m_aNumMat[TYPE_GUARDRAIL], &m_apMesh[TYPE_GUARDRAIL]);

    //==============================
    // �W�����O���W��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/playground.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_PLAYGROUND], NULL, &m_aNumMat[TYPE_PLAYGROUND], &m_apMesh[TYPE_PLAYGROUND]);

    //==============================
    // ���ӃT�C��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signEmar.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_EMAR], NULL, &m_aNumMat[TYPE_SIGN_EMAR], &m_apMesh[TYPE_SIGN_EMAR]);

    //==============================
    // �\���H�T�C��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signInter.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_INTER], NULL, &m_aNumMat[TYPE_SIGN_INTER], &m_apMesh[TYPE_SIGN_INTER]);

    //==============================
    // ���ԋ֎~�T�C��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/signNoPark.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SIGN_NO_PARK], NULL, &m_aNumMat[TYPE_SIGN_NO_PARK], &m_apMesh[TYPE_SIGN_NO_PARK]);

    //==============================
    // ��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/car.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CAR], NULL, &m_aNumMat[TYPE_CAR], &m_apMesh[TYPE_CAR]);

    //==============================
    // �R���r�j
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/convini.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CONVINI], NULL, &m_aNumMat[TYPE_CONVINI], &m_apMesh[TYPE_CONVINI]);

    //==============================
    // �R���e�i
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/container.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CONTAINER], NULL, &m_aNumMat[TYPE_CONTAINER], &m_apMesh[TYPE_CONTAINER]);

    //==============================
    // �K�X�^���N
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/gas.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_GAS], NULL, &m_aNumMat[TYPE_GAS], &m_apMesh[TYPE_GAS]);

    //==============================
    // �q��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/souko.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SOUKO], NULL, &m_aNumMat[TYPE_SOUKO], &m_apMesh[TYPE_SOUKO]);

    //==============================
    // �S��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/metal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_METAL], NULL, &m_aNumMat[TYPE_METAL], &m_apMesh[TYPE_METAL]);

    //==============================
    // �\�[���[�p�l��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/solar.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SOLAR], NULL, &m_aNumMat[TYPE_SOLAR], &m_apMesh[TYPE_SOLAR]);
    // �e�N�X�`��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/SolarPanel.jpg", &m_apTexture[TYPE_SOLAR]);

    //==============================
    // �^���[
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/tower.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_TOWER], NULL, &m_aNumMat[TYPE_TOWER], &m_apMesh[TYPE_TOWER]);

    //==============================
    // �N���X�^��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/crystal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CRYSTAL], NULL, &m_aNumMat[TYPE_CRYSTAL], &m_apMesh[TYPE_CRYSTAL]);

    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : �㓡�T�V��
//=============================================================================
void CBlock::Unload(void)
{
    for (int nCnt = 0; nCnt < CBlock::TYPE_MAX; nCnt++)
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
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CBlock * CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
    // �|�C���^��p��
    CBlock *pBlock = NULL;

    // �������m��
    pBlock = new CBlock;

    // �^�C�v��R�Â�(�������̒���Bind������̂����邽�߁A�ŏ��ɕR�Â�)
    pBlock->m_nType = nType;

    // ������
    pBlock->Init(pos, DEFAULT_SCALE);

    // ��]�����т���
    pBlock->SetRot(rot);

    // ��ނ��ƂɁA���т���l��ς���
    switch (nType)
    {
    case TYPE_BUILDING001:
        
        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;
        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_BUILDING101:

        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;
        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_BUILDING201:

        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;
        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_BUILDING301:

        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;
        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_BUILDING401:

        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;
        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_BUILDING501:

        pBlock->m_collisionSize = BLOCK_BUILDING_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;
        pBlock->m_nLife = BLOCK_BUILDING_LIFE;
        pBlock->m_nScore = BLOCK_BUILDING_SCORE;
        pBlock->m_nOrbs = BLOCK_BUILDING_ORBS;
        pBlock->m_bUseDestroyEffect = true;

        break;

    case TYPE_SIGNAL:

        pBlock->m_collisionSize = BLOCK_SIGNAL_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_SIGNAL_LIFE;
        pBlock->m_nScore = BLOCK_SIGNAL_SCORE;
        pBlock->m_nOrbs = BLOCK_SIGNAL_ORBS;

        break;

    case TYPE_LIGHT:

        pBlock->m_collisionSize = BLOCK_LIGHT_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_LIGHT_LIFE;
        pBlock->m_nScore = BLOCK_LIGHT_SCORE;
        pBlock->m_nOrbs = BLOCK_LIGHT_ORBS;

        break;

    case TYPE_LIGHT2:

        pBlock->m_collisionSize = BLOCK_LIGHT_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_LIGHT_LIFE;
        pBlock->m_nScore = BLOCK_LIGHT_SCORE;
        pBlock->m_nOrbs = BLOCK_LIGHT_ORBS;

        break;

    case TYPE_DUSTBOX:

        pBlock->m_collisionSize = BLOCK_DUSTBOX_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_DUSTBOX_LIFE;
        pBlock->m_nScore = BLOCK_DUSTBOX_SCORE;
        pBlock->m_nOrbs = BLOCK_DUSTBOX_ORBS;

        break;

    case TYPE_WOOD1:

        pBlock->m_collisionSize = BLOCK_WOOD_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_WOOD_LIFE;
        pBlock->m_nScore = BLOCK_WOOD_SCORE;
        pBlock->m_nOrbs = BLOCK_WOOD_ORBS;

        break;

    case TYPE_WOOD2:

        pBlock->m_collisionSize = BLOCK_WOOD2_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_WOOD_LIFE;
        pBlock->m_nScore = BLOCK_WOOD_SCORE;
        pBlock->m_nOrbs = BLOCK_WOOD_ORBS;

        break;

    case TYPE_WOOD3:

        pBlock->m_collisionSize = BLOCK_WOOD_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_nLife = BLOCK_WOOD_LIFE;
        pBlock->m_nScore = BLOCK_WOOD_SCORE;
        pBlock->m_nOrbs = BLOCK_WOOD_ORBS;

        break;

    case TYPE_BENCH:

        // �c���������������ŁA�����蔻���ς���
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_BENCH_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_BENCH_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_BENCH_LIFE;
        pBlock->m_nScore = BLOCK_BENCH_SCORE;
        pBlock->m_nOrbs = BLOCK_BENCH_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_TUKUE:

        // �c���������������ŁA�����蔻���ς���
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_TUKUE_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_TUKUE_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_TUKUE_LIFE;
        pBlock->m_nScore = BLOCK_TUKUE_SCORE;
        pBlock->m_nOrbs = BLOCK_TUKUE_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_GUARDRAIL:

        // �c���������������ŁA�����蔻���ς���
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_GUARDRAIL_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_GUARDRAIL_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_GUARDRAIL_LIFE;
        pBlock->m_nScore = BLOCK_GUARDRAIL_SCORE;
        pBlock->m_nOrbs = BLOCK_GUARDRAIL_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_PLAYGROUND:

        pBlock->m_collisionSize = BLOCK_PLAYGROUND_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;
        pBlock->m_nLife = BLOCK_PLAYGROUND_LIFE;
        pBlock->m_nScore = BLOCK_PLAYGROUND_SCORE;
        pBlock->m_nOrbs = BLOCK_PLAYGROUND_ORBS;

        break;

    case TYPE_SIGN_EMAR:

        pBlock->m_collisionSize = BLOCK_SIGN_EMAR_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_SIGN_EMAR_LIFE;
        pBlock->m_nScore = BLOCK_SIGN_EMAR_SCORE;
        pBlock->m_nOrbs = BLOCK_SIGN_EMAR_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_SIGN_INTER:

        pBlock->m_collisionSize = BLOCK_SIGN_INTER_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_SIGN_INTER_LIFE;
        pBlock->m_nScore = BLOCK_SIGN_INTER_SCORE;
        pBlock->m_nOrbs = BLOCK_SIGN_INTER_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_SIGN_NO_PARK:

        pBlock->m_collisionSize = BLOCK_SIGN_NO_PARK_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_SIGN_NO_PARK_LIFE;
        pBlock->m_nScore = BLOCK_SIGN_NO_PARK_SCORE;
        pBlock->m_nOrbs = BLOCK_SIGN_NO_PARK_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_CAR:

        // �c���������������ŁA�����蔻���ς���
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CAR_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CAR_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_CAR_LIFE;
        pBlock->m_nScore = BLOCK_CAR_SCORE;
        pBlock->m_nOrbs = BLOCK_CAR_ORBS;

        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;

        pBlock->m_bUseExplosion = true;

        break;

    case TYPE_CONVINI:

        // �c���������������ŁA�����蔻���ς���
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CONVINI_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CONVINI_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_CONVINI_LIFE;
        pBlock->m_nScore = BLOCK_CONVINI_SCORE;
        pBlock->m_nOrbs = BLOCK_CONVINI_ORBS;

        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;

        pBlock->m_bUseDestroyEffect = true;

        break;

    case TYPE_CONTAINER:

        // �c���������������ŁA�����蔻���ς���
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CONTAINER_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_CONTAINER_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_CONTAINER_LIFE;
        pBlock->m_nScore = BLOCK_CONTAINER_SCORE;
        pBlock->m_nOrbs = BLOCK_CONTAINER_ORBS;

        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;

        pBlock->m_bUseDestroyEffect = true;

        break;

    case TYPE_GAS:

        pBlock->m_collisionSize = BLOCK_GAS_COLLISION_SIZE;
        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;
        pBlock->m_nLife = BLOCK_GAS_LIFE;
        pBlock->m_nScore = BLOCK_GAS_SCORE;
        pBlock->m_nOrbs = BLOCK_GAS_ORBS;
        pBlock->m_bUseDestroyEffect = true;

        pBlock->m_bUseExplosion = true;

        break;

    case TYPE_SOUKO:

        // �c���������������ŁA�����蔻���ς���
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_SOUKO_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_SOUKO_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_SOUKO_LIFE;
        pBlock->m_nScore = BLOCK_SOUKO_SCORE;
        pBlock->m_nOrbs = BLOCK_SOUKO_ORBS;

        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;

        pBlock->m_bUseDestroyEffect = true;

        break;

    case TYPE_METAL:

        pBlock->m_collisionSize = BLOCK_METAL_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_METAL_LIFE;
        pBlock->m_nScore = BLOCK_METAL_SCORE;
        pBlock->m_nOrbs = BLOCK_METAL_ORBS;

        pBlock->m_bWallRun = true;

        break;

    case TYPE_SOLAR:

        // �c���������������ŁA�����蔻���ς���
        if (BLOCK_HORIZONTAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_SOLAR_HORIZONTAL_COLLISION_SIZE;
        }
        else if (BLOCK_VERTICAL_COLLISION(rot.y))
        {
            pBlock->m_collisionSize = BLOCK_SOLAR_VERTICAL_COLLISION_SIZE;
        }

        pBlock->m_nLife = BLOCK_SOLAR_LIFE;
        pBlock->m_nScore = BLOCK_SOLAR_SCORE;
        pBlock->m_nOrbs = BLOCK_SOLAR_ORBS;

        pBlock->m_bWallRun = true;
        pBlock->m_bEnemyWallRan = true;

        pBlock->m_bUseDestroyEffect = true;

        break;

    case TYPE_TOWER:

        pBlock->m_collisionSize = BLOCK_TOWER_COLLISION_SIZE;
        pBlock->m_nLife = BLOCK_TOWER_LIFE;
        pBlock->m_nScore = BLOCK_TOWER_SCORE;
        pBlock->m_nOrbs = BLOCK_TOWER_ORBS;

        pBlock->m_bEnemyWallRan = true;

        break;

    case TYPE_CRYSTAL:

        pBlock->m_collisionSize = BLOCK_CRYSTAL_COLLISION_SIZE;
        pBlock->m_bEnemyWallRan = true;
        pBlock->m_nLife = BLOCK_CRYSTAL_LIFE;
        pBlock->m_nScore = BLOCK_CRYSTAL_SCORE;
        pBlock->m_nOrbs = BLOCK_CRYSTAL_ORBS;

        break;
    }

#ifdef COLLISION_TEST
    CDebug::Create(pos, pBlock->m_collisionSize, CDebug::TYPE_PERMANENT);
#endif #endif // COLLISION_TEST

    return pBlock;
}

//=============================================================================
// �I�[�u���o���܂�����
// Author : �㓡�T�V��
//=============================================================================
void CBlock::ScatterOrbs(void)
{
    // �ʒu���擾
    D3DXVECTOR3 pos = GetPos();

    // �I�[�u��U�G�t�F�N�g
    float fSpeedX = 0.0f;   //����X
    float fSpeedY = 0.0f;   //����Y
    float fSpeedZ = 0.0f;   //����Z
    int nPlusMinusX = 0;    //����X
    int nPlusMinusZ = 0;    //����Z
    for (int nCnt = 0; nCnt < m_nOrbs; nCnt++)
    {
        // �����ƁA�����������_���Ɍ��߂�
        fSpeedX = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_X, 0)) / 100.0f;
        fSpeedY = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Y, EFFECT_CHARGE_SPREAD_MIN_Y)) / 100.0f;
        fSpeedZ = float(GetRandNum(EFFECT_CHARGE_SPREAD_MAX_Z, 0)) / 100.0f;
        nPlusMinusX = GetRandNum(2, 1);
        nPlusMinusZ = GetRandNum(2, 1);
        if (nPlusMinusX == 1)
        {
            nPlusMinusX = 1;
        }
        else
        {
            nPlusMinusX = -1;
        }

        if (nPlusMinusZ == 1)
        {
            nPlusMinusZ = 1;
        }
        else
        {
            nPlusMinusZ = -1;
        }

        // �ړ��ʂ����߂�
        D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY, fSpeedZ*nPlusMinusZ);

        // �G�t�F�N�g����(Y����n�ʂŌŒ�)
        CEffect3D::Create(D3DXVECTOR3(pos.x, 0.0f, pos.z), EFFECT_CHARGE_SIZE, move,
            EFFECT3D_PURPLE_FIRE_FIRST_COLOR, DEFAULT_VECTOR, EFFECT3D_PURPLE_FIRE_FADE_OUT_RATE, CEffect3D::TYPE_CHARGE);
    }
}

//=============================================================================
// �|��G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBlock::DestroyEffect(void)
{
    // �|��G�t�F�N�g�p�̕ϐ��錾
    float fAngle = 0.0f;                       // �p�x
    float fRadius = 0.0f;                      // ���a
    float fSpeed = 0.0f;                       // ����
    float fSize = 0.0f;                        // �傫��
    for (int nCnt = 0; nCnt < EFFECT_DESTROY_NUMBER_ONCE; nCnt++)
    {
        // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // ���a���o��
        int nAverageSize = ((int)m_collisionSize.x + (int)m_collisionSize.z) / 2;
        fRadius = float(GetRandNum(nAverageSize * MODEL_EFFECT_BIG_COLLAPSE_RADIUS, 0)) / 100.0f;

        // �������o��
        fSpeed = float(GetRandNum(EFFECT_DESTROY_MAX_SPEED, EFFECT_DESTROY_MIN_SPEED)) / 100.0f;

        // �ړ��ʂ����߂�
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeed, 0.0f);

        // �傫�����o��
        fSize = float(GetRandNum(EFFECT_DESTROY_MAX_SIZE, EFFECT_DESTROY_MIN_SIZE)) / 100.0f;

        // �傫�������߂�
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // �ʒu�����߂�(Y����n�ʂ��甼�����������ʒu�ŌŒ�)
        D3DXVECTOR3 pos = D3DXVECTOR3(-sinf(fAngle)*fRadius + GetPos().x, fSize / 2, -cosf(fAngle)*fRadius + GetPos().z);

        // �G�t�F�N�g����
        CEffect3D::Create(pos, size, move, DEFAULT_COLOR,
            DEFAULT_VECTOR, EFFECT_DESTROY_FADE_OUT_RATE, CEffect3D::TYPE_SMOKE);
    }

    // ����G�t�F�N�g
    CollapseEffect();
}

//=============================================================================
// �����G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBlock::ExplosionEffect(void)
{
    // �ʒu���擾
    D3DXVECTOR3 pos = GetPos();

    // �������K���L���΂�
    for (int nCnt = 0; nCnt < MODEL_EFFECT_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE_LV2; nCnt++)
    {
        // �ϐ��錾
        float fSpeedY = float(GetRandNum(MODEL_EFFECT_EXPLOSION_SPEED_Y_MAX, MODEL_EFFECT_EXPLOSION_SPEED_Y_MIN)) / 100.0f;
        float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

        // �ړ��ʂ����߂�
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f,
            fSpeedY, 0.0f);

        // ��]�ʂ����߂�
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
        float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // X��Z�̒��Ԃ̑傫���𓾂�
        float fAverageSize = (m_collisionSize.x + m_collisionSize.z) / 2;

        // �ʒu�����߂�
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*fAverageSize,
            0.0f, -cosf(fAngle)*fAverageSize) + pos;

        // ��ނ����߂�
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

        // �G�t�F�N�g����
        CModelEffect::Create(effectPos, rotMove, move, rotMove, modelEffectType);
    }

    // ��������
    D3DXVECTOR3 size = D3DXVECTOR3(BULLET_EXPLOSION_COLLISION_SIZE_LV2, BULLET_EXPLOSION_COLLISION_SIZE_LV2, BULLET_EXPLOSION_COLLISION_SIZE_LV2);

    // ���X�g�q�b�g���v���C���[�Ȃ�
    if (m_lastHit == LAST_HIT_PLAYER)
    {
        // �푰���v���C���[��
        CBullet::Create(pos, size, DEFAULT_VECTOR,
            CBullet::TYPE_EXPLOSION, CBullet::TRIBE_PLAYER);
    }
    // ����ȊO�͎푰��G��
    else
    {
        CBullet::Create(pos, size, DEFAULT_VECTOR,
            CBullet::TYPE_EXPLOSION, CBullet::TRIBE_ENEMY);
    }
}

//=============================================================================
// ����G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBlock::CollapseEffect(void)
{
    // �K���L������鉹�Đ�
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_DESTROY_RUBBLE);

    for (int nCnt = 0; nCnt < MODEL_EFFECT_COLLAPSE_NUMBER_ONCE; nCnt++)
    {
        // ����G�t�F�N�g�p�̕ϐ��錾
        float fPosX = 0.0f;                 // �ʒuX
        float fPosZ = 0.0f;                 // �ʒuZ
        float fRotSpeedX = 0.0f;            // ��]���xX
        float fRotSpeedY = 0.0f;            // ��]���xY
        float fRotSpeedZ = 0.0f;            // ��]���xZ
        CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_001;

        // �ʒu�����炷
        fPosX = float(GetRandNum((int)m_collisionSize.x, 0)) - float(GetRandNum((int)m_collisionSize.x, 0));
        fPosZ = float(GetRandNum((int)m_collisionSize.z, 0)) - float(GetRandNum((int)m_collisionSize.z, 0));

        // ��]�̑��������߂�
        fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
        fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;

        // ��]�ʂ����߂�
        D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

        // ��ނ����߂�
        int nRandNum = GetRandNum(2, 0);
        switch (nRandNum)
        {
        case 0:
            modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_001;
            break;
        case 1:
            modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_002;
            break;
        case 2:
            modelEffectType = CModelEffect::TYPE_DEBRI_LARGE_003;
            break;
        }

        // �G�t�F�N�g����(Y���͏�̖ʂŌŒ�)
        CModelEffect::Create(GetPos() + D3DXVECTOR3(fPosX, m_collisionSize.y, fPosZ), rotMove, DEFAULT_VECTOR, rotMove, modelEffectType);
    }
}

//=============================================================================
// ���ɓ|��Ă����G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBlock::FallDownModel(void)
{
    // �����擾
    CSound*pSound = CManager::GetSound();

    // �ʒu�Ɖ�]���擾
    D3DXVECTOR3 pos = GetPos();
    D3DXVECTOR3 rot = GetRot();

    // �|�������������_���Ɍ��߂�
    int nRandNum = GetRandNum(4, 1);
    D3DXVECTOR3 rotMove = DEFAULT_VECTOR;
    switch (nRandNum)
    {
    case 1:
        rotMove = D3DXVECTOR3(MODEL_EFFECT_FALL_DOWN_FIRST_SPEED, 0.0f, 0.0f);
        break;
    case 2:
        rotMove = D3DXVECTOR3(-MODEL_EFFECT_FALL_DOWN_FIRST_SPEED, 0.0f, 0.0f);
        break;
    case 3:
        rotMove = D3DXVECTOR3(0.0f, 0.0f, MODEL_EFFECT_FALL_DOWN_FIRST_SPEED);
        break;
    case 4:
        rotMove = D3DXVECTOR3(0.0f, 0.0f, -MODEL_EFFECT_FALL_DOWN_FIRST_SPEED);
        break;
    }

    // ��ނ��ƂɃG�t�F�N�g����
    switch (m_nType)
    {
    case TYPE_SIGNAL:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_SIGNAL);
        break;

    case TYPE_LIGHT:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_LIGHT);
        break;

    case TYPE_LIGHT2:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_LIGHT2);
        break;

    case TYPE_WOOD1:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_WOOD1);
        break;

    case TYPE_WOOD2:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_WOOD2);
        break;

    case TYPE_WOOD3:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_WOOD3);
        break;

    case TYPE_GUARDRAIL:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, BLOCK_GUARDRAIL_ROT_MOVE, CModelEffect::TYPE_GUARDRAIL);
        break;

    case TYPE_SIGN_EMAR:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_SIGN_EMAR);
        break;

    case TYPE_SIGN_INTER:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_SIGN_INTER);
        break;

    case TYPE_SIGN_NO_PARK:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_SIGN_NO_PARK);
        break;

    case TYPE_METAL:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, rotMove, CModelEffect::TYPE_METAL);
        break;

    case TYPE_CRYSTAL:
        CModelEffect::Create(pos, rot, DEFAULT_VECTOR, DEFAULT_VECTOR, CModelEffect::TYPE_MINI_CRYSTAL);
        CModelEffect::Create(pos, rot, BLOCK_CRYSTAL_SHOOT_UP_MOVE, BLOCK_CRYSTAL_SHOOT_UP_ROT_MOVE, CModelEffect::TYPE_LONG_CRYSTAL);
        pSound->Play(CSound::LABEL_SE_DESTROY_CRYSTAL);
        break;
    }
}

//=============================================================================
// �~�b�V�����N���A���ɉ��Z����
// Author : �㓡�T�V��
//=============================================================================
void CBlock::AddMissionClear(void)
{
    // �X�e�[�W�ƌ��݂̃~�b�V�����ɉ����āA�N���A���𑝉�
    CGame::STAGE stage = CGame::GetStage();
    CGame::CURRENT_MISSION currentMission = CGame::GetCurrentMission();
    switch (stage)
    {
    case CGame::STAGE_1:

        switch (currentMission)
        {
        case CGame::CURRENT_MISSION_1:
            CGame::SetClearNumMission1();
            break;

        case CGame::CURRENT_MISSION_2:

            break;
        }

        break;

    case CGame::STAGE_2:

        break;

    case CGame::STAGE_FINAL:

        switch (currentMission)
        {
        case CGame::CURRENT_MISSION_1:
            // �N���X�^���Ȃ�
            if (m_nType == TYPE_CRYSTAL)
            {
                CGame::SetClearNumMission1();
            }
            break;

        case CGame::CURRENT_MISSION_2:

            break;
        }

        break;
    }
}
