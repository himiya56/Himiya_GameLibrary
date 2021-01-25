//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SOUND_PASS_SHOT			"./data/SE/shot001.wav"
#define SOUND_PASS_EXPLOSION	"./data/SE/explosion001.wav"
#define SOUND_PASS_GETITEM		"./data/SE/get_item.wav"

#define	BGM_TITLE "./data/BGM/title.wav"
#define	BGM_STAGE1 "./data/BGM/stage01.wav"
#define	BGM_RESULT "./data/BGM/result.wav"


//*****************************************************************************
// サウンドクラス
//*****************************************************************************
class CSound
{
public:
	CSound();
	~CSound();

	// 列挙型宣言
	typedef enum
	{
		SOUND_LABEL_SE_SHOT,		// 弾発射音
		SOUND_LABEL_SE_EXPLOSION,	// 爆発音
		SOUND_LABEL_SE_GETITEM,		// アイテム取得音
		SOUND_LABEL_BGM_TITLE,
		SOUND_LABEL_BGM_01,
		SOUND_LABEL_BGM_RESULT,
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);


private:
	// 構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} PARAM;

	IXAudio2				*m_pXAudio2 = NULL;						// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice	*m_pMasteringVoice = NULL;				// マスターボイス
	IXAudio2SourceVoice		*m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE					*m_apDataAudio[SOUND_LABEL_MAX] = {};	// オーディオデータ
	DWORD					m_aSizeAudio[SOUND_LABEL_MAX] = {};		// オーディオデータサイズ
	static PARAM			m_aParam[SOUND_LABEL_MAX];				// 各音素材のパラメータ

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
};

#endif