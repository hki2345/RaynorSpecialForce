#pragma once
#include "KCollider3D_DE.h"


class KSphere_Col : public KCollider3D_DE
{
public:
	KSphereCon* pSphere;

public:
	void Update_Figure() override;
	void DebugRender() override;

public:
	KSphere_Col();
	~KSphere_Col();
};

