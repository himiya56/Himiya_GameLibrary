/****************************************************************************
*	Copylight(C) 2010 Kanazawa-soft-design,LLC.All Rights Reserved.
****************************************************************************/
/*!
*	@file	WaveFile.cpp
*
*	@brief	WAVEファイルの読み込みと書き込み.
*
*	@author	金澤 宣明
*/

#include "wav.h"

//! WAVEファイルヘッダ構造体.
typedef struct {
	DWORD		RiffId;						//!< RIFFファイル識別子 (RIFF).
	DWORD		FileSize;					//!< ファイルサイズ - 8.
	DWORD		FileType;					//!< ファイルタイプ ("WAVE").
	DWORD		FormatId;					//!< フォーマット識別子 ("fmt ").
	DWORD		FormatSize;					//!< フォーマット・サイズ - 8.
	WORD		FormatType;					//!< フォーマットタイプ.
	WORD		Channels;					//!< チャンネル数.
	DWORD		SamplesPerSec;				//!< サンプリング周期.
	DWORD		AvgBytesPerSec;				//!< 1秒あたりのバイト数.
	WORD		BlockAlign;					//!< 1チャンネルのバイト数.
	WORD		BitsPerSample;				//!< 1データあたりのビット数.
} WAVFILEHEADER;

//! ファクトリチャンク・ヘッダ構造体.
typedef struct {
	DWORD		Id;							//!< データ識別子("fact").
	DWORD		Size;						//!< チャンクサイズ - 8.
	DWORD		Samples;					//!< 全サンプル数.
} WAVEFACTCHUNK;

//! データチャンク・ヘッダ構造体.
typedef struct {
	DWORD		Id;							//!< データ識別子("data").
	DWORD		Size;						//!< データ・サイズ.
} WAVEDATACHUNK;

// 関数プロトタイプ.
static BOOL		SaveWaveFileHeader(void);

// ローカル変数.
static HANDLE			hFile = NULL;		//!< ファイル・ハンドル.
static WAVFILEHEADER	Header;				//!< WAVEファイルのヘッダ構造体.
static BOOL				ReadFlag = TRUE;	//!< TRUE = ファイル読込み / FALSE = ファイル書き込み.
static DWORD			DataLen = 0;		//!< 書き込んだWAVEデータのバイト数.

#define	RIFFCC( _x )	(((_x >> 24) & 0xFF) + ((_x >> 8) & 0xFF00) + ((_x << 8) & 0xFF0000) + ((_x << 24) & 0xFF000000))

											/****************************************************************************/
											/*!
											*	@brief	ファイルを読み込みモードでオープンする.
											*
											*	@param	[in]	file_name	ファイル名のポインタ.
											*
											*	@retval	TRUE = OK. / FALSE = NG.
											*/
BOOL	ReadOpenWaveFile(LPCTSTR file_name)
{
	DWORD offset = 0;

	//! 既にファイル・オープン中の場合、エラーにする.
	if (hFile) {
		return FALSE;
	}
	//! 引数が不正な場合、エラーにする.
	if (!file_name || _tcslen(file_name) == 0) {
		return FALSE;
	}
	//! ファイルを読み込みモードでオープンする.
	hFile = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		hFile = NULL;
		return FALSE;
	}
	//! 動作モードを"ファイル読み込み"にする.
	ReadFlag = TRUE;

	//! ファイルヘッダを解析する.
	do {
		//! ファイルの先頭からストリームを読み込む.
		BYTE	buf[256];
		DWORD	len = ReadWaveFile(buf, 256);
		if (len == 0) break;

		//! ファイルヘッダを取り出す.
		Header = *((WAVFILEHEADER*)&buf[offset]);
		offset += sizeof(Header);

		//! RIFFヘッダを調べる.
		if ((Header.RiffId != RIFFCC('RIFF'))
			|| (Header.FileType != RIFFCC('WAVE'))
			|| (Header.FileSize <= (sizeof(Header) - 8))) {
			break;
		}
		//! フォーマットヘッダを調べる.
		if ((Header.FormatId != RIFFCC('fmt '))
			|| (Header.Channels == 0)
			|| (Header.SamplesPerSec == 0)
			|| (Header.AvgBytesPerSec == 0)
			|| (Header.BlockAlign == 0)
			|| (Header.BitsPerSample == 0)
			|| (Header.FormatSize     <  16)) {
			break;
		}
		for (; offset < 256 && buf[offset] == 0; offset++);

		//! ファクトリチャンクを取り出す.
		WAVEFACTCHUNK fact;
		fact = *((WAVEFACTCHUNK*)&buf[offset]);
		if (fact.Id == RIFFCC('fact')) {
			offset += sizeof(WAVEFACTCHUNK);
		}
		else {
			fact.Samples = 0;
		}
		for (; offset < 256 && buf[offset] == 0; offset++);

		//! データヘッダを取り出す.
		WAVEDATACHUNK data;
		data = *((WAVEDATACHUNK*)&buf[offset]);
		offset += sizeof(WAVEDATACHUNK);

		//! データヘッダを調べる.
		if (data.Id != RIFFCC('data') || data.Size == 0) {
			break;
		}
		//! ファイルから読み込んだデータが少ない場合、エラーにする.
		if (len <= offset) {
			break;
		}
		//! ファイルの読み込み位置を先頭フレームに合わせておく.
		LARGE_INTEGER pos1, pos2;
		pos1.QuadPart = offset;
		SetFilePointerEx(hFile, pos1, &pos2, FILE_BEGIN);

		return TRUE;

	} while (FALSE);

	//! エラーの場合、ファイルをクローズして終了する.
	CloseWaveFile();
	return FALSE;
}

