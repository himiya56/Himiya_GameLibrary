//=============================================================================
//
// 自作簡易処理関数群 [Library.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _LIBRARY_H_
#define _LIBRARY_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "d3dx9.h"
#include <dinput.h>

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

#pragma comment(lib,"dsound.lib")

#pragma comment(lib,"winmm.lib")

//*****************************************************************************
// 自作関数
//*****************************************************************************
void Delay(const int nSecond);
int GetRandom(const int min, const int max);
int Sum(int nValue, ...);
 char *NumberToChar(const int nNumber);
 char *NumberToChar(const float fNumber);
 float Vec3Length(D3DXVECTOR3 *pVec);
#endif
