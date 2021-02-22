//=============================================================================
//
// サウンドの処理 [sound.cpp] (サウンド)
// Author : 樋宮匠(必要な音の洗い出し、音量調整) / 後藤慎之助(各所に実装)
//
//=============================================================================
#include "sound.h"

//============================================
// 静的メンバ変数宣言
//============================================
CSound::PARAM CSound::m_aParam[LABEL_MAX] =
{
    // SE
    { "data/SOUND/SE/00_system/00_pause.wav", 0 },		        // ポーズ画面開くとき
    { "data/SOUND/SE/00_system/01_select.wav", 0 },		        // 選択変更時の音
    { "data/SOUND/SE/00_system/02_mode_change.wav", 0 },		// モードが変更されるときとかの音(ポーズ画面->タイトルなど)  
    { "data/SOUND/SE/01_mission/00_mission_notice.wav", 0 },	// ミッション通知音
    { "data/SOUND/SE/02_machine_gun/00_shot.wav", 0 },		    // マシンガン発射音(連射音ではないから実際にplayを複数回呼ぶ必要有)        
    { "data/SOUND/SE/02_machine_gun/01_reload.wav", 0 },		// マシンガンリロード音
    { "data/SOUND/SE/03_canon/00_shot.wav", 0 },		        // キャノン発射音
    { "data/SOUND/SE/03_canon/01_charge_weak.wav", 0 },		    // キャノンチャージ音(1～3段階)
    { "data/SOUND/SE/03_canon/02_charge_strong.wav", 0 },		// キャノンチャージ音(4段階)
    { "data/SOUND/SE/04_destroy/00_metal.wav", 0 },		        // 金属系破壊音
    { "data/SOUND/SE/04_destroy/01_rubble.wav", 0 },		    // 瓦礫系破壊音
    { "data/SOUND/SE/04_destroy/02_crystal.wav", 0 },		    // クリスタル破壊音
    { "data/SOUND/SE/05_explosion/00_small.wav", 0 },			// 小爆発音
    { "data/SOUND/SE/05_explosion/01_midium.wav", 0 },			// 中爆発音
    { "data/SOUND/SE/05_explosion/02_big.wav", 0 },				// 大爆発音
    { "data/SOUND/SE/06_damage/00_player.wav", 0 },				// プレイヤーダメージ音	(声)
    { "data/SOUND/SE/06_damage/01_enemy.wav", 0 },				// エネミーダメージ音		(肉々しい音)
    { "data/SOUND/SE/06_damage/02_wyvern.wav", 0 },				// ワイバーンダメージ音	(鈍い金属音)
    { "data/SOUND/SE/06_damage/03_crystal.wav", 0 },			// クリスタルダメージ音
    { "data/SOUND/SE/07_boss/00_roar.wav", 0 },					// ボス咆哮
    { "data/SOUND/SE/07_boss/01_shot.wav", 0 },					// 弾発射 (大砲みたいな音、使い方はマシンガンのshotと同様)
    { "data/SOUND/SE/07_boss/02_flap.wav", -1 },				// 羽ばたき音 (SEではこいつだけが唯一ループ、stopを呼ぶ必要有)
    { "data/SOUND/SE/07_boss/03_wind.wav", 0 },					// 強風音 (サマソとか強襲で使えるかも)
    { "data/SOUND/SE/07_boss/04_death.wav", 0 },				// ボス撃破音
    { "data/SOUND/SE/08_ex/00_heal.wav", 0 },					// 回復音
    { "data/SOUND/SE/08_ex/01_orb.wav", 0 },					// オーブ音
    { "data/SOUND/SE/08_ex/02_land.wav", 0 },					// ボスの着地音
    { "data/SOUND/SE/08_ex/03_fail.wav", 0 },					// 失敗音
    { "data/SOUND/SE/08_ex/04_next.wav", 0 },					// 次へ音
    { "data/SOUND/SE/08_ex/05_clear.wav", 0 },					// クリア音
    { "data/SOUND/SE/08_ex/06_bug.wav", 0 },					// 虫撃破音
    { "data/SOUND/SE/08_ex/07_kick.wav", 0 },					// キック音
    { "data/SOUND/SE/08_ex/08_force.wav", 0 },					// 防衛軍撃破音
    { "data/SOUND/SE/08_ex/09_crack.wav", 0 },					// 地面がひび割れる音
    { "data/SOUND/SE/08_ex/10_foot.wav", 0 },					// 足音

    // BGM
    { "data/SOUND/BGM/00_title/00_title.wav", -1 },				// タイトル
    { "data/SOUND/BGM/01_stage/00_stage_00.wav", -1 },			// ステージ1	通常
    { "data/SOUND/BGM/01_stage/01_boss_00.wav", -1 },			// ステージ1	ボス
    { "data/SOUND/BGM/01_stage/02_stage_01.wav", -1 },			// ステージ2	通常
    { "data/SOUND/BGM/01_stage/03_boss_01.wav", -1 },			// ステージ2	ボス
    { "data/SOUND/BGM/01_stage/04_stage_02.wav", -1 },			// ステージ3	通常
    { "data/SOUND/BGM/01_stage/05_boss_02.wav", -1 },			// ステージ3	ボス
    { "data/SOUND/BGM/02_result/00_result.wav", -1 },			// リザルト
};

