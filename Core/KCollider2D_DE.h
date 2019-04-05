#pragma once
#include "KCollision.h"

class KCollider2D_DE : public KCollision
{
protected:
	bool m_SizeSelf;

	KVector2 Pivot;
	KVector2 Size;

public:
	void PosSetting();
	void SizeSetting();

public:
	KCollider2D_DE();
	~KCollider2D_DE();
};

