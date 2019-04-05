#include "KImage.h"
#include "Texture.h"
#include "Sampler.h"
#include "ResourceManager.h"
#include "KMacro.h"


KImage::KImage() 
{
	m_UVVec.push_back({ 0.0f, 0.0f, 1.0f, 1.0f });
}

KImage::~KImage()
{
}

KPtr<Texture> KImage::texture() { return m_Tex; }
KPtr<Sampler> KImage::sampler() { return m_Sam; }

bool KImage::Load(const wchar_t* _SmpName /*= L"DefaultSmp"*/)
{
	m_Tex = ResourceManager<Texture>::Find(FileNameExt());

	if (nullptr == m_Tex)
	{
		m_Tex = ResourceManager<Texture>::Load(FileForder(), FileNameExt());
		KASSERT(nullptr == m_Tex);
	}

	m_Sam = ResourceManager<Sampler>::Find(_SmpName);

	if (nullptr == m_Sam)
	{
		m_Sam = ResourceManager<Sampler>::Create(_SmpName);
		KASSERT(nullptr == m_Tex);
	}

	return true;
}


bool KImage::Load(const int& _CutX, const int& _CutY, const wchar_t* _SmpName /*= L"DefaultSmp"*/)
{
	Load(_SmpName);
	Split(_CutX, _CutY);
	return true;
}

void KImage::Split(size_t _X, size_t _Y)
{
	if (0 >= _X)
	{
		KASSERT(0 >= _X);
	}

	if (0 >= _Y)
	{
		KASSERT(0 >= _Y);
	}

	m_CutIndex.x = (float)_X;
	m_CutIndex.y = (float)_Y;

	// 0,0,1,1 ³¯·È´Ù.
	m_UVVec.clear();

	KVector4 SplitVec;

	SplitVec.z = 1.0f / (float)_X;
	SplitVec.w = 1.0f / (float)_Y;

	for (size_t Y = 0; Y < (size_t)_Y; ++Y)
	{
		for (size_t X = 0; X < (size_t)_X; ++X)
		{
			SplitVec.x = SplitVec.z * X;
			SplitVec.y = SplitVec.w * Y;
			m_UVVec.push_back(SplitVec);
		}
	}
}

size_t KImage::Split(KVector4 _Vec) 
{
	m_UVVec.push_back(_Vec);
	return m_UVVec.size() - 1;
}