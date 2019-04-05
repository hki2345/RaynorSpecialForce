#include "Renderer_UI.h"
#include "KMacro.h"
#include "KWindow.h"
#include "TransPosition.h"
#include "Core_Class.h"

#include "InputManager.h"

Renderer_UI::Renderer_UI() : m_AlphaValue(1.0f)
{
	ROpt.Defferd_orForward = 0;
}


Renderer_UI::~Renderer_UI()
{
}


bool Renderer_UI::Init(int _Order /*= 0*/)
{
	Renderer::Init(_Order);

	if (false == Set_Material(L"RECT3DMAT"))
	{
		return false;
	}
	if (false == Set_Mesh(L"RECT"))
	{
		return false;
	}

	Set_RSState(L"SBACK");
	m_OriWinSize = KVector2(800.0f, 600.0f);


	return true;
}


void Renderer_UI::Update()
{
	if (kwindow()->size() != m_OriWinSize + m_TmpWinSize)
	{
		m_TmpWinSize = kwindow()->size() - m_OriWinSize;
		KVector TO = m_Trans->pos_local();


		KVector2 TT = m_TmpWinSize;
		TT *= .4f;
		
		if (0 > TO.x)
		{
			TT.x *= -1.0f;
		}
		if (0.0f == TO.x)
		{
			TT.x = .0f;
		}
		if (0 > TO.y)
		{
			TT.y *= -1.0f;
		}
		if (0.0f == TO.y)
		{
			TT.y = .0f;
		}

		TO = TO + TT;
		m_Trans->pos_local(TO);
	}

}

void Renderer_UI::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	Core_Class::MainDevice().Set_DSS(L"ALWAYS");
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}


	Update_TexSmp();
	KVector m_Cut;
	m_Cut.x = m_CutFade;
	m_Cut.y = m_CutValue;
	m_Cut.z = m_AlphaValue;

	m_MtlVec[0]->PShader()->SettingCB<KVector>(L"FADE_CUT", m_Cut);
	m_MtlVec[0]->VShader()->SettingCB<MatrixContainer>(L"MATCON", m_MD);
	m_MtlVec[0]->Update();
	m_MeshVec[0]->Update();
	m_MeshVec[0]->Render();

	Core_Class::MainDevice().Set_DSS(L"BASIC");
}



void Renderer_UI::Update_Trans(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}


	KMatrix m_View;
	KMatrix m_Proj;
	m_View.ViewToLH(KVector4::Zero, KVector4::Forword, KVector4::Up);
	m_Proj.OrthLH(Core_Class::MainWindow().width_f(), Core_Class::MainWindow().height_f(), 0.1f, 1000.0f);

	SubTransUpdate();

	m_MD.m_W = m_Trans->worldmat_const();
	m_MD.m_V = m_View;
	m_MD.m_P = m_Proj;
	m_MD.m_WV = m_MD.m_W * m_View;
	m_MD.m_WVP = m_MD.m_WV * m_MD.m_P;
	m_MD.m_CamPos = KVector(.0f, .0f, -10.0f);
	m_MD.RTrans();
}


bool Renderer_UI::Mouse_In()
{
	KVector2 TT = InputManager::MousePos();

	TT.x -= kwindow()->size().x * .5f;
	TT.y -= kwindow()->size().y * .5f;
	TT.y *= -1.0f;

	float L = m_Trans->pos_local().x - m_Trans->scale_local().x * .5f;
	float R = m_Trans->pos_local().x + m_Trans->scale_local().x * .5f;
	float U = m_Trans->pos_local().y - m_Trans->scale_local().y * .5f;
	float D = m_Trans->pos_local().y + m_Trans->scale_local().y * .5f;

	if (TT.x < L || TT.x > R)
	{
		return false;
	}
	if (TT.y > D || TT.y < U)
	{
		return false;
	}

	return true;
}