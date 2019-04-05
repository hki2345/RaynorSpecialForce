#include "HSubTransCom.h"
#include "TransPosition.h"
#include "KMacro.h"


HSubTransCom::HSubTransCom()
	: m_eMode(HSubTransCom::NONE)
	, UScale(1.0f), m_Rot(0.0f), m_Pivot(0.0f)
{
	m_SubWMat.Identity();
}


HSubTransCom::~HSubTransCom()
{
}

void HSubTransCom::SubTransUpdate() 
{
	// �θ��� ����� �����ָ� �Ǵ°� �ȵǴ°�?
	switch (m_eMode)
	{
	case HSubTransCom::NONE:
		m_SubWMat = m_Trans->worldmat_const();
		break;
	case HSubTransCom::PARENT:
		m_SMat.Scale(UScale); // ũ
		m_RMatX.RotX(m_Rot.x); // X
		m_RMatY.RotY(m_Rot.y); // Y
		m_RMatZ.RotZ(m_Rot.z); // Z
		m_RMat = m_RMatX * m_RMatY * m_RMatZ;
		m_PMat.Translate(m_Pivot); // ��
		m_SubWMat = m_SMat * m_RMat * m_PMat * m_Trans->worldmat_const();
		break;
	case HSubTransCom::SELF:
		m_SMat.Scale(UScale); // ũ
		m_RMatX.RotX(m_Rot.x); // X
		m_RMatY.RotY(m_Rot.y); // Y
		m_RMatZ.RotZ(m_Rot.z); // Z
		m_RMat = m_RMatX * m_RMatY * m_RMatZ;
		m_PMat.Translate(m_Pivot + m_Trans->pos_world()); // ��
		m_SubWMat = m_SMat * m_RMat * m_PMat;
		break;
	default:
		break;
	}
}

void HSubTransCom::ComInit()
{
	Component_DE::ComInit();

	KASSERT(nullptr == m_Trans);

	if (nullptr == m_Trans)
	{
		return;
	}
}