// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"

Texture2D g_Tex_0 : register(t0);
SamplerState g_Sam_0 : register(s0);

// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.
struct VTXTEX_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
struct VTXTEX_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
};

struct PS_OUTPUT
{
	float4 vCol : SV_Target;
};


VTXTEX_OUTPUT VS_Img(VTXTEX_INPUT _iN)
{
	VTXTEX_OUTPUT outData = (VTXTEX_OUTPUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;

	return outData;
}

cbuffer MulColor : register(b0)
{
	float4 CBColor;
}

cbuffer ImageUv : register(b1)
{
	float2 vStart = float2(0.0f, 0.0f);
	float2 vSize = float2(1.0f, 1.0f);
};

// StartX StartY; 0.5 0.5
// SizeX SizeY; 0.5 0.5
// EndX EndY; 1.0f 1.0f

// �װ��� �ƴϰ�
// � �̹����� 2 2 0,0, 0,1 1,0 1,1
// 1,1

// vStart.x + (vSize.x * _in.vUv.x)
// 0.5, 0.5 // StartX StartY;
// 1.0f, 0.5
// 0.5f, 1.0
// 1.0f, 1.0f;

// 0							,		0
// 1							,		0
// 0							,		1
// 1							,		1

// �� �������
// �ؽ�ó�� ũ�� x y

PS_OUTPUT PS_Img(VTXTEX_OUTPUT _in)
{
	//float4 vLeftColor = g_Tex_0.Sample(g_Sam_0,
	//	float2(vStart.x + (vSize.x * _in.vUv.x) - 1 / �̹��� ������.x
	//		, vStart.y + (vSize.y * _in.vUv.y)));
	//if (true)
	//{

	//}

	float4 vCol = g_Tex_0.Sample(g_Sam_0, 
		float2(vStart.x + (vSize.x * _in.vUv.x)
			, vStart.y + (vSize.y * _in.vUv.y)));

	//float4 vCol = g_Tex_0.Sample(g_Sam_0, 
	//	float2(_in.vUv.x, _in.vUv.y) );
	// vCol.rgb = (vCol.r + vCol.g + vCol.b) / 3.0f;
	if (0 >= vCol.a)
	{
		clip(-1);
	}

	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
	outData.vCol = vCol * CBColor;

	// outData.vCol = vCol;

	return outData;
}