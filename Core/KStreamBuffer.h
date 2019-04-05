#pragma once
#include "Resource.h"
#include "DXContainer.h"



// ��ƼŬ ���̴��� ��Ʈ�� �������� ������ ���ؤ� �������
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

