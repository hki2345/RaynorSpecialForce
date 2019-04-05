#include "KDevice.h"
#include "DXContainer_DE.h"
#include "KWindow.h"
#include "Stl_AID.h"
#include "KMesh.h"
#include "Core_Class.h"
#include "ResourceManager.h"
#include "KMaterial.h"
#include "KBlend.h"
#include "KFont.h"
#include "KMath.h"
#include "Light.h"
#include "Renderer.h"
#include "Texture.h"
#include "Sampler.h"
#include "RenderTarget_Multi.h"
#include "Renderer_Terrain.h"
#include "Renderer_Effect.h"

#include "Shader_Vertex.h"
#include "Shader_Pixel.h"
#include "Shader_Domain.h"
#include "Shader_Hul.h"
#include "Shader_GeoMetry.h"



bool KDevice::DefRenderTaget() 
{


	// Defferd용
	ResourceManager<RenderTarget>::Create(L"COLOR_DIFFUSE" , Core_Class::MainWindow().width_u() , Core_Class::MainWindow().height_u()	, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE	, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"POSTION", Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"NORMAL", Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"DEPTH", Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// KPtr<RenderTarget_Multi> DefMRT = ResourceManager<RenderTarget_Multi>::Create(L"DEFFERD", L"COLOR_DIFFUSE", L"POSTION", L"NORMAL", L"DEPTH");

	// Defferd Light 머지 용
	ResourceManager<RenderTarget>::Create(L"LIGHT_DIFFUSE", Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), KVector4::Black, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"LIGHT_SPECULAR", Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(),KVector4::Black,  D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	ResourceManager<RenderTarget_Multi>::Create(L"DEFFERD", L"COLOR_DIFFUSE", L"POSTION", L"NORMAL", L"DEPTH");
	ResourceManager<RenderTarget_Multi>::Create(L"LIGHT", L"LIGHT_DIFFUSE", L"LIGHT_SPECULAR");
	// KPtr<RenderTarget_Multi> LightMRT = ResourceManager<RenderTarget_Multi>::Create(L"LIGHT", L"LIGHT_DIFFUSE", L"LIGHT_SPECULAR");

	// 드로우 렌더 타겟
	ResourceManager<RenderTarget_Multi>::Create(L"DRAW", L"COLOR_DIFFUSE", L"POSTION", L"NORMAL", L"DEPTH");



	// 블룸 렌더 타겟
	ResourceManager<RenderTarget>::Create(L"S512", 512, 512, KVector::Blue, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"S256", 256, 256, KVector::Blue, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"S128", 128, 128, KVector::Blue, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"S64", 64, 64, KVector::Blue, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	ResourceManager<RenderTarget_Multi>::Create(L"CUT", L"S512", L"S256", L"S128", L"S64");



	return true;
}

bool KDevice::Def3DCreate()
{
	ResourceManager<Sampler>::Create(L"DefaultSmp");

	// WRAP 바둑판 배열을 하여라 -> 예 - 0과 3사이의 텍스쳐는 세번 반복된다.
	// MIRROR 텍스쳐를 뒤집는다. -> 0과 1은 정상 1과 2는 미러 2와 3은 다시 정상 식으로
	// CLAMP .0 ~ 1.0 범위의 칼라로 설정 -> 지금 쓰고 있는 것
	// MIRROR_ONCE MIRROR와 CLAMP의 짬뽕 -> 텍스쳐 좌표의 절대값을 취하고 최대값으로 클림핑 한다.
	// 지형 샘플러는 -> UV값이 정수그대로 적용된다. -> .0f ~ 1.0f 식의 비율 계산이 아님
	// 그냥 int형 으로 정수가 때려 박히는 식이다.
	ResourceManager<Sampler>::Create(L"TerrainSmp"
		, D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR
		, D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP
		, D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP
		, D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP);




	ResourceManager<KBlend>::Create(L"ALPHA");



	Core_Class::MainDevice().CreateCB<KColor>(L"FORCE_COLOR", D3D11_USAGE_DYNAMIC, 5);
	Core_Class::MainDevice().CreateCB<MatrixContainer>(L"MATCON", D3D11_USAGE_DYNAMIC, 10);
	Core_Class::MainDevice().CreateCB<RenderOption>(L"RENDEROPTION", D3D11_USAGE_DYNAMIC, 11);
	Core_Class::MainDevice().CreateCB<Light::LightCBDATA>(L"LIGHTDATA", D3D11_USAGE_DYNAMIC, 12);
	Core_Class::MainDevice().CreateCB<Light::LightData>(L"DEFFERDLIGHTDATA", D3D11_USAGE_DYNAMIC, 12);

	Core_Class::MainDevice().Create_RS(L"SNONE", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_NONE);
	Core_Class::MainDevice().Create_RS(L"SBACK", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK);
	Core_Class::MainDevice().Create_RS(L"SFRONT", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_FRONT);

	Core_Class::MainDevice().Create_RS(L"WNONE", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_NONE);
	Core_Class::MainDevice().Create_RS(L"WBACK", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_BACK);
	Core_Class::MainDevice().Create_RS(L"WFRONT", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_FRONT);
	Core_Class::MainDevice().Set_RSDef(L"SBACK");

	{
		D3D11_DEPTH_STENCIL_DESC DepthState;

		DepthState.DepthEnable = TRUE;
		// D3D11_DEPTH_WRITE_MASK_ALL 뎊스비교를 하겠다는 것이다.
		// D3D11_DEPTH_WRITE_MASK_ZERO 쓰지 않겠다.
		DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
		// 스텐실에 관련된 것.
		// 0x000000ff 가장 뒤에 사용하겠다.
		DepthState.StencilEnable = FALSE;
		DepthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		DepthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		D3D11_DEPTH_STENCILOP_DESC defaultStencil =
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
		DepthState.FrontFace = defaultStencil;
		DepthState.BackFace = defaultStencil;
		Core_Class::MainDevice().Create_DSS(L"EFFECT_POST", DepthState);
	}

	{
		D3D11_DEPTH_STENCIL_DESC DepthState;

		DepthState.DepthEnable = FALSE;
		// D3D11_DEPTH_WRITE_MASK_ALL 뎊스비교를 하겠다는 것이다.
		// D3D11_DEPTH_WRITE_MASK_ZERO 쓰지 않겠다.
		DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthState.DepthFunc = D3D11_COMPARISON_NEVER;
		// 스텐실에 관련된 것.
		// 0x000000ff 가장 뒤에 사용하겠다.
		DepthState.StencilEnable = FALSE;
		DepthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		DepthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		D3D11_DEPTH_STENCILOP_DESC defaultStencil =
		{ D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_ZERO, D3D11_COMPARISON_NEVER };
		DepthState.FrontFace = defaultStencil;
		DepthState.BackFace = defaultStencil;
		Core_Class::MainDevice().Create_DSS(L"EFFECT", DepthState);
	}

	{	// 기본 뎁스
		// 뎊스를 켜놓고
		D3D11_DEPTH_STENCIL_DESC DepthState;

		DepthState.DepthEnable = TRUE;
		// D3D11_DEPTH_WRITE_MASK_ALL 뎊스비교를 하겠다는 것이다.
		// D3D11_DEPTH_WRITE_MASK_ZERO 쓰지 않겠다.
		DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
		// 스텐실에 관련된 것.
		// 0x000000ff 가장 뒤에 사용하겠다.
		DepthState.StencilEnable = FALSE;
		DepthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		DepthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		D3D11_DEPTH_STENCILOP_DESC defaultStencil =
		{	D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, 
			D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
		DepthState.FrontFace = defaultStencil;
		DepthState.BackFace = defaultStencil;
		Core_Class::MainDevice().Create_DSS(L"DEBUG", DepthState);

		// 정상적인것.
		DepthState.DepthFunc = D3D11_COMPARISON_LESS;
		Core_Class::MainDevice().Create_DSS(L"BASIC", DepthState);

		DepthState.DepthFunc = D3D11_COMPARISON_GREATER;
		Core_Class::MainDevice().Create_DSS(L"GREATER", DepthState);

		DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Core_Class::MainDevice().Create_DSS(L"ALWAYS", DepthState);

		// 깊이 스텐실 버퍼에 대한 쓰기를 끈다
		DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Core_Class::MainDevice().Create_DSS(L"LIGHTDEPTH", DepthState);


		//////////////// VOLUMBACK
		DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthState.DepthFunc = D3D11_COMPARISON_GREATER;
		DepthState.StencilEnable = TRUE;

		D3D11_DEPTH_STENCILOP_DESC LightStencil;
		LightStencil.StencilFunc = D3D11_COMPARISON_ALWAYS;
		LightStencil.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		LightStencil.StencilPassOp = D3D11_STENCIL_OP_REPLACE;

		DepthState.FrontFace = LightStencil;
		DepthState.BackFace = LightStencil;
		Core_Class::MainDevice().Create_DSS(L"BACK_ST", DepthState);

		//////////////// VOLUMFORONT
		DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthState.DepthFunc = D3D11_COMPARISON_LESS;
		DepthState.StencilEnable = TRUE;

		LightStencil.StencilFunc = D3D11_COMPARISON_EQUAL;
		LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		LightStencil.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		LightStencil.StencilPassOp = D3D11_STENCIL_OP_KEEP;

		DepthState.FrontFace = LightStencil;
		DepthState.BackFace = LightStencil;
		Core_Class::MainDevice().Create_DSS(L"FRONT_ST", DepthState);

		//////////////// PASS

		DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
		DepthState.StencilEnable = TRUE;

		LightStencil.StencilFunc = D3D11_COMPARISON_EQUAL;
		LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		LightStencil.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		LightStencil.StencilPassOp = D3D11_STENCIL_OP_ZERO;

		DepthState.FrontFace = LightStencil;
		DepthState.BackFace = LightStencil;

		Core_Class::MainDevice().Create_DSS(L"PASS_ST", DepthState);

		// 하나를 더 만들어야 한다.

		Core_Class::MainDevice().Set_DSSDef(L"BASIC");
	}

	{
		D3D11_BLEND_DESC m_Desc;

		m_Desc.AlphaToCoverageEnable = false;

		// 다른 랜더 타겟도 따로따로 쓰겠다 혹은 아니다.
		// 블랜드 설정이 false로 하면 0번으로 초기화 된다.
		m_Desc.IndependentBlendEnable = false;


		// 디퓨즈 -> 디퓨즈는 기존 꺼에 덮어 씌우는 개념 ㅇㅇ
		m_Desc.RenderTarget[0].BlendEnable = true;
		m_Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		m_Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		m_Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		m_Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		m_Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		m_Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		m_Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;



		for (size_t i = 1; i < 8; i++)
		{
			m_Desc.RenderTarget[i] = D3D11_RENDER_TARGET_BLEND_DESC{};
		}


		ResourceManager<KBlend>::Create(L"FORWARD_COVER", m_Desc);
	}

	///////////////////////////////////////////// BS
	D3D11_BLEND_DESC m_Desc;

	m_Desc.AlphaToCoverageEnable = false;

	// 다른 랜더 타겟도 따로따로 쓰겠다 혹은 아니다.
	// 블랜드 설정이 false로 하면 0번으로 초기화 된다.
	m_Desc.IndependentBlendEnable = true;


	// 디퓨즈 -> 디퓨즈는 기존 꺼에 덮어 씌우는 개념 ㅇㅇ
	m_Desc.RenderTarget[0].BlendEnable = true;
	m_Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	m_Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;

	m_Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;


	// 스펙 큘러 - 색이 있으면 알파 랜더러 적요 -> 기존 알파렌더와 같은 방식으로 적용된다.
	// 기존 알파 방식은 -> 알파와 알파가 섞이는 식임
	m_Desc.RenderTarget[1].BlendEnable = true;
	m_Desc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Desc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[1].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	m_Desc.RenderTarget[1].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	m_Desc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_Desc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;


	for (size_t i = 2; i < 8; i++)
	{
		m_Desc.RenderTarget[i] = D3D11_RENDER_TARGET_BLEND_DESC{};
	}

	ResourceManager<KBlend>::Create(L"LIGHTONE", m_Desc);



	// 볼륨메쉬는 블랜드를 끈다 -> 볼륨은 충돌 검사를 위해서 임
	m_Desc.RenderTarget[0] = D3D11_RENDER_TARGET_BLEND_DESC{};
	m_Desc.RenderTarget[0].RenderTargetWriteMask = 0;
	
	ResourceManager<KBlend>::Create(L"VOLUME", m_Desc);


	return true;
}
bool KDevice::Mesh3DCreate() {

	/* Mesh */
#pragma region Rect
	VTX3D ArrVTX3D[4];
	ArrVTX3D[0].Pos = KVector4(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrVTX3D[1].Pos = KVector4(0.5f, 0.5f, 0.0f, 1.0f);
	ArrVTX3D[2].Pos = KVector4(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrVTX3D[3].Pos = KVector4(0.5f, -0.5f, 0.0f, 1.0f);

	ArrVTX3D[0].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrVTX3D[1].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrVTX3D[2].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrVTX3D[3].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);

	ArrVTX3D[0].Uv = KVector2(0.0f, 0.0f);
	ArrVTX3D[1].Uv = KVector2(1.0f, 0.0f);
	ArrVTX3D[2].Uv = KVector2(0.0f, 1.0f);
	ArrVTX3D[3].Uv = KVector2(1.0f, 1.0f);

	ArrVTX3D[0].Normal = KVector4(0.0f, 0.0f, -1.0f, 0.0f);
	ArrVTX3D[1].Normal = KVector4(0.0f, 0.0f, -1.0f, 0.0f);
	ArrVTX3D[2].Normal = KVector4(0.0f, 0.0f, -1.0f, 0.0f);
	ArrVTX3D[3].Normal = KVector4(0.0f, 0.0f, -1.0f, 0.0f);

	ArrVTX3D[0].Tangent = KVector4(1.0f, 0.0f, 0.0f, 0.0f);
	ArrVTX3D[1].Tangent = KVector4(1.0f, 0.0f, 0.0f, 0.0f);
	ArrVTX3D[2].Tangent = KVector4(1.0f, 0.0f, 0.0f, 0.0f);
	ArrVTX3D[3].Tangent = KVector4(1.0f, 0.0f, 0.0f, 0.0f);

	ArrVTX3D[0].BNormal = KVector4(0.0f, -1.0f, 0.0f, 0.0f);
	ArrVTX3D[1].BNormal = KVector4(0.0f, -1.0f, 0.0f, 0.0f);
	ArrVTX3D[2].BNormal = KVector4(0.0f, -1.0f, 0.0f, 0.0f);
	ArrVTX3D[3].BNormal = KVector4(0.0f, -1.0f, 0.0f, 0.0f);

	IDX16 ArrColorIDX[2] = {};

	ArrColorIDX[0] = IDX16(0, 3, 2);
	ArrColorIDX[1] = IDX16(0, 1, 3);

	ResourceManager<KMesh>::Create(L"RECT"
		, 4, (UINT)VTX3D::TypeSize(), D3D11_USAGE_DYNAMIC, ArrVTX3D
		, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrColorIDX
		, IDX16::FM());
#pragma endregion

#pragma region Cube

	VTX3D Cube3D_Vert[8] = {};
	IDX16 Cube3D_Idx[12] = {};

	Cube3D_Vert[0].Pos = KVector4(-0.5f, 0.5f, -0.5f, 1.0f);
	Cube3D_Vert[1].Pos = KVector4(0.5f, 0.5f, -0.5f, 1.0f);
	Cube3D_Vert[2].Pos = KVector4(-0.5f, -0.5f, -0.5f, 1.0f);
	Cube3D_Vert[3].Pos = KVector4(0.5f, -0.5f, -0.5f, 1.0f);
	Cube3D_Vert[4].Pos = KVector4(-0.5f, 0.5f, 0.5f, 1.0f);
	Cube3D_Vert[5].Pos = KVector4(0.5f, 0.5f, 0.5f, 1.0f);
	Cube3D_Vert[6].Pos = KVector4(-0.5f, -0.5f, 0.5f, 1.0f);
	Cube3D_Vert[7].Pos = KVector4(0.5f, -0.5f, 0.5f, 1.0f);

	Cube3D_Vert[0].Normal = KVector4(-0.5f, 0.5f, -0.5f, .0f);
	Cube3D_Vert[1].Normal = KVector4(0.5f, 0.5f, -0.5f, .0f);
	Cube3D_Vert[2].Normal = KVector4(-0.5f, -0.5f, -0.5f, .0f);
	Cube3D_Vert[3].Normal = KVector4(0.5f, -0.5f, -0.5f, .0f);
	Cube3D_Vert[4].Normal = KVector4(-0.5f, 0.5f, 0.5f, .0f);
	Cube3D_Vert[5].Normal = KVector4(0.5f, 0.5f, 0.5f, .0f);
	Cube3D_Vert[6].Normal = KVector4(-0.5f, -0.5f, 0.5f, .0f);
	Cube3D_Vert[7].Normal = KVector4(0.5f, -0.5f, 0.5f, .0f);

	for (size_t i = 0; i < 8; i++)
	{
		Cube3D_Vert[i].Normal.NormalizeVec3();
	}

	Cube3D_Vert[0].Uv = KVector2(0.0f, 0.0f);
	Cube3D_Vert[1].Uv = KVector2(1.0f, 0.0f);
	Cube3D_Vert[2].Uv = KVector2(0.0f, 1.0f);
	Cube3D_Vert[3].Uv = KVector2(1.0f, 1.0f);
	Cube3D_Vert[4].Uv = KVector2(0.0f, 0.0f);
	Cube3D_Vert[5].Uv = KVector2(1.0f, 0.0f);
	Cube3D_Vert[6].Uv = KVector2(1.0f, 1.0f);
	Cube3D_Vert[7].Uv = KVector2(0.0f, 1.0f);

	Cube3D_Vert[0].Color = KColor::White;
	Cube3D_Vert[1].Color = KColor::White;
	Cube3D_Vert[2].Color = KColor::White;
	Cube3D_Vert[3].Color = KColor::White;
	Cube3D_Vert[4].Color = KColor::White;
	Cube3D_Vert[5].Color = KColor::White;
	Cube3D_Vert[6].Color = KColor::White;
	Cube3D_Vert[7].Color = KColor::White;



	Cube3D_Idx[0] = IDX16(0, 3, 2);
	Cube3D_Idx[1] = IDX16(0, 1, 3);
	Cube3D_Idx[2] = IDX16(0, 2, 6);
	Cube3D_Idx[3] = IDX16(0, 6, 4);
	Cube3D_Idx[4] = IDX16(4, 1, 0);
	Cube3D_Idx[5] = IDX16(4, 5, 1);
	Cube3D_Idx[6] = IDX16(4, 6, 5);
	Cube3D_Idx[7] = IDX16(5, 6, 7);
	Cube3D_Idx[8] = IDX16(1, 7, 3);
	Cube3D_Idx[9] = IDX16(1, 5, 7);
	Cube3D_Idx[10] = IDX16(7, 6, 3);
	Cube3D_Idx[11] = IDX16(2, 3, 6);


	ResourceManager<KMesh>::Create(L"CUBE",
		8, (UINT)sizeof(VTX3D), D3D11_USAGE_DYNAMIC, Cube3D_Vert,
		36, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, Cube3D_Idx,
		IDX16::FM()/*, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP*/);


	ResourceManager<KMesh>::Create(L"DEBUGCUBE",
		8, (UINT)sizeof(VTX3D), D3D11_USAGE_DYNAMIC, Cube3D_Vert,
		36, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, Cube3D_Idx,
		IDX16::FM(), D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

#pragma endregion

#pragma region Sphere
	VTX3D V;
	
	std::vector<VTX3D> SphereVtx;
	std::vector<UINT> SphereIdx;

	float fRadius = 0.5f;

	// 북극점
	V.Pos = KVector4{ 0.0f, fRadius, 0.0f, 1.0f };
	V.Uv = KVector2{ 0.5f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = V.Pos;
	V.Normal.NormalizeVec3();
	V.Normal.w = 0.0f;
	V.Tangent = KVector4{ 1.0f, 0.0f, 0.0f, 0.0f };
	V.BNormal = KVector4{ 0.0f, 0.0f, 1.0f, 0.0f };

	SphereVtx.push_back(V);

	UINT iStackCount = 40; // 가로 분할 개수
	UINT iSliceCount = 40; // 세로 분할 개수

	float yRotAngle = DirectX::XM_PI / (float)iStackCount;
	float zRotAngle = DirectX::XM_2PI / (float)iSliceCount;

	float yUvRatio = 1.0f / (float)iSliceCount;
	float zUvRatio = 1.0f / (float)iStackCount;

	// PointToDir

	for (UINT y = 1; y < iStackCount; ++y)
	{
		float phi = y * yRotAngle;

		for (UINT z = 0; z < iSliceCount + 1; ++z)
		{
			float theta = z * zRotAngle;

			V.Pos = KVector4
			{
				fRadius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
				fRadius * cosf(y * yRotAngle),
				fRadius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
				1.0f
			};

			V.Pos.ElipseCut();

			V.Pos.w = 1.0f;
			V.Uv = KVector2{ yUvRatio * z, zUvRatio * y };
			V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
			V.Normal = V.Pos;
			V.Normal.NormalizeVec3();
			V.Normal.w = 0.0f;

			V.Tangent.x = -fRadius * sinf(phi) * sinf(theta);
			V.Tangent.y = 0.0f;
			V.Tangent.z = fRadius * sinf(phi) * cosf(theta);
			V.Tangent.NormalizeVec3();
			V.Tangent.w = 0.0f;

			V.BNormal = KVector4::cross3D(V.Tangent, V.Normal);
			V.BNormal.NormalizeVec3();
			V.BNormal.w = 0.0f;

			SphereVtx.push_back(V);
		}
	}

	// 북극점
	V.Pos = KVector4{ 0.0f, -fRadius, 0.0f, 1.0f };
	V.Uv = KVector2{ 0.5f, 1.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = V.Pos;
	V.Normal.NormalizeVec3();
	V.Normal.w = 0.0f;

	SphereVtx.push_back(V);

	SphereIdx.clear();

	// 북극점 인덱스
	for (UINT i = 0; i < iSliceCount; i++)
	{
		SphereIdx.push_back(0);
		SphereIdx.push_back(i + 2);
		SphereIdx.push_back(i + 1);
	}


	for (UINT y = 0; y < iStackCount - 2; ++y)
	{
		for (UINT z = 0; z < iSliceCount; ++z)
		{
			SphereIdx.push_back((iSliceCount + 1) * y + z + 1);
			SphereIdx.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
			SphereIdx.push_back((iSliceCount + 1) * (y + 1) + z + 1);

			SphereIdx.push_back((iSliceCount + 1) * y + z + 1);
			SphereIdx.push_back((iSliceCount + 1) * y + (z + 1) + 1);
			SphereIdx.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
		}
	}

	// 북극점 인덱스

	UINT iBotStart = (UINT)SphereVtx.size() - 1;

	for (UINT i = 0; i < iSliceCount; ++i)
	{
		SphereIdx.push_back(iBotStart);
		SphereIdx.push_back(iBotStart - (i + 2));
		SphereIdx.push_back(iBotStart - (i + 1));
	}

	ResourceManager<KMesh>::Create(L"SPHERE"
		, (UINT)SphereVtx.size(), (UINT)VTX3D::TypeSize(), D3D11_USAGE_DYNAMIC, &SphereVtx[0]
		, (UINT)SphereIdx.size(), (UINT)IDX32::MemberSize(), D3D11_USAGE_DEFAULT, &SphereIdx[0]
		, IDX32::FM());


	std::vector<VTX3D> SkyVtx = SphereVtx;

	//for (size_t i = 0; i < SkyVtx.size(); i++)
	//{
	//	SkyVtx[i].Normal = .0f;
	//	SkyVtx[i].Normal.w = -5.0f;
	//	//V.Normal.w = 0.0f;
	//}

	//ResourceManager<KMesh>::Create(L"SKYSPHERE"
	//	, (UINT)SkyVtx.size(), (UINT)VTX3D::TypeSize(), D3D11_USAGE_DYNAMIC, &SkyVtx[0]
	//	, (UINT)SphereIdx.size(), (UINT)IDX32::MemberSize(), D3D11_USAGE_DEFAULT, &SphereIdx[0]
	//	, IDX32::FM());

#pragma endregion
	return true;
}


bool KDevice::Mat3DCreate() 
{

	// NONE
	KPtr<Shader_Vertex> NONE3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"NONEVTX", L"Shader", L"NONE.fx", "VTXNONE3D");
	NONE3DVTX->Add_LayoutFin("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> NONE3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"NONEPIX", L"Shader", L"NONE.fx", "PIXNONE3D");

	KPtr<KMaterial> NONEMAT = ResourceManager<KMaterial>::Create(L"NONEMAT");
	NONEMAT->Set_VTShader(L"NONEVTX");
	NONEMAT->Set_PXShader(L"NONEPIX");
	NONEMAT->Set_Blend(L"ALPHA");



	KPtr<Shader_Vertex> TAGETDEBUGVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"TAGETDEBUGVTX", L"Shader", L"TagetDebug.fx", "VS_TagetTex");
	TAGETDEBUGVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	TAGETDEBUGVTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<Shader_Pixel> TAGETDEBUGPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"TAGETDEBUGPIX", L"Shader", L"TagetDebug.fx", "PS_TagetTex");

	KPtr<KMaterial> TAGETDEBUGMAT = ResourceManager<KMaterial>::Create(L"TAGETDEBUGMAT");
	TAGETDEBUGMAT->Set_VTShader(L"TAGETDEBUGVTX");
	TAGETDEBUGMAT->Set_PXShader(L"TAGETDEBUGPIX");
	TAGETDEBUGMAT->Set_Blend(L"ALPHA");

	///////////////////////////////////////// GRID
	KPtr<Shader_Vertex> GRID3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"GRID3DVTX", L"Shader", L"GRID3D.fx", "VS_GRID3D");
	GRID3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	GRID3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	GRID3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	GRID3DVTX->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	KPtr<Shader_Pixel> GRID3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"GRID3DPIX", L"Shader", L"GRID3D.fx", "PS_GRID3D");
	GRID3DPIX->CreateCB<KVector4>(L"GRIDDATA", D3D11_USAGE_DYNAMIC, 0);

	KPtr<KMaterial> GRID3DMAT = ResourceManager<KMaterial>::Create(L"GRID3DMAT");
	GRID3DMAT->Set_VTShader(L"GRID3DVTX");
	GRID3DMAT->Set_PXShader(L"GRID3DPIX");
	GRID3DMAT->Set_Blend(L"ALPHA");

	// RECT
	KPtr<Shader_Vertex> RECT3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"RECT3DVTX", L"Shader", L"RECT3D.fx", "VS_RECT3D");
	RECT3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RECT3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	RECT3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RECT3DVTX->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> RECT3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"RECT3DPIX", L"Shader", L"RECT3D.fx", "PS_RECT3D");
	RECT3DPIX->CreateCB<KVector>(L"FADE_CUT", D3D11_USAGE_DYNAMIC, 5);

	KPtr<KMaterial> RECT3DMAT = ResourceManager<KMaterial>::Create(L"RECT3DMAT");
	RECT3DMAT->Set_VTShader(L"RECT3DVTX");
	RECT3DMAT->Set_PXShader(L"RECT3DPIX");
	RECT3DMAT->Set_Blend(L"ALPHA");

	// Image 
	KPtr<Shader_Vertex> IMGVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"IMGVTX", L"Shader", L"ImageShader.fx", "VS_Img");
	IMGVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	IMGVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<Shader_Pixel> IMGPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"IMGPIX", L"Shader", L"ImageShader.fx", "PS_Img");

	KPtr<KMaterial> IMGMAT = ResourceManager<KMaterial>::Create(L"IMGMAT");
	IMGMAT->Set_VTShader(L"IMGVTX");
	IMGMAT->Set_PXShader(L"IMGPIX");
	IMGMAT->Set_Blend(L"ALPHA");

	{
		// SKY FORWARD
		KPtr<Shader_Vertex> SKY3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"SKY3DVTXFORWARD", L"Shader", L"SkyBox.fx", "VS_SKY3D");
		SKY3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		SKY3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
		SKY3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		SKY3DVTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		SKY3DVTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		SKY3DVTX->Add_LayoutFin("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		KPtr<Shader_Pixel> SKY3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"SKY3DPIXFORWARD", L"Shader", L"SkyBox.fx", "PS_SKY3DFORWARD");

		KPtr<KMaterial> SKY3DMAT = ResourceManager<KMaterial>::Create(L"SKY3DMATFORWARD");
		SKY3DMAT->Set_VTShader(L"SKY3DVTXFORWARD");
		SKY3DMAT->Set_PXShader(L"SKY3DPIXFORWARD");
		SKY3DMAT->Set_Blend(L"ALPHA");
	}

	{
		// SKY DEFFERD
		KPtr<Shader_Vertex> SKY3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"SKY3DVTXDEFFERD", L"Shader", L"SkyBox.fx", "VS_SKY3D");
		SKY3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		SKY3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
		SKY3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		SKY3DVTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		SKY3DVTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		SKY3DVTX->Add_LayoutFin("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		KPtr<Shader_Pixel> SKY3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"SKY3DPIXDEFFERD", L"Shader", L"SkyBox.fx", "PS_SKY3DDEFFERD");

		KPtr<KMaterial> SKY3DMAT = ResourceManager<KMaterial>::Create(L"SKY3DMATDEFFERD");
		SKY3DMAT->Set_VTShader(L"SKY3DVTXDEFFERD");
		SKY3DMAT->Set_PXShader(L"SKY3DPIXDEFFERD");
		SKY3DMAT->Set_Blend(L"ALPHA");
	}


	{
		// DEBUG
		KPtr<Shader_Vertex> VTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DEBUGVTX", L"Shader", L"DebugShader.fx", "VS_DEBUG");
		VTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
		VTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_LayoutFin("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		KPtr<Shader_Pixel> PIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DEBUGPIX", L"Shader", L"DebugShader.fx", "PS_DEBUG");
		PIX->CreateCB<KVector>(L"DEBUG_COLOR", D3D11_USAGE_DYNAMIC, 2);

		KPtr<KMaterial> MTL = ResourceManager<KMaterial>::Create(L"DEBUGMTL");
		MTL->Set_VTShader(L"DEBUGVTX");
		MTL->Set_PXShader(L"DEBUGPIX");
		MTL->Set_Blend(L"ALPHA");
		MTL->Insert_TexData(TEX_TYPE::TEX_TARGET, 1, L"DEPTH");
	}


	KPtr<Shader_Vertex> MESH3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"MESH3DVTX", L"Shader", L"MeshMat.fx", "VS_MESH3D");
	MESH3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	MESH3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->Add_LayoutFin("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> MESH3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"MESH3DPIX", L"Shader", L"MeshMat.fx", "PS_MESH3D");

	KPtr<KMaterial> MESH3DMAT = ResourceManager<KMaterial>::Create(L"MESH3DMAT");
	MESH3DMAT->Set_VTShader(L"MESH3DVTX");
	MESH3DMAT->Set_PXShader(L"MESH3DPIX");
	MESH3DMAT->Set_Blend(L"ALPHA");



	// FORWARD
	KPtr<Shader_Vertex> FORWARD3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"FORWARD3DVTX", L"Shader", L"Forward.fx", "VS_FORWARD");
	FORWARD3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	FORWARD3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	FORWARD3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	FORWARD3DVTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	FORWARD3DVTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	FORWARD3DVTX->Add_LayoutFin("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> FORWARD3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"FORWARD3DPIX", L"Shader", L"Forward.fx", "PS_FORWARD");

	KPtr<KMaterial> FORWARD3DMAT = ResourceManager<KMaterial>::Create(L"FORWARD3DMAT");
	FORWARD3DMAT->Set_VTShader(L"FORWARD3DVTX");
	FORWARD3DMAT->Set_PXShader(L"FORWARD3DPIX");
	FORWARD3DMAT->Set_Blend(L"ALPHA");

	

	// DEFFERD
	KPtr<Shader_Vertex> DEFFERD3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DEFFERD3DVTX", L"Shader", L"Defferd.fx", "VS_DEFFERD");
	DEFFERD3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	DEFFERD3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->Add_LayoutFin("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> DEFFERD3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DEFFERD3DPIX", L"Shader", L"Defferd.fx", "PS_DEFFERD");

	KPtr<KMaterial> DEFFERD3DMAT = ResourceManager<KMaterial>::Create(L"DEFFERD3DMAT");
	DEFFERD3DMAT->Set_VTShader(L"DEFFERD3DVTX");
	DEFFERD3DMAT->Set_PXShader(L"DEFFERD3DPIX");
	DEFFERD3DMAT->Set_Blend(L"ALPHA");


	// Bone Ani Mat
	KPtr<Shader_Vertex> DEFFERD3DANIVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DEFFERD3DANIVTX", L"Shader", L"DefferdAni.fx", "VS_DEFFERDANI");
	DEFFERD3DANIVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DANIVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	DEFFERD3DANIVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DANIVTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DANIVTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DANIVTX->Add_Layout("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DANIVTX->Add_Layout("WEIGHTS", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DANIVTX->Add_LayoutFin("BLENDINDICES", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> DEFFERD3DANIPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DEFFERD3DANIPIX", L"Shader", L"DefferdAni.fx", "PS_DEFFERDANI");

	KPtr<KMaterial> DEFFERD3DANIMAT = ResourceManager<KMaterial>::Create(L"DEFFERD3DANIMAT");
	DEFFERD3DANIMAT->Set_VTShader(L"DEFFERD3DANIVTX");
	DEFFERD3DANIMAT->Set_PXShader(L"DEFFERD3DANIPIX");
	DEFFERD3DANIMAT->Set_Blend(L"ALPHA");




	// DefferdDirLight
	KPtr<Shader_Vertex> DEFFERDLIGHTVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DEFFERDLIGHTVTX", L"Shader", L"Defferd.fx", "VS_DEFFERDLIGHT");
	DEFFERDLIGHTVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERDLIGHTVTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<Shader_Pixel> DEFFERDLIGHTPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DEFFERDLIGHTPIX", L"Shader", L"Defferd.fx", "PS_DEFFERDLIGHT");

	KPtr<KMaterial> DEFFERDMAT = ResourceManager<KMaterial>::Create(L"DEFFERDLIGHTMAT");
	DEFFERDMAT->Set_VTShader(L"DEFFERDLIGHTVTX");
	DEFFERDMAT->Set_PXShader(L"DEFFERDLIGHTPIX");
	DEFFERDMAT->Set_Blend(L"LIGHTONE");
	DEFFERDMAT->Insert_TexData(TEX_TYPE::TEX_TARGET, 0, L"POSTION");
	DEFFERDMAT->Insert_TexData(TEX_TYPE::TEX_TARGET, 1, L"NORMAL");
	DEFFERDMAT->Insert_TexData(TEX_TYPE::TEX_TARGET, 2, L"DEPTH");




	// LIGHT MERGE
	KPtr<Shader_Vertex> DEFFERDMERGEVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DEFFERDMERGEVTX", L"Shader", L"Defferd.fx", "VS_DEFFERDMERGE");
	DEFFERDMERGEVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERDMERGEVTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<Shader_Pixel> DEFFERDMERGEPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DEFFERDMERGEPIX", L"Shader", L"Defferd.fx", "PS_DEFFERDMERGE");

	KPtr<KMaterial> DEFFERDMERGEMAT = ResourceManager<KMaterial>::Create(L"DEFFERDMERGEMAT");
	DEFFERDMERGEMAT->Set_VTShader(L"DEFFERDMERGEVTX");
	DEFFERDMERGEMAT->Set_PXShader(L"DEFFERDMERGEPIX");
	DEFFERDMERGEMAT->Set_Blend(L"ALPHA");
	DEFFERDMERGEMAT->Insert_TexData(TEX_TYPE::TEX_TARGET, 0, L"COLOR_DIFFUSE");
	DEFFERDMERGEMAT->Insert_TexData(TEX_TYPE::TEX_TARGET, 1, L"LIGHT_DIFFUSE");
	DEFFERDMERGEMAT->Insert_TexData(TEX_TYPE::TEX_TARGET, 2, L"LIGHT_SPECULAR");




	// SCREEN MERGE
	KPtr<Shader_Vertex> SCREENMERGEVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"SCREENMERGEVTX", L"Shader", L"ScreenMerge.fx", "VS_SCREENMERGE");
	SCREENMERGEVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	SCREENMERGEVTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<Shader_Pixel> SCREENMERGEPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"SCREENMERGEPIX", L"Shader", L"ScreenMerge.fx", "PS_SCREENMERGE");

	KPtr<KMaterial> SCREENMERGEMAT = ResourceManager<KMaterial>::Create(L"SCREENMERGEMAT");
	SCREENMERGEMAT->Set_Blend(L"ALPHA");
	SCREENMERGEMAT->Set_VTShader(L"SCREENMERGEVTX");
	SCREENMERGEMAT->Set_PXShader(L"SCREENMERGEPIX");



	// VOLUME - 충돌
	KPtr<Shader_Vertex> VOLUMEVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"VOLUMEVTX", L"Shader", L"VolumeMesh.fx", "VS_VOLUME");
	VOLUMEVTX->Add_LayoutFin("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> VOLUMEPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"VOLUMEPIX", L"Shader", L"VolumeMesh.fx", "PS_VOLUME");

	KPtr<KMaterial> VOLUMEMAT = ResourceManager<KMaterial>::Create(L"VOLUMEMAT");
	VOLUMEMAT->Set_Blend(L"VOLUME");
	VOLUMEMAT->Set_VTShader(L"VOLUMEVTX");
	VOLUMEMAT->Set_PXShader(L"VOLUMEPIX");



	KPtr<Shader_Vertex> DEFFERDTERRAINVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DEFFERDTERRAINVTX", L"Shader", L"TerrainDefferd.fx", "VS_TERRAINDEFFERD");
	DEFFERDTERRAINVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERDTERRAINVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	DEFFERDTERRAINVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERDTERRAINVTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERDTERRAINVTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERDTERRAINVTX->Add_LayoutFin("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> DEFFERDTERRAINPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DEFFERDTERRAINPIX", L"Shader", L"TerrainDefferd.fx", "PS_TERRAINDEFFERD");
	DEFFERDTERRAINPIX->CreateCB<TERRAIN_FD>(L"TERRAIN_FD", D3D11_USAGE_DYNAMIC, 0);
	DEFFERDTERRAINPIX->CreateCB<DRAW_INFO>(L"DRAW_INFO", D3D11_USAGE_DYNAMIC, 1);

	KPtr<KMaterial> DEFFERDTERRAINMAT = ResourceManager<KMaterial>::Create(L"DEFFERDTERRAINMAT");
	DEFFERDTERRAINMAT->Set_VTShader(L"DEFFERDTERRAINVTX");
	DEFFERDTERRAINMAT->Set_PXShader(L"DEFFERDTERRAINPIX");
	DEFFERDTERRAINMAT->Set_Blend(L"ALPHA");



	KPtr<Shader_Vertex> DTESSLEVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DTESSLEVTX", L"Shader", L"TerrainDefferd_Tessel.fx", "VS_TERRAINDEFFERD");
	DTESSLEVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DTESSLEVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	DTESSLEVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DTESSLEVTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DTESSLEVTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DTESSLEVTX->Add_LayoutFin("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	
	KPtr<Shader_Hul>	DTESSLEHUL = ResourceManager<Shader_Hul>::Load_FromKey(L"DTESSLEHUL", L"Shader", L"TerrainDefferd_Tessel.fx", "HS_TERRAINEDEFFERED");
	KPtr<Shader_Domain> DTESSLEDOM = ResourceManager<Shader_Domain>::Load_FromKey(L"DTESSLEDOM", L"Shader", L"TerrainDefferd_Tessel.fx", "DS_TERRAINEDEFFERED");
	KPtr<Shader_Pixel>	DTESSLEPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DTESSLEPIX", L"Shader", L"TerrainDefferd_Tessel.fx", "PS_TERRAINDEFFERD");
	DTESSLEPIX->CreateCB<TERRAIN_FD>(L"TERRAIN_FD", D3D11_USAGE_DYNAMIC, 0);
	DTESSLEPIX->CreateCB<DRAW_INFO>(L"DRAW_INFO", D3D11_USAGE_DYNAMIC, 1);

	KPtr<KMaterial> DTESSLEMAT = ResourceManager<KMaterial>::Create(L"DTESSLEMAT");
	DTESSLEMAT->Set_VTShader(L"DTESSLEVTX");
	DTESSLEMAT->Set_HUShader(L"DTESSLEHUL");
	DTESSLEMAT->Set_DMShader(L"DTESSLEDOM");
	DTESSLEMAT->Set_PXShader(L"DTESSLEPIX");
	DTESSLEMAT->Set_Blend(L"ALPHA");	

	{
		KPtr<Shader_Vertex> VTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"EFFECTFILTERVTX", L"Shader", L"Effect.fx", "VS_EFFECTFILTER");
		VTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

		D3D11_SO_DECLARATION_ENTRY _Data[2];
		_Data[0].Stream = 0; // n번째 스트림 버퍼를 쓰는지
		_Data[0].OutputSlot = 0; // n번째 스트림 버퍼에 출력하겠다
		_Data[0].StartComponent = 0; // 어디서부터 시작할 건지
		_Data[0].ComponentCount = 4; // 어디까지 쓸건지
		_Data[0].SemanticIndex = 0; //  POSITION[0] POSITION[1] POSITION[2]
		_Data[0].SemanticName = "POSITION"; // 이름이 어떤가?


		_Data[1].Stream = 0;
		_Data[1].OutputSlot = 0;
		_Data[1].StartComponent = 0;
		_Data[1].ComponentCount = 4;
		_Data[1].SemanticIndex = 0;
		_Data[1].SemanticName = "NORMAL";

		KUINT BufSize = sizeof(Renderer_Effect::Effect_Data);
		KPtr<Shader_GeoMetry> GEO = ResourceManager<Shader_GeoMetry>::Load_FromKey(
			L"EFFECTFILTERGEO", L"Shader", L"Effect.fx", "GS_EFFECTFILTER", _Data, 2, &BufSize, 1, D3D11_SO_NO_RASTERIZED_STREAM);

		KPtr<KMaterial> MTL = ResourceManager<KMaterial>::Create(L"EFFECTFILTERMTL");
		MTL->Set_VTShader(L"EFFECTFILTERVTX");
		MTL->Set_GMShader(L"EFFECTFILTERGEO");
		MTL->Set_Blend(L"ALPHA");
	}

	{
		KPtr<Shader_Vertex> VTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"EFFECTRENDERVTX", L"Shader", L"Effect.fx", "VS_EFFECTRENDER");
		VTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

		KPtr<Shader_GeoMetry> GEO = ResourceManager<Shader_GeoMetry>::Load_FromKey(L"EFFECTRENDERGEO", L"Shader", L"Effect.fx", "GS_EFFECTRENDER");
		KPtr<Shader_Pixel> PIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"EFFECTRENDERPIX", L"Shader", L"Effect.fx", "PS_EFFECTRENDER");

		KPtr<KMaterial> MTL = ResourceManager<KMaterial>::Create(L"EFFECTRENDERMTL");
		MTL->Set_VTShader(L"EFFECTRENDERVTX");
		MTL->Set_GMShader(L"EFFECTRENDERGEO");
		MTL->Set_PXShader(L"EFFECTRENDERPIX");
		MTL->Set_Blend(L"ALPHA");
		MTL->Insert_TexData(TEX_TYPE::TEX_TARGET, 1, L"DEPTH");
	}

	{
		KPtr<Shader_Vertex> VTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"POSTGAUVTX", L"Shader", L"Effect_Gaussian.fx", "VS_GN");
		VTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
		KPtr<Shader_Pixel> PIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"POSTGAUPIX", L"Shader", L"Effect_Gaussian.fx", "PS_GN");
		PIX->CreateCB<KVector>(L"SCREENSIZE", D3D11_USAGE_DYNAMIC, 0);

		KPtr<KMaterial> MTL = ResourceManager<KMaterial>::Create(L"POSTGAU");
		MTL->Set_VTShader(L"POSTGAUVTX");
		MTL->Set_PXShader(L"POSTGAUPIX");
		MTL->Set_Blend(L"ALPHA");
	}


	{
		KPtr<Shader_Vertex> VTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DRAWVTX", L"Shader", L"Draw.fx", "VS_DRAW3D");
		VTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
		KPtr<Shader_Pixel> PIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DRAWPIX", L"Shader", L"Draw.fx", "PS_DRAW3D");

		KPtr<KMaterial> MTL = ResourceManager<KMaterial>::Create(L"DRAWMTL");
		MTL->Set_VTShader(L"DRAWVTX");
		MTL->Set_PXShader(L"DRAWPIX");
		MTL->Set_Blend(L"ALPHA");
	}


	{
		KPtr<Shader_Vertex> VTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"ANIEFFVTX", L"Shader", L"ANIEFFFECTMAT.fx", "VS_ANIEFFECT");
		VTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
		KPtr<Shader_Pixel> PIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"ANIEFFPIX", L"Shader", L"ANIEFFFECTMAT.fx", "PS_ANIEFFECT");
		PIX->CreateCB<ANIEFFCB>(L"ANIEFFCB", D3D11_USAGE_DYNAMIC, 0);

		KPtr<KMaterial> MTL = ResourceManager<KMaterial>::Create(L"ANIEFFMTL");
		MTL->Insert_TexData(TEX_TYPE::TEX_TARGET, 1, L"DEPTH");
		MTL->Set_VTShader(L"ANIEFFVTX");
		MTL->Set_PXShader(L"ANIEFFPIX");
		MTL->Set_Blend(L"ALPHA");
	}


	{
		KPtr<Shader_Vertex> VTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"POSTEXVTX", L"Shader", L"Bloom.fx", "VS_EX");
		VTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
		VTX->CreateCB<KVector>(L"SCREENSIZE", D3D11_USAGE_DYNAMIC, 0);
		VTX->CreateCB<KVector>(L"TARGETSIZE", D3D11_USAGE_DYNAMIC, 1);
		KPtr<Shader_Pixel> PIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"POSTEXPIX", L"Shader", L"Bloom.fx", "PS_EX");

		KPtr<KMaterial> MAT = ResourceManager<KMaterial>::Create(L"POSTEX");
		MAT->Set_VTShader(L"POSTEXVTX");
		MAT->Set_PXShader(L"POSTEXPIX");
		MAT->Set_Blend(L"ALPHA");
	}

	{
		KPtr<Shader_Vertex> VTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"POSTSMALLVTX", L"Shader", L"Bloom.fx", "VS_SMALL");
		VTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
		VTX->CreateCB<KVector>(L"SCREENSIZE", D3D11_USAGE_DYNAMIC, 0);
		VTX->CreateCB<KVector>(L"TARGETSIZE", D3D11_USAGE_DYNAMIC, 1);
		KPtr<Shader_Pixel> PIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"POSTSMALLPIX", L"Shader", L"Bloom.fx", "PS_SMALL");

		KPtr<KMaterial> MAT = ResourceManager<KMaterial>::Create(L"POSTSMALL");
		MAT->Set_VTShader(L"POSTSMALLVTX");
		MAT->Set_PXShader(L"POSTSMALLPIX");
		MAT->Set_Blend(L"ALPHA");
	}

	{
		KPtr<Shader_Vertex> VTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"POSTBLOOMVTX", L"Shader", L"Bloom.fx", "VS_BLOOMMERGE");
		VTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
		KPtr<Shader_Pixel> PIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"POSTBLOOMPIX", L"Shader", L"Bloom.fx", "PS_BLOOMMERGE");
		PIX->CreateCB<KVector>(L"SCREENSIZE", D3D11_USAGE_DYNAMIC, 0);

		KPtr<KMaterial> MAT = ResourceManager<KMaterial>::Create(L"POSTBLOOMMERGE");
		MAT->Set_VTShader(L"POSTBLOOMVTX");
		MAT->Set_PXShader(L"POSTBLOOMPIX");
		MAT->Set_Blend(L"ALPHA");
	}

	return true;
}

bool KDevice::Init_BasicFigure3D() 
{

	DefRenderTaget();
	Def3DCreate();
	Mesh3DCreate();
	Mat3DCreate();
	Create_DefFilter();

	return true;
}
