#include "Shader_GeoMetry.h"
#include "Core_Class.h"
#include "KMacro.h"


Shader_GeoMetry::Shader_GeoMetry()
{
	m_Type = SHTYPE::ST_DOM;
}


Shader_GeoMetry::~Shader_GeoMetry()
{
	if (nullptr != m_pShader) { m_pShader->Release(); }
}







bool Shader_GeoMetry::Load(const char* _FuncName, const KUINT& _VH, const KUINT& _VL)
{
	UINT iFlag = 0;

	// 11이든 10이든 짜피 같은 세이더 디버그로 된다. -> 애초에 11은 있지도 않음
#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};
	sprintf_s(szBuffer, "gs_%d_%d", _VH, _VL);

	// 컴파일 만 해보는 것이다 ㅇㅇㅇ
	if (S_OK != D3DCompileFromFile(AllPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName, szBuffer, iFlag, 0, &m_pBlob, &m_pErrBlob))
	{
		// "strcut {"

		// 이 내부에 있는 데이터를 확인할수 있다.
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();
		BBY;
		return false;
	}

	// 해당 디바이스에 픽셀세이더를 세팅
	if (S_OK != Core_Class::PDevice()->CreateGeometryShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr, &m_pShader))
	{
		BBY;
		return false;
	}

	Init_Default();
	return true;
}


// 정점이 진짜 생성될 때의 쉐이더 세팅
// D3D11_SO_DECLARATION_ENTRY 쉐이더 통과된 버텍스들의 데이터 배열
bool Shader_GeoMetry::Load(const char* _FuncName, D3D11_SO_DECLARATION_ENTRY* _Data, const KUINT& _ENDTRYCNT, const KUINT* _pBufferStrides
	, const KUINT& _BufCnt, const KUINT& _StreamRS, const KUINT& _VH/* = 5*/, const KUINT& _VL/* = 0*/)
{
	UINT iFlag = 0;

	// 11이든 10이든 짜피 같은 세이더 디버그로 된다. -> 애초에 11은 있지도 않음
#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};
	sprintf_s(szBuffer, "gs_%d_%d", _VH, _VL);

	// 컴파일 만 해보는 것이다 ㅇㅇㅇ
	if (S_OK != D3DCompileFromFile(AllPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName, szBuffer, iFlag, 0, &m_pBlob, &m_pErrBlob))
	{
		// "strcut {"

		// 이 내부에 있는 데이터를 확인할수 있다.
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();
		BBY;
		return false;
	}

	if (S_OK != Core_Class::PDevice()->CreateGeometryShaderWithStreamOutput(
		m_pBlob->GetBufferPointer()
	, m_pBlob->GetBufferSize()
	, _Data
	, _ENDTRYCNT
	, _pBufferStrides
	, _BufCnt
	, _StreamRS
	, nullptr, &m_pShader))
	{
		return false;
	}

	Init_Default();
	return true;
}




void Shader_GeoMetry::Update()
{
	// 세팅된 데이터를 돌리는 식
	Core_Class::Context()->GSSetShader(m_pShader, 0, 0);
}

void Shader_GeoMetry::UpdateCB(KPtr<KShader::ConstBuffer> _Buf)
{
	m_pContext->GSSetConstantBuffers(_Buf->iReg, 1, &_Buf->pConstBuffer);
}