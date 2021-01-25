/****************************************************************************
*	Copylight(C) 2010 Kanazawa-soft-design,LLC.All Rights Reserved.
****************************************************************************/
/*!
*	@file	WaveFile.h
*
*	@brief	WAVEÉtÉ@ÉCÉãÇÃì«Ç›çûÇ›Ç∆èëÇ´çûÇ›.
*
*	@author	ã‡‡V êÈñæ
*/
#ifndef	__WAV_H__
#define	__WAV_H__

#include "main.h"

BOOL			ReadOpenWaveFile(LPCTSTR file_name);
int				SaveOpenWaveFile(LPCTSTR file_name, WAVEFORMATEX* wf, BOOL over_write);
void			CloseWaveFile(void);
DWORD			ReadWaveFile(void* data, DWORD size);
DWORD			WritedWaveFile(void* data, DWORD size);
BOOL			IsOpenWaveFile(void);
WAVEFORMATEX*	GetAudioFormat(WAVEFORMATEX* wf);

#endif	// __WAVEFILE_H__
/* End of file */