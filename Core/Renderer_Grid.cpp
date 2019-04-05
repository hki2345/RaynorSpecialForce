#include "Renderer_Grid.h"
#include "KMacro.h"
#include "KWindow.h"
#include "TransPosition.h"
#include "DebugManager.h"


Renderer_Grid::Renderer_Grid() : m_BlockSizeInter(10.0f)
{
	ROpt.Defferd_orForward = 0;
}


Renderer_Grid::~Renderer_Grid()
{
}

void Renderer_Grid::Set_Data(KPtr<Camera> _Camera) 
{
	float Y = fabsf(_Camera->Trans()->pos_world().y);
	float CheckUper = m_BlockSizeInter;
	float MaxSize = m_BlockSizeInter;

	// 제곱수 변환
	while (MaxSize <= Y)	{
		MaxSize *= m_BlockSizeInter;
	}

	MaxSize *= 0.1f;

	m_GD.x = MaxSize;

	// 0 ~ m_Size 크기 설정
	if (m_BlockSizeInter >= m_GD.x)
	{
		MaxSize = m_BlockSizeInter;
		m_GD.x = m_BlockSizeInter;
	}

	CheckUper = MaxSize * m_BlockSizeInter;

	m_GD.y = Y * 0.01f;

	m_GD.z = fabsf((Y - CheckUper) / (CheckUper - MaxSize));


	DRAW_LOG(L"Alpha : %f", m_GD.z);
	
	// 알파는 비율로
	// m_GD.m_GD.z = 1.0f - (CamY / (MaxSize * m_Size));
}

void Renderer_Grid::Update_Trans(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	Set_Data(_Camera);

	// 부모인지 요놈인지 계산되어야 한다.
	SubTransUpdate();

	m_MD.m_W = m_Trans->worldmat_const();

	m_MD.m_W.v4.x = state()->camera()->Trans()->pos_world().x;
	m_MD.m_W.v4.y = state()->camera()->Trans()->pos_world().y;

	m_MD.m_V = _Camera->View();
	m_MD.m_P = _Camera->Proj();
	m_MD.m_WVP = (CSWMat() * _Camera->ViewProj()).RTranspose();

	Renderer::Update_TransCB();
}

void Renderer_Grid::RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx)
{
	m_MtlVec[0]->PShader()->SettingCB<KVector4>(L"GRIDDATA", m_GD);
}


void Renderer_Grid::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	if (false == DebugManager::Is_Debug())
	{
		return;
	}

	Renderer::Render(_Camera, _MeshIdx, _MtlIdx, _Data);
}

bool Renderer_Grid::Init(int _Order /*= 0*/)
{
	Renderer::Init(_Order);

	if (false == Set_Material(L"GRID3DMAT"))
	{
		return false;
	}
	if (false == Set_Mesh(L"RECT"))
	{
		return false;
	}

	if (nullptr == kwindow()->Device().FindCB(L"GRIDDATA"))
	{
		kwindow()->Device().CreateCB<KVector4>(L"GRIDDATA", D3D11_USAGE_DYNAMIC, 0);
	}

	Set_RSState(L"SNONE");

	return true;
}


void Renderer_Grid::DebugRender() 
{


}