#include "State.h"
#include "TheOne.h"
#include "Stl_AID.h"
#include "TransPosition.h"
#include "KMacro.h"
#include "WriteStream.h"
#include "ReadStream.h"
#include "StateManager.h"


#include "Core_Class.h"
#include "KDevice.h"
#include <atlstr.h>

Camera* State::camera() 
{
	return m_Camera;
}

void State::camera(Camera* _Cam)
{
	m_Camera = _Cam;
}

State::State() : m_Camera(nullptr), m_bBuild(false), m_bRender(true)
{
}


State::~State()
{
	if (0 != m_SUpdaterList.size())
	{
		std::list<KPtr<State_Updater>>::iterator StartUpdaterIter = m_SUpdaterList.begin();
		std::list<KPtr<State_Updater>>::iterator EndUpdaterIter = m_SUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->End_State();
		}
	}

	Release();
}


void State::Progress()
{
	if (0 != m_SUpdaterList.size())	
	{	
		std::list<KPtr<State_Updater>>::iterator StartUpdaterIter = m_SUpdaterList.begin();
		std::list<KPtr<State_Updater>>::iterator EndUpdaterIter = m_SUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->Update_Edit();
			(*StartUpdaterIter)->Update_State();
		}
	}

	PrevUpdate();
	Update();
	NextUpdate();
	FinalUpdate();
	DebugUpdate();
	This_Col2DManager.Progress();
	This_Col3DManager.Progress();
}

void State::PrevUpdate() {
	m_SOI = m_OneMap.begin();
	m_EOI = m_OneMap.end();

	for (; m_SOI != m_EOI; ++m_SOI)
	{
		m_SOLI = m_SOI->second.begin();
		m_EOLI = m_SOI->second.end();

		for (; m_SOLI != m_EOLI; ++m_SOLI)
		{
			if (false == (*m_SOLI)->Is_Active())
			{
				continue;
			}

			(*m_SOLI)->PrevUpdate();
		}
	}
}
void State::Update() {
	m_SOI = m_OneMap.begin();
	m_EOI = m_OneMap.end();

	for (; m_SOI != m_EOI; ++m_SOI)
	{
		m_SOLI = m_SOI->second.begin();
		m_EOLI = m_SOI->second.end();

		for (; m_SOLI != m_EOLI; ++m_SOLI)
		{
			if (false == (*m_SOLI)->Is_Active())
			{
				continue;
			}

			(*m_SOLI)->Update();
		}
	}
}
void State::NextUpdate() {
	m_SOI = m_OneMap.begin();
	m_EOI = m_OneMap.end();

	for (; m_SOI != m_EOI; ++m_SOI)
	{
		m_SOLI = m_SOI->second.begin();
		m_EOLI = m_SOI->second.end();

		for (; m_SOLI != m_EOLI; ++m_SOLI)
		{
			if (false == (*m_SOLI)->Is_Active())
			{
				continue;
			}

			(*m_SOLI)->NextUpdate();
		}
	}
}


void State::FinalUpdate() {
	m_SOI = m_OneMap.begin();
	m_EOI = m_OneMap.end();

	for (; m_SOI != m_EOI; ++m_SOI)
	{
		m_SOLI = m_SOI->second.begin();
		m_EOLI = m_SOI->second.end();

		for (; m_SOLI != m_EOLI; ++m_SOLI)
		{
			if (false == (*m_SOLI)->Is_Active())
			{
				continue;
			}

			(*m_SOLI)->FinalUpdate();
		}
	}
}

void State::DebugUpdate() {
	m_SOI = m_OneMap.begin();
	m_EOI = m_OneMap.end();

	for (; m_SOI != m_EOI; ++m_SOI)
	{
		m_SOLI = m_SOI->second.begin();
		m_EOLI = m_SOI->second.end();

		for (; m_SOLI != m_EOLI; ++m_SOLI)
		{
			if (false == (*m_SOLI)->Is_Active())
			{
				continue;
			}

			(*m_SOLI)->DebugUpdate();
		}
	}
}

void State::Render() 
{
	if (false == m_bRender)
	{
		return;
	}
	This_RenderManager.Render();
}

void State::DebugRender() {
	m_SOI = m_OneMap.begin();
	m_EOI = m_OneMap.end();

	for (; m_SOI != m_EOI; ++m_SOI)
	{
		m_SOLI = m_SOI->second.begin();
		m_EOLI = m_SOI->second.end();

		for (; m_SOLI != m_EOLI; ++m_SOLI)
		{
			if (false == (*m_SOLI)->Is_Active())
			{
				continue;
			}

			(*m_SOLI)->DebugRender();
		}
	}

	if (0 != m_SUpdaterList.size())
	{
		std::list<KPtr<State_Updater>>::iterator StartUpdaterIter = m_SUpdaterList.begin();
		std::list<KPtr<State_Updater>>::iterator EndUpdaterIter = m_SUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->DebugRender();
		}
	}
}

