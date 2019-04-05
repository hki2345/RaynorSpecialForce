
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
    int IsDefferdAndForward; // ���۴��� �������?
	int LightVtxToPix; // ���ؽ��� �ȼ��̳�? // 0 �̸� ���ؽ� 1�̸� �ȼ�
    int TexCount; // ���ؽ��� �ȼ��̳�? // 8
    int IsAni; // ���ؽ��� �ȼ��̳�? // 8
    int RendrDataType; // ������ ����̳� // 8 - Ŀ�Ǿ�� �� ��� �ʿ䰡 ����.
    float Light_Opa; // �� ���� 
    int IsAlpha; // �������̳� �����̳�? // 8
    TexData ArrTex[8]; // 20 216
}
