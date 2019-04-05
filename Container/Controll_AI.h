#pragma once
#include <Component.h>
#include "DXContainer.h"



class Force_Unit;
class Renderer_Terrain;
class Renderer_BonAni;
class SoundPlayer;
class Controll_AI : public Component
{
public:
	Controll_AI();
	~Controll_AI();


public:
	enum MOVE_TYPE
	{
		MT_IDLE = 0,
		MT_MOVE,
	};

	enum ACT_TYPE
	{
		AT_IDLE = 0,


		AT_BURROW,
		AT_HIDE,
		AT_UNBURROW,

		AT_ATTACK01,
		AT_ATTACK02,
		AT_ATTACK03,
		
		AT_DEATH,
	};

protected:
	bool m_ASound;
	bool m_Attack;
	float m_UTime;
	float m_ATime;

	float m_MRange;
	float m_ARange;

	std::list<KPtr<Renderer_BonAni>> m_RList;
	std::list<KPtr<Renderer_BonAni>>::iterator m_SRI;
	std::list<KPtr<Renderer_BonAni>>::iterator m_ERI;

	KPtr<Force_Unit>		m_pUnit;
	KPtr<Force_Unit>		m_pPUnit;

	MOVE_TYPE	m_MType;
	ACT_TYPE	m_AType;

	KVector		m_AIRot;
	KVector		m_AIPos;
	KVector		m_RenderRot;

	KPtr<SoundPlayer> m_cSound;

protected:
	virtual void Init_Value();


	void Update_Death();
	void Update_Dir();
	void Update_RenCol();
	void Update_Move();
	void Update_Act();


	virtual void Update_MIDLE() ;
	virtual void Update_MOVE();

	virtual void Update_AIDLE();

	virtual void Update_BURROW();
	virtual void Update_HIDE();
	virtual void Update_UNBURROW();

	virtual void Update_ATTACK01();
	virtual void Update_ATTACK02();
	virtual void Update_ATTACK03();
	virtual void Update_DEATH();


	void Update_Terrain();

public:
	KVector& pos_player()
	{
		return m_AIPos;
	}

	KVector& rot_player()
	{
		return m_AIRot;
	}
	KVector& rot_render()
	{
		return m_RenderRot;
	}

	void Set_Render();
	void Add_Render(KPtr<Renderer_BonAni> _Other);
	void Del_Render(KPtr<Renderer_BonAni> _Other);

	bool Init(KPtr<Force_Unit>	_Unit);
	void Update();
};

