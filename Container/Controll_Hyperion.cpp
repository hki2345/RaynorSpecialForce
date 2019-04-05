#include "Controll_Hyperion.h"
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


Controll_Hyperion::Controll_Hyperion()
{
}


Controll_Hyperion::~Controll_Hyperion()
{
	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();

	for (m_SPI = m_TPos.begin(); m_SULI != m_EULI; ++m_SULI, ++m_SPI)
	{
		(*m_SULI)->one()->Set_Death();
	}

	m_pUnitList.clear();

	m_pSound->Stop();
}



bool Controll_Hyperion::Init(
	const KVector& _InitPos
	, const KVector& _Forward
	, const KVector& _Right
	, const KVector& _RotPos
	, KPtr<Renderer_Terrain> _Ter)
{
	m_For = _Forward;

	m_pUnitList.push_back(Con_Class::s2_manager()->Find_Force(L"TT")->Create_Unit(L"HYPERION", _Ter, state()));
	for (size_t i = 0; i < 8; i++)
	{
		m_pUnitList.push_back(Con_Class::s2_manager()->Find_Force(L"TT")->Create_Unit(L"BATTLECRUISER", _Ter, state()));
	}

	m_SULI = m_pUnitList.begin();
	(*m_SULI)->one()->Trans()->scale_local(KVector(3.0f, 3.0f, 3.0f));

	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();

	for (; m_SULI != m_EULI; ++m_SULI)
	{
		(*(*m_SULI)->list_renderer().begin())->rot_pivot(_RotPos);
	}


	float Front = 15.0f;

	m_TPos.push_back(KVector(_InitPos + _Forward * (Front + 8.0f) + KVector(.0f, 9.0f, .0f)));
	
	m_TPos.push_back(KVector(_InitPos + _Forward * (Front + 7.0f)  + _Right * 11.0f + KVector(.0f, 7.0f, .0f)));
	m_TPos.push_back(KVector(_InitPos + _Forward * (Front + 7.0f)  + _Right * -11.0f + KVector(.0f, 7.0f, .0f)));
												   
	m_TPos.push_back(KVector(_InitPos + _Forward * (Front + 10.0f) + _Right * 10.0f + KVector(.0f, 10.0f, .0f)));
	m_TPos.push_back(KVector(_InitPos + _Forward * (Front + 10.0f) + _Right * -10.0f + KVector(.0f, 10.0f, .0f)));
												   
	m_TPos.push_back(KVector(_InitPos + _Forward * (Front + 0.0f)  + _Right * 12.0f + KVector(.0f, 8.0f, .0f)));
	m_TPos.push_back(KVector(_InitPos + _Forward * (Front + 0.0f)  + _Right * -12.0f + KVector(.0f, 8.0f, .0f)));
												   
	m_TPos.push_back(KVector(_InitPos + _Forward * (Front -3.0f)  + _Right * 6.5f + KVector(.0f, 10.0f, .0f)));
	m_TPos.push_back(KVector(_InitPos + _Forward * (Front -3.0f)  + _Right * -6.5f + KVector(.0f, 10.0f, .0f)));



	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();

	for (m_SPI = m_TPos.begin(); m_SULI != m_EULI; ++m_SULI, ++m_SPI)
	{
		(*m_SULI)->one()->Trans()->pos_local((*m_SPI) + _Forward * -100.0f);
	}


	m_MType = MOVE_TYPE::MT_COME;
	m_UTime = .0f;
	m_ETime = .0f;
	m_LauCnt = 1;

	m_pSound = new SoundPlayer();
	m_pASound = new SoundPlayer();
 	m_pSound->Play(L"Battlecruiser_What00.ogg");

	m_Battle = false;


	std::list<KPtr<Force_Unit>>::iterator S = Con_Class::s2_manager()->Find_Force(L"POP+STARS")->unit_list()->begin();
	std::list<KPtr<Force_Unit>>::iterator E = Con_Class::s2_manager()->Find_Force(L"POP+STARS")->unit_list()->end();

	for (; S != E; ++S)
	{
		if (false == (*S)->Is_HPDeath())
		{
			m_pEnemyList.push_back(*S);
		}
	}

	return true;
}


void Controll_Hyperion::Update()
{
	switch (m_MType)
	{
	case Controll_Hyperion::MT_COME:
		Update_COME();
		break;
	case Controll_Hyperion::MT_WARPIN:
		Update_WARPIN();
		break;
	case Controll_Hyperion::MT_ATTACK:
		Update_ATTACK();
		break;
	case Controll_Hyperion::MT_WARPOUT:
		Update_WARPOUT();
		break;

	default:
		break;
	}
}

void Controll_Hyperion::Update_COME()
{
	m_UTime += DELTATIME;
	if (2.5f <= m_UTime && false == m_Battle)
	{
		m_pSound->Play(L"Battlecruiser_Yes01.ogg");

		m_Battle = true;
	}

	if (7.0f <= m_UTime)
	{
		m_pSound->Play(L"Battlecruiser_HyperspaceIn01.wav");


		m_MType = MOVE_TYPE::MT_WARPIN;
		m_UTime = .0f;
	}
}

