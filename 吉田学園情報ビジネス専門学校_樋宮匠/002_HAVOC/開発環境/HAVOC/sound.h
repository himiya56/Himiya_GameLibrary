//====================================================================
//
// サウンド処理 (sound.h)
// Author : 樋宮匠(必要な音の洗い出し) / 後藤慎之助(各所に実装)
//
//====================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// クラス宣言
//================================================

// サウンドのクラス
class CSound
{
public:
    typedef enum
    {
        // SE
        LABEL_SE_SYSTEM_PAUSE = 0,		// ポーズ画面開くとき
        LABEL_SE_SYSTEM_SELECT,			// 選択変更時の音
        LABEL_SE_SYSTEM_MODE_CHANGE,	// モードが変更されるときとかの音(ポーズ画面->タイトルなど)
        LABEL_SE_MISSION_NOTICE,		// ミッション通知音
        LABEL_SE_MACHINE_GUN_SHOT,		// マシンガン発射音(連射音ではないから実際にplayを複数回呼ぶ必要有)
        LABEL_SE_MACHINE_GUN_RELOAD,	// マシンガンリロード音
        LABEL_SE_CANON_SHOT,			// キャノン発射音
        LABEL_SE_CANON_CHARGE_WEAK,		// キャノンチャージ音(1～3段階)
        LABEL_SE_CANON_CHARGE_STRONG,	// キャノンチャージ音(4段階)
        LABEL_SE_DESTROY_METAL,			// 金属系破壊音
        LABEL_SE_DESTROY_RUBBLE,		// 瓦礫系破壊音
        LABEL_SE_DESTROY_CRYSTAL,		// クリスタル破壊音
        LABEL_SE_EXPLOSION_SMALL,		// 小爆発音
        LABEL_SE_EXPLOSION_MIDIUM,		// 中爆発音
        LABEL_SE_EXPLOSION_BIG,			// 大爆発音
        LABEL_SE_DAMAGE_PLAYER,			// プレイヤーダメージ音	(声)
        LABEL_SE_DAMAGE_ENEMY,			// エネミーダメージ音		(肉々しい音)
        LABEL_SE_DAMAGE_WYVERN,			// ワイバーンダメージ音	(鈍い金属音)
        LABEL_SE_DAMAGE_CRYSTAL,		// クリスタルダメージ音
        LABEL_SE_BOSS_ROAR,				// ボス咆哮
        LABEL_SE_BOSS_SHOT,				// 弾発射 (大砲みたいな音、使い方はマシンガンのshotと同様)
        LABEL_SE_BOSS_FLAP,				// 羽ばたき音 (SEではこいつだけが唯一ループ、stopを呼ぶ必要有)
        LABEL_SE_BOSS_WIND,				// 強風音 (サマソとか強襲で使えるかも)
        LABEL_SE_BOSS_DEATH,			// ボス撃破音
        LABEL_SE_HEAL,				    // 回復音
        LABEL_SE_ORB,				    // オーブ音
        LABEL_SE_LAND,                  // ボスの着地音
        LABEL_SE_FAIL,                  // 失敗音
        LABEL_SE_NEXT,                  // 次へ音
        LABEL_SE_CLEAR,                 // クリア音
        LABEL_SE_BUG,                   // 虫撃破音
        LABEL_SE_KICK,                  // 壁キック音
        LABEL_SE_FORCE,                 // 防衛軍撃破音
        LABEL_SE_CRACK,                 // 地面がひび割れる音
        LABEL_SE_FOOT,                  // 足音

        // BGM
        LABEL_BGM_TITLE,				// タイトル
        LABEL_BGM_STAGE_01_NORMAL,		// ステージ1通常
        LABEL_BGM_STAGE_01_BOSS,		// ステージ1ボス
        LABEL_BGM_STAGE_02_NORMAL,		// ステージ2通常
        LABEL_BGM_STAGE_02_BOSS,		// ステージ2ボス
        LABEL_BGM_STAGE_03_NORMAL,		// ステージ3通常
        LABEL_BGM_STAGE_03_BOSS,		// ステージ3ボス
        LABEL_BGM_RESULT,				// リザルト
        LABEL_MAX,
    } LABEL;

    CSound();
    ~CSound();  // 親クラスのデストラクタにvirtualをつけることで、メモリリークを抑える
    HRESULT Init(HWND hWnd);
    void Uninit(void);
    HRESULT Play(LABEL label);
    void Stop(LABEL label);
    void StopAll(void);

private:
    typedef struct
    {
        char *pFilename;	// ファイル名
        int nCntLoop;		// ループカウント (-1でループ、0でループなし)
    }PARAM;
    HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
    HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

    IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
    IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
    IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
    BYTE *m_apDataAudio[LABEL_MAX];					    // オーディオデータ
    DWORD m_aSizeAudio[LABEL_MAX];					    // オーディオデータサイズ
    static PARAM m_aParam[LABEL_MAX];                   // 各音素材のパラメータ
};

#endif