#include "sight.h"
#include "player.h"
#include "equipment.h"

CSight::CSight()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

CSight::~CSight()
{
}

HRESULT CSight::Init(void)
{
	CPolygon3D::Init();

	m_pos = CPlayer::GetPlayerPos();

	SetPos(D3DXVECTOR3(m_pos.x, m_pos.y - 300 / 2, -10.0f));
	SetCol(D3DCOLOR_RGBA(255, 0, 0, 255));
	SetSize(SIGHT_HEIGHT, SIGHT_WIDTH);

	return S_OK;
}

void CSight::Uninit(void)
{
	CPolygon3D::Uninit();
}

void CSight::Update(void)
{
	m_pos = CPlayer::GetPlayerPos();

	SetPos(D3DXVECTOR3(m_pos.x, m_pos.y - 300 / 2, 0.0f));

	CPolygon3D::Update();
}

void CSight::Draw(void)
{
	CPolygon3D::Draw();
}

CSight * CSight::Create(void)
{
	CSight *pSight;
	pSight = new CSight;
	pSight->Init();
	return pSight;
}