void State::UIRender() {
	m_SOI = m_OneMap.begin();
	m_EOI = m_OneMap.end();

	for (; m_SOI != m_EOI; ++m_SOI)
	{
		m_SOLI = m_SOI->second.begin();
		m_EOLI = m_SOI->second.end();

		for (; m_SOLI != m_EOLI; ++m_SOLI)
		{
			if (false == (*m_SOLI)->Is_Active())
			{
				continue;
			}

			(*m_SOLI)->UIRender();
		}
	}

	if (0 != m_SUpdaterList.size())
	{
		std::list<KPtr<State_Updater>>::iterator StartUpdaterIter = m_SUpdaterList.begin();
		std::list<KPtr<State_Updater>>::iterator EndUpdaterIter = m_SUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->UIRender();
		}
	}
}


void State::Release() 
{
	This_RenderManager.Release();
	This_Col2DManager.Release();
	This_Col3DManager.Release();

	m_SOI = m_OneMap.begin();
	m_EOI = m_OneMap.end();

	for (; m_SOI != m_EOI; ++m_SOI)
	{
		m_SOLI = m_SOI->second.begin();
		m_EOLI = m_SOI->second.end();

		for (; m_SOLI != m_EOLI; )
		{
			if (false == (*m_SOLI)->Is_Death())
			{
				(*m_SOLI)->Release();
				++m_SOLI;
				continue;
			}

			m_SOLI = m_SOI->second.erase(m_SOLI);
		}
	}
}

void State::Build() 
{
	if (nullptr != m_SBuilder)
	{
		m_SBuilder->Build_State();
	}


	m_bBuild = true;

} 

void State::Start() 
{
	if (0 != m_SUpdaterList.size())
	{
		std::list<KPtr<State_Updater>>::iterator StartUpdaterIter = m_SUpdaterList.begin();
		std::list<KPtr<State_Updater>>::iterator EndUpdaterIter = m_SUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->Start_State();
		}
	}
}

std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator ActorIter;
KPtr<TheOne> State::Create_One(const wchar_t* _pName /*= L"HObject"*/, int _Order /*= 0*/)
{
	if (false == Is_MapFind(m_OneMap, _Order)) 
	{
		m_OneMap.insert(std::unordered_map<int, std::list<KPtr<TheOne>>>::value_type(_Order, std::list<KPtr<TheOne>>()));
	}
	ActorIter = m_OneMap.find(_Order);
	TheOne* pNewActor = new TheOne();
	pNewActor->name(_pName);
	pNewActor->order(_Order);
	pNewActor->Set_Type();
	pNewActor->kwindow(kwindow());
	pNewActor->state(this);
	pNewActor->Add_Component<TransPosition>();

	ActorIter->second.push_back(pNewActor);

	return pNewActor;
}

KPtr<TheOne> State::Create_OneNoneTrans(const wchar_t* _pName /*= L"HObject"*/, int _Order /*= 0*/)
{
	if (false == Is_MapFind(m_OneMap, _Order))
	{
		m_OneMap.insert(std::unordered_map<int, std::list<KPtr<TheOne>>>::value_type(_Order, std::list<KPtr<TheOne>>()));
	}
	ActorIter = m_OneMap.find(_Order);
	TheOne* pNewActor = new TheOne();
	pNewActor->name(_pName);
	pNewActor->order(_Order);
	pNewActor->Set_Type();
	pNewActor->kwindow(kwindow());
	pNewActor->state(this);

	ActorIter->second.push_back(pNewActor);

	return pNewActor;
}


void State::MoveActor(KPtr<TheOne> _Actor)
{
	ActorIter = m_OneMap.find(_Actor->order());

	KASSERT(ActorIter == m_OneMap.end());
	if (ActorIter == m_OneMap.end())
	{
		return;
	}

	ActorIter->second.push_back(_Actor);

	return;

}

bool State::EraseActor(KPtr<TheOne> _Actor)
{
	ActorIter = m_OneMap.find(_Actor->order());

	KASSERT(ActorIter == m_OneMap.end());
	if (ActorIter == m_OneMap.end()) {
		return false;
	}

	std::list<KPtr<TheOne>>::iterator StartListIter = ActorIter->second.begin();
	std::list<KPtr<TheOne>>::iterator EndListIter = ActorIter->second.end();

	for (; StartListIter != EndListIter; StartListIter++)
	{
		if (_Actor == *StartListIter)
		{
			ActorIter->second.erase(StartListIter);
			return true;
		}
	}

	KASSERT(ActorIter == m_OneMap.end());
	return false;
}

