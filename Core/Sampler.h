#pragma once
#include "Resource.h"
#include "DXContainer.h"

class Sampler : public KResource
{
private:
	D3D11_SAMPLER_DESC m_Desc;
	ID3D11SamplerState* m_pState;

public:
	ID3D11SamplerState* SamState() 
	{
		return m_pState;
	}

	// �ϴ� �׳� ����Ʈ �������� �����.
	bool Create();

	bool Create(D3D11_FILTER Filter,
		D3D11_TEXTURE_ADDRESS_MODE AddressU = D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_MODE AddressV = D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_MODE AddressW = D3D11_TEXTURE_ADDRESS_CLAMP,
		FLOAT MipLODBias = 1.0f,
		KUINT MaxAnisotropy = 0,
		D3D11_COMPARISON_FUNC ComparisonFunc = D3D11_COMPARISON_NEVER,
		KColor BorderColor = KVector4::One,
		FLOAT MinLOD = -FLT_MAX,
		FLOAT MaxLOD = FLT_MAX
		// �ּ� �κ��� �ѹ� �غ���� 
	);

	bool Set_State(D3D11_FILTER Filter = D3D11_FILTER::/*D3D11_FILTER_MIN_MAG_MIP_LINEAR*/D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	D3D11_TEXTURE_ADDRESS_MODE AddressU = D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_MODE AddressV = D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_MODE AddressW = D3D11_TEXTURE_ADDRESS_CLAMP,
	FLOAT MipLODBias = 1.0f,
	KUINT MaxAnisotropy = 0,
	D3D11_COMPARISON_FUNC ComparisonFunc = D3D11_COMPARISON_NEVER,
	KColor BorderColor = KVector4::One,
	FLOAT MinLOD = -FLT_MAX,
	FLOAT MaxLOD = FLT_MAX
	// �ּ� �κ��� �ѹ� �غ���� 
	);

public:
	void Update(unsigned int _Slot);

public:
	Sampler();
	~Sampler();
};

