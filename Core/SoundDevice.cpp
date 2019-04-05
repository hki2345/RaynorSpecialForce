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
	// �������� �ϴ���?
	FMOD::System_Create(&SoundSystem);

	KASSERT(nullptr == SoundSystem);

	if (nullptr == SoundSystem)
	{
		return;
	}

	// ��ü�� ���Դ�.
	// �׷��� �׻� �˻��ض�.
	// Release
	// Delete
	// Create
	// Init�� ���� �ý����� �غ��ϴ� �Լ��̴�.
	// 1��° ���� ���ÿ� � ���带 ���???
	// 2��° � ������� ���鷹. 0������ �׳� ����ϰ�
	// 3��° ����  ���� ��ġ ���ö� Ư���ϰ� �����������Ұ� �ֳ�
	// ��ġ�� �������.
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
		// �ݹ��Լ� ȣ���� ����������.
		SoundSystem->update();
	}
}