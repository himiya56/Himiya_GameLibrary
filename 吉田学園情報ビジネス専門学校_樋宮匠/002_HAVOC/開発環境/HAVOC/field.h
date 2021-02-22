//====================================================================
//
// �t�B�[���h�̏��� (character.h)
// Author : ��{��
//
//====================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene.h"
#include "tile.h"
#include "game.h"

//================================================
// �}�N����`
//================================================

#define MAX_TILE_LINE 80   // ���̃^�C���̍ő吔

//================================================
// �N���X�錾
//================================================

// �t�B�[���h�N���X
class CField : public CScene
{
public:
    CField();
    ~CField();

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void LoadMapData(CGame::STAGE stage, int nWidth, int nDepth);

    static CField *Create(CGame::STAGE stage, int nWidth, int nDepth);

    void SetField(int nWidth, int nDepth);

private:
    CTile					*m_apTile[MAX_TILE_LINE][MAX_TILE_LINE];
    int						m_aGround[MAX_TILE_LINE][MAX_TILE_LINE];
};

#endif