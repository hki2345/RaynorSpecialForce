#include "Renderer.h"
#include "KMacro.h"
#include "ResourceManager.h"
#include "State.h"
#include "KDevice.h"
#include "KWindow.h"
#include "KMaterial.h"
#include "Core_Class.h"


Renderer::Renderer() : m_RsState(nullptr)
{
	if (false == Set_Material(L"NONEMAT"))
	{
		BBY;
	}

	ROpt.Render_DT = RENDER_DATATYPE::RDT_BASE;
}


Renderer::~Renderer()
{
}

void Renderer::Set_RSState(const wchar_t* _Name) {
	m_RsState = kwindow()->Device().Find_RS(_Name);

	if (nullptr == m_RsState)
	{
		KASSERT(true);
		return;
	}
}

bool Renderer::Init(int _Order)
{
	m_Order = _Order;
	state()->This_RenderManager.Insert_Renderer(this);
	return true;
}

bool Renderer::Set_Mesh(KPtr<KMesh> _Mesh, const int& _Index)
{
	KASSERT(nullptr == _Mesh);

	if ((int)m_MeshVec.size() <= _Index)
	{
		m_MeshVec.resize(_Index + 1);
	}

	m_MeshVec[_Index] = _Mesh;
	KASSERT(nullptr == m_MeshVec[_Index]);

	if (nullptr == m_MeshVec[_Index])
	{
		return false;
	}
	return true;
}

bool Renderer::Set_Mesh(const wchar_t* _Res, const int& _Index) 
{
	if ((int)m_MeshVec.size() <= _Index)
	{
		m_MeshVec.resize(_Index + 1);
	}

	m_MeshVec[_Index] = ResourceManager<KMesh>::Find(_Res);
	KASSERT(nullptr == m_MeshVec[_Index]);

	if (nullptr == m_MeshVec[_Index])
	{
		return false;
	}
	return true;
}

bool Renderer::Set_Material(const wchar_t* _Res, const int& _Index)
{
	if ((int)m_MtlVec.size() <= _Index)
	{
		m_MtlVec.resize(_Index + 1);
	}

	m_MtlVec[_Index] = ResourceManager<KMaterial>::Find(_Res);
	KASSERT(nullptr == m_MtlVec[_Index]);

	if (nullptr == m_MtlVec[_Index])
	{
		return false;
	}

	if (L"FORWARD3DMAT" == _Res)
	{
		ROpt.Defferd_orForward = 0;
	}

	return true;
}

KPtr<KMaterial> Renderer::material(const int& _Index) 
{
	if (0 == m_MtlVec.size())
	{
		return nullptr;
	}

	if (m_MtlVec[_Index]->m_bOrigin)
	{
		m_MtlVec[_Index] = m_MtlVec[_Index]->Clone();
	}

	return m_MtlVec[_Index];
}



KPtr<KMesh> Renderer::mesh(const int& _Index /*= 0*/)
{
	if (0 == m_MeshVec.size())
	{
		return nullptr;
	}
	return m_MeshVec[_Index];
}

void Renderer::RenderUpdate()
{
	if (nullptr != m_RsState)
	{
		m_RsState->Update();
	}
}


void Renderer::RenderBegin(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx)
{
	return;
}


// ·»´õ·¯ ÅëÇÕ - ÀÏ¹ÝÀûÀÎ ·»´õ·¯µéÀº ÀÌ ÇüÅÂ¸¦ Áö´Ñ´Ù. -> º» ¾Ö´Ï¿ÍÀÇ ±¸ºÐ¸¸ Áö¾î³õÀº »óÈ²
// ¾ö°ÝÇÏ°Ô µûÁö¸é º» ¾Ö´Ïµµ ±¸ºÐÀÌ µÇ¾î¾ß µÇÁö¸¸ ÆíÀÇ»ó ÇÏ³ª·Î ¹­µÇ nullptr·Î ÆÇÁ°ÇÔ
void Renderer::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	Update_TexSmp(_MtlIdx);
	Update_MtlCB(_MtlIdx);
	Update_Material(_MtlIdx);

	if (nullptr == _Data)
	{
		Update_Mesh(_MeshIdx);
	}
	else
	{
		Update_SelectMesh(_Data->Mesh, _Data->Vertex, _Data->Sub_inx);
	}
}

