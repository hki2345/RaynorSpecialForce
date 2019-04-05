#pragma once
#include "KCollider3D_DE.h"


class KPlane3D_Col : public KCollider3D_DE
{
public:
	KPlane3D_Col();
	~KPlane3D_Col();

private:
	KPlaneCon*	pPlane;
	KVector		m_ColPoint;

public:
	void Update() override;
	void Update_Figure() override;
	void DebugRender() override;
};

