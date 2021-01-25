//=============================================================================
//
// �A�C�e������ [item.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _ITEM_H_
#define _ITEM_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ITEM_HEIGHT 30	// �e�̍���
#define ITEM_WIDTH  30    // �e�̕�
#define ITEM_SPEED  20
#define MAGAZINE_COUNT 2

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CSound;

//*****************************************************************************
// �o���b�g�N���X��`
//*****************************************************************************
class CItem :public CPolygon3D
{
public:
	CItem();
	~CItem();

	HRESULT Init(D3DXVECTOR3 pos);
	void Update();
	void Draw();
	void Uninit();
	static HRESULT Load(void);
	static void Unload(void);
	static CItem * Create(D3DXVECTOR3 pos);
	static 	D3DXVECTOR3 SpawnItem(D3DXVECTOR3 pos);
private:
	bool ItemHit(D3DXVECTOR3 pos, CSound *pSound);
	static LPDIRECT3DTEXTURE9		m_pTexture;					 // �e�N�X�`���ւ̃|�C���^
	static int						m_nMagazineCount;
	D3DXVECTOR3						m_move;						 // �|���S���̈ړ���
	D3DXVECTOR3						m_rot;						 // ��]����p�x
	float							m_fWaitTimer;
	bool							m_bItemMove;
};
#endif