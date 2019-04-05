#pragma once
#include "KCollider3D_DE.h"


class KBox_Col : public KCollider3D_DE
{
public:
	KBox_Col();
	~KBox_Col();


public:
	KBoxCon* pBox;

public:
	bool Init(int _Order = 0);

	void Update_Figure() override;
	void DebugRender() override;
};

