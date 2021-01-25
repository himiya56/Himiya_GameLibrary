//=============================================================================
//
// ���C�t���� [life.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _LIFE_H_
#define _LIFE_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon2d.h"
#include "map.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_LIFE_POLYGON    2   // �|���S���̐�
#define LIFE_WIDTH			SCREEN_WIDTH / 4
#define LIFE_HEIGHT			50.0f

//*****************************************************************************
// ���C�t�N���X��`
//*****************************************************************************
class CLife :public CPolygon2D
{
public:
	CLife();
	~CLife();

	typedef enum
	{
		TEX_LIFE_0 = 0,
		TEX_LIFE_01,
		TEX_LIFE_MAX
	}LIFE;
	HRESULT Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CLife * Create(void);
	static void SubtractLife(void);
	static float GetLife(void) { return m_fLife; }
private:
	static LPDIRECT3DTEXTURE9		m_apTexture[TEX_LIFE_MAX];	 // �e�N�X�`���ւ̃|�C���^
	static float					m_fLife;
	CPolygon2D						*m_apPolygon[NUM_LIFE_POLYGON];
	float							m_fRot = D3DXToRadian(0.0f);	 // ��]����p�x
	float							m_fSizeX = POLYGON_SIZE * MAP_WIDTH;
	float							m_fSizeY = SCREEN_HEIGHT;
	float							m_fMaxLife;
};
#endif