#include "KMaterial.h"
#include "Shader_Vertex.h"
#include "Shader_Pixel.h"
#include "ResourceManager.h"
#include "KMacro.h"
#include "Sampler.h"
#include "Texture.h"
#include "Texture_Multi.h"
#include "RenderTarget.h"


KMaterial::KMaterial() :  m_VTS(nullptr), m_PXS(nullptr), m_bOrigin(false)
{
}


KMaterial::~KMaterial()
{
}

KMaterial::KMaterial(const KMaterial& _Other)
	: KResource(_Other),
	m_VTS(_Other.m_VTS),
	m_PXS(_Other.m_PXS),
	m_DMS(_Other.m_DMS),
	m_HUS(_Other.m_HUS),
	m_GMS(_Other.m_GMS),
	m_Blend(_Other.m_Blend),
	m_TDVec(_Other.m_TDVec),
	m_TexMap(_Other.m_TexMap),
	m_SamMap(_Other.m_SamMap)

{


	// *this = _Mat;
}

bool KMaterial::Set_VTShader(const wchar_t* _VtxName) 
{
	m_VTS = ResourceManager<Shader_Vertex>::Find(_VtxName);
	KASSERT(nullptr == m_VTS);

	if (nullptr == m_VTS)
	{
		return false;
	}

	return true;
}

bool KMaterial::Set_PXShader(const wchar_t* _PixName) 
{
	m_PXS = ResourceManager<Shader_Pixel>::Find(_PixName);
	KASSERT(nullptr == m_PXS);

	if (nullptr == m_PXS)
	{
		return false;
	}

	return true;
}

bool KMaterial::Set_HUShader(const wchar_t * _Name)
{
	m_HUS = ResourceManager<Shader_Hul>::Find(_Name);
	KASSERT(nullptr == m_HUS);

	if (nullptr == m_HUS)
	{
		return false;
	}

	return true;
}

bool KMaterial::Set_DMShader(const wchar_t * _Name)
{
	m_DMS = ResourceManager<Shader_Domain>::Find(_Name);
	KASSERT(nullptr == m_DMS);

	if (nullptr == m_DMS)
	{
		return false;
	}

	return true;
}

bool KMaterial::Set_GMShader(const wchar_t * _Name)
{
	m_GMS = ResourceManager<Shader_GeoMetry>::Find(_Name);
	KASSERT(nullptr == m_GMS);

	if (nullptr == m_GMS)
	{
		return false;
	}

	return true;
}






bool KMaterial::Set_Blend(const wchar_t* _PixName) 
{
	m_Blend = ResourceManager<KBlend>::Find(_PixName);
	KASSERT(nullptr == m_Blend);

	if (nullptr == m_Blend)
	{
		return false;
	}

	return true;
}

void KMaterial::Update() 
{
	if (nullptr != m_VTS)	{ m_VTS->Update(); m_VTS->Set_Layout(); }

	if (nullptr != m_DMS) { m_DMS->Update(); }
	if (nullptr != m_HUS) { m_HUS->Update(); }
	if (nullptr != m_GMS) { m_GMS->Update(); }

	if (nullptr != m_PXS) { m_PXS->Update(); }
	if (nullptr != m_Blend) { m_Blend->Update(); }
}

bool KMaterial::Create() 
{
	m_bOrigin = true;
	return true;
}





void KMaterial::Set_MultiTex(const KUINT& _Slot, const wchar_t* _TexName)
{
	KPtr<Texture_Multi> FindTex = ResourceManager<Texture_Multi>::Find(_TexName);
	if (nullptr == FindTex)
	{
		BBY;
	}

	std::map<KUINT, KPtr<Texture_Multi>>::iterator FindIter = m_MTexMap.find(_Slot);

	if (FindIter == m_MTexMap.end())
	{
		m_MTexMap.insert(std::map<KUINT, KPtr<Texture_Multi>>::value_type(_Slot, FindTex));
	}
	else
	{
		FindIter->second = FindTex;
	}
}

void KMaterial::Set_Tex(const KUINT& _Slot, const wchar_t* _TexName)
{
	KPtr<Texture> FindTex = ResourceManager<Texture>::Find(_TexName);
	if (nullptr == FindTex)
	{
		BBY;
	}

	std::map<KUINT, KPtr<Texture>>::iterator FindIter =  m_TexMap.find(_Slot);

	if (FindIter == m_TexMap.end())
	{
		m_TexMap.insert( std::map<KUINT, KPtr<Texture>>::value_type(_Slot, FindTex));
	}
	else 
	{
		FindIter->second = FindTex;
	}
}


