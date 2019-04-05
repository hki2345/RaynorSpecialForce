#include "KCoreLauncher.h"
#include <Core_Class.h>
#include <KWindow.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <DebugManager.h>


#include <Force_Unit.h>
#include <Renderer_BonAni.h>

#include <KImage.h>
#include <Texture_Multi.h>
#include <Sound.h>
#include <KFont.h>
#include <KVideo.h>

#include <KThread.h>
#include <Con_Class.h>

#include "SBuilder.h"
#include "SUpdater.h"
#include "GBuilder.h"
#include "GUpdater.h"
#include "EBuilder.h"
#include "EUpdater.h"

KCoreLauncher::KCoreLauncher()
{
}


KCoreLauncher::~KCoreLauncher()
{
}

void KCoreLauncher::Build()
{
	Core_Class::MainWindow().full_size();
	Core_Class::MainWindow().Show();



	PathManager::Create_ForderPath(L"Shader");
	PathManager::Create_ForderPath(L"Texture");
	PathManager::Create_ForderPath(L"Sound");
	PathManager::Create_ForderPath(L"Mesh");
	PathManager::Create_ForderPath(L"Video");
	PathManager::Create_ForderPath(L"Data");
	PathManager::Create_ForderPath(L"KM3", L"Data\\KM3");
	PathManager::Create_ForderPath(L"KCA", L"Data\\KCA");
	PathManager::Create_ForderPath(L"KUD", L"Data\\KUD");
	PathManager::Create_ForderPath(L"KTD", L"Data\\KTD");
	PathManager::Create_ForderPath(L"FORCE", L"Data\\Force");
	PathManager::Create_ForderPath(L"STATE", L"Data\\State");
	PathManager::Create_ForderPath(L"SpriteBack", L"Data\\Back\\Sprite");

	if (false == IS_KEY(L"RB"))
	{
		InputManager::Create_Command(L"RB", VK_RBUTTON);
	}
	if (false == IS_KEY(L"LB"))
	{
		InputManager::Create_Command(L"LB", VK_LBUTTON);
	}
	if (false == IS_KEY(L"ESC"))
	{
		InputManager::Create_Command(L"ESC", VK_ESCAPE);
	}
	if (false == IS_KEY(L"DEBUG"))
	{
		InputManager::Create_Command(L"DEBUG", VK_LSHIFT, 'B');
	}

	// 이렇게 경로를 잡은 뒤 해야함
	Core_Class::MainWindow().Init_Device();
	
	ResourceManager<KImage>::All_Load();
	ResourceManager<Sound>::All_Load();
	
	ResourceManager<Changer_Animation>::All_Load();
	ResourceManager<MeshContainer>::All_Load();
	ResourceManager<KVideo>::All_Load();

	ResourceManager<KFont>::Create(L"Kostar", L"Kostar");

	// 멀티 텍스쳐
	KPtr<Texture_Multi> MTex = ResourceManager<Texture_Multi>::Create(L"FB");
	KPtr<Texture_Multi> MTex2 = ResourceManager<Texture_Multi>::Create(L"FC");
	MTex->Create_MultiTex(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"char_dirtL.png", L"char_dirtnormalL.png");
	MTex2->Create_MultiTex(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"char_rockL.png", L"char_rocknormal.jpg");
	
	Core_Class::MainSceneMgr().Create_State<SBuilder, SUpdater>(L"Start");
	Core_Class::MainSceneMgr().Create_State<GBuilder, GUpdater>(L"InGame");
	Core_Class::MainSceneMgr().Create_State<EBuilder, EUpdater>(L"End");
	Core_Class::MainSceneMgr().Change_State(L"Start");

#if _DEBUG
	DebugManager::Debug_On();
#else
	DebugManager::Debug_Off();
#endif // _DEBUG
}