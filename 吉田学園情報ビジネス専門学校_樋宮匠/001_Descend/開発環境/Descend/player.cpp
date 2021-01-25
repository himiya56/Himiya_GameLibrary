//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "player.h"
#include "keyboard.h"
#include "manager.h"
#include "weapon.h"
#include "sound.h"
#include "object.h"
#include "life.h"
#include "magazine.h"
#include "enemy.h"
#include "particle.h"
#include "effect.h"
#include "fade.h"
#include "block.h"
#include "select.h"
#include "equipment.h"
#include "logo.h"
#include "mic.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
D3DXVECTOR3 CPlayer::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CPlayer::m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
bool CPlayer::m_bCameraFollowing = true;
int CPlayer::m_nCombo = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer()
{	
	// �����o�ϐ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�̍��W
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�̈ړ���
	m_fGravDest = PLAYER_GRAVITY;
	m_nInvisibleTimer = 30;
	m_nDir = DIR_RIGHT;
	m_bJump = false;
	m_bShot = false;
	m_bHit = false;
	m_nCombo = 0;
	m_nCountDeath = 180;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	CPolygon3D::Init();

	m_nInvisibleTimer = 30;
	m_nDir = DIR_RIGHT;
	m_bJump = false;
	m_bShot = false;
	m_bHit = false;
	m_bCameraFollowing = true;
	m_nCombo = 0;
	m_nCountDeath = 180;

	m_pos = pos;
	SetPos(m_pos);
	// �e�N�X�`���̍쐬
	BindTexture(m_pTexture);
	SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update()
{
	if (CLogo::GetPlaying())
	{
		if (!m_bDeath)
		{
			// �v���C���[�����擾
			CInputKeyboard *pInput = CManager::GetInput();
			m_pos = GetPosition();

			// ���ֈړ�
			if (pInput->GetKeyboardPress(DIK_A) || pInput->GetKeyboardPress(DIK_LEFT))
			{
				m_move.x = -PLAYER_SPEED;
				m_nDir = DIR_LEFT;
			}
			// �E�ֈړ�
			if (pInput->GetKeyboardPress(DIK_D) || pInput->GetKeyboardPress(DIK_RIGHT))
			{
				m_move.x = PLAYER_SPEED;
				m_nDir = DIR_RIGHT;
			}

			if (pInput->GetKeyboardTrigger(DIK_1))
			{
				CWeapon::SetWeaponType(CWeapon::WEAPON_MACHINEGUN);
				CLife::SubtractLife();
				CEquipment::SetEquipment(CEquipment::EQUIPMENT_LASERSIGHT);
			}
			if (pInput->GetKeyboardTrigger(DIK_2))
			{
				CWeapon::SetWeaponType(CWeapon::WEAPON_TRIPLE);
			}
			if (pInput->GetKeyboardTrigger(DIK_3))
			{
				CWeapon::SetWeaponType(CWeapon::WEAPON_PUNCHER);
			}

			// �W�����v�Ƌ󒆂ł̏���
			switch (m_bJump)
			{
			case true:
				// �d�͂����񂾂񋭂�����
				m_fGravDest += PLAYER_GRAVITY / 10.0f;

				// �e��łĂ�悤�ɂ���
				if (pInput->GetKeyboardTrigger(DIK_SPACE) || pInput->GetKeyboardTrigger(DIK_RSHIFT))
				{
					m_bShot = true;
					m_nFireRate = 10;
				}
				// �����[�X�Œe��łĂȂ��悤��
				if (pInput->GetKeyboardRelease(DIK_SPACE) || pInput->GetKeyboardRelease(DIK_RSHIFT))
				{
					m_bShot = false;
					m_nFireRate = 0;
					m_fGravDest = PLAYER_GRAVITY / 2;
				}
				// �e���łĂ邩�L�[��������Ă���Ȃ���s
				if (m_bShot && pInput->GetKeyboardPress(DIK_SPACE) || pInput->GetKeyboardPress(DIK_RSHIFT))
				{
					m_nFireRate++;

					// �}�K�W���̎c�e����1�ȏ�Ȃ�ł�
					if (m_nFireRate > FIRE_RATE && CMagazine::GetMag() >= 1)
					{
						CWeapon::CreateShot(D3DXVECTOR3(m_pos.x, m_pos.y - POLYGON_SIZE / 4, 0.0f));
						m_move.y += 10.0f;
						m_fGravDest = 20.0f;
						m_nFireRate = 0;
					}
				}
				// �d�͂̏C��
				if (m_fGravDest > PLAYER_GRAVITY)
				{
					m_fGravDest = PLAYER_GRAVITY;
				}
				break;
			case false:
				if (pInput->GetKeyboardTrigger(DIK_SPACE) || pInput->GetKeyboardTrigger(DIK_RSHIFT))
				{
					m_move.y = 0.0f;
					m_fGravDest = 150.0f;
					m_bJump = true;
				}
				break;
			default:
				break;
			}

			// �_���[�W���̏���
			if (m_bHit)
			{
				m_nInvisibleTimer--;

				SetCol(D3DCOLOR_RGBA(255, 0, 0, 120));
				if (m_nInvisibleTimer <= 0)
				{
					m_nInvisibleTimer = 60;
					m_bHit = false;
					SetCol(D3DCOLOR_RGBA(255, 255, 255, 255));
				}
			}

			// �����̌v�Z
			m_move.x += (0.0f - m_move.x) * FRICTION;
			m_move.y += (m_fGravDest - m_move.y) * FRICTION;

			// �ʒu�̈ړ�
			m_pos.x += m_move.x;
			m_pos.y += m_move.y;

			// �����蔻��
			Collision(m_pos);
			SetDir(m_nDir);
		}
		else
		{
			m_nCountDeath--;
			if (m_nCountDeath < 0)
			{
				Uninit();
				CFade::SetFade(CManager::MODE_RESULT);
				m_bDeath = false;
			}
		}
	}

	if (CLife::GetLife() <= 0.0f)
	{
		if (!m_bDeath)
		{
			m_bDeath = true;

			for (int nCount = 0; nCount < 5; nCount++)
			{
				CParticle::SetParticle(m_pos);
			}
		}
	}
	else
	{
		CPolygon3D::Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw()
{
	if (CLife::GetLife() > 0.0f)
	{
		CPolygon3D::Draw();
	}
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit()
{
	CPolygon3D::Uninit();
}

//=============================================================================
// �Փ˔��菈��
//=============================================================================
void CPlayer::Collision(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g�������[�v
	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		// �I�u�W�F�N�g�擾
		CObject *pObject = CObject::GetObj(nCount);

		if (pObject != NULL)
		{
			// �I�u�W�F�N�g�^�C�v���擾
			CObject::OBJ_TYPE type = pObject->GetObjType();

			// ���W���擾
			D3DXVECTOR3 objPos = ((CPolygon3D *)pObject)->GetPosition();

			// x�̋������v�Z
			float distX = fabsf(objPos.x - pos.x);

			// y�̋������v�Z
			float distY = fabsf(objPos.y - pos.y);

			switch (type)
			{
			case OBJ_TYPE_BLOCK:
				// �߂荞��ł���Ƃ�(�����̗������|���S���傫����菬�����Ƃ�)
				if (distX < POLYGON_SIZE / 2 && distY < POLYGON_SIZE / 2)
				{
					if (objPos.x - POLYGON_SIZE / 2 + 10 < pos.x && pos.x < objPos.x + POLYGON_SIZE / 2 - 10)
					{
						if (m_move.y < 0.0f && m_pos.y > objPos.y)
						{
							pos.y = objPos.y + POLYGON_SIZE / 2;
							m_fGravDest = 0.0f;
							m_bJump = false;
							m_nFireRate = 0;
							m_nCombo = 0;
							CMagazine::SetMagFull();
						}
						if (m_move.y > 0.0f)
						{
							pos.y = objPos.y - POLYGON_SIZE / 2;
						}
					}
					if (objPos.y + POLYGON_SIZE / 2 - 10 > pos.y && pos.y > objPos.y - POLYGON_SIZE / 2 + 10)
					{
						if (pos.x > objPos.x)
						{
							pos.x = objPos.x + POLYGON_SIZE / 2;
						}
						if (pos.x < objPos.x)
						{
							pos.x = objPos.x - POLYGON_SIZE / 2;
						}
					}
				}
				else
				{
					m_fGravDest = PLAYER_GRAVITY;
				}
				break;
			case OBJ_TYPE_ENEMY:
				// �߂荞��ł���Ƃ�(�����̗������|���S���傫����菬�����Ƃ�)
				if (distX < POLYGON_SIZE / 2 && distY < POLYGON_SIZE / 2)
				{
					if (!m_bHit)
					{
						CLife::SubtractLife();
						m_bHit = true;
					}
				}
				break;
			case OBJ_TYPE_BASE:
				if (pos.y < objPos.y)
				{
					m_bCameraFollowing = false;
				}
				else
				{
					m_bCameraFollowing = true;
				}
				break;
			case OBJ_TYPE_CLEAR:
				if (pos.y < objPos.y - POLYGON_SIZE)
				{
					if (CManager::GetCurrentMode() != CManager::MODE_TITLE && CManager::GetCurrentMode() != CManager::MODE_GAME_STAGE_03)
					{
						CFade::SetFade(CManager::MODE_EQUPMENT);
					}
					else
					{
						CFade::SetFade(CManager::GetNextStage());

					}
				}

				break;
			default:
				break;
			}
		}
	}

	SetPos(pos);
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CPlayer::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/rocket.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CPlayer::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer;
	pPlayer->SetObjType(OBJ_TYPE_PLAYER);
	pPlayer->Init(pos);
	return pPlayer;
}

//=============================================================================
// �_���[�W���̏���
//=============================================================================
void CPlayer::Damage(void)
{
	CLife::SubtractLife();
}