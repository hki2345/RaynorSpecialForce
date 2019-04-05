#include "KPlane3D_Col.h"
#include "Renderer_Terrain.h"



KPlane3D_Col::KPlane3D_Col()
{
	pPlane = CreateColFi<KPlaneCon>(COLTYPE::CT_PLANE3D);
}


KPlane3D_Col::~KPlane3D_Col()
{
}


void KPlane3D_Col::Update()
{
	KVector TT = pPlane->m_ColPoint;
}

void KPlane3D_Col::Update_Figure()
{
	// KVector RU = _Left.v1;
	// KVector RD = _Left.v2;
	// KVector LU = _Left.v3;
	// KVector LD = _Left.v4;

	// 기본적으로 자신의 크기 - 로 세계에서 돌려본후 충돌했는지 안했는지 판별
	pPlane->m_Mat = KMath::EdgeOfPlane_XY(m_Trans);

	pPlane->m_Mat.v1.w = .0f;
	pPlane->m_Mat.v2.w = .0f;
	pPlane->m_Mat.v3.w = .0f;
	pPlane->m_Mat.v4.w = .0f;
}


void KPlane3D_Col::DebugRender()
{
	int a = 0;
}