#pragma once
#include "Begin_Updater.h"

#include <unordered_map>
#include <list>

#include "Mof_KWindow.h"
#include "Mof_State.h"
#include "RenderManager.h"
#include "CollisionManager.h"

#include "ReadStream.h"


// c++ 11 이후
// final -->  여기 이후로 상속을 내리지 않는다.

class TheOne;
class StateManager;
class Camera;
class State final : public Begin_Updater, public Mof_KWindow
{
private:
	friend StateManager;
	friend class RenderManager;
	friend TheOne;

#pragma region UPDATEBUILD

public:
	class State_Builder : public Begin_Type, public Mof_State, public Mof_KWindow
	{
	public:
		friend State;


	public:
		virtual void Build_State() = 0;

	protected:
		State_Builder() {}
		~State_Builder() {}
	};

	class State_Updater : public Begin_Type, public Mof_State, public Mof_KWindow
	{
	public:
		friend State;

	public:
		virtual void Start_State() {}
		virtual void Update_Edit() {}
		virtual void Update_State() {}
		virtual void DebugRender() {}
		virtual void UIRender() {}

		virtual void AfterRenderStart_State() {}

		virtual void End_State() {}

	protected:
		State_Updater() {}
		~State_Updater() {}
	};

private:
	KPtr<State_Builder> m_SBuilder;
	std::list<KPtr<State_Updater>> m_SUpdaterList;
	bool m_bBuild;
	bool m_bRender;

public:
	void Set_Render(const bool& _Value)
	{
		m_bRender = _Value;
	}
	
	bool IsBuild() {
		return m_bBuild;
	}

private:
	State();
	~State();

public:
	StateManager* m_Mof_StateMgr;

	StateManager* statemanager() {
		return m_Mof_StateMgr;
	}

private:
	void statemanager(StateManager* _SceneMgr) 
	{
		m_Mof_StateMgr = _SceneMgr;
	}


public:
	template<typename T>
	KPtr<T> Updater()
	{
		return m_HSceneUpdater;
	}

	template<typename T>
	KPtr<T> Builder()
	{
		return m_SBuilder;
	}

public:
	template<typename T>
	void CreateUpdater() 
	{
		T* State_Updater = new T();
		State_Updater->Set_Type();
		State_Updater->state(this);
		State_Updater->kwindow(kwindow());

		m_SUpdaterList.push_back(State_Updater);
	}

	template<typename T>
	void CreateBuilder()
	{
		if (nullptr != m_SBuilder)
		{
			m_SBuilder = nullptr;
		}

		m_SBuilder = new T();
		m_SBuilder->state(this);
		m_SBuilder->kwindow(kwindow());
	}

#pragma endregion

public:
	void Build();
	void Start();

private:
	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator m_SOI;
	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator m_EOI;
	std::unordered_map<int, std::list<KPtr<TheOne>>> m_OneMap;

	std::list<KPtr<TheOne>>::iterator m_SOLI;
	std::list<KPtr<TheOne>>::iterator m_EOLI;

private:
	std::vector<KPtr<TheOne>> AllObjectList();

public:
	std::unordered_map<int, std::list<KPtr<TheOne>>> AllActor() {
		return m_OneMap;
	}

public:
	friend Camera;

private:
	Camera* m_Camera;

public:
	Camera* camera();
	void camera(Camera* _Cam);


private:
	void Progress();
	void PrevUpdate();
	void Update();
	void NextUpdate();
	void FinalUpdate();
	void DebugUpdate();

	void NextProgress();
	// TheOne의 Componet의 End_Update를 실행한다. 
	void EndUpdate();

private:
	void Render();
	void DebugRender();
	void UIRender();
	void Release();

private:
	KPtr<TheOne> Create_One(ReadStream& _Stream, void(*_ComLoadPtr)(KPtr<TheOne>, const std::string& _ComName), bool _Root = true);

public:
	// 트랜스폼 기본 추가
	KPtr<TheOne> Create_One(const wchar_t* _pName = L"TheOne", int _Order = 0);

	// 트렌스폼 없음
	KPtr<TheOne> Create_OneNoneTrans(const wchar_t* _pName = L"TheOne", int _Order = 0);

private:
	void MoveActor(KPtr<TheOne> _Actor);
	bool EraseActor(KPtr<TheOne> _Actor);

public:
	///////////////////////////// This_RenderManager;
	RenderManager This_RenderManager;

public:
	///////////////////////////// This_RenderManager;
	CollisionManager This_Col2DManager;
	CollisionManager This_Col3DManager;

private:
	void OverPushActor(TheOne* _pActor);

};

