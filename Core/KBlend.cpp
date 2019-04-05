#include "KBlend.h"
#include "Core_Class.h"
#include "KMacro.h"


KBlend::KBlend() : m_pBlendState(nullptr), m_Color(KColor::Black)
{
}


KBlend::~KBlend()
{
	if (nullptr != m_pBlendState)
	{
		m_pBlendState->Release();
		m_pBlendState = nullptr;
	}
}

bool KBlend::Create() 
{
	m_Desc.AlphaToCoverageEnable = false;
	m_Desc.IndependentBlendEnable = false;

	m_Desc.RenderTarget[0].BlendEnable = true;
	m_Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	m_Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	m_Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	m_Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	if (S_OK != Core_Class::PDevice()->CreateBlendState(&m_Desc, &m_pBlendState))
	{
		KASSERT(true);
		return false;
	}

	return true;
}

bool KBlend::Create(D3D11_BLEND_DESC _Decs) {
	m_Desc = _Decs;
	if (S_OK != Core_Class::PDevice()->CreateBlendState(&_Decs, &m_pBlendState))
	{
		KASSERT(true);
		return false;
	}

	return true;
}

bool KBlend::Update() 
{
	Core_Class::Context()->OMSetBlendState(m_pBlendState, m_Color.s, 0xffffffff);

	return true;
}