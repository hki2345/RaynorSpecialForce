#pragma once

#ifdef X64
#pragma comment(lib, "fmod64_vc")
#else
#pragma comment(lib, "fmod_vc")
#endif

#include <fmod.hpp>

class SoundDevice
{
public:
	friend class KCore;
	friend class Sound;
	friend class SoundPlayer;

private:
	class StaticDestroy	{ 		friend SoundDevice;		~StaticDestroy() { SoundDevice::Destroy(); }		};
	friend StaticDestroy;
	static StaticDestroy Des;

private:
	static FMOD::System* SoundSystem;
	static void Init();
	static void Update();
	static void Destroy();


private:
	SoundDevice();
	~SoundDevice();
};

