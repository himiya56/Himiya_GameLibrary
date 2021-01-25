//=============================================================================
//
// �w�i���� [bg.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _BG_H_
#define _BG_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "polygon3d.h"
#include "map.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_ANIM_SPEED   30
#define MAX_BG_POLYGON  3   // �|���S���̐�

//*****************************************************************************
// �w�i�N���X��`
//*****************************************************************************
class CBg :public CPolygon3D
{
public:
	CBg();
	~CBg();

	// �񋓌^�錾
	typedef enum
	{
		TEX_BG_0 = 0,
		TEX_BG_1,
		TEX_BG_2,
		TEX_BG_MAX
	}TEX_BG;

	HRESULT Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CBg * Create(void);
private:
	static LPDIRECT3DTEXTURE9		m_apTexture[TEX_BG_MAX];	 // �e�N�X�`���ւ̃|�C���^
	float							m_fTexUVY;					     // �e�N�X�`��UV�ړ��p�ϐ�
};
#endif