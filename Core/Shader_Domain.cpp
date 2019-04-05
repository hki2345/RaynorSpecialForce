#include "Shader_Domain.h"
#include "Core_Class.h"
#include "KMacro.h"


Shader_Domain::Shader_Domain()
{
	m_Type = SHTYPE::ST_DOM;
}


Shader_Domain::~Shader_Domain()
{
	if (nullptr != m_pShader) { m_pShader->Release(); }
}

bool Shader_Domain::Load(const char* _FuncName, UINT _VH, UINT _VL)
{
	UINT iFlag = 0;

	// 11�̵� 10�̵� ¥�� ���� ���̴� ����׷� �ȴ�. -> ���ʿ� 11�� ������ ����
#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};
	sprintf_s(szBuffer, "ds_%d_%d", _VH, _VL);

	// ������ �� �غ��� ���̴� ������
	if (S_OK != D3DCompileFromFile(AllPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName, szBuffer, iFlag, 0, &m_pBlob, &m_pErrBlob))
	{
		// "strcut {"

		// �� ���ο� �ִ� �����͸� Ȯ���Ҽ� �ִ�.
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();
		KASSERT(true);
		return false;
	}

	// �ش� ����̽��� �ȼ����̴��� ����
	if (S_OK != Core_Class::PDevice()->CreateDomainShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr, &m_pShader))
	{
		return false;
	}

	Init_Default();

	return true;
}

void Shader_Domain::Update()
{
	// ���õ� �����͸� ������ ��
	Core_Class::Context()->DSSetShader(m_pShader, 0, 0);
}

void Shader_Domain::UpdateCB(KPtr<KShader::ConstBuffer> _Buf)
{
	m_pContext->DSSetConstantBuffers(_Buf->iReg, 1, &_Buf->pConstBuffer);
}