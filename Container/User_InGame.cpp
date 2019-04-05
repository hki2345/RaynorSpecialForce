#include "Controll_User.h"

#include "Con_Class.h"
#include "SC2Manager.h"
#include "SC2_Force.h"

#include "Force_Unit.h"

#include <State.h>
#include <KBox_Col.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <Core_Class.h>
#include <KWindow.h>
#include <InputManager.h>
#include <TimeManager.h>
#include <Renderer_UI.h>

#include <KFont.h>
#include <SoundPlayer.h>


void Controll_User::Init_Game()
{
	m_pSound = new SoundPlayer();

	m_pFont = ResourceManager<KFont>::Find(L"Kostar");

	m_MType = MOVE_TYPE::MT_IDLE;
	m_AType = ACT_TYPE::AT_IDLE;
	m_OType = OPTI_TYPE::OT_ODIN;
	m_pUnit->Get_Component<KBox_Col>()->StayFunc(L"Game User", this, &Controll_User::Update_GameCol);
	m_pUnit->Get_Component<KBox_Col>()->ExitFunc(L"Game User", this, &Controll_User::Update_GExitCol);
	m_uConsole->one()->Active_Off();
	m_uFKey->one()->Active_Off();

	m_uConsole->alpha_value(.8f);

	m_OutSound = false;
	m_InGame = false;
	m_OutGame = false;
	Con_Class::s2_manager()->m_GameSet = 0;


	m_pUnit->maxhp(100.0f);
	m_pUnit->hp(100.0f);
	m_GameTime = .0f;
	m_iStory = 0;
	m_OpTime = 1000000.0f;
	m_fCover = .0f;


	if (nullptr == Core_Class::BGM())
	{
		KPtr<SoundPlayer> nB = state()->Create_One(L"Sound")->Add_Component<SoundPlayer>();
		nB->Play(L"Music_T01.ogg");
		nB->Loop();
		nB->Set_FadeIn();
		Core_Class::BGM(nB);
	}

	else
	{
		Core_Class::BGM()->Stop();
		Core_Class::BGM()->Play(L"Music_T01.ogg");
		Core_Class::BGM()->Loop();
		Core_Class::BGM()->Set_FadeIn();
	}
}

void Controll_User::Update_Game()
{
	Update_Story();
	if (nullptr != m_pStoryUnit)
	{
		KLOG(m_pStoryUnit->name());
		KLOG(L"Story Num: %d", m_iStory);

		if (L"QUEENCHAMBER" == m_pStoryUnit->ws_name())
		{
			m_OutGame = true;
		}

		if (true == KEY_DOWN(L"F"))
		{
			if (m_AType == Controll_User::AT_STORY)
			{
				Update_Story();
				return;
			}

			m_pSound->Play(L"UI_BnetSelect01_1.wav");

			if (L"TYCHUS" == m_pStoryUnit->ws_name())
			{
				Core_Class::BGM()->Set_FadeOut();
				m_MType = Controll_User::MT_NONE;
				m_AType = Controll_User::AT_STORY;
				m_SType = Controll_User::ST_TYCHUS;
				m_iStory = 0;
			}
			if (L"NOVA" == m_pStoryUnit->ws_name())
			{
				Core_Class::BGM()->Set_FadeOut();
				m_MType = Controll_User::MT_NONE;
				m_AType = Controll_User::AT_STORY;
				m_SType = Controll_User::ST_NOVA;
				m_iStory = 0;
			}
			if (L"COMMANDCENTER" == m_pStoryUnit->ws_name())
			{
				Core_Class::BGM()->Set_FadeOut();
				m_MType = Controll_User::MT_NONE;
				m_AType = Controll_User::AT_STORY;
				m_SType = Controll_User::ST_COMMAND;
				m_iStory = 0;
			}
			if (L"MEDIVAC" == m_pStoryUnit->ws_name())
			{
				m_pUnit->hp(100.0f);
				m_pSound->Play(L"Medivac_Heal02.ogg");
				m_pSound->Play(L"Medivac_HealLoop.mp3");
				m_pSound->Play(L"Medivac_HealStart.wav");
			}
		}
	}
}


