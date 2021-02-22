//==========================================================================================
//
// キャラクターの処理 (character.h)
// Author : 樋宮匠(クラス設計、階層構造) / 後藤慎之助(複数当たり判定、HPやダメージ周りの管理)
//
//==========================================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"
#include "model.h"
#include "minimap.h"

//================================================
// マクロ定義
//================================================
#define CHARCTER_PARTS_MAX 25       // キャラクターのパーツの最大数
#define CHARCTER_COLLISION_MAX 18   // キャラクターの当たり判定の最大数

// プレイヤーが敵に触れた時に与えるダメージ(種類別)
#define PLAYER_TOUCH_SPIDER 375
#define PLAYER_TOUCH_ANT    250
#define PLAYER_TOUCH_UFO    300
#define PLAYER_TOUCH_TOWER  1000
#define PLAYER_TOUCH_WYVERN 500

//================================================
// クラス宣言
//================================================
class CAnimation;

// キャラクタークラス
class CCharacter : public CScene
{
public:
    CCharacter();
    CCharacter(OBJTYPE objType);
    ~CCharacter();

    // 種族(animation.hにも追加が必要)
    typedef enum
    {
        TRIBE_NONE = 0,
        TRIBE_PLAYER,			// プレイヤー
        TRIBE_SPIDER,			// 蜘蛛
        TRIBE_ANT,              // 蟻
        TRIBE_FORCE,            // 防衛軍
        TRIBE_UFO,              // UFO
        TRIBE_TOWER,            // タワー
        TRIBE_WYVERN,           // ワイバーン
        TRIBE_MAX			    // 種類の最大数
    }TRIBE;

    // 当たり判定の情報
    typedef struct
    {
        D3DXVECTOR3 pos;        // 位置
        D3DXVECTOR3 size;       // 矩形のサイズ
    }COLLISION;

    // ラストヒット
    typedef enum
    {
        LAST_HIT_PLAYER = 0,	// プレイヤーの攻撃
        LAST_HIT_ENEMY,	        // 敵の攻撃
    }LAST_HIT;

    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    HRESULT LoadModelData(char* cFilePass);

    // セッター
    void SetPartPos(int index, D3DXVECTOR3 pos) { m_apModel[index]->SetPos(pos); }
    void SetPartRot(int index, D3DXVECTOR3 rot) { m_apModel[index]->SetRot(rot); }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetTribe(TRIBE tribe) { m_tribe = tribe; }
    void SetPartNum(int nPartNum) { m_nPartsNum = nPartNum; }
    void SetLife(const int nLife) { m_nLife = nLife; }
    void SetTakeDamage(const int nDamage, const LAST_HIT lastHit) { m_nLife = m_nLife - nDamage; m_lastHit = lastHit; }
    void SetHealing(const int nHeal) { m_nLife = m_nLife + nHeal; }         // 回復
    void SetPosOld(const D3DXVECTOR3 posOld) { m_posOld = posOld; }

    void BindMesh(TRIBE type, int nPartIndex, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat);

    // ゲッター
    D3DXVECTOR3 GetDefaultPos(int index) { return m_posDefault[index]; }
    D3DXVECTOR3 GetPartsRot(int index) { return m_apModel[index]->GetRot(); }
    CModel * GetParts(int index) { return m_apModel[index]; }
    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetRot(void) { return m_rot; }
    TRIBE GetTribe(void) { return m_tribe; }
    CAnimation * GetAnimation(void) { return m_pAnimation; }
    int GetLife(void) { return m_nLife; }
    D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
    LAST_HIT GetLastHit(void) { return m_lastHit; }
    bool GetBossSetting(void) { return m_bBossSetting; }

    // 当たり判定に関するゲッターやセッター
    void SetActiveCollisionSize(const D3DXVECTOR3 collisionSize) { m_activeCollisionSize = collisionSize; }
    D3DXVECTOR3 GetActiveCollisionSize(void) { return m_activeCollisionSize; }
    COLLISION GetCollision(int nCnt) { return m_aCollision[nCnt]; }                         // 各パーツの、衝突に関する情報を返す
    void SetCollisionPos(int nCnt, D3DXVECTOR3 pos) { m_aCollision[nCnt].pos = pos; }       // 各パーツの、衝突の位置を設定
    void SetCollisionSize(int nCnt, D3DXVECTOR3 size) { m_aCollision[nCnt].size = size; }   // 各パーツの、衝突のサイズを設定
    int GetCntCollision(void) { return m_nCntCollision; }                                   // 当たり判定の数を返す
    void SetCntCollision(int nCnt) { m_nCntCollision = nCnt; }                              // 当たり判定の数を設定

    // アニメーションを使用しない場合のセッター
    void SetUnableAnimation(void) { m_bUseAnimation = false; }

    // ボス仕様に変えるときのセッター
    void SetBossSetting(void) { m_bBossSetting = true; }

private:
    LPD3DXMESH				m_pMesh[TRIBE_MAX][CHARCTER_PARTS_MAX];
    LPD3DXBUFFER			m_pBuffMat[TRIBE_MAX][CHARCTER_PARTS_MAX];
    DWORD					m_nNumMat[TRIBE_MAX][CHARCTER_PARTS_MAX];
    D3DXVECTOR3				m_posDefault[CHARCTER_PARTS_MAX];
    int						m_nIndexParent[CHARCTER_PARTS_MAX];

    D3DXVECTOR3		m_pos;                                              // 全ての親の位置
    D3DXVECTOR3		m_rot;                                              // 全ての親の向き
    TRIBE           m_tribe;                                            // 種族
    int				m_nPartsNum;                                        // パーツ数

    CModel			*m_apModel[CHARCTER_PARTS_MAX];                     // モデルへのポインタ
    CAnimation		*m_pAnimation;                                      // アニメーションへのポインタ
    CMinimap		*m_pMinimap;                                        // ミニマップへのポインタ

    D3DXVECTOR3 m_activeCollisionSize;                                  // 能動的な当たり判定の大きさ(壁に対して使ったりする)
    int m_nLife;                                                        // HP
    D3DXVECTOR3		m_posOld;                                           // 1F前の位置

    char* m_cFilePass;                                                  // ファイルのパス

    COLLISION m_aCollision[CHARCTER_COLLISION_MAX];                     // 当たり判定の情報
    int m_nCntCollision;                                                // 当たり判定の数

    LAST_HIT m_lastHit;                                                 // 最後に攻撃してきたもの

    bool m_bUseAnimation;												// アニメーションさせるかどうか
    bool m_bBossSetting;                                                // ボス仕様にするかどうか
};

#endif