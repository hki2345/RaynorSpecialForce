#include "KCollider2D.h"
#include "State.h"
#include "TransPosition.h"
#include "DebugManager.h"


KCollider2D::KCollider2D()
{
	m_2DCol = CreateColFi<Figure2D_DE>(COLTYPE::CT_RECT2D);
}


KCollider2D::~KCollider2D()
{
}

bool KCollider2D::Init(int _Order /*= 0*/)
{
	m_Order = _Order;

	state()->This_Col2DManager.PushCol(this);

	return true;
}

void KCollider2D::Update_Figure() 
{
	m_2DCol->m_Vec.m_Pos = Pivot + m_Trans->pos_world();

	if (false == m_SizeSelf) 
	{
		switch (m_2DCol->m_ColType)
		{
		case CT_RECT2D:
			m_2DCol->m_Vec.m_Size.set_vector2(fabsf(m_Trans->scale_local().x), fabsf(m_Trans->scale_local().y));
			break;
		case CT_CIRCLE2D:
			m_2DCol->m_Vec.m_Vec3.Radius = (fabsf(m_Trans->scale_local().x) * 0.5f);
			break;
		default:
			break;
		}
	} else {
		switch (m_2DCol->m_ColType)
		{
		case CT_RECT2D:
			m_2DCol->m_Vec.m_Size.set_vector2(fabsf(Size.x), fabsf(Size.y));
			break;
		case CT_CIRCLE2D:
			m_2DCol->m_Vec.m_Vec3.Radius = (fabsf(Size.x) * 0.5f);
			break;
		default:
			break;
		}
	}
}

void KCollider2D::Mode(COLTYPE _Type) 
{
	switch (_Type)
	{
	case CT_POINT2D:
	case CT_RECT2D:
	case CT_CIRCLE2D:
		m_2DCol->m_ColType = _Type;
		break;
	}
}

void KCollider2D::DebugRender() 
{
// #ifdef _DEBUG
	Update_Figure();
// #endif

	DebugManager::Draw_Rect(m_2DCol->m_Vec);
}