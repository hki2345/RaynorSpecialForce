// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "GValue.fx"
#include "PSOUT.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

cbuffer TerrainBuffer : register(b0)
{
    int FloorCount;
    int VTXX;
    int VTXY;
    int temp3;
    int IsBump[4];
}

struct VTX3DMESH_INPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT; // U와 일치하는 X축
    float4 vBNormal : BINORMAL; // V와 일치하는 탄젠트
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct VTX3DMESH_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT; // U와 일치하는 X축
    float4 vBNormal : BINORMAL; // V와 일치하는 탄젠트
};


VTX3DMESH_OUTPUT VS_TERRAINDEFFERD(VTX3DMESH_INPUT _in)
{
    VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT) 0.f;

    outData.vPos = mul(_in.vPos, g_WVP);
    outData.vUv = _in.vUv;
    outData.vColor.rgba = _in.vColor.rgba;
    outData.vViewPos = mul(_in.vPos, g_WV);
    outData.vNormal = normalize(mul(_in.vNormal, g_WV));
    outData.vTangent = normalize(mul(_in.vTangent, g_WV));
    outData.vBNormal = normalize(mul(_in.vBNormal, g_WV));

    if (0 == IsLight)
    {
        return outData;
    }

    return outData;
}

PS_DEFFERDOUTPUT PS_TERRAINDEFFERD(VTX3DMESH_OUTPUT _in)
{
    PS_DEFFERDOUTPUT outData = (PS_DEFFERDOUTPUT) 0.0f;
    outData.vDiffuse = _in.vColor;
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    CalColor *= GetMTexToColor(8, 8, _in.vUv, 0.0f);
    float4 BumpNormal = CalMBump(8, 8, _in.vUv, 1.0f, _in.vTangent, _in.vBNormal, _in.vNormal);
    
    BumpNormal = mul(BumpNormal, g_V);

    float2 SpUv;

    SpUv.x = _in.vUv.x / VTXX;
    SpUv.y = _in.vUv.y / VTXY;

    for (int i = 0; i < FloorCount; ++i)
    {
        // 색깔 섞기.
        float4 Ratio = GetTexToColor(i, i, SpUv);
        float RatioValuie = (Ratio.x + Ratio.y + Ratio.z) / 3.0f;
        float4 FloorColor = GetMTexToColor(9 + i, 9 + i, _in.vUv, 0.0f);
        float4 SrcColor = CalColor;
        FloorColor.xyz *= RatioValuie;
        SrcColor.xyz *= (1.0f - Ratio.x);
        CalColor = FloorColor + SrcColor;
    }
    
    // 포워드 색깔을 아예 사용하지 않는 것은 아니다.
    // outData.vDiffuse.rgb = float4(1.0f, 1.0f, 1.0f, 1.0f);
    outData.vDiffuse.rgb = CalColor.xyz;
    outData.vDiffuse.a = 1.0f;
    outData.vNoraml = _in.vNormal;
    outData.vNoraml.a = 1.0f;
    outData.vPosition = _in.vViewPos;
    outData.vDepth.x = 1.0f - _in.vPos.z;
    outData.vDepth.w = 1.0f;

    return outData;
}