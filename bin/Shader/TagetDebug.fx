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

VTXTEX_OUTPUT VS_TagetTex(VTXTEX_INPUT _iN)
{
	VTXTEX_OUTPUT outData = (VTXTEX_OUTPUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;

	return outData;
}

PS_OUTPUT PS_TagetTex(VTXTEX_OUTPUT _in)
{
	float4 vCol = g_Tex_0.Sample(g_Sam_0, _in.vUv);

	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
	outData.vCol = vCol;

	return outData;
}