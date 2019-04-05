#pragma once
#include "Resource.h"
#include "DXContainer.h"
#include "Texture.h"
#include "Sampler.h"

#include <vector>

class Texture;
class Sampler;
class KImage : public KResource
{
private:
	KPtr<Texture> m_Tex;
	KPtr<Sampler> m_Sam;
	std::vector<KVector4> m_UVVec;

	KVector2 m_CutIndex;

public:
	KVector2 CutIndex() {
		return m_CutIndex;
	}

	size_t CutCount() {
		return m_UVVec.size();
	}

	const KVector4& Uv(size_t _Index = 0) 
	{
		return m_UVVec[_Index];
	}


public:
	KPtr<Texture> texture();
	KPtr<Sampler> sampler();

public:
	// �̹����� �ڸ��� �Լ��Դϴ�.
	void Split(size_t _X, size_t _Y);

	// ���ͷ� ������ ���������� ���������� ���� �˷��ش�.
	size_t Split(KVector4 _Vec);

public:
	bool Load(const wchar_t* _SmpName = L"DefaultSmp");
	bool Load(const int& _CutX, const int& _CutY, const wchar_t* _SmpName = L"DefaultSmp");

public:
	KImage();
	~KImage();
};

