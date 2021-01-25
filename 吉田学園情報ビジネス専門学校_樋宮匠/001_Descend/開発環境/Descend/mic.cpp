#include "mic.h"
#include "wav.h"

CMic::CMic()
{
	m_hwi = NULL;
	memset(&m_wav, 0, sizeof(m_wav));
	m_recFlag = false;
}

CMic::~CMic()
{
}

HRESULT CMic::Init(HWND hWnd)
{
	MMRESULT rc;
	DWORD	 msg, devNum;

	m_wav.wFormatTag = WAVE_FORMAT_PCM;
	m_wav.nChannels = 2;
	m_wav.wBitsPerSample = 16;
	m_wav.nBlockAlign = 4;
	m_wav.nSamplesPerSec = SAMPLING_HZ;
	m_wav.nAvgBytesPerSec = SAMPLING_HZ * m_wav.nBlockAlign;
	m_wav.cbSize = 0;

	devNum = waveInGetNumDevs();

	if (devNum != 0)
	{
		for (int nCount = 0; nCount < devNum; nCount++)
		{
			WAVEINCAPS caps;

			if (waveInGetDevCaps(nCount, &caps, sizeof(caps)) == MMSYSERR_NOERROR)
			{
				if (waveInOpen(&m_hwi, nCount, &m_wav, (DWORD)hWnd, NULL, CALLBACK_WINDOW) != MMSYSERR_NOERROR)
				{
					return S_FALSE;
				}
			}
		}

		memset(m_wh, 0, sizeof(m_wh));

		for (int nCount = 0; nCount < MAX_SOUND_BUFF; nCount++)
		{
			m_wh[nCount].lpData = new char[BUFSIZ];
			m_wh[nCount].dwBufferLength = BUFSIZ;


			if (waveInPrepareHeader(m_hwi, &m_wh[nCount], sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
			{
				return S_FALSE;
			}
		}

		waveInReset(m_hwi);
	}

	StartRec();

	return S_OK;
}

void CMic::Uninit(void)
{

	if (m_hwi)
	{
		waveInStop(m_hwi);

		for (int nCount = 0; nCount < MAX_SOUND_BUFF; nCount++)
		{
			waveInUnprepareHeader(m_hwi, &m_wh[nCount], sizeof(WAVEHDR));

			if (m_wh[nCount].lpData)
			{
				delete[] m_wh[nCount].lpData;
				m_wh[nCount].lpData = NULL;
			}
		}

		waveInClose(m_hwi);
		m_hwi = NULL;
	}
}

void CMic::Update(void)
{
	SoundInputDone();
}

void CMic::StartRec(void)
{
	for (int nCount = 0; nCount < MAX_SOUND_BUFF; nCount++)
	{
		m_wh[nCount].dwBytesRecorded = 0;
		waveInAddBuffer(m_hwi, &m_wh[nCount], sizeof(WAVEHDR));
	}

	if (waveInStart(m_hwi) != MMSYSERR_NOERROR)
	{
		return;
	}

	m_recFlag = true;
}

void CMic::StopRec(void)
{
	SaveOpenWaveFile("./data/test.wav", &m_wav, FALSE);

	Uninit();

	CloseWaveFile();

	m_recFlag = false;
}

bool CMic::IsInputSound(void)
{
	for (int nCount = 0; nCount < MAX_SOUND_BUFF; nCount++)
	{
		if (m_wh[nCount].dwFlags == WHDR_DONE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void CMic::SoundInputDone(void)
{
	WAVEHDR		OutHdr[2];
	HWAVEOUT		hWO; 

	memset(OutHdr, 0, sizeof(OutHdr));
	hWO = NULL;

	if (m_hwi)
	{
		if (m_wh->dwBytesRecorded > 0)
		{
			if (m_recFlag)
			{
				if (WritedWaveFile(m_wh->lpData,m_wh->dwBytesRecorded) == 0)
				{
					CloseWaveFile();
				}
			}

			for (int nCount = 0; nCount < MAX_SOUND_BUFF; nCount++)
			{
				if (OutHdr[nCount].dwBufferLength == 0 && OutHdr[nCount].lpData)
				{
					memcpy(OutHdr[nCount].lpData, m_wh->lpData, m_wh->dwBytesRecorded);
					OutHdr[nCount].dwBufferLength = m_wh->dwBytesRecorded;
					waveOutWrite(hWO, &OutHdr[nCount], sizeof(WAVEHDR));
					break;
				}
			}

			m_wh->dwBytesRecorded = 0;
			waveInAddBuffer(m_hwi, m_wh, sizeof(WAVEHDR));
		}
	}
}