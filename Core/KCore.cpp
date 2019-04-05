#include "KCore.h"
#include "KWindow.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Core_Class.h"
#include "ResourceManager.h"
#include "KCollision.h"
#include "SoundDevice.h"

/////////////////////////// Launcher

KCore::Launcher::Launcher() 
{

}

KCore::Launcher::~Launcher() 
{

}

/////////////////////////// Core

KCore::KCore()
{
}


KCore::~KCore()
{
}

int KCore::Loop() 
{
	MSG msg;
	while (true)
	{
		// GetMessage는 메시지가 발생하기 전까지 로직 중단시킨다.
		// 픽메시지는 메세지가 있을때만 처리하는 함수.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Progress();
		}

		if (0 == KWindow::WindowCount())
		{
			break;
		}
	}

	return 1;
}

void KCore::Progress() 
{
	TimeManager::Update();
	InputManager::Update();
	SoundDevice::Update();
	KWindow::Progress();

	InputManager::EndUpdate();
	// 데드 타임을 이용할것이다.
}

void KCore::Init_Core(HINSTANCE _Hinst, const wchar_t* _pMainWindowName, const bool& _Full, HWND _hWnd) {

	// 윈도우 초기화
	KWindow::Init(_Hinst);
	if (nullptr == _hWnd)
	{
		if (nullptr == KWindow::Create_KWindow(_pMainWindowName, _Full))
		{
			return;
		}
	}
	else {
		if (nullptr == KWindow::Create_KWindow(_pMainWindowName, _Full, _hWnd))
		{
			return;
		}
	}
	

	Core_Class::MainWindow(_pMainWindowName);
	FBXLoader::Init_FBXLoader();
	SoundDevice::Init();
	PathManager::Init();
	TimeManager::Init();
	KCollision::ColInit();
}