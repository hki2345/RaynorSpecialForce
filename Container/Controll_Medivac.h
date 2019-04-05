#pragma once
#include <Component.h>
#include "DXContainer.h"


class Force_Unit;
class Renderer_Terrain;
class Renderer_BonAni;
class SoundPlayer;
class Controll_Medivac : public Component
{
public:
	Controll_Medivac();
	~Controll_Medivac();


public:
	enum MOVE_TYPE
	{
		MT_IDLE = 0,
		MT_LAND,
	};

private:
	bool m_MirrorY;

	KPtr<SoundPlayer> m_pSound;
	KPtr<Force_Unit>		m_pUnit;

	MOVE_TYPE	m_MType;

	KVector		m_TPos;
	KVector		m_PlayRot;
	KVector		m_PlayPos;
	KVector		m_RenderRot;

	float m_UTime;

private:
	void Update_MIDLE();
	void Update_LAND();


public:
	bool Init(const KVector& _InitPos, const KVector& _Rot, KPtr<Renderer_Terrain> _Ter);
	void Update();
};

