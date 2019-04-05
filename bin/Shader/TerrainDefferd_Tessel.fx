// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "GValue.fx"
#include "PSOUT.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

cbuffer TerrainBuffer : register(b0)
{
    int FloorCount;
    int VTXX;
    int VTXY;
    int temp3;
    int IsBump[4];
}

cbuffer Draw_Info : register(b1)
{
    float2 MPos;
    float2 PPUV;
    float BSize;
    float Click;
    float Tmp1;
    float Tmp2;
};


/************************** 버텍스 ******************************/
struct VTX3DMESH_INPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT; // U와 일치하는 X축
    float4 vBNormal : BINORMAL; // V와 일치하는 탄젠트
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


VTX3DMESH_OUTPUT VS_TERRAINDEFFERD(VTX3DMESH_INPUT _in)
{
    VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT) 0.f;

    outData.vPos = mul(_in.vPos, g_W);
    outData.vUv = _in.vUv;
    outData.vColor.rgba = _in.vColor.rgba;
    
    return outData;
}

/************************** 헐 ******************************/
struct TesselFactor
{
    float EdgeArr[3] : SV_TessFactor;
    float fInsideFactor : SV_InsideTessFactor;
};

// 테셀레이션의 보간 값을 구하는 함수
// 월드기준으로 게산을 한다.
// 노말이나 바이노말은 이거 계산후 다시  계산이 되어야 함
// 짜피 버텍스가 하나 새로 생기는 구현이기 때문에 여기서 달라짐
// 그리고 가까이 갈 때 무조건 나눠버릴 수 있으니 그 거리의 최소값, 최대값, 및 테셀의 최대 최소 까지 존재
float Cal_TessLv(float _fMinDist, float _fMaxDist, float _fMinTess, float _fMaxTess, float3 _vWorldPos)
{
    // 카메라 기준으로 쪼개져야 하는부분
    float3 vDis = g_CamPos.xyz - _vWorldPos;
    // vDis.y = .0f;
    float fDist = length(vDis.xyz);
    float fRatio = (fDist - _fMinDist) / (_fMaxDist - _fMinDist);


    return pow(2, saturate(floor(lerp(4, 0, fRatio)) / 4.0f) * 4.0f);

}

// 버텍스 데이터 3개를 받는다 - 이걸로 삼각형을 구성하기 때문에
TesselFactor HS_CONSTANTDATA(InputPatch<VTX3DMESH_OUTPUT,  3> _VtxData)
{
    TesselFactor Temp = (TesselFactor) .0f;

    float3 vStdPos = (_VtxData[0].vPos.xyz + _VtxData[1].vPos.xyz) / 2.0f;
    float XRatio = _VtxData[2].vPos.x - _VtxData[1].vPos.x;
    float ZRatio = _VtxData[2].vPos.z - _VtxData[0].vPos.z;


    float3 XPos = float3(vStdPos.x + XRatio, vStdPos.y, vStdPos.z);
    float3 ZPos = float3(vStdPos.x, vStdPos.y, vStdPos.z + ZRatio);

    float FTess = Cal_TessLv(5.0f, 10.0f, .0f, .0f, vStdPos);
    float FXTess = Cal_TessLv(5.0f, 10.0f, .0f, .0f, XPos);
    float FZTess = Cal_TessLv(5.0f, 10.0f, .0f, .0f, ZPos);



    if(FTess < FZTess)
    {
        Temp.EdgeArr[0] = FZTess;
    }
    else
    {
        Temp.EdgeArr[0] = FTess;
    }

    if (FTess < FXTess)
    {
        Temp.EdgeArr[1] = FXTess;
    }
    else
    {
        Temp.EdgeArr[1] = FTess;
    }

    Temp.EdgeArr[2] = FTess;
    Temp.fInsideFactor = FTess;

    return Temp;
}

struct HULL_OUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
};



/************************** 진짜 헐 세이더 ******************************/
// 헐에서 파생되는(계산되는) 게 2가지 있는데 그 중하나를 위에서 다시 한번 계산하겠다는
// 뜻이다. 그 과정이 위에 적혀있고
[domain("tri")] // 그리는 방식  // 어떤 도형으로 만들것인지
[partitioning("integer")] // 삼각형을 그릴때 다이 [partitioning("fractional_odd")]// 삼각형 번호
[outputcontrolpoints(3)] // 
[maxtessfactor(64.0f)]
[patchconstantfunc("HS_CONSTANTDATA")] // 헐셰이더 나갈때 콘스트쪽에서 실행되는 함수
[outputtopology("triangle_ccw")]
HULL_OUT HS_TERRAINEDEFFERED(InputPatch<VTX3DMESH_OUTPUT, 3> _PatchData
, uint i : SV_OutputControlPointID
, uint _patchID : SV_PrimitiveID)
{
    HULL_OUT outData = (HULL_OUT) .0f;

    outData.vPos = _PatchData[i].vPos;
    outData.vUv = _PatchData[i].vUv;

    return outData;
}


// 계산 상 뭐 실 수 
struct DOMAIN_OUT
{
    float4 vPos : SV_Position;
    float2 vUv : TEXCOORD;
    
