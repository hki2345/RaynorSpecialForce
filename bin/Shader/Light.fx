// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
class LIGHTCOLOR
{
	float4 Diff; // Ȯ�걤
	float4 Spec; // �ݻ籤
	float4 Ambi; // ȯ�汤
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

    // �𷺼�
    if (_LightInfo.Type == 0)
    {
        vLight = _LightInfo.LightDirInv;
    }

    // ����Ʈ
    else if (_LightInfo.Type == 1)
    {
        vLight = normalize(_LightInfo.LightPos - _vViewPos);
        float Dis = distance(_LightInfo.LightPos, _vViewPos);
        // ���� �ָ��ִ¾ִ� 0.1�� ������ �ϰ�
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

