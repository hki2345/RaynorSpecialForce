// 쉐이더는 hlsl이라는 그래픽카드를 통해서
// 컴파일되는 언어로서 그래픽카드에게 이렇게 처리해달라고 명령을 내리는 것이다.
//cbuffer TransformBuffer : register(b0)
//{
//	matrix g_W;
//	matrix g_V;
//	matrix g_P;
//}

cbuffer TransformBuffer : register(b0)
{
	matrix g_WVP;
}

cbuffer TransformBuffer : register(b1)
{
	float2 g_UvTexelSize;
	float g_Border;
}

// 버텍스 버퍼에 대한 정보를 정의하는 
// POSITION을 시맨틱이라고 하는데.
// 버텍스 쉐이더에서 이용할 것.
struct VTXCOL_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct VTXCOL_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
};

struct PS_OUTPUT
{
	float4 vCol : SV_Target;
};

VTXCOL_OUTPUT VS_BDebugRect(VTXCOL_INPUT _iN)  
{
	VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;

	return outData;
}

PS_OUTPUT PS_BDebugRect(VTXCOL_OUTPUT _in)
{
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
/*
	if (_in.vUv.x > 0.9f)
	{
		outData.vCol.xyzw = 1.0f;
		return outData;
	}
	if (_in.vUv.x < 0.1f)
	{
		outData.vCol.xyzw = 1.0f;
		return outData;
	}
	if (_in.vUv.y > 0.9f)
	{
		outData.vCol.xyzw = 1.0f;
		return outData;
	}
	if (_in.vUv.y < 0.1f)
	{
		outData.vCol.xyzw = 1.0f;
		return outData;
	}*/

	return outData;
}