// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
class LIGHTCOLOR
{
	float4 Diff; // 확산광
	float4 Spec; // 반사광
	float4 Ambi; // 환경광
};

class LIGHTDATA
{
	LIGHTCOLOR Lc;
    float4 LightColor;
    float4 SpecColor;
	float4 LightPos;
    float4 LightDir;
    float4 LightDirInv;
	float Range;
	float Angle;
	int Type;
    int Dump;
};

LIGHTCOLOR CalDirLight(float4 _vViewPos, float4 _vViewNormal, LIGHTDATA _LightInfo)
{
    LIGHTCOLOR CALCOLOR;
    float4 vLight = (float4) 0.0f;
    float fFactor = 1.0f;

    // 디렉션
    if (_LightInfo.Type == 0)
    {
        vLight = _LightInfo.LightDirInv;
    }

    // 포인트
    else if (_LightInfo.Type == 1)
    {
        vLight = normalize(_LightInfo.LightPos - _vViewPos);
        float Dis = distance(_LightInfo.LightPos, _vViewPos);
        // 가장 멀리있는애는 0.1에 가깝게 하고
        //                  35                 35   / 35 
        //                  35                 10   / 35 
        fFactor = saturate((_LightInfo.Range - Dis) / _LightInfo.Range);
    }
    else if (_LightInfo.Type == 2)
    {

    }



    vLight = normalize(vLight);
    CALCOLOR.Diff = _LightInfo.Lc.Diff * saturate(dot(_vViewNormal, vLight)) * fFactor;
    CALCOLOR.Diff.a = _vViewPos.a;

    float3 vReflect = normalize(2.f * dot(vLight, _vViewNormal) * _vViewNormal - vLight);
    float3 vEye = normalize(-_vViewPos);

    CALCOLOR.Spec = _LightInfo.Lc.Spec * pow(saturate(dot(vEye, vReflect)), 1) * fFactor;
    CALCOLOR.Ambi = _LightInfo.Lc.Ambi;

    CALCOLOR.Spec.a = _vViewPos.a;
    CALCOLOR.Ambi.a = _vViewPos.a;

    return CALCOLOR;
}

