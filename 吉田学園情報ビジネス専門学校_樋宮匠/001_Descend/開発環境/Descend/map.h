//=============================================================================
//
// �}�b�v�������� [map.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _MAP_H_
#define _MAP_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "polygon2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAP_WIDTH		12  // �}�b�v�̕�
#define MAP_HEIGHT		70  // �}�b�v�̍���
#define MAX_BLOCK		MAP_WIDTH * MAP_HEIGHT

//*****************************************************************************
// �w�i�N���X��`
//*****************************************************************************
class CMap
{
public:
	CMap();
	~CMap();

	typedef enum
	{
		MAP_NONE = 0,
		MAP_TITLE,
		MAP_01,
		MAP_02,
		MAP_03,
		MAP_MAX
	}MAP_ID;

	HRESULT Init(void);
	static void GenerateMap(MAP_ID id);
	void LoadMapData(void);
	void Uninit(void);
	static CMap * Create(void);
private:
	FILE *pFile[MAP_MAX];
	static int  aMapData[MAP_MAX][MAX_BLOCK];
	static int	m_nNumBlock[MAP_MAX];
};
#endif