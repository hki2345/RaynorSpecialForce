// ������۴� b
// �ؽ�ó�� t
// ���÷��� s

cbuffer TransformBuffer : register(b0)
{
	matrix g_WVP;
}

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

VTXTEX_OUTPUT VS_Tex(VTXTEX_INPUT _iN)
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

PS_OUTPUT PS_Tex(VTXTEX_OUTPUT _in)
{
	float4 vCol = g_Tex_0.Sample(g_Sam_0, _in.vUv);
	// vCol.rgb = (vCol.r + vCol.g + vCol.b) / 3.0f;
	if (0 >= vCol.a)
	{
		clip(-1);
	}

	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
	// outData.vCol = vCol;
	outData.vCol = vCol * CBColor;

	//if (0 >= outData.vCol.a)
	//{
	//	clip(-1);
	//}

	return outData;
}