void Controll_User::Update_Story()
{
	m_fCover += DELTATIME;

	if (3.0f < m_fCover)
	{
		if (false == m_InGame)
		{
			m_GameTime += DELTATIME;
			m_uCover->one()->Active_On();

			if (m_GameTime < 3.0f)
			{
				m_uCover->alpha_value(1.0f - m_GameTime * .33f);
			}
			else
			{
				m_InGame = true;
				m_uCover->alpha_value(1.0f);
				m_uCover->one()->Active_Off();
				m_GameTime = .0f;
			}
		}
	}


	if (true == m_OutGame)
	{
		m_GameTime += DELTATIME;
		m_uCover->one()->Active_On();

		if (false == m_OutSound)
		{
			Core_Class::BGM()->Set_FadeOut();
			m_OutSound = true;
		}

		if (m_GameTime < 3.0f)
		{
			m_uCover->alpha_value(m_GameTime * .33f);
		}
		else
		{
			Con_Class::s2_manager()->Clear_Force();
			if (false == m_pUnit->Is_HPDeath())
			{
				Con_Class::s2_manager()->m_GameSet = 1;
			}
			else
			{
				Con_Class::s2_manager()->m_GameSet = -1;
			}
		}
	}
}


void Controll_User::Update_GameCol(KCollision* _Left, KCollision* _Right)
{
	m_pStoryUnit = _Left->Get_Component<Force_Unit>();
	if (m_pStoryUnit == m_pUnit)
	{
		m_pStoryUnit = _Right->Get_Component<Force_Unit>();

		if (m_pStoryUnit == m_pUnit)
		{
			m_pStoryUnit = nullptr;
			return;
		}

		if (m_pStoryUnit == nullptr)
		{
			return;
		}

		if (L"TYCHUS" == m_pStoryUnit->ws_name() ||
			L"NOVA" == m_pStoryUnit->ws_name() ||
			L"COMMANDCENTER" == m_pStoryUnit->ws_name() ||
			L"MEDIVAC" == m_pStoryUnit->ws_name() ||
			L"QUEENCHAMBER" == m_pStoryUnit->ws_name())
		{
			return;
		}
	}


	m_pStoryUnit = nullptr;
	return;
}


void Controll_User::Update_GExitCol(KCollision* _Left, KCollision* _Right)
{
	m_pStoryUnit = nullptr;
}

