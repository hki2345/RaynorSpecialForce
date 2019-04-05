#include "Controll_Ultra.h"
#include "Force_Unit.h"
#include "SC2_Force.h"

#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <TimeManager.h>



#include <SoundPlayer.h>



Controll_Ultra::Controll_Ultra() : m_DeathSound(false)
{
}
Controll_Ultra::~Controll_Ultra()
{
}


void Controll_Ultra::Init_Value()
{
	Controll_AI::Init_Value();
}

void Controll_Ultra::Update_MIDLE()
{
	Controll_AI::Update_MIDLE();
}
void Controll_Ultra::Update_MOVE()
{
	Controll_AI::Update_MOVE();
}



void Controll_Ultra::Update_AIDLE()
{
	Controll_AI::Update_AIDLE();
}


void Controll_Ultra::Update_BURROW()
{
	m_AType = Controll_AI::AT_IDLE;
}
void Controll_Ultra::Update_HIDE()
{
	m_AType = Controll_AI::AT_IDLE;
}
void Controll_Ultra::Update_UNBURROW()
{
	m_AType = Controll_AI::AT_IDLE;
}


void Controll_Ultra::Update_ATTACK01()
{
	Controll_AI::Update_ATTACK01();
	if (m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 323 && false == m_ASound)
	{
		if (m_ARange > m_pPUnit->one()->Trans()->pos_local().distance(one()->Trans()->pos_local()))
		{
			m_pPUnit->Damage(50.0f);
		}

		m_cSound->Play(L"Ultralisk_AttackLaunch0.wav");
		m_ASound = true;
	}
	else if (m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 323)
	{
		m_ASound = false;
	}
}
void Controll_Ultra::Update_ATTACK02()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK02);
}
void Controll_Ultra::Update_ATTACK03()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK03);
}
void Controll_Ultra::Update_DEATH()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::DEATH);

	if (false == m_DeathSound)
	{
		m_cSound->Play(L"Ultralisk_Death0.wav", .5f);
		m_DeathSound = true;
	}

	if (true == m_pUnit->Check_AniDone())
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}
}