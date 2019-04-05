#pragma once
#include <Windows.h>

// 디버그 하는놈 - 릭 잡는 놈
#include <crtdbg.h>

#include "SmartPtr.h"
#include "KMacro.h"

class KCore
{
public: 
	class Launcher : public SmartPtr
	{
	public:
		virtual void Build() = 0;

	protected:
		Launcher();
		virtual ~Launcher() = 0;
	};

private:
	friend int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPWSTR    lpCmdLine,
		_In_ int       nCmdShow);

public:
	template<typename T>
	static int Start(HINSTANCE _Hinst, const wchar_t* _pMainWindowName, const bool& _Full, HWND _hWnd = nullptr)
	{
		// 인위적이 릭 발 생 -> 잘 작동하는지 판별
		new int;

		// 이놈 먼저 초기화 - 프로세스와 윈도우를 띄운다.
		Init_Core(_Hinst, _pMainWindowName, _Full, _hWnd);

		// 그 후에 빌더의 내용의 실행하는 식
		T m_pBuilder = T();
		m_pBuilder.Build();


		return 1;
	}


private:
	static void Init_Core(HINSTANCE _Hinst, const wchar_t* _pMainWindowName, const bool& _Full, HWND _hWnd = nullptr);
	static int Loop();

public:
	static void Progress();

private:
	KCore();
	~KCore();
};


// 연계되는 놈 - 런쳐에서 이놈을 쓴다. 헤더 쪽에
// 그러면 자동으로 이게 실질적인 메인역할을 하게 되며 안에 있는 것들이 실행된다.
// 여기서 코어의 스타트를 끊어주고 해당 빌더의 내용일 실행한다. 물론, 빌더의 내용은 윈도우가 만들어지고 나서다.
// Start() -> Init() -> 
#pragma region CORESTMACRO
#define CORESTART(BUILDER, WINNAME, FULL) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); \
	KCore::Start<BUILDER>(hInstance, WINNAME, FULL); \
	KCore::Loop(); \
	return 0; \
}
#pragma endregion