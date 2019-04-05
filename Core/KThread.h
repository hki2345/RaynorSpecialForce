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

	// 쓰

private:
	static std::unordered_map<std::wstring, KPtr<KThread>> m_TMap;

public:
	static KPtr<KThread> FindThread(const wchar_t* _Name);
	static KPtr<KThread> CreateThread(const wchar_t* _Name);
	static bool Pause_Thread(const wchar_t* _Name);
	static bool Recov_Thread(const wchar_t* _Name);

	// 일반 전역 함수 시작 -> 바로 스레드에 적용 가능
	// 스레드 이름, 실행할 함수, 구동되는 클래스
	// 해당함수의 반환값은 unsigned int, 인수 값은 void* _args
	static bool Start_Thread(const wchar_t* _Name, unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg = nullptr);

	// 어떤 객체 안 멤버함수를 실행 시켜주는 스레드 -> 형을 맞추어 실행
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


	// 기본적으로 실행될 함수 -> 여기서 스레드가 실행된다.
	static unsigned int __stdcall Act_Thread(void* _Arg);

////////////////////// 맴버

private:
	FUNC_MODE m_eMode;

	// 어디에서 어디랑 공유될 건지
	HANDLE m_Handle;


	// 스레드를 일시중지 혹은 시작
	bool m_bPause;

	// 어떤 함수를 실행할지에 대한 포인터
	void* m_pArg;

	// 전역으로 할때.
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

