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

// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.
struct VTXCOL_INPUT
{
	float4 vPos : POSITION;
	float4 vCol : COLOR;
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
struct VTXCOL_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vCol : COLOR;
};

struct PS_OUTPUT
{
	float4 vCol : SV_Target;
};

VTXCOL_OUTPUT VS_Color(VTXCOL_INPUT _iN)  
{
	VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vCol = _iN.vCol;

	return outData;
}

PS_OUTPUT PS_Color(VTXCOL_OUTPUT _in)
{
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
	outData.vCol = _in.vCol;
	return outData;
}