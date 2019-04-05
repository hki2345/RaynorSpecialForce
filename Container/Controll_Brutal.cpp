#include "Controll_Brutal.h"
#include "Force_Unit.h"
#include "SC2_Force.h"

#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <TimeManager.h>

#include <SoundPlayer.h>

Controll_Brutal::Controll_Brutal()
{
}
Controll_Brutal::~Controll_Brutal()
{
}


void Controll_Brutal::Init_Value()
{
	Controll_AI::Init_Value();
	m_ARange = 2.0f;
	m_MRange = 10.0f;
}

void Controll_Brutal::Update_MIDLE()
{
	Controll_AI::Update_MIDLE();
}
void Controll_Brutal::Update_MOVE()
{
	Controll_AI::Update_MOVE();
}



void Controll_Brutal::Update_AIDLE()
{
	Controll_AI::Update_AIDLE();
}


void Controll_Brutal::Update_BURROW()
{
	m_AType = Controll_AI::AT_IDLE;
}
void Controll_Brutal::Update_HIDE()
{
	m_AType = Controll_AI::AT_IDLE;
}
void Controll_Brutal::Update_UNBURROW()
{
	m_AType = Controll_AI::AT_IDLE;
}


void Controll_Brutal::Update_ATTACK01()
{
	Controll_AI::Update_ATTACK01();
	if (false == m_ASound && (m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 1293 ||
		m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 1307))
	{
		if (m_ARange > m_pPUnit->one()->Trans()->pos_local().distance(one()->Trans()->pos_local()))
		{
			m_pPUnit->Damage(8.0f);
		}


		m_cSound->Play(L"Ultralisk_AttackLaunch0.wav");
		m_ASound = true;
	}
	else if (m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 1294 ||
		m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 1308)
	{
		m_ASound = false;
	}
}
void Controll_Brutal::Update_ATTACK02()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK02);
}
void Controll_Brutal::Update_ATTACK03()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK03);
}



void Controll_Brutal::Update_DEATH()
{
	m_pUnit->Reset_Renderer();
	if (nullptr == DeathRender)
	{
		m_cSound->Play(L"Brutalisk_Roar2.wav", .5f);

		DeathRender = one()->Add_Component<Renderer_BonAni>();
		DeathRender->Set_Fbx(L"Brutalisk_Death.KM3");
		DeathRender->Create_Animation(false);
		DeathRender->Set_Clip(0);
	}

	if (true == DeathRender->Check_AniDone())
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}
}