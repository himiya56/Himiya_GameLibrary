//=============================================================================
//
// �T�E���h�̏��� [sound.cpp] (�T�E���h)
// Author : ��{��(�K�v�ȉ��̐􂢏o���A���ʒ���) / �㓡�T�V��(�e���Ɏ���)
//
//=============================================================================
#include "sound.h"

//============================================
// �ÓI�����o�ϐ��錾
//============================================
CSound::PARAM CSound::m_aParam[LABEL_MAX] =
{
    // SE
    { "data/SOUND/SE/00_system/00_pause.wav", 0 },		        // �|�[�Y��ʊJ���Ƃ�
    { "data/SOUND/SE/00_system/01_select.wav", 0 },		        // �I��ύX���̉�
    { "data/SOUND/SE/00_system/02_mode_change.wav", 0 },		// ���[�h���ύX�����Ƃ��Ƃ��̉�(�|�[�Y���->�^�C�g���Ȃ�)  
    { "data/SOUND/SE/01_mission/00_mission_notice.wav", 0 },	// �~�b�V�����ʒm��
    { "data/SOUND/SE/02_machine_gun/00_shot.wav", 0 },		    // �}�V���K�����ˉ�(�A�ˉ��ł͂Ȃ�������ۂ�play�𕡐���ĂԕK�v�L)        
    { "data/SOUND/SE/02_machine_gun/01_reload.wav", 0 },		// �}�V���K�������[�h��
    { "data/SOUND/SE/03_canon/00_shot.wav", 0 },		        // �L���m�����ˉ�
    { "data/SOUND/SE/03_canon/01_charge_weak.wav", 0 },		    // �L���m���`���[�W��(1�`3�i�K)
    { "data/SOUND/SE/03_canon/02_charge_strong.wav", 0 },		// �L���m���`���[�W��(4�i�K)
    { "data/SOUND/SE/04_destroy/00_metal.wav", 0 },		        // �����n�j��
    { "data/SOUND/SE/04_destroy/01_rubble.wav", 0 },		    // ���I�n�j��
    { "data/SOUND/SE/04_destroy/02_crystal.wav", 0 },		    // �N���X�^���j��
    { "data/SOUND/SE/05_explosion/00_small.wav", 0 },			// ��������
    { "data/SOUND/SE/05_explosion/01_midium.wav", 0 },			// ��������
    { "data/SOUND/SE/05_explosion/02_big.wav", 0 },				// �唚����
    { "data/SOUND/SE/06_damage/00_player.wav", 0 },				// �v���C���[�_���[�W��	(��)
    { "data/SOUND/SE/06_damage/01_enemy.wav", 0 },				// �G�l�~�[�_���[�W��		(���X������)
    { "data/SOUND/SE/06_damage/02_wyvern.wav", 0 },				// ���C�o�[���_���[�W��	(�݂�������)
    { "data/SOUND/SE/06_damage/03_crystal.wav", 0 },			// �N���X�^���_���[�W��
    { "data/SOUND/SE/07_boss/00_roar.wav", 0 },					// �{�X���K
    { "data/SOUND/SE/07_boss/01_shot.wav", 0 },					// �e���� (��C�݂����ȉ��A�g�����̓}�V���K����shot�Ɠ��l)
    { "data/SOUND/SE/07_boss/02_flap.wav", -1 },				// �H�΂����� (SE�ł͂����������B�ꃋ�[�v�Astop���ĂԕK�v�L)
    { "data/SOUND/SE/07_boss/03_wind.wav", 0 },					// ������ (�T�}�\�Ƃ����P�Ŏg���邩��)
    { "data/SOUND/SE/07_boss/04_death.wav", 0 },				// �{�X���j��
    { "data/SOUND/SE/08_ex/00_heal.wav", 0 },					// �񕜉�
    { "data/SOUND/SE/08_ex/01_orb.wav", 0 },					// �I�[�u��
    { "data/SOUND/SE/08_ex/02_land.wav", 0 },					// �{�X�̒��n��
    { "data/SOUND/SE/08_ex/03_fail.wav", 0 },					// ���s��
    { "data/SOUND/SE/08_ex/04_next.wav", 0 },					// ���։�
    { "data/SOUND/SE/08_ex/05_clear.wav", 0 },					// �N���A��
    { "data/SOUND/SE/08_ex/06_bug.wav", 0 },					// �����j��
    { "data/SOUND/SE/08_ex/07_kick.wav", 0 },					// �L�b�N��
    { "data/SOUND/SE/08_ex/08_force.wav", 0 },					// �h�q�R���j��
    { "data/SOUND/SE/08_ex/09_crack.wav", 0 },					// �n�ʂ��Ђъ���鉹
    { "data/SOUND/SE/08_ex/10_foot.wav", 0 },					// ����

    // BGM
    { "data/SOUND/BGM/00_title/00_title.wav", -1 },				// �^�C�g��
    { "data/SOUND/BGM/01_stage/00_stage_00.wav", -1 },			// �X�e�[�W1	�ʏ�
    { "data/SOUND/BGM/01_stage/01_boss_00.wav", -1 },			// �X�e�[�W1	�{�X
    { "data/SOUND/BGM/01_stage/02_stage_01.wav", -1 },			// �X�e�[�W2	�ʏ�
    { "data/SOUND/BGM/01_stage/03_boss_01.wav", -1 },			// �X�e�[�W2	�{�X
    { "data/SOUND/BGM/01_stage/04_stage_02.wav", -1 },			// �X�e�[�W3	�ʏ�
    { "data/SOUND/BGM/01_stage/05_boss_02.wav", -1 },			// �X�e�[�W3	�{�X
    { "data/SOUND/BGM/02_result/00_result.wav", -1 },			// ���U���g
};

