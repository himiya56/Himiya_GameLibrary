//=============================================================================
//
// �p�[�e�B�N������ [bullet.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PARTICLE_LIFE 30	// �e�̎���
#define PARTICLE_MOVE 3
#define PARTICLE_NUM 5

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �o���b�g�N���X��`
//*****************************************************************************
class CParticle :public CPolygon3D
{
public:
	CParticle();
	~CParticle();
	HRESULT Init(D3DXVECTOR3 pos);
	void Update();
	void Draw();
	void Uninit();
	static HRESULT Load(void);
	static void Unload(void);
	static CParticle * Create(D3DXVECTOR3 pos);
	static void SetParticle(D3DXVECTOR3 pos);
private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3						m_pos;
	D3DXVECTOR3						m_move;		    // �|���S���̈ړ���
	int								m_nLife;		// �p�[�e�B�N���̎���
};
#endif
