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

// 督析伯希
// 督析級精 牌雌 陥 働舛廃 汽戚斗姥繕研 伯希拭 旋嬢鎌陥. 
// 督析曽嫌原陥 稽漁敗呪亜 陥牽陥.

//  溌舌切 原陥 陥献 敗呪研 叔楳 -> 陥戚刑闘拭辞 益係惟 馬艦猿
// 更 雁尻廃 暗畏陥.
// 舘 督析 溌舌切澗 督析拭 獣拙 娃 溌舌切研 姥紺馬奄 是廃 析曽税 坪球幻 陥研屍
// 郊戚格軒 莫殿澗 旭陥. (章硲鉢馬走 照泌陥檎) 馬檎 更 差硲鉢 背醤馬澗 暗壱
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
		//png, bmp, jpg溌舌切澗 else稽 広製 しし 
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

	// 1託据戚虞 株戚 0
	tDecs.Width = _W;
	tDecs.ArraySize = 1;
	tDecs.BindFlags = _BindFlag;
	tDecs.Usage = _eUsage;
	tDecs.MipLevels = 1;
	tDecs.Format = _eFormat;
	tDecs.MiscFlags = 0;
	tDecs.CPUAccessFlags = 0;


	// 軒社什研 隔嬢爽澗 引舛
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

	// MipMap竺舛
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

	// 戚耕走 朕辞 坦軒 公敗 せせせせせせせせ
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


	// 努什団研 益軒澗 橿汐-> 戚橿汐戚 煽舌鞠檎 png亜 鞠澗 依戚陥.
	Core_Class::Context()->Map(m_pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubData);

	memcpy_s(SubData.pData, TexSize, _pSrc, _Size);
	Core_Class::Context()->Unmap(m_pTex, 0);
}