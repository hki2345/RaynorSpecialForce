#pragma once
#include "KShader.h"
#include <vector>

class Shader_Vertex : public KShader
{
private:
	ID3D11VertexShader* m_pShader;

	// 인풋시킬 버텍스 데이터를 가지고 있는 벡터
	// 
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputLayoutData;
	UINT m_Offset;
	ID3D11InputLayout*				      m_pInputLayout;

public:
	// 버전 HLSL 컴파일러 버전을 의미한다.
	bool Load(const char* _FuncName, UINT _VH = 5, UINT _VL = 0);
	void Update() override;

	//D3D11_INPUT_ELEMENT_DESC 에 추가되어야할 사항-> 버텍스 세이더를 이렇게 설정해야한다.
	// 이름, 인덱스의 반복 여부, 포멧, 데이터 넘길 슬롯, 지정된 버텍스 사이즈
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

