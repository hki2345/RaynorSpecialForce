#include "OBJ_Bomb.h"
#include "Force_Unit.h"
#include "SC2_Force.h"

#include <State.h>
#include <KBox_Col.h>
#include <KRay3D.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>
#include <Renderer_AniEffect.h>

#include <InputManager.h>
#include <TimeManager.h>

#include <SoundPlayer.h>

OBJ_Bomb::OBJ_Bomb()
{
}
OBJ_Bomb::~OBJ_Bomb()
{
}


void OBJ_Bomb::Init_Value()
{
	Controll_AI::Init_Value();
		
	m_Grav = .01f;
	m_Spd = 2.f;

	m_TUnitList.clear();

	m_MType = Controll_AI::MT_MOVE;
	m_AType = Controll_AI::AT_IDLE;

	one()->Trans()->scale_local(KVector(.1f, .1f, .1f, .0f));
	one()->Trans()->pos_local(m_pPUnit->one()->Trans()->pos_local() + KVector(.0f, .6f, .0f) + m_pPUnit->one()->Trans()->forward_local() * .4f);
	m_Dir = state()->camera()->Get_Component<KRay3D>()->direction();

	Get_Component<KBox_Col>()->StayFunc(L"Bomb", this, &OBJ_Bomb::Update_StayCol);
}

void OBJ_Bomb::Update_MIDLE() { }
void OBJ_Bomb::Update_MOVE()
{
	one()->Trans()->Moving(m_Dir.Normalize() * (m_Spd));
	one()->Trans()->Moving(KVector::Down * (.4f * m_Grav));
	one()->Trans()->Rotating_Deg(KVector(.0f, 5.0f, .0f));
	
	m_Grav += DELTATIME;
	m_Spd -= DELTATIME;
	
	if (m_Spd < .0f)
	{
		m_Spd = .0f;
	}
	KVector TT = one()->Trans()->pos_local();
	
	KLOG(L"Bomb Pos: %f, %f, %f", TT.x, TT.y, TT.z);
}



void OBJ_Bomb::Update_AIDLE()
{
	one()->Trans()->Rotating_Deg(KVector(.0f, .8f, .0f));


	if (m_pPUnit->terrain()->Y_Terrain(one()->Trans()->pos_local()) >= one()->Trans()->pos_local().y)
	{
		m_AType = Controll_AI::AT_ATTACK01;
	}
	else if(0 < m_TUnitList.size())
	{
		m_AType = Controll_AI::AT_ATTACK01;
	}
}

void OBJ_Bomb::Update_BURROW() {}
void OBJ_Bomb::Update_HIDE() { }
void OBJ_Bomb::Update_UNBURROW() { }


void OBJ_Bomb::Update_ATTACK01()
{
	m_cSound->Play(L"HH_Hellion_Grenade_AttackLaunch01.wav");


	m_pUnit->Reset_Renderer();
	KPtr<Renderer_AniEffect> EXP1 = state()->Create_One(L"TT")->Add_Component<Renderer_AniEffect>();
	EXP1->one()->Trans()->scale_local(KVector4::One);
	EXP1->one()->Trans()->pos_local(one()->Trans()->pos_local());
	EXP1->EffectSetting(L"Explosion21.png", 4, 4, false, false, 0.01f);


	std::list<KPtr<Force_Unit>>::iterator S = m_TUnitList.begin();
	std::list<KPtr<Force_Unit>>::iterator E = m_TUnitList.end();

	for (; S != E; ++S)
	{
		(*S)->Damage(8);
	}
	m_UTime = .0f;
	m_AType = Controll_AI::AT_DEATH;
}


void OBJ_Bomb::Update_ATTACK02() { }
void OBJ_Bomb::Update_ATTACK03() { }
void OBJ_Bomb::Update_DEATH()
{
	m_UTime += DELTATIME;
	if (1.0f < m_UTime)
	{
		Get_Component<Force_Unit>()->force()->Delete_Unit(m_pUnit);
	}

}

void OBJ_Bomb::Update_StayCol(KCollision* _Left, KCollision* _Right)
{
	KPtr<Force_Unit> Tmp = _Left->Get_Component<Force_Unit>();

	// NULL 검사
	if (nullptr == Tmp || Tmp == m_pUnit)
	{
		Tmp = _Right->Get_Component<Force_Unit>();
		if (nullptr == Tmp)
		{
			return;
		}
	}

	KLOG(Tmp->name());

	// 유저 및 죽은 애검사
	if (true == Tmp->Is_HPDeath() || Tmp == m_pPUnit || false == Tmp->one()->Is_Active())
	{
		return;
	}
	if (L"ODIN" == Tmp->ws_name() ||
		L"TYCHUS" == Tmp->ws_name() ||
		L"NOVA" == Tmp->ws_name() ||
		L"COMMANDCENTER" == Tmp->ws_name())
	{
		return;
	}

	// 있는 애 검사
	std::list<KPtr<Force_Unit>>::iterator S = m_TUnitList.begin();
	std::list<KPtr<Force_Unit>>::iterator E = m_TUnitList.end();

	for (; S != E; ++S)
	{
		if ((*S) == Tmp)
		{
			return;
		}
	}

	m_TUnitList.push_back(Tmp);
}