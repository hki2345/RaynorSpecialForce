// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "GValue.fx"
#include "PSOUT.fx"
#include "GTex.fx"



cbuffer Draw_Info : register(b0)
{
    float2 MPos;
    float2 PPUV;
    float BSize;
    float Click;
    float Tmp1;
    float Tmp2;
};

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


VTX3DMESH_OUTPUT VS_DRAW3D(VTX3DMESH_INPUT _in)
{
    VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT) 0.f;

    outData.vPos = mul(_in.vPos, g_WVP);
    outData.vUv = _in.vUv;
    
    return outData;
}

PS_FORWARDOUTPUT PS_DRAW3D(VTX3DMESH_OUTPUT _in)
{
    PS_FORWARDOUTPUT outData = (PS_FORWARDOUTPUT) 0.0f;      


    // float4 Tmp = mul(MPos, g_WVP);
    float4 CalColor = float4(.0f, .0f, .0f, 0.0f);
    
    if (MPos.x - PPUV.x * BSize < _in.vUv.x &&
        MPos.x + PPUV.x * BSize > _in.vUv.x && 
        MPos.y - PPUV.y * BSize < _in.vUv.y &&
        MPos.y + PPUV.y * BSize > _in.vUv.y)
    {
        // 이미지 UV의 시작
        float2 Tmp = float2(
        _in.vUv.x - (MPos.x - PPUV.x * BSize),
        _in.vUv.y - (MPos.y - PPUV.y * BSize));
        // CalColor += float4(1.0f, 1.0f, .0f, .0f);
        CalColor = GetTexToColor(ArrTex[0].Tex_Idx, ArrTex[0].Tex_Smp, Tmp * BSize * .5f);
    }
    
    //if (MPos.y - PPUV.y * BSize < _in.vUv.y &&
    //    MPos.y + PPUV.y * BSize > _in.vUv.y)
    //{
    //    CalColor += float4(1.0f, .0f, .0f, .0f);
    //}

    // float4 CalColor = GetTexToColor(ArrTex[0].Tex_Idx, ArrTex[0].Tex_Smp, _in.vUv);
    // CalColor = float4(1.0f, .0f, .0f, 1.0f);

    outData.vDiffuse.rgba = CalColor;
    return outData;
}