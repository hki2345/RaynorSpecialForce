#include "GValue.fx"

struct NONE_VTXOUTPUT
{
	float4 vPos : SV_POSITION;
};

NONE_VTXOUTPUT VTXNONE3D(float4 _Pos : POSITION)
{
	NONE_VTXOUTPUT None = (NONE_VTXOUTPUT)0.0f;
	None.vPos = mul(_Pos, g_WVP);
	return None;
}

struct NONE_PIXOUTPUT
{
	float4 vColor : SV_Target;
};

NONE_PIXOUTPUT PIXNONE3D(float4 _in : SV_POSITION)
{
	NONE_PIXOUTPUT None = (NONE_PIXOUTPUT)0.0f;

	None.vColor = float4(1.0f, 0.0f, 1.0f, 1.0f);
	return None;
}