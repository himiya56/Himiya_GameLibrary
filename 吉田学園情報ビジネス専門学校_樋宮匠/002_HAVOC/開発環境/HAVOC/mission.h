//====================================================================
//
// ミッション処理 (mission.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _MISSION_H_
#define _MISSION_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================
class CNumber;

//================================================
// マクロ定義
//================================================
#define MAX_MISSION_NUMBER 2                              // ミッションの桁数

#define MISSION_POS D3DXVECTOR3(300.0f, 295.5f, 0.0f)     // ミッションの数字の位置
#define MISSION_SIZE D3DXVECTOR3(42.5f, 30.0f, 0.0f)      // ミッションの数字の大きさ

//================================================
// クラス宣言
//================================================

// ミッションのクラス
class CMission : public CScene
{
public:
    CMission();
    ~CMission();
    static CMission *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    void DispFirst(const int nNumber);                     // 1番目のミッションを表示
    void DispSecond(const int nNumber);                    // 2番目のミッションを表示

private:
    CNumber *m_apFirstNumber[MAX_MISSION_NUMBER];          // 1番目のミッションの情報
    CNumber *m_apSecondNumber[MAX_MISSION_NUMBER];         // 2番目のミッションの情報
};

#endif