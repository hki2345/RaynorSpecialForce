// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "GValue.fx"

// Texture2D g_Tex_0 : register(t0);
// SamplerState g_Sam_0 : register(s0);

// 버텍스 버퍼에 대한 정보를 정의하는 
// POSITION을 시맨틱이라고 하는데.
// 버텍스 쉐이더에서 이용할 것.
#include "GTex.fx"
#include "PSOUT.fx"


cbuffer MulColor : register(b0)
{
	float4 CBColor;
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


struct PS3D_OUTPUT
{
	float4 vColor : SV_Target;
};

VTX3DMESH_OUTPUT VS_SKY3D(VTX3DMESH_INPUT _iN)
{
	VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;
	outData.vColor = _iN.vColor;
    outData.vNormal = _iN.vNormal;
    outData.vViewPos = mul(_iN.vPos, g_WV);
    outData.vNormal = normalize(mul(_iN.vNormal, g_WV));
    outData.vTangent = normalize(mul(_iN.vTangent, g_WV));
    outData.vBNormal = normalize(mul(_iN.vBNormal, g_WV));
    

	return outData;
}


PS_FORWARDOUTPUT PS_SKY3DFORWARD(VTX3DMESH_OUTPUT _in)
{
//	PS3D_OUTPUT outData = (PS3D_OUTPUT)0.0f;
//	outData.vColor = g_Tex_0.Sample(g_Sam_0, float2(_in.vUv.x, _in.vUv.y));
//    return outData;

    
    PS_FORWARDOUTPUT outData = (PS_FORWARDOUTPUT) 0.0f;
    
    outData.vDiffuse = _in.vColor;
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    


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
    
    // 포워드 색깔을 아예 사용하지 않는 것은 아니다.
    // 빛계산 노말로 들어가는 중
    outData.vDiffuse.rgba = CalColor;
    return outData;
}


PS_DEFFERDOUTPUT PS_SKY3DDEFFERD(VTX3DMESH_OUTPUT _in)
{
//	PS3D_OUTPUT outData = (PS3D_OUTPUT)0.0f;
//	outData.vColor = g_Tex_0.Sample(g_Sam_0, float2(_in.vUv.x, _in.vUv.y));
//    return outData;

    
    PS_DEFFERDOUTPUT outData = (PS_DEFFERDOUTPUT) 0.0f;
    
    outData.vDiffuse = _in.vColor;
    outData.vNoraml = _in.vNormal;
    outData.vNoraml.a = 1.0f;
    outData.vPosition = _in.vViewPos;
    outData.vDepth.x = outData.vPosition.z;
    outData.vDepth.w = 1.0f;

    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    


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
    
    // 포워드 색깔을 아예 사용하지 않는 것은 아니다.
    // 빛계산 노말로 들어가는 중
    outData.vDiffuse.rgba = CalColor;
    
    outData.vNoraml.a = CalColor.a;
    outData.vDepth.g = Light_Opa;
    
    return outData;
}