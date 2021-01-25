//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "life.h"
#include "player.h"
#include "particle.h"
#include "item.h"
#include "sound.h"
#include "equipment.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[TYPE_MAX] = {};


//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ポリゴンの移動量
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ポリゴンの移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 1;
	m_nCountMove = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, int life, ENEMY_TYPE type)
{
	CPolygon3D::Init();
	m_pos = pos;
	SetPos(pos);
	m_nLife = life;
	m_enemyType = type;
	m_nCountMove = 0;

	// テクスチャの作成
	BindTexture(m_apTexture[m_enemyType]);
	SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update()
{
	// サウンドオブジェクトを取得
	CSound *pSound = CManager::GetSound();
	m_pos = GetPosition();

	float distX = 0.0f;
	float distY = 0.0f;
	float fAngle = 0.0f;

	m_nCountMove--;

	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		CObject *pObject = CObject::GetObj(nCount);
		if (pObject != NULL)
		{
			if (pObject->GetObjType() == OBJ_TYPE_PLAYER)
			{
				D3DXVECTOR3 objPos = ((CPolygon3D *)pObject)->GetPosition();

				distX = objPos.x - m_pos.x;
				distY = objPos.y - m_pos.y;

				fAngle = atan2f(distY, distX);

				if (m_enemyType == TYPE_SLIME && fabsf(distY) < POLYGON_SIZE * 5)
				{
					m_bChase = true;
					m_move = D3DXVECTOR3(cos(fAngle) * ENEMY_SPEED * 2, sin(fAngle) * ENEMY_SPEED / 2, 0.0f);
				}
				else if (m_enemyType == TYPE_BAT && objPos.y - POLYGON_SIZE <= m_pos.y)
				{
					m_bChase = true;
					m_move = D3DXVECTOR3(cos(fAngle) * ENEMY_SPEED * 2, sin(fAngle) * ENEMY_SPEED, 0.0f);
				}
				else if (m_enemyType == TYPE_EYE &&  fabsf(distY) < POLYGON_SIZE * 5)
				{
					m_bChase = true;
					if (m_nCountMove < 0)
					{
						int nMove = GetRandom(-180, 180);
						m_move = D3DXVECTOR3(cos(nMove) * ENEMY_SPEED, sin(nMove) * ENEMY_SPEED, 0.0f);
						m_nCountMove = 60;
					}
				}
			}
		}
	}

	if (m_bChase)
	{
		m_pos += m_move;
	}

	Collision(m_pos);

	if (m_nLife < 0)
	{
		// 爆発の生成
		CParticle::SetParticle(m_pos);
		pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
		CPlayer::AddCombo();

		if (CEquipment::IsUse(CEquipment::EQUIPMENT_AMPRIFICATER))
		{
			// アイテムを生成
			for (int nCount = 0; nCount < 15; nCount++)
			{
				CItem::Create(m_pos);
			}
		}
		else
		{
			// アイテムを生成
			for (int nCount = 0; nCount < 10; nCount++)
			{
				CItem::Create(m_pos);
			}
		}

		Uninit();
	}
	else
	{
		CPolygon3D::Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw()
{
	CPolygon3D::Draw();
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit()
{
	CPolygon3D::Uninit();
}

//=============================================================================
// 衝突判定処理
//=============================================================================
void CEnemy::Collision(D3DXVECTOR3 pos)
{
	// オブジェクト数分ループ
	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		// オブジェクト取得
		CObject *pObject = CObject::GetObj(nCount);

		if (pObject != NULL)
		{
			// オブジェクトタイプを取得
			CObject::OBJ_TYPE type = pObject->GetObjType();

			// 座標を取得
			D3DXVECTOR3 objPos = ((CPolygon3D *)pObject)->GetPosition();

			// xの距離を計算
			float distX = fabsf(objPos.x - pos.x);

			// yの距離を計算
			float distY = fabsf(objPos.y - pos.y);

			// めり込んでいるとき(距離の両方がポリゴン大きさより小さいとき)
			if (distX < POLYGON_SIZE / 2 && distY < POLYGON_SIZE / 2)
			{
				switch (type)
				{
				case OBJ_TYPE_BLOCK:
					if (objPos.x - POLYGON_SIZE / 2 + 10 < pos.x && pos.x < objPos.x + POLYGON_SIZE / 2 - 10)
					{
						if (m_move.y < 0.0f)
						{
							pos.y = objPos.y + POLYGON_SIZE / 2;
						}
						if (m_move.y > 0.0f)
						{
							pos.y = objPos.y - POLYGON_SIZE / 2;
						}
					}
					if (objPos.y + POLYGON_SIZE / 2 - 10 > pos.y && pos.y > objPos.y - POLYGON_SIZE / 2 + 10)
					{
						if (pos.x > objPos.x)
						{
							pos.x = objPos.x + POLYGON_SIZE / 2;
						}
						if (pos.x < objPos.x)
						{
							pos.x = objPos.x - POLYGON_SIZE / 2;
						}
					}
					break;
				case OBJ_TYPE_BULLET:
					m_nLife--;
					break;
				default:
					break;
				}
			}
		}
	}

	SetPos(pos);
}

//=============================================================================
// テクスチャ読み込み
//=============================================================================
HRESULT CEnemy::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/fly.png", &m_apTexture[TYPE_SLIME]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/bat.png", &m_apTexture[TYPE_BAT]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/random.png", &m_apTexture[TYPE_EYE]);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄
//=============================================================================
void CEnemy::Unload(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, ENEMY_TYPE type)
{
	CEnemy *pEnemy;
	pEnemy = new CEnemy;
	pEnemy->SetObjType(OBJ_TYPE_ENEMY);

	int nLife = 1;
	switch (type)
	{
	case TYPE_SLIME:
		nLife = SLIME_LIFE;
		break;
	case TYPE_BAT:
		nLife = SLIME_LIFE;
		break;
	default:
		break;
	}
	pEnemy->Init(pos, nLife, type);
	return pEnemy;
}