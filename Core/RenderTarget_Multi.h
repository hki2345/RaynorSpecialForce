#pragma once
#include "RenderTarget.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "KMacro.h"

// ���ҽ� ����Ʈ ����
// define���� ���� �Ǿ� �ְ� c++ �̳� ����� ��Ʃ����� ����
// �޶�����.
// https://docs.microsoft.com/en-us/windows/desktop/direct3d11/overviews-direct3d-11-resources-limits
// �ش� �ּҴ� ���̴��� �� ���� ���� �� �ִ� ���� �� -> ���� ����� ��������
class RenderTarget_Multi : public KResource
{
public:
	friend RenderManager;

private:
	std::vector<KPtr<RenderTarget>> m_RenderTaget;
	std::vector<ID3D11RenderTargetView*> m_RenderTagetView;

	// Depth
	bool m_bDefaultDepth;
	KPtr<Texture> m_DepthTex;
	D3D11_DEPTH_STENCIL_DESC	m_DepthState;
	ID3D11DepthStencilState*	m_pDepthStencilState;
	bool						m_bClear;

public:
	void Set_Clear()
	{
		m_bClear = true;
	}

	void Set_UnClear()
	{
		m_bClear = false;
	}


	// UINT �����쿡 ���ǵ��� �־ WINDOW
	unsigned int TagetCount() 
	{
		return (int)m_RenderTaget.size();
	}

public:
	void OMSetOneTarget(const int& _Inx, const bool& _Depth = true);
	void OMSet(const bool& _Depth = true);

	void Clear_OneTarget(const int& _Value);
	void Clear();
	void CreateDepth(unsigned int _W, unsigned int _H);


public:
	template<typename... Rest>
	bool Create(Rest... _Arg)
	{
		m_RenderTaget.reserve(sizeof...(_Arg));
		InsertTaget(_Arg...);
		return true;
	}

private:
	template<typename... Rest>
	void InsertTaget(const wchar_t* _TagetName, Rest... _Arg)
	{
		KPtr<RenderTarget> Taget = ResourceManager<RenderTarget>::Find(_TagetName);

		if (nullptr == Taget)
		{
			BBY;
			return;
		}

		m_RenderTaget.push_back(Taget);
		m_RenderTagetView.push_back(Taget->target_tex()->RTV());
		InsertTaget(_Arg...);
	}

	void InsertTaget() {		}

public:
	void CreateTarget(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);

	std::vector<KPtr<RenderTarget>> TagetTexList() 
	{
		return m_RenderTaget;
	}

	KPtr<Texture> target_tex(unsigned int _Idx);

public:
	RenderTarget_Multi();
	~RenderTarget_Multi();
};

