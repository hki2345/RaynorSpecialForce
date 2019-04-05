#pragma once
#include "SomeOne.h"
#include <Camera.h>
#include <DebugManager.h>

class KMesh;
class KMaterial;
class Sampler;
class Controll_User;
class Force_Unit;
class SC2_Camera : public SomeOne
{
public:
	SC2_Camera();
	~SC2_Camera();

public:
	enum SC2_CAMMODE
	{
		S2M_EDIT,
		S2M_INGAME,
		S2M_PART,
	};


private:
	KPtr<Camera>		m_Cam;
	Controll_User*		m_pUser;


	KVector2 PrevMouse;
	KVector2 RotPos;
	float m_RotSpeed;
	float m_Speed;
	float m_Boost;


	SC2_CAMMODE m_CMode;
	KVector2 OutSize;
	KVector2 OutPos;

	KPtr<KMesh> m_Mesh;
	KPtr<KMaterial> m_Mtl;
	KPtr<Sampler> m_Smp;


	// x - 현재 거리
	// y - 최소 거리
	// z - 최대 거리
	// w - 관심 거리 - target
	KVector m_PlayCamInfo;

private:
	void Update_Edit();
	void Update_InGame();
	void Update_Part();

	bool Update_ScrCheck();
	void Update_Key();
	void Update_Wheel();

public:
	KPtr<Camera> camera()
	{
		return m_Cam;
	}
	bool Init(const int& _Order = 0);
	void Update();

	void Set_Edit();
	void Set_Part();
	KPtr<Force_Unit> Set_InGame();


	void Set_User(Controll_User* _Other)
	{
		m_pUser = _Other;
	}

	SC2_CAMMODE& cam_mode()
	{
		return m_CMode;
	}


	void out_size(const KVector2& _Value)
	{
		OutSize = _Value;
	}

	void out_pos(const KVector2& _Value)
	{
		OutPos = _Value;
	}
};

