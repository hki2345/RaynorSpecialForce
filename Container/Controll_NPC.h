#pragma once
#include <Component.h>



class Force_Unit;
class Renderer_Terrain;
class Renderer_BonAni;
class Controll_NPC : public Component
{
public:
	Controll_NPC();
	~Controll_NPC();



public:
	enum ACT_TYPE
	{
		AT_IDLE = 0,
		AT_DEATH,
	};

protected:
	std::list<KPtr<Renderer_BonAni>> m_RList;
	std::list<KPtr<Renderer_BonAni>>::iterator m_SRI;
	std::list<KPtr<Renderer_BonAni>>::iterator m_ERI;

	KPtr<Force_Unit>		m_pUnit;

protected:
	void Set_Render();

public:
	bool Init(KPtr<Force_Unit>	_Unit);
	void Update();
};

