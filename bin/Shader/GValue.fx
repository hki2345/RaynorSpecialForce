
cbuffer TransformBuffer : register(b10)
{
	matrix g_W;
	matrix g_V;
	matrix g_P;
    matrix g_WV;
    matrix g_VP;
	matrix g_WVP;
    float4 g_CamPos;
}

class TexData
{
    int Type;
    int Tex_Idx;
    int Tex_Smp;
    int Temp;
};

cbuffer RenderOption : register(b11)
{
	int IsLight;
    int IsDefferdAndForward; // 디퍼느냐 포워드냐?
	int LightVtxToPix; // 버텍스냐 픽셀이냐? // 0 이면 버텍스 1이면 픽셀
    int TexCount; // 버텍스냐 픽셀이냐? // 8
    int IsAni; // 버텍스냐 픽셀이냐? // 8
    int RendrDataType; // 렌더가 어떤것이냐 // 8 - 커ㅗ어에서 씀 고로 필요가 없다.
    float Light_Opa; // 빛 연산 
    int IsAlpha; // 불투명이냐 투명이냐? // 8
    TexData ArrTex[8]; // 20 216
}
