#include "EUpdater.h"
#include <Core_Class.h>
#include <KVideo.h>
#include <SoundPlayer.h>
#include <InputManager.h>


EUpdater::EUpdater()
{
}


EUpdater::~EUpdater()
{
}

void  EUpdater::Start_State()
{
	Core_Class::BGM()->Stop();
	 
	m_bVideo = false;
	m_fUTime = .0f;
	Core_Class::MainDevice().Clear_Target();
	Core_Class::MainDevice().Present();

	state()->Set_Render(false);
	TVideo = ResourceManager<KVideo>::Find(L"RaynorLUV.avi");

	RECT rc;
	GetClientRect(kwindow()->KHwnd(), &rc);
	TVideo->UpdateVideoWindow(&rc);

	// TVideo->HandleGraphEvent(OnGraphEvent);
}

void  EUpdater::Update_State()
{
	m_fUTime += DELTATIME;

	if (m_fUTime > 3.0f && false == m_bVideo)
	{
		TVideo->Play();
		m_bVideo = true;

		m_fUTime = .0f;
	}
	
	if (true == m_bVideo && (PlaybackState::STATE_RUNNING != TVideo->State() || true == KEY_DOWN(L"ESC")))
	{
		TVideo = ResourceManager<KVideo>::Find(L"RaynorLUV.avi");
		TVideo->Stop();
		Core_Class::MainSceneMgr().Change_State(L"Start");

	}
}