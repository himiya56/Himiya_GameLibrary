//====================================================================
//
// �T�E���h���� (sound.h)
// Author : ��{��(�K�v�ȉ��̐􂢏o��) / �㓡�T�V��(�e���Ɏ���)
//
//====================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//================================================
// �N���X�錾
//================================================

// �T�E���h�̃N���X
class CSound
{
public:
    typedef enum
    {
        // SE
        LABEL_SE_SYSTEM_PAUSE = 0,		// �|�[�Y��ʊJ���Ƃ�
        LABEL_SE_SYSTEM_SELECT,			// �I��ύX���̉�
        LABEL_SE_SYSTEM_MODE_CHANGE,	// ���[�h���ύX�����Ƃ��Ƃ��̉�(�|�[�Y���->�^�C�g���Ȃ�)
        LABEL_SE_MISSION_NOTICE,		// �~�b�V�����ʒm��
        LABEL_SE_MACHINE_GUN_SHOT,		// �}�V���K�����ˉ�(�A�ˉ��ł͂Ȃ�������ۂ�play�𕡐���ĂԕK�v�L)
        LABEL_SE_MACHINE_GUN_RELOAD,	// �}�V���K�������[�h��
        LABEL_SE_CANON_SHOT,			// �L���m�����ˉ�
        LABEL_SE_CANON_CHARGE_WEAK,		// �L���m���`���[�W��(1�`3�i�K)
        LABEL_SE_CANON_CHARGE_STRONG,	// �L���m���`���[�W��(4�i�K)
        LABEL_SE_DESTROY_METAL,			// �����n�j��
        LABEL_SE_DESTROY_RUBBLE,		// ���I�n�j��
        LABEL_SE_DESTROY_CRYSTAL,		// �N���X�^���j��
        LABEL_SE_EXPLOSION_SMALL,		// ��������
        LABEL_SE_EXPLOSION_MIDIUM,		// ��������
        LABEL_SE_EXPLOSION_BIG,			// �唚����
        LABEL_SE_DAMAGE_PLAYER,			// �v���C���[�_���[�W��	(��)
        LABEL_SE_DAMAGE_ENEMY,			// �G�l�~�[�_���[�W��		(���X������)
        LABEL_SE_DAMAGE_WYVERN,			// ���C�o�[���_���[�W��	(�݂�������)
        LABEL_SE_DAMAGE_CRYSTAL,		// �N���X�^���_���[�W��
        LABEL_SE_BOSS_ROAR,				// �{�X���K
        LABEL_SE_BOSS_SHOT,				// �e���� (��C�݂����ȉ��A�g�����̓}�V���K����shot�Ɠ��l)
        LABEL_SE_BOSS_FLAP,				// �H�΂����� (SE�ł͂����������B�ꃋ�[�v�Astop���ĂԕK�v�L)
        LABEL_SE_BOSS_WIND,				// ������ (�T�}�\�Ƃ����P�Ŏg���邩��)
        LABEL_SE_BOSS_DEATH,			// �{�X���j��
        LABEL_SE_HEAL,				    // �񕜉�
        LABEL_SE_ORB,				    // �I�[�u��
        LABEL_SE_LAND,                  // �{�X�̒��n��
        LABEL_SE_FAIL,                  // ���s��
        LABEL_SE_NEXT,                  // ���։�
        LABEL_SE_CLEAR,                 // �N���A��
        LABEL_SE_BUG,                   // �����j��
        LABEL_SE_KICK,                  // �ǃL�b�N��
        LABEL_SE_FORCE,                 // �h�q�R���j��
        LABEL_SE_CRACK,                 // �n�ʂ��Ђъ���鉹
        LABEL_SE_FOOT,                  // ����

        // BGM
        LABEL_BGM_TITLE,				// �^�C�g��
        LABEL_BGM_STAGE_01_NORMAL,		// �X�e�[�W1�ʏ�
        LABEL_BGM_STAGE_01_BOSS,		// �X�e�[�W1�{�X
        LABEL_BGM_STAGE_02_NORMAL,		// �X�e�[�W2�ʏ�
        LABEL_BGM_STAGE_02_BOSS,		// �X�e�[�W2�{�X
        LABEL_BGM_STAGE_03_NORMAL,		// �X�e�[�W3�ʏ�
        LABEL_BGM_STAGE_03_BOSS,		// �X�e�[�W3�{�X
        LABEL_BGM_RESULT,				// ���U���g
        LABEL_MAX,
    } LABEL;

    CSound();
    ~CSound();  // �e�N���X�̃f�X�g���N�^��virtual�����邱�ƂŁA���������[�N��}����
    HRESULT Init(HWND hWnd);
    void Uninit(void);
    HRESULT Play(LABEL label);
    void Stop(LABEL label);
    void StopAll(void);

private:
    typedef struct
    {
        char *pFilename;	// �t�@�C����
        int nCntLoop;		// ���[�v�J�E���g (-1�Ń��[�v�A0�Ń��[�v�Ȃ�)
    }PARAM;
    HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
    HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

    IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
    IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
    IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
    BYTE *m_apDataAudio[LABEL_MAX];					    // �I�[�f�B�I�f�[�^
    DWORD m_aSizeAudio[LABEL_MAX];					    // �I�[�f�B�I�f�[�^�T�C�Y
    static PARAM m_aParam[LABEL_MAX];                   // �e���f�ނ̃p�����[�^
};

#endif