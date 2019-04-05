#include "GValue.fx"
#include "DefferdLight.fx"
#include "GTex.fx"



/************************** ���ؽ� ******************************/
struct EFFECT_INPUT
{
    float4 vPos : POSITION;
    float4 vValue : NORMAL;
};


// ���ؽ����� �̰��� ������� ���� �Ǻ����� - ���ؽ��� �ϴ� �� ����
EFFECT_INPUT VS_EFFECTFILTER(EFFECT_INPUT _Data)
{
    return _Data;
};


/************************** ������Ʈ�� ******************************/
[maxvertexcount(2)]
void GS_EFFECTFILTER(point EFFECT_INPUT _Data[1], inout PointStream<EFFECT_INPUT> _EStream)
{
   // ���ڿ��ٰ� �ٿ��� �ִ´�.
    // �� �̹��� ��� ���ڰ� �������� �����Ѵ�.
    if (_Data[0].vPos.x > 0.0f)
    {
        EFFECT_INPUT NEF = (EFFECT_INPUT) 0.0f;
        NEF.vPos = mul(_Data[0].vPos, g_W);
        NEF.vValue = _Data[0].vValue;
        _EStream.Append(NEF);
    }

    // ��Ʈ���� �ѱ�� �װ� �ٽ� ���̴� ������ ���͸��� ���ؽ�����
    // �׸������� �ٽ� �ִ´�.
}

/************************** �� ���ε� - ���� �׸��� ���� ******************************/
struct EFFECT_VSOUT
{
    float4 Pos : POSITION;
    float4 Value : NORMAL;
};

struct EFFECT_GSOUT
{
    float4 Pos : SV_POSITION;
    float4 ViewPos : POSITION;
    float4 Value : NORMAL;
    float2 UV : TEXCOORD;
};

static float2 UVMX[4] =
{
    float2(.0f, 1.0f),
    float2(1.0f, 1.0f),
    float2(.0f, .0f),
    float2(1.0f, .0f),
};


EFFECT_VSOUT VS_EFFECTRENDER(EFFECT_INPUT _Data)
{
    EFFECT_VSOUT Tmp = (EFFECT_VSOUT) .0f;
    Tmp.Pos = _Data.vPos;
    Tmp.Value = _Data.vValue;
    return Tmp;
}


/************************** ������Ʈ�� ******************************/
// ���⼭ 4��� ���ڴ� �簢�����׸��ڴٴ� ��
[maxvertexcount(4)]
void GS_EFFECTRENDER(point EFFECT_INPUT _Data[1], inout TriangleStream<EFFECT_GSOUT> _TStream)
{
    float3 Up = float3(.0f, 1.0f, .0f);
    float3 Right = float3(1.0f, .0f, .0f);

    float4 vTri[4];
    vTri[0] = float4(_Data[0].vPos.xyz + Right - Up, 1.0f);
    vTri[1] = float4(_Data[0].vPos.xyz + Right + Up, 1.0f);
    vTri[2] = float4(_Data[0].vPos.xyz - Right - Up, 1.0f);
    vTri[3] = float4(_Data[0].vPos.xyz - Right + Up, 1.0f);
                                                     

    EFFECT_GSOUT NVtx = (EFFECT_GSOUT) .0f;


    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        NVtx.Pos = mul(vTri[i], g_WVP);
        NVtx.ViewPos = mul(vTri[i], g_WV);
        NVtx.Value.w = _Data[0].vValue.w;
        NVtx.UV = UVMX[i];
        _TStream.Append(NVtx);
    }

    return;
}


/************************** �ȼ� ******************************/
// ������Ʈ������ ���� �� �ȼ��� �׸���.
float4 PS_EFFECTRENDER(EFFECT_GSOUT _Data) : SV_Target
{
    float4 Tmp = g_Tex_0.Sample(g_Sam_0, _Data.UV);
    float4 ZPos = g_Tex_1.Load(int3((int) _Data.Pos.x, (int) _Data.Pos.y, .0f));

    float X = ZPos.x - _Data.Pos.z;

    if(ZPos.x == .0f)
    {
        Tmp.a = 1.0f * _Data.Value.a * Tmp.a;
    }
    else if(_Data.Pos.z < ZPos.x)
    {
        Tmp.a = 1.0f * _Data.Value.a * Tmp.a;
    }
    else
    {
        Tmp.a = .0f * _Data.Value.a * Tmp.a;
    }
    
    //float X = ZPos.x - _Data.Pos.z;
    
    //Tmp.a *= X * _Data.Value.a;

    return Tmp;
}
