
struct PS_DEFFERDOUTPUT
{
    float4 vDiffuse     : SV_Target; // ����.
    float4 vPosition    : SV_Target1; // ��ġ
    float4 vNoraml      : SV_Target2; // �븻
    float4 vDepth       : SV_Target3; // ���� �׸���.
    // float4 vDepth       : SV_Target4; // ���� �׸���.
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.


struct VS_COLORINPUT
{
    float4 vPos : POSITION;
    float4 vCol : COLOR;
};

struct VS_COLOROUTPUT
{
    float4 vPos : SV_POSITION;
    float4 vCol : COLOR;
};


struct VS_TEXINPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
};

struct VS_TEXOUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
};


struct PS_OUTPUT
{
    float4 vCol : SV_Target;
};

struct PS_FORWARDOUTPUT
{
    float4 vDiffuse : SV_Target; // ����.
};

struct PS_MERGEOUTPUT
{
    float4 vMergeColor : SV_Target; // ����.
};

struct VS_DEFFERDLIGHTINPUT
{
    float4 vPos: POSITION; // ����.
    float2 vUv: TEXCOORD; // ��ġ
};

struct VS_DEFFERDLIGHTOUTPUT
{
    float4 vPos : SV_POSITION; // ����.
    float2 vUv : TEXCOORD; // ��ġ
};


struct PS_DEFFERDLIGHTOUTPUT
{
    float4 vDiffuse : SV_Target; // Ȯ�걤.
    float4 vSpaculer : SV_Target1; // �ݻ籤.
};