//====================================================================
//
// 残弾処理 (bulletRemain.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BULLET_REMAIN_H_
#define _BULLET_REMAIN_H_

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
#define MAX_BULLET_REMAIN 3     // 残弾の桁数

#define BULLET_REMAIN_POS D3DXVECTOR3(1054.5f, 612.5f, 0.0f)    // 残弾の位置
#define BULLET_REMAIN_SIZE D3DXVECTOR3(80.0f, 40.0f, 0.0f)      // 残弾の大きさ

//================================================
// クラス宣言
//================================================

// 残弾のクラス
class CBulletRemain : public CScene
{
public:
    CBulletRemain();
    ~CBulletRemain();
    static CBulletRemain *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);             // 数字情報開放
    void Update(void);
    void Draw(void);
    void Disp(const int nNumber);                   // 残弾を表示

private:
    CNumber *m_apNumber[MAX_BULLET_REMAIN];  // 数字情報
};

#endif