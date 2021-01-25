//=============================================================================
//
// �e���� [bullet.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "bullet.h"
#include "manager.h"
#include "particle.h"
#include "sound.h"
#include "object.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "equipment.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBullet::CBullet()
{
	// �����o�ϐ�
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �o���b�g�̈ړ���
	m_nLife = BULLET_LIFE;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ��]����p�x
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float rot, BULLET_TYPE type)
{
	// ������
	CPolygon3D::Init();

	m_move = move;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, rot); // �p�x
	m_BulletType = type;

	if (CEquipment::IsUse(CEquipment::EQUIPMENT_LASERSIGHT))
	{
		m_nLife = BULLET_LIFE * 1.5;
	}
	else
	{
		m_nLife = BULLET_LIFE;
	}

	SetPos(pos);
	SetRot(m_rot);
	SetSize(BULLET_WIDTH, BULLET_HEIGHT);

	// �e�N�X�`���̍쐬
	BindTexture(m_pTexture);
	SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update()
{
	// �e�̌��ݒn���擾
	D3DXVECTOR3 pos = GetPosition();
	// �T�E���h�I�u�W�F�N�g���擾
	CSound *pSound = CManager::GetSound();

	// �e�̎��������炷
	m_nLife--;

	// �e�̍��W�𓮂���
	pos += m_move;

	// �e�̓����蔻��
	// �e�����ɂ��Ԃ����Ă��Ȃ��Ȃ���s
	if (!BulletHit(pos, pSound))
	{
		// �������Ȃ��Ȃ�����I�u�W�F�N�g��j��
		if (m_nLife < 0)
		{
			// �����̐���
			pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
			CParticle::SetParticle(pos);
			Uninit();
		}
		else
		{
			// �e�̏����X�V
			SetPos(pos);
			SetRot(m_rot);

			CPolygon3D::Update();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw()
{
	CPolygon3D::Draw();
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit()
{
	CPolygon3D::Uninit();
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CBullet::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Bullet.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CBullet::Unload(void)
{
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float rot, BULLET_TYPE type)
{
	CBullet *pBullet;
	pBullet = new CBullet;
	pBullet->SetObjType(OBJ_TYPE_BULLET);
	pBullet->Init(pos, move, rot, type);

	// SE���Đ�
	CSound *pSound = CManager::GetSound();
	pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
	
	return pBullet;
}

//=============================================================================
// �e�̓����蔻��
//=============================================================================
// �e���Ԃ����true��Ԃ�
bool CBullet::BulletHit(D3DXVECTOR3 pos, CSound *pSound)
{
	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		CObject *pObject = CObject::GetObj(nCount);
		if (pObject != NULL)
		{
			D3DXVECTOR3 objPos = ((CPolygon3D *)pObject)->GetPosition();
			if (pos.x <= objPos.x + POLYGON_SIZE / 4 &&
				pos.x >= objPos.x - POLYGON_SIZE / 4 &&
				pos.y <= objPos.y + POLYGON_SIZE / 4 &&
				pos.y >= objPos.y - POLYGON_SIZE / 4)
			{
				switch (pObject->GetObjType())
				{
					case OBJ_TYPE_BLOCK:
						// �����̐���
						Uninit();
						pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
						CParticle::SetParticle(objPos);
						return true;
						break;
					case OBJ_TYPE_ENEMY:
						Uninit();
						pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
						CParticle::SetParticle(objPos);
						return true;
					default:
						break;
				}
			}
		}
	}
}
