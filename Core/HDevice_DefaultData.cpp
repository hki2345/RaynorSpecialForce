#include "KDevice.h"
#include "DXContainer_DE.h"
#include "KWindow.h"
#include "Stl_AID.h"
#include "KMesh.h"
#include "Core_Class.h"
#include "Shader_Vertex.h"
#include "Shader_Pixel.h"
#include "ResourceManager.h"
#include "KMaterial.h"
#include "KBlend.h"
#include "KFont.h"

bool KDevice::Init_BasicFigure2D() 
{
	ResourceManager<KBlend>::Create(L"AlphaBlend");

	// Color Vtx
	COLORVTX ArrColorVTX[4] = {};
	IDX16 ArrColorIDX[2] = {};
	ArrColorVTX[0].Pos = KVector4(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrColorVTX[1].Pos = KVector4(0.5f, 0.5f, 0.0f, 1.0f);
	ArrColorVTX[2].Pos = KVector4(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrColorVTX[3].Pos = KVector4(0.5f, -0.5f, 0.0f, 1.0f);

	ArrColorVTX[0].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColorVTX[1].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColorVTX[2].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColorVTX[3].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);

	ArrColorIDX[0] = IDX16(0, 3, 2);
	ArrColorIDX[1] = IDX16(0, 1, 3);

	WORD ArrLineRectIdX[5] = { 0, 1, 3, 2, 0 };



	// Tex Vtx
	TEXVTX ArrTexVTX[4] = {};
	IDX16 ArrTexIDX[2] = {};
	ArrTexVTX[0].Pos = KVector4(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrTexVTX[1].Pos = KVector4(0.5f, 0.5f, 0.0f, 1.0f);
	ArrTexVTX[2].Pos = KVector4(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrTexVTX[3].Pos = KVector4(0.5f, -0.5f, 0.0f, 1.0f);

	ArrTexVTX[0].Uv = KVector2(0.0f, 0.0f);
	ArrTexVTX[1].Uv = KVector2(1.0f, 0.0f);
	ArrTexVTX[2].Uv = KVector2(0.0f, 1.0f);
	ArrTexVTX[3].Uv = KVector2(1.0f, 1.0f);

	ArrTexIDX[0] = IDX16(0, 3, 2);
	ArrTexIDX[1] = IDX16(0, 1, 3);


	// Boder Rect Shader
	KPtr<Shader_Vertex> BRectV = ResourceManager<Shader_Vertex>::Load_FromKey(L"VSBRECTCOLOR", L"Shader", L"BRectShader.fx", "VS_BDebugRect");
	BRectV->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	BRectV->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	BRectV->Create_Layout();
	ResourceManager<Shader_Pixel>::Load_FromKey(L"PSBRECTCOLOR", L"Shader", L"BRectShader.fx", "PS_BDebugRect");

	KPtr<KMaterial> BRECTMAT = ResourceManager<KMaterial>::Create(L"BRECTMAT");
	BRECTMAT->Set_VTShader(L"VSBRECTCOLOR");
	BRECTMAT->Set_PXShader(L"PSBRECTCOLOR");


	// LineRenderData
	ResourceManager<KMesh>::Create(L"LINERECTMESH"
		, 4, (UINT)COLORVTX::TypeSize(), D3D11_USAGE_DYNAMIC, ArrColorVTX
		, 5, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrLineRectIdX
		, IDX16::FM(), D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	KPtr<Shader_Vertex> RectV = ResourceManager<Shader_Vertex>::Load_FromKey(L"VSDEBUGRECTCOLOR", L"Shader", L"DebugRectShader.fx", "VS_DebugRect");
	RectV->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RectV->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RectV->Create_Layout();
	ResourceManager<Shader_Pixel>::Load_FromKey(L"PSDEBUGRECTCOLOR", L"Shader", L"DebugRectShader.fx", "PS_DebugRect");

	KPtr<KMaterial> DEBUGMAT = ResourceManager<KMaterial>::Create(L"DEBUGRECTMAT");
	DEBUGMAT->Set_VTShader(L"VSDEBUGRECTCOLOR");
	DEBUGMAT->Set_PXShader(L"PSDEBUGRECTCOLOR");

	// ColorRenderData
	ResourceManager<KMesh>::Create(L"COLORMESH"
		, 4, (UINT)COLORVTX::TypeSize(), D3D11_USAGE_DYNAMIC, ArrColorVTX
		, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrColorIDX
		, IDX16::FM());

	KPtr<Shader_Vertex> ColV = ResourceManager<Shader_Vertex>::Load_FromKey(L"VSCOLOR", L"Shader", L"ColorShader.fx", "VS_Color");
	ColV->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ColV->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ColV->Create_Layout();
	ResourceManager<Shader_Pixel>::Load_FromKey(L"PSCOLOR", L"Shader", L"ColorShader.fx", "PS_Color");

	KPtr<KMaterial> COLORMAT = ResourceManager<KMaterial>::Create(L"COLORMAT");
	COLORMAT->Set_VTShader(L"VSCOLOR");
	COLORMAT->Set_PXShader(L"PSCOLOR");

	// TexRenderData
	ResourceManager<KMesh>::Create(L"TEXMESH"
		, 4, (UINT)TEXVTX::TypeSize(), D3D11_USAGE_DYNAMIC, ArrTexVTX
		, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrTexIDX
		, IDX16::FM());

	KPtr<Shader_Vertex> TexV = ResourceManager<Shader_Vertex>::Load_FromKey(L"VSTEX", L"Shader", L"TexShader.fx", "VS_Tex");
	TexV->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	TexV->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	TexV->Create_Layout();
	ResourceManager<Shader_Pixel>::Load_FromKey(L"PSTEX", L"Shader", L"TexShader.fx", "PS_Tex");

	KPtr<KMaterial> TEXMAT = ResourceManager<KMaterial>::Create(L"TEXMAT");
	TEXMAT->Set_VTShader(L"VSTEX");
	TEXMAT->Set_PXShader(L"PSTEX");
	TEXMAT->Set_Blend(L"AlphaBlend");

	// ImageData
	KPtr<Shader_Vertex> ImgV = ResourceManager<Shader_Vertex>::Load_FromKey(L"VSIMG", L"Shader", L"ImageShader.fx", "VS_Img");
	ImgV->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ImgV->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	ImgV->Create_Layout();
	ResourceManager<Shader_Pixel>::Load_FromKey(L"PSIMG", L"Shader", L"ImageShader.fx", "PS_Img");

	KPtr<KMaterial> IMGMAT = ResourceManager<KMaterial>::Create(L"IMGMAT");
	IMGMAT->Set_VTShader(L"VSIMG");
	IMGMAT->Set_PXShader(L"PSIMG");
	IMGMAT->Set_Blend(L"AlphaBlend");

	//ResourceManager<KBlend>::Create(L"AlphaBlend");
	//ResourceManager<KFont>::Create(L"±Ã¼­", L"±Ã¼­");

	//Core_Class::MainDevice().CreateCB<KMatrix>(L"TRANS", D3D11_USAGE_DYNAMIC, 0);
	//Core_Class::MainDevice().CreateCB<HCOLOR>(L"MULCOLOR", D3D11_USAGE_DYNAMIC, 0);
	//Core_Class::MainDevice().CreateCB<KVector4>(L"IMGUV", D3D11_USAGE_DYNAMIC, 1);

	return true;
}
