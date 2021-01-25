//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "bullet.h"
#include "manager.h"
#include "particle.h"
#include "sound.h"
#include "object.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "equipment.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet()
{
	// メンバ変数
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// バレットの移動量
	m_nLife = BULLET_LIFE;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 回転する角度
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float rot, BULLET_TYPE type)
{
	// 初期化
	CPolygon3D::Init();

	m_move = move;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, rot); // 角度
	m_BulletType = type;

	if (CEquipment::IsUse(CEquipment::EQUIPMENT_LASERSIGHT))
	{
		m_nLife = BULLET_LIFE * 1.5;
	}
	else
	{
		m_nLife = BULLET_LIFE;
	}

	SetPos(pos);
	SetRot(m_rot);
	SetSize(BULLET_WIDTH, BULLET_HEIGHT);

	// テクスチャの作成
	BindTexture(m_pTexture);
	SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update()
{
	// 弾の現在地を取得
	D3DXVECTOR3 pos = GetPosition();
	// サウンドオブジェクトを取得
	CSound *pSound = CManager::GetSound();

	// 弾の寿命を減らす
	m_nLife--;

	// 弾の座標を動かす
	pos += m_move;

	// 弾の当たり判定
	// 弾が何にもぶつかっていないなら実行
	if (!BulletHit(pos, pSound))
	{
		// 寿命がなくなったらオブジェクトを破棄
		if (m_nLife < 0)
		{
			// 爆発の生成
			pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
			CParticle::SetParticle(pos);
			Uninit();
		}
		else
		{
			// 弾の情報を更新
			SetPos(pos);
			SetRot(m_rot);

			CPolygon3D::Update();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw()
{
	CPolygon3D::Draw();
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit()
{
	CPolygon3D::Uninit();
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CBullet::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Bullet.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void CBullet::Unload(void)
{
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float rot, BULLET_TYPE type)
{
	CBullet *pBullet;
	pBullet = new CBullet;
	pBullet->SetObjType(OBJ_TYPE_BULLET);
	pBullet->Init(pos, move, rot, type);

	// SEを再生
	CSound *pSound = CManager::GetSound();
	pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
	
	return pBullet;
}

//=============================================================================
// 弾の当たり判定
//=============================================================================
// 弾がぶつかるとtrueを返す
bool CBullet::BulletHit(D3DXVECTOR3 pos, CSound *pSound)
{
	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		CObject *pObject = CObject::GetObj(nCount);
		if (pObject != NULL)
		{
			D3DXVECTOR3 objPos = ((CPolygon3D *)pObject)->GetPosition();
			if (pos.x <= objPos.x + POLYGON_SIZE / 4 &&
				pos.x >= objPos.x - POLYGON_SIZE / 4 &&
				pos.y <= objPos.y + POLYGON_SIZE / 4 &&
				pos.y >= objPos.y - POLYGON_SIZE / 4)
			{
				switch (pObject->GetObjType())
				{
					case OBJ_TYPE_BLOCK:
						// 爆発の生成
						Uninit();
						pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
						CParticle::SetParticle(objPos);
						return true;
						break;
					case OBJ_TYPE_ENEMY:
						Uninit();
						pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
						CParticle::SetParticle(objPos);
						return true;
					default:
						break;
				}
			}
		}
	}
}
