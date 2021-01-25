//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "item.h"
#include "manager.h"
#include "sound.h"
#include "object.h"
#include "player.h"
#include "Score.h"
#include "equipment.h"
#include "magazine.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CItem::m_pTexture = NULL;
int CItem::m_nMagazineCount = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CItem::CItem()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWaitTimer = 30.0f;
	m_nMagazineCount = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CItem::~CItem()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos)
{
	// ������
	CPolygon3D::Init();

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �p�x
	m_nMagazineCount = 0;

	SetPos(pos);
	SetRot(m_rot);
	SetSize(ITEM_WIDTH, ITEM_HEIGHT);

	// �e�N�X�`���̍쐬
	BindTexture(m_pTexture);
	SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CItem::Update()
{
	// �A�C�e���̌��ݒn���擾
	D3DXVECTOR3 pos = GetPosition();
	// �T�E���h�I�u�W�F�N�g���擾
	CSound *pSound = CManager::GetSound();

	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		CObject *pObject = CObject::GetObj(nCount);
		if (pObject != NULL)
		{
			if (pObject->GetObjType() == OBJ_TYPE_PLAYER)
			{
				D3DXVECTOR3 objPos = ((CPolygon3D *)pObject)->GetPosition();

				float distX = objPos.x - pos.x;
				float distY = objPos.y - pos.y;

				float fAngle = atan2f(distY, distX);

				m_move = D3DXVECTOR3(cos(fAngle)*ITEM_SPEED, sin(fAngle)*ITEM_SPEED, 0.0f);
			}
		}
	}

	// �e�̍��W�𓮂���
	if (m_fWaitTimer <= 0.0f)
	{
		pos += m_move;
		m_fWaitTimer = 0.0f;
		//m_rot.z += D3DXToRadian(10);
	}

	// �e�̓����蔻��
	// �e�����ɂ��Ԃ����Ă��Ȃ��Ȃ���s
	if (!ItemHit(pos, pSound))
	{
		// �e�̏����X�V
		SetPos(pos);
		SetRot(m_rot);

		// �^�C�}�[�����炷
		m_fWaitTimer--;

		CPolygon3D::Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CItem::Draw()
{
	CPolygon3D::Draw();
}

//=============================================================================
// �I������
//=============================================================================
void CItem::Uninit()
{
	CPolygon3D::Uninit();
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CItem::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Crystal.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CItem::Unload(void)
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
CItem * CItem::Create(D3DXVECTOR3 pos)
{
	CItem *pItem;
	pItem = new CItem;
	pItem->SetObjType(OBJ_TYPE_ITEM);
	pItem->Init(SpawnItem(pos));
	return pItem;
}

//=============================================================================
// �A�C�e�������ʒu�v�Z����
//=============================================================================
D3DXVECTOR3 CItem::SpawnItem(D3DXVECTOR3 pos)
{
	float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
	float fRadius = float(rand() % (POLYGON_SIZE * 200)) / 100.0f;

	return D3DXVECTOR3(pos.x - sinf(fAngle) * fRadius, pos.y - cosf(fAngle)*fRadius, 0.0f);
}

//=============================================================================
// �A�C�e���̓����蔻��
//=============================================================================
// �v���C���[�ɂԂ����true��Ԃ�
bool CItem::ItemHit(D3DXVECTOR3 pos, CSound * pSound)
{
	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		CObject *pObject = CObject::GetObj(nCount);
		if (pObject != NULL)
		{
			D3DXVECTOR3 objPos = ((CPolygon3D *)pObject)->GetPosition();
			if (pos.x <= objPos.x + POLYGON_SIZE / 2 &&
				pos.x >= objPos.x - POLYGON_SIZE / 2 &&
				pos.y <= objPos.y + POLYGON_SIZE / 2 &&
				pos.y >= objPos.y - POLYGON_SIZE / 2 &&
				pObject->GetObjType() == OBJ_TYPE_PLAYER)
			{
				// SE���Đ�
				CSound *pSound = CManager::GetSound();
				// �X�R�A�����Z
				CScore::AddScore(1237 * CPlayer::GetCombo());

				// �N���X�^���n�c�f�����������Ă���Ȃ�}�K�W������
				if (CEquipment::IsUse(CEquipment::EQUIPMENT_GENERATOR))
				{
					m_nMagazineCount++;
					if (m_nMagazineCount > MAGAZINE_COUNT)
					{
						m_nMagazineCount = 0;
						CMagazine::AddMag();
					}
				}
				pSound->Play(CSound::SOUND_LABEL_SE_GETITEM);

				// �C���X�^���X�j��
				Uninit();
				return true;
			}
		}
	}
}