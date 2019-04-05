#include "Effect_Gaussian.h"
#include "Core_Class.h"

#include "KMesh.h"
#include "KMaterial.h"
#include "Sampler.h"


Effect_Gaussian::Effect_Gaussian()
{
}


Effect_Gaussian::~Effect_Gaussian()
{
}



bool Effect_Gaussian::Create()
{
	m_Mesh = ResourceManager<KMesh>::Find(L"RECT");
	m_Mtl = ResourceManager<KMaterial>::Find(L"POSTGAU");
	m_Spl = ResourceManager<Sampler>::Find(L"DefaultSmp");


	m_OTarget = new RenderTarget_Multi();
	m_OTarget->CreateTarget(Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	if (nullptr == m_Mesh || nullptr == m_Mtl)
	{
		return false;
	}

	return true;
}


void Effect_Gaussian::Update()
{
	Core_Class::MainDevice().Set_DSS(L"EFFECT_POST");
	m_OTarget->Clear();
	m_OTarget->OMSet();
	m_RTarget->target_tex(0)->Update(0);
	m_Spl->Update(0);
	KVector SrnSize = Core_Class::MainWindow().size();
	m_Mtl->PShader()->SettingCB(L"SCREENSIZE", SrnSize);
	m_Mtl->Update();
	m_Mesh->Update();
	m_Mesh->Render();
	m_RTarget->target_tex(0)->Reset(0);
}
