#pragma once
#include <Component.h>
#include <list>

class Force_Unit;
class SC2_Camera;
class Renderer_Terrain;
class Renderer_BonAni;
class Renderer_UI;
class KCollision;
class KFont;
class KRay3D;
class SoundPlayer;
class Controll_User : public Component
{
public:
	Controll_User();
	~Controll_User();


public:

	enum STORY_TYPE
	{
		ST_NONE = 0,
		ST_TYCHUS,
		ST_NOVA,
		ST_COMMAND,
	};

	enum OPTI_TYPE
	{
		OT_ODIN = 0,
		OT_NOVA,
		OT_HYPERION,		
	};

	enum MOVE_TYPE
	{
		MT_NONE = 0,
		MT_IDLE,
		MT_MOVE,
		MT_RUN,
	};

	enum ACT_TYPE
	{
		AT_IDLE = 0,
		AT_ATTACK,
		AT_BOMB,
		AT_STORY,
		AT_HEAL,
		AT_OPTI,

		AT_DEATH,
	};

private:
	bool m_ASound;
	bool m_MSound;

	bool m_MirrorY;

	std::list<KPtr<Renderer_BonAni>> m_RList;
	std::list<KPtr<Renderer_BonAni>>::iterator m_SRI;
	std::list<KPtr<Renderer_BonAni>>::iterator m_ERI;

	KPtr<Force_Unit>		m_pUnit;
	KPtr<SC2_Camera>		m_pCam;
	KPtr<SoundPlayer>		m_pSound;

	
	KPtr<Renderer_UI>		m_uMedic;
	KPtr<Renderer_UI>		m_uFire;
	KPtr<Renderer_UI>		m_uOpti;


	KPtr<Renderer_UI>		m_uCMedic;
	KPtr<Renderer_UI>		m_uCFire;
	KPtr<Renderer_UI>		m_uCOpti;


	KPtr<Renderer_UI>		m_uHp;
	KPtr<Renderer_UI>		m_uBHP;

	KPtr<Renderer_UI>		m_uFocusHP;
	KPtr<Renderer_UI>		m_uFBackHP;
	KPtr<Renderer_UI>		m_uAim;
	KPtr<Renderer_UI>		m_uConsole;
	KPtr<Renderer_UI>		m_uFKey;
	KPtr<Renderer_UI>		m_uCover;


	MOVE_TYPE	m_MType;
	ACT_TYPE	m_AType;
	OPTI_TYPE	m_OType;
	STORY_TYPE	m_SType;

	KVector		m_PlayRot;
	KVector		m_PlayPos;
	KVector		m_RenderRot;

	KPtr<Force_Unit>		m_pFocusUnit;
	KPtr<Force_Unit>		m_pStoryUnit;
	KPtr<KFont>				m_pFont;
	KPtr<KRay3D> RayCol;


	float m_fCover;
	float m_UTime;


	float m_FiTime;
	float m_MeTime;
	float m_OpTime;

	float m_FiATime;
	float m_MeATime;
	float m_OpATime;


	int m_iStory;
	float m_GameTime;
	bool m_InGame;
	bool m_OutGame;
	bool m_OutSound;

private:
	void Init_UI();
	void Update_UI();
	void Update_AUI();


	void Init_Game();
	void Update_Game();
	void Update_Story();


	void Update_GameCol(KCollision* _Left, KCollision* _Right);
	void Update_GExitCol(KCollision* _Left, KCollision* _Right);
	void Update_StayCol(KCollision* _Left, KCollision* _Right);
	void Update_ExitCol(KCollision* _Left, KCollision* _Right);
	

	void Update_RenCol();
	void Update_Move();
	void Update_Act();
	void Update_Mouse();


	void Update_MIDLE();
	void Update_MOVE();
	void Update_RUN();
	void Udpate_DEATH();

	void Update_AIDLE();
	void Update_ATTACK();
	void Update_BOMB();
	void Update_STORY();
	void Update_HEAL();
	void Update_OPTI();

	void Update_DEATH();

	void Update_Terrain();

	void UIRender();

public:
	KVector& pos_player()
	{
		return m_PlayPos;
	}

	KVector& rot_player()
	{
		return m_PlayRot;
	}
	KVector& rot_render()
	{
		return m_RenderRot;
	}

	void Set_Render();
	void Add_Render(KPtr<Renderer_BonAni> _Other);
	void Del_Render(KPtr<Renderer_BonAni> _Other);

	bool Init(KPtr<Force_Unit>	_Unit, KPtr<SC2_Camera> _Cam);
	void Update();
};

