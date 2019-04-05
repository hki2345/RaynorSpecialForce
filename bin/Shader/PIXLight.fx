// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "Light.fx"

// Texture2D g_Tex_0 : register(t0);
// SamplerState g_Sam_0 : register(s0);

// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.
struct VTX3DMESH_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
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
};

struct PS3D_OUTPUT
{
	float4 vColor : SV_Target;
};

VTX3DMESH_OUTPUT VS_MESH3D(VTX3DMESH_INPUT _in)
{
	VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT)0.f;

	outData.vPos = mul(_in.vPos, g_WVP);
	outData.vUv = _in.vUv;
	outData.vColor.rgba = _in.vColor.rgba;
	outData.vViewPos = mul(mul(_in.vPos, g_W), g_V);
	_in.vNormal.w = 0.0f;
	outData.vNormal = normalize(mul(mul(_in.vNormal, g_W), g_V));


	return outData;
}

PS3D_OUTPUT PS_MESH3D(VTX3DMESH_OUTPUT _in)
{
	PS3D_OUTPUT outData = (PS3D_OUTPUT)0.0f;

	//LIGHTDATA L;
	//L.Range = 500.f;
	//L.Lc.Diff = float4(1.0f, 1.0f, 1.0f, 0.0f);
	//L.Lc.Spec = float4(0.2f, 0.2f, 0.2f, 0.0f);
	//L.Lc.Ambi = float4(0.1f, 0.1f, 0.1f, 0.0f);
	//// ������� ���� �־��ٰ� �����ϰ�
	//L.LightDir = float4(1.0f, 0.0f, 0.0f, 0.0f);
	//L.LightDir = normalize(L.LightDir);
	//L.LightPos = float4(200.0f, 200.0f, 500.0f, 0.0f);
	//L.LightPos = -mul(L.LightPos, g_V);

	LIGHTCOLOR LColor = CalDirLight(_in.vViewPos, _in.vNormal, ArrLight[0]);
	// CALL.Lc.Diff.rgb
	outData.vColor.rgb = _in.vColor.rgb * LColor.Diff.rgb + LColor.Spec.rgb + LColor.Ambi;
	outData.vColor.a = _in.vColor.a;
	return outData;
}