// ���̴��� hlsl�̶�� �׷���ī�带 ���ؼ�
// �����ϵǴ� ���μ� �׷���ī�忡�� �̷��� ó���ش޶�� ����� ������ ���̴�.
//cbuffer TransformBuffer : register(b0)
//{
//	matrix g_W;
//	matrix g_V;
//	matrix g_P;
//}

cbuffer TransformBuffer : register(b0)
{
	matrix g_WVP;
}

cbuffer TransformBuffer : register(b1)
{
	float2 g_UvTexelSize;
	float g_Border;
}

// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.
struct VTXCOL_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
struct VTXCOL_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
};

struct PS_OUTPUT
{
	float4 vCol : SV_Target;
};

VTXCOL_OUTPUT VS_BDebugRect(VTXCOL_INPUT _iN)  
{
	VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;

	return outData;
}

PS_OUTPUT PS_BDebugRect(VTXCOL_OUTPUT _in)
{
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
/*
	if (_in.vUv.x > 0.9f)
	{
		outData.vCol.xyzw = 1.0f;
		return outData;
	}
	if (_in.vUv.x < 0.1f)
	{
		outData.vCol.xyzw = 1.0f;
		return outData;
	}
	if (_in.vUv.y > 0.9f)
	{
		outData.vCol.xyzw = 1.0f;
		return outData;
	}
	if (_in.vUv.y < 0.1f)
	{
		outData.vCol.xyzw = 1.0f;
		return outData;
	}*/

	return outData;
}