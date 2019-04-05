#include "KBox_Col.h"

#include "Core_Class.h"
#include "ResourceManager.h"
#include "KMesh.h"
#include "KMaterial.h"
#include "Renderer.h"


KBox_Col::KBox_Col()
{
	pBox = CreateColFi<KBoxCon>(COLTYPE::CT_OBB3D);
}


KBox_Col::~KBox_Col()
{
}




bool KBox_Col::Init(int _Order/* = 0*/)
{
	KCollider3D_DE::Init(_Order);
	m_CurColor = KColor::Green;
	return true;
}


void KBox_Col::Update_Figure()
{
	m_PosPivot.y += m_ScalePivot.y * .25f;

	// 기본적으로 자신의 크기 - 로 세계에서 돌려본후 충돌했는지 안했는지 판별
	pBox->m_Box.Center = m_PosPivot.m_XMVec3;
	pBox->m_Box.Extents = (m_ScalePivot * .5f).m_XMVec3;
	pBox->m_Box.Orientation = m_RotPivot.Compute_Quarternion().m_XMVec4;
}


void KBox_Col::DebugRender()
{
	KPtr<KMesh> Mesh = ResourceManager<KMesh>::Find(L"DEBUGCUBE");
	KPtr<KMaterial> Mat = ResourceManager<KMaterial>::Find(L"DEBUGMTL");

	MatrixContainer		m_MD;

	KMatrix S;
	KMatrix R;
	KMatrix P;
	KMatrix RX;
	KMatrix RY;
	KMatrix RZ;
	KMatrix W;

	S.Scale(m_ScalePivot);
	RX.RotX(m_RotPivot.x);
	RY.RotY(m_RotPivot.y);
	RZ.RotZ(m_RotPivot.z);
	P.Translate(m_PosPivot);
	R = RX * RY * RZ;
	W = S * R * P;

	m_MD.m_W = W.RVTranspose();
	m_MD.m_V = one()->state()->camera()->View().RVTranspose();
	m_MD.m_P = one()->state()->camera()->Proj().RVTranspose();
	m_MD.m_WV = (W * one()->state()->camera()->View()).RTranspose();
	m_MD.m_VP = (one()->state()->camera()->ViewProj()).RVTranspose();
	m_MD.m_WVP = (W * one()->state()->camera()->ViewProj()).RTranspose();
	m_MD.m_CamPos = one()->state()->camera()->Trans()->pos_world();

	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_VS);
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_PS);

	Mat->PShader()->SettingCB<KVector>(L"DEBUG_COLOR", m_CurColor);

	Mat->Update();
	Mesh->Update();
	Mesh->Render();
}