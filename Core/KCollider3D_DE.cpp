#include "KCollider3D_DE.h"
#include "State.h"


KCollider3D_DE::KCollider3D_DE()
{
}


KCollider3D_DE::~KCollider3D_DE()
{
}

bool KCollider3D_DE::Init(const int& _Order /*= 0*/)
{
	m_Order = _Order;
	state()->This_Col3DManager.PushCol(this);
	return true;
}