#pragma once
#include "StateManager.h"
#include "KDevice.h"
#include "RenderManager.h"

#include <Windows.h>

// 윈도우 창을 여러개 띄울 수있게 하며, 그 창을 해쉬로 관리
// unordered_map - 해쉬로 관리하며 따라서 순서는 없다.ㄴ
#include <unordered_map>



class KCore;
class KWindow : public Begin
{
public:
	friend KCore;

protected:
	KWindow(const wchar_t* _Name, HWND _hWnd, const bool& _Full);
	~KWindow();


	// 데이터 - 다른 윈도우와 공유될 수 있는 분모 격
private:
	static std::unordered_map<std::wstring, KPtr<KWindow>>::iterator m_KSI;
	static std::unordered_map<std::wstring, KPtr<KWindow>>::iterator m_KEI;

	static std::unordered_map<std::wstring, KPtr<KWindow>> g_WinMap;

private:
	// 이 클래스는 인스턴스를 들고 있음 -> HWND 같은 거는 데이터 영역이 아님
	// 멤버로 들고있어야 할 녀석들은 멤버로
	// 요녀석은 곧 프로세스다 보니 공통 분모임
	static HINSTANCE g_HInst;
	static void Init(HINSTANCE _HInst);

public:
	static KPtr<KWindow> Create_KWindow(const wchar_t* _Name, const bool& _Full, HWND _hWnd = nullptr);
	static KPtr<KWindow> Find_KWindow(const wchar_t* _Name);
	static void Erase_KWindow(const wchar_t* _Name);
	static void Erase_KWindow(const HWND _Name);
	static void Progress();

private:
	// 해당 윈도우의 포록시 -> 메시지 받는 거
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static size_t WindowCount()
	{
		return g_WinMap.size();
	}

	

	// 멤버들 - 이 창만이 가져야 할 정보들 

private:
	HWND m_HWnd;
	HDC m_Hdc;
	size_t m_Width;
	size_t m_Height;
	bool m_bFull;


public:

	// 다이렉트 11에서는 Full모드가 false이고 창모드가 true이다. 따라서 한번 바뀌어야 함
	bool IsFull() { return !m_bFull; }

	HWND KHwnd() { return m_HWnd; }
	HDC KHdc() { return m_Hdc; }


	KUINT width_u() { return (KUINT)m_Width; }
	KUINT height_u() { return (KUINT)m_Height; }
	size_t width_st() { return m_Width; }
	size_t height_st() { return m_Height; }
	float width_f() { return (float)m_Width; }
	float height_f() { return (float)m_Height; }


	KVector2 size() { return{ width_f(), height_f() }; }
	void size(const size_t&_X, const size_t& _Y);
	void full_size();
	RECT size_rect() 
	{
		RECT RC;
		if (nullptr != m_HWnd)
		{
			GetClientRect(m_HWnd, &RC);
		}

		return RC;
	}

public:
	void Show(int _ShowOption = SW_SHOWDEFAULT);
	void FullScr_On();
	void FullScr_Off();

private:
	void Update();
	ATOM KRegisterClass();
	BOOL Init_Instance(const bool& _Full);

public:
	StateManager statemanager;

////////////////////////////////////////////////////////// 디바이스
// 윈도우에 디바이스가 장착되는 형식
private:
	KDevice m_Device;

public:
	KDevice& Device() { return m_Device; }

public:
	bool Init_Device();
};

