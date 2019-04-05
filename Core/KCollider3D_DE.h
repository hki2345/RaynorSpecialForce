#pragma once
#include "KCollision.h"


class KMesh;
class KCollider3D_DE : public KCollision
{

protected:
	KVector		m_PosPivot;
	KVector		m_ScalePivot;
	KVector		m_RotPivot;
	
	KColor		m_CurColor;

public:
	void debug_color(const KVector& _Value)
	{
		m_CurColor = _Value;
	}


	void pos_pivot(const KVector& _Value)
	{
		m_PosPivot = _Value;
	}

	void rot_pivot(const KVector& _Value)
	{
		m_RotPivot = _Value;
	}

	void scale_pivot(const KVector& _Value)
	{
		m_ScalePivot = _Value;
	}

	virtual bool Init(const int& _Order = 0);

public:
	KCollider3D_DE();
	~KCollider3D_DE();
};

