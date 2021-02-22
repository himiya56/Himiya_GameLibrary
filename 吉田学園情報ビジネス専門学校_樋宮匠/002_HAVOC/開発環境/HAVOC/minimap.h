//====================================================================
//
// �~�j�}�b�v�̏��� (minimap.h)
// Author : ��{��
//
//====================================================================
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// �}�N����`
//================================================
#define MINIMAP_SIZE	 350.0f											// �~�j�}�b�v�̃T�C�Y
#define MINIMAP_POS_X	 1200.0f - MINIMAP_SIZE / 2						// �~�j�}�b�v��X���W
#define MINIMAP_POS_Y	 155.0f											// �~�j�}�b�v��Y���W
#define MINIMAP_CENTER	 D3DXVECTOR3(MINIMAP_POS_X, MINIMAP_POS_Y)	    // �~�j�}�b�v�̒��S���W
#define ENEMY_ICON_SIZE  5.0f											// �~�j�}�b�v��̃A�C�R���T�C�Y
#define MAX_ICON_RANGE	 63.0f											// �~�j�}�b�v�A�C�R�������S���痣�����ő勗��
#define MAP_VISION_RANGE 5000.0f                                        // �~�j�}�b�v�ɕ\������v���C���[����̋���
#define MAP_RELATIVE     MAX_ICON_RANGE / MAP_VISION_RANGE              // ���΍��W�v�Z�p
#define MINIMAP_ICON_DISP_DELAY        3                                // �~�j�}�b�v�ɕ\�������܂ł̒x�点��t���[����

#define ICON_COLOR_ENEMY D3DCOLOR_RGBA(255, 0, 0, 255)					// �~�j�}�b�v�A�C�R���̓G�̐F
#define ICON_COLOR_LIFE  D3DCOLOR_RGBA(0, 255, 0, 255)					// �~�j�}�b�v�A�C�R���̉񕜃A�C�e���̐F
#define ICON_COLOR_TARGET  D3DCOLOR_RGBA(255, 255, 0, 255)				// �~�j�}�b�v�A�C�R���̃^�[�Q�b�g�̐F

//================================================
// �N���X�錾
//================================================

// �|�[�Y�N���X
class CMinimap :public CScene2D
{
public:
    CMinimap();
    ~CMinimap();

    typedef enum
    {
        ICON_NONE,
        ICON_ENEMY,         // �G
        ICON_LIFE,          // �񕜃A�C�e��
        ICON_TARGET,        // �~�b�V�����̃^�[�Q�b�g
        ICON_MAX,
    }TYPE;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CMinimap *Create(TYPE type);
    float CalculateDistance(void);                                   // �v���C���[�ƓG�̊p�x�v�Z�֐�
    float CalculateAngle(void);                                      // �v���C���[�ƓG�̋����v�Z�֐�
    void SetFieldPos(const D3DXVECTOR3 pos) { m_fieldPos = pos; }    // �t�B�[���h��̍��W�ݒ�֐�
    void UnableIcon(void) { m_bUse = false; }                        // �}�b�v�A�C�R��������

private:
    D3DXVECTOR3					m_fieldPos;				// �\������A�C�R���̃t�B�[���h��ł̍��W
    float						m_fAngle;				// �v���C���[�Ƃ̊p�x
    float						m_fDistance;			// �v���C���[�Ƃ̋���
    bool						m_bUse;

    bool						m_bDisp;				// �\�����邩�ǂ���
    int							m_nCntTime;				// ���Ԃ��J�E���g

    TYPE						m_type;					// �\�������A�C�R���̃^�C�v
};

#endif