#include "DXContainer.h"


const KVector4 KVector4::Left = KVector4(-1.0f, 0.0f, 0.0f, 0.0f);
const KVector4 KVector4::Right = KVector4(1.0f, 0.0f, 0.0f, 0.0f);
const KVector4 KVector4::Up = KVector4(0.0f, 1.0f, 0.0f, 0.0f);
const KVector4 KVector4::Down = KVector4(0.0f, -1.0f, 0.0f, 0.0f);
const KVector4 KVector4::Forword = KVector4(0.0f, 0.0f, 1.0f, 0.0f);
const KVector4 KVector4::Back = KVector4(0.0f, 0.0f, -1.0f, 0.0f);
const KVector4 KVector4::Zero = KVector4(0.0f);
const KVector4 KVector4::One = KVector4(1.0f);


const KVector4 KColor::Color_None = KVector4(0.0f, 0.0f, 0.0f, 1.0f);
const KVector4 KColor::White = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
const KVector4 KColor::Black = KVector4(0.0f, 0.0f, 0.0f, 1.0f);
const KVector4 KColor::Red = KVector4(1.0f, 0.0f, 0.0f, 1.0f);
const KVector4 KColor::Green = KVector4(0.0f, 1.0f, 0.0f, 1.0f);
const KVector4 KColor::Blue = KVector4(0.0f, 0.0f, 1.0f, 1.0f);
const KVector4 KColor::Yellow = KVector4(1.0f, 1.0f, 0.0f, 1.0f);
const KVector4 KColor::Majenta = KVector4(1.0f, 0.0f, 1.0f, 1.0f);
const KVector4 KColor::Cyan = KVector4(0.0f, 1.0f, 1.0f, 1.0f);
const KVector4 KColor::Gray = KVector4(0.5f, 0.5f, 0.5f, 1.0f);
const KVector4 KColor::Brown = KVector4(0.5f, .25f, .0f, 1.0f);
const KVector4 KColor::Orange = KVector4(1.0f, 0.25f, 0.f, 1.0f);

const KVector3 KVector3::Zero = KVector3(.0f);
const KVector3 KVector3::Left = KVector3(-1.0f, .0f, .0f);
const KVector3 KVector3::Right = KVector3(1.0f, .0f, .0f);
const KVector3 KVector3::Up = KVector3(.0f, 1.0f, .0f);
const KVector3 KVector3::Down = KVector3(.0f, -1.0f, .0f);
const KVector3 KVector3::Forword = KVector3(0.0f, 0.0f, 1.0f);
const KVector3 KVector3::Back = KVector3(0.0f, 0.0f, -1.0f);
const KVector3 KVector3::One = KVector3(1.0f);

const KVector2 KVector2::Zero = KVector2(.0f, .0f);
const KVector2 KVector2::Left = KVector2(-1.0f, .0f);
const KVector2 KVector2::Right = KVector2(1.0f, .0f);
const KVector2 KVector2::Up = KVector2(.0f, 1.0f);
const KVector2 KVector2::Down = KVector2(.0f, -1.0f);


const float HMATHVAR::DTOR = DirectX::XM_PI / 180.0f;
const float HMATHVAR::RTOD = 180.0f / DirectX::XM_PI;



KUINT  VS_Format::Size_Format(DXGI_FORMAT _Fm)
{
	switch (_Fm)
	{
	case DXGI_FORMAT_UNKNOWN:
		break;
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 16;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 12;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
		return 8;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		return 4;
	case DXGI_FORMAT_R8G8_TYPELESS:
		break;
	case DXGI_FORMAT_R8G8_UNORM:
		break;
	case DXGI_FORMAT_R8G8_UINT:
		break;
	case DXGI_FORMAT_R8G8_SNORM:
		break;
	case DXGI_FORMAT_R8G8_SINT:
		break;
	case DXGI_FORMAT_R16_TYPELESS:
		break;
	case DXGI_FORMAT_R16_FLOAT:
		break;
	case DXGI_FORMAT_D16_UNORM:
		break;
	case DXGI_FORMAT_R16_UNORM:
		break;
	case DXGI_FORMAT_R16_UINT:
		break;
	case DXGI_FORMAT_R16_SNORM:
		break;
	case DXGI_FORMAT_R16_SINT:
		break;
	case DXGI_FORMAT_R8_TYPELESS:
		break;
	case DXGI_FORMAT_R8_UNORM:
		break;
	case DXGI_FORMAT_R8_UINT:
		break;
	case DXGI_FORMAT_R8_SNORM:
		break;
	case DXGI_FORMAT_R8_SINT:
		break;
	case DXGI_FORMAT_A8_UNORM:
		break;
	case DXGI_FORMAT_R1_UNORM:
		break;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		break;
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
		break;
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		break;
	case DXGI_FORMAT_BC1_TYPELESS:
		break;
	case DXGI_FORMAT_BC1_UNORM:
		break;
	case DXGI_FORMAT_BC1_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC2_TYPELESS:
		break;
	case DXGI_FORMAT_BC2_UNORM:
		break;
	case DXGI_FORMAT_BC2_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC3_TYPELESS:
		break;
	case DXGI_FORMAT_BC3_UNORM:
		break;
	case DXGI_FORMAT_BC3_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC4_TYPELESS:
		break;
	case DXGI_FORMAT_BC4_UNORM:
		break;
	case DXGI_FORMAT_BC4_SNORM:
		break;
	case DXGI_FORMAT_BC5_TYPELESS:
		break;
	case DXGI_FORMAT_BC5_UNORM:
		break;
	case DXGI_FORMAT_BC5_SNORM:
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		break;
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		break;
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		return 4;
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
		break;

	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC6H_TYPELESS:
		break;
	case DXGI_FORMAT_BC6H_UF16:
		break;
	case DXGI_FORMAT_BC6H_SF16:
		break;
	case DXGI_FORMAT_BC7_TYPELESS:
		break;
	case DXGI_FORMAT_BC7_UNORM:
		break;
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		break;
	case DXGI_FORMAT_AYUV:
		break;
	case DXGI_FORMAT_Y410:
		break;
	case DXGI_FORMAT_Y416:
		break;
	case DXGI_FORMAT_NV12:
		break;
	case DXGI_FORMAT_P010:
		break;
	case DXGI_FORMAT_P016:
		break;
	case DXGI_FORMAT_420_OPAQUE:
		break;
	case DXGI_FORMAT_YUY2:
		break;
	case DXGI_FORMAT_Y210:
		break;
	case DXGI_FORMAT_Y216:
		break;
	case DXGI_FORMAT_NV11:
		break;
	case DXGI_FORMAT_AI44:
		break;
	case DXGI_FORMAT_IA44:
		break;
	case DXGI_FORMAT_P8:
		break;
	case DXGI_FORMAT_A8P8:
		break;
	case DXGI_FORMAT_B4G4R4A4_UNORM:
		break;
	case DXGI_FORMAT_FORCE_UINT:
		break;
	default:
		break;
	}

	return 0;
}