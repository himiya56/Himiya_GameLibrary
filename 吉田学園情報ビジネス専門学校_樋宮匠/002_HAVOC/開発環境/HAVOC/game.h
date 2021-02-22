//=============================================================================
//
// �Q�[������ [game.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "mode.h"
#include "tile.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;
class CScore;
class CBulletRemain;
class CMission;
class CScene2D;
class CPause;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �~�b�V����UI�̍ő吔
#define MISSION_UI_MAX 2

// �Q�[����Ԃ̊Ǘ��t���[��
#define COUNT_WAIT_FINISH 300	    // �Q�[���I�����ɁA�^�C�g����ʂ֍s���܂ł̎���

// �e�X�e�[�W���Ƃ́A�}�b�v�����A�~�b�V�����̍ő吔
// �X�e�[�W1
#define STAGE1_MAP_LIMIT 65.0f
#define STAGE1_WEST_LIMIT -(TILE_SIZE / 2)
#define STAGE1_SOUTH_LIMIT -(TILE_SIZE / 2)
#define STAGE1_NORTH_LIMIT STAGE1_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE1_EAST_LIMIT STAGE1_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE1_MISSION1_MAX 50
#define STAGE1_MISSION2_MAX 4

// �X�e�[�W2
#define STAGE2_MAP_LIMIT 65.0f
#define STAGE2_WEST_LIMIT -(TILE_SIZE / 2)
#define STAGE2_SOUTH_LIMIT -(TILE_SIZE / 2)
#define STAGE2_NORTH_LIMIT STAGE2_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE2_EAST_LIMIT STAGE2_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE2_MISSION1_MAX 8
#define STAGE2_MISSION2_MAX 1

// �X�e�[�W�t�@�C�i��
#define STAGE_FINAL_MAP_LIMIT 70.0f
#define STAGE_FINAL_WEST_LIMIT -(TILE_SIZE / 2)
#define STAGE_FINAL_SOUTH_LIMIT -(TILE_SIZE / 2)
#define STAGE_FINAL_NORTH_LIMIT STAGE_FINAL_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE_FINAL_EAST_LIMIT STAGE_FINAL_MAP_LIMIT * TILE_SIZE -(TILE_SIZE / 2)
#define STAGE_FINAL_MISSION1_MAX 3
#define STAGE_FINAL_MISSION2_MAX 1

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CMode
{
public:

    // �Q�[���̏��
    typedef enum
    {
        STATE_START = 0,           // �J�n
        STATE_NORMAL,              // �ʏ�
        STATE_FINISH,	           // �I��
        STATE_IN_FADE,             // �t�F�[�h��
        STATE_PAUSE,	           // �|�[�Y
        STATE_MAX			       // ��Ԃ̍ő吔
    }STATE;

    // ��������G�̎��
    typedef enum
    {
        ENEMYSET_WHITE_ANT = 0,    // ���A��
        ENEMYSET_BLACK_ANT,        // ���单�A��
        ENEMYSET_YELLOW_SPIDER,	   // ���O��
        ENEMYSET_BLACK_SPIDER,	   // ���单�O��
        ENEMYSET_UFO,              // UFO
        ENEMYSET_MAX			   // ��������G�̎�ނ̍ő吔
    }ENEMYSET;

    // �X�e�[�W
    typedef enum
    {
        STAGE_NONE = 0,            // �Ȃ�
        STAGE_1,                   // �X�e�[�W1
        STAGE_2,	               // �X�e�[�W2
        STAGE_FINAL,	           // �ŏI�X�e�[�W
    }STAGE;

    // ���݂̃~�b�V����
    typedef enum
    {
        CURRENT_MISSION_1 = 0,     // �~�b�V����1
        CURRENT_MISSION_2,	       // �~�b�V����2
    }CURRENT_MISSION;

    // �}�b�v����
    typedef struct
    {
        float fSouth;              // ��
        float fNorth;              // �k
        float fEast;               // ��
        float fWest;               // ��
    }MAP_LIMIT;

    CGame();
    ~CGame();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);

    void Judgment(void);                                                            // ���s����
    void ManageMission(void);                                                       // �~�b�V�������Ǘ�
    void ManageState(void);                                                         // �Q�[����Ԃ̊Ǘ�

    static void LoadMapData(STAGE stage);                                           // �X�e�[�W��ǂݍ���
    static void LoadEnemyData(STAGE stage);                                         // �G�l�~�[��ǂݍ���

    // �Z�b�^�[
    static void SetState(STATE state) { m_state = state; }                          // �Q�[���̏�Ԃ�ݒ�
    static void SetStage(STAGE stage);                                              // �X�e�[�W��ݒ�
    static void SetMissionJudgment(bool bJudgment) { m_bMissionClear = bJudgment; } // �~�b�V�����N���A�������ǂ�����ݒ�
    static void SetClearNumMission1(void) { m_nClearNum_Misson1++; }                // �~�b�V����1�̃N���A����1�グ��
    static void SetClearNumMission2(void) { m_nClearNum_Misson2++; }                // �~�b�V����2�̃N���A����1�グ��

    // �Q�b�^�[
    static STATE GetState(void) { return m_state; }                                 // �Q�[���̏�Ԃ��擾
    static CPlayer* GetPlayer(void) { return m_pPlayer; }                           // �v���C���[���擾
    static CScore *GetScore(void) { return m_pScore; }                              // �X�R�A���擾
    static CBulletRemain *GetBulletRemain(void) { return m_pBulletRemain; }         // �c�e���擾
    static STAGE GetStage(void) { return m_stage; }                                 // �X�e�[�W���擾
    static MAP_LIMIT GetMapLimit(void) { return m_mapLimit; }                       // �}�b�v�������擾
    static bool GetMissionJudgment(void) { return m_bMissionClear; }                // �~�b�V�����N���A�������ǂ������擾
    static CURRENT_MISSION GetCurrentMission(void) { return m_currentMission; }     // ���݂̃~�b�V�������擾

private:

    static CScore *m_pScore;                            // �X�R�A�̃|�C���^
    static CPlayer *m_pPlayer;                          // �v���C���[�̃|�C���^
    static CBulletRemain *m_pBulletRemain;              // �c�e���ւ̃|�C���^
    static CMission *m_pMission;                        // �~�b�V�����ւ̃|�C���^
    static CScene2D *m_apMissionUI[MISSION_UI_MAX];     // �~�b�V������UI
    static CPause *m_pPause;                            // �|�[�Y�̃|�C���^

    int m_nCntGameTime;                                 // �Q�[�����Ԃ̃J�E���^

    static STATE m_state;                               // ���
    static STAGE m_stage;                               // �X�e�[�W
    static MAP_LIMIT m_mapLimit;                        // �}�b�v����
    static bool m_bMissionClear;                        // �~�b�V�����N���A�������ǂ���

    static int m_nClearNum_Misson1;                     // �~�b�V����1�̃N���A��
    static int m_nClearNum_Misson2;                     // �~�b�V����2�̃N���A��
    static CURRENT_MISSION m_currentMission;            // ���݂̃~�b�V����
};

#endif