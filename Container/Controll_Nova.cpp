#include "Controll_Nova.h"
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


Controll_Nova::Controll_Nova()
{
}


Controll_Nova::~Controll_Nova()
{
	m_pEnemyList.clear();
	m_pSound->Stop();
}



void Controll_Nova::Update_MIDLE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::STAND02);
	m_UTime += DELTATIME;

	if (true == m_pUnit->Check_AniDone())
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();


	}
}
void Controll_Nova::Update_ATTACK()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK02);
	m_UTime += DELTATIME;
	m_ATime += DELTATIME;
	if (m_UTime < 2.0f)
	{
		return;
	}

	if (0 == m_pEnemyList.size())
	{
		Core_Class::BGM()->Set_FadeIn();
		m_MType = MOVE_TYPE::MT_IDLE;
		return;
	}
	if (m_ATime > .1f)
	{
		m_pSound->Play(L"Ghost_SnipeAttackLaunch.wav");

		m_ATime = .0f;

		std::list<KPtr<Force_Unit>>::iterator S = m_pEnemyList.begin();

		std::list<KPtr<Renderer_AniEffect>>::iterator SU1 = m_pTargetList.begin();
		std::list<KPtr<Renderer_AniEffect>>::iterator SU2 = m_pMTargetList.begin();

		(*S)->Damage(1000.0f);
		m_pEnemyList.erase(S);


		(*SU1)->one()->Set_Death();
		(*SU2)->one()->Set_Death();

		m_pTargetList.erase(SU1);
		m_pMTargetList.erase(SU2);
	}

	if (0 == m_pEnemyList.size())
	{
		Core_Class::BGM()->Set_FadeIn();
		m_MType = MOVE_TYPE::MT_IDLE;
	}
}


bool Controll_Nova::Init(const KVector& _InitPos, const KVector& _RotPos, KPtr<Renderer_Terrain> _Ter)
{
	m_pSound = new SoundPlayer();
	m_pSound->Play(L"HeroNovaMP1_Nuclear_Strike02.ogg");

	m_pUnit = Con_Class::s2_manager()->Find_Force(L"BAAM")->Create_Unit(L"NOVA", _Ter, state());

	m_TPos = _InitPos + KVector(1.0f, .0f, .0f);
	m_pUnit->one()->Trans()->pos_local(m_TPos);


	(*m_pUnit->list_renderer().begin())->rot_pivot(_RotPos);
	m_MType = MOVE_TYPE::MT_ATTACK;



	int Limit = 5;
	int THp = 3;

	std::list<KPtr<Force_Unit>> TList = *Con_Class::s2_manager()->Find_Force(L"GLASSBEAD")->unit_list();
	std::list<KPtr<Force_Unit>>::iterator S = TList.begin();
	std::list<KPtr<Force_Unit>>::iterator E = TList.end();

	for (; S != E; ++S)
	{
		if (THp < (*S)->hp() && 0 < (*S)->hp() &&  (int)m_pEnemyList.size() < Limit)
		{
			m_pEnemyList.push_back((*S));


			KPtr<Renderer_AniEffect> EXP1 = state()->Create_One(L"TT")->Add_Component<Renderer_AniEffect>();
			EXP1->one()->Trans()->scale_local(KVector4::One * 5.0f);

			if (Force_Unit::ANI_TYPE::DEATH == (*S)->Get_Animation())
			{
				EXP1->one()->Trans()->pos_local((*S)->one()->Trans()->pos_local());
			}
			else
			{
				EXP1->one()->Trans()->pos_local((*S)->one()->Trans()->pos_local() + KVector(.0f, (*S)->scale_unit().y * .5f, .0f));
			}
			EXP1->EffectSetting(L"lockOnbase.png", 1, 1, true, 0.02f);


			KPtr<Renderer_AniEffect> EXP2 = state()->Create_One(L"TT")->Add_Component<Renderer_AniEffect>();
			EXP2->one()->Trans()->scale_local(KVector4::One * 5.0f);

			if (Force_Unit::ANI_TYPE::DEATH == (*S)->Get_Animation())
			{
				EXP2->one()->Trans()->pos_local((*S)->one()->Trans()->pos_local());
			}
			else
			{
				EXP2->one()->Trans()->pos_local((*S)->one()->Trans()->pos_local() + KVector(.0f, (*S)->scale_unit().y * .5f, .0f));
			}
			EXP2->EffectSetting(L"lockOnEffect.png", 1, 1, true, 0.02f);

			m_pTargetList.push_back(EXP1);
			m_pMTargetList.push_back(EXP2);
		}
	}

	THp -= 100;

	m_UTime = .0f;
	m_ATime = .0f;
	return true;
}


void Controll_Nova::Init_UI()
{

}


void Controll_Nova::Update()
{
	switch (m_MType)
	{
	case Controll_Nova::MT_IDLE:
		Update_MIDLE();
		break;
	case Controll_Nova::MT_ATTACK:
		Update_ATTACK();
		Update_UI();
		break;
	default:
		break;
	}
}

void Controll_Nova::Update_UI()
{
	std::list<KPtr<Renderer_AniEffect>>::iterator S = m_pMTargetList.begin();
	std::list<KPtr<Renderer_AniEffect>>::iterator E = m_pMTargetList.end();

	std::list<KPtr<Renderer_AniEffect>>::iterator NS = m_pMTargetList.begin();
	std::list<KPtr<Renderer_AniEffect>>::iterator NE = m_pMTargetList.end();


	std::list<KPtr<Force_Unit>> TList = *Con_Class::s2_manager()->Find_Force(L"GLASSBEAD")->unit_list();
	std::list<KPtr<Force_Unit>>::iterator TS = TList.begin();
	std::list<KPtr<Force_Unit>>::iterator TE = TList.end();

	for (; S != E; ++S, ++TS, ++NS)
	{
		(*S)->one()->Trans()->Rotating_Deg(KVector(.0f, .0f, 5.0f, .0f));
	}
}