#pragma once
#include <Component.h>
#include "DXContainer.h"


class Force_Unit;
class Renderer_Terrain;
class Renderer_BonAni;
class SoundPlayer;
class Controll_Odin : public Component
{
public:
	Controll_Odin();
	~Controll_Odin();

public:
	enum MOVE_TYPE
	{
		MT_ATTACK = 0,
		MT_LAND,
		MT_RISE,
	};

private:
	bool m_bAttack;

	std::list<KPtr<Force_Unit>>		m_pEnemyList;
	KPtr<Force_Unit>		m_pUnit;
	KPtr<SoundPlayer>		m_pSound;

	MOVE_TYPE	m_MType;
	KVector		m_TPos;

	float m_UTime;
	float m_ATime;
	float m_Gravity;

private:
	void Update_LAND();
	void Update_ATTACK();
	void Update_RISE();

public:
	bool Init(const KVector& _InitPos, const KVector& _Rot, KPtr<Renderer_Terrain> _Ter);
	void Update();
};

