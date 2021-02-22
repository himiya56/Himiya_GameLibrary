//===============================================
//
// �G�t�F�N�g3D���� (effect3d.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
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
// �ÓI�����o�ϐ��錾
//========================================
LPDIRECT3DTEXTURE9 CEffect3D::m_apTexture[TYPE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
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
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CEffect3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �r���{�[�h�̏�����
    CBillboard::Init(pos, size);

    // �e�N�X�`�����o�C���h
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CEffect3D::Uninit(void)
{
    CBillboard::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CEffect3D::Update(void)
{
    // �ʒu�Ƒ傫�����擾
    D3DXVECTOR3 pos = GetPos();
    D3DXVECTOR3 size = GetSize();

    // �g�p����t���O
    bool bUse = true;

    // �A�j���[�V����������������ǂ���
    bool bOneRound = false;

    switch (m_type)
    {
    case TYPE_PURPLE_FIRE:

        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;

        // ���ɋ߂Â��Ă���
        m_col.r -= EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_R;
        m_col.g -= EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_G;

        break;

    case TYPE_PARTICLE:

        // ���񂾂�k��
        size.x -= m_fFadeOutRate;
        size.y -= m_fFadeOutRate;

        // �傫���𔽉f
        SetSize(size);

        break;

    case TYPE_RELOADING:

        // ���񂾂�k��
        size.x -= m_fFadeOutRate;
        size.y -= m_fFadeOutRate;

        // �E��Ɉړ�
        GetMoveToRightHand(pos);

        // �傫���𔽉f
        SetSize(size);

        break;

    case TYPE_WAVE:

        // ���񂾂�g��
        size.x += EFFECT_WAVE_EXTEND_SIZE;
        size.y += EFFECT_WAVE_EXTEND_SIZE;

        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;

        // �傫���𔽉f
        SetSize(size);

        break;

    case TYPE_GUN_MARKS:

        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;

        // �Ԃɋ߂Â��Ă���
        m_col.g -= EFFECT_YELLOW_FIRE_TO_RED_RATE;

        break;


    case TYPE_CRACK1:
        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;
        break;

    case TYPE_CRACK2:
        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;
        break;

    case TYPE_CRACK3:
        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;
        break;

    case TYPE_CRACK4:
        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;
        break;

    case TYPE_CHARGE:

        // ����ɋz��
        GetAngleToLeftHand(pos);

        // ���ɋ߂Â��Ă���
        m_col.r -= EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_R;
        m_col.g -= EFFECT3D_WHITE_FIRE_TO_PURPLE_RATE_G;

        // �t�F�[�h�A�E�g�J�n�̃t���O��true�Ȃ�
        if (m_bStartFadeOut == true)
        {
            // ���񂾂񓧖���
            m_col.a -= m_fFadeOutRate;
        }
        else
        {
            // �d�����
            //// �g���C���G�t�F�N�g����
            //CEffect3D::Create(pos, size / 2, m_move * EFFECT_CHARGE_TRAIL_MOVE_CHANGE_RATE, EFFECT_CHARGE_TRAIL_COLOR,
            //    DEFAULT_VECTOR, EFFECT_CHARGE_TRAIL_FADE_OUT_RATE, CEffect3D::TYPE_PARTICLE);
        }

        break;

    case TYPE_SMOKE:

        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;

        // ���ɋ߂Â��Ă���
        m_col.r -= EFFECT_SMOKE_COLOR_TO_BLACK_RATE;
        m_col.g -= EFFECT_SMOKE_COLOR_TO_BLACK_RATE;
        m_col.b -= EFFECT_SMOKE_COLOR_TO_BLACK_RATE;

        // �A�j���[�V�����̃J�E���^���~�߂�t���O��false�Ȃ�
        if (m_bStartFadeOut == false)
        {
            // �J�E���g�A�b�v
            m_nCntTime++;

            // �e�N�X�`�����W���X�V
            if (m_nCntTime == EFFECT_SMOKE_UPDATE_PATTERN_SPEED)
            {
                bOneRound = SetUpdateTexLeftToRight(EFFECT_SMOKE_MAX_PATTERN);

                // �J�E���^���Z�b�g
                m_nCntTime = 0;

                // ���������A�A�j���[�V�����̃J�E���^���~�߂�
                if (bOneRound == true)
                {
                    m_bStartFadeOut = true;
                }
            }
        }

        break;

    case TYPE_ACID:

        // ���񂾂�g��
        size.x += EFFECT_ACID_EXTEND_SIZE;
        size.y += EFFECT_ACID_EXTEND_SIZE;

        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;

        // ���񂾂�΂�
        m_col.r -= EFFECT_ACID_TO_GREEN_RATE_R;

        // �傫���𔽉f
        SetSize(size);

        break;

    case TYPE_ACID_ATTACK:

        // ���񂾂�g��
        size.x += EFFECT_ACID_EXTEND_SIZE;
        size.y += EFFECT_ACID_EXTEND_SIZE;

        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;

        // ���񂾂񎇂�
        m_col.r -= EFFECT_ACID_ATTACK_TO_PURPLE_RATE;

        // �傫���𔽉f
        SetSize(size);

        break;

    case TYPE_WAVE_ATTACK:

        // ���񂾂�g��
        size.x += EFFECT_BLUE_FIRE_EXTEND_RATE;
        size.y += EFFECT_BLUE_FIRE_EXTEND_RATE;

        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;

        // �ɋ߂Â��Ă���
        m_col.g -= EFFECT_WHITE_FIRE_TO_BLUE_RATE;
        m_col.r -= EFFECT_WHITE_FIRE_TO_BLUE_RATE;

        // �傫���𔽉f
        SetSize(size);

        break;

    case TYPE_BURST_CENTER:

        // ���񂾂�g��
        size.x += EFFECT_BURST_CENTER_EXTEND_SIZE;
        size.y += EFFECT_BURST_CENTER_EXTEND_SIZE;

        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;

        // ���񂾂񉩐F��
        m_col.r += EFFECT_BURST_CENTER_TO_YELLOW_RATE_R;

        // ���F�ɒB������
        if (m_col.r >= 1.0f)
        {
            // ���񂾂�ԐF��
            m_col.g -= EFFECT_BURST_CENTER_TO_RED_RATE_G;
        }

        // ���ӂ̎��ӂ𐶐�
        SetBurstAround(pos, size);

        // ���J�E���^�Ŕ�������
        m_nCntTime++;
        if (m_nCntTime == EFFECT_BURST_CENTER_EXPLOSION_FRAME)
        {
            CBullet::Create(pos, D3DXVECTOR3(BULLET_EXPLOSION_COLLISION_SIZE_LV3, BULLET_EXPLOSION_COLLISION_SIZE_LV3, 
                BULLET_EXPLOSION_COLLISION_SIZE_LV3), DEFAULT_VECTOR,
                CBullet::TYPE_EXPLOSION, CBullet::TRIBE_ENEMY);
        }

        // �傫���𔽉f
        SetSize(size);

        break;

    case TYPE_BURST_AROUND:

        // ���񂾂�g��
        size.x += EFFECT_BURST_AROUND_EXTEND_SIZE;
        size.y += EFFECT_BURST_AROUND_EXTEND_SIZE;

        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;

        // �傫���𔽉f
        SetSize(size);

        break;

    case TYPE_SPRAY:

        // ���񂾂񓧖���
        m_col.a -= m_fFadeOutRate;

        // ���񂾂��]������
        m_fRotAngle += m_fSpeed;

        // �ړ��ʂ𐧌�
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

        // �傫���𔽉f
        SetSize(size);

        break;

    case TYPE_EXPLOSION:

        // ���񂾂񓧖���
        m_col.a -= EFFECT_EX_DOWN_ALPHA_RATE;

        // ���񂾂�k��
        size.x -= m_fFadeOutRate;
        size.y -= m_fFadeOutRate;

        // ���񂾂�Ԃ�
        m_col.g -= EFFECT_EX_TO_RED_RATE;

        // ���J�E���^�ŁA�ړ��ʕω�
        m_nCntTime++;
        if (m_nCntTime > EFFECT_EX_CHANGE_MOVE_START_COUNT)
        {
            pos.y += EFFECT_EX_CHANGE_MOVE_RATE;
            m_move.y = 0.0f;
        }

        // ��]�̑��x���p�x�ɑ���
        m_fRotAngle += m_fSpeed;

        // �傫���𔽉f
        SetSize(size);

        break;

    case TYPE_BLUE_FIRE:

        // ���񂾂񓧖���
        m_col.a -= EFFECT_EX_DOWN_ALPHA_RATE;

        // ���񂾂�k��
        size.x -= m_fFadeOutRate;
        size.y -= m_fFadeOutRate;

        // �ɋ߂Â��Ă���
        m_col.g -= EFFECT_WHITE_FIRE_TO_BLUE_RATE;
        m_col.r -= EFFECT_WHITE_FIRE_TO_BLUE_RATE;

        // ���J�E���^�ŁA�ړ��ʕω�
        m_nCntTime++;
        if (m_nCntTime > EFFECT_EX_CHANGE_MOVE_START_COUNT)
        {
            pos.y += EFFECT_EX_CHANGE_MOVE_RATE;
            m_move.y = 0.0f;
        }

        // ��]�̑��x���p�x�ɑ���
        m_fRotAngle += m_fSpeed;

        // �傫���𔽉f
        SetSize(size);

        break;
    }

    // �ړ�
    pos += m_move;

    // �ʒu�A�F�𔽉f
    SetPos(pos);
    SetCol(m_col);

    // �r���{�[�h�̍X�V���A�^�C�v���ƂŐ؂�ւ���
    if (m_type == TYPE_SPRAY || m_type == TYPE_EXPLOSION || m_type == TYPE_BLUE_FIRE)
    {
        // �r���{�[�h���X�V
        CBillboard::Update(m_fRotAngle);
    }
    else
    {
        // �r���{�[�h���X�V
        CBillboard::Update();
    }

    // �����A�傫�����Ȃ��Ȃ�g�p�t���O��false��
    if (m_col.a <= 0.0f || size.x < 0.0f || size.y < 0.0f)
    {
        bUse = false;
    }

    // �g�p�t���O��false�Ȃ�A����
    if (bUse == false)
    {
        Uninit();
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
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
// �f�[�^�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
HRESULT CEffect3D::Load(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �e�N�X�`���̓ǂݍ���
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
// ���f���f�[�^�j������
// Author : �㓡�T�V��
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
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CEffect3D * CEffect3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 rot, float fFadeOutRate, TYPE type)
{
    // �������̊m��
    CEffect3D *pEffect3D = NULL;

    // �^�C�v���ƂɁAZ�o�b�t�@�𖳌�������G�t�F�N�g���ǂ��������߂�
    if (type == TYPE_ACID || type == TYPE_ACID_ATTACK || type == TYPE_WAVE_ATTACK
        || type == TYPE_BURST_CENTER || type == TYPE_BURST_AROUND)
    {
        pEffect3D = new CEffect3D(CScene::OBJTYPE_EFFECT3D_Z_NONE);
    }
    else
    {
        pEffect3D = new CEffect3D(CScene::OBJTYPE_EFFECT3D);
    }

    // ���������m�ۏo������
    if (pEffect3D != NULL)
    {
        // ��ނ����т��Ă���
        pEffect3D->m_type = type;

        // ������
        pEffect3D->Init(pos, size);

        // �r���{�[�h�֐ݒ�𔽉f
        pEffect3D->SetRot(rot);
        pEffect3D->SetCol(col);

        // ��ނ��Ƃ̐ݒ�
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

        // ��������A�����o�ϐ������т���
        pEffect3D->m_col = col;
        pEffect3D->m_move = move;
        pEffect3D->m_fFadeOutRate = fFadeOutRate;
    }

    return pEffect3D;
}

//=============================================================================
// �����_���ɉ�]���x�����߂�
// Author : �㓡�T�V��
//=============================================================================
float CEffect3D::GetRandRotSpeed(int nMax)
{
    float fReturnSpeed = 0.0f;

    // �����_���ɒl���擾
    fReturnSpeed = float(GetRandNum(nMax, 0)) / 100.0f - float(GetRandNum(nMax, 0)) / 100.0f;

    // �l���p�x�ɕς���
    fReturnSpeed = D3DXToRadian(fReturnSpeed);

    return fReturnSpeed;
}

//=============================================================================
// ����ւ̊p�x�����߂�
// Author : �㓡�T�V��
//=============================================================================
void CEffect3D::GetAngleToLeftHand(D3DXVECTOR3 pos)
{
    // �J�E���^���Z
    m_nCntTime++;

    // �J�E���^�����ȏ�Ńz�[�~���O�J�n
    if (m_nCntTime >= EFFECT_CHARGE_START_HOMING)
    {
        // ���@�̍���̈ʒu���擾
        CPlayer *pPlayer = NULL;
        pPlayer = CGame::GetPlayer();
        CCharacter*pCharacter = NULL;
        pCharacter = (CCharacter*)pPlayer;
        D3DXVECTOR3 leftHandPos = D3DXVECTOR3(pCharacter->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._41,
            pCharacter->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._42,
            pCharacter->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._43);

        float fTargetPosX = 0.0f;                  // �ڕW��X���W
        float fTargetPosZ = 0.0f;                  // �ڕW��Y���W
        float fMyPosX = 0.0f;                      // ���g��X���W
        float fMyPosZ = 0.0f;                      // ���g��Y���W
        float fAngle = 0.0f;                       // ���߂�p�x

        // �ڕW�̍��W�Ǝ��g�̍��W
        fTargetPosX = leftHandPos.x, fTargetPosZ = leftHandPos.z;
        fMyPosX = pos.x, fMyPosZ = pos.z;

        // �p�x�����߂�
        fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosZ - fTargetPosZ));

        // ���ړ��̒l�����߂�
        m_move.x = -sinf(fAngle) * m_fSpeed;
        m_move.z = -cosf(fAngle) * m_fSpeed;

        // �v���C���[�Ƃ̋����𑪂�
        float fDistance = sqrtf(
            powf((leftHandPos.x - pos.x), 2) +
            powf((leftHandPos.z - pos.z), 2));

        // �v���C���[�Ƃ̍����̍��𑪂�
        float fHeight = fabsf((leftHandPos.y) - pos.y);

        // �p�x�����߂�
        float fAngleY = atan2(fDistance, fHeight);

        // �c�̈ړ��ʂ����߂�
        m_move.y = cosf(fAngleY) * m_fSpeed;

        // ����̈ʒu�̂ق����A���g�̈ʒu���Ⴂ�Ȃ�
        if (leftHandPos.y < pos.y)
        {
            // Y���̈ړ��p�x���t�ɂ���
            m_move.y *= -1;
        }

        // �t�F�[�h�A�E�g�̊J�n�t���O��false��
        if (m_bStartFadeOut == false)
        {
            // �ړI�n�ɋ߂��Ȃ�
            if (fDistance <= EFFECT_CHARGE_CONSIDER_CLOSE_DEST && fHeight <= EFFECT_CHARGE_CONSIDER_CLOSE_DEST_Y)
            {
                // �������Ă���Ȃ�
                if (pPlayer != NULL)
                {
                    if (pPlayer->GetDisp() == true)
                    {
                        //// �����擾
                        //CSound*pSound = CManager::GetSound();
                        //pSound->Play(CSound::LABEL_SE_ORB);

                        // �I�[�u���㏸
                        pPlayer->SetRisingLeftHandOrbs();
                    }
                }

                // �t�F�[�h�A�E�g�J�n
                m_bStartFadeOut = true;
            }
        }
    }
}

//=============================================================================
// �E��ւ̈ړ��ʂ����߂�
// Author : �㓡�T�V��
//=============================================================================
void CEffect3D::GetMoveToRightHand(D3DXVECTOR3 pos)
{
    // ���@�̉E��̈ʒu���擾
    CPlayer *pPlayer = NULL;
    pPlayer = CGame::GetPlayer();
    CCharacter*pCharacter = NULL;
    pCharacter = (CCharacter*)pPlayer;
    D3DXVECTOR3 rightHandPos = D3DXVECTOR3(pCharacter->GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._41,
        pCharacter->GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._42,
        pCharacter->GetParts(CPlayer::PARTS_RHAND)->GetWorldMtx()._43);

    float fTargetPosX = 0.0f;                  // �ڕW��X���W
    float fTargetPosZ = 0.0f;                  // �ڕW��Y���W
    float fMyPosX = 0.0f;                      // ���g��X���W
    float fMyPosZ = 0.0f;                      // ���g��Y���W
    float fAngle = 0.0f;                       // ���߂�p�x

    // �ڕW�̍��W�Ǝ��g�̍��W
    fTargetPosX = rightHandPos.x, fTargetPosZ = rightHandPos.z;
    fMyPosX = pos.x, fMyPosZ = pos.z;

    // �p�x�����߂�
    fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosZ - fTargetPosZ));

    // ���ړ��̒l�����߂�
    m_move.x = -sinf(fAngle) * m_fSpeed;
    m_move.z = -cosf(fAngle) * m_fSpeed;

    // �������ׁA�c�ړ��𒲐�
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

    // �c�ړ��̒l�����߂�
    m_move.y = (fDistanceY / m_fSpeed) * fAdjustment;
}

//=============================================================================
// ���ӂ̎��ӂ𐶐�
// Author : �㓡�T�V��
//=============================================================================
void CEffect3D::SetBurstAround(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
    float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

    // �ݒu����ʒu�̔��a���o��
    float fRadius = float(GetRandNum(EFFECT_BURST_CENTER_MAX_RADIUS, 0));

    // �ʒu�����߂�(Y�����������������ʒu�ŌŒ�)
    D3DXVECTOR3 effectPos = D3DXVECTOR3(-sinf(fAngle)*fRadius + pos.x, EFFECT_ACID_FLOAT_FROM_SURFACE_POS, -cosf(fAngle)*fRadius + pos.z);

    CEffect3D::Create(effectPos, size*EFFECT_BURST_AROUND_FIRST_SIZE_RATE,
        DEFAULT_VECTOR, D3DXCOLOR(m_col.r, m_col.g, m_col.b, 1.0f), HIT_TOP, EFFECT_BURST_AROUND_FADE_OUT_RATE, CEffect3D::TYPE_BURST_AROUND);
}
