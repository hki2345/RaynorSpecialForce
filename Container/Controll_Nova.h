#pragma once
#include <Component.h>
#include "DXContainer.h"
#include <list>


class Force_Unit;
class Renderer_Terrain;
class Renderer_BonAni;
class Renderer_AniEffect;
class SoundPlayer;
class Controll_Nova : public Component
{
public:
	Controll_Nova();
	~Controll_Nova();

public:
	enum MOVE_TYPE
	{
		MT_IDLE = 0,
		MT_ATTACK,
	};

private:
	bool m_MirrorY;

	KPtr<SoundPlayer> m_pSound;
	KPtr<Force_Unit>		m_pUnit;
	std::list<KPtr<Force_Unit>>				m_pEnemyList;
	std::list<KPtr<Renderer_AniEffect>>		m_pTargetList;
	std::list<KPtr<Renderer_AniEffect>>		m_pMTargetList;

	MOVE_TYPE	m_MType;

	KVector		m_TPos;
	KVector		m_PlayRot;
	KVector		m_PlayPos;
	KVector		m_RenderRot;

	float m_UTime;
	float m_ATime;

private:
	void Update_UI();

	void Update_MIDLE();
	void Update_ATTACK();


public:
	bool Init(const KVector& _InitPos, const KVector& _RotPos, KPtr<Renderer_Terrain> _Ter);
	void Init_UI();
	void Update();

};

