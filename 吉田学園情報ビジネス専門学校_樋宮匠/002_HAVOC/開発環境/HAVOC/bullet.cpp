//===============================================
//
// �e���� (bullet.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
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
// �ÓI�����o�ϐ��錾
//========================================
LPD3DXMESH	CBullet::m_apMesh[CBullet::TYPE_MAX] = {};
LPD3DXBUFFER CBullet::m_apBuffMat[CBullet::TYPE_MAX] = {};
DWORD CBullet::m_aNumMat[CBullet::TYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CBullet::m_apTexture[CBullet::TYPE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
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
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene3D::Init(pos, size);

    // �X�P�[����ݒ�
    SetScale(size);

    // ���b�V���A�e�N�X�`�����o�C���h
    BindMesh(m_apMesh[m_type], m_apBuffMat[m_type], m_aNumMat[m_type]);
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CBullet::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CBullet::Update(void)
{
    // �ʒu�A�傫�����擾
    D3DXVECTOR3 pos = GetPos();

    // 1F�O�̈ʒu�����т���
    m_posOld = pos;

    // �ړ��ʂ���ނɂ���ĕς���
    switch (m_type)
    {
    case TYPE_AR:
        // �ړ��ʂ��ʒu�Ɍ��т���
        pos += m_moveAngle * m_fSpeed;
        break;
    case TYPE_BLOOD:
        // �ړ��ʂ��ʒu�Ɍ��т���(�d�͗L��)
        m_nCntTime++;
        pos += D3DXVECTOR3(m_moveAngle.x * m_fSpeed, m_moveAngle.y * m_fSpeed + (m_nCntTime * (-GRAVITY)), m_moveAngle.z * m_fSpeed);
        break;
    case TYPE_ACID:
        // �ړ��ʂ��ʒu�Ɍ��т���(�d�͗L��)
        m_nCntTime++;
        pos += D3DXVECTOR3(m_moveAngle.x * m_fSpeed, m_moveAngle.y * m_fSpeed + (m_nCntTime * (-GRAVITY)), m_moveAngle.z * m_fSpeed);
        break;
    case TYPE_BOMB:
        // �ړ��ʂ��ʒu�Ɍ��т���(�e�������L��)
        pos += m_moveAngle * m_fSpeed;
        m_nCntTime++;
        pos.y += m_nCntTime * BULLET_BOMB_FALL_DOWN_RATE;
        break;
    case TYPE_CLAP:
        // �ړ��ʂ��ʒu�Ɍ��т���
        pos += m_moveAngle * m_fSpeed;
        break;
    case TYPE_SHOCK:
        // �ړ��ʂ��ʒu�Ɍ��т���
        pos += m_moveAngle * m_fSpeed;
        break;
    }

#ifdef COLLISION_TEST
    if (m_type == TYPE_EXPLOSION)
    {
        // �����̓����蔻��������ڒʂ�ɂ��邽�߂ɁA�T�C�Y��4����1�A���ɉ�����
        D3DXVECTOR3 explosionPos = pos - D3DXVECTOR3(0.0f, (m_collisionSize.y / 4), 0.0f);
        CDebug::Create(explosionPos, m_collisionSize, CDebug::TYPE_MOMENT);
    }
    else  if (m_type == TYPE_SHOCK)
    {
        // �Ռ��g�̓����蔻��������ڒʂ�ɂ��邽�߂ɁA�T�C�Y��ς���
        CDebug::Create(pos, BULLET_SHOCK_COLLISION_REAL_SIZE, CDebug::TYPE_MOMENT);
        CDebug::Create(GetMiddlePos(pos, m_posOld), BULLET_SHOCK_COLLISION_REAL_SIZE, CDebug::TYPE_MOMENT);
    }
    else
    {
        CDebug::Create(pos, m_collisionSize, CDebug::TYPE_MOMENT);
        CDebug::Create(GetMiddlePos(pos, m_posOld), m_collisionSize, CDebug::TYPE_MOMENT);
    }
#endif #endif // COLLISION_TEST

    // �ʒu��ݒ�
    SetPos(pos);

    // ��ނ��ƂɁA�G�t�F�N�g����
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

    // �����蔻��(����������ȊO��)
    bool bUse = true;
    if (m_type == TYPE_EXPLOSION)
    {
        ExplosionCollision(pos);
    }
    else
    {
        bUse = Collision(pos);
    }

    // ���C�t����
    m_nLife--;

    // ���C�t���Ȃ��Ȃ����A�܂��͎g�p�t���O���Ȃ��Ȃ�����A����
    if (m_nLife <= 0 || bUse == false)
    {
        Uninit();
    }
}

//=============================================================================
// �Փ˔��菈��
// Author : �㓡�T�V��
//=============================================================================
bool CBullet::Collision(D3DXVECTOR3 pos)
{
    // �g���t���O
    bool bUse = true;

    // �����蔻��̑傫��
    D3DXVECTOR3 bulletCollisionSize = m_collisionSize;

    // �Ռ��g�͓����蔻��̑傫�������
    if (m_type == TYPE_SHOCK)
    {
        bulletCollisionSize = BULLET_SHOCK_COLLISION_REAL_SIZE;
    }

    // �n�ʂ�艺�ɍs������
    if (pos.y <= 0.0f)
    {
        // �A�T���g���C�t���̒e�Ȃ�
        if (m_type == TYPE_AR)
        {
            // �e������
            CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_RED_FIRE_SIZE,
                DEFAULT_VECTOR, EFFECT_RED_FIRE_FIRST_COLOR, HIT_TOP, EFFECT_RED_FIRE_FADE_OUT_RATE, CEffect3D::TYPE_GUN_MARKS);
        }
        // ���Ȃ�
        else if (m_type == TYPE_BLOOD)
        {
            // �_����
            CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_ACID_FIRST_SIZE,
                DEFAULT_VECTOR, EFFECT_ACID_FIRST_COLOR, HIT_TOP, EFFECT_ACID_FADE_OUT_RATE, CEffect3D::TYPE_ACID);
        }
        // �_�Ȃ�
        else if (m_type == TYPE_ACID)
        {
            // �U���_����
            CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_ACID_FIRST_SIZE,
                DEFAULT_VECTOR, EFFECT_ACID_ATTACK_FIRST_COLOR, HIT_TOP, EFFECT_ACID_FADE_OUT_RATE, CEffect3D::TYPE_ACID_ATTACK);
        }
        // �h�q�R�̒e�Ȃ�
        else if (m_type == TYPE_CLAP)
        {
            // �e������
            CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_RED_FIRE_SIZE,
                DEFAULT_VECTOR, EFFECT_RED_FIRE_FIRST_COLOR, HIT_TOP, EFFECT_RED_FIRE_FADE_OUT_RATE, CEffect3D::TYPE_GUN_MARKS);
        }
        // �Ռ��g�Ȃ�
        else if (m_type == TYPE_SHOCK)
        {
            // �Ռ��g�U���̃G�t�F�N�g����
            CEffect3D::Create(D3DXVECTOR3(pos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_ACID_FIRST_SIZE,
                DEFAULT_VECTOR, EFFECT_BLUE_FIRE_FIRST_COLOR, HIT_TOP, EFFECT_BLUE_FIRE_FADE_OUT_RATE, CEffect3D::TYPE_WAVE_ATTACK);
        }

        // �Ռ��g�ȊO�͖��g�p��
        if (m_type != TYPE_SHOCK)
        {
            bUse = false;
        }
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

            // �e�̈ʒu
            D3DXVECTOR3 bulletPos = pos;

            // ���݂̈ʒu�A1F�O�ƌ��݂̈ʒu�̊Ԃ�2�œ����蔻������
            for (int nCnt = 0; nCnt < 2; nCnt++)
            {
                // 2��ڂ́A���Ԃ̈ʒu�ɕς���
                if (nCnt == 1)
                {
                    bulletPos = GetMiddlePos(pos, m_posOld);
                }

                // �����蔻��v�Z
                blockHitInfo = (BlockCollision3D(&bulletPos, &m_posOld, &blockPos,
                    &bulletCollisionSize, &blockSize));

                // ���������Ȃ�Afor�𔲂���
                if (blockHitInfo.hitSurface != HIT_SURFACE_NONE)
                {
                    // �e���v���C���[�̂��̂Ȃ�
                    if (m_tribe == TRIBE_PLAYER)
                    {
                        // ���X�g�q�b�g���v���C���[�Ń_���[�W
                        pBlock->SetTakeDamage(m_nDamage, CBlock::LAST_HIT_PLAYER);

                        // �N���X�^���Ȃ�
                        if (pBlock->GetType() == CBlock::TYPE_CRYSTAL)
                        {
                            HitEffect(bulletPos, HIT_CRYSTAL);
                        }
                        // ����ȊO�͔ėp�G�t�F�N�g
                        else
                        {
                            HitEffect(bulletPos, HIT_NON_LIVING);
                        }
                    }
                    // �e�������A�܂��͏Ռ��g�Ȃ�
                    else if (m_tribe == TRIBE_NEUTRAL || m_type == TYPE_SHOCK)
                    {
                        // ���X�g�q�b�g���G�Ń_���[�W��^����(�����̂݁A�_���[�W�����L��)
                        float fDamage = (float)m_nDamage;
                        if (m_tribe == TRIBE_NEUTRAL)
                        {
                            float fDamage = (float)m_nDamage * BULLET_NEUTRAL_ATTENUATION_FOR_BLOCK_RATE;
                        }
                        pBlock->SetTakeDamage((int)fDamage, CBlock::LAST_HIT_ENEMY);

                        // �񐶕��ւ̃q�b�g�G�t�F�N�g
                        HitEffect(bulletPos, HIT_NON_LIVING);
                    }

                    // �Ռ��g�ȊO��
                    if (m_type != TYPE_SHOCK)
                    {
                        // ���g�p��
                        bUse = false;
                    }

                    break;
                }
            }

            // �q�b�g�ʂɉt�̔���
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

                // �t�̂Ȃ�
                if (m_tribe == TRIBE_LIQUID)
                {
                    // �_����
                    CEffect3D::Create(D3DXVECTOR3(pos.x, blockPos.y + blockSize.y + EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.z), EFFECT_ACID_FIRST_SIZE,
                        DEFAULT_VECTOR, color, HIT_TOP, EFFECT_ACID_FADE_OUT_RATE, effectType);
                }

                break;

            case HIT_SURFACE_LEFT:

                // �t�̂Ȃ�
                if (m_tribe == TRIBE_LIQUID)
                {
                    // �_����
                    CEffect3D::Create(D3DXVECTOR3(blockPos.x - (blockSize.x / 2) - EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.y, pos.z),
                        EFFECT_ACID_FIRST_SIZE, DEFAULT_VECTOR, color, HIT_LEFT, EFFECT_ACID_FADE_OUT_RATE, effectType);
                }

                break;

            case HIT_SURFACE_BACK:

                // �t�̂Ȃ�
                if (m_tribe == TRIBE_LIQUID)
                {
                    // �_����
                    CEffect3D::Create(D3DXVECTOR3(pos.x, pos.y, blockPos.z + (blockSize.z / 2) + EFFECT_ACID_FLOAT_FROM_SURFACE_POS),
                        EFFECT_ACID_FIRST_SIZE, DEFAULT_VECTOR, color, HIT_BACK, EFFECT_ACID_FADE_OUT_RATE, effectType);
                }

                break;

            case HIT_SURFACE_RIGHT:

                // �t�̂Ȃ�
                if (m_tribe == TRIBE_LIQUID)
                {
                    // �_����
                    CEffect3D::Create(D3DXVECTOR3(blockPos.x + (blockSize.x / 2) + EFFECT_ACID_FLOAT_FROM_SURFACE_POS, pos.y, pos.z),
                        EFFECT_ACID_FIRST_SIZE, DEFAULT_VECTOR, color, HIT_RIGHT, EFFECT_ACID_FADE_OUT_RATE, effectType);
                }

                break;

            case HIT_SURFACE_FRONT:

                // �t�̂Ȃ�
                if (m_tribe == TRIBE_LIQUID)
                {
                    // �_����
                    CEffect3D::Create(D3DXVECTOR3(pos.x, pos.y, blockPos.z - (blockSize.z / 2) - EFFECT_ACID_FLOAT_FROM_SURFACE_POS),
                        EFFECT_ACID_FIRST_SIZE, DEFAULT_VECTOR, color, HIT_FRONT, EFFECT_ACID_FADE_OUT_RATE, effectType);
                }

                break;
            }
            //=====================================================================================

            // ���������Ȃ�Afor���𔲂���
            if (blockHitInfo.hitSurface != HIT_SURFACE_NONE)
            {
                break;
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

    // �e���v���C���[�������A�܂��͏Ռ��g�Ȃ�
    if (m_tribe == TRIBE_PLAYER || m_tribe == TRIBE_NEUTRAL || m_type == TYPE_SHOCK)
    {
        // �G�ւ̓����蔻��
        pScene = CScene::GetTopScene(CScene::OBJTYPE_ENEMY);
        for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_ENEMY); nCntScene++)
        {
            // ���g������Ȃ�
            if (pScene != NULL)
            {
                // ���̃V�[�����L��
                CScene*pNextScene = pScene->GetNextScene();

                // �L�����N�^�[�ɃL���X�g
                CCharacter *pEnemy = (CCharacter*)pScene;

                // �����蔻��̐�����
                for (int nCntCollision = 0; nCntCollision < pEnemy->GetCntCollision(); nCntCollision++)
                {
                    // �G�l�~�[�̈ʒu�Ƒ傫�����擾
                    CCharacter::COLLISION enemyCollision = pEnemy->GetCollision(nCntCollision);

                    // �e�̈ʒu
                    D3DXVECTOR3 bulletPos = pos;

                    // ���݂̈ʒu�A1F�O�ƌ��݂̈ʒu�̊Ԃ�2�œ����蔻������
                    for (int nCnt = 0; nCnt < 2; nCnt++)
                    {
                        // 2��ڂ́A���Ԃ̈ʒu�ɕς���
                        if (nCnt == 1)
                        {
                            bulletPos = GetMiddlePos(pos, m_posOld);
                        }

                        // �������Ă���Ȃ�
                        if (RectangleCollision3D(&bulletPos, &enemyCollision.pos,
                            &bulletCollisionSize, &enemyCollision.size) == true)
                        {
                            // �{�X�d�l�łȂ��Ȃ�
                            if (pEnemy->GetBossSetting() == false)
                            {
                                // �푰�ɂ���āA�_���[�W��^����Ƃ��̃��X�g�q�b�g��ς���
                                if (m_tribe == TRIBE_PLAYER)
                                {
                                    pEnemy->SetTakeDamage(m_nDamage, CCharacter::LAST_HIT_PLAYER);
                                }
                                else
                                {
                                    // �G�Ƀ_���[�W(�_���[�W�����L��)
                                    float fDamage = (float)m_nDamage * BULLET_NEUTRAL_ATTENUATION_FOR_ENEMY_RATE;
                                    pEnemy->SetTakeDamage((int)fDamage, CCharacter::LAST_HIT_ENEMY);
                                }
                            }

                            // �Ռ��g�łȂ��Ȃ�
                            if (m_type != TYPE_SHOCK)
                            {
                                // �푰���Ƃɏꍇ����
                                CSpider*pSpider = NULL;
                                CAnt*pAnt = NULL;
                                CUfo*pUfo = NULL;
                                CTower*pTower = NULL;
                                CWyvern*pWyvern = NULL;
                                switch (pEnemy->GetTribe())
                                {
                                case CCharacter::TRIBE_SPIDER:

                                    // �X�p�C�_�[�^�ɃL���X�g
                                    pSpider = (CSpider*)pEnemy;

                                    // �v���C���[�̒e�Ȃ�A�x��
                                    if (m_tribe == TRIBE_PLAYER)
                                    {
                                        // �x������
                                        pSpider->SetWarning(true);

                                        // ���͂̒��ԂɌx������
                                        pSpider->WarnFellow();
                                    }

                                    // �����ւ̃q�b�g�G�t�F�N�g
                                    HitEffect(bulletPos, HIT_LIVING);

                                    break;

                                case CCharacter::TRIBE_ANT:

                                    // �A���^�ɃL���X�g
                                    pAnt = (CAnt*)pEnemy;

                                    // �v���C���[�̒e�Ȃ�A�x��
                                    if (m_tribe == TRIBE_PLAYER)
                                    {
                                        // �x������
                                        pAnt->SetWarning(true);

                                        // ���͂̒��ԂɌx������
                                        pAnt->WarnFellow();
                                    }

                                    // �����ւ̃q�b�g�G�t�F�N�g
                                    HitEffect(bulletPos, HIT_LIVING);

                                    break;

                                case CCharacter::TRIBE_FORCE:

                                    // �l�Ԃւ̃q�b�g�G�t�F�N�g
                                    HitEffect(bulletPos, HIT_HUMAN);

                                    break;

                                case CCharacter::TRIBE_UFO:

                                    // UFO�^�ɃL���X�g
                                    pUfo = (CUfo*)pEnemy;

                                    // �v���C���[�̒e�Ȃ�A�x��
                                    if (m_tribe == TRIBE_PLAYER)
                                    {
                                        // �x������
                                        pUfo->SetWarning(true);

                                        // ���͂̒��ԂɌx������
                                        pUfo->WarnFellow();
                                    }

                                    // �񐶕��ւ̃q�b�g�G�t�F�N�g
                                    HitEffect(bulletPos, HIT_NON_LIVING);

                                    break;

                                case CCharacter::TRIBE_TOWER:

                                    // �^���[�^�ɃL���X�g
                                    pTower = (CTower*)pEnemy;

                                    // �_���[�W�𓖂������ꏊ�ɉ����Čv�Z
                                    pTower->TakeDamage(bulletPos, m_nDamage, (CTower::PARTS)nCntCollision);

                                    // �񐶕��ւ̃q�b�g�G�t�F�N�g
                                    HitEffect(bulletPos, HIT_NON_LIVING);

                                    break;

                                case CCharacter::TRIBE_WYVERN:

                                    // ���C�o�[���^�ɃL���X�g
                                    pWyvern = (CWyvern*)pEnemy;

                                    // �_���[�W�𓖂������ꏊ�ɉ����Čv�Z
                                    pWyvern->TakeDamage(bulletPos, m_nDamage, nCntCollision);

                                    // �񐶕��ւ̃q�b�g�G�t�F�N�g
                                    HitEffect(bulletPos, HIT_NON_LIVING);

                                    break;
                                }
                            }

                            // �e�𖢎g�p��
                            bUse = false;
                        }

                        // ���������Ȃ�for�𔲂���
                        if (bUse == false)
                        {
                            break;
                        }
                    }

                    // ���������Ȃ�for�𔲂���
                    if (bUse == false)
                    {
                        break;
                    }
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
    }

    // �e���G�������̂��̂Ȃ�
    if(m_tribe == TRIBE_ENEMY || m_tribe == TRIBE_NEUTRAL)
    {
        // ���@�̈ʒu���擾
        D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

        // �e�̈ʒu
        D3DXVECTOR3 bulletPos = pos;

        // ���݂̈ʒu�A1F�O�ƌ��݂̈ʒu�̊Ԃ�2�œ����蔻������
        for (int nCnt = 0; nCnt < 2; nCnt++)
        {
            // 2��ڂ́A���Ԃ̈ʒu�ɕς���
            if (nCnt == 1)
            {
                bulletPos = GetMiddlePos(pos, m_posOld);
            }

            // �������Ă���Ȃ�
            if (RectangleCollision3D(&bulletPos, &playerPos,
                &bulletCollisionSize, &PLAYER_COLLISION_SIZE) == true)
            {
                // �N���b�v�́A���G���Ԃ�m�b�N�o�b�N��݂����A���ڑ̗͂����炷
                if (m_type == TYPE_CLAP)
                {  
                    // �Q�[���̃X�e�[�g���ʏ�Ȃ�
                    if (CGame::GetState() == CGame::STATE_NORMAL)
                    {
                        // ���G���Ԃ��Ȃ��Ȃ�
                        if (CGame::GetPlayer()->GetInvincibleFrame() <= 0)
                        {
                            // ���@�ɒ��ڃ_���[�W
                            CGame::GetPlayer()->SetTakeDamage(m_nDamage, CCharacter::LAST_HIT_ENEMY);

                            // �����ւ̃q�b�g�G�t�F�N�g
                            HitEffect(bulletPos, HIT_LIVING);
                        }
                    }
                }
                else
                {
                    // ���@�Ƀ_���[�W
                    CGame::GetPlayer()->TakeDamage(m_nDamage, m_posOld);

                    // �����ւ̃q�b�g�G�t�F�N�g
                    HitEffect(bulletPos, HIT_LIVING);
                }

                // �e�𖢎g�p��
                bUse = false;
            }

            // ���������Ȃ�for�𔲂���
            if (bUse == false)
            {
                break;
            }
        }
    }

    // ���e�Ȃ�
    if (m_type == TYPE_BOMB)
    {
        // ���g�p�ɂȂ����ۂɔ�������
        if (bUse == false)
        {
            SetExplosion(pos);
        }
    }

    // �Ռ��g�͖��g�p�ɂȂ�Ȃ�
    if (m_type == TYPE_SHOCK)
    {
        bUse = true;
    }

    return bUse;
}

//=============================================================================
// �����̓����蔻��
// Author : �㓡�T�V��
//=============================================================================
void CBullet::ExplosionCollision(D3DXVECTOR3 pos)
{
    // �����蔻�肪��������܂ł̎��Ԃ𐔂���
    m_nCntTime++;
    if (m_nCntTime >= BULLET_EXPLOSION_START_COLLISION)
    {
        // �����̓����蔻��������ڒʂ�ɂ��邽�߂ɁA�T�C�Y��4����1�A���ɉ�����
        D3DXVECTOR3 explosionPos = pos - D3DXVECTOR3(0.0f, (m_collisionSize.y / 4), 0.0f);

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
                if (RectangleCollision3D(&explosionPos, &blockPos,
                    &m_collisionSize, &blockSize) == true)
                {
                    // �푰�ɂ���āA�_���[�W��^����Ƃ��̃��X�g�q�b�g��ς���
                    if (m_tribe == TRIBE_PLAYER)
                    {
                        pBlock->SetTakeDamage(m_nDamage, CBlock::LAST_HIT_PLAYER);
                    }
                    else
                    {
                        pBlock->SetTakeDamage(m_nDamage, CBlock::LAST_HIT_ENEMY);
                    }
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

        // �G�ւ̓����蔻��
        pScene = CScene::GetTopScene(CScene::OBJTYPE_ENEMY);
        for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_ENEMY); nCntScene++)
        {
            // ���g������Ȃ�
            if (pScene != NULL)
            {
                // ���̃V�[�����L��
                CScene*pNextScene = pScene->GetNextScene();

                // �L�����N�^�[�ɃL���X�g
                CCharacter *pEnemy = (CCharacter*)pScene;

                // �����蔻��̐�����
                bool bHitWyvern = false;    // ���C�o�[���ɓ����������ǂ���
                float fDistance1 = 0.0f;    // ����1
                float fDistance2 = 0.0f;    // ����2
                int nCntHit = 0;            // ����������
                for (int nCntCollision = 0; nCntCollision < pEnemy->GetCntCollision(); nCntCollision++)
                {
                    // �G�l�~�[�̈ʒu�Ƒ傫�����擾
                    CCharacter::COLLISION enemyCollision = pEnemy->GetCollision(nCntCollision);

                    // �������Ă���Ȃ�
                    if (RectangleCollision3D(&explosionPos, &enemyCollision.pos,
                        &m_collisionSize, &enemyCollision.size) == true)
                    {
                        // �{�X�d�l�łȂ��Ȃ�
                        if (pEnemy->GetBossSetting() == false)
                        {
                            // �푰�ɂ���āA�_���[�W��^����Ƃ��̃��X�g�q�b�g��ς���
                            if (m_tribe == TRIBE_PLAYER)
                            {
                                pEnemy->SetTakeDamage(m_nDamage, CCharacter::LAST_HIT_PLAYER);
                            }
                            else
                            {
                                pEnemy->SetTakeDamage(m_nDamage, CCharacter::LAST_HIT_ENEMY);
                            }
                        }
                        // �{�X�d�l�Ȃ�A���i�q�b�g����̂��߁A1F���������̃_���[�W��ʂ�
                        else
                        {
                            // ���C�o�[���Ȃ�
                            if (pEnemy->GetTribe() == CCharacter::TRIBE_WYVERN)
                            {
                                // �{�X�ɑ΂��ẮA�ǂ̔����ł��v���C���[�̂��̂Ƃ��Ĉ���
                                if (m_nCntTime == BULLET_EXPLOSION_USE_COLLISION_FRAME_FOR_BOSS)
                                {
                                    // ����������ɁA����1�Ƌ���2���r����
                                    if (bHitWyvern == true)
                                    {
                                        // ����2�����߂�
                                        fDistance2 = sqrtf(powf((enemyCollision.pos.x - pos.x), 2) + powf((enemyCollision.pos.z - pos.z), 2));

                                        // �������r���āA����2�̂ق����������Ȃ�
                                        if (fDistance1 > fDistance2)
                                        {
                                            // ����1���X�V
                                            fDistance1 = fDistance2;

                                            // ���������������т���
                                            nCntHit = nCntCollision;
                                        }
                                    }

                                    // �������ĂȂ��Ȃ�A�ŏ��̋��������т���
                                    if (bHitWyvern == false)
                                    {
                                        // ����1�����߂�
                                        fDistance1 = sqrtf(powf((enemyCollision.pos.x - pos.x), 2) + powf((enemyCollision.pos.z - pos.z), 2));

                                        // ���������t���O��true��
                                        bHitWyvern = true;

                                        // ���������������т���
                                        nCntHit = nCntCollision;
                                    }
                                }
                            }
                            else
                            {
                                // ���C�o�[���ȊO�̃{�X�ɑ΂��ẮA�ǂ̔����ł��v���C���[�̂��̂Ƃ��Ĉ���
                                if (m_nCntTime == BULLET_EXPLOSION_USE_COLLISION_FRAME_FOR_BOSS)
                                {
                                    pEnemy->SetTakeDamage(m_nDamage, CCharacter::LAST_HIT_PLAYER);
                                }
                            }
                        }

                        // ���C�o�[���łȂ��Ȃ�
                        if (pEnemy->GetTribe() != CCharacter::TRIBE_WYVERN)
                        {
                            // ���i�q�b�g������邽�߁Afor�𔲂���
                            break;
                        }
                    }
                }

                // ���C�o�[���ɓ������Ă���Ȃ�
                if (bHitWyvern == true)
                {
                    // ���C�o�[���^�ɃL���X�g
                    CWyvern *pWyvern = (CWyvern*)pEnemy;

                    // �푰�ɂ���āA�_���[�W���ς��
                    float fHitDamage = (float)m_nDamage;
                    if (m_tribe == TRIBE_ENEMY)
                    {
                        // �G�̔����Ȃ�_���[�W����
                        fHitDamage *= WYVERN_ENEMY_EXPLOSION_ATTENUATION_RATE;
                    }

                    // �_���[�W�𓖂������ꏊ�ɉ����Čv�Z
                    pWyvern->TakeDamage(pos, (int)fHitDamage, nCntHit);
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

        // ���@�̈ʒu���擾
        D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

        // �����̃T�C�Y��ݒ�
        D3DXVECTOR3 collisionSize = m_collisionSize;

        // �����̔����Ȃ�
        if (m_tribe == TRIBE_PLAYER)
        {
            // �T�C�Y����
            collisionSize *= BULLET_EXPLOSION_SIZE_ATTENUATION_FOR_PLAYER_RATE;
        }

        // �������Ă���Ȃ�
        if (RectangleCollision3D(&explosionPos, &playerPos,
            &collisionSize, &PLAYER_COLLISION_SIZE) == true)
        {
            // ���@�Ƀ_���[�W(�_���[�W�����L��)
            float fDamage = (float)m_nDamage * BULLET_EXPLOSION_ATTENUATION_FOR_PLAYER_RATE;
            CGame::GetPlayer()->TakeDamage((int)fDamage, m_posOld);
        }
    }
}

//=============================================================================
// ������������
// Author : �㓡�T�V��
//=============================================================================
void CBullet::SetExplosion(D3DXVECTOR3 pos)
{
    // �R���W�����T�C�Y�ɂ���āA�������锚���̑傫����ς���
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
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CBullet::Draw(void)
{
    // �e�̓��f�����o���Ȃ��ق������h�����ǂ������̂ŁADraw���Ȃ�
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
HRESULT CBullet::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //==============================
    // �A�T���g���C�t���̒e
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bullet.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_AR], NULL, &m_aNumMat[TYPE_AR], &m_apMesh[TYPE_AR]);

    //==============================
    // ��
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bullet.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BLOOD], NULL, &m_aNumMat[TYPE_BLOOD], &m_apMesh[TYPE_BLOOD]);

    //==============================
    // �_
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Acid.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_ACID], NULL, &m_aNumMat[TYPE_ACID], &m_apMesh[TYPE_ACID]);

    //==============================
    // ����̔��e
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bomb.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_BOMB], NULL, &m_aNumMat[TYPE_BOMB], &m_apMesh[TYPE_BOMB]);

    //==============================
    // ����
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bomb.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_EXPLOSION], NULL, &m_aNumMat[TYPE_EXPLOSION], &m_apMesh[TYPE_EXPLOSION]);

    //==============================
    // �A�T���g���C�t���̒e
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bullet.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_CLAP], NULL, &m_aNumMat[TYPE_CLAP], &m_apMesh[TYPE_CLAP]);

    //==============================
    // �Ռ��g
    //==============================
    // ���f��
    D3DXLoadMeshFromX(LPCSTR("data/MODEL/Bomb.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL,
        &m_apBuffMat[TYPE_SHOCK], NULL, &m_aNumMat[TYPE_SHOCK], &m_apMesh[TYPE_SHOCK]);

    return S_OK;
}

