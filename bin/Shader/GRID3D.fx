// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"
#include "GTex.fx"
// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.
struct VTX3DMESH_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
struct VTX_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vViewPos : POSITION;
    float4 vWorldPos : POSTION;
};

struct PS_OUTPUT
{
    float4 vColor : SV_Target;
};

VTX_OUTPUT VS_GRID3D(VTX3DMESH_INPUT _Data)
{
    VTX_OUTPUT outData = (VTX_OUTPUT) 0.f;

    outData.vUv = _Data.vUv;
    outData.vPos = mul(_Data.vPos, g_WVP);
    outData.vViewPos = mul(_Data.vPos, g_WV);
    outData.vWorldPos = mul(_Data.vPos, g_W);

    return outData;
}

cbuffer GRIDDATA : register(b0)
{
    float4 g_GridData;
}

PS_OUTPUT PS_GRID3D(VTX_OUTPUT _in)
{
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;
	// outData.vColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� y�� �ѱ�� ����� ��ȿ�� ���̴�.
	// ���� -> �ȼ� �ϳ��ϳ� ���� ����� �ϰ� �ɰ��̹Ƿ�.

	// ĭ ������ ����. 100.0f
	// ��� ������ ����
	// 100.0f ������
    
    // float fDepth = g_Tex_7.Sample(g_Sam_0, _in.vUv).x;



    float BlockSize = g_GridData.x;
    float Alpha = g_GridData.z;
    float Boader = g_GridData.y;


    float fX = abs(_in.vWorldPos.x % (BlockSize * 10));
    float fZ = abs(_in.vWorldPos.z % (BlockSize * 10));

	// ���� -> ī�޶��� ���̸� ���ؼ� ����Ѵ�.

	// �׷����� �ϴ� �ֵ鸸 ���������� �׸���.
    if (fZ <= Boader || fX <= Boader)
    {
        outData.vColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
        return outData;
    }

    fX = abs(_in.vWorldPos.x % (BlockSize));
    fZ = abs(_in.vWorldPos.z % (BlockSize));

    if (fZ <= Boader || fX <= Boader)
    {      
        // �ƹ����� ..... �����̶� ������ ������� �ʴ°�...:?????????
        // if (fDepth < _in.vViewPos.z)
        // {
        //     outData.vColor = float4(1.0f, 1.0f, 1.0f, Alpha);
        // }
        // else
        {
            outData.vColor = float4(1.0f, 1.0f, 1.0f, Alpha /** .0f*/);
        }
        return outData;
    }

    clip(-1);

    return outData;
}