#pragma once
#include "Resource.h"
#include "DXContainer.h"
#include <unordered_map>

class KShader : public KResource
{
public:

protected:
	SHTYPE      m_Type;


	// 쉐이더 코드를 컴파일 했을때 나오는 "코드"의 바이트 행렬
	// 쉐이더 코드를 컴파일 했을때 나오는 "에러"의 바이트 행렬

	ID3DBlob*	m_pBlob;
	ID3DBlob*	m_pErrBlob;


	// 분리시키는 작업 ->뭘? 디바이스와 다른 것들을
	ID3D11DeviceContext*		m_pContext;				// 컨텍스트

public:
	SHTYPE ShaderType() {
		return m_Type;
	}

public:
	void Init_Default();


protected:
	// 윈도우 기반이기 때문에 Window가 그냥 값으로 들고 있을 것이다.

	/******************************** 상수 버퍼 ************************************/
	class ConstBuffer : public SmartPtr
	{
	public:
		D3D11_BUFFER_DESC tDesc;

		// 요놈이 상수 버퍼 -> D11에선 쉐이더에서 쓰일 버퍼따위도
		// 모두 요놈 자료형으로 저장된당
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

		// 큰 데이터를 한번에 넘기려고 하면 느리다.
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

