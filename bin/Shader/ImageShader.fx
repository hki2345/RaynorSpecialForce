// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "GValue.fx"

Texture2D g_Tex_0 : register(t0);
SamplerState g_Sam_0 : register(s0);

// 버텍스 버퍼에 대한 정보를 정의하는 
// POSITION을 시맨틱이라고 하는데.
// 버텍스 쉐이더에서 이용할 것.
struct VTXTEX_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct VTXTEX_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
};

struct PS_OUTPUT
{
	float4 vCol : SV_Target;
};


VTXTEX_OUTPUT VS_Img(VTXTEX_INPUT _iN)
{
	VTXTEX_OUTPUT outData = (VTXTEX_OUTPUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;

	return outData;
}

cbuffer MulColor : register(b0)
{
	float4 CBColor;
}

cbuffer ImageUv : register(b1)
{
	float2 vStart = float2(0.0f, 0.0f);
	float2 vSize = float2(1.0f, 1.0f);
};

// StartX StartY; 0.5 0.5
// SizeX SizeY; 0.5 0.5
// EndX EndY; 1.0f 1.0f

// 네개가 아니고
// 어떤 이미지를 2 2 0,0, 0,1 1,0 1,1
// 1,1

// vStart.x + (vSize.x * _in.vUv.x)
// 0.5, 0.5 // StartX StartY;
// 1.0f, 0.5
// 0.5f, 1.0
// 1.0f, 1.0f;

// 0							,		0
// 1							,		0
// 0							,		1
// 1							,		1

// 꼭 상수버퍼
// 텍스처의 크기 x y

PS_OUTPUT PS_Img(VTXTEX_OUTPUT _in)
{
	//float4 vLeftColor = g_Tex_0.Sample(g_Sam_0,
	//	float2(vStart.x + (vSize.x * _in.vUv.x) - 1 / 이미지 사이즈.x
	//		, vStart.y + (vSize.y * _in.vUv.y)));
	//if (true)
	//{

	//}

	float4 vCol = g_Tex_0.Sample(g_Sam_0, 
		float2(vStart.x + (vSize.x * _in.vUv.x)
			, vStart.y + (vSize.y * _in.vUv.y)));

	//float4 vCol = g_Tex_0.Sample(g_Sam_0, 
	//	float2(_in.vUv.x, _in.vUv.y) );
	// vCol.rgb = (vCol.r + vCol.g + vCol.b) / 3.0f;
	if (0 >= vCol.a)
	{
		clip(-1);
	}

	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
	outData.vCol = vCol * CBColor;

	// outData.vCol = vCol;

	return outData;
}