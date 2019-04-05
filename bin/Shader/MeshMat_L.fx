// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "GValue.fx"
#include "PSOUT.fx"
#include "Light.fx"
#include "GTex.fx"

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


VTX3DMESH_OUTPUT VS_MESH3D(VTX3DMESH_INPUT _in)
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

    if (1 == LightVtxToPix)
    {
        return outData;
    }

    if (0 == LightCount)
    {
        return outData;
    }

    LIGHTCOLOR CalLColor;
    CalLColor.Ambi = (float4) 0.0f;
    CalLColor.Diff = (float4) 0.0f;
    CalLColor.Spec = (float4) 0.0f;

    for (int i = 0; i < LightCount; ++i)
    {
        LIGHTCOLOR LColor = CalDirLight(outData.vViewPos, outData.vNormal, ArrLight[0]);
        CalLColor.Diff += LColor.Diff;
        CalLColor.Spec += LColor.Spec;
        CalLColor.Ambi += LColor.Ambi;
    }

    CalLColor.Diff /= (float) LightCount;
    CalLColor.Spec /= (float) LightCount;
    CalLColor.Ambi /= (float) LightCount;

    outData.vColor.rgb = _in.vColor.rgb * CalLColor.Diff.rgb + CalLColor.Spec.rgb + CalLColor.Ambi.rgb;
    outData.vColor.a = _in.vColor.a;

    return outData;
}

PS_DEFFERDOUTPUT PS_MESH3D(VTX3DMESH_OUTPUT _in)
{
    PS_DEFFERDOUTPUT outData = (PS_DEFFERDOUTPUT) 0.0f;
    outData.vDiffuse = _in.vColor;

    // outData.vColor *= GetTexToColor(ArrTex[0].Tex_Idx, ArrTex[0].Tex_Smp, _in.vUv);
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    // float4 CalNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);

    // CalColor = g_Tex_0.Sample(g_Sam_0, _in.vUv);

    for (int i = 0; i < TexCount; ++i)
    {
        if (-1 != ArrTex[i].Tex_Idx)
        {
            if (ArrTex[i].Type == TEX)
            {
                CalColor *= GetTexToColor(ArrTex[i].Tex_Idx, ArrTex[i].Tex_Smp, _in.vUv) * _in.vColor;
            }
            else if (ArrTex[i].Type == BUMP)
            {
                _in.vNormal = CalBump(ArrTex[i].Tex_Idx, ArrTex[i].Tex_Smp, _in.vUv, _in.vTangent, _in.vBNormal, _in.vNormal);
            }
        }
    }

    // 계산.
    if (0 == IsLight)
    {
        return outData;
    }

    if (0 == LightVtxToPix)
    {
        return outData;
    }

    if (0 == LightCount)
    {
        return outData;
    }

    // 포워드 색깔을 아예 사용하지 않는 것은 아니다.

    // 여기까지의 연산은 포워드 인것.
    LIGHTCOLOR CalLColor;
    CalLColor.Ambi = (float4) 0.0f;
    CalLColor.Diff = (float4) 0.0f;
    CalLColor.Spec = (float4) 0.0f;

    for (int i = 0; i < LightCount; ++i)
    {
        LIGHTCOLOR LColor = CalDirLight(_in.vViewPos, _in.vNormal, ArrLight[i]);
        CalLColor.Diff += LColor.Diff;
        CalLColor.Spec += LColor.Spec;
        CalLColor.Ambi += LColor.Ambi;
    }

    outData.vDiffuse.rgb = CalColor.rgb * CalLColor.Diff.rgb + CalLColor.Spec.rgb + CalLColor.Ambi.rgb;
    outData.vDiffuse.a = _in.vColor.a;

	// outData.vColor = _in.vColor;
    return outData;
}