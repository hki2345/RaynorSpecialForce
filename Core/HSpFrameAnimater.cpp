#include "KMacro.h"
#include "Stl_AID.h"
#include "HSpFrameAnimater.h"
#include "HSpRenderer.h"
#include "TimeManager.h"


HSpFrameAnimater::HSpFrameAnimater() : m_SpRender(nullptr)
{
}


HSpFrameAnimater::~HSpFrameAnimater()
{
}

void HSpFrameAnimater::SettingSpRender(KPtr<HSpRenderer> _SpRender)
{
	m_SpRender = _SpRender;
}

bool HSpFrameAnimater::CreateAniT(const wchar_t* _AniName, const wchar_t* _ImageName, size_t _Start, size_t _End, float _FrameSpeed /*= 0.1f*/, bool _bLoop /*= true*/) {
	return CreateAniL(_AniName, _ImageName, _Start, _End, _bLoop, _FrameSpeed);
}

bool HSpFrameAnimater::CreateAniL(const wchar_t* _AniName, const wchar_t* _ImageName, size_t _Start, size_t _End, bool _bLoop /*= true*/, float _FrameSpeed /*= 0.1f*/) {

	SpriteAni* NewAni = new SpriteAni();

	NewAni->m_AniName = _AniName;
	NewAni->m_ImageName = _ImageName;
	NewAni->m_Start = _Start;
	NewAni->m_End = _End;
	NewAni->m_bLoop = _bLoop;
	NewAni->m_FrameSpeed = _FrameSpeed;
	NewAni->m_Inter = NewAni->m_End - NewAni->m_Start + 1;
	m_AniMap.insert(std::unordered_map<std::wstring, KPtr<SpriteAni>>::value_type(_AniName, NewAni));
	return true;
}



void HSpFrameAnimater::Update() 
{
	if (nullptr != m_CurAni)
	{
		m_CurFrameTime += TimeManager::DeltaTime();
		size_t Count = (size_t)(m_CurFrameTime / m_CurAni->m_FrameSpeed);
		if (0 < Count)
		{
			m_CurFrameTime -= m_CurAni->m_FrameSpeed * Count;
		}

		if (false == m_CurAni->m_bLoop && m_CurAni->m_End <= m_CurFrame + Count)
		{
			m_CurFrame = m_CurAni->m_End;
			m_SpRender->ImageIndex(m_CurFrame);
			return;
		}
		
		m_CurFrame += Count;

		if (m_CurAni->m_End + 1 <= m_CurFrame)
		{
			m_CurFrame = m_CurAni->m_Start + ((m_CurFrame - m_CurAni->m_Start) % m_CurAni->m_Inter);
		}

		m_SpRender->ImageIndex(m_CurFrame);
	}
}

bool HSpFrameAnimater::ChangeAni(const wchar_t* _AniName, size_t _StartIndex /*= 0*/)
{

	KASSERT(nullptr == m_SpRender);

	KPtr<SpriteAni> pAni = Map_Find<KPtr<SpriteAni>>(m_AniMap, _AniName);

	if (nullptr == pAni)
	{
		KASSERT(nullptr == pAni);
		return false;
	}

	m_CurAni = pAni;
	m_CurFrame = m_CurAni->m_Start + _StartIndex;
	m_CurFrameTime = 0.0f;
	m_SpRender->Image(m_CurAni->m_ImageName.c_str());
	m_SpRender->ImageIndex(m_CurFrame);

	return true;
}