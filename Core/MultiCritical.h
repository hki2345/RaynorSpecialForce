#pragma once
#include <Windows.h>
#include <process.h>
#include <unordered_map>

class MultiCritical
{

public:
	MultiCritical(int _Index)
	{
		Index = _Index;
		EnterCriticalSection(GetCB(_Index));
	}

	~MultiCritical() {
		LeaveCriticalSection(GetCB(Index));
	}

	// 스레드를 그룹으로 관리하며 그 그룹 하나하나하에 대한
	// 초기화도 필요한 부분이므로 크리티컬섹션이 개별적으로 하나씩 가지고있다.
	// 개별 색션은 그룹당 공유되는 식이며 그 키를 쥐고 있는 스레드가 있으면
	// 나머지는 대기를 타야한다.
	static bool bInit;
	static std::unordered_map<int, CRITICAL_SECTION*> m_Map;
	static CRITICAL_SECTION MapCB;

public:
	class Creator { public:		Creator() { MultiCritical::Init(); } };
	class Destroy { public:		~Destroy() { MultiCritical::Relase(); } };
	static Creator CreatorInst;
	static Destroy DestroyInst;


	int Index;
	CRITICAL_SECTION* GetCB(INT _Index)
	{
		EnterCriticalSection(&MapCB);

		std::unordered_map<int, CRITICAL_SECTION*>::iterator FindIter = m_Map.find(_Index);

		if (FindIter == m_Map.end())
		{
			CRITICAL_SECTION* pNewCS = new CRITICAL_SECTION();
			memset(pNewCS, 0, sizeof(CRITICAL_SECTION));
			InitializeCriticalSection(pNewCS);
			m_Map.insert(std::unordered_map<int, CRITICAL_SECTION*>::value_type(_Index, pNewCS));
			FindIter = m_Map.find(_Index);
		}

		CRITICAL_SECTION* ReturnCb = FindIter->second;

		LeaveCriticalSection(&MapCB);

		return ReturnCb;
	}

public:
	static void Init()
	{
		InitializeCriticalSection(&MapCB);
		bInit = true;
	}

	static void Relase()
	{
		std::unordered_map<int, CRITICAL_SECTION*>::iterator StartIter = m_Map.begin();
		std::unordered_map<int, CRITICAL_SECTION*>::iterator EndIter = m_Map.end();

		for (; StartIter != EndIter; StartIter++)
		{
			delete StartIter->second;
		}
	}
};
