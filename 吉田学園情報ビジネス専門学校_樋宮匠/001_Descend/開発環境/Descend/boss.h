//=============================================================================
//
// �{�X���� [boss.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _BOSS_H_
#define _BOSS_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	BOSS_LIFE 100
#define BOSS_HEIGHT 8
#define BOSS_ANIM_SPEED 8
#define BOSS_ANIM_KEY 8

//*****************************************************************************
// �v���C���[�N���X��`
//*****************************************************************************
class CBoss :public CPolygon3D
{
public:
	CBoss();
	~CBoss();
	HRESULT Init(D3DXVECTOR3 pos);
	void Update();
	void Draw();
	void Uninit();
	static HRESULT Load(void);
	static void Unload(void);
	static CBoss * Create(D3DXVECTOR3 pos);
	static void Damage(void);
private:
	// �ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	static D3DXVECTOR3				m_pos;				// �|���S���̍��W
	static D3DXVECTOR3				m_move;				// �|���S���̈ړ���
	static int						m_nLife;
	float							m_fTex;
	int								m_nAnimCount;
};
#endif