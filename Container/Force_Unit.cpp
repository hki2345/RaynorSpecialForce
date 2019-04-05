#include "Force_Unit.h"
#include "SC2_Force.h"


#include <Core_Class.h>

#include <ReadStream.h>
#include <WriteStream.h>

#include <ResourceManager.h>
#include <Renderer_Terrain.h>
#include <Renderer_BonAni.h>
#include <KBox_Col.h>


Force_Unit::Force_Unit() :
	m_RenLoop (true)
{
	m_AType = ANI_TYPE::ALL;
	m_Info.WType = WEAPON_TYPE::NONE;
	m_Info.LSpeed = .0f;
	m_Info.RSpeed = .0f;
	m_Info.UScale = KVector::Zero;
	m_Info.PBType = PLAYABLE_TYPE::PBT_NONE;

	m_Info.HP = 10;
	m_Info.Interval = 1.0f;
	m_Info.Score = 50.0f;
}


Force_Unit::~Force_Unit()
{
}



bool Force_Unit::Init(const wchar_t* _Name, KPtr<Renderer_Terrain> _Ter, const bool& _Find)
{
	if (nullptr == _Ter)
	{
		BBY;
	}

	if (true == _Find)
	{
		Load(_Name);
	}
	else
	{
		Load_NoFind(_Name);
	}


	m_MAXHP = m_Info.HP;
	m_pTer = _Ter;


	return true;
}

void Force_Unit::Update()
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		if (nullptr != m_Force)
		{
			(*m_SRI)->force_name(m_Force->force_name());
			(*m_SRI)->force_color(m_Force->force_color());
		}
	}

	Update_RenderAni();
	Update_Col();
}


// 리소스 메니저에서 이미 All_LOad를 하였다 가정한다.
bool Force_Unit::Load(const wchar_t* _Name)
{
	Reset_Renderer();
	name(_Name);
	std::wstring Tmp = _Name;
	Tmp += L".KUD";

	Tmp = PathManager::Find_ForderPath(L"KUD") + Tmp;

	ReadStream RS = ReadStream(Tmp.c_str());
	

	RS.Read(m_Info.WType);
	RS.Read(m_Info.LSpeed);
	RS.Read(m_Info.RSpeed);
	RS.Read(m_Info.UScale);
	RS.Read(m_Info.PBType);
	RS.Read(m_Info.HP);
	RS.Read(m_Info.Interval);
	RS.Read(m_Info.Score);

	int Cnt = 0;
	RS.Read(Cnt);
	for (int i = 0; i < Cnt; i++)
	{
		wchar_t Tmp[NAMENUM];
		RS.Read(Tmp, sizeof(wchar_t) * NAMENUM);
		m_StrList.push_back(Tmp);
	}



	m_SCI = m_StrList.begin();
	m_ECI = m_StrList.end();

	for (; m_SCI != m_ECI; ++m_SCI)
	{
		Tmp.clear();
		Tmp += (*m_SCI) + L".KM3";

		if (nullptr == one())
		{
			KPtr<TheOne> TOne = Core_Class::MainScene()->Create_One(L"TKM3");
			TOne->Set_Component(this);
		}

		
		KPtr<Renderer_BonAni> TRen = one()->Add_Component<Renderer_BonAni>();
		TRen->Set_Fbx(Tmp.c_str());
		TRen->Create_Animation();
		m_RList.push_back(TRen);
	}


	return true;
}


