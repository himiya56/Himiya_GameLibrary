//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : 樋宮 匠
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "item.h"
#include "manager.h"
#include "sound.h"
#include "object.h"
#include "player.h"
#include "Score.h"
#include "equipment.h"
#include "magazine.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CItem::m_pTexture = NULL;
int CItem::m_nMagazineCount = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CItem::CItem()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWaitTimer = 30.0f;
	m_nMagazineCount = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CItem::~CItem()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos)
{
	// 初期化
	CPolygon3D::Init();

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 角度
	m_nMagazineCount = 0;

	SetPos(pos);
	SetRot(m_rot);
	SetSize(ITEM_WIDTH, ITEM_HEIGHT);

	// テクスチャの作成
	BindTexture(m_pTexture);
	SetTexUV(0.0f, 1.0f, 0.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void CItem::Update()
{
	// アイテムの現在地を取得
	D3DXVECTOR3 pos = GetPosition();
	// サウンドオブジェクトを取得
	CSound *pSound = CManager::GetSound();

	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		CObject *pObject = CObject::GetObj(nCount);
		if (pObject != NULL)
		{
			if (pObject->GetObjType() == OBJ_TYPE_PLAYER)
			{
				D3DXVECTOR3 objPos = ((CPolygon3D *)pObject)->GetPosition();

				float distX = objPos.x - pos.x;
				float distY = objPos.y - pos.y;

				float fAngle = atan2f(distY, distX);

				m_move = D3DXVECTOR3(cos(fAngle)*ITEM_SPEED, sin(fAngle)*ITEM_SPEED, 0.0f);
			}
		}
	}

	// 弾の座標を動かす
	if (m_fWaitTimer <= 0.0f)
	{
		pos += m_move;
		m_fWaitTimer = 0.0f;
		//m_rot.z += D3DXToRadian(10);
	}

	// 弾の当たり判定
	// 弾が何にもぶつかっていないなら実行
	if (!ItemHit(pos, pSound))
	{
		// 弾の情報を更新
		SetPos(pos);
		SetRot(m_rot);

		// タイマーを減らす
		m_fWaitTimer--;

		CPolygon3D::Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CItem::Draw()
{
	CPolygon3D::Draw();
}

//=============================================================================
// 終了処理
//=============================================================================
void CItem::Uninit()
{
	CPolygon3D::Uninit();
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CItem::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "./data/TEXTURE/Crystal.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void CItem::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CItem * CItem::Create(D3DXVECTOR3 pos)
{
	CItem *pItem;
	pItem = new CItem;
	pItem->SetObjType(OBJ_TYPE_ITEM);
	pItem->Init(SpawnItem(pos));
	return pItem;
}

//=============================================================================
// アイテム生成位置計算処理
//=============================================================================
D3DXVECTOR3 CItem::SpawnItem(D3DXVECTOR3 pos)
{
	float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
	float fRadius = float(rand() % (POLYGON_SIZE * 200)) / 100.0f;

	return D3DXVECTOR3(pos.x - sinf(fAngle) * fRadius, pos.y - cosf(fAngle)*fRadius, 0.0f);
}

//=============================================================================
// アイテムの当たり判定
//=============================================================================
// プレイヤーにぶつかるとtrueを返す
bool CItem::ItemHit(D3DXVECTOR3 pos, CSound * pSound)
{
	for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
	{
		CObject *pObject = CObject::GetObj(nCount);
		if (pObject != NULL)
		{
			D3DXVECTOR3 objPos = ((CPolygon3D *)pObject)->GetPosition();
			if (pos.x <= objPos.x + POLYGON_SIZE / 2 &&
				pos.x >= objPos.x - POLYGON_SIZE / 2 &&
				pos.y <= objPos.y + POLYGON_SIZE / 2 &&
				pos.y >= objPos.y - POLYGON_SIZE / 2 &&
				pObject->GetObjType() == OBJ_TYPE_PLAYER)
			{
				// SEを再生
				CSound *pSound = CManager::GetSound();
				// スコアを加算
				CScore::AddScore(1237 * CPlayer::GetCombo());

				// クリスタルハツデンを所持しているならマガジンを回復
				if (CEquipment::IsUse(CEquipment::EQUIPMENT_GENERATOR))
				{
					m_nMagazineCount++;
					if (m_nMagazineCount > MAGAZINE_COUNT)
					{
						m_nMagazineCount = 0;
						CMagazine::AddMag();
					}
				}
				pSound->Play(CSound::SOUND_LABEL_SE_GETITEM);

				// インスタンス破棄
				Uninit();
				return true;
			}
		}
	}
}