#include "Controll_Odin.h"
#include "Force_Unit.h"
#include "SC2_Force.h"
#include "Con_Class.h"

#include <ResourceManager.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>
#include <InputManager.h>

#include <Core_Class.h>
#include <SoundPlayer.h>
#include <Renderer_AniEffect.h>
	
Controll_Odin::Controll_Odin() : m_bAttack(false)
{
}


Controll_Odin::~Controll_Odin()
{
	m_pSound->Stop();
}



void Controll_Odin::Update_LAND()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK01, false);

	if (m_TPos.y < m_pUnit->one()->Trans()->pos_local().y)
	{
		m_Gravity += 1.0f;
		m_pUnit->one()->Trans()->Moving(KVector(.0f, -m_Gravity, .0f) * DELTATIME);
	}
	else
	{
		m_UTime = .0f;
		m_ATime = .0f;

		m_MType = MOVE_TYPE::MT_ATTACK;

		m_pUnit->one()->Trans()->pos_local(m_TPos);

		m_pSound->Play(L"Tychus_AC_Mx_Dead_Cue_01.wav", .4f);

		m_pSound->Play(L"TychusOdin_Pkup_Mech_01.wav");
	}
}


void Controll_Odin::Update_ATTACK()
{
	m_UTime += DELTATIME;
	m_ATime += DELTATIME;

	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK02);

	if (15.0f <= m_UTime)
	{
		m_MType = MOVE_TYPE::MT_RISE;
		m_TPos.y += 10.0f;


		m_pSound->Play(L"TychusOdin_TakeOff_Bkup_Alarm_01.wav", .7f);

		m_pSound->Play(L"TychusAnnouncer_GamePaused00.ogg", 1.f);

		m_pSound->Play(L"Tychus_AC_Mx_AccSteel_Bumper_01.wav", .8f);
	}

	if (0 == m_pEnemyList.size() && true == m_pUnit->Check_AniDone())
	{
		m_MType = MOVE_TYPE::MT_RISE;
		m_TPos.y += 10.0f;


		m_pSound->Play(L"TychusOdin_TakeOff_Bkup_Alarm_01.wav", .7f);
		
		m_pSound->Play(L"TychusAnnouncer_GamePaused00.ogg", 1.f);

		m_pSound->Play(L"Tychus_AC_Mx_AccSteel_Bumper_01.wav", .8f);
	}

	
	if (m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 581 && false == m_bAttack)
	{
		m_bAttack = true;

		m_pSound->Play(L"Thor_AttackImpact2.wav", .6f);

		m_ATime = .0f;
		std::list<KPtr<Force_Unit>>::iterator S = m_pEnemyList.begin();
		std::list<KPtr<Force_Unit>>::iterator E = m_pEnemyList.end();


		for (; S != E;)
		{
			if (true == (*S)->Is_HPDeath())
			{
				S = m_pEnemyList.erase(S);
			}
			else
			{
				++S;
			}
		}

		if (0 >= m_pEnemyList.size())
		{
			return;
		}

		S = m_pEnemyList.begin();
		(*S)->Damage(260.0f);
		KVector Temp = (*S)->one()->Trans()->pos_local();

		float X = Temp.x + (*S)->scale_unit().x * .5f;
		float Y = Temp.y + (*S)->scale_unit().y;

		X = KMath::random_f(X + .25f, Temp.x - .25f);
		Y = KMath::random_f(Y + .25f, Temp.y - .25f);

		Temp.x = X;
		Temp.y = Y;


		KPtr<Renderer_AniEffect> EXP1 = state()->Create_One(L"TT")->Add_Component<Renderer_AniEffect>();
		EXP1->one()->Trans()->scale_local(KVector4::One * 4.f);
		EXP1->one()->Trans()->pos_local(Temp);
		EXP1->EffectSetting(L"Explosion03.png", 4, 4, false, false, 0.02f);
	}

	if (true == m_bAttack && m_pUnit->Get_Component<Renderer_BonAni>()->index_frame() == 582)
	{
		m_bAttack = false;
	}
}


void Controll_Odin::Update_RISE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::STAND01, false);

	if (m_TPos.y > m_pUnit->one()->Trans()->pos_local().y)
	{
		m_pUnit->one()->Trans()->Moving(KVector(.0f, 2.5f, .0f) * DELTATIME);
	}
	else
	{

		Core_Class::BGM()->Set_FadeIn();
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}
}


bool Controll_Odin::Init(const KVector& _InitPos, const KVector& _Rot, KPtr<Renderer_Terrain> _Ter)
{
	m_pUnit = Con_Class::s2_manager()->Find_Force(L"GLASSBEAD")->Create_Unit(L"ODIN", _Ter, state());
	(*m_pUnit->list_renderer().begin())->rot_pivot(_Rot);
	m_pUnit->Rot_Unit(_Rot);


	KVector TT = _InitPos;
	m_TPos = TT;


	TT.y += 50.0f;
	m_Gravity = 2.5f;
	m_pUnit->one()->Trans()->pos_local(TT);

	m_MType = MOVE_TYPE::MT_LAND;



	m_UTime = .0f;
	m_ATime = .0f;

	m_pSound = new SoundPlayer();
	m_pSound->Play(L"Odin_Down.mp3");

	int Limit = 15;
	int Cnt = 0;

	std::list<KPtr<Force_Unit>> TList = *Con_Class::force_enemy()->unit_list();
	std::list<KPtr<Force_Unit>>::iterator S = TList.begin();
	std::list<KPtr<Force_Unit>>::iterator E = TList.end();

	float LimitRange = 10;


	for (size_t i = 0; i < 4; i++)
	{
		std::list<KPtr<Force_Unit>>::iterator S = TList.begin();
		std::list<KPtr<Force_Unit>>::iterator E = TList.end();

		for (; S != E; ++S)
		{
			if (LimitRange < m_pUnit->one()->Trans()->pos_local().distance((*S)->one()->Trans()->pos_local()) && 
				(int)m_pEnemyList.size() <= Limit)
			{
				m_pEnemyList.push_back((*S));
			}
			else if ((int)m_pEnemyList.size() > Limit)
			{
				return true;
			}
		}

		LimitRange -= 1;
	}

	return true;
}


void Controll_Odin::Update()
{
	switch (m_MType)
	{
	case Controll_Odin::MT_ATTACK:
		Update_ATTACK();
		break;
	case Controll_Odin::MT_LAND:
		Update_LAND();
		break;
	case Controll_Odin::MT_RISE:
		Update_RISE();
		break;
	default:
		break;
	}
}