//=============================================================================
// サウンドのデフォルトコンストラクタ
//=============================================================================
CSound::CSound()
{
    m_pXAudio2 = NULL;
    m_pMasteringVoice = NULL;
    memset(m_apSourceVoice, 0, sizeof(m_apSourceVoice));
    memset(m_apDataAudio, 0, sizeof(m_apDataAudio));
    memset(m_aSizeAudio, 0, sizeof(m_aSizeAudio));
}

//=============================================================================
// サウンドのデストラクタ
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
// サウンド初期化処理
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
    HRESULT hr;

    // COMライブラリの初期化
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // XAudio2オブジェクトの作成
    hr = XAudio2Create(&m_pXAudio2, 0);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

        // COMライブラリの終了処理
        CoUninitialize();

        return E_FAIL;
    }

    // マスターボイスの生成
    hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

        if (m_pXAudio2)
        {
            // XAudio2オブジェクトの開放
            m_pXAudio2->Release();
            m_pXAudio2 = NULL;
        }

        // COMライブラリの終了処理
        CoUninitialize();

        return E_FAIL;
    }

    // サウンドデータの初期化
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        HANDLE hFile;
        DWORD dwChunkSize = 0;
        DWORD dwChunkPosition = 0;
        DWORD dwFiletype;
        WAVEFORMATEXTENSIBLE wfx;
        XAUDIO2_BUFFER buffer;

        // バッファのクリア
        memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

        // サウンドデータファイルの生成
        hFile = CreateFile(m_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }
        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {// ファイルポインタを先頭に移動
            MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }

        // WAVEファイルのチェック
        hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        if (dwFiletype != 'EVAW')
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // フォーマットチェック
        hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // オーディオデータ読み込み
        hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
        hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // ソースボイスの生成
        hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
        if (FAILED(hr))
        {
            MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // バッファの値設定
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
        buffer.AudioBytes = m_aSizeAudio[nCntSound];
        buffer.pAudioData = m_apDataAudio[nCntSound];
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.LoopCount = m_aParam[nCntSound].nCntLoop;

        // オーディオバッファの登録
        m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

        // ファイルをクローズ
        CloseHandle(hFile);
    }

    return S_OK;
}

//=============================================================================
// サウンド終了処理
//=============================================================================
void CSound::Uninit(void)
{
    // 一時停止
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        if (m_apSourceVoice[nCntSound])
        {
            // 一時停止
            m_apSourceVoice[nCntSound]->Stop(0);

            // ソースボイスの破棄
            m_apSourceVoice[nCntSound]->DestroyVoice();
            m_apSourceVoice[nCntSound] = NULL;

            // オーディオデータの開放
            free(m_apDataAudio[nCntSound]);
            m_apDataAudio[nCntSound] = NULL;
        }
    }

    // マスターボイスの破棄
    m_pMasteringVoice->DestroyVoice();
    m_pMasteringVoice = NULL;

    if (m_pXAudio2)
    {
        // XAudio2オブジェクトの開放
        m_pXAudio2->Release();
        m_pXAudio2 = NULL;
    }

    // COMライブラリの終了処理
    CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT CSound::Play(LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;
    XAUDIO2_BUFFER buffer;

    // バッファの値設定
    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = m_aSizeAudio[label];
    buffer.pAudioData = m_apDataAudio[label];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = m_aParam[label].nCntLoop;

    // 状態取得
    m_apSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中
     // 一時停止
        m_apSourceVoice[label]->Stop(0);

        // オーディオバッファの削除
        m_apSourceVoice[label]->FlushSourceBuffers();
    }

    // オーディオバッファの登録
    m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

    // 再生
    m_apSourceVoice[label]->Start(0);

    return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void CSound::Stop(LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;

    // 状態取得
    m_apSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中
     // 一時停止
        m_apSourceVoice[label]->Stop(0);

        // オーディオバッファの削除
        m_apSourceVoice[label]->FlushSourceBuffers();
    }
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void CSound::StopAll(void)
{
    // 一時停止
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        if (m_apSourceVoice[nCntSound])
        {
            // 一時停止
            m_apSourceVoice[nCntSound]->Stop(0);
        }
    }
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
    HRESULT hr = S_OK;
    DWORD dwRead;
    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD dwBytesRead = 0;
    DWORD dwOffset = 0;

    if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを先頭に移動
        return HRESULT_FROM_WIN32(GetLastError());
    }

    while (hr == S_OK)
    {
        if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
        {// チャンクの読み込み
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
        {// チャンクデータの読み込み
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        switch (dwChunkType)
        {
        case 'FFIR':
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
            {// ファイルタイプの読み込み
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
            break;

        default:
            if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
            {// ファイルポインタをチャンクデータ分移動
                return HRESULT_FROM_WIN32(GetLastError());
            }
        }

        dwOffset += sizeof(DWORD) * 2;
        if (dwChunkType == format)
        {
            *pChunkSize = dwChunkDataSize;
            *pChunkDataPosition = dwOffset;

            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        if (dwBytesRead >= dwRIFFDataSize)
        {
            return S_FALSE;
        }
    }

    return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
    DWORD dwRead;

    if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを指定位置まで移動
        return HRESULT_FROM_WIN32(GetLastError());
    }

    if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
    {// データの読み込み
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return S_OK;
}

