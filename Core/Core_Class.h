#pragma once
#include "DXContainer.h"
#include "KCore.h"
#include "SmartPtr.h"
#include "StateManager.h"
#include "KWindow.h"
#include "KDevice.h"
#include "State.h"
#include "KFBX.h"

// ������ ���� ���������� Ŭ�������� �����ͷ� ��� ���� - �ϳ� ������ Ŭ����
class SoundPlayer;
class Core_Class
{
public:
	friend KCore;

private:
	static KPtr<SoundPlayer> g_GameBGM;
	static KPtr<KWindow> g_MainWindow;
	static void MainWindow(const wchar_t* _Name);

public:
	static KPtr<SoundPlayer> BGM();
	static void BGM(KPtr<SoundPlayer> _Other);
	static KWindow& MainWindow();
	static StateManager& MainSceneMgr();
	static KPtr<State> MainScene();
	static KPtr<State> next_state();

	static KDevice& MainDevice();
	static ID3D11Device* PDevice();
	static ID3D11DeviceContext*	Context();

public:
	Core_Class();
	~Core_Class();
};

