#include "Controll_NPC.h"
#include "Force_Unit.h"

#include <State.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <TimeManager.h>



Controll_NPC::Controll_NPC()
{
}


Controll_NPC::~Controll_NPC()
{
}


void Controll_NPC::Set_Render()
{
	m_RList = one()->Get_Component_List<Renderer_BonAni>();
}


bool Controll_NPC::Init(KPtr<Force_Unit> _Unit)
{
	m_pUnit = _Unit;
	
	Set_Render();

	return true;
}


void Controll_NPC::Update()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::STAND01);


	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		(*m_SRI)->rot_pivot(one()->Trans()->rotate_localDeg()/* + KVector(.0f, KPI, .0f)*/);
	}

	m_pUnit->Rot_Unit(one()->Trans()->rotate_local());
}