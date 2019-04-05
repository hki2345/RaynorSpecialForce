#include "Texture.h"
#include "Core_Class.h"
#include "KMacro.h"
#include "MultiCritical.h"



#if X64
#if _DEBUG
#pragma comment(lib, "DirectXTex_X64_DEBUG")
#else if NDEBUG
#pragma comment(lib, "DirectXTex_X64_RELEASE")
#endif
#else if WIN32
#if _DEBUG
#pragma comment(lib, "DirectXTex_X86_DEBUG")
#else if NDEBUG
#pragma comment(lib, "DirectXTex_X86_RELEASE")
#endif
#endif // DEBUG



Texture::Texture() :
	m_pTex(nullptr),
	m_TexInfo(nullptr),
	m_pSRV(nullptr), 
	m_pRTV(nullptr),
	m_pDSV(nullptr)
{
	SubData = D3D11_MAPPED_SUBRESOURCE();
}


Texture::~Texture()
{
	KRELESE(m_pTex);
	KRELESE(m_pSRV);
	KRELESE(m_pRTV);
	KRELESE(m_pDSV);
}

// 파일헤더
// 파일들은 항상 다 특정한 데이터구조를 헤더에 적어놨다. 
// 파일종류마다 로딩함수가 다르다.

//  확장자 마다 다른 함수를 실행 -> 다이렉트에서 그렇게 하니까
// 뭐 당연한 거겠다.
// 단 파일 확장자는 파일에 시작 간 확장자를 구별하기 위한 일종의 코드만 다를뿐
// 바이너리 형태는 같다. (암호화하지 안흔다면) 하면 뭐 복호화 해야하는 거고
bool Texture::Load() 
{
	MultiCritical A{ 0 };

	// bmp
	if (true == Compare_Ext(L".DDs") || true == Compare_Ext(L".dds"))
	{
		if (S_OK != DirectX::LoadFromDDSFile(AllPath(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}
	else if(true == Compare_Ext(L".tga") || true == Compare_Ext(L".TGA"))
	{
		if (S_OK != DirectX::LoadFromTGAFile(AllPath(), nullptr, m_Image))
		{
			return false;
		}
	}
	else {
		//png, bmp, jpg확장자는 else로 묶음 ㅇㅇ 
		if (S_OK != DirectX::LoadFromWICFile(AllPath(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}

	if (S_OK != DirectX::CreateShaderResourceView(Core_Class::PDevice(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		&m_pSRV))
	{
		return false;
	}

	KASSERT(nullptr == m_pSRV);

	m_pSRV->GetResource((ID3D11Resource**)(&m_pTex));

	KASSERT(nullptr == m_pTex);

	m_TexInfo = &typeid(ID3D11Texture2D*);
	return true;
}

void Texture::Update(const KUINT& _Slot)
{
	Core_Class::Context()->VSSetShaderResources(_Slot, 1, &m_pSRV);
	Core_Class::Context()->PSSetShaderResources(_Slot, 1, &m_pSRV);
}

void Texture::Reset(const KUINT& _Slot)
{
	ID3D11ShaderResourceView* Reset = nullptr;
	Core_Class::Context()->VSSetShaderResources(_Slot, 1, &Reset);
	Core_Class::Context()->PSSetShaderResources(_Slot, 1, &Reset);
}

KVector4 Texture::GetPixel(int _X, int _Y) 
{
	if (Width() <= _X || Height() <= _Y)
	{
		return KVector::Zero;
	}

	size_t Size = VS_Format::Size_Format(m_Image.GetMetadata().format);

	BaseColor Color;
	uint8_t* p = m_Image.GetPixels();

	p += ((m_Image.GetMetadata().width * _Y) * Size) + (_X * Size);

	memcpy_s(&Color, Size, p, Size);

	KVector4 ReturnColor;

	ReturnColor.Set_Color(Color.b, Color.g, Color.r, Color.a);

	return ReturnColor;
}

KColor Texture::GetPixelF(const int& _X, const int& _Y)
{
	if (Width() <= _X || Height() <= _Y)
	{
		return KVector::Zero;
	}

	size_t Size = VS_Format::Size_Format(m_Image.GetMetadata().format);

	BaseColor Color;
	uint8_t* p = m_Image.GetPixels();

	p += ((m_Image.GetMetadata().width * _Y) * Size) + (_X * Size);

	memcpy_s(&Color, Size, p, Size);

	KColor ReturnColor =
	KColor(
		Color.b / 255.0f, 
		Color.g / 255.0f,
		Color.r / 255.0f,
		Color.a / 255.0f);

	return ReturnColor;
}




bool Texture::Create(KUINT _W, void* _pInitData, KUINT _Size, KUINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage /*= D3D11_USAGE::D3D11_USAGE_DEFAULT*/)
{
	D3D11_TEXTURE1D_DESC tDecs = {};

	// 1차원이라 높이 0
	tDecs.Width = _W;
	tDecs.ArraySize = 1;
	tDecs.BindFlags = _BindFlag;
	tDecs.Usage = _eUsage;
	tDecs.MipLevels = 1;
	tDecs.Format = _eFormat;
	tDecs.MiscFlags = 0;
	tDecs.CPUAccessFlags = 0;


	// 리소스를 넣어주는 과정
	D3D11_SUBRESOURCE_DATA Data;
	Data.SysMemSlicePitch = 0;
	Data.SysMemPitch = _Size;
	Data.pSysMem = _pInitData;

	if (nullptr == _pInitData)
	{
		Core_Class::PDevice()->CreateTexture1D(&tDecs, nullptr, (ID3D11Texture1D**)&m_pTex);
	}
	else
	{
		Core_Class::PDevice()->CreateTexture1D(&tDecs, &Data, (ID3D11Texture1D**)&m_pTex);
	}

	if (nullptr == m_pTex)
	{
		BBY;	
		return false;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC tSR = {};
	tSR.Format = tDecs.Format;
	tSR.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	tSR.Texture1D.MipLevels = tDecs.MipLevels;
	tSR.Texture1D.MostDetailedMip = 0;


	Set_View(_BindFlag, nullptr, nullptr, nullptr);
	m_TexInfo = &typeid(ID3D11Texture1D*);

	return true;

}



bool Texture::Create(KUINT _W, KUINT _H, KUINT _BindFlag, DXGI_FORMAT _eFormat,
	D3D11_USAGE _eUsage /*= D3D11_USAGE::D3D11_USAGE_DEFAULT*/) 
{
	D3D11_TEXTURE2D_DESC tDecs = {};

	tDecs.Width = _W;
	tDecs.Height = _H;
	tDecs.ArraySize = 1;
	tDecs.BindFlags = _BindFlag;
	tDecs.Usage = _eUsage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _eUsage)
	{
		tDecs.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else {
		tDecs.CPUAccessFlags = 0;
	}

	// MipMap설정
	tDecs.Format = _eFormat;
	tDecs.SampleDesc.Count = 1;
	tDecs.SampleDesc.Quality = 0;
	tDecs.MipLevels = 1;

	if (S_OK != Core_Class::PDevice()->CreateTexture2D(&tDecs, nullptr, (ID3D11Texture2D**)&m_pTex))
	{
		BBY;
		return false;
	}

	Set_View(_BindFlag, nullptr, nullptr, nullptr);
	m_TexInfo = &typeid(ID3D11Texture2D*);

	return true;
}

bool Texture::Create(ID3D11Texture2D* _pTex2D, KUINT _BindFlag)
{
	m_TexInfo = &typeid(ID3D11Texture2D*);
	return true;
}

void Texture::Set_View(KUINT _BindFlag
	, const D3D11_DEPTH_STENCIL_VIEW_DESC* _DSD/* = nullptr*/
	, const D3D11_RENDER_TARGET_VIEW_DESC* _RTD/* = nullptr*/
	, const D3D11_SHADER_RESOURCE_VIEW_DESC* _SRD /*= nullptr*/)
{
	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & _BindFlag)
	{
		if (S_OK != Core_Class::PDevice()->CreateDepthStencilView(m_pTex, _DSD, &m_pDSV))
		{
			BBY;
			return;
		}
	}
	else 
	{
		if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & _BindFlag)
		{
			if (S_OK != Core_Class::PDevice()->CreateRenderTargetView(m_pTex, _RTD, &m_pRTV))
			{
				BBY;
				return;
			}
		}
		if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & _BindFlag)
		{
			if (S_OK != Core_Class::PDevice()->CreateShaderResourceView(m_pTex, _SRD, &m_pSRV))
			{
				BBY;
				return;
			}
		}
	}

	HRESULT TRes = DirectX::CaptureTexture(Core_Class::PDevice(), Core_Class::Context(), m_pTex, m_Image);

	// 이미지 커서 처리 못함 ㅋㅋㅋㅋㅋㅋㅋㅋ
	if (E_OUTOFMEMORY == TRes)
	{
		return;
	}
	if (S_OK != TRes)
	{
		BBY;
		return;
	}
}

void Texture::Set_Pixel(void* _pSrc, size_t _Size)
{
	size_t TexSize = VS_Format::Size_Format(m_Image.GetMetadata().format) *
		m_Image.GetMetadata().width * m_Image.GetMetadata().height;

	if (TexSize < _Size)
	{
		return;
	}


	// 텍스쳐를 그리는 녀석-> 이녀석이 저장되면 png가 되는 것이다.
	Core_Class::Context()->Map(m_pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubData);

	memcpy_s(SubData.pData, TexSize, _pSrc, _Size);
	Core_Class::Context()->Unmap(m_pTex, 0);
}