// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
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


/************************** ���ؽ� ******************************/
struct VTX3DMESH_INPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT; // U�� ��ġ�ϴ� X��
    float4 vBNormal : BINORMAL; // V�� ��ġ�ϴ� ź��Ʈ
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
    float4 vTangent : TANGENT; // U�� ��ġ�ϴ� X��
    float4 vBNormal : BINORMAL; // V�� ��ġ�ϴ� ź��Ʈ
};


VTX3DMESH_OUTPUT VS_TERRAINDEFFERD(VTX3DMESH_INPUT _in)
{
    VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT) 0.f;

    outData.vPos = mul(_in.vPos, g_W);
    outData.vUv = _in.vUv;
    outData.vColor.rgba = _in.vColor.rgba;
    
    return outData;
}

/************************** �� ******************************/
struct TesselFactor
{
    float EdgeArr[3] : SV_TessFactor;
    float fInsideFactor : SV_InsideTessFactor;
};

// �׼����̼��� ���� ���� ���ϴ� �Լ�
// ����������� �Ի��� �Ѵ�.
// �븻�̳� ���̳븻�� �̰� ����� �ٽ�  ����� �Ǿ�� ��
// ¥�� ���ؽ��� �ϳ� ���� ����� �����̱� ������ ���⼭ �޶���
// �׸��� ������ �� �� ������ �������� �� ������ �� �Ÿ��� �ּҰ�, �ִ밪, �� �׼��� �ִ� �ּ� ���� ����
float Cal_TessLv(float _fMinDist, float _fMaxDist, float _fMinTess, float _fMaxTess, float3 _vWorldPos)
{
    // ī�޶� �������� �ɰ����� �ϴºκ�
    float3 vDis = g_CamPos.xyz - _vWorldPos;
    // vDis.y = .0f;
    float fDist = length(vDis.xyz);
    float fRatio = (fDist - _fMinDist) / (_fMaxDist - _fMinDist);


    return pow(2, saturate(floor(lerp(4, 0, fRatio)) / 4.0f) * 4.0f);

}

// ���ؽ� ������ 3���� �޴´� - �̰ɷ� �ﰢ���� �����ϱ� ������
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



/************************** ��¥ �� ���̴� ******************************/
// �濡�� �Ļ��Ǵ�(���Ǵ�) �� 2���� �ִµ� �� ���ϳ��� ������ �ٽ� �ѹ� ����ϰڴٴ�
// ���̴�. �� ������ ���� �����ְ�
[domain("tri")] // �׸��� ���  // � �������� ���������
[partitioning("integer")] // �ﰢ���� �׸��� ���� [partitioning("fractional_odd")]// �ﰢ�� ��ȣ
[outputcontrolpoints(3)] // 
[maxtessfactor(64.0f)]
[patchconstantfunc("HS_CONSTANTDATA")] // ����̴� ������ �ܽ�Ʈ�ʿ��� ����Ǵ� �Լ�
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


// ��� �� �� �� �� 
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




/************************** �ȼ� ******************************/
PS_DEFFERDOUTPUT PS_TERRAINDEFFERD(DOMAIN_OUT _in)
{
    PS_DEFFERDOUTPUT outData = (PS_DEFFERDOUTPUT) 0.0f;
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);


    // ���� ���ǵ� �������� ��ְ����� ������ �ؾ��Ѵ�.
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
        // ���� ����.
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


        // �̹��� UV�� ����
        float2 Tmp = float2(
        _in.vUv.x / VTXX - (MPos.x - PPUV.x * BSize),
        _in.vUv.y / VTXY - (MPos.y - PPUV.y * BSize));
        // CalColor += float4(1.0f, 1.0f, .0f, .0f);
        CalColor += GetTexToColor(ArrTex[3].Tex_Idx, ArrTex[3].Tex_Smp, Tmp * float2(g_W._11, g_W._33) * .5f * BSize) * Tmp4;

        
    }

	// Į �÷��� ���ΰ����� ���;� �Ѵ�.

	// ������ ������ �ƿ� ������� �ʴ� ���� �ƴϴ�.
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

