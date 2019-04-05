#include "Camera.h"
#include "TransPosition.h"
#include "TheOne.h"
#include "KWindow.h"
#include "Core_Class.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "KMesh.h"
#include "KMaterial.h"







bool Effect_Post::Create()
{
	return true;
}
void Effect_Post::Update()
{

}



Camera::Camera() : m_eSMode(SM_WINDOW), m_ePMode(PM_ORTH), m_fFov(3.14f * 0.5f), m_fNear(1.0f), m_fFar(1000.0f), m_Order(0)
{
	m_CamMesh = ResourceManager<KMesh>::Find(L"RECT");
	m_CamLightMtl = ResourceManager<KMaterial>::Find(L"DEFFERDMERGEMAT");
	m_CamScreenMtl = ResourceManager<KMaterial>::Find(L"SCREENMERGEMAT");
}

Camera::~Camera()
{
}

bool Camera::Init(int _Order /*= 0*/)
{
	if (nullptr == m_Trans)
	{
		return false;
	}

	m_Order = _Order;

	m_Proj.Identity();
	m_View.Identity();
	
	state()->This_RenderManager.Insert_Cam(this);

	if (nullptr == state()->m_Camera)
	{
		state()->m_Camera = this;
	}
/*
	m_DefferdTarget =
		ResourceManager<RenderTarget_Multi>::Create(
			PathManager::Chain_StringInt(L"DEFFERD_", order()).c_str(), L"COLOR_DIFFUSE", L"POSTION", L"NORMAL", L"DEPTH");

	m_LightTarget = 
		ResourceManager<RenderTarget_Multi>::Create(
			PathManager::Chain_StringInt(L"LIGHT_", order()).c_str(), L"LIGHT_DIFFUSE", L"LIGHT_SPECULAR");*/
	

	m_DefferdTarget = ResourceManager<RenderTarget_Multi>::Find(L"DEFFERD");
	m_LightTarget = ResourceManager<RenderTarget_Multi>::Find(L"LIGHT");

	m_CamTarget = new RenderTarget_Multi();
	m_CamTarget->CreateTarget(Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	return true;
}

void Camera::Update() 
{
}

void Camera::Set_Target()
{
	m_CamTarget->Clear();
	m_CamTarget->OMSet();
}

KVector4 Camera::ScreenTo_World(KVector2 _ScreenPos)
{
	switch (m_ePMode)
	{
	case Camera::PM_PERS:
		break;
	case Camera::PM_ORTH:
	{
		KVector4 Pos;

		KVector2 WinSize = kwindow()->size();
		//         1024               200
		float fX = _ScreenPos.x / WinSize.x;
		float fY = _ScreenPos.y / WinSize.y;

		//         600
		float rX = m_ScreenSize.x * fX - m_ScreenSize.x_part();
		float rY = m_ScreenSize.y * -fY + m_ScreenSize.y_part();

		Pos.m_Pos = KVector2(rX, rY);

		Pos.m_Pos += m_Trans->pos_world();

		return Pos;
	}
	default:
		break;
	}

	return KVector4::Zero;
}

void Camera::FinalUpdate()
{
	m_View.ViewToLH(m_Trans->pos_world(), m_Trans->forward_world(), m_Trans->up_world());

	switch (m_eSMode)
	{
	case Camera::SM_WINDOW:
		m_ScreenSize.x = kwindow()->width_f();
		m_ScreenSize.y = kwindow()->height_f();
		break;
	default:
		break;
	}

	if (0 < m_ScreenSize.y)
	{
		m_ScreenRatioX = m_ScreenSize.x / m_ScreenSize.y;
	}
	if (0 < m_ScreenSize.x)
	{
		m_ScreenRatioY = m_ScreenSize.y / m_ScreenSize.x;
	}

	switch (m_ePMode)
	{
	case Camera::PM_PERS:
		m_Proj.PersLH(m_fFov, m_ScreenSize.x, m_ScreenSize.y, m_fNear, m_fFar);
		break;
	case Camera::PM_ORTH:
		m_Proj.OrthLH(m_ScreenSize.x, m_ScreenSize.y, m_fNear, m_fFar);
		break;
	default:
		break;
	}

	m_VP = m_View * m_Proj;

}



void Camera::Progress_Post()
{
	if (0 == m_EPMap.size())
	{
		return;
	}

	m_SFMI = m_EPMap.begin();
	m_EFMI = m_EPMap.end();

	KPtr<RenderTarget_Multi> TmpTarget = m_CamTarget;


	int Cnt = 0;
	for (; m_SFMI != m_EFMI; ++m_SFMI)
	{
		if (true == m_SFMI->second->effect_on_off())
		{
			m_SFMI->second->m_RTarget = TmpTarget;
			m_SFMI->second->Update();
			TmpTarget = m_SFMI->second->m_OTarget;
			++Cnt;
		}
	}


	if (1 <= Cnt)
	{
		// 누적시켜서 찍는 방식
		m_CamTarget->OMSet();
		TmpTarget->target_tex(0)->Update(0);
		m_CamScreenMtl->Update();
		m_CamMesh->Update();
		m_CamMesh->Render();
		TmpTarget->target_tex(0)->Reset(0);

	}
}



void Camera::EndUpdate() 
{
	
}

void Camera::Merge_Light()
{
	if (nullptr == m_CamMesh || nullptr == m_CamLightMtl)
	{
		BBY;
	}

	m_CamLightMtl->Update();
	m_CamLightMtl->Update_Tex();
	m_CamMesh->Update();
	m_CamMesh->Render();
	m_CamLightMtl->Reset_Tex();
}

void Camera::Merge_Screen()
{
	if (nullptr == m_CamMesh || nullptr == m_CamScreenMtl)
	{
		BBY;
	}

	m_CamTarget->target_tex(0)->Update(0);
	m_CamScreenMtl->Update();
	m_CamMesh->Update();
	m_CamMesh->Render();
	m_CamTarget->target_tex(0)->Reset(0);
}