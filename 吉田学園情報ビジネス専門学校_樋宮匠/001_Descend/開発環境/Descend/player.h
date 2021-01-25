//=============================================================================
//
// �v���C���[���� [player.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _PLAYER_H_
#define _PLAYER_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FRICTION 0.2f		 // ����
#define PLAYER_SEG_X 1.0f	 // �v���C���[�e�N�X�`���̉�������
#define PLAYER_SEG_Y 1.0f	 // �v���C���[�e�N�X�`���̏c������
#define PLAYER_SPEED 5.0f	 // �v���C���[�̈ړ����x
#define PLAYER_GRAVITY -5.0f  // �d�͂̋���
#define FIRE_RATE 10

//*****************************************************************************
// �v���C���[�N���X��`
//*****************************************************************************
class CPlayer :public CPolygon3D
{
public:
	CPlayer();
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos);
	void Update();
	void Draw();
	void Uninit();
	void Collision(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	static CPlayer * Create(D3DXVECTOR3 pos);
	static void Damage(void);
	static D3DXVECTOR3 GetPlayerPos(void) { return m_pos; }
	static D3DXVECTOR3 GetPlayerMove(void) { return m_move; }
	static void AddCombo(void) { m_nCombo++; }
	static int GetCombo(void) { return m_nCombo; }
	static bool IsCameraFollowing(void) { return m_bCameraFollowing; }
private:
	// �ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	static D3DXVECTOR3				m_pos;				// �|���S���̍��W
	static D3DXVECTOR3				m_move;				// �|���S���̈ړ���
	static bool						m_bCameraFollowing;
	static int						m_nCombo;
	// �����o�ϐ�
	float							m_fGravDest;
	int								m_nFireRate;
	int								m_nInvisibleTimer;
	int								m_nDir;
	int								m_nCountDeath;
	bool							m_bJump;
	bool							m_bShot;
	bool							m_bHit;
	bool							m_bDeath;
};
#endif