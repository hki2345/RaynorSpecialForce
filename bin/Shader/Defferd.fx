// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"
#include "PSOUT.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

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


VTX3DMESH_OUTPUT VS_DEFFERD(VTX3DMESH_INPUT _in)
{
    VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT) 0.f;

    outData.vPos = mul(_in.vPos, g_WVP);
    outData.vUv = _in.vUv;
    outData.vColor.rgba = _in.vColor.rgba;
    outData.vViewPos = mul(_in.vPos, g_WV);
    outData.vNormal = normalize(mul(_in.vNormal, g_WV));
    outData.vTangent = normalize(mul(_in.vTangent, g_WV));
    outData.vBNormal = normalize(mul(_in.vBNormal, g_WV));
    
    return outData;
}


PS_DEFFERDOUTPUT PS_DEFFERD(VTX3DMESH_OUTPUT _in)
{
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
                _in.vNormal = CalSBump(ArrTex[i].Tex_Idx, ArrTex[i].Tex_Smp, _in.vUv, _in.vTangent, _in.vBNormal, _in.vNormal);
            }
            else if (ArrTex[i].Type == SPEC)
            {
                float4 Spec = GetTexToColor(ArrTex[i].Tex_Idx, ArrTex[i].Tex_Smp, _in.vUv) * _in.vColor;
                CalColor *= saturate(Spec * 1.6f + CalColor);
            }
            else if (ArrTex[i].Type == EMIS)
            {
                CalColor += GetTexToColor(ArrTex[i].Tex_Idx, ArrTex[i].Tex_Smp, _in.vUv) * _in.vColor;
            }
        }
    }
    


    // ������ ������ �ƿ� ������� �ʴ� ���� �ƴϴ�.
    // ����� �븻�� ���� ��
    outData.vDiffuse.rgb = CalColor.rgb;
    outData.vDiffuse.a = _in.vColor.a;
    outData.vNoraml = _in.vNormal;
    outData.vNoraml.a = 1.0f;
    outData.vPosition = _in.vViewPos;
    outData.vDepth.x = outData.vPosition.z;
    outData.vDepth.w = 1.0f;
    
    return outData;
}

static matrix DirMat =
{
    { 2.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

VS_DEFFERDLIGHTOUTPUT VS_DEFFERDLIGHT(VS_DEFFERDLIGHTINPUT _Input)
{
    VS_DEFFERDLIGHTOUTPUT OUTDATA = (VS_DEFFERDLIGHTOUTPUT) 0.0F;
    OUTDATA.vPos = mul(_Input.vPos, DirMat);
    OUTDATA.vUv = _Input.vUv;
    return OUTDATA;
}

// ���õ� �ؽ�ó��
// 0. ������
// 1. �븻
// 2. ����

PS_DEFFERDLIGHTOUTPUT PS_DEFFERDLIGHT(VS_DEFFERDLIGHTOUTPUT _Input)
{
    PS_DEFFERDLIGHTOUTPUT OUTDATA = (PS_DEFFERDLIGHTOUTPUT) 0.0F;

    // ���� ���� 
    float4 TT = g_Tex_2.Sample(g_Sam_0, _Input.vUv);
    float fDepth = TT.x;
    float fLO = TT.g;

    // SKY depth    - �� ������ ���� �ʴ���.
    

    if (fDepth <= 0.01f)
    {
        clip(-1);
    }

    float4 vViewPos = g_Tex_0.Sample(g_Sam_0, _Input.vUv);
    float4 vNormal = g_Tex_1.Sample(g_Sam_0, _Input.vUv);


    LIGHTCOLOR info = CalDirLight(vViewPos, vNormal, LightData);
    
    // ���� �ι�°�� �ִ� - �ϴ��̴�.
    if (0 < fLO)
    {
        OUTDATA.vDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f) * fLO;
        OUTDATA.vSpaculer = float4(.0f, .0f, .0f, 1.0f);
        return OUTDATA;
    }


    // ���� �����ų� ���ų��� ���� - ���� �༺ �ణ �Ӱ� ����
    //OUTDATA.vDiffuse.rgb = info.Diff.rgb + float3(.7f, .7f, .7f); // * float3(.7f, .2f, .2f);
    //OUTDATA.vDiffuse.a = 1.0f;
    //OUTDATA.vSpaculer.rgb = info.Spec.rgb;
    //OUTDATA.vSpaculer.a = 1.0f;
    
    OUTDATA.vDiffuse.rgb = info.Diff.rgb * LightData.LightColor.rgb + float3(.5f, .5f, .5f);
    OUTDATA.vDiffuse.a = 1.0f;
    OUTDATA.vSpaculer.rgb = info.Spec.rgb * LightData.SpecColor.rgb;
    OUTDATA.vSpaculer.a = 1.0f;
    
    return OUTDATA;
}

// ���� ���� ���̴�
VS_DEFFERDLIGHTOUTPUT VS_DEFFERDMERGE(VS_DEFFERDLIGHTINPUT _Input)
{
    VS_DEFFERDLIGHTOUTPUT OUTDATA = (VS_DEFFERDLIGHTOUTPUT) 0.0F;
    OUTDATA.vPos = mul(_Input.vPos, DirMat);
    OUTDATA.vUv = _Input.vUv;
    return OUTDATA;
}

// ���õ� �ؽ�ó��
// 0. ��ǻ�� �÷�
// 1. ��ǻ�� ����Ʈ �÷�
// 2. ����ŧ�� ����Ʈ �÷�
// �ƿ�ǲ�� �׳� Ÿ�ٿ� ���
PS_MERGEOUTPUT PS_DEFFERDMERGE(VS_DEFFERDLIGHTOUTPUT _Input)
{
    PS_MERGEOUTPUT OUTDATA = (PS_MERGEOUTPUT) 0.0F;
    float4 vColor = g_Tex_0.Sample(g_Sam_0, _Input.vUv);
    float4 vDiff = g_Tex_1.Sample(g_Sam_0, _Input.vUv);
    float4 vSpec = g_Tex_2.Sample(g_Sam_0, _Input.vUv);

    vColor.w = 1.0f;
    vDiff.w = 1.0f;
    vSpec.w = 0.0f;

    if (OUTDATA.vMergeColor.a != 0.f)
    {
        OUTDATA.vMergeColor.rgb = float3(1.f, 1.f, 1.f) * vColor.a;
    }

    OUTDATA.vMergeColor.rgb += vColor.rgb * vDiff.rgb + vSpec.rgb /*+ float3(0.1f, 0.1f, 0.1f)*/;
    OUTDATA.vMergeColor.a = 1.0f;

    return OUTDATA;
}