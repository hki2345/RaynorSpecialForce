#include "KStreamBuffer.h"
#include "Core_Class.h"



KStreamBuffer::KStreamBuffer()
{
}


KStreamBuffer::~KStreamBuffer()
{
	if (nullptr != pBuffer)
	{
		pBuffer->Release();
	}
}


bool KStreamBuffer::Create(const KUINT& _BufSize, void* _InitData, const KUINT& _Flag)
{
	tDesc.ByteWidth = _BufSize;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = 0;
	tDesc.BindFlags = _Flag;


	if (nullptr == _InitData)
	{
		if (S_OK != Core_Class::PDevice()->CreateBuffer(&tDesc, nullptr, &pBuffer))
		{
			BBY;
			return false;
		}
	}
	else
	{
		D3D11_SUBRESOURCE_DATA Data;
		Data.SysMemSlicePitch = 0;
		Data.SysMemPitch = _BufSize;
		Data.pSysMem = _InitData;

		if (S_OK != Core_Class::PDevice()->CreateBuffer(&tDesc, &Data, &pBuffer))
		{
			BBY;
			return false;
		}
	}

	return true;
}


bool KStreamBuffer::Create(const KUINT& _BufSize, void* _InitData, const KUINT& _Flag, D3D11_USAGE _Data)
{
	tDesc.ByteWidth = _BufSize;
	tDesc.Usage = _Data;
	if (tDesc.Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		tDesc.CPUAccessFlags = 0;
	}

	tDesc.MiscFlags = 0;
	tDesc.BindFlags = _Flag;


	if (nullptr == _InitData)
	{
		if (S_OK != Core_Class::PDevice()->CreateBuffer(&tDesc, nullptr, &pBuffer))
		{
			BBY;
			return false;
		}
	}
	else
	{
		D3D11_SUBRESOURCE_DATA Data;
		Data.SysMemSlicePitch = 0;
		Data.SysMemPitch = _BufSize;
		Data.pSysMem = _InitData;

		if (S_OK != Core_Class::PDevice()->CreateBuffer(&tDesc, &Data, &pBuffer))
		{
			BBY;
			return false;
		}
	}

	return true;
}
void KStreamBuffer::Update_Data(const KUINT& _BufSize, void* _InitData)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	Core_Class::Context()->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _InitData, _BufSize);
	Core_Class::Context()->Unmap(pBuffer, 0);
}


// ��Ʈ�� ���� ������Ʈ �κ�
// ��Ʈ�� ���۶� �ص� ���۴� �׳� ������ ����
void KStreamBuffer::Update()
{
	KUINT BuffOffset = 0;
	Core_Class::Context()->SOSetTargets(1, &pBuffer, &BuffOffset);
}

// ������ ���Դٰ� �װ� ������ ��ƼŬ �ٹ̴� ��������
// �ѹ��� ��Ʈ�� ������ �ʿ��ϤӴ�.
void KStreamBuffer::Reset()
{
	KUINT BuffOffset = 0;
	ID3D11Buffer* tBuffer[1] = { nullptr, };
	Core_Class::Context()->SOSetTargets(1, tBuffer, &BuffOffset);
}