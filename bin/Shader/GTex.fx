

// 샘플러와 텍스처를 의미가 다르다.
// 샘플러는 세팅을 안하면 오류를 뱉는다.
// 함

#define TEX 0
#define BUMP 1
#define SPEC 2
#define EMIS 3

Texture2D g_Tex_0 : register(t0);
Texture2D g_Tex_1 : register(t1);
Texture2D g_Tex_2 : register(t2);
Texture2D g_Tex_3 : register(t3);
Texture2D g_Tex_4 : register(t4);
Texture2D g_Tex_5 : register(t5);
Texture2D g_Tex_6 : register(t6);
Texture2D g_Tex_7 : register(t7);

Texture2DArray g_ArrTex_0 : register(t8);
Texture2DArray g_ArrTex_1 : register(t9);
Texture2DArray g_ArrTex_2 : register(t10);
Texture2DArray g_ArrTex_3 : register(t11);
Texture2DArray g_ArrTex_4 : register(t12);
Texture2DArray g_ArrTex_5 : register(t13);
Texture2DArray g_ArrTex_6 : register(t14);
Texture2DArray g_ArrTex_7 : register(t15);

SamplerState g_Sam_0 : register(s0);
SamplerState g_Sam_1 : register(s1);
SamplerState g_Sam_2 : register(s2);
SamplerState g_Sam_3 : register(s3);
SamplerState g_Sam_4 : register(s4);
SamplerState g_Sam_5 : register(s5);
SamplerState g_Sam_6 : register(s6);
SamplerState g_Sam_7 : register(s7);
SamplerState g_Sam_8 : register(s8);
SamplerState g_Sam_9 : register(s9);
SamplerState g_Sam_10 : register(s10);
SamplerState g_Sam_11 : register(s11);
SamplerState g_Sam_12 : register(s12);
SamplerState g_Sam_13 : register(s13);
SamplerState g_Sam_14 : register(s14);
SamplerState g_Sam_15 : register(s15);

//Texture2D GetTex(int Slot)
//{
//}

float4 GetColor(Texture2D _Tex, int _Smp, float2 _Uv)
{
    switch (_Smp)
    {
        case 0:
            return _Tex.Sample(g_Sam_0, _Uv);
        case 1:
            return _Tex.Sample(g_Sam_1, _Uv);
        case 2:
            return _Tex.Sample(g_Sam_2, _Uv);
        case 3:
            return _Tex.Sample(g_Sam_3, _Uv);
        case 4:
            return _Tex.Sample(g_Sam_4, _Uv);
        case 5:
            return _Tex.Sample(g_Sam_5, _Uv);
        case 6:
            return _Tex.Sample(g_Sam_6, _Uv);
        case 7:
            return _Tex.Sample(g_Sam_7, _Uv);
        default:
            return (float4)0.0f;
    }
}

float4 GetTexToColor(int _Tex, int _Smp, float2 _Uv)
{
    switch (_Tex)
    {
        case 0:
            return GetColor(g_Tex_0, _Smp, _Uv);
        case 1:
            return GetColor(g_Tex_1, _Smp, _Uv);
        case 2:
            return GetColor(g_Tex_2, _Smp, _Uv);
        case 3:
            return GetColor(g_Tex_3, _Smp, _Uv);
        case 4:
            return GetColor(g_Tex_4, _Smp, _Uv);
        case 5:
            return GetColor(g_Tex_5, _Smp, _Uv);
        case 6:
            return GetColor(g_Tex_6, _Smp, _Uv);
        case 7:
            return GetColor(g_Tex_7, _Smp, _Uv);
        default:
            return (float4)0.0f;
    }
}


float4 GetMColor(Texture2DArray _Tex, int _Smp, float2 _Uv, float _Idx)
{
    float3 UV3D;

    UV3D.xy = _Uv.xy;
    UV3D.z = _Idx;

    switch (_Smp)
    {
        case 8:
            return _Tex.Sample(g_Sam_8, UV3D);
        case 9:
            return _Tex.Sample(g_Sam_9, UV3D);
        case 10:
            return _Tex.Sample(g_Sam_10, UV3D);
        case 11:
            return _Tex.Sample(g_Sam_11, UV3D);
        case 12:
            return _Tex.Sample(g_Sam_12, UV3D);
        case 13:
            return _Tex.Sample(g_Sam_13, UV3D);
        case 14:
            return _Tex.Sample(g_Sam_14, UV3D);
        case 15:
            return _Tex.Sample(g_Sam_15, UV3D);
        default:
            return (float4) 0.0f;
    }
}

