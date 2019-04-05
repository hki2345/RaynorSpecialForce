#include "TransPosition.h"
#include "TheOne.h"



bool TransPosition::IsMulti(TheOne* _pActor) {

	if (nullptr != _pActor->Get_Component<TransPosition>())
	{
		return false;
	}

	return true;
}

TransPosition::TransPosition() : m_LScale(1.0f), m_bLS(true), m_bLR(true), m_bLP(true), m_bW(true)
{
	m_LAx[AXIS::AX_F] = KVector4::Forword;
	m_LAx[AXIS::AX_U] = KVector4::Up;
	m_LAx[AXIS::AX_R] = KVector4::Right;

	m_WAx[AXIS::AX_F] = KVector4::Forword;
	m_WAx[AXIS::AX_U] = KVector4::Up;
	m_WAx[AXIS::AX_R] = KVector4::Right;
}


TransPosition::~TransPosition()
{
}

void TransPosition::Reset()
{
	m_LPos = 0.0f;
	m_LRot = 0.0f;
	m_LScale = 1.0f;
	m_bLS, m_bLR, m_bLP, m_bW = true;

	m_LAx[AXIS::AX_F] = KVector4::Forword;
	m_LAx[AXIS::AX_U] = KVector4::Up	;
	m_LAx[AXIS::AX_R] = KVector4::Right;

	m_WAx[AXIS::AX_F] = KVector4::Forword;
	m_WAx[AXIS::AX_U] = KVector4::Up;
	m_WAx[AXIS::AX_R] = KVector4::Right;
}

bool TransPosition::Init()
{
	one()->Trans(this);
	return true;
}

void TransPosition::FinalUpdate()
{
	if (true == m_bLS)
	{
		m_SMat.Scale(m_LScale); // 크
		m_bLS = false;
	}

	if (true == m_bLR)
	{
		m_RMatX.RotX(m_LRot.x); // X
		m_RMatY.RotY(m_LRot.y); // Y
		m_RMatZ.RotZ(m_LRot.z); // Z
							   // 자전
		m_RMat = m_RMatX * m_RMatY * m_RMatZ;

		m_LAx[AXIS::AX_F] = KVector4::Forword;
		m_LAx[AXIS::AX_U] = KVector4::Up;
		m_LAx[AXIS::AX_R] = KVector4::Right;
		for (size_t i = 0; i < AXIS::AX_MAX; i++)
		{
			m_LAx[i] = m_RMat.MulVecZero(m_LAx[i]);
			m_LAx[i].NormalizeVec3();
		}

		if (nullptr != one()->Parent())
		{
			m_WRot = one()->Trans()->rotate_world() + m_LRot;

			KMatrix WRotMat;
			KMatrix TempWRotMatX;
			KMatrix TempWRotMatY;
			KMatrix TempWRotMatZ;

			TempWRotMatX.RotX(m_WRot.x); // X
			TempWRotMatY.RotY(m_WRot.y); // Y
			TempWRotMatZ.RotZ(m_WRot.z); // Z
										 // 자전
			WRotMat = TempWRotMatX * TempWRotMatY * TempWRotMatZ;

			m_WAx[AXIS::AX_F] = KVector4::Forword;
			m_WAx[AXIS::AX_U] = KVector4::Up;
			m_WAx[AXIS::AX_R] = KVector4::Right;

			for (size_t i = 0; i < AXIS::AX_MAX; i++)
			{
				m_WAx[i] = WRotMat.MulVecZero(m_WAx[i]);
				m_WAx[i].NormalizeVec3();
			}
		}
		else {
			for (size_t i = 0; i < AXIS::AX_MAX; i++)
			{
				m_WAx[i] = m_LAx[i];
				m_WAx[i].NormalizeVec3();
			}
		}

		m_bLR = false;
	}

	if (true == m_bLP)
	{
		m_PMat.Translate(m_LPos); // 이
		m_bLP = false;
	}

	if (nullptr == one()->Parent())
	{
		if (true == m_bW)
		{
			m_WMat = m_SMat * m_RMat * m_PMat;

			m_WPos = m_WMat.v4;
			m_WScale = KVector4(m_WMat.v1.x, m_WMat.v2.y, m_WMat.v3.z);
		}
	}
	else if (nullptr != one()->Parent()->Trans())
	{
		if (true == m_bW || true == one()->Parent()->Trans()->m_bW)
		{
			KMatrix WW = one()->Parent()->Trans()->matrix_world();
			m_WMat = m_SMat * m_RMat * m_PMat * one()->Parent()->Trans()->matrix_world();

			m_WPos = m_WMat.v4;
			m_WScale = KVector4(m_WMat.v1.x, m_WMat.v2.y, m_WMat.v3.z);
		}
	}
}

void TransPosition::EndUpdate() 
{
	m_bW = false;
}