#pragma once
#include "KShader.h"
#include <vector>

class Shader_Vertex : public KShader
{
private:
	ID3D11VertexShader* m_pShader;

	// ��ǲ��ų ���ؽ� �����͸� ������ �ִ� ����
	// 
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputLayoutData;
	UINT m_Offset;
	ID3D11InputLayout*				      m_pInputLayout;

public:
	// ���� HLSL �����Ϸ� ������ �ǹ��Ѵ�.
	bool Load(const char* _FuncName, UINT _VH = 5, UINT _VL = 0);
	void Update() override;

	//D3D11_INPUT_ELEMENT_DESC �� �߰��Ǿ���� ����-> ���ؽ� ���̴��� �̷��� �����ؾ��Ѵ�.
	// �̸�, �ε����� �ݺ� ����, ����, ������ �ѱ� ����, ������ ���ؽ� ������
	//LPCSTR SemanticName;
	//UINT SemanticIndex;
	//DXGI_FORMAT Format;
	//UINT InputSlot;
	//UINT AlignedByteOffset;
	//D3D11_INPUT_CLASSIFICATION InputSlotClass;
	//UINT InstanceDataStepRate;
	bool Add_Layout(LPCSTR _SName, UINT _SI, DXGI_FORMAT _Fm, UINT _IS, D3D11_INPUT_CLASSIFICATION _ISC = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, UINT _IDSR = 0);
	bool Add_LayoutFin(LPCSTR _SName, UINT _SI, DXGI_FORMAT _Fm, UINT _IS, D3D11_INPUT_CLASSIFICATION _ISC = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, UINT _IDSR = 0);
	bool Create_Layout();
	void Set_Layout();

public:
	void UpdateCB(KPtr<ConstBuffer> _Buf);

public:
	Shader_Vertex();
	~Shader_Vertex();
};

