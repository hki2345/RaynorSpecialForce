#include <KMacro.h>
#include "SC2Manager.h"
#include "SC2_Force.h"
#include "Force_Unit.h"


#include "SC2_Camera.h"

#include <Core_Class.h>
#include <ResourceManager.h>
#include <WriteStream.h>
#include <ReadStream.h>

#include <Changer_Animation.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>


SC2Manager::SC2Manager() :
	m_bGR(GRT_PLAY),
	m_bInit(false),
	m_GameSet(false)
{
	Init();
}


SC2Manager::~SC2Manager()
{
	m_SFI = m_FMap.begin();
	m_EFI = m_FMap.end();

	for (; m_SFI != m_EFI; ++m_SFI)
	{
		m_SFI = m_FMap.erase(m_SFI);
	}
}



KPtr<SC2_Force> SC2Manager::Create_Camera(const wchar_t* _Name, const KVector2& _Size, const KVector2& _Pos)
{
	return nullptr;
}
KPtr<SC2_Force> SC2Manager::Find_Camera(const wchar_t* _Name)
{
	return nullptr;
}

KPtr<SC2_Force> SC2Manager::Create_Force(const wchar_t* _Name, const KColor& _Color)
{
	m_SFI = m_FMap.find(_Name);

	if (m_FMap.end() != m_SFI)
	{
		return m_SFI->second;
	}

	KPtr<SC2_Force> NewForce = new SC2_Force(_Name, _Color);

	if (nullptr != NewForce)
	{
		m_FMap.insert(std::map<std::wstring, KPtr<SC2_Force>>::value_type(_Name, NewForce));
	}


	return NewForce;
}

KPtr<SC2_Force> SC2Manager::Find_Force(const wchar_t* _Name)
{
	m_SFI = m_FMap.find(_Name);
	
	if (m_FMap.end() == m_SFI)
	{
		return nullptr;
	}

	return m_SFI->second;
}

KPtr<SC2_Force> SC2Manager::Find_Force(const KColor& _Color)
{
	m_SFI = m_FMap.begin();
	m_EFI = m_FMap.begin();

	for (; m_SFI != m_EFI; ++m_SFI)
	{
		if (_Color == m_SFI->second->force_container()->Color)
		{
			return m_SFI->second;
		}
	}

	return nullptr;
}


bool SC2Manager::Init()
{
	if (true == m_bInit)
	{
		return true;
	}

	Create_Force(L"BOOMBAYAH", KColor(1.0f, .1f, .1f, 1.0f));
	Create_Force(L"TT", KColor(.1f, .1f, .9f, 1.0f));
	Create_Force(L"AHYEAH", KColor(.1f, .9f, .1f, 1.0f));
	Create_Force(L"GLASSBEAD", KColor(.9f, .9f, .1f, 1.0f));
	Create_Force(L"LUV", KColor(.9f, .9f, .9f, 1.0f));
	Create_Force(L"BAAM", KColor(.1f, .9f, .9f, 1.0f));
	Create_Force(L"REDFLAVER", KColor(.9f, .5f, .5f, 1.0f));
	Create_Force(L"POP+STARS", KColor(.5f, .0f, .8f, 1.0f));

	return true;
}




void SC2Manager::Update()
{
	switch (m_bGR)
	{
	case SC2Manager::GRT_PAUSE:
	{
		m_SFI = m_FMap.begin();
		m_EFI = m_FMap.begin();

		for (; m_SFI != m_EFI; ++m_SFI)
		{
			// force 전체가 멈출 수 있다. 
			// 게임적으로 멈추지 랜더나 애니메이션 및 사운드는 계속 됌
			m_SFI->second->Active_Off();
		}

		// 카메라도 멈추는 걸로 - 초상화 포함
		m_SCI = m_CMap.begin();
		m_ECI = m_CMap.end();

		for (; m_SCI != m_ECI; ++m_SCI)
		{
			m_SCI->second->Active_Off();
		}
	}
		break;

	case SC2Manager::GRT_PLAY:
	{
		m_SFI = m_FMap.begin();
		m_EFI = m_FMap.begin();

		for (; m_SFI !=m_EFI; ++m_SFI)
		{
			// force 전체가 멈출 수 있다. 
			// 게임적으로 멈추지 랜더나 애니메이션 및 사운드는 계속 됌
			if (false == m_SFI->second->Is_Active())
			{
				m_SFI->second->Active_On();
			}

			m_SFI->second->Update();
		}
		
		// 카메라도 멈추는 걸로 - 초상화 포함
		m_SCI = m_CMap.begin();
		m_ECI = m_CMap.end();

		for (; m_SCI != m_ECI; ++m_SCI)
		{
			if (false == m_SFI->second->Is_Active())
			{
				m_SFI->second->Active_On();
			}

			m_SCI->second->Update();
		}
	}

	case SC2Manager::GRT_ERROR:
		KASSERT(true);
		break;
	default:
		break;
	}
}



std::list<KPtr<Force_Unit>>* SC2Manager::force_unit_list(const wchar_t* _Name)
{
	KPtr<SC2_Force> TT = Find_Force(_Name);

	return &TT->m_UList;
}


void SC2Manager::Save(const wchar_t* _Name)
{
	std::wstring Tmp = _Name;
	Tmp += L".state";

	Tmp = PathManager::Find_ForderPath(L"STATE") + Tmp;

	WriteStream WS = WriteStream(Tmp.c_str());


	int size = (int)m_FMap.size();

	WS.Write(size);

	m_SFI = m_FMap.begin();
	m_EFI = m_FMap.end();

	for (; m_SFI != m_EFI; ++m_SFI)
	{
		WS.Write(m_SFI->first.c_str(), sizeof(wchar_t) * NAMENUM);
		WS.Write(&m_SFI->second->force_color(), sizeof(KVector));
		m_SFI->second->Save();
	}
}


void SC2Manager::Load(const wchar_t* _Name, KPtr<Renderer_Terrain> _Ter)
{
	Clear_Force();

	std::wstring Tmp = _Name;
	Tmp += L".state";

	Tmp = PathManager::Find_ForderPath(L"STATE") + Tmp;
	ReadStream RS = ReadStream(Tmp.c_str());

	if (0 == ResourceManager<Changer_Animation>::All_Count())
	{
		ResourceManager<Changer_Animation>::All_Load();
		ResourceManager<MeshContainer>::All_Load();
	}


	int size;

	RS.Read(size);

	for (int i = 0; i < size; ++i)
	{
		wchar_t TT[NAMENUM];
		KVector TCol;
		RS.Read(TT, sizeof(wchar_t) * NAMENUM);
		RS.Read(&TCol, sizeof(KVector));

		KPtr<SC2_Force> TForce = Create_Force(TT, TCol);
		TForce->Load(TT, _Ter);
		m_FMap.insert(std::map<std::wstring, KPtr<SC2_Camera>>::value_type(TT, TForce));
	}
}

void SC2Manager::AllClear_Force()
{
	Clear_Force();


	m_SFI = m_FMap.begin();
	m_EFI = m_FMap.end();

	for (; m_SFI != m_EFI; ++m_SFI)
	{
		m_SFI->second->Set_Death();
	}

	m_FMap.clear();
}

void SC2Manager::Clear_Force()
{
	m_SFI = m_FMap.begin();
	m_EFI = m_FMap.end();

	for (; m_SFI != m_EFI; ++m_SFI)
	{
		m_SFI->second->Clear_Unit();
		m_SFI->second->Set_Death();
	}

	m_FMap.clear();
}