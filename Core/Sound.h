#pragma once
#include "Resource.h"
#include "SoundDevice.h"

class Sound final : public KResource
{
private:
	friend class SoundPlayer;

private:
	FMOD::Sound* m_pSound;

public:
	bool Load();


public:
	Sound();
	~Sound();
};