void State::NextProgress()
{
	EndUpdate();
}

void State::EndUpdate() {
	m_SOI = m_OneMap.begin();
	m_EOI = m_OneMap.end();

	for (; m_SOI != m_EOI; ++m_SOI)
	{
		m_SOLI = m_SOI->second.begin();
		m_EOLI = m_SOI->second.end();

		for (; m_SOLI != m_EOLI; ++m_SOLI)
		{
			if (false == (*m_SOLI)->Is_Active())
			{
				continue;
			}

			(*m_SOLI)->EndUpdate();
		}
	}
}

void State::OverPushActor(TheOne* _pActor) 
{
	if (false == Is_MapFind(m_OneMap, _pActor->m_Order))
	{
		m_OneMap.insert(std::unordered_map<int, std::list<KPtr<TheOne>>>::value_type(_pActor->m_Order, std::list<KPtr<TheOne>>()));
	}

	ActorIter = m_OneMap.find(_pActor->m_Order);

	if (ActorIter == m_OneMap.end())
	{
		return;
	}
	// 액터를 넣는건 처리가 되었다.
	ActorIter->second.push_back(_pActor);

	// 랜더러를 다 넣어준다.
	_pActor->Insert_ABSRender(&This_RenderManager);
	_pActor->Insert_ABSCol(&This_Col2DManager, &This_Col3DManager);
}

std::vector<KPtr<TheOne>> State::AllObjectList()
{
	std::vector<KPtr<TheOne>> ReturnVector;

	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator StartMapIter;
	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator EndMapIter;

	size_t ReserveSize = 0;

	StartMapIter = m_OneMap.begin();
	EndMapIter = m_OneMap.end();
	for (; StartMapIter != EndMapIter; ++StartMapIter)
	{
		ReserveSize += StartMapIter->second.size();
	}

	ReturnVector.reserve(ReserveSize);

	std::list<KPtr<TheOne>>::iterator StartListIter;
	std::list<KPtr<TheOne>>::iterator EndListIter;

	StartMapIter = m_OneMap.begin();
	EndMapIter = m_OneMap.end();
	for (; StartMapIter != EndMapIter; ++StartMapIter)
	{
		StartListIter = StartMapIter->second.begin();
		EndListIter = StartMapIter->second.end();
		for (; StartListIter != EndListIter; ++StartListIter)
		{
			ReturnVector.push_back((*StartListIter));
		}
	}

	return ReturnVector;
}

KPtr<TheOne> State::Create_One(ReadStream& _Stream, void(*_ComLoadPtr)(KPtr<TheOne>, const std::string& _ComName), bool _Root)
{
	wchar_t Name[1024];

	UINT ReadSize;
	_Stream.Read(&ReadSize, sizeof(UINT));

	if (true == _Root)
	{
		if (false == Is_MapFind(m_OneMap, ReadSize))
		{
			m_OneMap.insert(std::unordered_map<int, std::list<KPtr<TheOne>>>::value_type(ReadSize, std::list<KPtr<TheOne>>()));
		}
		ActorIter = m_OneMap.find(ReadSize);
	}

	TheOne* pNewActor = new TheOne();
	pNewActor->Set_Type();
	pNewActor->kwindow(kwindow());
	pNewActor->state(this);
	pNewActor->order(ReadSize);

	_Stream.Read(&ReadSize, sizeof(UINT));
	_Stream.Read((void*)Name, sizeof(wchar_t) * ReadSize);

	pNewActor->name(Name);

	if (true == _Root)
	{
		ActorIter->second.push_back(pNewActor);
	}

	_Stream.Read(&ReadSize, sizeof(UINT));

	std::string ComName;

	for (size_t i = 0; i < ReadSize; i++)
	{
		char Arr[256];
		unsigned int Size;
		_Stream.Read(&Size, sizeof(unsigned int));
		_Stream.Read(Arr, sizeof(char) * Size);

		ComName = Arr;

		if (nullptr == _ComLoadPtr)
		{
			continue;
		}
		_ComLoadPtr(pNewActor, ComName);
	}

	_Stream.Read(&ReadSize, sizeof(UINT));

	for (size_t i = 0; i < ReadSize; i++)
	{
		KPtr<TheOne> ChildActor = Create_One(_Stream, _ComLoadPtr, false);
		pNewActor->Add_Child(ChildActor, true);
	}

	return pNewActor;
}