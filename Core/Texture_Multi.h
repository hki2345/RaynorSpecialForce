#pragma once
#include "ResourceManager.h"
#include "Texture.h"


class Texture_Multi : public Texture
{
private:
	std::vector<KPtr<Texture>> m_TexVec;
	std::vector<ID3D11Texture2D*> m_TexVec2D;

	D3D11_USAGE					m_eUsage;
	DirectX::ScratchImage		m_Image;
	ID3D11Texture2D*			m_MultiTex;

	ID3D11ShaderResourceView*	m_pSRV;

private:
	void Create_SubMTex(D3D11_USAGE _eUsage);

public:
	// 리소스라 Create는 필요 - 비어있음
	bool Create();

public:
	template<typename... Rest>
	void Create_MultiTex(D3D11_USAGE _eUsage, Rest... _Arg)
	{
		m_eUsage = _eUsage;
		m_TexVec.reserve(sizeof...(_Arg));
		Insert_Tex(_Arg...);
	}

private:

	// 리소스 메니저에 있는 텍스쳐를 넣는다.
	template<typename... Rest>
	void Insert_Tex(const wchar_t* _TName, Rest... _Arg)
	{
		KPtr<Texture> FTex = ResourceManager<Texture>::Find(_TName);

		if (nullptr == FTex)
		{
			BBY;
		}

		m_TexVec.push_back(FTex);
		Insert_Tex(_Arg...);
	}

	void Insert_Tex()
	{
		Create_SubMTex(m_eUsage);
	}


public:
	void Update(const KUINT& _Slot);
	void Reset(const KUINT& _Slot);

public:
	Texture_Multi();
	~Texture_Multi();
};

