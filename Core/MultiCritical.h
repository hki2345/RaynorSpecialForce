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

	// �����带 �׷����� �����ϸ� �� �׷� �ϳ��ϳ��Ͽ� ����
	// �ʱ�ȭ�� �ʿ��� �κ��̹Ƿ� ũ��Ƽ�ü����� ���������� �ϳ��� �������ִ�.
	// ���� ������ �׷�� �����Ǵ� ���̸� �� Ű�� ��� �ִ� �����尡 ������
	// �������� ��⸦ Ÿ���Ѵ�.
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
