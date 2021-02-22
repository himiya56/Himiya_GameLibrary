//=============================================================================
//
// ����ȈՏ����֐��Q [Library.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _LIBRARY_H_
#define _LIBRARY_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "d3dx9.h"
#include <dinput.h>

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

#pragma comment(lib,"dsound.lib")

#pragma comment(lib,"winmm.lib")

//*****************************************************************************
// ����֐�
//*****************************************************************************
void Delay(const int nSecond);
int GetRandom(const int min, const int max);
int Sum(int nValue, ...);
 char *NumberToChar(const int nNumber);
 char *NumberToChar(const float fNumber);
 float Vec3Length(D3DXVECTOR3 *pVec);
#endif
