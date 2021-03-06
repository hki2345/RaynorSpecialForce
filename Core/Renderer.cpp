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


// 렌더러 통합 - 일반적인 렌더러들은 이 형태를 지닌다. -> 본 애니와의 구분만 지어놓은 상황
// 엄격하게 따지면 본 애니도 구분이 되어야 되지만 편의상 하나로 묶되 nullptr로 판졀함
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
	// 멧 데이터 모든 세이더에 다 넣는 구조 -> 그렇게 좋은 구조는 아님
	// 약간 고생해서 다 구분지어 쉐이더 마다 다 세팅 따로 해주는 게 맞긴 함
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




// 렌더데이터를 쉐이더로 보내기 위해 세팅이 먼저 되어야 한다. - 춛가해야 한다.
// 그 작업
void Renderer::Insert_RenderData(const KUINT& _Mesh, const KUINT& _Vtx, const KUINT& _Sub, const KUINT _Mat)
{
	m_RDVec.push_back({ _Mesh, _Vtx, _Sub, _Mat });
}

// 해당 메쉬 따로 최신화
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