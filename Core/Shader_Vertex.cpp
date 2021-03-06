#include "Shader_Vertex.h"
#include "Core_Class.h"
#include "KMacro.h"	


Shader_Vertex::Shader_Vertex() : m_Offset(0)
{
	m_Type = SHTYPE::ST_VS;
}


Shader_Vertex::~Shader_Vertex()
{
	if (nullptr != m_pShader) { m_pShader->Release(); }
	if (nullptr != m_pInputLayout) { m_pInputLayout->Release(); }
}

bool Shader_Vertex::Load(const char* _FuncName, UINT _VH, UINT _VL) 
{
	UINT iFlag = 0;

	// 11戚窮 10戚窮 促杷 旭精 室戚希 巨獄益稽 吉陥. -> 蕉段拭 11精 赤走亀 省製
#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};
	sprintf_s(szBuffer, "vs_%d_%d", _VH, _VL);


	// 陳督析 幻 背左澗 依戚陥 しししししし
	if (S_OK != D3DCompileFromFile(AllPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName, szBuffer, iFlag, 0, &m_pBlob, &m_pErrBlob))
	{
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();
		KASSERT(true);

		return false;
	}


	// 背雁 巨郊戚什拭 獄努什室戚希研 室特
	if (S_OK != Core_Class::PDevice()->CreateVertexShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr, &m_pShader))
	{
		return false;
	}

	KShader::Init_Default();

	return true;
}

void Shader_Vertex::Update() 
{
	// 食奄辞澗 傾戚焼数聖 宜鍵陥.
	Core_Class::Context()->VSSetShader(m_pShader, 0, 0);
}

bool Shader_Vertex::Add_LayoutFin(LPCSTR _SName, UINT _SI, DXGI_FORMAT _Fm, UINT _IS, D3D11_INPUT_CLASSIFICATION _ISC /*= D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA*/, UINT _IDSR /*= 0*/)
{
	Add_Layout(_SName, _SI, _Fm, _IS, _ISC, _IDSR);
	Create_Layout();

	return true;
}

bool Shader_Vertex::Add_Layout(LPCSTR _SName, UINT _SI, DXGI_FORMAT _Fm
	, UINT _IS, D3D11_INPUT_CLASSIFICATION _ISC, UINT _IDSR) 
{
	D3D11_INPUT_ELEMENT_DESC _NewInputLayout = D3D11_INPUT_ELEMENT_DESC();

	if (_IS >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT)
	{
		return false;
	}

	_NewInputLayout.SemanticName = _SName;
	_NewInputLayout.SemanticIndex = _SI;
	_NewInputLayout.Format = _Fm;
	_NewInputLayout.InputSlot  = _IS;
	_NewInputLayout.InputSlotClass = _ISC;
	_NewInputLayout.InstanceDataStepRate = _IDSR;
	_NewInputLayout.AlignedByteOffset = m_Offset;

	m_Offset += VS_Format::Size_Format(_Fm);
	m_InputLayoutData.push_back(_NewInputLayout);

	return true;
}

bool Shader_Vertex::Create_Layout() 
{
	KASSERT(nullptr == m_pShader);
	if (nullptr == m_pShader)
	{
		return false;
	}

	if (S_OK != Core_Class::PDevice()->CreateInputLayout(&m_InputLayoutData[0], (UINT)m_InputLayoutData.size(), m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), &m_pInputLayout) )
	{
		return false;
	}

	return true;
}

void Shader_Vertex::Set_Layout() 
{
	Core_Class::Context()->IASetInputLayout(m_pInputLayout);
}

void Shader_Vertex::UpdateCB(KPtr<Shader_Vertex::ConstBuffer> _Buf) 
{
	m_pContext->VSSetConstantBuffers(_Buf->iReg, 1, &_Buf->pConstBuffer);
}