float4 GetMTexToColor(int _Tex, int _Smp, float2 _Uv, float _Idx)
{
    switch (_Tex)
    {
        case 8:
            return GetMColor(g_ArrTex_0, _Smp, _Uv, _Idx);
        case 9:
            return GetMColor(g_ArrTex_1, _Smp, _Uv, _Idx);
        case 10:
            return GetMColor(g_ArrTex_2, _Smp, _Uv, _Idx);
        case 11:
            return GetMColor(g_ArrTex_3, _Smp, _Uv, _Idx);
        case 12:
            return GetMColor(g_ArrTex_4, _Smp, _Uv, _Idx);
        case 13:
            return GetMColor(g_ArrTex_5, _Smp, _Uv, _Idx);
        case 14:
            return GetMColor(g_ArrTex_6, _Smp, _Uv, _Idx);
        case 15:
            return GetMColor(g_ArrTex_7, _Smp, _Uv, _Idx);
        default:
            return (float4) 0.0f;
    }
}


//                                          x                    y
// 알파가 Red - Red가 Blue
float4 CalSBump(int _Bumpidx, int _BumpSmp, float2 _Uv, float4 _ViewTangent, float4 _ViewBNormal, float4 _Normal)
{
    float4 BumpNormal = GetTexToColor(_Bumpidx, _BumpSmp, _Uv);

    
    BumpNormal.b = BumpNormal.r;
    BumpNormal.r = BumpNormal.a;

        
    BumpNormal = BumpNormal * 2.0f - 1.0f; // 0.0 ~ 1.0f -> -1.0f ~ 1.0f
    BumpNormal = normalize(BumpNormal);
    BumpNormal.w = 0.0f;
    
    // 탄젠트 공간행렬이라고 한다.
    float3x3 matTBN = float3x3(_ViewTangent.xyz, _ViewBNormal.xyz, _Normal.xyz);
    BumpNormal.xyz = mul(BumpNormal.xyz, matTBN);
    BumpNormal.w = 0.0f;

    return BumpNormal;
}

float4 CalBump(int _Bumpidx, int _BumpSmp, float2 _Uv, float4 _ViewTangent, float4 _ViewBNormal, float4 _Normal)
{
    float4 BumpNormal = GetTexToColor(_Bumpidx, _BumpSmp, _Uv);
    
    BumpNormal = BumpNormal * 2.0f - 1.0f; // 0.0 ~ 1.0f -> -1.0f ~ 1.0f
    BumpNormal = normalize(BumpNormal);
    BumpNormal.w = 0.0f;
    
    // 탄젠트 공간행렬이라고 한다.
    float3x3 matTBN = float3x3(_ViewTangent.xyz, _ViewBNormal.xyz, _Normal.xyz);
    BumpNormal.xyz = mul(BumpNormal.xyz, matTBN);
    BumpNormal.w = 0.0f;

    return BumpNormal;
}

float4 CalMBump(int _Bumpidx, int _BumpSmp, float2 _Uv, float _Idx, float4 _ViewTangent, float4 _ViewBNormal, float4 _Normal)
{
    float4 BumpNormal = GetMTexToColor(_Bumpidx, _BumpSmp, _Uv, _Idx);

    BumpNormal = BumpNormal * 2.0f - 1.0f; // 0.0 ~ 1.0f -> -1.0f ~ 1.0f
    BumpNormal = normalize(BumpNormal);
    BumpNormal.w = 0.0f;
    // 탄젠트 공간행렬이라고 한다.

    float3x3 matTBN = float3x3(_ViewTangent.xyz, _ViewBNormal.xyz, _Normal.xyz);
    BumpNormal.xyz = mul(BumpNormal.xyz, matTBN);
    BumpNormal.w = 0.0f;

    return BumpNormal;
}