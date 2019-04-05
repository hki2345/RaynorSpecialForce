#include "Controll_AI.h"
#include "Force_Unit.h"
#include "Con_Class.h"

#include <KBox_Col.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>
#include <InputManager.h>
#include <KWindow.h>

#include <SoundPlayer.h>


Controll_AI::Controll_AI() :
	m_Attack(false),
	m_ASound(false)
{
}


Controll_AI::~Controll_AI()
{
	m_cSound->Stop();
}


void Controll_AI::Set_Render()
{
	m_RList = one()->Get_Component_List<Renderer_BonAni>();
}

void Controll_AI::Add_Render(KPtr<Renderer_BonAni> _Other)
{
	m_RList.push_back(_Other);
}

void Controll_AI::Del_Render(KPtr<Renderer_BonAni> _Other)
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		if ((*m_SRI) == _Other)
		{
			m_RList.erase(m_SRI);
			return;
		}
	}
}

bool Controll_AI::Init(KPtr<Force_Unit> _Unit)
{
	m_pPUnit = (*Con_Class::force_player()->unit_list()->begin());
	m_pUnit = _Unit;
	
	
	if (nullptr == m_pUnit->terrain())
	{
		BBY;
	}


	m_MType = MOVE_TYPE::MT_IDLE;
	m_AType = ACT_TYPE::AT_HIDE;


	m_pUnit->playable_type(PLAYABLE_TYPE::PBT_ENEMY);
	Set_Render();
	Init_Value();


	m_cSound = new SoundPlayer();
	m_cSound->Play(L"Ghost_SnipeAttackLaunch.wav");
	m_cSound->Stop();

	return true;
}

void Controll_AI::Init_Value()
{
	m_Attack = false;

	m_UTime = .0f;
	m_ATime = .0f;

	m_ARange = 3.0f;
	m_MRange = 12.0f;
}

void Controll_AI::Update()
{
	if (true == m_pPUnit->Is_HPDeath())
	{
		return;
	}

	Update_Death();
	Update_Dir();
	Update_Act();
	Update_Move();
	Update_Terrain();
	Update_RenCol();
}


void Controll_AI::Update_Death()
{
	if (.0f > m_pUnit->hp())
	{
		one()->Get_Component<KBox_Col>()->Active_Off();
		m_AType = Controll_AI::AT_DEATH;
	}
}

void Controll_AI::Update_Dir()
{
	if (nullptr == m_pPUnit || 0 > m_pUnit->hp())
	{
		return;
	}

	
	KVector Dir = m_pUnit->one()->Trans()->pos_local() - m_pPUnit->one()->Trans()->pos_local();
	Dir.Normalize();

	m_AIPos = m_pUnit->one()->Trans()->pos_local();

	float TT = Dir.Dot(KVector::Right);
	m_AIRot = Dir.cross3D(KVector::Right);


	m_AIRot.x = m_AIRot.z = .0f;
	m_AIRot.y += 1.0f;
	m_AIRot.y *= .5f;
	m_AIRot.y *= KPI;

	if (0 < TT)
	{
		m_AIRot *= -1.0f;
	}

	m_RenderRot = m_AIRot;
}

void Controll_AI::Update_RenCol()
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		(*m_SRI)->rot_pivot(m_RenderRot/* + KVector(.0f, KPI, .0f)*/);
	}

	m_pUnit->Rot_Unit(m_RenderRot);
}


void Controll_AI::Update_Move()
{
	if (Controll_AI::AT_IDLE != m_AType)
	{
		return;
	}

	switch (m_MType)
	{
	case Controll_AI::MT_IDLE:
		Update_MIDLE();
		KLOG(L"Unit Move: IDLE");
		break;
	case Controll_AI::MT_MOVE:
		Update_MOVE();
		KLOG(L"Unit Move: MOVE");
		break;
	default:
		break;
	}
}


void Controll_AI::Update_Act()
{
	KLOG(L"Unit Act: %f", m_pUnit->hp());

	switch (m_AType)
	{
	case Controll_AI::AT_IDLE:
		Update_AIDLE();
		break;

	case Controll_AI::AT_BURROW:
		Update_BURROW();
		break;
	case Controll_AI::AT_HIDE:
		Update_HIDE();
		break;
	case Controll_AI::AT_UNBURROW:
		Update_UNBURROW();
		break;

	case Controll_AI::AT_ATTACK01:
		Update_ATTACK01();
		break;
	case Controll_AI::AT_ATTACK02:
		Update_ATTACK02();
		break;
	case Controll_AI::AT_ATTACK03:
		Update_ATTACK03();
		break;

	case Controll_AI::AT_DEATH:
		Update_DEATH();
		
		break;
	default:
		break;
	}
}

void Controll_AI::Update_Terrain()
{
	KVector4 TT = one()->Trans()->pos_local();


	if (m_pUnit->terrain()->TER_Left() > TT.x)
	{
		TT.x = m_pUnit->terrain()->TER_Left();
	}
	// ²¨ ¾¾¹ß
	// if (m_pUnit->terrain()->TER_Right() < TT.x)
	// {
	// 	TT.x = m_pUnit->terrain()->TER_Right();
	// }
	if (m_pUnit->terrain()->TER_Down() > TT.z)
	{
		TT.z = m_pUnit->terrain()->TER_Down();
	}
	// if (m_pUnit->terrain()->TER_Up() < TT.z)
	// {
	// 	TT.z = m_pUnit->terrain()->TER_Up();
	// }
	one()->Trans()->pos_local(TT);
}