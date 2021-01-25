//=============================================================================
//
// �������� [explosion.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EXPLOSION_HEIGHT 50		// �e�̍���
#define EXPLOSION_WIDTH  50		// �e�̕�
#define EXPLOSION_SEG_X 8.0f	// �v���C���[�e�N�X�`���̉�������
#define EXPLOSION_SEG_Y 1.0f	// �v���C���[�e�N�X�`���̏c������
#define ANIM_SPEED		6		// �A�j���[�V�������x

//*****************************************************************************
// �o���b�g�N���X��`
//*****************************************************************************
class CExplosion :public CPolygon3D
{
public:
	CExplosion();
	~CExplosion();
	HRESULT Init(D3DXVECTOR3 pos);
	void Update();
	void Draw();
	void Uninit();
	static HRESULT Load(void);
	static void Unload(void);
	static CExplosion * Create(D3DXVECTOR3 pos);
private:
	static LPDIRECT3DTEXTURE9		m_pTexture;					 // �e�N�X�`���ւ̃|�C���^
	int								m_nCountAnim = 0;			 // �A�j���[�V�����J�E���^
};
#endif