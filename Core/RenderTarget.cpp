#include "RenderTarget.h"
#include "KMacro.h"
#include "Core_Class.h"

RenderTarget::RenderTarget() : 
	m_Color(KVector4::Blue),
	m_bClear(true)
{
}

RenderTarget::~RenderTarget()
{
}

bool RenderTarget::Create(const KUINT& _W, const KUINT& _H, KVector4 _Color, const KUINT& _BindFlag,
	DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage /*= D3D11_USAGE::D3D11_USAGE_DEFAULT*/)
{
	m_Tex = new Texture();
	if (false == m_Tex->Create(_W, _H, _BindFlag, _eFormat, _eUsage))
	{
		m_Tex = nullptr;
		BBY;
	}
	m_Color = _Color;

	return true;
}

bool RenderTarget::Create(const KUINT& _W, const KUINT& _H, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage)
{
	return Create(_W, _H, m_Color, _BindFlag, _eFormat, _eUsage);
}

// 타겟에 새 택스쳐 박아넣는 방식 -> 새 텍스쳐 만들고 거기에 온 텍스쳐 그림
// 이거 합치는 거임
bool RenderTarget::Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag) 
{
	m_Tex = new Texture();
	if (false == m_Tex->Create(_pTex2D, _BindFlag))
	{
		m_Tex = nullptr;
		BBY;
	}
	return true;
}

void RenderTarget::Clear() 
{
	if (nullptr == m_Tex || nullptr == m_Tex->RTV())
	{
		BBY;
	}

	// Color s -> 포인터 형으로 넘김 -> 컬러를 ㅇㅇ
	if (true == m_bClear)
	{
		Core_Class::Context()->ClearRenderTargetView(m_Tex->RTV(), m_Color.s);	
	}
	else
	{
		int  a = 0;
	}
}