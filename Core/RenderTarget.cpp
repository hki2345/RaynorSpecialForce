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

// Ÿ�ٿ� �� �ý��� �ھƳִ� ��� -> �� �ؽ��� ����� �ű⿡ �� �ؽ��� �׸�
// �̰� ��ġ�� ����
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

	// Color s -> ������ ������ �ѱ� -> �÷��� ����
	if (true == m_bClear)
	{
		Core_Class::Context()->ClearRenderTargetView(m_Tex->RTV(), m_Color.s);	
	}
	else
	{
		int  a = 0;
	}
}