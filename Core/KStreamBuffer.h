#pragma once
#include "Resource.h"
#include "DXContainer.h"



// 파티클 쉐이더를 스트림 형식으로 보내기 위해ㅔ 만들어짐
class KStreamBuffer : public KResource
{
private:
	D3D11_BUFFER_DESC	tDesc;
	ID3D11Buffer*		pBuffer;
	UINT				iReg;


public:
	ID3D11Buffer* buffer()
	{
		return pBuffer;
	}

public:
	bool Create(const KUINT& _BufSize, void* _InitData, const KUINT& _Flag);
	bool Create(const KUINT& _BufSize, void* _InitData, const KUINT& _Flag, D3D11_USAGE _Data);
	void Update_Data(const KUINT& _BufSize, void* _InitData);



	void Update();
	void Reset();

public:
	KStreamBuffer();
	~KStreamBuffer();
};

