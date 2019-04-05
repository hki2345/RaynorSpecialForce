#pragma once
// 유닛도 세력을 알아야하지만 유닛은 간접적으로 아는 식(포인터)
// 요놈은 유닛을 직접 만들어야 하니까 값으로 알아야 겠다.
#include <Begin_Updater.h>
#include <DXContainer.h>
#include "Container.h"

#include <map>

// 이름과 그 세력의 고유 색을 가진다
class Force_Unit;
class Renderer_Terrain;
class State;
class SC2_Force : public Begin_Updater
{
public:
	friend class SC2Manager;

public:
	enum PLAYER_TYPE
	{
		PRT_NONE,
		PRT_PC,
		PRT_PERSON,
	};

private:	
	std::list<KPtr<Force_Unit>> m_UList;
	std::list<KPtr<Force_Unit>>::iterator m_SUI;
	std::list<KPtr<Force_Unit>>::iterator m_EUI;

	Force_Container m_Force;

private:
	bool Init();
	void Update();

public:

	KColor& force_color()
	{
		return m_Force.Color;
	}

	wchar_t*  force_name()
	{
		return m_Force.Name;
	}

	void force_color(const KColor& _Value)
	{
		m_Force.Color = _Value;
	}

	void  force_name(const wchar_t* _Value)
	{
		memcpy_s(m_Force.Name, sizeof(wchar_t) * NAMENUM, _Value, sizeof(wchar_t) * NAMENUM);
	}



	Force_Container* force_container()
	{
		return &m_Force;
	}

	bool Set_Force(const Force_Container& _Value)
	{
		name(_Value.Name);
		memcpy_s(m_Force.Name, sizeof(wchar_t) * NAMENUM, _Value.Name, sizeof(wchar_t) * NAMENUM);
		m_Force.Color = _Value.Color;
	}



	KUINT units_size()
	{
		return (KUINT)m_UList.size();
	}

	std::list<KPtr<Force_Unit>>* unit_list()
	{
		return &m_UList;
	}

	void playable_type(const PLAYABLE_TYPE& _Value);
	void playable_type(const PLAYABLE_TYPE& _Value, KPtr<State> _State);
	PLAYABLE_TYPE& playable_type();

	void Set_Active(const bool& _Value);
	KPtr<Force_Unit> Create_Unit(const wchar_t* _Name, KPtr<Renderer_Terrain> _Ter);
	KPtr<Force_Unit> Create_Unit(const wchar_t* _Name, KPtr<Renderer_Terrain> _Ter, KPtr<State> _State);
	KPtr<Force_Unit> Find_Unit(const wchar_t* _Name);
	KPtr<Force_Unit> Find_Unit(const int& _Name);
	void Delete_Unit(KPtr<Force_Unit> _Unit);
	void Clear_Unit();

	void Save();
	void Load(const wchar_t* _Name, KPtr<Renderer_Terrain> _Ter);
	

public:
	SC2_Force(const wchar_t* _Name, const KColor& _Color);
	~SC2_Force();
};

