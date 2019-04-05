#include "SC2_Force.h"
#include "Force_Unit.h"
#include "Con_Class.h"


#include <WriteStream.h>
#include <ReadStream.h>
#include <Core_Class.h>

#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>


#include "Controll_Abrr.h"
#include "Controll_Zergling.h"
#include "Controll_Ultra.h"
#include "Controll_Brutal.h"
#include "Controll_Infested.h"
#include "Controll_User.h"
#include "Controll_NPC.h"
#include "SC2_Camera.h"


SC2_Force::SC2_Force(const wchar_t* _Name, const KColor& _Color)
{
	name(_Name);
	memcpy_s(m_Force.Name, NAMENUM, _Name, NAMENUM);
	this->m_Force.Color = _Color;
}


SC2_Force::~SC2_Force()
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		m_SUI = m_UList.erase(m_SUI);
	}
} 

void SC2_Force::Set_Active(const bool& _Value)
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		if (true == _Value)
		{
			(*m_SUI)->one()->Active_On();
		}
		else
		{
			(*m_SUI)->one()->Active_Off();
		}
	}
}



KPtr<Force_Unit> SC2_Force::Create_Unit(const wchar_t* _Name,
	KPtr<Renderer_Terrain> _Ter)
{
	KPtr<State> TabScene = Core_Class::MainScene();
	KPtr<TheOne> TOne = TabScene->Create_One();

	TOne->Trans()->pos_local(KVector::Zero);
	TOne->Trans()->scale_local(KVector(1.f, 1.f, 1.f));
	KPtr<Force_Unit> TT = TOne->Add_Component<Force_Unit>(_Name, _Ter, false);
	TT->force(this);
	TT->Insert_Collider();

	m_UList.push_back(TT);

	return TT;
}


KPtr<Force_Unit> SC2_Force::Create_Unit(const wchar_t* _Name, KPtr<Renderer_Terrain> _Ter, KPtr<State> _State)
{
	KPtr<State> TabScene = _State;
	KPtr<TheOne> TOne = TabScene->Create_One();

	TOne->Trans()->pos_local(KVector::Zero);
	TOne->Trans()->scale_local(KVector(1.f, 1.f, 1.f));
	KPtr<Force_Unit> TT = TOne->Add_Component<Force_Unit>(_Name, _Ter, false);
	TT->force(this);
	TT->Insert_Collider();

	m_UList.push_back(TT);

	return TT;
}

KPtr<Force_Unit> SC2_Force::Find_Unit(const wchar_t* _Name)
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		if (true == PathManager::Is_StrVSStr((*m_SUI)->name(), _Name))
		{
			return (*m_SUI);
		}
	}
	return nullptr;
}

KPtr<Force_Unit> SC2_Force::Find_Unit(const int& _Name)
{
	if (_Name > (int)m_UList.size() || 0 > _Name)
	{
		BBY;
	}

	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (int Cnt = 0; m_SUI != m_EUI; ++m_SUI, ++Cnt)
	{
		if (_Name == Cnt)
		{
			return (*m_SUI);
		}
	}
	return nullptr;
}

void SC2_Force::Clear_Unit()
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		(*m_SUI)->one()->Set_Death();
	}

	m_UList.clear();
}

void SC2_Force::Delete_Unit(KPtr<Force_Unit> _Unit)
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		if ((*m_SUI) == _Unit)
		{
			(*m_SUI)->one()->Set_Death();
			m_UList.erase(m_SUI);
			return;
		}
	}
}


bool SC2_Force::Init()
{
	return true;
}


void SC2_Force::Update()
{
}



// 해당 세력의 유닛 배치
void SC2_Force::Save()
{
	std::wstring Tmp = ws_name();
	Tmp += L".force";

	Tmp = PathManager::Find_ForderPath(L"FORCE") + Tmp;


	// 개수
	// 유닛 이름 - 위치
	WriteStream WS = WriteStream(Tmp.c_str());


	int Size = (int)m_UList.size();
	WS.Write(Size);


	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		KVector TT = (*m_SUI)->one()->Trans()->pos_world();
		KVector RT = (*m_SUI)->one()->Trans()->rotate_localDeg();
		WS.Write((*m_SUI)->name(), sizeof(wchar_t) * NAMENUM);
		WS.Write(&TT, sizeof(KVector));
		WS.Write(&RT, sizeof(KVector));
	}
}


