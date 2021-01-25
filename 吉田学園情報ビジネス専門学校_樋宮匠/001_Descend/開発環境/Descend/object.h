//=============================================================================
//
// �|���S������ [object.h]
// Author : ��{ ��
//
//=============================================================================

// ��d�C���N���[�h�h�~�p
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �I�u�W�F�N�g�N���X��`
//*****************************************************************************
class CObject
{
public:
	// �����o�֐�
	CObject();
	virtual ~CObject();

	// �񋓌^�錾
	typedef enum
	{
		OBJ_TYPE_NONE = 0,
		OBJ_TYPE_HUD,
		OBJ_TYPE_EXPLOSION,
		OBJ_TYPE_PLAYER,
		OBJ_TYPE_ENEMY,
		OBJ_TYPE_BOSS,
		OBJ_TYPE_BULLET,
		OBJ_TYPE_ITEM,
		OBJ_TYPE_EFFECT,
		OBJ_TYPE_BLOCK,
		OBJ_TYPE_BASE,
		OBJ_TYPE_CLEAR,
		OBJ_TYPE_WALL,
		OBJ_TYPE_MAX
	}OBJ_TYPE;

	// �������z�֐�
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static  void UpdateAll(void);
	static  void DrawAll(void);
	static  void ReleaseAll(void);	// �S�I�u�W�F�N�g�j���֐�
	static	CObject *GetObj(int nIndex);
	void SetObjType(const OBJ_TYPE type); // �I�u�W�F�N�g�^�C�v��ݒ�
	OBJ_TYPE GetObjType(void);			 // �I�u�W�F�N�g�^�C�v�̎擾
protected:
	void Release(void);		// �I�u�W�F�N�g�j���֐�
	OBJ_TYPE m_Type;
private:
	// �����o�ϐ�
	static CObject *m_apObject[MAX_POLYGON]; // Object�C���X�^���X
	static int		m_nNumAll;				 // �������ꂽObject�h���N���X�̃C���X�^���X�̐�
	int m_nNumIndex;
};

#endif

