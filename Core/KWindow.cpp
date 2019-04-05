#include "KWindow.h"
#include "Stl_AID.h"

#include "KMacro.h"

#include "KVideo.h"
#include "ResourceManager.h"
#include "InputManager.h"


#pragma comment(lib, "Gdi32")
/*************** Static ************/

HINSTANCE KWindow::g_HInst = nullptr;


std::unordered_map<std::wstring, KPtr<KWindow>>::iterator KWindow::m_KSI;
std::unordered_map<std::wstring, KPtr<KWindow>>::iterator KWindow::m_KEI;

std::unordered_map<std::wstring, KPtr<KWindow>> KWindow::g_WinMap;



// 생성자에서 윈도우 관련 것들이 실행 된다.
KWindow::KWindow(const wchar_t* _Name, HWND _hWnd, const bool& _Full) : Begin(_Name), m_HWnd(nullptr), statemanager(this), m_Device(this), m_bFull(true)
{
	if (nullptr == _hWnd)
	{
		KRegisterClass();
		if (FALSE == Init_Instance(_Full))
		{
			BBY;
		}

		m_Hdc = GetDC(m_HWnd);
		return;
	}
	else
	{
		m_HWnd = _hWnd;

		RECT RC;
		GetClientRect(_hWnd, &RC);

		m_Width = RC.right;
		m_Height = RC.bottom;

		m_Hdc = GetDC(m_HWnd);
	}
}


KWindow::~KWindow()
{
}


/*************** Load_ByThread ************/


void KWindow::Init(HINSTANCE _HInst) 
{
	g_HInst = _HInst;
}


KPtr<KWindow> KWindow::Create_KWindow(const wchar_t* _Name, const bool& _Full, HWND _hWnd)
{
	// 스마트 포인터를 사용하기 때문에 이제 왠만
	KPtr<KWindow> Win = Map_Find<KPtr<KWindow>>(g_WinMap, _Name);

	if (nullptr != Win)
	{
		return Win;
	}

	KWindow* pNewWindow = nullptr;

	pNewWindow = new KWindow(_Name, _hWnd, _Full);
	
	if (nullptr == pNewWindow->m_HWnd)
	{
		delete pNewWindow;
		return nullptr;
	}

	pNewWindow->Set_Type();
	

	g_WinMap.insert(std::unordered_map<std::wstring, KPtr<KWindow>>::value_type(_Name, pNewWindow));

	return pNewWindow;
}

KPtr<KWindow> KWindow::Find_KWindow(const wchar_t* _Name) 
{
	return Map_Find<KPtr<KWindow>>(g_WinMap, _Name);
}

void KWindow::Erase_KWindow(const wchar_t* _Name)
{
	KPtr<KWindow> Win = Map_Find<KPtr<KWindow>>(g_WinMap, _Name);

	KASSERT(nullptr == Win);

	if (nullptr == Win)
	{
		return;
	}

	std::wstring Name = Win->name();
	Map_Erase(g_WinMap, Name.c_str());
}


void KWindow::Erase_KWindow(const HWND _Hwnd)
{
	m_KSI = g_WinMap.begin();
	m_KEI = g_WinMap.end();

	for (; m_KSI != m_KEI; ++m_KSI)
	{
		if (m_KSI->second->m_HWnd == _Hwnd)
		{
			Map_Erase(g_WinMap, m_KSI->first);
			return;
		}
	}

	// 여기선 지우지 못하면 터짐 -> 없다는 소리니깡
	BBY;
}

void KWindow::Progress()
{
	m_KSI = g_WinMap.begin();
	m_KEI = g_WinMap.end();

	for (; m_KSI != m_KEI; ++m_KSI)
	{
		m_KSI->second->Update();
	}
}