// 리소스 메니저에서 이미 All_LOad를 하였다 가정한다.
bool Force_Unit::Load_NoFind(const wchar_t* _Name)
{
	Reset_Renderer();
	name(_Name);
	std::wstring Tmp = _Name;
	Tmp += L".KUD";

	Tmp = PathManager::Find_ForderPath(L"KUD") + Tmp;

	ReadStream RS = ReadStream(Tmp.c_str());


	RS.Read(m_Info.WType);
	RS.Read(m_Info.LSpeed);
	RS.Read(m_Info.RSpeed);
	RS.Read(m_Info.UScale);
	RS.Read(m_Info.PBType);
	RS.Read(m_Info.HP);
	RS.Read(m_Info.Interval);
	RS.Read(m_Info.Score);

	int Cnt = 0;
	RS.Read(Cnt);
	for (int i = 0; i < Cnt; i++)
	{
		wchar_t Tmp[NAMENUM];
		RS.Read(Tmp, sizeof(wchar_t) * NAMENUM);
		m_StrList.push_back(Tmp);
	}



	m_SCI = m_StrList.begin();
	m_ECI = m_StrList.end();

	for (; m_SCI != m_ECI; ++m_SCI)
	{
		Tmp.clear();
		Tmp += (*m_SCI) + L".KM3";

		if (nullptr == one())
		{
			KPtr<TheOne> TOne = Core_Class::MainScene()->Create_One(L"TKM3");
			TOne->Set_Component(this);
		}


		KPtr<Renderer_BonAni> TRen = one()->Add_Component<Renderer_BonAni>();
		TRen->Set_Fbx(Tmp.c_str());
		TRen->Create_Animation(false);
		m_RList.push_back(TRen);
	}


	return true;
}

bool Force_Unit::Save()
{
	std::wstring Tmp = ws_name();
	Tmp += L".KUD";

	Tmp = PathManager::Find_ForderPath(L"KUD") + Tmp;
	WriteStream WS = WriteStream(Tmp.c_str());

	WS.Write(m_Info.WType);
	WS.Write(m_Info.LSpeed);
	WS.Write(m_Info.RSpeed);
	WS.Write(m_Info.UScale);
	WS.Write(m_Info.PBType);
	WS.Write(m_Info.HP);
	WS.Write(m_Info.Interval);
	WS.Write(m_Info.Score);

	int Cnt = (int)m_StrList.size();
	WS.Write(Cnt); 
	m_SCI = m_StrList.begin();
	m_ECI = m_StrList.end();

	for (; m_SCI != m_ECI; ++m_SCI)
	{
		WS.Write((*m_SCI).c_str(), sizeof(wchar_t) * NAMENUM);
	}

	return true;
}

void Force_Unit::Reset_Renderer()
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		(*m_SRI)->Set_Death();
	}

	m_RList.clear();
	m_StrList.clear();
	Update_StrList();
}


void Force_Unit::Delete_Renderer(KPtr<Renderer_BonAni> _Other)
{
	if (nullptr == _Other)
	{
		BBY;
	}

	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();


	for (; m_SRI != m_ERI; ++m_SRI)
	{
		if ((*m_SRI) == _Other)
		{
			m_RList.erase(m_SRI);
			break;
		}
	}
	Update_StrList();
}

void Force_Unit::Insert_Renderer(KPtr<Renderer_BonAni> _Other)
{
	if (nullptr == _Other)
	{
		BBY;
	}

	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		if ((*m_SRI) == _Other)
		{
			return;
		}
	}
	
	m_RList.push_back(_Other);
	Update_StrList();
}

void Force_Unit::Insert_Collider()
{
	if (nullptr == m_Col)
	{
		m_Col = one()->Add_Component<KBox_Col>(100);
		// m_Col->StayFunc(L"ForceUnit Collision", this, &Force_Unit::Collision_OtherUnit);
	}
}

bool Force_Unit::Check_AniDone()
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		if (true == (*m_SRI)->Check_AniDone())
		{
			return true;
		}
	}

	return false;
}


void Force_Unit::Update_StrList()
{
	m_StrList.clear();
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		m_StrList.push_back((*m_SRI)->mesh_container()->FileName());
	}
}



void Force_Unit::Update_RenderAni()
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		// 0 번째는 그거 임 전체 애니
		(*m_SRI)->Set_Clip((int)m_AType + 1);
		(*m_SRI)->loop(m_RenLoop);
	}
}


void Force_Unit::Update_Col()
{
	if (nullptr == m_Col)
	{
		return;
	}

	m_Col->pos_pivot(one()->Trans()->pos_world());
	m_Col->scale_pivot(m_Info.UScale);
	m_Col->rot_pivot(m_ColPivot);
}



void Force_Unit::Collision_OtherUnit(KCollision* _Left, KCollision* _Right)
{
	// KLOG(L"Unit Collision: %b", true);
}