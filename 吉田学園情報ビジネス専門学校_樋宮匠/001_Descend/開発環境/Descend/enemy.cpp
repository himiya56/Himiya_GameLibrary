//=============================================================================
//
// �G���� [enemy.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "life.h"
#include "player.h"
#include "particle.h"
#include "item.h"
#include "sound.h"
#include "equipment.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[TYPE_MAX] = {};


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �|���S���̈ړ���
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �|���S���̈ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 1;
	m_nCountMove = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, int life, ENEMY_TYPE type)
{
	CPolygon3D::Init();
	m_pos = pos;
	SetPos(pos);
	m_nLife = life;
	m_enemyType = type;
	m_nCountMove = 0;

	// �e�N�X�`���̍쐬
	BindTexture(m_apTexture[m_enemyType]);
	SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update()
{
	// �T�E���h�I�u�W�F�N�g���擾
	CSound *pSound = CManager::GetSound();
	m_pos = GetPosition();

	float distX = 0.0f;
	float distY = 0.0f;
	float fAngle = 0.0f;

	m_nCountMove--;

	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		CObject *pObject = CObject::GetObj(nCount);
		if (pObject != NULL)
		{
			if (pObject->GetObjType() == OBJ_TYPE_PLAYER)
			{
				D3DXVECTOR3 objPos = ((CPolygon3D *)pObject)->GetPosition();

				distX = objPos.x - m_pos.x;
				distY = objPos.y - m_pos.y;

				fAngle = atan2f(distY, distX);

				if (m_enemyType == TYPE_SLIME && fabsf(distY) < POLYGON_SIZE * 5)
				{
					m_bChase = true;
					m_move = D3DXVECTOR3(cos(fAngle) * ENEMY_SPEED * 2, sin(fAngle) * ENEMY_SPEED / 2, 0.0f);
				}
				else if (m_enemyType == TYPE_BAT && objPos.y - POLYGON_SIZE <= m_pos.y)
				{
					m_bChase = true;
					m_move = D3DXVECTOR3(cos(fAngle) * ENEMY_SPEED * 2, sin(fAngle) * ENEMY_SPEED, 0.0f);
				}
				else if (m_enemyType == TYPE_EYE &&  fabsf(distY) < POLYGON_SIZE * 5)
				{
					m_bChase = true;
					if (m_nCountMove < 0)
					{
						int nMove = GetRandom(-180, 180);
						m_move = D3DXVECTOR3(cos(nMove) * ENEMY_SPEED, sin(nMove) * ENEMY_SPEED, 0.0f);
						m_nCountMove = 60;
					}
				}
			}
		}
	}

	if (m_bChase)
	{
		m_pos += m_move;
	}

	Collision(m_pos);

	if (m_nLife < 0)
	{
		// �����̐���
		CParticle::SetParticle(m_pos);
		pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
		CPlayer::AddCombo();

		if (CEquipment::IsUse(CEquipment::EQUIPMENT_AMPRIFICATER))
		{
			// �A�C�e���𐶐�
			for (int nCount = 0; nCount < 15; nCount++)
			{
				CItem::Create(m_pos);
			}
		}
		else
		{
			// �A�C�e���𐶐�
			for (int nCount = 0; nCount < 10; nCount++)
			{
				CItem::Create(m_pos);
			}
		}

		Uninit();
	}
	else
	{
		CPolygon3D::Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw()
{
	CPolygon3D::Draw();
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit()
{
	CPolygon3D::Uninit();
}

//=============================================================================
// �Փ˔��菈��
//=============================================================================
void CEnemy::Collision(D3DXVECTOR3 pos)
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

			// �߂荞��ł���Ƃ�(�����̗������|���S���傫����菬�����Ƃ�)
			if (distX < POLYGON_SIZE / 2 && distY < POLYGON_SIZE / 2)
			{
				switch (type)
				{
				case OBJ_TYPE_BLOCK:
					if (objPos.x - POLYGON_SIZE / 2 + 10 < pos.x && pos.x < objPos.x + POLYGON_SIZE / 2 - 10)
					{
						if (m_move.y < 0.0f)
						{
							pos.y = objPos.y + POLYGON_SIZE / 2;
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
					break;
				case OBJ_TYPE_BULLET:
					m_nLife--;
					break;
				default:
					break;
				}
			}
		}
	}

	SetPos(pos);
}

//=============================================================================
// �e�N�X�`���ǂݍ���
//=============================================================================
HRESULT CEnemy::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/fly.png", &m_apTexture[TYPE_SLIME]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/bat.png", &m_apTexture[TYPE_BAT]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/random.png", &m_apTexture[TYPE_EYE]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CEnemy::Unload(void)
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
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, ENEMY_TYPE type)
{
	CEnemy *pEnemy;
	pEnemy = new CEnemy;
	pEnemy->SetObjType(OBJ_TYPE_ENEMY);

	int nLife = 1;
	switch (type)
	{
	case TYPE_SLIME:
		nLife = SLIME_LIFE;
		break;
	case TYPE_BAT:
		nLife = SLIME_LIFE;
		break;
	default:
		break;
	}
	pEnemy->Init(pos, nLife, type);
	return pEnemy;
}