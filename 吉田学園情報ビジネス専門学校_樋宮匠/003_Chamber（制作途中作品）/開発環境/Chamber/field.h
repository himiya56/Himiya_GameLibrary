//=============================================================================
//
// �w�i���� [wall.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _WALL_H_
#define _WALL_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "tile.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FIELD_SIZE 2000
#define FIELD_NUM 2
#define MAX_TILE_LINE 80   // ���̃^�C���̍ő吔

//*****************************************************************************
// �v���C���[�N���X��`
//*****************************************************************************
class CField : public CObject
{
public:
	CField();
	~CField();

	typedef enum
	{
		FLOOR = 0,
		CEILING,
	}TYPE;

	HRESULT Init(void);
	void Update();
	void Draw();
	void Uninit();
	void LoadMapData(TYPE type, int nWidth, int nDepth);
	static CField *Create(TYPE type, int nWidth, int nDepth);
	void SetField(int nWidth, int nDepth);
private:
	// �����o�ϐ�
	CTile						*m_apTile[MAX_TILE_LINE][MAX_TILE_LINE];			// 3D�|���S���ւ̃|�C���^
	int							m_aGround[MAX_TILE_LINE][MAX_TILE_LINE];
};
#endif