    float4 vWorldPos : POSITION0;
    float4 vViewPos : POSITION1;
    float4 vProjPos : POSITION2;
};

[domain("tri")]
DOMAIN_OUT DS_TERRAINEDEFFERED(TesselFactor _pFactor
, float3 _vTriRatio : SV_DomainLocation
, const OutputPatch<HULL_OUT, 3> _ControlPoints)
{
    DOMAIN_OUT outPut = (DOMAIN_OUT) .0f;

    outPut.vWorldPos.xyz =
    (_vTriRatio.x * _ControlPoints[0].vPos.xyz)
    + (_vTriRatio.y * _ControlPoints[1].vPos.xyz)
    + (_vTriRatio.z * _ControlPoints[2].vPos.xyz);
    outPut.vWorldPos.w = 1.0f;
    

    outPut.vViewPos = mul(outPut.vWorldPos, g_V);
    outPut.vPos = mul(outPut.vViewPos, g_P);
    outPut.vProjPos = outPut.vPos;
    outPut.vUv = 
    (_vTriRatio.x * _ControlPoints[0].vUv)
    + (_vTriRatio.y * _ControlPoints[1].vUv)
    + (_vTriRatio.z * _ControlPoints[2].vUv);


    return outPut;
}




/************************** 픽셀 ******************************/
PS_DEFFERDOUTPUT PS_TERRAINDEFFERD(DOMAIN_OUT _in)
{
    PS_DEFFERDOUTPUT outData = (PS_DEFFERDOUTPUT) 0.0f;
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);


    // 새로 정의된 정점들의 노멀값들을 재정의 해야한다.
    float3 Right = float3(_in.vWorldPos.x + 1.0f, _in.vWorldPos.y, _in.vWorldPos.z);
    float3 Top = float3(_in.vWorldPos.x, _in.vWorldPos.y, _in.vWorldPos.z + 1.0f);
   
    float3 Binormal = normalize(Top - _in.vWorldPos.xyz);
    float3 Tangent = normalize(Right - _in.vWorldPos.xyz);
    float3 Normal = normalize(cross(Binormal, Tangent));


    CalColor *= GetMTexToColor(8, 8, _in.vUv, 0.0f);
    float4 BumpNormal = CalMBump(8, 8, _in.vUv, 1.0f, float4(Tangent, .0f), float4(Binormal, .0f), float4(Normal, .0f));
    
    BumpNormal = mul(BumpNormal, g_V);

    float2 SpUv;

    SpUv.x = _in.vUv.x / VTXX;
    SpUv.y = _in.vUv.y / VTXY;

    for (int i = 0; i < FloorCount; ++i)
    {
        // 색깔 섞기.
        float4 Ratio = GetTexToColor(i, i, SpUv);
        float RatioValuie = (Ratio.x + Ratio.y + Ratio.z) / 3.0f;
        float4 FloorColor = GetMTexToColor(9 + i, 9 + i, _in.vUv, 0.0f);
        float4 SrcColor = CalColor;
        FloorColor.xyz *= RatioValuie;
        SrcColor.xyz *= (1.0f - Ratio.x);
        CalColor = FloorColor + SrcColor;
    }




    if (MPos.x - PPUV.x * BSize /* * BSize * .5f*/ < _in.vUv.x / VTXX &&
        MPos.x + PPUV.x * BSize /* * BSize * .5f*/ > _in.vUv.x / VTXX &&
        MPos.y - PPUV.y * BSize /* * BSize * .5f*/ < _in.vUv.y / VTXY &&
        MPos.y + PPUV.y * BSize /* * BSize * .5f*/ > _in.vUv.y / VTXY)
    {
        float4 Tmp4 = float4(1.0f, 1.0f, 1.0f, 1.0f);
        if (1.0f == Click)
        {
            Tmp4 *= float4(.0f, 1.0f, .0f, 1.0f);
        }
        if (-1.0f == Click)
        {
            Tmp4 *= float4(1.0f, .0f, .0f, 1.0f);
        }


        // 이미지 UV의 시작
        float2 Tmp = float2(
        _in.vUv.x / VTXX - (MPos.x - PPUV.x * BSize),
        _in.vUv.y / VTXY - (MPos.y - PPUV.y * BSize));
        // CalColor += float4(1.0f, 1.0f, .0f, .0f);
        CalColor += GetTexToColor(ArrTex[3].Tex_Idx, ArrTex[3].Tex_Smp, Tmp * float2(g_W._11, g_W._33) * .5f * BSize) * Tmp4;

        
    }

	// 칼 컬러가 섞인것으로 나와야 한다.

	// 포워드 색깔을 아예 사용하지 않는 것은 아니다.
    // outData.vDiffuse.rgb = float4(1.0f, 1.0f, 1.0f, 1.0f);
    outData.vDiffuse.rgb = CalColor.xyz;
    outData.vDiffuse.a = 1.0f;
    outData.vNoraml = BumpNormal;
    outData.vNoraml.a = 1.0f;
    outData.vPosition = _in.vViewPos;
    outData.vDepth.x = _in.vPos.z;
    outData.vDepth.w = 1.0f;

    return outData;
}

