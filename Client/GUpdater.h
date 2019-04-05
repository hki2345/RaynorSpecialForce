#pragma once
#include <State.h>


class Renderer_Terrain;
class Renderer_Mesh;
class Renderer_UI;
class SC2_Camera;
class GUpdater : public State::State_Updater
{
public:
	GUpdater();
	~GUpdater();


private:
	float m_GTime;

	KPtr<Renderer_UI>		m_uCover;
	KPtr<SC2_Camera>		m_pCam;
	KPtr<Renderer_Terrain> m_pTer;
	KPtr<Renderer_Mesh> SkySphere;

private:
	void Init_Terrain();
	void Init_Mesh();
	void Init_Unit();

public:
	virtual void End_State() override;
	virtual void  Start_State() override;
	virtual void  Update_State() override;
	virtual void UIRender() override;
};

