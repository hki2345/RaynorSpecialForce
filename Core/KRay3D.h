#pragma once
#include "KCollider3D_DE.h"
#include "Camera.h"


class KRay3D : public KCollider3D_DE
{
private:
	KRayCon* pRayCon;
	Camera* pCam;
	float m_Dist;

public:
	KRayCon* ray_container()
	{
		return pRayCon;
	}

public:
	KVector direction()
	{
		return pRayCon->Dir;
	}

	virtual bool Init(int _Order = 0);

	void Set_Camera(Camera* _pCamera);
	void Set_Distance(const float& _Value)
	{
		m_Dist = _Value;
	}
	void Update() override;

	void Update_Figure() override;
	void DebugRender() override;


public:
	KRay3D();
	~KRay3D();
};

