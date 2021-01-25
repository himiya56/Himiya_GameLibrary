#include "combo.h"
#include "player.h"

bool CCombo::m_bCombo = false;

CCombo::CCombo()
{
}

CCombo::~CCombo()
{
}

HRESULT CCombo::Init(void)
{
	return E_NOTIMPL;
}

void CCombo::Update()
{
	CPolygon3D::Update();
}

void CCombo::Draw()
{
	if (CPlayer::GetCombo() > 2)
	{
		CPolygon3D::Draw();
	}
}

void CCombo::Uninit()
{
}

CCombo * CCombo::Create(void)
{
	return nullptr;
}