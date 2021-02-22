//=============================================================================
//
// �w�i���� [wall.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "portal.h"
#include "keyboard.h"
#include "manager.h"
#include "player.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPortal::m_pTexture[PORTAL_POLYGON_NUM] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPortal::CPortal()
{
	// ������
	memset(m_pCamera, 0, sizeof(m_pCamera));
	memset(m_apPolygon3D, 0, sizeof(m_apPolygon3D));
	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		m_bUse[nCount] = false;
	}

	m_nRenderToTexture = 0;
	m_nCountUsePortal = 0;
	m_bAbleToPortal = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPortal::~CPortal()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPortal::Init()
{
	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		// �|���S�� �C���X�^���X�����E������
		m_apPolygon3D[nCount] = new CPolygon3D;
		m_apPolygon3D[nCount]->Init();
		m_apPolygon3D[nCount]->SetSize(PORTAL_HEIGHT, PORTAL_WIDTH);
		m_apPolygon3D[nCount]->SetTexUV(PORTAL_TEXTURE_UV);
		m_apPolygon3D[nCount]->SetPos(PORTAL_INIT_POS);

		// �J���� �C���X�^���X�����E������
		m_pCamera[nCount] = CCameraPortal::Create((CCameraPortal::COLOR)nCount);

		// portal������
		m_bUse[nCount] = false;
	}

	for (int nCount = CCameraPortal::COL_MAX; nCount < PORTAL_POLYGON_NUM; nCount++)
	{
		// �|���S�� �C���X�^���X�����E������
		m_apPolygon3D[nCount] = new CPolygon3D;
		m_apPolygon3D[nCount]->Init();
		m_apPolygon3D[nCount]->SetSize(PORTAL_HEIGHT, PORTAL_WIDTH);
		m_apPolygon3D[nCount]->BindTexture(m_pTexture[nCount - 2]);
		m_apPolygon3D[nCount]->SetPos(PORTAL_INIT_POS);
	}

	// OBJ_TYPE��ݒ�
	m_apPolygon3D[CCameraPortal::COL_BLUE]->SetObjType(OBJ_TYPE_PORTAL_BLUE);
	m_apPolygon3D[CCameraPortal::COL_ORANGE]->SetObjType(OBJ_TYPE_PORTAL_ORANGE);

	m_bAbleToPortal = true;
	m_nCountUsePortal = 0;

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CPortal::Update()
{
	LPDIRECT3DTEXTURE9 pTexturePortal[CCameraPortal::COL_MAX] = {};	 // �|�[�^���ւ̃e�N�X�`��

	// ���ꂼ��̔��΂�portal����̌i�F���e�N�X�`���֊��蓖��
	pTexturePortal[CCameraPortal::COL_ORANGE] = m_pCamera[CCameraPortal::COL_BLUE]->GetTexture();
	pTexturePortal[CCameraPortal::COL_BLUE] = m_pCamera[CCameraPortal::COL_ORANGE]->GetTexture();

	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		// �擾�����e�N�X�`���̓\��t��
		if (m_bUse[CCameraPortal::COL_BLUE] == m_bUse[CCameraPortal::COL_ORANGE])
		{
			m_apPolygon3D[nCount]->BindTexture(pTexturePortal[nCount]);
		}
		else
		{
			m_apPolygon3D[nCount]->BindTexture(m_pTexture[nCount + CCameraPortal::COL_MAX]);
		}

		// �|���S���̖@���擾
		D3DXVECTOR3 normal = m_apPolygon3D[nCount]->GetNormal();
		m_pCamera[nCount]->SetNormal(normal);
	}

	// �e���|�[�g����
	TeleportToPairPortal();
}

