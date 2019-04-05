#include "GValue.fx"
#include "PSOUT.fx"
#include "GTex.fx"
#include "DefferdLight.fx"


static matrix DirMat =
{
    { 2.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

//static float g_vGaussian[9] =
//{
//    1.0f, 2.0f, 1.0f, = 4
//    2.0f, 4.0f, 2.0f, = 8
//    1.0f, 2.0f, 1.0f = 4
//};

// ����þ� ���� 
static float g_vGaussian[25] =
{
    1.0f, 2.0f, 4.0f, 2.0f, 1.0f, // 10
    2.0f, 4.0f, 8.0f, 4.0f, 2.0f, // 20
    4.0f, 8.0f, 16.0f, 8.0f, 4.0f, // 40
    2.0f, 4.0f, 8.0f, 4.0f, 2.0f, // 20
    1.0f, 2.0f, 4.0f, 2.0f, 1.0f // 10
};
// ���� ���� �� ��ģ ���� ������ �ȴ�.
cbuffer POSTDATA : register(b0)
{
    float4 g_ScreenSize;
}


VS_TEXOUTPUT VS_GN(VS_TEXINPUT _In)
{
    VS_TEXOUTPUT outData = (VS_TEXOUTPUT) 0.f;
    outData.vPos = mul(_In.vPos, DirMat);
    outData.vUv = _In.vUv;
    return outData;
}

PS_OUTPUT PS_GN(VS_TEXOUTPUT _in)
{
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;
    // ��� ���۸� �����. 

    // ȭ��ũ�� ������ 1�ϸ� 
    float PixelU = 1 / g_ScreenSize.x;
    float PixelV = 1 / g_ScreenSize.y;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            float2 vUV = _in.vUv + float2((i - 1) * PixelU, (j - 1) * PixelV);
            outData.vCol += g_Tex_0.Sample(g_Sam_0, vUV) * g_vGaussian[i * 5 + j];
        }
    }
    // ���⼭ ��Ʈ���� ������ ���ϸ� ��
    outData.vCol /= 100.0f;

    return outData;
}