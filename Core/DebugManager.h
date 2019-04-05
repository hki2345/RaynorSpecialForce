#pragma once
#include "DXContainer.h"
#include "FW1FontWrapper.h"
#include <list>


#define DRAW_FONT	DebugManager::Draw_Font
#define DRAW_LOG	DebugManager::Draw_Log
#define KLOG		DRAW_LOG


class DebugManager
{
public:
	DebugManager();
	~DebugManager();

	class LogMsg
	{
	public:
		std::wstring Msg;
		KVector m_COLOR;
	};

private:
	friend class KWindow;

	static bool m_bDebug;
	static KColor m_LogColor;
	static float m_LogSize;
	static std::wstring	Messege;
	/*static std::list<std::wstring>::iterator m_S_Log;
	static std::list<std::wstring>::iterator m_E_Log;
	static std::list<std::wstring> m_LogList;*/

	static std::list<LogMsg>::iterator m_S_Log;
	static std::list<LogMsg>::iterator m_E_Log;
	static std::list<LogMsg> m_LogList;

	
public:
	static void Switch_Debug()
	{
		if (true == m_bDebug)
		{
			m_bDebug = false;
		}
		else
		{
			m_bDebug = true;
		}
	}

	static void Debug_On()
	{
		m_bDebug = true;
	}
	static void Debug_Off()
	{
		m_bDebug = false;
	}
	static bool Is_Debug() 
	{
		return m_bDebug;
	}

	static void Draw_Rect(const KRect& _Rect, float _Border = 1);
	static void Draw_Rect(KVector2 _Pos, KVector2 _Size, float _Border = 1);
	static void Draw_Font(wchar_t* _pStr, KVector2 _Pos, float _fSize, KVector4 _COLOR = KColor::White, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
	static void Draw_Log(const wchar_t* const _Str, ...);

public:
	static void Update();
	static void Logging();
	static void Targetting();

};

