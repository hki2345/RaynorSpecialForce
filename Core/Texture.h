#pragma once
#include "Resource.h"
#include "DXContainer.h"
#include "DirectXTex.h"


class Texture : public KResource
{
public:
	friend class Texture_Multi;

public:
	Texture();
	~Texture();

private:
	DirectX::ScratchImage			m_Image;
	
	// 1, 2, 3������ �ؽ��ĸ� ��� ���� �� �ִ�.
	// ID3D11Texture2D*				m_pTex2D;
	ID3D11Resource*					m_pTex;
	const type_info*				m_TexInfo;


	// ���̴��� �ؽ�ó�� �־��ٶ� �̳༮���� �־��ش�.
	// �ؽ��Ŀ� �־��� ���̴�
	ID3D11ShaderResourceView*		m_pSRV;
	ID3D11RenderTargetView*			m_pRTV;
	ID3D11DepthStencilView*			m_pDSV;

	// �ؽ��ĸ� ���������� �����͸� ������� 
	D3D11_MAPPED_SUBRESOURCE SubData;

public:
	float Width() { return (float)m_Image.GetMetadata().width; }
	float Height() { return (float)m_Image.GetMetadata().height; }
	KVector2 ImageSize() { return { Width(), Height() }; }
	KColor GetPixel(int _X, int _Y);
	KColor GetPixelF(const int& _X, const int& _Y);


	// Set_View
	ID3D11ShaderResourceView*		SRV() { return m_pSRV; }
	ID3D11RenderTargetView*			RTV() { return m_pRTV; }
	ID3D11DepthStencilView*			DSV() { return m_pDSV; }


	const ID3D11RenderTargetView*	RTV_Const()  { return m_pRTV; }

public:
	void Set_View(KUINT _BindFlag
	, const D3D11_DEPTH_STENCIL_VIEW_DESC* _DSD = nullptr
		, const D3D11_RENDER_TARGET_VIEW_DESC* _RTD = nullptr
		, const D3D11_SHADER_RESOURCE_VIEW_DESC* _SRD = nullptr);

	// 1���� �ؽ��� ����
	bool Create(KUINT _W, void* _pInitData, KUINT _Size, KUINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(KUINT _W, KUINT _H, KUINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _pTex2D, KUINT _BindFlag);


	ID3D11Texture1D* texture1D()
	{
		if (m_TexInfo != &typeid(ID3D11Texture1D*))
		{
			return nullptr;
		}

		return (ID3D11Texture1D*)m_pTex;
	}

	ID3D11Texture2D* texture2D()
	{
		if (m_TexInfo != &typeid(ID3D11Texture2D*))
		{
			return nullptr;
		}

		return (ID3D11Texture2D*)m_pTex;
	}

	ID3D11Texture3D* texture3D()
	{
		if (m_TexInfo != &typeid(ID3D11Texture3D*))
		{
			return nullptr;
		}

		return (ID3D11Texture3D*)m_pTex;
	}


public:
	void Set_Pixel(void* Src, size_t _Size);
	bool Load();

public:
	void Update(const KUINT& _Slot);
	void Reset(const KUINT& _Slot);

};

