#include "HSpRenderer.h"
#include "KMacro.h"
#include "KWindow.h"
#include "ResourceManager.h"
#include "KBlend.h"
#include "Sampler.h"
#include "Texture.h"


HSpRenderer::HSpRenderer() : m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_ImgIndex(0)
{
}


HSpRenderer::~HSpRenderer()
{
}

void HSpRenderer::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	if (nullptr == m_Img)
	{
		return;
	}

	// 버텍스 쉐이더가 

	SubTransUpdate();

	m_MtlVec[0]->VShader()->SettingCB<KMatrix>(L"TRANS", (CSWMat() * _Camera->ViewProj()).RTranspose());
	m_MtlVec[0]->PShader()->SettingCB<KVector4>(L"MULCOLOR", m_Color);
	m_MtlVec[0]->PShader()->SettingCB<KVector4>(L"IMGUV", m_Img->Uv(m_ImgIndex));

	m_MtlVec[0]->Update();

	m_Img->sampler()->Update(0);
	m_Img->texture()->Update(0);

	m_MeshVec[0]->Update();
	m_MeshVec[0]->Render();
}

bool HSpRenderer::Init(int _Order)
{
	Renderer::Init(_Order);

	if (false == Set_Material(L"IMGMAT"))
	{
		return false;
	}
	if (false == Set_Mesh(L"TEXMESH")) 
	{
		return false;
	}

	return true;
}

bool HSpRenderer::Init(const wchar_t* _ImgName, int _Order) 
{
	Image(_ImgName);
	if (nullptr == m_Img)
	{
		return false;
	}

	if (false == Init(_Order))
	{
		return false;
	}

	return true;
}

void HSpRenderer::ImageScaleSetting() 
{
	if (nullptr == m_Img)
	{
		return;
	}

	KVector4 Scale = m_Img->texture()->ImageSize();

	Scale.z = 1.0f;

	SubScale(Scale);
}

void HSpRenderer::Image(const wchar_t* _ImageName) 
{
	if (nullptr == _ImageName)
	{
		m_Img = nullptr;
		return;
	}

	m_Img = ResourceManager<KImage>::Find(_ImageName);
	KASSERT(nullptr == m_Img);
}
