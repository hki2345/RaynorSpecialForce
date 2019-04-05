#include "stdafx.h"
#include "Edit_Launcher.h"
#include <Core_Class.h>
#include <KWindow.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <DebugManager.h>

#include <KFont.h>


#include <KImage.h>
#include <Texture_Multi.h>
#include <Sound.h>
#include <KVideo.h>


#include <KThread.h>
#include <Renderer_BonAni.h>
#include <Con_Class.h>



#include <SoundPlayer.h>
Edit_Launcher::Edit_Launcher()
{
}


Edit_Launcher::~Edit_Launcher()
{
}

void Edit_Launcher::Build()
{
	PathManager::Create_ForderPath(L"Shader");
	PathManager::Create_ForderPath(L"Texture");
	PathManager::Create_ForderPath(L"Video");
	PathManager::Create_ForderPath(L"Sound");
	PathManager::Create_ForderPath(L"Mesh");
	PathManager::Create_ForderPath(L"Data");
	PathManager::Create_ForderPath(L"KM3", L"Data\\KM3");
	PathManager::Create_ForderPath(L"KCA", L"Data\\KCA");
	PathManager::Create_ForderPath(L"KUD", L"Data\\KUD");
	PathManager::Create_ForderPath(L"KTD", L"Data\\KTD");
	PathManager::Create_ForderPath(L"FORCE", L"Data\\Force");
	PathManager::Create_ForderPath(L"STATE", L"Data\\State");
	PathManager::Create_ForderPath(L"SpriteBack", L"Data\\Back\\Sprite");

	// 이렇게 경로를 잡은 뒤 해야함
	Core_Class::MainWindow().Init_Device();

	// 디바이스 초기화를 하고 해야함
	ResourceManager<KImage>::All_Load();
	ResourceManager<Sound>::All_Load();
	ResourceManager<KFont>::Create(L"Kostar", L"Kostar");
	

	InputManager::Create_Command(L"DEL", VK_DELETE);
	InputManager::Create_Command(L"ESC", VK_ESCAPE);
	InputManager::Create_Command(L"RB", VK_RBUTTON);
	InputManager::Create_Command(L"LB", VK_LBUTTON);
	InputManager::Create_Command(L"Up", 'W');
	InputManager::Create_Command(L"Left", 'A');
	InputManager::Create_Command(L"Down", 'S');
	InputManager::Create_Command(L"Right", 'D');
	InputManager::Create_Command(L"E", 'E');
	InputManager::Create_Command(L"F", 'F');
	InputManager::Create_Command(L"Q", 'Q');
	InputManager::Create_Command(L"MUNLOCK", VK_MENU);
	InputManager::Create_Command(L"DEBUG", VK_LSHIFT, 'B');
	InputManager::Create_Command(L"INGAME", VK_LSHIFT, 'P');



	// 멀티 텍스쳐
	KPtr<Texture_Multi> MTex = ResourceManager<Texture_Multi>::Create(L"FB");
	KPtr<Texture_Multi> MTex2 = ResourceManager<Texture_Multi>::Create(L"FC");
	MTex->Create_MultiTex(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"char_rockL.png", L"char_rocknormal.jpg");
	MTex2->Create_MultiTex(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"char_dirtL.png", L"StoneBump.jpg");

	DebugManager::Debug_On();

	


	// SoundPlayer TT = SoundPlayer();
	// TT.Play(L"Blue Stahli - 88 Rounds Per Minute.mp3");
}