//=============================================================================
// ���f���f�[�^�j������
// Author : �㓡�T�V��
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
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 collisionSize, D3DXVECTOR3 moveAngle, TYPE type, TRIBE tribe)
{
    // �|�C���^��p��
    CBullet *pBullet = NULL;

    // �������m��
    pBullet = new CBullet;

    // �^�C�v��R�Â�(�������̒���Bind������̂����邽�߁A�ŏ��ɕR�Â�)
    pBullet->m_type = type;

    // ������
    pBullet->Init(pos, DEFAULT_SCALE);

    // 1F�O�̈ʒu���A�ʒu�ƌ��т���
    pBullet->m_posOld = pos;

    // �ړ��̊p�x�����т���
    pBullet->m_moveAngle = moveAngle;

    // �푰�����т���
    pBullet->m_tribe = tribe;

    // �����蔻��̑傫�������߂�
    pBullet->m_collisionSize = collisionSize;

    // ��ނ��ƂɁA���т���l��ς���
    CSound *pSound = CManager::GetSound();
    switch (type)
    {
    case TYPE_AR:

        pBullet->m_fSpeed = BULLET_AR_SPEED;
        pBullet->m_nLife = BULLET_AR_LIFE;
        pBullet->m_nDamage = BULLET_AR_DAMAGE;

        if (tribe == TRIBE_PLAYER)
        {
            // SE���Đ�
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
            // SE���Đ�
            pSound->Play(CSound::LABEL_SE_CANON_SHOT);
        }

        break;

    case TYPE_EXPLOSION:

        // �R���W�����T�C�Y�ɂ���āA���C�t��_���[�W�����߂�
        if (collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV1 && collisionSize.x < BULLET_EXPLOSION_COLLISION_SIZE_LV2)
        {
            pBullet->m_nLife = BULLET_EXPLOSION_LIFE_LV1;
            pBullet->m_nDamage = BULLET_EXPLOSION_DAMAGE_LV1;

            // SE���Đ�
            pSound->Play(CSound::LABEL_SE_EXPLOSION_SMALL);
        }
        else if (collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV2 && collisionSize.x < BULLET_EXPLOSION_COLLISION_SIZE_LV3)
        {
            pBullet->m_nLife = BULLET_EXPLOSION_LIFE_LV2;
            pBullet->m_nDamage = BULLET_EXPLOSION_DAMAGE_LV2;

            // SE���Đ�
            pSound->Play(CSound::LABEL_SE_EXPLOSION_SMALL);
        }
        else if (collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV3 && collisionSize.x < BULLET_EXPLOSION_COLLISION_SIZE_LV4)
        {
            pBullet->m_nLife = BULLET_EXPLOSION_LIFE_LV3;
            pBullet->m_nDamage = BULLET_EXPLOSION_DAMAGE_LV3;

            // SE���Đ�
            pSound->Play(CSound::LABEL_SE_EXPLOSION_MIDIUM);
        }
        else if (collisionSize.x >= BULLET_EXPLOSION_COLLISION_SIZE_LV4)
        {
            pBullet->m_nLife = BULLET_EXPLOSION_LIFE_LV4;
            pBullet->m_nDamage = BULLET_EXPLOSION_DAMAGE_LV4;

            // SE���Đ�
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
// ���Ԃ̈ʒu���擾
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CBullet::GetMiddlePos(D3DXVECTOR3 pos, D3DXVECTOR3 posOld)
{
    D3DXVECTOR3 returnPos = DEFAULT_VECTOR;

    returnPos = (pos + posOld) / 2;

    return returnPos;
}

//=============================================================================
// �q�b�g�G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBullet::HitEffect(D3DXVECTOR3 pos, HIT hit)
{
    // ���擾
    CSound *pSound = CManager::GetSound();

    // �������ɓ���������
    if (hit == HIT_NON_LIVING)
    {
        for (int nCnt = 0; nCnt < MODEL_EFFECT_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
        {
            // �ϐ��錾
            float fSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

            // �ړ��ʂ����߂�(�J�����̂ق��ɔ��ł��Ă���C���[�W)
            CCamera *pCamera = CManager::GetCamera();
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(pCamera->GetAngle())*fSpeedX,
                fSpeedY, -cosf(pCamera->GetAngle())*fSpeedZ);

            // ��]�ʂ����߂�
            D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

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
            CModelEffect::Create(pos, rotMove, move, rotMove, modelEffectType);
        }

        // �������ւ̃p�[�e�B�N����p�����q�b�g�G�t�F�N�g�p�̕ϐ��錾
        float fAngle = 0.0f;                       // �p�x
        float fSpeed = 0.0f;                       // ����
        float fSize = 0.0f;                        // �傫��
        CCamera *pCamera = CManager::GetCamera();  // �J�������擾
        for (int nCnt = 0; nCnt < EFFECT_HIT_NON_LIVING_NUMBER_ONCE; nCnt++)
        {
            // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

            // �������o��
            fSpeed = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SPEED, EFFECT_HIT_NON_LIVING_MIN_SPEED)) / 100.0f;

            // �ړ��ʂ����߂�
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed,
                cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

            // �傫�����o��
            fSize = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SIZE, EFFECT_HIT_NON_LIVING_MIN_SIZE)) / 100.0f;

            // �傫�������߂�
            D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

            // �G�t�F�N�g����
            CEffect3D::Create(pos, size, move, EFFECT_HIT_NON_LIVING_COLOR,
                DEFAULT_VECTOR, EFFECT_HIT_NON_LIVING_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }
    }
    // �����ɓ���������
    else if (hit == HIT_LIVING)
    {
        // �v���C���[�̒e�Ȃ�
        if (m_tribe == TRIBE_PLAYER)
        {
            // SE���Đ�
            pSound->Play(CSound::LABEL_SE_DAMAGE_ENEMY);
        }

        // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
        float fAngleX = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        float fAngleY = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        float fAngleZ = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

        // ���̃o���b�g�𐶐�
        CBullet::Create(pos, BULLET_BLOOD_COLLISION_SIZE, D3DXVECTOR3(fAngleX, fAngleY, fAngleZ), CBullet::TYPE_BLOOD, CBullet::TRIBE_LIQUID);

        // �����ւ̃q�b�g�G�t�F�N�g�p�̕ϐ��錾
        float fAngle = 0.0f;                       // �p�x
        float fSpeed = 0.0f;                       // ����
        float fSize = 0.0f;                        // �傫��
        CCamera *pCamera = CManager::GetCamera();  // �J�������擾
        for (int nCnt = 0; nCnt < EFFECT_HIT_LIVING_NUMBER_ONCE; nCnt++)
        {
            // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

            // �������o��
            fSpeed = float(GetRandNum(EFFECT_HIT_LIVING_MAX_SPEED, EFFECT_HIT_LIVING_MIN_SPEED)) / 100.0f;

            // �ړ��ʂ����߂�
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed,
                cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

            // �傫�����o��
            fSize = float(GetRandNum(EFFECT_HIT_LIVING_MAX_SIZE, EFFECT_HIT_LIVING_MIN_SIZE)) / 100.0f;

            // �傫�������߂�
            D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

            // �G�t�F�N�g����
            CEffect3D::Create(pos, size, move, EFFECT_HIT_LIVING_COLOR,
                DEFAULT_VECTOR, EFFECT_HIT_LIVING_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }
    }
    // �l�Ԃɓ���������
    else if (hit == HIT_HUMAN)
    {
        // �v���C���[�̒e�Ȃ�
        if (m_tribe == TRIBE_PLAYER)
        {
            // SE���Đ�
            pSound->Play(CSound::LABEL_SE_DAMAGE_ENEMY);
        }

        // �p�[�e�B�N����p�����q�b�g�G�t�F�N�g�p�̕ϐ��錾
        float fAngle = 0.0f;                       // �p�x
        float fSpeed = 0.0f;                       // ����
        float fSize = 0.0f;                        // �傫��
        CCamera *pCamera = CManager::GetCamera();  // �J�������擾
        for (int nCnt = 0; nCnt < EFFECT_HIT_NON_LIVING_NUMBER_ONCE; nCnt++)
        {
            // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

            // �������o��
            fSpeed = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SPEED, EFFECT_HIT_NON_LIVING_MIN_SPEED)) / 100.0f;

            // �ړ��ʂ����߂�
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed,
                cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

            // �傫�����o��
            fSize = float(GetRandNum(EFFECT_HIT_NON_LIVING_MAX_SIZE, EFFECT_HIT_NON_LIVING_MIN_SIZE)) / 100.0f;

            // �傫�������߂�
            D3DXVECTOR3 size = D3DXVECTOR3(fSize, fSize, 0.0f);

            // �G�t�F�N�g����
            CEffect3D::Create(pos, size, move, EFFECT_HIT_NON_LIVING_COLOR,
                DEFAULT_VECTOR, EFFECT_HIT_NON_LIVING_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }
    }
    // �N���X�^���ɓ���������
    else if (hit == HIT_CRYSTAL)
    {
        // �v���C���[�̒e�Ȃ�
        if (m_tribe == TRIBE_PLAYER)
        {
            // SE���Đ�
            pSound->Play(CSound::LABEL_SE_DAMAGE_CRYSTAL);
        }

        for (int nCnt = 0; nCnt < MODEL_EFFECT_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
        {
            // �ϐ��錾
            float fSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_SPEED_MAX, 0)) / 100.0f;
            float fRotSpeedX = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedY = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            float fRotSpeedZ = float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f - float(GetRandNum(MODEL_EFFECT_COLLAPSE_ROT_MAX, MODEL_EFFECT_COLLAPSE_ROT_MIN)) / 100.0f;
            CModelEffect::TYPE modelEffectType = CModelEffect::TYPE_DEBRI_SMALL_001;

            // �ړ��ʂ����߂�(�J�����̂ق��ɔ��ł��Ă���C���[�W)
            CCamera *pCamera = CManager::GetCamera();
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(pCamera->GetAngle())*fSpeedX,
                fSpeedY, -cosf(pCamera->GetAngle())*fSpeedZ);

            // ��]�ʂ����߂�
            D3DXVECTOR3 rotMove = D3DXVECTOR3(D3DXToRadian(fRotSpeedX), D3DXToRadian(fRotSpeedY), D3DXToRadian(fRotSpeedZ));

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
            CModelEffect::Create(pos, rotMove, move, rotMove, modelEffectType);
        }

        // �G�t�F�N�g����
        float fAngle = 0.0f;    //�p�x
        float fSpeed = EFFECT_HIT_MINI_CRYSTAL_SPEED;   //����
        CCamera*pCamera = CManager::GetCamera();
        for (int nCnt = 0; nCnt < EFFECT_HIT_CRYSTAL_NUMBER_ONCE; nCnt++)
        {
            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->GetAngle())*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->GetAngle())*fSpeed);

            // �G�t�F�N�g����
            CEffect3D::Create(pos, EFFECT_HIT_MINI_CRYSTAL_SIZE, move, EFFECT_HIT_CRYSTAL_COLOR,
                DEFAULT_VECTOR, EFFECT_HIT_MINI_CRYSTAL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }
    }
}

