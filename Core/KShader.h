#pragma once
#include "Resource.h"
#include "DXContainer.h"
#include <unordered_map>

class KShader : public KResource
{
public:

protected:
	SHTYPE      m_Type;


	// ���̴� �ڵ带 ������ ������ ������ "�ڵ�"�� ����Ʈ ���
	// ���̴� �ڵ带 ������ ������ ������ "����"�� ����Ʈ ���

	ID3DBlob*	m_pBlob;
	ID3DBlob*	m_pErrBlob;


	// �и���Ű�� �۾� ->��? ����̽��� �ٸ� �͵���
	ID3D11DeviceContext*		m_pContext;				// ���ؽ�Ʈ

public:
	SHTYPE ShaderType() {
		return m_Type;
	}

public:
	void Init_Default();


protected:
	// ������ ����̱� ������ Window�� �׳� ������ ��� ���� ���̴�.

	/******************************** ��� ���� ************************************/
	class ConstBuffer : public SmartPtr
	{
	public:
		D3D11_BUFFER_DESC tDesc;

		// ����� ��� ���� -> D11���� ���̴����� ���� ���۵�����
		// ��� ��� �ڷ������� ����ȴ�
		ID3D11Buffer*	pConstBuffer;
		UINT			iReg;

		~ConstBuffer()
		{
			if (nullptr != pConstBuffer) { pConstBuffer->Release(); };
		}
	};

private:
	std::unordered_map<std::wstring, KPtr<ConstBuffer>> m_MapConstBuffer;

public:
	template<typename BUFTYPE>
	bool CreateCB(const wchar_t* _Name, D3D11_USAGE _eUsage, UINT _iReg)
	{
		ConstBuffer* NewBuf = new ConstBuffer();

		NewBuf->tDesc.ByteWidth = sizeof(BUFTYPE);
		NewBuf->tDesc.Usage = _eUsage;
		NewBuf->tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (NewBuf->tDesc.Usage == D3D11_USAGE_DYNAMIC)
		{
			NewBuf->tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		if (false == CreateCB(NewBuf))
		{
			return false;
		}

		NewBuf->iReg = _iReg;

		m_MapConstBuffer.insert(std::unordered_map<std::wstring, KPtr<ConstBuffer>>::value_type(_Name, NewBuf));

		return true;
	}

	template<typename BUFTYPE>
	void SettingCB(const wchar_t* _Name, const BUFTYPE& _Data)
	{
		KPtr<ConstBuffer> Buf = FindCB(_Name);

		if (nullptr == Buf || Buf->tDesc.ByteWidth != sizeof(BUFTYPE))
		{
			return;
		}

		// ū �����͸� �ѹ��� �ѱ���� �ϸ� ������.
		D3D11_MAPPED_SUBRESOURCE tSub = {};
		m_pContext->Map(Buf->pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
		memcpy(tSub.pData, &_Data, sizeof(BUFTYPE));
		m_pContext->Unmap(Buf->pConstBuffer, 0);

		UpdateCB(Buf);
	}

	bool CreateCB(ConstBuffer* NewBuf);
	KPtr<ConstBuffer> FindCB(const wchar_t* _Name);


private:
	virtual void Update() = 0;
	virtual void UpdateCB(KPtr<ConstBuffer> Buf) = 0;

public:
	KShader();
	~KShader();
};

