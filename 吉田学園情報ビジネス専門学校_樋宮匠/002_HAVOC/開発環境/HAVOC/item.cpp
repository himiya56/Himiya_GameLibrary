//===============================================
//
// �A�C�e������ (item.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "debug.h"
#include "game.h"
#include "score.h"
#include "effect3d.h"
#include "library.h"
#include "modelEffect.h"
#include "block.h"
#include "player.h"
#include "sound.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
LPD3DXMESH	CItem::m_apMesh[CItem::TYPE_MAX] = {};
LPD3DXBUFFER CItem::m_apBuffMat[CItem::TYPE_MAX] = {};
DWORD CItem::m_aNumMat[CItem::TYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CItem::m_apTexture[CItem::TYPE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CItem::CItem() :CScene3D(CScene::OBJTYPE_ITEM)
{
    m_type = TYPE_HEAL;
    m_collisionSize = DEFAULT_VECTOR;
    m_posOld = DEFAULT_VECTOR;
    m_move = DEFAULT_VECTOR;

    m_nLife = 0;
    m_bUse = true;
    m_bBlinking = false;

    m_pMinimap = NULL;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CItem::~CItem()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene3D::Init(pos, size);

    // �X�P�[����ݒ�
    SetScale(size);

    // ���b�V���A�e�N�X�`�����o�C���h
    BindMesh(m_apMesh[m_type], m_apBuffMat[m_type], m_aNumMat[m_type]);
    BindTexture(m_apTexture[m_type]);

    // �~�j�}�b�v��ɃA�C�R���𐶐�
    m_pMinimap = CMinimap::Create(CMinimap::ICON_LIFE);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CItem::Uninit(void)
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
void CItem::Update(void)
{
    // �ʒu���擾
    D3DXVECTOR3 pos = GetPos();

    // 1F�O�̈ʒu�����т���
    m_posOld = pos;

    // �ړ��ʂƈʒu�����т���
    pos += m_move;

    // �n�ʂ���Ȃ�
    if (pos.y > 0.0f)
    {
        // �d��
        m_move.y -= GRAVITY;
    }
    else
    {
        // �n�ʂɂ���
        pos.y = 0.0f;
        m_move.y = 0.0f;
    }

    // �u���b�N�ւ̓����蔻��
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BLOCK);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BLOCK); nCntScene++)
    {
        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // ���̃V�[�����L��
            CScene*pNextScene = pScene->GetNextScene();

            // �u���b�N�ɃL���X�g
            CBlock *pBlock = (CBlock*)pScene;

            // �u���b�N�̈ʒu�Ƒ傫�����擾
            D3DXVECTOR3 blockPos = pBlock->GetPos();
            D3DXVECTOR3 blockSize = pBlock->GetCollisionSize();

            // �u���b�N�Ƃ̓����蔻����v�Z
            BLOCK_HIT_INFO blockHitInfo;
            blockHitInfo.hitSurface = HIT_SURFACE_NONE;
            blockHitInfo.pos = pos;
            blockHitInfo = (BlockCollision3D(&pos, &m_posOld, &blockPos,
                &m_collisionSize, &blockSize));

            // �u���b�N�̓����蔻��Ōv�Z�����ʒu���A���т���
            pos = blockHitInfo.pos;

            // ��̖ʂɓ������Ă���Ȃ�A�c�̈ړ��ʂ�0�ɂ���
            if (blockHitInfo.hitSurface == HIT_SURFACE_TOP)
            {
                m_move.y = 0.0f;
            }

            // ���̃V�[���ɂ���
            pScene = pNextScene;
        }
        else
        {
            // ���g���Ȃ��Ȃ�A�����ŏ������I����
            break;
        }
    }

    // �v���C���[�Ƃ̓����蔻��
    CPlayer*pPlayer = NULL;
    pPlayer = CGame::GetPlayer();

    // ���@�̈ʒu���擾
    D3DXVECTOR3 playerPos = pPlayer->GetPos();

    // �v���C���[���\������Ă���Ȃ�
    if (pPlayer->GetDisp() == true)
    {
        // �������Ă���Ȃ�
        if (RectangleCollision3D(&pos, &playerPos,
            &m_collisionSize, &PLAYER_COLLISION_SIZE) == true)
        {
            // SE���Đ�
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_HEAL);

            // ���@����
            CGame::GetPlayer()->Healing(ITEM_HEAL_VALUE);

            // ���̃A�C�e���𖢎g�p��
            m_bUse = false;
        }
    }

    // �ʒu��ݒ�
    SetPos(pos);

    // ��ނ��Ƃ̃G�t�F�N�g
    switch (m_type)
    {
    case TYPE_HEAL:
        HealEffect();
        break;
    }

    // �~�j�}�b�v�A�C�R���̏��X�V
    if (m_pMinimap != NULL)
    {
        m_pMinimap->SetFieldPos(pos);
    }