void Controll_Hyperion::Update_WARPIN()
{
	m_UTime += DELTATIME;
	
	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();
	
	
	if (m_LauCnt > 8)
	{
		m_SPI = m_TPos.begin();

		if ((*m_SULI)->one()->Trans()->pos_local().x >= (*m_SPI).x ||
			(*m_SULI)->one()->Trans()->pos_local().y >= (*m_SPI).y)
		{
			(*m_SULI)->one()->Trans()->pos_local((*m_SPI));
			m_UTime = .0f;
			m_ATime = .0f;
			m_MType = MOVE_TYPE::MT_ATTACK;


			m_pSound->Play(L"Fleet_Attack.mp3");
			m_pSound->Play(L"AC_Alarm_AirRaid_Siren.ogg", .4f);
		}
		else
		{
			(*m_SULI)->one()->Trans()->Moving(m_For * 5.0f);
		}
	}
	else
	{
		int Cnt = 0;
		for (m_SPI = m_TPos.begin(); m_SULI != m_EULI; ++m_SULI, ++m_SPI, ++Cnt)
		{
			if ((*m_SULI)->one()->Trans()->pos_local().x >= (*m_SPI).x ||
				(*m_SULI)->one()->Trans()->pos_local().y >= (*m_SPI).y)
			{
				++m_LauCnt;
				(*m_SULI)->one()->Trans()->pos_local((*m_SPI));
			}

			if (Cnt == m_LauCnt)
			{
				(*m_SULI)->one()->Trans()->Moving(m_For * 20.0f);
			}
		}
	}


}


void Controll_Hyperion::Update_ATTACK()
{
	m_UTime += DELTATIME;
	m_ATime += DELTATIME;

	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();

	for (m_SPI = m_TPos.begin(); m_SULI != m_EULI; ++m_SULI, ++m_SPI)
	{
		(*m_SULI)->one()->Trans()->Moving(m_For * .01f);
	}

	Core_Class::BGM()->Stop();

	if (m_ATime > 20.0f)
	{
		m_MType = MOVE_TYPE::MT_WARPOUT;

		m_pSound->Play(L"Battlecruiser_HyperspaceOut01.wav");
	}

	if (m_UTime > .33f)
	{
		m_UTime = .0f;

		std::list<KPtr<Force_Unit>>::iterator S = m_pEnemyList.begin();
		if (m_pEnemyList.end() == S)
		{
			return;
		}

		(*S)->Damage(15.f);

		if (true == (*S)->Is_HPDeath())
		{
			m_pEnemyList.erase(S);
		}
		
		m_pASound->Play(L"Battlecruiser_AttackLaunch0.wav", .1f);
	}
	Update_Effect();
}


void Controll_Hyperion::Update_WARPOUT()
{
	m_UTime += DELTATIME;

	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();

	for (m_SPI = m_TPos.begin(); m_SULI != m_EULI; ++m_SULI, ++m_SPI)
	{
		(*m_SULI)->one()->Trans()->Moving(m_For * 20.0f);
	}

	if (3.0f <= m_UTime)
	{
		m_SULI = m_pUnitList.begin();
		m_EULI = m_pUnitList.end();

		for (; m_SULI != m_EULI; ++m_SULI)
		{
			(*m_SULI)->force()->Delete_Unit((*m_SULI));
			one()->Set_Death();
			Core_Class::BGM()->Stop();
			Core_Class::BGM()->Set_FadeIn();
			Core_Class::BGM()->Play(L"Love_Theme_V2_FULL.ogg");
		}

		m_pUnitList.clear();
		m_TPos.clear();
	}
}

void Controll_Hyperion::Update_Effect()
{
	m_ETime += DELTATIME;

	if (.2f < m_ETime)
	{
		KPtr<TransPosition> Temp = (*m_pUnitList.begin())->one()->Trans();

		float X = Temp->pos_local().x;
		float Z = Temp->pos_local().y;

		X = KMath::random_f(X - Temp->right_local().x * 25.0f, X + Temp->right_local().x * 25.0f);
		Z = KMath::random_f(Z + Temp->forward_local().z * 25.0f, Z + Temp->forward_local().z * 50);
		float Y = (*m_pUnitList.begin())->terrain()->Y_Terrain(KVector(X, .0f, Z));

		KPtr<Renderer_AniEffect> EXP1 = state()->Create_One(L"TT")->Add_Component<Renderer_AniEffect>();
		EXP1->one()->Trans()->scale_local(KVector4::One * 8.0f);
		EXP1->one()->Trans()->pos_local(KVector(X, Y, Z));
		EXP1->EffectSetting(L"ExPlosion.png", 5, 5, false, false, 0.02f);

		m_ETime = .0f;
	}
}