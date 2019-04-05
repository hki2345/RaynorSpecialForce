#include "KShader.h"
#include "Stl_AID.h"
#include "Core_Class.h"

KShader::KShader() : m_Type(ST_NONE)
{
}

KShader::~KShader()
{
	// �� ��ü....
	// �� �������� �Լ��� ���� �̰͵��� ����Ʈ �����ͷ� �����ִٴ� ���� �� �� �ִ�.
	// ���� ���⼭ ����Ʈ ���� ������ ���� ��
	if (nullptr != m_pBlob)	{		m_pBlob->Release();	}
	if (nullptr != m_pErrBlob)	{		m_pErrBlob->Release();	}
}

void KShader::Init_Default() 
{
	m_pContext = Core_Class::Context();
}

bool KShader::CreateCB(ConstBuffer* NewBuf)
{
	if (S_OK != Core_Class::PDevice()->CreateBuffer(&NewBuf->tDesc, nullptr, &NewBuf->pConstBuffer))
	{
		delete NewBuf;
		return false;
	}

	return true;
}

KPtr<KShader::ConstBuffer> KShader::FindCB(const wchar_t* _Name)
{
	return Map_Find<KPtr<KShader::ConstBuffer>>(m_MapConstBuffer, _Name);
}
