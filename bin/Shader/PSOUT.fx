
struct PS_DEFFERDOUTPUT
{
    float4 vDiffuse     : SV_Target; // 색깔.
    float4 vPosition    : SV_Target1; // 위치
    float4 vNoraml      : SV_Target2; // 노말
    float4 vDepth       : SV_Target3; // 깊이 그림자.
    // float4 vDepth       : SV_Target4; // 깊이 그림자.
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.


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
    float4 vDiffuse : SV_Target; // 색깔.
};

struct PS_MERGEOUTPUT
{
    float4 vMergeColor : SV_Target; // 색깔.
};

struct VS_DEFFERDLIGHTINPUT
{
    float4 vPos: POSITION; // 색깔.
    float2 vUv: TEXCOORD; // 위치
};

struct VS_DEFFERDLIGHTOUTPUT
{
    float4 vPos : SV_POSITION; // 색깔.
    float2 vUv : TEXCOORD; // 위치
};


struct PS_DEFFERDLIGHTOUTPUT
{
    float4 vDiffuse : SV_Target; // 확산광.
    float4 vSpaculer : SV_Target1; // 반사광.
};