void KMaterial::Set_TargetTex(const KUINT& _Slot, const wchar_t* _TexName)
{
	KPtr<RenderTarget> FindTaget = ResourceManager<RenderTarget>::Find(_TexName);
	if (nullptr == FindTaget)
	{
		BBY;
	}

	std::map<KUINT, KPtr<Texture>>::iterator FindIter = m_TexMap.find(_Slot);

	if (FindIter == m_TexMap.end())
	{
		m_TexMap.insert(std::map<KUINT, KPtr<Texture>>::value_type(_Slot, FindTaget->target_tex()));
	}
	else
	{
		FindIter->second = FindTaget->target_tex();
	}
}

void KMaterial::Set_Sampler(const KUINT& _Slot, const wchar_t* _SmpName)
{
	KPtr<Sampler> FindSmp = ResourceManager<Sampler>::Find(_SmpName);
	if (nullptr == FindSmp)
	{
		BBY;
	}

	std::map<KUINT, KPtr<Sampler>>::iterator FindIter = m_SamMap.find(_Slot);

	if (FindIter == m_SamMap.end())
	{
		m_SamMap.insert(std::map<KUINT, KPtr<Sampler>>::value_type(_Slot, FindSmp));
	}
	else
	{
		FindIter->second = FindSmp;
	}
}



void KMaterial::Update_Tex() {

	m_TSI = m_TexMap.begin();
	m_TEI = m_TexMap.end();
	for (; m_TSI != m_TEI; ++m_TSI)
	{
		m_TSI->second->Update(m_TSI->first);
	}


	m_MTSI = m_MTexMap.begin();
	m_MTEI = m_MTexMap.end();
	for (; m_MTSI != m_MTEI; ++m_MTSI)
	{
		m_MTSI->second->Update(m_MTSI->first);
	}
}

void KMaterial::Reset_Tex() 
{
	m_TSI = m_TexMap.begin();
	m_TEI = m_TexMap.end();
	for (; m_TSI != m_TEI; ++m_TSI)
	{
		m_TSI->second->Reset(m_TSI->first);
	}


	m_MTSI = m_MTexMap.begin();
	m_MTEI = m_MTexMap.end();
	for (; m_MTSI != m_MTEI; ++m_MTSI)
	{
		m_MTSI->second->Reset(m_MTSI->first);
	}
}

void KMaterial::Update_Sam() 
{
	m_SSI = m_SamMap.begin();
	m_SEI = m_SamMap.end();
	for (; m_SSI != m_SEI; ++m_SSI)
	{
		m_SSI->second->Update(m_SSI->first);
	}
}

void KMaterial::Insert_TexData(TEX_TYPE _Type, const KUINT& _TexSlot, const wchar_t* _TexName, UINT _SmpSlot, const wchar_t* _SmpName)
{
	Tex_Data NewTexData;
	NewTexData.Type = (int)_Type;
	NewTexData.Tex_Idx = _TexSlot;
	NewTexData.Tex_Smp = _SmpSlot;
	m_TDVec.push_back(NewTexData);

	if (_Type == TEX_TYPE::TEX_TARGET)
	{
		Set_TargetTex(_TexSlot, _TexName);
	}
	else if (_Type == TEX_TYPE::TEX_MULTI)
	{
		Set_MultiTex(_TexSlot, _TexName);
	}
	else {
		Set_Tex(_TexSlot, _TexName);
	}

	Set_Sampler(_SmpSlot, _SmpName);
}


KUINT KMaterial::Set_TexData(Tex_Data* _Data) 
{
	for (size_t i = 0; i < m_TDVec.size(); i++)
	{
		_Data[i] = m_TDVec[i];
	}

	return (KUINT)m_TDVec.size();
}

KPtr<KMaterial> KMaterial::Clone() 
{
	return new KMaterial(*this);
}

std::vector<KPtr<Texture >> KMaterial::texture_vec()
{
	std::vector<KPtr<Texture>> TmpVec;

	m_TSI = m_TexMap.begin();
	m_TEI = m_TexMap.end();


	for (; m_TSI != m_TEI; ++m_TSI)
	{
		TmpVec.push_back(m_TSI->second);
	}

	return TmpVec;
}

KPtr<Texture> KMaterial::Find_Texture(const KUINT& _Index /* = 0*/)
{
	m_TSI = m_TexMap.begin();
	m_TEI = m_TexMap.end();
	
	for (; m_TSI != m_TEI; ++m_TSI)
	{
		if (_Index == m_TSI->first)
		{
			return m_TSI->second;
		}
	}

	return nullptr;
}

KPtr<Texture> KMaterial::Create_FindTexture(const KUINT& _Index/* = 0*/)
{
	KPtr<Texture> Tmp = Find_Texture(_Index);

	return Tmp;
}