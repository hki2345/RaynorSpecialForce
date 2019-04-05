#pragma once
#include <State.h>
#include <list>


class Renderer_Mesh;
class Renderer_UI;
class SoundPlayer;
class SUpdater : public State::State_Updater
{
public:
	SUpdater();
	~SUpdater();


private:
	bool m_GameLauncher;
	bool m_LSound;

	float m_LTime;


	KPtr<Renderer_Mesh> MSphere;


	std::list<KPtr<SoundPlayer>> m_SList;
	std::list<KPtr<SoundPlayer>>::iterator m_SSI;
	std::list<KPtr<SoundPlayer>>::iterator m_ESI;

	KPtr<Renderer_UI> m_Logo;
	KPtr<Renderer_UI> m_Start;
	KPtr<Renderer_UI> m_Quit;

	KPtr<SoundPlayer> SCount;
	KPtr<SoundPlayer> SCF;
	KPtr<SoundPlayer> STt;

private:
	void Init_Mesh();
	void Init_Sound();
	void Init_UI();

public:
	virtual void  Start_State() override;
	virtual void  Update_State() override;
	virtual void  UIRender() override;
};

