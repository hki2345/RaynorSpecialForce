#include "Controll_Medivac.h"
#include "Force_Unit.h"
#include "SC2_Force.h"
#include "Con_Class.h"

#include <ResourceManager.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>
#include <InputManager.h>

#include <SoundPlayer.h>


Controll_Medivac::Controll_Medivac()
{
}


Controll_Medivac::~Controll_Medivac()
{
	m_pSound->Stop();
}



void Controll_Medivac::Update_MIDLE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK02);
	m_UTime += DELTATIME;

	if (10.0f <= m_UTime)
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}

	// 
}
void Controll_Medivac::Update_LAND()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK01, false);

	if (m_TPos.y < m_pUnit->one()->Trans()->pos_local().y)
	{
		m_pUnit->one()->Trans()->Moving(KVector(.0f, -2.5f, .0f) * DELTATIME);
	}
	else
	{
		m_MType = MOVE_TYPE::MT_IDLE;
	}
}


bool Controll_Medivac::Init(const KVector& _InitPos, const KVector& _Rot, KPtr<Renderer_Terrain> _Ter)
{
	m_pUnit = Con_Class::s2_manager()->Find_Force(L"AHYEAH")->Create_Unit(L"MEDIVAC", _Ter, state());
	(*m_pUnit->list_renderer().begin())->rot_pivot(_Rot);


	KVector TT = _InitPos;
	m_TPos = TT;
	
	
	TT.y += 5.0f;
	m_pUnit->one()->Trans()->pos_local(TT);

	m_pSound = new SoundPlayer();

	m_pSound->Play(L"Medivac_Yes05.ogg");

	m_pSound->Play(L"Tychus_MedivacPlatform_What_02.ogg");


	m_MType = MOVE_TYPE::MT_LAND;

	return true;
}


void Controll_Medivac::Update()
{
	switch (m_MType)
	{
	case Controll_Medivac::MT_IDLE:
		Update_MIDLE();
		break;
	case Controll_Medivac::MT_LAND:
		Update_LAND();
		break;
	default:
		break;
	}
}