#pragma once
#include <unordered_map>
#include <string>
#include "State.h"

class TheOne;
class KWindow;
class StateManager : public Mof_KWindow
{
public:
	StateManager(KWindow* _Window);
	~StateManager();

public:
	friend KWindow;
	friend TheOne;

private:
	std::list<KPtr<TheOne>> m_ABSOneList;

public:
	void Check_Abs(State* _NewScene);

public:
	bool Find_Abs(KPtr<TheOne> _Actor);

private:
	std::unordered_map<std::wstring, KPtr<State>> m_StateMap;
	KPtr<State> m_CurState;
	KPtr<State> m_NextState;

public:
	KPtr<State> cur_state() {
		return m_CurState;
	}

	KPtr<State> next_state() {
		return m_NextState;
	}

public:
	template<typename Builder, typename Updater>
	KPtr<State> Create_State(const wchar_t* _Name, bool _bBuild = true)
	{
		if (nullptr != Find_State(_Name))
		{
			return nullptr;
		}

		State* pNewScene = new State();
		pNewScene->statemanager(this);
		pNewScene->name(_Name);
		pNewScene->kwindow(kwindow());
		pNewScene->Set_Type();

		Check_Abs(pNewScene);

		pNewScene->CreateBuilder<Builder>();
		pNewScene->CreateUpdater<Updater>();

		if (true == _bBuild)
		{
			pNewScene->Build();
		}

		m_StateMap.insert(std::unordered_map<std::wstring, KPtr<State>>::value_type(_Name,pNewScene));
		return pNewScene;
	}

	KPtr<State> Create_State(const wchar_t* _Name);

	KPtr<State> Find_State(const wchar_t* _Name);
	void Change_State(const wchar_t* _Name);

private:
	void Progress();
	void Render();
	void NextProgress();
	void Release();

};

