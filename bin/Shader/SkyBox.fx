// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"

// Texture2D g_Tex_0 : register(t0);
// SamplerState g_Sam_0 : register(s0);

// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.
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
    float4 vTangent : TANGENT; // U�� ��ġ�ϴ� X��
    float4 vBNormal : BINORMAL; // V�� ��ġ�ϴ� ź��Ʈ
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
struct VTX3DMESH_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT; // U�� ��ġ�ϴ� X��
    float4 vBNormal : BINORMAL; // V�� ��ġ�ϴ� ź��Ʈ
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
    
    // ������ ������ �ƿ� ������� �ʴ� ���� �ƴϴ�.
    // ����� �븻�� ���� ��
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
    
    // ������ ������ �ƿ� ������� �ʴ� ���� �ƴϴ�.
    // ����� �븻�� ���� ��
    outData.vDiffuse.rgba = CalColor;
    
    outData.vNoraml.a = CalColor.a;
    outData.vDepth.g = Light_Opa;
    
    return outData;
}