#ifdef COLLISION_TEST
    CDebug::Create(GetPos(), m_collisionSize, CDebug::TYPE_MOMENT);
#endif #endif // COLLISION_TEST

    // �ݒu����Ă���̎��Ԃ��J�E���g�_�E��
    m_nLife--;

    // �̗͂�0�ȉ��Ȃ�A�g�p�t���O��false��
    if (m_nLife <= 0)
    {
        m_bUse = false;
    }

    // �g�p�t���O��false�Ȃ�A����
    if (m_bUse == false)
    {
        Uninit();
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CItem::Draw(void)
{
    // �_�ł��Ă��Ȃ��Ȃ�
    if (m_bBlinking == false)
    {
        CScene3D::Draw();
    }
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
HRESULT CItem::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //==============================
    // �q�[��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/heal.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_HEAL], NULL, &m_aNumMat[TYPE_HEAL], &m_apMesh[TYPE_HEAL]);

    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : �㓡�T�V��
//=============================================================================
void CItem::Unload(void)
{
    for (int nCnt = 0; nCnt < CItem::TYPE_MAX; nCnt++)
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
CItem * CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
    // �|�C���^��p��
    CItem *pItem = NULL;

    // �������m��
    pItem = new CItem;

    // �^�C�v��R�Â�(�������̒���Bind������̂����邽�߁A�ŏ��ɕR�Â�)
    pItem->m_type = type;

    // ������
    pItem->Init(pos, DEFAULT_SCALE);

    // ��]�����т���
    pItem->SetRot(rot);

    // 1F�O�̈ʒu��ݒ�
    pItem->m_posOld = pos;

    // ��ނ��ƂɁA���т���l��ς���
    switch (type)
    {
    case TYPE_HEAL:

        pItem->m_collisionSize = ITEM_HEAL_COLLISION_SIZE;
        pItem->m_nLife = ITEM_HEAL_LIFE;
        pItem->m_move.y = ITEM_HEAL_APPEAR_LAUNCH_VALUE;

        break;
    }

    return pItem;
}

//=============================================================================
// �񕜃A�C�e���̃G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CItem::HealEffect(void)
{
    // �ʒu�ƌ������擾
    D3DXVECTOR3 pos = GetPos();
    D3DXVECTOR3 rot = GetRot();

    // ��ɉ�]
    rot.y += ITEM_HEAL_ROT_SPEED;

    // �����𔽉f
    SetRot(rot);

    // ���Ԋu�ŃG�t�F�N�g����
    if (m_nLife % ITEM_HEAL_SET_EFFECT_INTERVAL == 0)
    {
        // �G�t�F�N�g�����p�̕ϐ��錾
        // �������o��
        float fSpeed = float(GetRandNum(EFFECT_HEAL_ITEM_MAX_SPEED, EFFECT_HEAL_ITEM_MIN_SPEED)) / 100.0f;

        // �ړ��ʂ����߂�
        D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeed, 0.0f);

        // �傫�����o��
        float fSize = float(GetRandNum(EFFECT_HEAL_ITEM_MAX_SIZE, EFFECT_HEAL_ITEM_MIN_SIZE)) / 100.0f;

        // �傫�������߂�
        D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

        // �p�x���o��
        float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // ���a���o��
        float fRadius = float(GetRandNum(EFFECT_HEAL_ITEM_MAX_RADIUS, 0)) / 100.0f;

        // �ʒu�����߂�
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*fRadius, fSize / 2, -cosf(fAngle)*fRadius) + pos;

        // �G�t�F�N�g����
        CEffect3D::Create(effectPos, size, move, EFFECT_HEAL_ITEM_COLOR,
            DEFAULT_VECTOR, EFFECT_HEAL_ITEM_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
    }

    // ��莞�ԂŁA�_�ł�����
    if (m_nLife <= ITEM_HEAL_START_BLINKING)
    {
        m_bBlinking = !m_bBlinking;
    }
}
