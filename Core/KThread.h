#pragma once
#include <process.h>
#include <Windows.h>
#include <unordered_map>
#include <string>
#include "SmartPtr.h"
#include <WinBase.h>
#include <functional>
#include "KMacro.h"

class KThread : public SmartPtr
{
private:
	KThread();
	~KThread();


private:
	enum FUNC_MODE
	{
		FUNC_MODE_NONE,
		FUNC_MODE_STD,
		FUNC_MODE_MEMBER,
	};

	// ��

private:
	static std::unordered_map<std::wstring, KPtr<KThread>> m_TMap;

public:
	static KPtr<KThread> FindThread(const wchar_t* _Name);
	static KPtr<KThread> CreateThread(const wchar_t* _Name);
	static bool Pause_Thread(const wchar_t* _Name);
	static bool Recov_Thread(const wchar_t* _Name);

	// �Ϲ� ���� �Լ� ���� -> �ٷ� �����忡 ���� ����
	// ������ �̸�, ������ �Լ�, �����Ǵ� Ŭ����
	// �ش��Լ��� ��ȯ���� unsigned int, �μ� ���� void* _args
	static bool Start_Thread(const wchar_t* _Name, unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg = nullptr);

	// � ��ü �� ����Լ��� ���� �����ִ� ������ -> ���� ���߾� ����
	template<typename TYPE>
	static bool Start_Thread(const wchar_t* _Name, unsigned int(TYPE::*pMember)(void*), TYPE* _pObj, void* _Arg = nullptr)
	{
		KPtr<KThread> Ptr = FindThread(_Name);
		if (nullptr == Ptr)
		{
			Ptr = CreateThread(_Name);
		}
	
		if (true == Ptr->Is_Run())
		{
			Ptr->Destroy();
		}
	
		Ptr->Start<TYPE>(pMember, _pObj, _Arg);
		return true;
	}


	// �⺻������ ����� �Լ� -> ���⼭ �����尡 ����ȴ�.
	static unsigned int __stdcall Act_Thread(void* _Arg);

////////////////////// �ɹ�

private:
	FUNC_MODE m_eMode;

	// ��𿡼� ���� ������ ����
	HANDLE m_Handle;


	// �����带 �Ͻ����� Ȥ�� ����
	bool m_bPause;

	// � �Լ��� ���������� ���� ������
	void* m_pArg;

	// �������� �Ҷ�.
	KUINT(__stdcall *StdFuncPtr)(void*);
	std::function<KUINT(void*)> m_MemberPtr;

	unsigned int(__stdcall *EndStdFuncPtr)(void*);
	std::function<KUINT(void*)> m_EndMemberPtr;

public:
	bool Is_Run() {
		return nullptr != m_Handle;
	}

private:
	bool Start(KUINT(__stdcall *_StdFuncPtr)(void*), void* _Arg = nullptr);

	template<typename T>
	bool Start(KUINT(T::*pMember)(void*), T* _pObj, void* _Arg = nullptr)
	{
		if (nullptr == _pObj || nullptr == pMember)
		{
			KASSERT(true);
			return false;
		}

		m_eMode = FUNC_MODE_MEMBER;

		StdFuncPtr = nullptr;
		m_MemberPtr = bind(pMember, _pObj, std::placeholders::_1);

		Set_Start(_Arg);

		return true;
	}

private:
	void Set_Start(void* _Arg);

public:
	void Pause();
	void Recov();

private:
	void Destroy();

};

