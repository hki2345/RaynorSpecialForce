#include "KSphere_Col.h"



KSphere_Col::KSphere_Col()
{
	pSphere = CreateColFi<KSphereCon>(COLTYPE::CT_SPHERE3D);
}


KSphere_Col::~KSphere_Col()
{
}



void KSphere_Col::Update_Figure()
{
	// 기본적으로 자신의 크기 - 로 세계에서 돌려본후 충돌했는지 안했는지 판별
	pSphere->m_Sphere.Center = m_Trans->worldmat_const().v4.m_XMVec3;
	pSphere->m_Sphere.Radius = m_Trans->worldmat_const().v1.x * .5f;
}


void KSphere_Col::DebugRender()
{
	KVector TT = pSphere->m_ColPoint;
}