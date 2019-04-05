#pragma once
#include "Resource.h"
#include "DXContainer.h"
#include "FW1CompileSettings.h"
#include "FW1FontWrapper.h"

#if X64
#if _DEBUG
#pragma comment(lib, "FW1FontWrapper_X64_DEBUG")
#else if NDEBUG
#pragma comment(lib, "FW1FontWrapper_X64_RELEASE")
#endif
#else if WIN32
#if _DEBUG
#pragma comment(lib, "FW1FontWrapper_X86_DEBUG")
#else if NDEBUG
#pragma comment(lib, "FW1FontWrapper_X86_RELEASE")
#endif
#endif // DEBUG

class KFont : public KResource
{
public:
	KFont();
	~KFont();

private:
	IFW1Factory* m_pFW1Factory;
	IFW1FontWrapper* m_pFontWrapper;

	KVector2 m_TmpWinSize;

public:
	bool Create(const wchar_t* _FontName);
	void Draw_Font(wchar_t* _pStr, KVector2 _Pos, float _fSize, KUINT _COLOR, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
};

