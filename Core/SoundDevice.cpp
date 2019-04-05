#include "SoundDevice.h"
#include "KMacro.h"

FMOD::System* SoundDevice::SoundSystem = nullptr;

SoundDevice::StaticDestroy SoundDevice::Des;

void SoundDevice::Destroy() 
{
	if (nullptr != SoundDevice::SoundSystem)
	{
		SoundDevice::SoundSystem->release();
		SoundDevice::SoundSystem = nullptr;
	}
}

SoundDevice::SoundDevice()
{
}


SoundDevice::~SoundDevice()
{
}

void SoundDevice::Init() 
{
	// 무슨일을 하느냐?
	FMOD::System_Create(&SoundSystem);

	KASSERT(nullptr == SoundSystem);

	if (nullptr == SoundSystem)
	{
		return;
	}

	// 객체를 얻어왔다.
	// 그러면 항상 검색해라.
	// Release
	// Delete
	// Create
	// Init은 사운드 시스템을 준비하는 함수이다.
	// 1번째 인자 동시에 몇개 사운드를 재생???
	// 2번째 어떤 방식으로 만들레. 0넣으면 그냥 평범하게
	// 3번째 인자  사운드 장치 얻어올때 특수하게 나한테지시할게 있네
	// 장치를 만들었다.
	FMOD_RESULT FR = SoundSystem->init(32, FMOD_DEFAULT, nullptr);

	KASSERT(FMOD_OK != FR);

	if (FMOD_OK != FR)
	{
		return;
	}
}

void SoundDevice::Update() 
{
	if (nullptr != SoundSystem)
	{
		// 콜백함수 호출이 가능해진다.
		SoundSystem->update();
	}
}