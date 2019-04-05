#include "RenderTarget_Multi.h"
#include "Core_Class.h"


RenderTarget_Multi::RenderTarget_Multi() : m_DepthTex(nullptr), m_bDefaultDepth(true), m_bClear(true)
{
}


RenderTarget_Multi::~RenderTarget_Multi()
{
	if (nullptr != m_pDepthStencilState)
	{
		m_pDepthStencilState->Release();
	}
}

// 이 Desc는 렌더 타겟마다 달라질 수 있다.
void RenderTarget_Multi::CreateDepth(unsigned int _W, unsigned int _H)
{
	m_DepthTex = new Texture();
	if (false == m_DepthTex->Create(_W, _H, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT_D24_UNORM_S8_UINT))
	{
		BBY;
		return;
	}

	m_DepthState.DepthEnable = TRUE;
	m_DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_DepthState.DepthFunc = D3D11_COMPARISON_LESS;
	m_DepthState.StencilEnable = FALSE;
	m_DepthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	m_DepthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC defaultStencilOp =
	{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
	m_DepthState.FrontFace = defaultStencilOp;
	m_DepthState.BackFace = defaultStencilOp;


	Core_Class::PDevice()->CreateDepthStencilState(&m_DepthState, &m_pDepthStencilState);

	if (nullptr == m_pDepthStencilState)
	{
		KASSERT(true);
		return;
	}

	return;
}


void RenderTarget_Multi:: OMSetOneTarget(const int& _Inx, const bool& _Depth /*= true*/)
{

	if (true == _Depth)
	{
		if (nullptr != m_DepthTex)
		{
			Core_Class::Context()->OMSetRenderTargets(TagetCount(), &m_RenderTagetView[0], m_DepthTex->DSV());
			Core_Class::Context()->OMSetDepthStencilState(m_pDepthStencilState, 1);
		}
		else
		{
			// 이 과정ㅇ은 -> 현재 이 멀티 타겟이 뎁스 스텐실을 가진 경우 그대로 하게 되고
			// 아니면 기존 디바이스가 가진 백버퍼의 뎁스스텐실을 적용시킨다는 뜻
			if (false == m_bDefaultDepth)
			{
				ID3D11DepthStencilView* OldDepth;
				Core_Class::Context()->OMGetRenderTargets(0, nullptr, &OldDepth);
				Core_Class::Context()->OMSetRenderTargets(1, &m_RenderTagetView[_Inx], OldDepth);
				// Core_Class::Context()->OMSetDepthStencilState(m_pDepthStencilState, 1);
				OldDepth->Release();
			}
			else
			{
				Core_Class::Context()->OMSetRenderTargets(1, &m_RenderTagetView[_Inx], Core_Class::MainDevice().Depth());
			}
		}
	}
	else
	{
		Core_Class::Context()->OMSetRenderTargets(1, &m_RenderTagetView[_Inx], nullptr);
	}
}

void RenderTarget_Multi::OMSet(const bool& _Depth /*= true*/)
{
	if (true == _Depth)
	{
		if (nullptr != m_DepthTex)
		{
			Core_Class::Context()->OMSetRenderTargets(TagetCount(), &m_RenderTagetView[0], m_DepthTex->DSV());
			Core_Class::Context()->OMSetDepthStencilState(m_pDepthStencilState, 1);
		}
		else
		{
			// 이 과정ㅇ은 -> 현재 이 멀티 타겟이 뎁스 스텐실을 가진 경우 그대로 하게 되고
			// 아니면 기존 디바이스가 가진 백버퍼의 뎁스스텐실을 적용시킨다는 뜻
			if (false == m_bDefaultDepth)
			{
				ID3D11DepthStencilView* OldDepth;
				Core_Class::Context()->OMGetRenderTargets(0, nullptr, &OldDepth);
				Core_Class::Context()->OMSetRenderTargets(TagetCount(), &m_RenderTagetView[0], OldDepth);
				// Core_Class::Context()->OMSetDepthStencilState(m_pDepthStencilState, 1);
				OldDepth->Release();
			}
			else
			{
				Core_Class::Context()->OMSetRenderTargets(TagetCount(), &m_RenderTagetView[0], Core_Class::MainDevice().Depth());
			}
		}
	}
	else
	{
		Core_Class::Context()->OMSetRenderTargets(TagetCount(), &m_RenderTagetView[0], nullptr);
	}
}

void RenderTarget_Multi::Clear_OneTarget(const int& _Value)
{
	m_RenderTaget[_Value]->Clear();
}

void RenderTarget_Multi::Clear() 
{
	if (false == m_bClear)
	{
		return;
	}

	for (size_t i = 0; i < m_RenderTaget.size(); i++)
	{
		m_RenderTaget[i]->Clear();
	}

	if (nullptr != m_DepthTex)
	{
		Core_Class::Context()->ClearDepthStencilView(m_DepthTex->DSV()
			, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}
}

void RenderTarget_Multi::CreateTarget(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage )
{
	KPtr<RenderTarget> Taget = new RenderTarget();
	if (false == Taget->Create(_W, _H, _BindFlag, _eFormat, _eUsage))
	{
		BBY;
	}

	m_RenderTagetView.push_back(Taget->target_tex()->RTV());
	m_RenderTaget.push_back(Taget);
}

KPtr<Texture> RenderTarget_Multi::target_tex(unsigned int _Idx)
{
	return m_RenderTaget[_Idx]->target_tex();
}