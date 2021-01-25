//=============================================================================
//
// �}�K�W������ [magazine.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _MAGAZINE_H_
#define _MAGAZINE_H_
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
#define MAX_MAGAZINE_POLYGON  2   // �|���S���̐�
#define MAX_MAG				  10  // �}�K�W���̍ő呕�U��

//*****************************************************************************
// �w�i�N���X��`
//*****************************************************************************
class CMagazine :public CObject
{
public:
	CMagazine();
	~CMagazine();

	// �񋓌^�錾
	typedef enum
	{
		TEX_MAG_0 = 0,
		TEX_MAG_01,
		TEX_MAG_MAX
	}TEX_MAG;

	HRESULT Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CMagazine * Create(void);
	static void SubtractMag(const int Value);
	static void SetMagFull(void) { m_nMag = MAX_MAG; }
	static void AddMag(void);
	static int	GetMag(void) { return m_nMag; }
private:
	static LPDIRECT3DTEXTURE9		m_apTexture[TEX_MAG_MAX];	 // �e�N�X�`���ւ̃|�C���^
	static int						m_nMag;
	CPolygon2D						*m_apPolygon[MAX_MAGAZINE_POLYGON];
	float							m_fRot = D3DXToRadian(0.0f);	 // ��]����p�x
	float							m_fSizeX = POLYGON_SIZE * MAP_WIDTH;
	float							m_fSizeY = SCREEN_HEIGHT;
	float							m_fTexUVY;					     // �e�N�X�`��UV�ړ��p�ϐ�
};
#endif