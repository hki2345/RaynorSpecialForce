#pragma once

#include <Windows.h>
#include <unordered_map>
#include <string>

#include "SmartPtr.h"


#define DELTATIME	TimeManager::DeltaTime()
#define FPS			TimeManager::Fps()

// �����带 ����� �� QueryPerformanceFrequency() 
// �� �Լ��� CPU�� Ŭ���� ������ ������ �ִµ� �Ϲ����� ��쿡�� 
// ��Ƽ ���μ��� ȯ�濡�� � ���μ������� ȣ��Ǵ����� �������
// ���������� ���������� BIOS�� ���װ� �ִ� ��쿡�� ȣ��Ǵ�
// ���μ����� ���� �ٸ� ���� ���ϵǴ� ��찡 �ִ�.

// GetProcessAffinityMask() �װ� �� �༮�� ������ �ִµ�...
// �Լ� ��ü�� ������ Ŀ���� �̿��Ͽ� �ű⼭�� ����ǰ� �Ѵ�.
// ������ CPU�� ���� �ٸ� �Լ��� �ش� Ŀ���� �̹� �����ϰ� �ִٸ�
// Ŀ�� ������ �Ұ��� -> ���� �����ϰ� �����ϱ� ���� Ŭ����
class SingleCore
{
public:
	class SingleCoreError
	{
	};

	ULONG_PTR oam_;

	SingleCore()
	{
		ULONG_PTR pam, sam;
		if (!GetProcessAffinityMask(GetCurrentProcess(), &pam, &sam))
			throw SingleCoreError();

		ULONG_PTR am = 1;
		int bits = CHAR_BIT * sizeof(am);

		for (int i = 0; i<bits; ++i)
		{
			if (am & pam)
			{
				oam_ = SetThreadAffinityMask(GetCurrentThread(), am);
				if (!oam_)
					throw SingleCoreError();

				break;
			}

			am <<= 1;
		}
	}

	~SingleCore()
	{
		SetThreadAffinityMask(GetCurrentThread(), oam_);
	}
};


class KCore;
class TimeManager
{
private:
	TimeManager();
	~TimeManager();


private:
	friend KCore;

public:
	// �ʴ� �������� ����� ������.
	class GameTimer : public SmartPtr
	{
	public:
		friend TimeManager;

	private:
		LARGE_INTEGER m_SecCount; // cpu �� 1�ʿ� ��� ���ڸ� ������.
		LARGE_INTEGER m_CurCount;
		LARGE_INTEGER m_NextCount;
		float m_DeltaTime;
		float m_AccFPS;
		int m_FPSCount;
		int m_Fps;

	public:
		void Init();
		float Update();

	public:
		GameTimer();
		~GameTimer();

	};

private:
	static GameTimer MainTimer;
	static std::unordered_map<std::wstring, KPtr<GameTimer>> m_MapTimer;

public:
	static KPtr<GameTimer> FindTimer(const wchar_t* _Name);
	static KPtr<GameTimer> CreatTimer(const wchar_t* _Name);

private:
	static void Init();
	static void Update();

public:
	static inline float DeltaTime()
	{
		return MainTimer.m_DeltaTime;
	}

	static inline int Fps()
	{
		return (int)MainTimer.m_Fps;
	}
};


