//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SOUND_PASS_SHOT			"./data/SE/shot001.wav"
#define SOUND_PASS_EXPLOSION	"./data/SE/explosion001.wav"
#define SOUND_PASS_GETITEM		"./data/SE/get_item.wav"

#define	BGM_TITLE "./data/BGM/title.wav"
#define	BGM_STAGE1 "./data/BGM/stage01.wav"
#define	BGM_RESULT "./data/BGM/result.wav"


//*****************************************************************************
// �T�E���h�N���X
//*****************************************************************************
class CSound
{
public:
	CSound();
	~CSound();

	// �񋓌^�錾
	typedef enum
	{
		SOUND_LABEL_SE_SHOT,		// �e���ˉ�
		SOUND_LABEL_SE_EXPLOSION,	// ������
		SOUND_LABEL_SE_GETITEM,		// �A�C�e���擾��
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
	// �\���̒�`
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} PARAM;

	IXAudio2				*m_pXAudio2 = NULL;						// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice	*m_pMasteringVoice = NULL;				// �}�X�^�[�{�C�X
	IXAudio2SourceVoice		*m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE					*m_apDataAudio[SOUND_LABEL_MAX] = {};	// �I�[�f�B�I�f�[�^
	DWORD					m_aSizeAudio[SOUND_LABEL_MAX] = {};		// �I�[�f�B�I�f�[�^�T�C�Y
	static PARAM			m_aParam[SOUND_LABEL_MAX];				// �e���f�ނ̃p�����[�^

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
};

#endif