//=============================================================================
// �T�E���h�̃f�t�H���g�R���X�g���N�^
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
// �T�E���h�̃f�X�g���N�^
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
// �T�E���h����������
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
    HRESULT hr;

    // COM���C�u�����̏�����
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // XAudio2�I�u�W�F�N�g�̍쐬
    hr = XAudio2Create(&m_pXAudio2, 0);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

        // COM���C�u�����̏I������
        CoUninitialize();

        return E_FAIL;
    }

    // �}�X�^�[�{�C�X�̐���
    hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

        if (m_pXAudio2)
        {
            // XAudio2�I�u�W�F�N�g�̊J��
            m_pXAudio2->Release();
            m_pXAudio2 = NULL;
        }

        // COM���C�u�����̏I������
        CoUninitialize();

        return E_FAIL;
    }

    // �T�E���h�f�[�^�̏�����
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        HANDLE hFile;
        DWORD dwChunkSize = 0;
        DWORD dwChunkPosition = 0;
        DWORD dwFiletype;
        WAVEFORMATEXTENSIBLE wfx;
        XAUDIO2_BUFFER buffer;

        // �o�b�t�@�̃N���A
        memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

        // �T�E���h�f�[�^�t�@�C���̐���
        hFile = CreateFile(m_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }
        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {// �t�@�C���|�C���^��擪�Ɉړ�
            MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }

        // WAVE�t�@�C���̃`�F�b�N
        hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }
        if (dwFiletype != 'EVAW')
        {
            MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }

        // �t�H�[�}�b�g�`�F�b�N
        hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }

        // �I�[�f�B�I�f�[�^�ǂݍ���
        hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }
        m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
        hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }

        // �\�[�X�{�C�X�̐���
        hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
        if (FAILED(hr))
        {
            MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }

        // �o�b�t�@�̒l�ݒ�
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
        buffer.AudioBytes = m_aSizeAudio[nCntSound];
        buffer.pAudioData = m_apDataAudio[nCntSound];
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.LoopCount = m_aParam[nCntSound].nCntLoop;

        // �I�[�f�B�I�o�b�t�@�̓o�^
        m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

        // �t�@�C�����N���[�Y
        CloseHandle(hFile);
    }

    return S_OK;
}

//=============================================================================
// �T�E���h�I������
//=============================================================================
void CSound::Uninit(void)
{
    // �ꎞ��~
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        if (m_apSourceVoice[nCntSound])
        {
            // �ꎞ��~
            m_apSourceVoice[nCntSound]->Stop(0);

            // �\�[�X�{�C�X�̔j��
            m_apSourceVoice[nCntSound]->DestroyVoice();
            m_apSourceVoice[nCntSound] = NULL;

            // �I�[�f�B�I�f�[�^�̊J��
            free(m_apDataAudio[nCntSound]);
            m_apDataAudio[nCntSound] = NULL;
        }
    }

    // �}�X�^�[�{�C�X�̔j��
    m_pMasteringVoice->DestroyVoice();
    m_pMasteringVoice = NULL;

    if (m_pXAudio2)
    {
        // XAudio2�I�u�W�F�N�g�̊J��
        m_pXAudio2->Release();
        m_pXAudio2 = NULL;
    }

    // COM���C�u�����̏I������
    CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
HRESULT CSound::Play(LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;
    XAUDIO2_BUFFER buffer;

    // �o�b�t�@�̒l�ݒ�
    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = m_aSizeAudio[label];
    buffer.pAudioData = m_apDataAudio[label];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = m_aParam[label].nCntLoop;

    // ��Ԏ擾
    m_apSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// �Đ���
     // �ꎞ��~
        m_apSourceVoice[label]->Stop(0);

        // �I�[�f�B�I�o�b�t�@�̍폜
        m_apSourceVoice[label]->FlushSourceBuffers();
    }

    // �I�[�f�B�I�o�b�t�@�̓o�^
    m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

    // �Đ�
    m_apSourceVoice[label]->Start(0);

    return S_OK;
}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void CSound::Stop(LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;

    // ��Ԏ擾
    m_apSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// �Đ���
     // �ꎞ��~
        m_apSourceVoice[label]->Stop(0);

        // �I�[�f�B�I�o�b�t�@�̍폜
        m_apSourceVoice[label]->FlushSourceBuffers();
    }
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void CSound::StopAll(void)
{
    // �ꎞ��~
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        if (m_apSourceVoice[nCntSound])
        {
            // �ꎞ��~
            m_apSourceVoice[nCntSound]->Stop(0);
        }
    }
}

//=============================================================================
// �`�����N�̃`�F�b�N
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
    {// �t�@�C���|�C���^��擪�Ɉړ�
        return HRESULT_FROM_WIN32(GetLastError());
    }

    while (hr == S_OK)
    {
        if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
        {// �`�����N�̓ǂݍ���
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
        {// �`�����N�f�[�^�̓ǂݍ���
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        switch (dwChunkType)
        {
        case 'FFIR':
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
            {// �t�@�C���^�C�v�̓ǂݍ���
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
            break;

        default:
            if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
            {// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
    DWORD dwRead;

    if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// �t�@�C���|�C���^���w��ʒu�܂ňړ�
        return HRESULT_FROM_WIN32(GetLastError());
    }

    if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
    {// �f�[�^�̓ǂݍ���
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return S_OK;
}

