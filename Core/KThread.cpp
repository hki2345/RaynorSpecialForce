#include "KThread.h"
#include "Stl_AID.h"

std::unordered_map<std::wstring, KPtr<KThread>> KThread::m_TMap;

KPtr<KThread> KThread::FindThread(const wchar_t* _Name) 
{
	return Map_Find<KPtr<KThread>>(m_TMap, _Name);
}

KPtr<KThread> KThread::CreateThread(const wchar_t* _Name)
{
	KThread* NewThread = new KThread();

	m_TMap.insert(std::unordered_map<std::wstring, KPtr<KThread>>::value_type(_Name, NewThread));

	return NewThread;
}

// ���ٸ� �Ҷ��� 

bool KThread::Start_Thread(const wchar_t* _Name, unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg)
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

	Ptr->Start(_StdFuncPtr, _Arg);
	return true;
}

unsigned int __stdcall KThread::Act_Thread(void* _Arg)
{
	KThread* pThread = (KThread*)_Arg;

	switch (pThread->m_eMode)
	{
	case FUNC_MODE_STD:
		return pThread->StdFuncPtr(pThread->m_pArg);
		break;
	case FUNC_MODE_MEMBER:
		return pThread->m_MemberPtr(pThread->m_pArg);
		break;
	default:
		break;
	}

	return 0;
}


bool KThread::Pause_Thread(const wchar_t* _Name) 
{
	KPtr<KThread> Ptr = FindThread(_Name);

	if (nullptr == Ptr)
	{
		KASSERT(true);
	}

	if (true == Ptr->m_bPause)
	{
		return true;
	}

	Ptr->Pause();

	return true;
}

bool KThread::Recov_Thread(const wchar_t* _Name)
{
	KPtr<KThread> Ptr = FindThread(_Name);

	if (nullptr == Ptr)
	{
		KASSERT(true);
	}

	if (false == Ptr->m_bPause)
	{
		return true;
	}

	Ptr->Recov();

	return true;
} 

KThread::KThread() : m_eMode(FUNC_MODE_NONE), m_Handle(nullptr)
, m_bPause(false), m_pArg(nullptr), StdFuncPtr(nullptr)
, m_MemberPtr(nullptr), m_EndMemberPtr(nullptr), EndStdFuncPtr(nullptr)
{

}
KThread::~KThread() 
{
	// ���⿡�� ������ �ϰ� ������ �ǰ� ����� �ȴ�.

	if (nullptr != EndStdFuncPtr)
	{
		EndStdFuncPtr(this);
	}

	if (nullptr != m_EndMemberPtr)
	{
		m_EndMemberPtr(this);
	}

	Destroy();
}

bool KThread::Start(unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg)
{
	if (nullptr == _StdFuncPtr)
	{
		KASSERT(true);
		return false;
	}

	m_eMode = FUNC_MODE_STD;

	StdFuncPtr = _StdFuncPtr;
	m_MemberPtr = nullptr;
	Set_Start(_Arg);

	return true;
}


void KThread::Set_Start(void* _Arg)
{
	m_pArg = _Arg;
	m_Handle = (HANDLE)_beginthreadex(nullptr, 0, &KThread::Act_Thread, this, 0, nullptr);
}

void KThread::Pause() 
{
	if ((DWORD)-1 != SuspendThread(m_Handle) )
	{
		m_bPause = true;
		return;
	}
	else {
		int a = 0;
	}


	return;
}

void KThread::Recov() 
{
	if ((DWORD)-1 != ResumeThread(m_Handle))
	{
		m_bPause = false;
		return;
	}

	return;
}

void KThread::Destroy() 
{
	if (nullptr != m_Handle)
	{
		TerminateThread(m_Handle, 0);
		m_Handle = nullptr;
	}
}


//
//void KThread::Create() 
//{
	// ��ť��Ƽ : �ڽ� ���μ������� ������� ������ ��ĥ�� �׿� ���� ���뿡 ���� ����ü�� �־��ټ� �ִµ�. �Ⱦ�
	// ���û����� : ���� ũ��
	// �Լ� ������
	// �ʱ�����
	// ���� �ɼ� CREATE_SUSPENDED�� ���۽� �����Ѵ�.
	// � �Լ��� �ѹ��� �����ϰ� �����ϴ� ������� ����.
	// _beginthreadex(nullptr, 0, nullptr, (void*)0, CREATE_SUSPENDED, nullptr);
// }
