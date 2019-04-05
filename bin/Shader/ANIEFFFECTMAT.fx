// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "GValue.fx"
#include "PSOUT.fx"
#include "ForwardLight.fx"
#include "GTex.fx"

struct VTX3DMESH_INPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct VTX3DMESH_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
};


VTX3DMESH_OUTPUT VS_ANIEFFECT(VTX3DMESH_INPUT _in)
{
    VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT) 0.f;

    outData.vPos = mul(_in.vPos, g_WVP);
    outData.vUv = _in.vUv;

    return outData;
}


cbuffer ANIEFFECTCB : register(b0)
{
    float4 vColor;
    float2 vStart = float2(0.0f, 0.0f);
    float2 vSize = float2(1.0f, 1.0f);
}



PS_FORWARDOUTPUT PS_ANIEFFECT(VTX3DMESH_OUTPUT _in)
{
    PS_FORWARDOUTPUT outData = (PS_FORWARDOUTPUT) 0.0f;
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    float4 vCol = g_Tex_0.Sample(g_Sam_0,
		float2(vStart.x + (vSize.x * _in.vUv.x)
			, vStart.y + (vSize.y * _in.vUv.y)));

    // float4 vCol = g_Tex_0.Sample(g_Sam_0, _in.vUv);

    if (0 >= vCol.a)
    {
        clip(-1);
    }
    
    float4 ZPos = g_Tex_1.Load(int3((int) _in.vPos.x, (int) _in.vPos.y, .0f));
    
    if (.0f == vColor.a)
    {
        if (ZPos.x == .0f)
        {
            vCol.a *= 1.0f;
        }
        else if (_in.vPos.z >= ZPos.x)
        {
            vCol.a *= 1.0f;
        }
        else
        {
            vCol.a *= (ZPos.x - _in.vPos.z) * .057f;
        }
    }

    float4 TCor = vColor;
    TCor.a = 1.0f;
    outData.vDiffuse = vCol * TCor;


    return outData;
}