void SC2_Force::Load(const wchar_t* _Name, KPtr<Renderer_Terrain> _Ter)
{
	Clear_Unit();


	name(_Name);
	std::wstring Tmp = _Name;
	Tmp += L".force";
	Tmp = PathManager::Find_ForderPath(L"FORCE") + Tmp;

	ReadStream RS = ReadStream(Tmp.c_str());


	// 개수
	// 유닛 이름 - 위치 - 회전

	int Size = 0;
	RS.Read(Size);


	for (int i = 0; i < Size; i++)
	{
		wchar_t Tmp[NAMENUM];
		KVector TT;
		KVector RT;
		RS.Read(Tmp, sizeof(wchar_t) * NAMENUM);
		RS.Read(&TT, sizeof(KVector));
		RS.Read(&RT, sizeof(KVector));

		KPtr<Force_Unit> TU = Create_Unit(Tmp, _Ter);
		TU->one()->Trans()->pos_local(TT);
		TU->one()->Trans()->rotate_local(RT);

		TU->Rot_Unit(RT);
		std::list<KPtr<Renderer_BonAni>> Renders = TU->list_renderer();
		std::list<KPtr<Renderer_BonAni>>::iterator S = Renders.begin();
		std::list<KPtr<Renderer_BonAni>>::iterator E = Renders.end();

		for (; S != E; ++S)
		{
			(*S)->rot_pivot(RT);
		}
	}
}


void SC2_Force::playable_type(const PLAYABLE_TYPE& _Value)
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		(*m_SUI)->playable_type(_Value);

		switch (_Value)
		{
		case PBT_NONE:
		{
			(*m_SUI)->Delete_Component<Controll_Abrr>();
			(*m_SUI)->Delete_Component<Controll_Zergling>();
			(*m_SUI)->Delete_Component<Controll_Infested>();
			(*m_SUI)->Delete_Component<Controll_Ultra>();
			(*m_SUI)->Delete_Component<Controll_Brutal>();
			(*m_SUI)->Delete_Component<Controll_User>();

			(*m_SUI)->Add_Component<Controll_NPC>((*m_SUI));
			if (this == Con_Class::force_enemy())
			{
				Con_Class::force_enemy(nullptr);
			}
			if (this == Con_Class::force_player())
			{
				Con_Class::force_player(nullptr);
			}
			break;
		}
		case PBT_ENEMY:
		{
			(*m_SUI)->Delete_Component<Controll_Abrr>();
			(*m_SUI)->Delete_Component<Controll_User>();

			if (L"ABERRATION" == (*m_SUI)->ws_name())
			{
				(*m_SUI)->Add_Component<Controll_Abrr>((*m_SUI));
			}
			else if (L"ZERGLING" == (*m_SUI)->ws_name())
			{
				(*m_SUI)->Add_Component<Controll_Zergling>((*m_SUI));
			}
			else if (L"INFESTEDMARINE" == (*m_SUI)->ws_name())
			{
				(*m_SUI)->Add_Component<Controll_Infested>((*m_SUI));
			}
			else if (L"ULTRALISK" == (*m_SUI)->ws_name())
			{
				(*m_SUI)->Add_Component<Controll_Ultra>((*m_SUI));
			}
			else if (L"BRUTALISK" == (*m_SUI)->ws_name())
			{
				(*m_SUI)->Add_Component<Controll_Brutal>((*m_SUI));
			}
			else
			{
				(*m_SUI)->Add_Component<Controll_AI>((*m_SUI));
			}

			Con_Class::force_enemy(this);

			if (this == Con_Class::force_player())
			{
				Con_Class::force_player(nullptr);
			}
			break;
		}
		case PBT_USER:
		{
			(*m_SUI)->Delete_Component<Controll_Abrr>();
			(*m_SUI)->Delete_Component<Controll_User>();
			(*m_SUI)->Add_Component<Controll_User>((*m_SUI), Core_Class::MainScene()->camera()->Get_Component<SC2_Camera>());

			Con_Class::force_player(this);

			if (this == Con_Class::force_enemy())
			{
				Con_Class::force_enemy(nullptr);
			}
			break;
		}
		default:
			break;
		}
	}
}


void SC2_Force::playable_type(const PLAYABLE_TYPE& _Value, KPtr<State> _State)
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		(*m_SUI)->playable_type(_Value);

		switch (_Value)
		{
		case PBT_USER:
		{
			(*m_SUI)->Delete_Component<Controll_Abrr>();
			(*m_SUI)->Delete_Component<Controll_Zergling>();
			(*m_SUI)->Delete_Component<Controll_Ultra>();
			(*m_SUI)->Delete_Component<Controll_Brutal>();
			(*m_SUI)->Delete_Component<Controll_Infested>();

			(*m_SUI)->Delete_Component<Controll_User>();
			(*m_SUI)->Add_Component<Controll_User>((*m_SUI), _State->camera()->Get_Component<SC2_Camera>());

			Con_Class::force_player(this);

			if (this == Con_Class::force_enemy())
			{
				Con_Class::force_enemy(nullptr);
			}
			break;
		}
		default:
			break;
		}
	}
}


PLAYABLE_TYPE& SC2_Force::playable_type()
{
	return (*m_UList.begin())->playable_type();
}