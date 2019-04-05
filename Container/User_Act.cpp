#include "Controll_User.h"
#include "Force_Unit.h"

#include <State.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <TimeManager.h>

#include "Controll_Medivac.h"

#include "Con_Class.h"
#include "Controll_Nova.h"
#include "Controll_Odin.h"
#include "Controll_Hyperion.h"
#include "OBJ_Bomb.h"

#include <Core_Class.h>
#include <SoundPlayer.h>

#include <Renderer_AniEffect.h>

void Controll_User::Update_MIDLE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::STAND01);

	if (true == KEY_DOWN(L"MFOR") || true == KEY_DOWN(L"MBACK") || 
		true == KEY_DOWN(L"MLEFT") || true == KEY_DOWN(L"MRIGHT"))
	{
		m_MType = Controll_User::MT_MOVE;
	}
}
void Controll_User::Udpate_DEATH()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::DEATH);
}
void Controll_User::Update_MOVE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::WALK01);
	if (m_MSound == false &&
		(m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 4 ||
			m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 15))
	{
		m_pSound->Play(L"Marine_Death_Bodyfall_A_01.wav", .1f);
		m_MSound = true;
	}
	else if ((m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 5 ||
		m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 16))
	{
		m_MSound = false;
	}


	if (true == KEY_DOWN(L"RUN"))
	{
		m_MType = Controll_User::MT_RUN;
		return;
	}

	KPtr<TransPosition> TT = one()->Trans();
	if (true == KEY_PRESS(L"MFOR"))
	{
		TT->Moving(TT->forward_local() * m_pUnit->linear_speed() * DELTATIME);
		m_RenderRot.y = m_PlayRot.y + KPI;
	}
	if (true == KEY_PRESS(L"MBACK"))
	{
		TT->Moving(TT->back_local() * m_pUnit->linear_speed() * DELTATIME);
		m_RenderRot.y = m_PlayRot.y + KPI;
	}
	if (true == KEY_PRESS(L"MLEFT"))
	{
		TT->Moving(TT->left_local() * m_pUnit->linear_speed() * DELTATIME);

	}
	if (true == KEY_PRESS(L"MRIGHT"))
	{
		TT->Moving(TT->right_local() * m_pUnit->linear_speed() * DELTATIME);
	}

	// 종료
	if (true == KEY_UNPRESS(L"MFOR") && true == KEY_UNPRESS(L"MBACK") &&
		true == KEY_UNPRESS(L"MLEFT") && true == KEY_UNPRESS(L"MRIGHT"))
	{
		m_MType = Controll_User::MT_IDLE;
		return;
	}

	KVector TVec = TT->pos_local();
	TVec.y = m_pUnit->terrain()->Y_Terrain(TVec);
	TT->pos_local(TVec);
}


void Controll_User::Update_RUN()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::WALK01);
	if (m_MSound == false &&
		(m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 4 ||
		m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 15))
	{
		m_pSound->Play(L"Marine_Death_Bodyfall_A_01.wav", .1f);
		m_MSound = true;
	}
	else if ((m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 5 ||
		m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 16))
	{
		m_MSound = false;
	}


	float RunSpd = 1.5f;
	if (true == KEY_UNPRESS(L"RUN"))
	{
		m_MType = Controll_User::MT_MOVE;
		return;
	}


	KPtr<TransPosition> TT = one()->Trans();
	if (true == KEY_PRESS(L"MFOR"))
	{
		TT->Moving(TT->forward_local() * m_pUnit->linear_speed() * DELTATIME * RunSpd);
	}
	if (true == KEY_PRESS(L"MBACK"))
	{
		TT->Moving(TT->back_local() * m_pUnit->linear_speed() * DELTATIME* RunSpd);
	}
	if (true == KEY_PRESS(L"MLEFT"))
	{
		TT->Moving(TT->left_local() * m_pUnit->linear_speed() * DELTATIME * RunSpd);
	}
	if (true == KEY_PRESS(L"MRIGHT"))
	{
		TT->Moving(TT->right_local() * m_pUnit->linear_speed() * DELTATIME * RunSpd);
	}

	// 종료
	if (true == KEY_UNPRESS(L"MFOR") && true == KEY_UNPRESS(L"MBACK") &&
		true == KEY_UNPRESS(L"MLEFT") && true == KEY_UNPRESS(L"MRIGHT"))
	{
		m_MType = Controll_User::MT_IDLE;
		return;
	}

	KVector TVec = TT->pos_local();
	TVec.y = m_pUnit->terrain()->Y_Terrain(TVec);
	TT->pos_local(TVec);

	m_RenderRot.y = m_PlayRot.y + KPI;
}



