#include "Controll_AI.h"
#include "Force_Unit.h"
#include "SC2_Force.h"

#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <TimeManager.h>

#include <SoundPlayer.h>


void Controll_AI::Update_MIDLE()
{
	if (Controll_AI::AT_DEATH == m_AType)
	{
		return;
	}

	float TT = m_pPUnit->one()->Trans()->pos_local().distance(m_pUnit->one()->Trans()->pos_local());
	if (m_MRange >= TT)
	{
		m_MType = MOVE_TYPE::MT_MOVE;
	}

	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::STAND01);
}
void Controll_AI::Update_MOVE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::WALK01);
	m_RenderRot.y = m_AIRot.y + KPI;
	

	KPtr<TransPosition> TT = m_pUnit->one()->Trans();
	KVector Dir = m_pPUnit->one()->Trans()->pos_local() - m_pUnit->one()->Trans()->pos_local();
	Dir.Normalize();
	TT->Moving(Dir * m_pUnit->linear_speed() * DELTATIME);


	float Temp = Dir.distance();
	if (m_MRange <= Temp)
	{
		m_MType = MOVE_TYPE::MT_IDLE;
	}
}



void Controll_AI::Update_AIDLE()
{
	if (Controll_AI::AT_DEATH == m_AType)
	{
		m_AType = Controll_AI::AT_DEATH;
		return;
	}

	if (m_ARange >= m_pPUnit->one()->Trans()->pos_local().distance(m_pUnit->one()->Trans()->pos_local()))
	{
		m_AType = Controll_AI::AT_ATTACK01;
	}
}


void Controll_AI::Update_BURROW()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::FIDGET02);
}
void Controll_AI::Update_HIDE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::DEATH, false);

	if (m_MRange >= m_pPUnit->one()->Trans()->pos_local().distance(m_pUnit->one()->Trans()->pos_local()))
	{
		m_AType = Controll_AI::AT_UNBURROW;
		m_cSound->Play(L"Brutalisk_UnBurrow_01.wav", .4f);
	}
}
void Controll_AI::Update_UNBURROW()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::FIDGET01);
	if (true == m_pUnit->Get_Component<Renderer_BonAni>()->Check_AniDone())
	{
		m_AType = Controll_AI::AT_IDLE;
	}
}


void Controll_AI::Update_ATTACK01()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK01);


	if (true == m_pUnit->Get_Component<Renderer_BonAni>()->Check_AniDone())
	{
		m_AType = Controll_AI::AT_IDLE;
	}
}
void Controll_AI::Update_ATTACK02()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK02);
}
void Controll_AI::Update_ATTACK03()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK03);
}
void Controll_AI::Update_DEATH()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::DEATH);

	if (true == m_pUnit->Check_AniDone())
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}
}