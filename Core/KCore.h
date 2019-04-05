#pragma once
#include <Windows.h>

// ����� �ϴ³� - �� ��� ��
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
		// �������� �� �� �� -> �� �۵��ϴ��� �Ǻ�
		new int;

		// �̳� ���� �ʱ�ȭ - ���μ����� �����츦 ����.
		Init_Core(_Hinst, _pMainWindowName, _Full, _hWnd);

		// �� �Ŀ� ������ ������ �����ϴ� ��
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


// ����Ǵ� �� - ���Ŀ��� �̳��� ����. ��� �ʿ�
// �׷��� �ڵ����� �̰� �������� ���ο����� �ϰ� �Ǹ� �ȿ� �ִ� �͵��� ����ȴ�.
// ���⼭ �ھ��� ��ŸƮ�� �����ְ� �ش� ������ ������ �����Ѵ�. ����, ������ ������ �����찡 ��������� ������.
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