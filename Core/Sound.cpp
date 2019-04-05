#include "Sound.h"
#include "KMacro.h"
#include <atlstr.h>

Sound::Sound() : m_pSound(nullptr)
{
}


Sound::~Sound()
{
	if (nullptr != SoundDevice::SoundSystem)
	{
		if (nullptr != m_pSound)
		{
			m_pSound->release();
		}
	}
}

bool Sound::Load() 
{
	// ���ڵ��� ��������� �Ѵ�. UTF-8 ���ڿ��� �޴´�,.
	// CP_UTF8�����ָ� �ȵƴٰ�!!!!!!!!!!
	std::string Str = CW2A(AllPath(), CP_UTF8);

	if (FMOD_OK != SoundDevice::SoundSystem->createSound(Str.c_str(), 
		FMOD_DEFAULT, nullptr, &m_pSound))
	{
		KASSERT(true);
		return false;
	}

	KASSERT(nullptr == m_pSound);

	if (nullptr == m_pSound)
	{
		return false;
	}

	return true;

}