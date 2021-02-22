//=============================================================================
//
// 制限時間の処理 [timer.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"

//================================================
// 前方宣言
//================================================
class CNumber;

//=============================================================================
//マクロ定義
//=============================================================================

#define TIMER_NUMBER_SIZE_X 40   // 制限時間の数字の縦のサイズ
#define TIMER_NUMBER_SIZE_Y 50   // 制限時間の数字の横のサイズ

// タイマーの位置とサイズ
#define TIMER_POS D3DXVECTOR3(SCREEN_WIDTH / 2 + 55,SCREEN_HEIGHT - 45.0f,0.0f)
#define TIMER_SIZE D3DXVECTOR3(TIMER_NUMBER_SIZE_X,TIMER_NUMBER_SIZE_Y,0.0f)

#define MAX_SECONDS_DIGIT 2 // タイマーの秒の桁の数
#define MAX_MINUTES_DIGIT 1 // タイマーの分の桁の数

// 開始時の秒
#define START_TIMER_SECONDS 59

// 開始時の分
#define MINUTES_TIMER 9

//=============================================================================
//クラス定義
//=============================================================================
class CTimer :public CScene
{
public:

    CTimer();  
    ~CTimer();

    static CTimer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size); 
    void Update(void);                               
    void Uninit(void);                               
    void Draw(void);                                

    static bool GetTimeUp(void) { return m_bTimeUp; }    // タイムアップを取得

private:
    static bool m_bTimeUp;                               // タイムアップしたかどうか
    CNumber *m_pMinutesNumber;                           // 分の情報
    CNumber *m_apSecondsNumber[MAX_SECONDS_DIGIT];       // 秒の情報
    int m_nSeconds;                                      // 秒のカウンタ
    int m_nMinutes;                                      // 分のカウンタ
    int m_nCntFrame;                                     // フレームをカウント
};
#endif