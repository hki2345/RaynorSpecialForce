// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "GValue.fx"
#include "Light.fx"
#include "GTex.fx"


// Texture2D g_Tex_0 : register(t0);
// SamplerState g_Sam_0 : register(s0);

// 버텍스 버퍼에 대한 정보를 정의하는 
// POSITION을 시맨틱이라고 하는데.
// 버텍스 쉐이더에서 이용할 것.
struct VTX3DMESH_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct VTX3DMESH_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

struct PS3D_OUTPUT
{
	float4 vColor : SV_Target;
};


cbuffer CUT_FADE : register(b5)
{
    // X - Fade Y - Value
    float4 m_CutFade;
};


VTX3DMESH_OUTPUT VS_RECT3D(VTX3DMESH_INPUT _iN)
{
	VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;
	outData.vColor = _iN.vColor;
	outData.vNormal = _iN.vNormal;

	return outData;
}

PS3D_OUTPUT PS_RECT3D(VTX3DMESH_OUTPUT _in)
{
    PS3D_OUTPUT outData = (PS3D_OUTPUT) 0.0f;
    
    float4 CalColor = g_Tex_0.Sample(g_Sam_0, _in.vUv);


    if(0.0f == m_CutFade.x)
    {
        if (m_CutFade.y < _in.vUv.x)
        {
            CalColor *= float4(.0f, .0f, .0f, .0f);
        }
    }
    else if (1.0f == m_CutFade.x)
    {
        if (1.0f == m_CutFade.y)
        {
        }
        else if (m_CutFade.y > _in.vUv.x)
        {
            CalColor *= float4(.0f, .6f, .6f, 1.0f);
        }
        else
        {
            CalColor *= float4(.1f, .1f, .1f, 1.0f);
        }
    }
    outData.vColor = CalColor;
    outData.vColor.a = m_CutFade.z * CalColor.a;

	return outData;
}