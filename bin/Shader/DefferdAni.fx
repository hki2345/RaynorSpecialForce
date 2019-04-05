// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "GValue.fx"
#include "GameValue.fx"

#include "PSOUT.fx"
#include "DefferdLight.fx"
#include "BoneAni.fx"

struct VTX3DMESH_INPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT; // U와 일치하는 X축
    float4 vBNormal : BINORMAL; // V와 일치하는 탄젠트
    float4 vWeight : WEIGHTS; // U와 일치하는 X축
    float4 vIndices : BLENDINDICES; // V와 일치하는 탄젠트
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의 
// 변형을 불가능하게 한다.
struct VTX3DMESH_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT; // U와 일치하는 X축
    float4 vBNormal : BINORMAL; // V와 일치하는 탄젠트
};

class SkinInfo
{
    float4 vPos;
    float4 vNormal;
    float4 vBNormal;
    float4 vTangent; // U와 일치하는 X축
};


matrix GetBoneMat(int _BoneIdx, int _RowIdx)
{
    // 이녀석은 뭐냐?
    // Load 그냥 픽셀단위로 색깔을 가져오겠다.
    // 샘플러를 통해서가 아닌 
    matrix BoneMat =
    {
        // Load 는 색을 이쓴ㄴ 그대로 가져옴
        // 샘플링은 크기 뭐 이런거 고려해서 색을 뽑아오는 거라면
        // 이건 있는 색을 그대로 가져옴
        g_BoneTex.Load(int3((_BoneIdx * 4), _RowIdx, 0)),
        g_BoneTex.Load(int3((_BoneIdx * 4) + 1, _RowIdx, 0)),
        g_BoneTex.Load(int3((_BoneIdx * 4) + 2, _RowIdx, 0)),
        g_BoneTex.Load(int3((_BoneIdx * 4) + 3, _RowIdx, 0))
    };

    return BoneMat;
}

void Skinning(inout float4 _vPos, 
    inout float4 _vTangent, inout float4 _vBNormal, inout float4 _vNormal,
    inout float4 _vWeight, inout float4 _vIndices, int _RowIdx)
{
    SkinInfo Info = (SkinInfo)0.0f;

    for (int i = 0; i < 4; ++i)
    {
        if (0.0f == _vWeight[i])
        {
            continue;
        }

        matrix MatBone = GetBoneMat((int) _vIndices[i], _RowIdx);

        Info.vPos += mul(_vPos, MatBone) * _vWeight[i];
        Info.vTangent += mul(_vTangent, MatBone) * _vWeight[i];
        Info.vBNormal += mul(_vBNormal, MatBone) * _vWeight[i];
        Info.vNormal += mul(_vNormal, MatBone) * _vWeight[i];
    }

    Info.vPos.w = 1.0f;
    Info.vTangent.w = 0.0f;
    Info.vBNormal.w = 0.0f;
    Info.vNormal.w = 0.0f;

    _vPos = Info.vPos;
    _vTangent = normalize(Info.vTangent);
    _vBNormal = normalize(Info.vBNormal);;
    _vNormal = normalize(Info.vNormal);;

}

VTX3DMESH_OUTPUT VS_DEFFERDANI(VTX3DMESH_INPUT _in)
{
    VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT) 0.f;

    Skinning(_in.vPos, _in.vTangent, _in.vBNormal
    , _in.vNormal, _in.vWeight, _in.vIndices, 0);

    outData.vPos = mul(_in.vPos, g_WVP);
    outData.vUv = _in.vUv;

    // 세력 색깔 - 테스트 부분 - 여기에 박아넣기엔 메쉬를 재생산해야하므로
    // 그냥 컨스트 버퍼 하나 더 받는 식으로 했다.
    outData.vColor.rgba = _in.vColor.rgba;
    outData.vViewPos = mul(_in.vPos, g_WV);
    outData.vNormal = normalize(mul(_in.vNormal, g_WV));
    outData.vTangent = normalize(mul(_in.vTangent, g_WV));
    outData.vBNormal = normalize(mul(_in.vBNormal, g_WV));

    return outData;
}

PS_DEFFERDOUTPUT PS_DEFFERDANI(VTX3DMESH_OUTPUT _in)
{
    PS_DEFFERDOUTPUT outData = (PS_DEFFERDOUTPUT) 0.0f;
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);


    for (int i = 0; i < TexCount; ++i)
    {
        if (-1 != ArrTex[i].Tex_Idx)
        {
            // 세력 색깔이 텍스쳐 전체에 곱해지는 게 아니고 ...
            if (ArrTex[i].Type == TEX)
            {
                CalColor *= GetTexToColor(ArrTex[i].Tex_Idx, ArrTex[i].Tex_Smp, _in.vUv) /* * _in.vColor*/;
                if (.98f > CalColor.a)
                {                    
                    float4 TCol = g_ColorTex.Load(int3(0, 0, 0));
                    CalColor *= TCol;
                }

            }
            else if (ArrTex[i].Type == BUMP)
            {
                _in.vNormal = CalSBump(ArrTex[i].Tex_Idx, ArrTex[i].Tex_Smp, _in.vUv, _in.vTangent, _in.vBNormal, _in.vNormal);
            }
            else if (ArrTex[i].Type == SPEC)
            {
                float4 Spec = GetTexToColor(ArrTex[i].Tex_Idx, ArrTex[i].Tex_Smp, _in.vUv) * _in.vColor;
                CalColor *= saturate(Spec * 1.6f + CalColor);
            }
            else if (ArrTex[i].Type == EMIS)
            {
                CalColor += GetTexToColor(ArrTex[i].Tex_Idx, ArrTex[i].Tex_Smp, _in.vUv) * _in.vColor * 3.0f;
            }
        }
    }

    


    outData.vDiffuse.rgb = CalColor.rgb;
    outData.vDiffuse.a = _in.vColor.a;
    outData.vNoraml = _in.vNormal;
    outData.vNoraml.a = 1.0f;
    outData.vPosition = _in.vViewPos;
    outData.vDepth.x = outData.vPosition.z;
    outData.vDepth.w = 1.0f;

    return outData;
}
