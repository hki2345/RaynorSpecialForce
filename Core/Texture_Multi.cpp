#include "Texture_Multi.h"
#include "KMacro.h"
#include "Core_Class.h"



Texture_Multi::Texture_Multi()
{
}


Texture_Multi::~Texture_Multi()
{
	if (nullptr != m_pSRV)
	{
		m_pSRV->Release();
	}

	if (nullptr != m_MultiTex)
	{
		m_MultiTex->Release();
	}
}



void Texture_Multi::Create_SubMTex(D3D11_USAGE _eUsage)
{
	D3D11_TEXTURE2D_DESC tDecs = {};


	// 텍스쳐 배열 돌면서 얻은 픽셀로 새로운 텍스쳐 찍어냄
	for (size_t i = 0; i < m_TexVec.size(); i++)
	{
		if (nullptr == m_TexVec[i]->texture2D())
		{
			BBY;
		}

		m_TexVec[i]->texture2D()->GetDesc(&tDecs);

		tDecs.BindFlags = 0;
		tDecs.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		tDecs.Usage = D3D11_USAGE_STAGING;

		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = m_TexVec[i]->m_Image.GetPixels();
		tSub.SysMemPitch = (KUINT)m_TexVec[i]->m_Image.GetImages()->rowPitch;
		tSub.SysMemSlicePitch = (KUINT)m_TexVec[i]->m_Image.GetImages()->slicePitch;

		ID3D11Texture2D* pCopyTex = nullptr;
		if (S_OK != Core_Class::PDevice()->CreateTexture2D(&tDecs, &tSub, &pCopyTex))
		{
			BBY;
		}

		m_TexVec2D.push_back(pCopyTex);
	}


	m_TexVec[0]->texture2D()->GetDesc(&tDecs);
	D3D11_TEXTURE2D_DESC tVecDesc = {};
	tVecDesc.Width = tDecs.Width;
	tVecDesc.Height = tDecs.Height;
	tVecDesc.MipLevels = tDecs.MipLevels;
	tVecDesc.Format = tDecs.Format;
	tVecDesc.ArraySize = (KUINT)m_TexVec2D.size();
	tVecDesc.Usage = _eUsage;
	tVecDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tVecDesc.SampleDesc.Count = 1;
	tVecDesc.SampleDesc.Quality = 0;

	m_MultiTex = nullptr;

	if (S_OK != Core_Class::PDevice()->CreateTexture2D(&tVecDesc, nullptr, &m_MultiTex))
	{
		BBY;
	}

	for (KUINT i = 0; i < m_TexVec.size(); i++)
	{
		D3D11_MAPPED_SUBRESOURCE tSub = {};

		Core_Class::Context()->Map(m_TexVec2D[i], 0, D3D11_MAP_READ, 0, &tSub);

		Core_Class::Context()->UpdateSubresource(m_MultiTex
			, D3D11CalcSubresource(0, i, tVecDesc.MipLevels)
			, NULL
			, tSub.pData
			, tSub.RowPitch
			, tSub.DepthPitch
		);

		Core_Class::Context()->Unmap(m_TexVec2D[i], 0);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC tViewDesc = {};
	tViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	tViewDesc.Texture2DArray.ArraySize = (KUINT)m_TexVec2D.size();
	tViewDesc.Texture2DArray.FirstArraySlice = 0;
	tViewDesc.Texture2DArray.MipLevels = tDecs.MipLevels;
	tViewDesc.Texture2DArray.MostDetailedMip = 0;

	if (S_OK != Core_Class::PDevice()->CreateShaderResourceView(m_MultiTex, &tViewDesc, &m_pSRV))
	{
		BBY;
	}

	// 이미 로드된 텍스쳐에서 정보를 얻어온다.
	if (S_OK != DirectX::CaptureTexture(Core_Class::PDevice(), Core_Class::Context(), m_MultiTex, m_Image))
	{
		BBY;
	}

	for (size_t i = 0; i < m_TexVec2D.size(); i++)
	{
		m_TexVec2D[i]->Release();
	}
}


bool Texture_Multi::Create()
{
	return true;
}



void Texture_Multi::Update(const KUINT& _Slot)
{
	Core_Class::Context()->VSSetShaderResources(_Slot, 1, &m_pSRV);
	Core_Class::Context()->PSSetShaderResources(_Slot, 1, &m_pSRV);
}
void Texture_Multi::Reset(const KUINT& _Slot)
{
	ID3D11ShaderResourceView* Reset = nullptr;
	Core_Class::Context()->VSSetShaderResources(_Slot, 1, &Reset);
	Core_Class::Context()->PSSetShaderResources(_Slot, 1, &Reset);
}