void Renderer::RenderFin() 
{
	kwindow()->Device().Reset_RS();
	Core_Class::MainDevice().ResetContext();
}

void Renderer::Update_Trans(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	SubTransUpdate(); 

	m_MD.m_W = m_Trans->worldmat_const().RVTranspose();
	m_MD.m_V = _Camera->View().RVTranspose();
	m_MD.m_P = _Camera->Proj().RVTranspose();
	m_MD.m_WV = (m_Trans->worldmat_const() * _Camera->View()).RTranspose();
	m_MD.m_VP = (_Camera->ViewProj()).RVTranspose();
	m_MD.m_WVP = (m_Trans->worldmat_const() * _Camera->ViewProj()).RTranspose();
	m_MD.m_CamPos = _Camera->Trans()->pos_world();
}

void Renderer::Update_TransCB() 
{
	// ¸ä µ¥ÀÌÅÍ ¸ðµç ¼¼ÀÌ´õ¿¡ ´Ù ³Ö´Â ±¸Á¶ -> ±×·¸°Ô ÁÁÀº ±¸Á¶´Â ¾Æ´Ô
	// ¾à°£ °í»ýÇØ¼­ ´Ù ±¸ºÐÁö¾î ½¦ÀÌ´õ ¸¶´Ù ´Ù ¼¼ÆÃ µû·Î ÇØÁÖ´Â °Ô ¸Â±ä ÇÔ
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_VS);
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_PS);
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_DOM);
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_HUL);
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_GEO);
}




void Renderer::Update_MtlCB(const KUINT _Index /*= 0*/)
{
	if (nullptr != m_MtlVec[_Index])
	{
		ROpt.TexCnt = m_MtlVec[_Index]->Set_TexData(ROpt.TexArr);
	}


	Core_Class::MainDevice().SettingCB<RenderOption>(L"RENDEROPTION", ROpt, SHTYPE::ST_VS);
	Core_Class::MainDevice().SettingCB<RenderOption>(L"RENDEROPTION", ROpt, SHTYPE::ST_PS);
}

void Renderer::Update_TexSmp(const KUINT _Index/* = 0*/)
{
	if (nullptr != m_MtlVec[_Index])
	{
		m_MtlVec[_Index]->Update_Tex();
		m_MtlVec[_Index]->Update_Sam();
	}
	else
	{
		BBY;
	}
}

void Renderer::Update_Material(const KUINT _Index /*= 0*/)
{
	if (nullptr != m_MtlVec[_Index])
	{
		m_MtlVec[_Index]->Update();
	}
	else
	{
		BBY;
	}
}

void Renderer::Update_Mesh(const KUINT _Index /*= 0*/)
{
	if (nullptr != m_MeshVec[_Index])
	{
		m_MeshVec[_Index]->Update();
		m_MeshVec[_Index]->Render();
	}
	else
	{
		BBY;
	}
}




// ·»´õµ¥ÀÌÅÍ¸¦ ½¦ÀÌ´õ·Î º¸³»±â À§ÇØ ¼¼ÆÃÀÌ ¸ÕÀú µÇ¾î¾ß ÇÑ´Ù. - ­w°¡ÇØ¾ß ÇÑ´Ù.
// ±× ÀÛ¾÷
void Renderer::Insert_RenderData(const KUINT& _Mesh, const KUINT& _Vtx, const KUINT& _Sub, const KUINT _Mat)
{
	m_RDVec.push_back({ _Mesh, _Vtx, _Sub, _Mat });
}

// ÇØ´ç ¸Þ½¬ µû·Î ÃÖ½ÅÈ­
void Renderer::Update_SelectMesh(const KUINT& _Mesh, const KUINT& _Vtx, const KUINT& _Idx)
{
	if (nullptr != m_MeshVec[_Mesh])
	{
		m_MeshVec[_Mesh]->Update_Pick(_Vtx, _Idx);
	}
	else
	{
		BBY;
	}
}