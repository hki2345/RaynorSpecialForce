#include "StateManager.h"
#include "Stl_AID.h"
#include "KMacro.h"
#include "KWindow.h"
#include "DebugManager.h"

bool StateManager::Find_Abs(KPtr<TheOne> _Actor)
{
	std::list<KPtr<TheOne>>::iterator StartOverIter = m_ABSOneList.begin();
	std::list<KPtr<TheOne>>::iterator EndOverIter = m_ABSOneList.end();

	for (; StartOverIter != EndOverIter; ++StartOverIter)
	{
		if ((*StartOverIter) == _Actor)
		{
			return true;
		}
	}

	return false;
}

void StateManager::Check_Abs(State* _NewScene) 
{
	std::list<KPtr<TheOne>>::iterator StartIter = m_ABSOneList.begin();
	std::list<KPtr<TheOne>>::iterator EndIter = m_ABSOneList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (AT_ALLOVER != (*StartIter)->abs_type())
		{
			continue;
		}

		_NewScene->OverPushActor(*StartIter);
	}

}


StateManager::StateManager(KWindow* _Window)
{
	kwindow(_Window);
}

StateManager::~StateManager()
{
}

void StateManager::Change_State(const wchar_t* _Name) 
{
	if (nullptr == _Name)
	{
		m_NextState = nullptr;
		return;
	}

	m_NextState = Find_State(_Name);
	KASSERT(nullptr == m_NextState);
}

KPtr<State> StateManager::Find_State(const wchar_t* _Name) 
{
	return Map_Find<KPtr<State>>(m_StateMap, _Name);
}

void StateManager::Progress() 
{
	if (nullptr != m_NextState)
	{
		m_CurState = m_NextState;
		m_NextState = nullptr;

		if (false == m_CurState->IsBuild())
		{
			m_CurState->Build();
		}

		m_CurState->Start();
	}

	if (nullptr != m_CurState && true == m_CurState->Is_Active())
	{
		m_CurState->Progress();
	}

}

void StateManager::Render()
{
	if (nullptr != m_CurState && true == m_CurState->Is_Active())
	{
		m_CurState->Render();
	}
}


void StateManager::NextProgress()
{
	if (nullptr != m_CurState && true == m_CurState->Is_Active())
	{
		m_CurState->NextProgress();
	}
}

void StateManager::Release() {
	if (nullptr != m_CurState && true == m_CurState->Is_Active())
	{
		m_CurState->Release();
	}
}

KPtr<State> StateManager::Create_State(const wchar_t* _Name) 
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

	m_StateMap.insert(std::unordered_map<std::wstring, KPtr<State>>::value_type(_Name, pNewScene));
	return pNewScene;
}