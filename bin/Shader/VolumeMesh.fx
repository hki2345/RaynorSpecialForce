// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

struct VS_VOLUMEOUTPUT
{
    float4 vPos : SV_POSITION; // ����.
};

VS_VOLUMEOUTPUT VS_VOLUME(float4 _Pos : POSITION)
{
    VS_VOLUMEOUTPUT OUTDATA = (VS_VOLUMEOUTPUT) 0.0F;
    OUTDATA.vPos = mul(_Pos, g_WVP);
    return OUTDATA;
}

struct PS_VOLUMEOUTPUT
{
    float4 vColor : SV_Target; // ����.
};

PS_VOLUMEOUTPUT PS_VOLUME(VS_VOLUMEOUTPUT _Input)
{
    PS_VOLUMEOUTPUT OUTDATA = (PS_VOLUMEOUTPUT) 0.0F;
    return OUTDATA;
}