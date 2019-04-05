#include "Renderer_Rect.h"
#include "KMacro.h"
#include "KWindow.h"
#include "TransPosition.h"


Renderer_Rect::Renderer_Rect()
{
	ROpt.Defferd_orForward = 1;
}


Renderer_Rect::~Renderer_Rect()
{
}
void Renderer_Rect::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	SubTransUpdate();

	m_MD.m_W = m_Trans->worldmat_const();
	m_MD.m_V = _Camera->View();
	m_MD.m_P = _Camera->Proj();
	m_MD.m_WVP = (CSWMat() * _Camera->ViewProj()).RTranspose();

	m_MtlVec[0]->VShader()->SettingCB<MatrixContainer>(L"MATCON", m_MD);

	m_MtlVec[0]->Update();
	m_MeshVec[0]->Update();
	m_MeshVec[0]->Render();
}

bool Renderer_Rect::Init(int _Order /*= 0*/) 
{
	Renderer::Init(_Order);

	if (false == Set_Material(L"MESH3DMAT"))
	{
		return false;
	}
	if (false == Set_Mesh(L"RECT"))
	{
		return false;
	}

	Set_RSState(L"SBACK");
	return true;
}
