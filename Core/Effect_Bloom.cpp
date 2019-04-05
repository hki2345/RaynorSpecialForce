#include "Effect_Bloom.h"
#include "KMesh.h"
#include "KMaterial.h"
#include "Core_Class.h"
#include "Sampler.h"
#include "ResourceManager.h"




Effect_Bloom::Effect_Bloom()
{
}


Effect_Bloom::~Effect_Bloom()
{
}


bool Effect_Bloom::Create()
{
	m_Mesh = ResourceManager<KMesh>::Find(L"RECT");
	m_ExMat = ResourceManager<KMaterial>::Find(L"POSTEX");
	m_SmallMat = ResourceManager<KMaterial>::Find(L"POSTSMALL");
	m_MergeMat = ResourceManager<KMaterial>::Find(L"POSTBLOOMMERGE");
	m_Smp = ResourceManager<Sampler>::Find(L"DefaultSmp");

	m_LightTarget = ResourceManager<RenderTarget_Multi>::Find(L"LIGHT");

	m_ExTarget = new RenderTarget_Multi();
	m_ExTarget->CreateTarget(Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// 이건 사실 
	m_OTarget = new RenderTarget_Multi();
	m_OTarget->CreateTarget(Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	if (nullptr == m_Mesh || nullptr == m_ExMat)
	{
		return false;
	}

	ArrScreenSize[0] = KVector(Core_Class::MainWindow().size());
	ArrScreenSize[1] = KVector(512, 512);
	ArrScreenSize[2] = KVector(256, 256);
	ArrScreenSize[3] = KVector(128, 128);

	ArrTargetSize[0] = KVector(512, 512);
	ArrTargetSize[1] = KVector(256, 256);
	ArrTargetSize[2] = KVector(128, 128);
	ArrTargetSize[3] = KVector(64, 64);

	// 노출보정을 하려면
	// 1 1 짜리 타겟에다가 출력한다는 것인데.
	return true;
}

void Effect_Bloom::Update()
{
	KPtr<RenderTarget_Multi> FINDMRT = ResourceManager<RenderTarget_Multi>::Find(L"CUT");
	
	// 800 - 512
	Core_Class::MainDevice().Set_DSSDef(L"EFFECT_POST");
	FINDMRT->Clear_OneTarget(0);
	FINDMRT->OMSetOneTarget(0, false);
	m_LightTarget->target_tex(0)->Update(0);
	m_Smp->Update(0);
	m_ExMat->VShader()->SettingCB(L"SCREENSIZE", ArrScreenSize[0]);
	m_ExMat->VShader()->SettingCB(L"TARGETSIZE", ArrTargetSize[0]);
	m_ExMat->Update();
	m_Mesh->Update();
	m_Mesh->Render();
	m_LightTarget->target_tex(0)->Reset(0);

	// 512 - 256
	Core_Class::MainDevice().Set_DSSDef(L"EFFECT_POST");
	FINDMRT->Clear_OneTarget(1);
	FINDMRT->OMSetOneTarget(1, false);
	FINDMRT->target_tex(0)->Update(0);
	m_Smp->Update(0);
	m_ExMat->VShader()->SettingCB(L"SCREENSIZE", ArrScreenSize[0]);
	m_ExMat->VShader()->SettingCB(L"TARGETSIZE", ArrTargetSize[1]);
	m_ExMat->Update();
	m_Mesh->Update();
	m_Mesh->Render();
	FINDMRT->target_tex(0)->Reset(0);

	Core_Class::MainDevice().Set_DSSDef(L"EFFECT_POST");
	FINDMRT->Clear_OneTarget(2);
	FINDMRT->OMSetOneTarget(2, false);
	FINDMRT->target_tex(1)->Update(0);
	m_Smp->Update(0);
	m_SmallMat->VShader()->SettingCB(L"SCREENSIZE", ArrScreenSize[0]);
	m_SmallMat->VShader()->SettingCB(L"TARGETSIZE", ArrTargetSize[2]);
	m_SmallMat->Update();
	m_Mesh->Update();
	m_Mesh->Render();
	FINDMRT->target_tex(1)->Reset(0);


	Core_Class::MainDevice().Set_DSSDef(L"EFFECT_POST");
	FINDMRT->Clear_OneTarget(3);
	FINDMRT->OMSetOneTarget(3, false);
	FINDMRT->target_tex(2)->Update(0);
	m_Smp->Update(0);
	m_SmallMat->VShader()->SettingCB(L"SCREENSIZE", ArrScreenSize[0]);
	m_SmallMat->VShader()->SettingCB(L"TARGETSIZE", ArrTargetSize[3]);
	m_SmallMat->Update();
	m_Mesh->Update();
	m_Mesh->Render();
	FINDMRT->target_tex(2)->Reset(0);

	Core_Class::MainDevice().Set_DSSDef(L"EFFECT_POST");
	m_OTarget->Clear();
	m_OTarget->OMSet();
	// 화면
	m_RTarget->target_tex(0)->Update(0);
	FINDMRT->target_tex(3)->Update(1);
	m_Smp->Update(0);
	m_MergeMat->PShader()->SettingCB(L"SCREENSIZE", ArrTargetSize[3]);
	m_MergeMat->Update();

	m_Mesh->Update();
	m_Mesh->Render();
	m_RTarget->target_tex(0)->Reset(0);
	FINDMRT->target_tex(3)->Reset(1);
}
