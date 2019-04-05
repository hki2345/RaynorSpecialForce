// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"
#include "Light.fx"
#include "GTex.fx"


// Texture2D g_Tex_0 : register(t0);
// SamplerState g_Sam_0 : register(s0);

// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.
struct VTX3DMESH_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
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