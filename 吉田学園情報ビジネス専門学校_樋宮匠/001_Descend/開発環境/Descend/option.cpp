//=============================================================================
//
// �I�v�V�������� [option.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "manager.h"
#include "object.h"
#include "player.h"
#include "option.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 COption::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
COption::COption()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
COption::~COption()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT COption::Init(void)
{
	// ������
	CPolygon3D::Init();
	D3DXVECTOR3 playerPos = CPlayer::GetPlayerPos();

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �p�x
	m_pos = D3DXVECTOR3(playerPos.x, playerPos.y + POLYGON_SIZE, 0.0f);

	SetPos(m_pos);
	SetRot(m_rot);
	SetSize(POLYGON_SIZE, POLYGON_SIZE);

	// �e�N�X�`���̍쐬
	BindTexture(m_pTexture);
	SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void COption::Update()
{
	// �v���C���[�̓���փz�[�~���O
	D3DXVECTOR3 playerPos = CPlayer::GetPlayerPos();

	float distX = playerPos.x - m_pos.x;
	float distY = playerPos.y + POLYGON_SIZE - m_pos.y;

	float fAngle = atan2f(distY, distX);

	if (playerPos.x + 10 > m_pos.x &&
		playerPos.x - 10 < m_pos.x &&
		playerPos.y + POLYGON_SIZE + 10 > m_pos.y &&
		playerPos.y + POLYGON_SIZE - 10 < m_pos.y)
	{
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_move = D3DXVECTOR3(cos(fAngle)*DRONE_SPEED, sin(fAngle)*DRONE_SPEED, 0.0f);
	}

	m_pos += m_move;

	SetPos(m_pos);

	CPolygon3D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void COption::Draw()
{
	CPolygon3D::Draw();
}

//=============================================================================
// �I������
//=============================================================================
void COption::Uninit()
{
	CPolygon3D::Uninit();
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT COption::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Drone.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void COption::Unload(void)
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
COption * COption::Create(void)
{
	COption *pOption;
	pOption = new COption;
	pOption->SetObjType(OBJ_TYPE_BULLET);
	pOption->Init();
	return pOption;
}