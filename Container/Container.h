#pragma once
#include <SmartPtr.h>
#include <DXContainer.h>
#include <unordered_map>
#include <list>


class Force_Container : public SmartPtr
{
public:
	wchar_t Name[NAMENUM];
	KColor Color;

public:
	Force_Container();
	Force_Container(const Force_Container& _Other);
	~Force_Container() {};
};


enum ZERGUNIT_FORM
{
	ZF_STAND001 = 0,
	ZF_STAND002,
	ZF_STAND003,
	ZF_WALK01,
	ZF_WALK02,
	ZF_ATTACK,
	ZF_ATTACK_COVER01,
	ZF_ATTACK_COVER02,
	ZF_BURROW,
	ZF_UNBURROW,
	ZF_FIDGET01,
	ZF_FIDGET02,
};

enum TERRANUNIT_FORM
{
	TF_STAND001 = 0,
	TF_STAND002,
	TF_STAND003,
	TF_WALK01,
	TF_WALK02,
	TF_SPELL01,
	TF_SPELL02,
	TF_SPELL03,
	TF_ATTACK,
	TF_ATTACK_COVER01,
	TF_ATTACK_COVER02,
	TF_FIDGET01,
	TF_FIDGET02,
};


enum WEAPON_TYPE
{
	NONE = 0,
	INSTANCE,
	SHOT,
};

enum PLAYABLE_TYPE
{
	PBT_NONE = 0,
	PBT_ENEMY,
	PBT_USER,
};


template<typename Res>
class SC2_ConManager
{
private:
	SC2_ConManager() {}
	virtual ~SC2_ConManager() = 0 {}

private:
	static std::unordered_map<std::wstring, KPtr<Res>> m_RSMap;

public:
	static std::vector<KPtr<Res>> All_SingleVec()
	{
		std::vector<KPtr<Res>> vec;

		std::unordered_map<std::wstring, KPtr<Res>>::iterator StartIter = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator EndIter = m_RSMap.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			vec.push_back(StartIter->second);
		}

		return vec;
	}

	static std::list<KPtr<Res>> All_SingleList()
	{
		std::list<KPtr<Res>> ReturnList;

		std::unordered_map<std::wstring, KPtr<Res>>::iterator StartMapIter = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator EndMapIter = m_RSMap.end();

		for (; StartMapIter != EndMapIter; ++StartMapIter)
		{
			ReturnList.push_back(StartMapIter->second);
		}

		return ReturnList;
	}

	// 모든 리소스 개수
	static size_t All_Count() { return m_RSMap.size(); }


public:
	static KPtr<Res> Find(const wchar_t* _Name)
	{
		if (0 >= m_RSMap.size())
		{
			return nullptr;
		}
		KPtr<Res> pTemp = Map_Find<KPtr<Res>>(m_RSMap, _Name);

		return pTemp;
	}

	static KPtr<Res> Find_Name(const wchar_t* _Name)
	{
		if (0 >= m_RSMap.size())
		{
			return nullptr;
		}


		std::unordered_map<std::wstring, KPtr<Res>>::iterator S = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator E = m_RSMap.end();

		std::string Tmp = _Name;

		for (; S != E; ++S)
		{
			if (Tmp == S->second.ws_name())
			{
				return S->second;
			}
		}

		return nullptr;
	}


	static void Erase(const wchar_t* _Name)
	{
		m_RSMap.erase(_Name);
	}

	static void Erase(KPtr<Res> _Ptr)
	{
		std::unordered_map<std::wstring, KPtr<Res>>::iterator S = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator E = m_RSMap.end();

		for (; S != E; ++S)
		{
			if (_Ptr == S->second)
			{
				m_RSMap.erase(S->first);
				break;
			}
		}
	}


#pragma region LOAD
	static KPtr<Res> Load(const wchar_t* _Name)
	{
		Res* NewRes = new Res();

		std::unordered_map<std::wstring, KPtr<Res>>::iterator FI = m_RSMap.find(_Name);
		if (FI != m_RSMap.end())
		{
			delete NewRes;
			return FI->second;
		}

		if (false == NewRes->Load(_Name))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));

		return NewRes;
	}


#pragma endregion

	static bool Clear()
	{
		m_RSMap.clear();
		return true;
	}

public:
	// 모든 리소스를 다 가져온다
	static bool All_Load();
	

private:
	// 실제 인제 파일을 불러오려는 함수
	static bool All_LoadOrigin(const wchar_t* _Target);

	// 모든 리소스를 가져오려는 제귀 함수
	static bool All_LoadSub(const intptr_t& _Handle, _wfinddata_t& _FD, const wchar_t* _Target);
	static bool IsDot(const std::wstring& _Value);

};

template<typename Res>
std::unordered_map<std::wstring, KPtr<Res>> SC2_ConManager<Res>::m_RSMap;