/****************************************************************************/
/*!
*	@brief	ファイルを書き込みモードでオープンする.
*
*	@param	[in]	file_name	ファイル名のポインタ.
*	@param	[in]	wf			オーディオ・フォーマット構造体のポインタ.
*	@param	[in]	over_write	TRUE = 上書きモード / FALSE = 新規作成.
*
*	@retval	0 = OK. / -1 = エラー / -2 = 既にファイルが存在している.
*/
int		SaveOpenWaveFile(LPCTSTR file_name, WAVEFORMATEX* wf, BOOL over_write)
{
	//! 既にファイル・オープン中の場合、エラーにする.
	if (hFile) {
		return -1;
	}
	//! ファイル名が不正な場合、エラーにする.
	if (!file_name || _tcslen(file_name) == 0) {
		return -1;
	}
	//! オーディオ・フォーマットが不正な場合、エラーにする.
	if ((wf->wFormatTag != WAVE_FORMAT_PCM)
		|| (wf->nChannels == 0)
		|| (wf->nSamplesPerSec == 0)
		|| (wf->nAvgBytesPerSec == 0)
		|| (wf->nBlockAlign == 0)
		|| (wf->wBitsPerSample == 0)) {
		return -1;
	}

	if (over_write) {
		//! ファイルを上書きモードでオープンする.
		hFile = CreateFile(file_name, GENERIC_WRITE, FILE_SHARE_READ, NULL,
			CREATE_ALWAYS, FILE_FLAG_RANDOM_ACCESS, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			hFile = NULL;
			return -1;
		}
	}
	else {
		//! ファイルを新規作成モードでオープンする.
		hFile = CreateFile(file_name, GENERIC_WRITE, FILE_SHARE_READ, NULL,
			CREATE_NEW, FILE_FLAG_RANDOM_ACCESS, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			hFile = NULL;
			return -2;
		}
	}
	//! 動作モードを"ファイル書き込み"にする.
	ReadFlag = FALSE;

	//! データバイト数をクリアする.
	DataLen = 0;

	//! オーディオ・フォーマット情報をWAVEファイルヘッダにセットする.
	memset(&Header, 0, sizeof(Header));
	Header.RiffId = RIFFCC('RIFF');
	Header.FileSize = (16 + 12) + (8 + 4) + 8;		// WAVヘッダ + FACT + DATA.
	Header.FileType = RIFFCC('WAVE');
	Header.FormatId = RIFFCC('fmt ');
	Header.FormatSize = 16;
	Header.FormatType = wf->wFormatTag;
	Header.Channels = wf->nChannels;
	Header.SamplesPerSec = wf->nSamplesPerSec;
	Header.AvgBytesPerSec = wf->nAvgBytesPerSec;
	Header.BlockAlign = wf->nBlockAlign;
	Header.BitsPerSample = wf->wBitsPerSample;

	//! WAVファイルのヘッダ情報をファイルに書き出す.
	if (!SaveWaveFileHeader()) {
		CloseHandle(hFile);
		hFile = NULL;
		return -1;
	}
	return 0;
}

