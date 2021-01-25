//=============================================================================
//
// �G���� [enemy.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _ENEMY_H_
#define _ENEMY_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_SEG_X 1.0f   // �v���C���[�e�N�X�`���̉�������
#define ENEMY_SEG_Y 1.0f   // �v���C���[�e�N�X�`���̏c������
#define SLIME_LIFE 10
#define BAT_LIFE 10
#define ENEMY_SPEED 1

//*****************************************************************************
// �o���b�g�N���X��`
//*****************************************************************************
class CEnemy :public CPolygon3D
{
public:
	CEnemy();
	~CEnemy();
	typedef enum
	{
		TYPE_SLIME = 0,
		TYPE_BAT,
		TYPE_EYE,
		TYPE_MAX
	}ENEMY_TYPE;

	HRESULT Init(D3DXVECTOR3 pos, int life, ENEMY_TYPE type);
	void Update();
	void Draw();
	void Uninit();
	void Collision(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	static CEnemy * Create(D3DXVECTOR3 pos, ENEMY_TYPE type);
	ENEMY_TYPE GetEnemyType(void) { return m_enemyType; }
private:
	static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];	    // �e�N�X�`���ւ̃|�C���^
	ENEMY_TYPE						m_enemyType;	// �G�l�~�[�^�C�v
	int								m_nLife;
	int								m_nCountMove;
	D3DXVECTOR3						m_pos;
	D3DXVECTOR3						m_move;			// �|���S���̈ړ���
	D3DXVECTOR3						m_rot;
	bool							m_bChase;
};
#endif