void Controll_User::UIRender()
{
	float FontSize = 25.0f;
	switch (m_SType)
	{
	case Controll_User::ST_TYCHUS:
		m_uConsole->one()->Active_On();
		if (0 == m_iStory)
		{
			if (true == KEY_DOWN(L"F"))
			{
				++m_iStory;
			}
		}
		else if (1 == m_iStory)
		{
			m_pFont->Draw_Font(L"어이, 기다리느라 힘들었다고\n재미 좀 봐야하지 않겠어?",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				m_pSound->Play(L"UI_BnetSelect01_1.wav");
				++m_iStory;
			}
		}
		else if (2 == m_iStory)
		{
			m_pFont->Draw_Font(L"그 내 새 친구를 보고 싶다면\n 말만하라고.",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			Core_Class::BGM()->Stop();
			if (true == KEY_DOWN(L"F"))
			{
				m_pSound->Play(L"UI_BnetSelect01_1.wav");
				++m_iStory;
			}
		}
		else if (3 == m_iStory)
		{
			m_pFont->Draw_Font(L"얼른 저그 벌레들을 쓸어버리고\n주점에서 한 잔 하자고 친구",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				Core_Class::BGM()->Play(L"Music_T04.ogg");

				m_pSound->Play(L"UI_BnetSelect01_1.wav");
				m_OType = OPTI_TYPE::OT_ODIN;
				m_OpTime = 50.0f;

				m_SType = Controll_User::ST_NONE;
				m_AType = Controll_User::AT_IDLE;
				m_MType = Controll_User::MT_IDLE;
				m_uConsole->one()->Active_Off();
				m_iStory = 0;
			}
		}

		break;
	case Controll_User::ST_NOVA:
		m_uConsole->one()->Active_On();
		if (0 == m_iStory)
		{
			if (true == KEY_DOWN(L"F"))
			{
				++m_iStory;
			}
		}
		else if (1 == m_iStory)
		{
			m_pFont->Draw_Font(L"우리 자치령에서\n서둘러 발레리안 멩스크를\n지키라 저를 보냈는데...\n \n역시 어딜 가나\n당신이 문제의 중심이군요.",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f
					/* - m_Start->one()->Trans()->pos_local().y*/ - FontSize * .5f - FontSize * 3.0f)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				m_pSound->Play(L"UI_BnetSelect01_1.wav");
				++m_iStory;
			}
		}
		else if (2 == m_iStory)
		{
			m_pFont->Draw_Font(L"이 작전에서 당신을 도와드리죠.\n저 녀석들 화가 좀 났을 겁니다.",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f
					/* - m_Start->one()->Trans()->pos_local().y*/ - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			Core_Class::BGM()->Stop();
			if (true == KEY_DOWN(L"F"))
			{
				m_pSound->Play(L"UI_BnetSelect01_1.wav");

				Con_Class::s2_manager()->Find_Force(L"GLASSBEAD")->Set_Active(true);
				Con_Class::s2_manager()->Find_Force(L"GLASSBEAD")->playable_type(PLAYABLE_TYPE::PBT_ENEMY);
				++m_iStory;
			}
		}
		else if (3 == m_iStory)
		{
			m_pFont->Draw_Font(L"짐만 되지 마시죠.\n짐 레이너 씨.", KVector2(kwindow()->size().x * .5f,
				kwindow()->size().y * .5f/* - m_Start->one()->Trans()->pos_local().y*/ - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				Core_Class::BGM()->Play(L"nDLC01_Music_Cue02.ogg");

				m_pSound->Play(L"UI_BnetSelect01_1.wav");
				m_OType = OPTI_TYPE::OT_NOVA;
				m_OpTime = 20.0f;
				
				m_SType = Controll_User::ST_NONE;
				m_AType = Controll_User::AT_IDLE;
				m_MType = Controll_User::MT_IDLE;
				m_uConsole->one()->Active_Off();
				m_iStory = 0;
			}
		}
		break;
	case Controll_User::ST_COMMAND:
		m_uConsole->one()->Active_On();
		if (0 == m_iStory)
		{
			if (true == KEY_DOWN(L"F"))
			{
				++m_iStory;
			}
		}
		else if (1 == m_iStory)
		{
			m_pFont->Draw_Font(L"말씀하신 군락 공중 진영을\n성공적으로 진압했습니다.",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f
					/* - m_Start->one()->Trans()->pos_local().y*/ - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				Con_Class::s2_manager()->Find_Force(L"POP+STARS")->Set_Active(true);
				Con_Class::s2_manager()->Find_Force(L"POP+STARS")->playable_type(PLAYABLE_TYPE::PBT_ENEMY);

				m_pSound->Play(L"UI_BnetSelect01_1.wav");
				++m_iStory;
			}
		}
		else if (2 == m_iStory)
		{
			m_pFont->Draw_Font(L"이제 끝낼시간입니다.\n명령을 기다리겠습니다. 대장님.",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);


			Core_Class::BGM()->Stop();
			if (true == KEY_DOWN(L"F"))
			{
				Core_Class::BGM()->Play(L"Music_T16.ogg");

				m_pSound->Play(L"UI_BnetSelect01_1.wav");
				m_OType = OPTI_TYPE::OT_HYPERION;
				m_OpTime = 120.0f;

				m_SType = Controll_User::ST_NONE;
				m_AType = Controll_User::AT_IDLE;
				m_MType = Controll_User::MT_IDLE;
				m_uConsole->one()->Active_Off();
				m_iStory = 0;
			}
		}
		break;
	default:
		break;
	}
}