/****************************************************************************/
/*!
*	@brief	ファイル・クローズ.
*
*	@param	なし.
*
*	@retval	なし.
*/
void	CloseWaveFile(void)
{
	if (hFile) {
		//! WAVファイルの書き出し中の場合、WAVファイルヘッダを更新する.
		if (!ReadFlag) {
			SaveWaveFileHeader();
		}
		//! オープン中のファイルがある場合、ファイルをクローズする.
		CloseHandle(hFile);
	}
	hFile = NULL;
}

/****************************************************************************/
/*!
*	@brief	ファイルからWAVEデータを読み込む.
*
*	@param	[in]	data	WAVEデータを格納する領域のポインタ.
*	@param	[in]	size	WAVEデータを格納する領域のバイト数.
*
*	@retval	ファイルから読み込んだバイト数.
*/
DWORD	ReadWaveFile(void* data, DWORD size)
{
	DWORD len = 0;

	//! ファイルからデータを読み込む.
	if (hFile) {
		if (ReadFile(hFile, data, size, &len, NULL)) {
			return len;
		}
	}
	return 0;
}

/****************************************************************************/
/*!
*	@brief	ファイルにWAVEデータを書き込む.
*
*	@param	[out]	data	WAVEデータを格納した領域のポインタ.
*	@param	[in]	size	WAVEデータのバイト数.
*
*	@retval	ファイルに書き込んだバイト数.
*/
DWORD	WritedWaveFile(void* data, DWORD size)
{
	DWORD len = 0;

	//! ファイルにデータを書き込む.
	if (hFile) {
		if (WriteFile(hFile, data, size, &len, NULL)) {
			DataLen += len;
			return len;
		}
	}
	return 0;
}

/****************************************************************************/
/*!
*	@brief	WAVファイルがオープン中かどうかを調べる.
*
*	@param	なし.
*
*	@retval	TRUE = ファイル・オープン中 / FALSE = ファイル・オープンしていない.
*/
BOOL	IsOpenWaveFile(void)
{
	return hFile ? TRUE : FALSE;
}

/****************************************************************************/
/*!
*	@brief	オーディオ・フォーマットを取り出す.
*
*	@param	[out]	wf	オーディオ・フォーマットを格納する構造体のポインタ.
*
*	@retval	オーディオ・フォーマットを格納した構造体のポインタ.
*/
WAVEFORMATEX*	GetAudioFormat(WAVEFORMATEX* wf)
{
	if (wf) {
		wf->wFormatTag = WAVE_FORMAT_PCM;
		wf->nChannels = Header.Channels;
		wf->wBitsPerSample = Header.BitsPerSample;
		wf->nBlockAlign = Header.BlockAlign;
		wf->nSamplesPerSec = Header.SamplesPerSec;
		wf->nAvgBytesPerSec = Header.AvgBytesPerSec;
		wf->cbSize = 0;
	}
	return wf;
}

/****************************************************************************/
/*!
*	@brief	WAVファイルのヘッダ情報をファイルに書き出す.
*
*	@param	なし.
*
*	@retval	TRUE = OK. / FALSE = NG.
*/
static BOOL		SaveWaveFileHeader(void)
{
	DWORD len = 0;

	//! データの書き出し位置をファイルの先頭に移動する.
	LARGE_INTEGER pos1, pos2;
	pos1.QuadPart = 0;
	SetFilePointerEx(hFile, pos1, &pos2, FILE_BEGIN);

	//! WAVEファイルヘッダをファイルに書き出す.
	Header.FileSize += DataLen;
	if (!WriteFile(hFile, &Header, sizeof(Header), &len, NULL)) {
		return FALSE;
	}

	//! ファクトリチャンク・ヘッダをファイルに書き出す.
	WAVEFACTCHUNK fact;
	memset(&fact, 0, sizeof(fact));
	fact.Id = RIFFCC('fact');
	fact.Size = 4;
	fact.Samples = DataLen / Header.BlockAlign;
	if (!WriteFile(hFile, &fact, sizeof(fact), &len, NULL)) {
		return FALSE;
	}

	//! データチャンク・ヘッダをファイルに書き出す.
	WAVEDATACHUNK data;
	memset(&data, 0, sizeof(data));
	data.Id = RIFFCC('data');
	data.Size = DataLen;
	if (!WriteFile(hFile, &data, sizeof(data), &len, NULL)) {
		return FALSE;
	}
	return TRUE;
}

/* End of file */