void Controll_User::Update_AIDLE()
{
	if (Controll_User::AT_DEATH == m_AType || .0f > m_pUnit->hp())
	{
		m_AType = Controll_User::AT_DEATH;
		return;
	}

	if (true == KEY_PRESS(L"SHOT"))
	{
		m_UTime = .0f;
		m_AType = Controll_User::AT_ATTACK;
		m_ASound = false;
	}
	if (true == KEY_DOWN(L"BOMB"))
	{
		if (m_FiATime > m_FiTime)
		{
			m_AType = Controll_User::AT_BOMB;
			m_FiATime = .0f;
			m_ASound = false;
		}
	}
	if (true == KEY_DOWN(L"HEAL"))
	{
		if (m_MeATime > m_MeTime)
		{
			m_AType = Controll_User::AT_HEAL;
			m_MeATime = .0f;
		}
	}
	if (true == KEY_DOWN(L"OPTI"))
	{
		if (m_OpATime > m_OpTime)
		{
			Core_Class::BGM()->Set_FadeOut();
			m_AType = Controll_User::AT_OPTI;
			m_OpATime = .0f;
		}
	}
}
void Controll_User::Update_ATTACK()
{
	m_RenderRot.y = m_PlayRot.y + KPI;
	m_UTime += DELTATIME;

	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK01);



	if (true == KEY_UNPRESS(L"LB"))
	{
		m_AType = Controll_User::AT_IDLE;
		return;
	}


	if (m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 393 && m_ASound == false)
	{
		m_pSound->Play(L"Marine_AttackLaunch0.wav", .5f);
		m_ASound = true;

		if (nullptr == m_pFocusUnit)
		{
			return;
		}

		m_OpATime += 3.0f;
		m_pFocusUnit->Damage(4.0f);


		KVector Temp = m_pFocusUnit->one()->Trans()->pos_local();

		float X = Temp.x + m_pFocusUnit->scale_unit().x * .5f;
		float Y = Temp.y + m_pFocusUnit->scale_unit().y;

		X = KMath::random_f(X + .25f, Temp.x - .25f);
		Y = KMath::random_f(Y + .25f, Temp.y - .25f);

		Temp.x = X;
		Temp.y = Y;


		KPtr<Renderer_AniEffect> EXP1 = state()->Create_One(L"TT")->Add_Component<Renderer_AniEffect>();
		EXP1->one()->Trans()->scale_local(KVector4::One * .5f);
		EXP1->one()->Trans()->pos_local(Temp);
		EXP1->EffectSetting(L"sparks.png", 4, 1, false, 0.02f);

	}
	else if(m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 394)
	{
		m_ASound = false;
	}


	/*if (m_UTime > m_pUnit->interval())
	{
		m_UTime = .0f;
	}*/
}
void Controll_User::Update_BOMB()
{
	m_AType = Controll_User::AT_IDLE;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK01);

	KPtr<Force_Unit> TB = Con_Class::s2_manager()->Find_Force(L"LUV")->Create_Unit(L"COMMANDCENTER", m_pUnit->terrain(), state());
	TB->Add_Component<OBJ_Bomb>(TB);

	m_OpATime += 10.0f;
}
void Controll_User::Update_STORY()
{
	m_RenderRot.y = m_PlayRot.y + KPI;
}
void Controll_User::Update_HEAL()
{
	m_RenderRot.y = m_PlayRot.y + KPI;

	KPtr<TheOne> TOne = state()->Create_One();
	TOne->Add_Component<Controll_Medivac>(
		one()->Trans()->pos_world(),
		m_RenderRot + KVector(.0f, KPI, .0f),
		m_pUnit->terrain());

	m_AType = Controll_User::AT_IDLE;
}
void Controll_User::Update_OPTI()
{
	m_RenderRot.y = m_PlayRot.y + KPI; 

	switch (m_OType)
	{
	case Controll_User::OT_ODIN:
	{
		KPtr<TheOne> TOne = state()->Create_One();
		TOne->Add_Component<Controll_Odin>(
			one()->Trans()->pos_world(),
			m_RenderRot + KVector(.0f, KPI, .0f),
			m_pUnit->terrain());

		break;
	}
	case Controll_User::OT_NOVA:
	{
		KPtr<TheOne> TOne = state()->Create_One();
		TOne->Add_Component<Controll_Nova>(
			one()->Trans()->pos_world(),
			m_RenderRot + KVector(.0f, KPI, .0f),
			m_pUnit->terrain());

		break;
	}
	case Controll_User::OT_HYPERION:
	{
		KPtr<TheOne> TOne = state()->Create_One();
		TOne->Add_Component<Controll_Hyperion>(
			one()->Trans()->pos_world(),
			KVector(one()->Trans()->forward_local().x, .0f, one()->Trans()->forward_local().z),
			KVector(one()->Trans()->right_local().x, .0f, one()->Trans()->right_local().z),
			m_RenderRot + KVector(.0f, KPI, .0f),
			m_pUnit->terrain());

		break;
	}
	default:
		break;
	}
	m_AType = Controll_User::AT_IDLE;
}

void Controll_User::Update_DEATH()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::DEATH);
	m_OutGame = true;
}