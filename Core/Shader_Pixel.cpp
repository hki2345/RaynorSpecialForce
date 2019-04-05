#include "Shader_Pixel.h"
#include "Core_Class.h"
#include "KMacro.h"


Shader_Pixel::Shader_Pixel()
{
	m_Type = SHTYPE::ST_PS;
}


Shader_Pixel::~Shader_Pixel()
{
	if (nullptr != m_pShader) { m_pShader->Release(); }
}

bool Shader_Pixel::Load(const char* _FuncName, UINT _VH, UINT _VL) 
{
	UINT iFlag = 0;

	// 11이든 10이든 짜피 같은 세이더 디버그로 된다. -> 애초에 11은 있지도 않음
#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};
	sprintf_s(szBuffer, "ps_%d_%d", _VH, _VL);

	// 컴파일 만 해보는 것이다 ㅇㅇㅇ
	if (S_OK != D3DCompileFromFile(AllPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName, szBuffer, iFlag, 0, &m_pBlob, &m_pErrBlob))
	{
		// "strcut {"

		// 이 내부에 있는 데이터를 확인할수 있다.
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();
		KASSERT(true);
		return false;
	}

	// 해당 디바이스에 픽셀세이더를 세팅
	if (S_OK != Core_Class::PDevice()->CreatePixelShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr, &m_pShader))
	{
		return false;
	}

	Init_Default();

	return true;
}

void Shader_Pixel::Update() 
{
	// 세팅된 데이터를 돌리는 식
	Core_Class::Context()->PSSetShader(m_pShader, 0, 0);
}

void Shader_Pixel::UpdateCB(KPtr<KShader::ConstBuffer> _Buf)
{
	m_pContext->PSSetConstantBuffers(_Buf->iReg, 1, &_Buf->pConstBuffer);
}