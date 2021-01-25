//=============================================================================
//
// �e���� [bullet.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _BULLET_H_
#define _BULLET_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_HEIGHT 30	// �e�̍���
#define BULLET_WIDTH  30    // �e�̕�
#define BULLET_LIFE   20	// �e�̎���

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CSound;

//*****************************************************************************
// �o���b�g�N���X��`
//*****************************************************************************
class CBullet :public CPolygon3D
{
public:
	CBullet();
	~CBullet();

	// �񋓌^�錾
	typedef enum
	{
		BULLET_NONE = 0,
		BULLET_PLAYER,
		BULLET_ENEMY
	}BULLET_TYPE;

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float rot, BULLET_TYPE type);
	void Update();
	void Draw();
	void Uninit();
	static HRESULT Load(void);
	static void Unload(void);
	static CBullet * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float rot, BULLET_TYPE type);
private:
	bool BulletHit(D3DXVECTOR3 pos, CSound *pSound);
	static LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3						m_move;		    // �|���S���̈ړ���
	int								m_nLife;		// �e�̎���
	D3DXVECTOR3						m_rot;			// ��]����p�x
	BULLET_TYPE						m_BulletType;
};
#endif