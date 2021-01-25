//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : ��{ ��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "block.h"
#include "manager.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBlock::m_apTexture[TYPE_BLOCK_MAX] = {};
CBlock::TYPE_BLOCK CBlock::m_type = CBlock::TYPE_BLOCK_NONE;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBlock::CBlock()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBlock::~CBlock()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBlock::Init(D3DXVECTOR3 pos, TYPE_BLOCK type)
{
	CPolygon3D::Init();
	SetPos(pos);
	m_type = type;

	// �e�N�X�`���̍쐬
	BindTexture(m_apTexture[type]);
	SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);

	if (type == TYPE_BLOCK_BASE || type == TYPE_BLOCK_CLEAR)
	{
		SetCol(D3DCOLOR_RGBA(255, 255, 255, 0));
	}
	else if (type == TYPE_BLOCK_HOW_1 || 
			 type == TYPE_BLOCK_HOW_2 || 
			 type == TYPE_BLOCK_HOW_3)
	{
		SetPos(D3DXVECTOR3(pos.x + POLYGON_SIZE / 4, pos.y, pos.z));
		SetSize(POLYGON_SIZE * 3, POLYGON_SIZE * 6);
	}

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CBlock::Update(void)
{
	CPolygon3D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBlock::Draw(void)
{
	CPolygon3D::Draw();
}

//=============================================================================
// �I������
//=============================================================================
void CBlock::Uninit(void)
{
	CPolygon3D::Uninit();
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CBlock::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Block_iron.png", &m_apTexture[TYPE_BLOCK_01]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Block_02.png", &m_apTexture[TYPE_BLOCK_02]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/how02.png", &m_apTexture[TYPE_BLOCK_HOW_1]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/how01.png", &m_apTexture[TYPE_BLOCK_HOW_2]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/how03.png", &m_apTexture[TYPE_BLOCK_HOW_3]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CBlock::Unload(void)
{
	for (int nCount = 0; nCount < TYPE_BLOCK_MAX; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//	�C���X�^���X��������
//=============================================================================
CBlock * CBlock::Create(D3DXVECTOR3 pos, TYPE_BLOCK type)
{
	CBlock *pBlock;
	pBlock = new CBlock;
	pBlock->Init(pos, type);

	if (type == TYPE_BLOCK_BASE)
	{
		pBlock->SetObjType(OBJ_TYPE_BASE);
	}
	else if (type == TYPE_BLOCK_HOW_1 ||
		type == TYPE_BLOCK_HOW_2 ||
		type == TYPE_BLOCK_HOW_3 ||
		type == TYPE_BLOCK_LOGO)
	{
		pBlock->SetObjType(OBJ_TYPE_NONE);
	}
	else if (type == TYPE_BLOCK_CLEAR)
	{
		pBlock->SetObjType(OBJ_TYPE_CLEAR);
	}
	else
	{
		pBlock->SetObjType(OBJ_TYPE_BLOCK);
	}

	return pBlock;
}
