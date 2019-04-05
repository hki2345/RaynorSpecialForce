#include "Core_Class.h"
#include "KMacro.h"
#include "KWindow.h"
#include "KDevice.h"
#include "SoundPlayer.h"


KPtr<KWindow> Core_Class::g_MainWindow = nullptr;
KPtr<SoundPlayer> Core_Class::g_GameBGM = nullptr;


Core_Class::Core_Class()
{
}


Core_Class::~Core_Class()
{
}

void Core_Class::MainWindow(const wchar_t* _Name)
{
	g_MainWindow = KWindow::Find_KWindow(_Name);
	KASSERT(nullptr == g_MainWindow);
}

KWindow& Core_Class::MainWindow() {
	return *g_MainWindow;
}

StateManager& Core_Class::MainSceneMgr()
{
	return g_MainWindow->statemanager;
}

KPtr<State> Core_Class::MainScene() {
	return g_MainWindow->statemanager.cur_state();
}

KPtr<State> Core_Class::next_state() {
	return g_MainWindow->statemanager.next_state();
}

KDevice& Core_Class::MainDevice() {
	return g_MainWindow->Device();
}
ID3D11Device* Core_Class::PDevice() {
	return g_MainWindow->Device().PDevice();
}
ID3D11DeviceContext* Core_Class::Context() {
	return g_MainWindow->Device().Context();
}

KPtr<SoundPlayer> Core_Class::BGM()
{
	return g_GameBGM;
}

void Core_Class::BGM(KPtr<SoundPlayer> _Other)
{
	g_GameBGM = _Other;
}