#pragma once
#include "KCollider2D_DE.h"

class KCollider2D : public KCollider2D_DE
{
private:
	Figure2D_DE* m_2DCol;

public:
	bool Init(int _Order = 0);
	void Update_Figure() override;
	void Mode(COLTYPE _Type);
	void Radius(KVector2 _Size)
	{
		m_SizeSelf = true;
		m_2DCol->m_Vec.m_Size = _Size;
	}
	void Radius(float _Radius) 
	{
		m_SizeSelf = true;
		m_2DCol->m_Vec.m_Vec3.Radius = _Radius;
	}

public:
	void DebugRender() override;

public:
	KCollider2D();
	~KCollider2D();
};

