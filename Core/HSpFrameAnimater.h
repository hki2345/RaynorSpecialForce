#pragma once
#include "Component.h"
#include <unordered_map>
#include "SmartPtr.h"

class HSpRenderer;
class HSpFrameAnimater : public Component
{
private:
	class SpriteAni : public SmartPtr
	{
	public:
		friend HSpFrameAnimater;

	private:
		std::wstring m_AniName;
		std::wstring m_ImageName;
		size_t		 m_Start;
		size_t		 m_End;
		size_t		 m_Inter;
		float	     m_FrameSpeed;
		bool         m_bLoop;
	};

private:
	KPtr<HSpRenderer> m_SpRender;
	KPtr<SpriteAni> m_CurAni;
	size_t		 m_CurFrame;
	float	     m_CurFrameTime;
	std::unordered_map<std::wstring, KPtr<SpriteAni>> m_AniMap;

public:
	void Update();

	void SettingSpRender(KPtr<HSpRenderer> _SpRender);
	bool CreateAniT(const wchar_t* _AniName, const wchar_t* _ImageName, size_t _Start, size_t _End, float FrameSpeed = 0.1f, bool _bLoop = true);
	bool CreateAniL(const wchar_t* _AniName, const wchar_t* _ImageName, size_t _Start, size_t _End, bool _bLoop = true, float FrameSpeed = 0.1f);

	bool ChangeAni(const wchar_t* _AniName, size_t _StartIndex = 0);


public:
	HSpFrameAnimater();
	~HSpFrameAnimater();
};

