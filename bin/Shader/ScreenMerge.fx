// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"
#include "PSOUT.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

struct VS_SCREENMERGEINPUT
{
    float4 vPos : POSITION; // ����.
    float2 vUv : TEXCOORD; // ��ġ
};

struct VS_SCREENMERGEOUTPUT
{
    float4 vPos : SV_POSITION; // ����.
    float2 vUv : TEXCOORD; // ��ġ
};

static matrix DirMat =
{
    { 2.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

VS_SCREENMERGEOUTPUT VS_SCREENMERGE(VS_SCREENMERGEINPUT _Input)
{
    VS_SCREENMERGEOUTPUT OUTDATA = (VS_SCREENMERGEOUTPUT) 0.0F;
    OUTDATA.vPos = mul(_Input.vPos, DirMat);
    OUTDATA.vUv = _Input.vUv;
    return OUTDATA;
}

PS_MERGEOUTPUT PS_SCREENMERGE(VS_DEFFERDLIGHTOUTPUT _Input)
{
    PS_MERGEOUTPUT OUTDATA = (PS_MERGEOUTPUT) 0.0F;
    OUTDATA.vMergeColor = g_Tex_0.Sample(g_Sam_0, _Input.vUv);
    return OUTDATA;
}