//=============================================================================
// AR�̃G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBullet::AREffect(D3DXVECTOR3 pos)
{
    // 1F�o�߂����ォ��A�g���C�����o��
    if (m_nLife <= BULLET_AR_LIFE - 1)
    {
        // �g���C������
        float fSpeed = BULLET_AR_TRAIL_SPEED;
        CEffect3D::Create(pos, m_collisionSize * 3, m_moveAngle * fSpeed, EFFECT_AR_TRAIL_COLOR,
            DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

        // �g���C������
        CEffect3D::Create(pos, m_collisionSize * 2, m_moveAngle * fSpeed, D3DXCOLOR(1.0f, 0.99f, 0.0f, 0.8f),
            DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

        // �g���C������(���Ԉʒu)
        CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 3, m_moveAngle * fSpeed, EFFECT_AR_TRAIL_COLOR,
            DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

        // �g���C������(���Ԉʒu)
        CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2, m_moveAngle * fSpeed, D3DXCOLOR(1.0f, 0.99f, 0.0f, 0.8f),
            DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
    }

    // ���������߂�
    float fSpeedX = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedY = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedZ = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // �ړ��ʂ�ݒ�
    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // �g���C���f�R���[�V���������݂̈ʒu�ɔ���
    CEffect3D::Create(pos, m_collisionSize * 2, move, EFFECT_AR_TRAIL_DECORATION_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // ���������߂�
    fSpeedX = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedY = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedZ = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // �ړ��ʂ�ݒ�
    move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // �g���C���f�R���[�V�������ߋ��̈ʒu�ƌ��݂̈ʒu�̒��Ԃɔ���
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2, move, EFFECT_AR_TRAIL_DECORATION_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
}

//=============================================================================
// ���̒e�̃G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBullet::BloodEffect(D3DXVECTOR3 pos)
{
    float fSpeed = BULLET_BLOOD_TRAIL_SPEED;

    // �g���C������
    CEffect3D::Create(pos, m_collisionSize * 2, m_moveAngle * fSpeed, EFFECT_BLOOD_TRAIL_COLOR,
        DEFAULT_VECTOR, EFFECT_BLOOD_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // ���������߂�
    float fSpeedX = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedY = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedZ = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // �ړ��ʂ�ݒ�
    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // �g���C���f�R���[�V���������݂̈ʒu�ɔ���
    CEffect3D::Create(pos, m_collisionSize * 2, move, EFFECT_BLOOD_TRAIL_DECORATION_COLOR,
        DEFAULT_VECTOR, EFFECT_BLOOD_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // ���������߂�
    fSpeedX = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedY = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedZ = float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_BLOOD_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // �ړ��ʂ�ݒ�
    move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // �g���C���f�R���[�V�������ߋ��̈ʒu�ƌ��݂̈ʒu�̒��Ԃɔ���
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2, move, EFFECT_BLOOD_TRAIL_DECORATION_COLOR,
        DEFAULT_VECTOR, EFFECT_BLOOD_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
}

//=============================================================================
// �_�U���̃G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBullet::AcidEffect(D3DXVECTOR3 pos)
{
    // ���������߂�
    float fSpeedX = float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedY = float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedZ = float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_ACID_ATTACK_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // �ړ��ʂ�ݒ�
    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // �g���C���f�R���[�V���������݂̈ʒu�ɔ���
    CEffect3D::Create(pos, m_collisionSize * 2, move, EFFECT_ACID_ATTACK_TRAIL_DECORATION_COLOR,
        DEFAULT_VECTOR, EFFECT_ACID_ATTACK_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
}

//=============================================================================
// ����̔��e�̃G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBullet::BombEffect(D3DXVECTOR3 pos)
{
    // �g���C������
    float fSpeed = BULLET_BOMB_TRAIL_SPEED;
    CEffect3D::Create(pos, m_collisionSize * 2.5f, m_moveAngle * fSpeed, EFFECT_BOMB_AROUND_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // �g���C������
    CEffect3D::Create(pos, m_collisionSize * 1.75f, m_moveAngle * fSpeed, EFFECT_BOMB_CENTER_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // �g���C������(���Ԉʒu)
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2.5f, m_moveAngle * fSpeed, EFFECT_BOMB_AROUND_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // �g���C������(���Ԉʒu)
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 1.75f, m_moveAngle * fSpeed, EFFECT_BOMB_CENTER_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // ���������߂�
    float fSpeedX = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedY = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    float fSpeedZ = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // �ړ��ʂ�ݒ�
    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // �g���C���f�R���[�V���������݂̈ʒu�ɔ���
    CEffect3D::Create(pos, m_collisionSize * 2, move, EFFECT_BOMB_AROUND_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // ���������߂�
    fSpeedX = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedY = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;
    fSpeedZ = float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f - float(GetRandNum(EFFECT_AR_TRAIL_DECORATION_MAX_SPEED, 0)) / 100.0f;

    // �ړ��ʂ�ݒ�
    move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

    // �g���C���f�R���[�V�������ߋ��̈ʒu�ƌ��݂̈ʒu�̒��Ԃɔ���
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2, move, EFFECT_BOMB_AROUND_COLOR,
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_DECORATION_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
}

//=============================================================================
// �����̃G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBullet::ExplosionEffect(D3DXVECTOR3 pos)
{
    // �R���W�����T�C�Y�ɂ���āA�ړ��ʂƃt�F�[�h�A�E�g���������߂�
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

    // �������K���L���΂��J�E���^�ɂȂ�����
    if (m_nCntTime == BULLET_EXPLOSION_CREATE_SMALL_COLLAPSE_FRAME)
    {
        // �������K���L���΂�
        for (int nCnt = 0; nCnt < nSmallCollapseNumberOnce; nCnt++)
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

            // �ʒu�����߂�
            D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*m_collisionSize.x / 2,
                0.0f, -cosf(fAngle)*m_collisionSize.x / 2) + pos;

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
    }

    // �����̉��𔭐�
    for (int nCnt = 0; nCnt < EFFECT_EX_NUMBER_ONCE; nCnt++)
    {
        // ���������߂�(Y���́A������̂�)
        float fSpeedX = float(GetRandNum(nMaxSpeed_XZ, 0)) / 100.0f - float(GetRandNum(nMaxSpeed_XZ, 0)) / 100.0f;
        float fSpeedY = float(GetRandNum(EFFECT_EX_MAX_SPEED_Y, 0)) / 100.0f;
        float fSpeedZ = float(GetRandNum(nMaxSpeed_XZ, 0)) / 100.0f - float(GetRandNum(nMaxSpeed_XZ, 0)) / 100.0f;

        // �ړ��ʂ�ݒ�
        D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ);

        // �ԂƉ��F��2�F���o��
        D3DXCOLOR color = DEFAULT_COLOR;
        if (nCnt == 0)
        {
            color = EFFECT_EX_RED_COLOR;
        }
        else
        {
            color = EFFECT_EX_YELLOW_COLOR;
        }

        // ���݂̈ʒu���甭��
        CEffect3D::Create(pos, m_collisionSize, move, color,
            DEFAULT_VECTOR, fFadeOutRate, CEffect3D::TYPE_EXPLOSION);
    }
}

//=============================================================================
// �N���b�v�̃G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBullet::ClapEffect(D3DXVECTOR3 pos)
{
    // �g���C������
    float fSpeed = BULLET_CLAP_TRAIL_SPEED;

    // �g���C������
    CEffect3D::Create(pos, m_collisionSize * 3, m_moveAngle * fSpeed, D3DXCOLOR(1.0f, 0.99f, 0.0f, 1.0f),
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);

    // �g���C������(���Ԉʒu)
    CEffect3D::Create(GetMiddlePos(pos, m_posOld), m_collisionSize * 2, m_moveAngle * fSpeed, D3DXCOLOR(1.0f, 0.99f, 0.0f, 1.0f),
        DEFAULT_VECTOR, EFFECT_AR_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
}

//=============================================================================
// �Ռ��g�̃G�t�F�N�g
// Author : �㓡�T�V��
//=============================================================================
void CBullet::ShockEffect(D3DXVECTOR3 pos)
{
    // �������K���L���΂�
    for (int nCnt = 0; nCnt < MODEL_EFFECT_BLUE_EXPLOSION_SMALL_COLLAPSE_NUMBER_ONCE; nCnt++)
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

        // �ʒu�����߂�
        D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*m_collisionSize.x / 2 + pos.x,
            0.0f, -cosf(fAngle)*m_collisionSize.x / 2 + pos.z);

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

    // �����𔭐�
    for (int nCnt = 0; nCnt < EFFECT_EX_NUMBER_ONCE; nCnt++)
    {
        // ���������߂�(Y���́A������̂�)
        float fSpeedX = float(GetRandNum(EFFECT_EX_MAX_SPEED_LV4, 0)) / 100.0f - float(GetRandNum(EFFECT_EX_MAX_SPEED_LV4, 0)) / 100.0f;
        float fSpeedFireY = float(GetRandNum(EFFECT_EX_MAX_SPEED_Y, 0)) / 100.0f;
        float fSpeedZ = float(GetRandNum(EFFECT_EX_MAX_SPEED_LV4, 0)) / 100.0f - float(GetRandNum(EFFECT_EX_MAX_SPEED_LV4, 0)) / 100.0f;

        // �ړ��ʂ�ݒ�
        D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX, fSpeedFireY, fSpeedZ);

        // �ԂƉ��F��2�F���o��
        D3DXCOLOR color = DEFAULT_COLOR;
        if (nCnt == 0)
        {
            color = EFFECT_BLUE_FIRE_FIRST_COLOR;
        }
        else
        {
            color = EFFECT_BLUE_FIRE_BLUE;
        }

        // ���݂̈ʒu���甭��
        CEffect3D::Create(pos, m_collisionSize, move, color,
            DEFAULT_VECTOR, EFFECT_EX_FADE_OUT_RATE_LV4, CEffect3D::TYPE_BLUE_FIRE);
    }
}