// 브러쉬 색상 http://soen.kr/lecture/win32api/lec6/lec6-1-2.htm
// 색채우기 http://seowh119.wixsite.com/petronas/single-post/2014/11/11/%EA%B2%8C%EC%9E%84-%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D-141111-API-%EC%83%89%EC%9D%B4-%EB%93%A4%EC%96%B4%EA%B0%84-%EC%82%AC%EA%B0%81%ED%98%95-%EA%B7%B8%EB%A6%AC%EA%B8%B0
// https://docs.microsoft.com/en-us/previous-versions/y11315ch(v=vs.140)
// 모든 여러개 만들어진 윈도우가 공유하는 윈도우 프록시 이다.
LRESULT CALLBACK KWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		KPtr<KVideo> TV = ResourceManager<KVideo>::Find(L"RaynorLUV.avi");

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		if (nullptr != TV && TV->State() != STATE_NO_GRAPH && TV->HasVideo())
		{
			// The player has video, so ask the player to repaint. 
			TV->Repaint(hdc);
		}
		else
		{
			HBRUSH MyBrush = CreateSolidBrush(RGB(.0f, .0f, .0f));
			FillRect(hdc, &ps.rcPaint, MyBrush);
		}
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		Erase_KWindow(hWnd);
	}
	break;

	// 아오 마우스 휠은 메시지로 받는다 ㅋㅋㅋㅋㅋㅋ
	case WM_MOUSEWHEEL:
	{
		InputManager::Receive_Wheel((int)HIWORD(wParam));
	}	
	break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}




void KWindow::Show(int _ShowOption) {
	ShowWindow(m_HWnd, _ShowOption);
	UpdateWindow(m_HWnd);
}

void KWindow::FullScr_On() {
	m_bFull = false;
}

void KWindow::FullScr_Off() {
	m_bFull = true;
}

// 전체 창모드 참고
// https://blog.naver.com/aaa4379/221131333295
// GetDeviceCaps - 장치의 해상도
// https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-getdevicecaps

// GetSystemMetrics - 설정된 해상도
// http://blog.daum.net/victory6x/1046509

void KWindow::full_size()
{
	m_Width = (size_t)GetSystemMetrics(SM_CXSCREEN);
	m_Height = (size_t)GetSystemMetrics(SM_CYSCREEN);


	RECT Rc = { 0, 0, (long)m_Width, (long)m_Height };
	AdjustWindowRect(&Rc, WS_POPUP, false);
	SetWindowPos(m_HWnd, nullptr, 0, 0, Rc.right - Rc.left
		, Rc.bottom - Rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void KWindow::size(const size_t&_X, const size_t& _Y)
{
	m_Width = _X;
	m_Height = _Y;

	RECT Rc = { 0, 0, (long)_X, (long)_Y };
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_HWnd, nullptr, 0, 0, Rc.right - Rc.left
		, Rc.bottom - Rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void KWindow::Update() 
{
	statemanager.Progress();

	if (true == m_Device.IsInit())
	{
		// m_Device.Clear_Target();
		statemanager.Render();
		// m_Device.Present();
	}

	// 충돌
	statemanager.NextProgress();
	statemanager.Release();
}


ATOM KWindow::KRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_HInst;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = name();
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}


BOOL KWindow::Init_Instance(const bool& _Full /*= false*/)
{
	// 테두리 없는 윈도우를 만들고 싶다면 WS_OVERLAPPEDWINDOW WS_POPUP
	// 다른 걸로 넣어줘야 한다.

	if (false == _Full)
	{
		m_HWnd = CreateWindowW(name(), L"Title", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, g_HInst, nullptr);
	}
	else
	{
		m_HWnd = CreateWindowW(name(), L"Title", WS_POPUP,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, g_HInst, nullptr);
	}

	if (!m_HWnd)
	{
		return FALSE;
	}

	return TRUE;
}

bool KWindow::Init_Device() 
{
	bool Return = m_Device.Init();

	// m_Device.Init_BasicFigure2D();
	m_Device.Init_BasicFigure3D();

	return Return;
}