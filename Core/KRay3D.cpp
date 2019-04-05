#include "KRay3D.h"

#include "Core_Class.h"
#include "InputManager.h"
#include "DebugManager.h"
#include "DXContainer_DE.h"
#include "KMesh.h"




KRay3D::KRay3D() : m_Dist(1000.0f)
{
	pRayCon = CreateColFi<KRayCon>(COLTYPE::CT_RAY3D);
}


KRay3D::~KRay3D()
{
}



bool KRay3D::Init(int _Order/* = 0*/)
{
	KCollider3D_DE::Init(_Order);
	pCam = one()->Get_Component<Camera>();

	return true;
}

void KRay3D::Set_Camera(Camera* _pCamera)
{
	pCam = _pCamera;
}



// 충돌구조
// PrevUpdate();
// Update();
// NextUpdate();
// FinalUpdate();
// DebugUpdate();
// This_Col2DManager.Progress();
// This_Col3DManager.Progress();

void KRay3D::Update()
{

	if (nullptr == pCam)
	{
		return;
	}

	pRayCon->Ori = pCam->Trans()->pos_world();

	D3D11_VIEWPORT VPort = {};
	KUINT ViewIdx = 1;
	Core_Class::Context()->RSGetViewports(&ViewIdx, &VPort);
	KVector2 MP = InputManager::MousePos();


	// http://createcode.tistory.com/entry/Direct3D-%ED%88%AC%EC%98%81%EB%B3%80%ED%99%98%ED%96%89%EB%A0%AC
	// 화면 - 현재의 마우스 좌표를 화면비로 나타냄
	// 메트릭스 구성요소 투영행렬의 가로 세로 성분
	pRayCon->Dir.x = (((MP.x - VPort.TopLeftX) * 2.0f / VPort.Width) - 1.0f);
	pRayCon->Dir.x -= pCam->Proj().m_Mat._31;
	pRayCon->Dir.x /= pCam->Proj().m_Mat._11;

	pRayCon->Dir.y = -(((MP.y - VPort.TopLeftY) * 2.0f / VPort.Height) - 1.0f);
	pRayCon->Dir.y -= pCam->Proj().m_Mat._32;
	pRayCon->Dir.y /= pCam->Proj().m_Mat._22;

	pRayCon->Dir.z = VPort.MaxDepth;

	KMatrix VMX = pCam->View();
	KMatrix RVMX = VMX.InverseValue();

	pRayCon->Dir = RVMX.MulVecZero(pRayCon->Dir);
	pRayCon->Dir.NormalizeVec3();
	pRayCon->Dist = m_Dist;
}


void KRay3D::Update_Figure()
{
}

void KRay3D::DebugRender()
{

}