#include "TimeManager.h"
#include "Stl_AID.h"

TimeManager::GameTimer TimeManager::MainTimer = GameTimer();
std::unordered_map<std::wstring, KPtr<TimeManager::GameTimer>> TimeManager::m_MapTimer;

TimeManager::GameTimer::GameTimer()
{
	Init();
}
TimeManager::GameTimer::~GameTimer()
{

}



void TimeManager::GameTimer::Init()
{
	// ���� �ٱ��� �ʴ� cpu�� Ÿ���� �˾ƿ��� �Լ�
	QueryPerformanceFrequency(&m_SecCount);

	// ����Ÿ�� 
	QueryPerformanceCounter(&m_CurCount);
	m_AccFPS = 0;
	m_FPSCount = 0;
}

void TimeManager::Init() 
{
	MainTimer.Init();
}

KPtr<TimeManager::GameTimer> TimeManager::FindTimer(const wchar_t* _Name) 
{
	return Map_Find<KPtr<TimeManager::GameTimer>>(m_MapTimer, _Name);
}

KPtr<TimeManager::GameTimer> TimeManager::CreatTimer(const wchar_t* _Name) 
{
	KPtr<TimeManager::GameTimer> PTR = FindTimer(_Name);

	if (nullptr != PTR)
	{
		return PTR;
	}

	TimeManager::GameTimer* NewTimer = new TimeManager::GameTimer();
	m_MapTimer.insert(std::unordered_map<std::wstring, KPtr<GameTimer>>::value_type(_Name, NewTimer));

	return NewTimer;
}

float TimeManager::GameTimer::Update()
{
	// ��ŸŸ���� ���Ѵ�.
	QueryPerformanceCounter(&m_NextCount);

	m_DeltaTime = ((float)(m_NextCount.QuadPart - m_CurCount.QuadPart)) / ((float)m_SecCount.QuadPart);
	m_CurCount.QuadPart = m_NextCount.QuadPart;

	m_AccFPS += m_DeltaTime;

	++m_FPSCount;

	if (1.0f <= m_AccFPS)
	{
		m_Fps = (int)((float)m_FPSCount / m_AccFPS);
		m_AccFPS = 0.0f;
		m_FPSCount = 0;
	}

	return m_DeltaTime;
}

TimeManager::TimeManager()
{
}


TimeManager::~TimeManager()
{
}


void TimeManager::Update()
{
	MainTimer.Update();
}