//=============================================================================
// portal�Ԉړ�����
//=============================================================================
void CPortal::TeleportToPairPortal(void)
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 BluePos, OrangePos;
	D3DXVECTOR3 BlueRot, OrangeRot;

	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		// �I�u�W�F�N�g�擾
		CObject *pObject = CObject::GetObj(nCount);

		if (pObject != NULL)
		{
			if (pObject->GetObjType() == CObject::OBJ_TYPE_PLAYER)
			{
				pos = ((CCharacter*)pObject)->GetPos();
			}
		}
	}

	// portal�̍��W�E�p�x�擾
	BluePos = m_apPolygon3D[CCameraPortal::COL_BLUE]->GetPos();
	BlueRot = m_apPolygon3D[CCameraPortal::COL_BLUE]->GetRot();

	OrangePos = m_apPolygon3D[CCameraPortal::COL_ORANGE]->GetPos();
	OrangeRot = m_apPolygon3D[CCameraPortal::COL_ORANGE]->GetRot();

	// �@���̌��������Ƃɔ͈͂��v�Z
	D3DXVECTOR3 size[CCameraPortal::COL_MAX];
	size[CCameraPortal::COL_BLUE] = m_apPolygon3D[CCameraPortal::COL_BLUE]->GetNormal() * PORTAL_RANGE;
	size[CCameraPortal::COL_ORANGE] = m_apPolygon3D[CCameraPortal::COL_ORANGE]->GetNormal() * PORTAL_RANGE;

	// portal���g�p�ł���Ȃ�
	if (m_bAbleToPortal)
	{
		// �|�[�^���ɏd�Ȃ�����Ή�����portal�ɓ]��
		if (fabsf(BluePos.x - pos.x) <= size[CCameraPortal::COL_BLUE].x &&
			fabsf(BluePos.z - pos.z) <= size[CCameraPortal::COL_BLUE].z &&
			fabsf(BluePos.y - pos.y) <= size[CCameraPortal::COL_BLUE].y &&
			m_bUse[CCameraPortal::COL_ORANGE])
		{
			CManager::GetPlayer()->SetPos(OrangePos);
			CManager::GetPlayer()->SetRot(OrangeRot);
			m_bAbleToPortal = false;
		}
		else if (fabsf(OrangePos.x - pos.x) <= size[CCameraPortal::COL_ORANGE].x &&
			fabsf(OrangePos.z - pos.z) <= size[CCameraPortal::COL_ORANGE].z &&
			fabsf(OrangePos.y - pos.y) <= size[CCameraPortal::COL_ORANGE].y &&
			m_bUse[CCameraPortal::COL_BLUE])
		{
			CManager::GetPlayer()->SetPos(BluePos);
			CManager::GetPlayer()->SetRot(BlueRot);
			m_bAbleToPortal = false;
		}
	}
	else
	{
		m_nCountUsePortal++;
		if (m_nCountUsePortal >= PORTAL_COOL_DOWN)
		{
			m_nCountUsePortal = 0;
			m_bAbleToPortal = true;
		}
	}
}

//=============================================================================
// �|�[�^�����Z�b�g�����i�������j
//=============================================================================
void CPortal::ResetPortal(void)
{
	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		m_apPolygon3D[nCount]->SetPos(PORTAL_INIT_POS);
		// portal������
		m_bUse[nCount] = false;
	}
	for (int nCount = CCameraPortal::COL_MAX; nCount < PORTAL_POLYGON_NUM; nCount++)
	{
		m_apPolygon3D[nCount]->SetPos(PORTAL_INIT_POS);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPortal::Draw()
{
}

//=============================================================================
// �I������
//=============================================================================
void CPortal::Uninit()
{
	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		// �J�����I��
		m_pCamera[nCount]->Uninit();
		// �|���S���I��
		m_apPolygon3D[nCount]->Uninit();
	}
}

//=============================================================================
// �|�[�^����������
//=============================================================================
void CPortal::SetPortal(D3DXVECTOR3 pos, D3DXVECTOR3 angle ,CCameraPortal::COLOR col)
{
	m_apPolygon3D[col]->SetPos(pos);
	m_apPolygon3D[col + CCameraPortal::COL_MAX]->SetPos(pos);

	m_apPolygon3D[col]->SetRot(angle);
	m_apPolygon3D[col + CCameraPortal::COL_MAX]->SetRot(angle);

	// ���W�ݒ�
	m_pCamera[col]->SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));

	m_bUse[col] = true;
}

//=============================================================================
// �|�[�^�����W�擾�p
//=============================================================================
D3DXVECTOR3 CPortal::GetPos(CCameraPortal::COLOR col)
{
	return m_apPolygon3D[col]->GetPos();
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CPortal::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/blue.png", &m_pTexture[CCameraPortal::COL_BLUE]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/orange.png", &m_pTexture[CCameraPortal::COL_ORANGE]);

	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/blue_close.png", &m_pTexture[CCameraPortal::COL_BLUE + CCameraPortal::COL_MAX]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/orange_close.png", &m_pTexture[CCameraPortal::COL_ORANGE + CCameraPortal::COL_MAX]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CPortal::Unload(void)
{
	for (int nCount = 0; nCount < CCameraPortal::COL_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CPortal * CPortal::Create(void)
{
	CPortal *pPortal;
	pPortal = new CPortal;
	pPortal->Init();
	pPortal->SetObjType(OBJ_TYPE_PORTAL);
	return pPortal;
}
