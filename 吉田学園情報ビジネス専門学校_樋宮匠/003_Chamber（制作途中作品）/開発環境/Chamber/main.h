//=============================================================================
//
// ���C������ [main.h]
// Author : ��{ ��
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "Library.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define SCREEN_HEIGHT 1080 //��ʂ̍���
#define SCREEN_WIDTH 1920 //��ʂ̉���
#define POLYGON_SIZE 70
#define NUM_VERTEX 4
#define NUM_POLYGON 2
#define PI 3.14f
#define DEFAULT_VECTOR D3DXVECTOR3(0.0f,0.0f,0.0f)
#define DEFAULT_SCALE  D3DXVECTOR3(1.0f,1.0f,1.0f)
#define FPS 60

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;
class CObject;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float		rhw;
	D3DCOLOR	col;
	D3DXVECTOR2	tex;
}VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DCOLOR	col;
	D3DXVECTOR2	tex;
}VERTEX_3D;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int GetFPS(void);

#endif
