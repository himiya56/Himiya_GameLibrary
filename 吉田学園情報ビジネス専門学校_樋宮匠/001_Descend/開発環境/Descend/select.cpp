//=============================================================================
//
// �����I������ [equipment.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "keyboard.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "select.h"
#include "player.h"
#include "equipment.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSelect::m_apTexture[CEquipment::EQUIPMENT_MAX + 1] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSelect::CSelect()
{
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
	m_nSelect = SELECT_MIDDLE;
	m_fSize = POLYGON_SIZE * 2;
	m_bSize = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSelect::~CSelect()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSelect::Init(void)
{
	m_nSelect = SELECT_MIDDLE;
	m_fSize = POLYGON_SIZE * 3;
	m_bSize = true;

	// �������擾
	for (int nCount = 0; nCount < CEquipment::EQUIPMENT_MAX; nCount++)
	{
		m_bSelection[nCount] = CEquipment::IsUse(nCount);
	}

	for (int nCount = 0; nCount < SELECT_MAX; nCount++)
	{
		// ����������
		m_apPolygon[nCount] = CPolygon2D::Create();
		m_apPolygon[nCount]->Init();

		// �A�C�e���������_������
		while(1)
		{
			m_nEquipment[nCount] = rand() % CEquipment::EQUIPMENT_MAX;
			if (!m_bSelection[m_nEquipment[nCount]])
			{
				m_bSelection[m_nEquipment[nCount]] = true;
				break;
			}
		};

		// �e�N�X�`�����蓖��
		m_apPolygon[nCount]->BindTexture(m_apTexture[m_nEquipment[nCount]]);
		m_apPolygon[nCount]->SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);
		m_apPolygon[nCount]->SetCol(D3DCOLOR_RGBA(255, 255, 255, 255));

		m_apPolygon[nCount]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 4 * nCount + SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 0.0f));
		m_apPolygon[nCount]->SetSize(POLYGON_SIZE * 3, POLYGON_SIZE * 6);
	}

	m_apPolygon[3] = CPolygon2D::Create();
	m_apPolygon[3]->Init();
	m_apPolygon[3]->BindTexture(m_apTexture[CEquipment::EQUIPMENT_MAX + 1]);
	m_apPolygon[3]->SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);
	m_apPolygon[3]->SetCol(D3DCOLOR_RGBA(255, 255, 255, 255));

	m_apPolygon[3]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5, 0.0f));
	m_apPolygon[3]->SetSize(POLYGON_SIZE * 8, POLYGON_SIZE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSelect::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_SELECT_POLYGON; nCount++)
	{
		m_apPolygon[nCount]->Uninit();
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSelect::Update(void)
{
	CInputKeyboard *pInput = CManager::GetInput();

	// ���ֈړ�
	if (pInput->GetKeyboardTrigger(DIK_A) || pInput->GetKeyboardTrigger(DIK_LEFT))
	{
		m_nSelect--;

		if (m_nSelect < SELECT_LEFT)
		{
			m_nSelect = SELECT_LEFT;
		}
	}
	// �E�ֈړ�
	if (pInput->GetKeyboardTrigger(DIK_D) || pInput->GetKeyboardTrigger(DIK_RIGHT))
	{
		m_nSelect++;

		if (m_nSelect > SELECT_RIGHT)
		{
			m_nSelect = SELECT_RIGHT;
		}
	}

	if (pInput->GetKeyboardTrigger(DIK_SPACE) || pInput->GetKeyboardTrigger(DIK_RSHIFT))
	{
		CEquipment::SetEquipment(m_nEquipment[m_nSelect]);
		CFade::SetFade(CManager::GetNextStage());
	}

	if (m_bSize)
	{
		m_fSize += 0.5f;
		if (m_fSize >= (POLYGON_SIZE * 3) *1.2f)
		{
			m_bSize = false;
		}
	}
	if(!m_bSize)
	{
		m_fSize -= 0.5f;
		if (m_fSize <= POLYGON_SIZE * 3)
		{
			m_bSize = true;
		}
	}

	// �傫���̏C��
	switch (m_nSelect)
	{
	case SELECT_LEFT:
		m_apPolygon[SELECT_MIDDLE]->SetSize(POLYGON_SIZE * 3, POLYGON_SIZE * 6);
		m_apPolygon[SELECT_RIGHT]->SetSize(POLYGON_SIZE * 3, POLYGON_SIZE * 6);
		break;
	case SELECT_MIDDLE:
		m_apPolygon[SELECT_LEFT]->SetSize(POLYGON_SIZE * 3, POLYGON_SIZE * 6);
		m_apPolygon[SELECT_RIGHT]->SetSize(POLYGON_SIZE * 3, POLYGON_SIZE * 6);
		break;
	case SELECT_RIGHT:
		m_apPolygon[SELECT_LEFT]->SetSize(POLYGON_SIZE * 3, POLYGON_SIZE * 6);
		m_apPolygon[SELECT_MIDDLE]->SetSize(POLYGON_SIZE * 3, POLYGON_SIZE * 6);
		break;
	default:
		break;
	}

	m_apPolygon[m_nSelect]->SetSize(m_fSize, m_fSize * 2);

	for (int nCount = 0; nCount < MAX_SELECT_POLYGON; nCount++)
	{
		m_apPolygon[nCount]->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSelect::Draw(void)
{
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CSelect * CSelect::Create(void)
{
	CSelect *pSelect;
	pSelect = new CSelect;
	pSelect->Init();
	return pSelect;
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CSelect::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Laser_Select.png", &m_apTexture[CEquipment::EQUIPMENT_LASERSIGHT]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Drone_Select.png", &m_apTexture[CEquipment::EQUIPMENT_OPTION]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Amplificater_Select.png", &m_apTexture[CEquipment::EQUIPMENT_AMPRIFICATER]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Generator_Select.png", &m_apTexture[CEquipment::EQUIPMENT_GENERATOR]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Upgrade_Select.png", &m_apTexture[CEquipment::EQUIPMENT_MAX + 1]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CSelect::Unload(void)
{
	for (int nCount = 0; nCount < MAX_SELECT_POLYGON; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}