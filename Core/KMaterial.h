#pragma once
#include "Resource.h"
#include "Shader_Vertex.h"
#include "Shader_Pixel.h"

#include "Shader_GeoMetry.h"
#include "Shader_Domain.h"
#include "Shader_Hul.h"

#include "KBlend.h"
#include <vector>
#include <map>

// �����̶�� �ϴ� �༮�� ������̴�.
enum TEX_TYPE 
{
	TEX_COLOR,
	TEX_BUMP,
	TEX_SPEC,
	TEX_EMIS,
	TEX_TARGET,
	TEX_MULTI,
};

class Tex_Data
{
public:
	int Type;
	int Tex_Idx;
	int Tex_Smp;
	int Temp;

public:
	Tex_Data() : Tex_Idx(-1), Tex_Smp(-1), Type(-1) {}
};





// � ���̴��� �ؽ�ó�� ����ϴ� �༮�ΰ�?
// �ؽ�ó�� ����ϸ� � �ؽ�ó�� ����ϴ°�?
class Texture;
class Sampler;
class Texture_Multi;
class KMaterial : public KResource
{
	friend class Renderer;
	friend class Light;
	friend class Camera;

public:
	KMaterial();
	~KMaterial();

private:
	KMaterial(const KMaterial& _Other);

private:
	bool m_bOrigin;
	KPtr<Shader_Vertex> m_VTS;
	KPtr<Shader_Pixel> m_PXS;

	KPtr<Shader_Hul>	m_HUS;
	KPtr<Shader_Domain> m_DMS;
	KPtr<Shader_GeoMetry>	m_GMS;
	KPtr<KBlend> m_Blend;

public:
	KPtr<Shader_Vertex> VShader() {
		return m_VTS;
	}

	KPtr<Shader_Pixel> PShader() {
		return m_PXS;
	}

public:
	bool Set_VTShader(const wchar_t* _VtxName);
	bool Set_PXShader(const wchar_t* _PixName);
	bool Set_HUShader(const wchar_t* _VtxName);
	bool Set_DMShader(const wchar_t* _PixName);
	bool Set_GMShader(const wchar_t* _VtxName);

	bool Set_Blend(const wchar_t* _PixName);

public:
	bool Create();
	void Update();

	// �� �ؽ�ó�� ��� ������ ����ϴ���?
	// � �ؽ�ó��?

	// �� ���÷��� ��� ������ ����ϴ���?
	// � ���÷���?

	// ���� ���⿡ ���� ���̽� �ɼ��� �����ϴ� ���̴�.
private:
	std::vector<Tex_Data> m_TDVec;

public:
	void Insert_TexData(TEX_TYPE _Type, const KUINT& _TexSlot, const wchar_t* _TexName, UINT _SmpSlot = 0, const wchar_t* _SmpName = L"DefaultSmp");
private:
	std::map<unsigned int, KPtr<Texture>>::iterator m_TSI;
	std::map<unsigned int, KPtr<Texture>>::iterator m_TEI;
	std::map<unsigned int, KPtr<Texture>> m_TexMap;

	std::map<unsigned int, KPtr<Texture_Multi>>::iterator m_MTSI;
	std::map<unsigned int, KPtr<Texture_Multi>>::iterator m_MTEI;
	std::map<unsigned int, KPtr<Texture_Multi>> m_MTexMap;

	std::map<unsigned int, KPtr<Sampler>>::iterator m_SSI;
	std::map<unsigned int, KPtr<Sampler>>::iterator m_SEI;
	std::map<unsigned int, KPtr<Sampler>> m_SamMap;


public:
	std::vector<KPtr<Texture >> texture_vec();
	void Set_MultiTex(const KUINT& _Slot, const wchar_t* _TexName);
	void Set_Tex(const KUINT& _Slot, const wchar_t* _TexName);
	void Set_Sampler(const KUINT& _Slot, const wchar_t* _SmpName);
	void Set_TargetTex(const KUINT& _Slot, const wchar_t* _TexName);
	KUINT Set_TexData(Tex_Data* _Data);

public:
	void Update_Tex();
	void Update_Sam();


	// Ÿ������ �ϸ� ������ �ʿ�
	void Reset_Tex();
	KPtr<Texture> Find_Texture(const KUINT& _Index = 0);
	KPtr<Texture> Create_FindTexture(const KUINT& _Index = 0);

public:
	KPtr<KMaterial> Clone();
};

