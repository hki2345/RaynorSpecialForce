#include "KCollision.h"
#include "KMath.h"

bool(*KCollision::ColFunc[CT_MAX][CT_MAX])(const Figure_Col* _Left, const Figure_Col* _Right);

void KCollision::ColInit() {

	for (size_t y = 0; y < CT_MAX; ++y)
	{
		for (size_t x = 0; x < CT_MAX; ++x)
		{
			ColFunc[y][x] = nullptr;
		}
	}

	ColFunc[CT_RECT2D][CT_RECT2D] = &Funtion_Col::RectToRectFi;
	ColFunc[CT_CIRCLE2D][CT_CIRCLE2D] = &Funtion_Col::CirCleToCirCleFi;
	ColFunc[CT_CIRCLE2D][CT_POINT2D] = &Funtion_Col::CirCleToPointFi;
	ColFunc[CT_POINT2D][CT_CIRCLE2D] = &Funtion_Col::PointToCirCleFi;
	ColFunc[CT_RECT2D][CT_POINT2D] = &Funtion_Col::RectToPointFi;
	ColFunc[CT_POINT2D][CT_RECT2D] = &Funtion_Col::PointToRectFi;
	ColFunc[CT_RECT2D][CT_CIRCLE2D] = &Funtion_Col::RectToCirCleFi;
	ColFunc[CT_CIRCLE2D][CT_RECT2D] = &Funtion_Col::CirCleToRectFi;

	ColFunc[CT_SPHERE3D][CT_SPHERE3D] = &Funtion_Col::SphereToSphereFunc;
	ColFunc[CT_SPHERE3D][CT_RAY3D] = &Funtion_Col::SphereToRayFunc;
	ColFunc[CT_RAY3D][CT_SPHERE3D] = &Funtion_Col::RayToSphereFunc;
	ColFunc[CT_PLANE3D][CT_RAY3D] = &Funtion_Col::PlaneToRayFunc;
	ColFunc[CT_RAY3D][CT_PLANE3D] = &Funtion_Col::RayToPlaneFunc;
	ColFunc[CT_OBB3D][CT_RAY3D] = &Funtion_Col::OBBToRayFunc;
	ColFunc[CT_RAY3D][CT_OBB3D] = &Funtion_Col::RayToOBBFunc;
	ColFunc[CT_OBB3D][CT_OBB3D] = &Funtion_Col::OBBToOBBFunc;
}

KCollision::KCollision()
{
}


KCollision::~KCollision()
{
	if (nullptr != m_Fi)
	{
		delete m_Fi;
	}
}

void KCollision::Update_Figure() {

}

void KCollision::ColCheck(KCollision* _Col) {

	if (nullptr == ColFunc)
	{
		return;
	}

 	bool Check = FiColCheck(_Col->m_Fi);

	if (true == Check)
	{
		ColFindIter = m_ColSet.find(_Col);
		// Enter
		if (ColFindIter == m_ColSet.end())
		{
			m_ColSet.insert(_Col);
			_Col->m_ColSet.insert(this);
			CallEnterList(_Col);
		}

		// Stay
		else {
			CallStayList(_Col);
		}
	}
	else 
	{
		ColFindIter = m_ColSet.find(_Col);
	
		// Exit
		if (ColFindIter != m_ColSet.end())
		{
			m_ColSet.erase(_Col);
			_Col->m_ColSet.erase(this);
			CallExitList(_Col);
		}
	}
	return;
}

bool KCollision::FiColCheck(const Figure_Col* _Col)
{
	if (nullptr == _Col)
	{
		return false;
	}

	if (_Col->m_ColType == CT_MAX || m_Fi->m_ColType == CT_MAX)
	{
		return false;
	}

	return ColFunc[m_Fi->m_ColType][_Col->m_ColType](m_Fi, _Col);
}

bool KCollision::Init()
{
	return true;
}

void KCollision::CallEnterList(KCollision* _Right)
{
	m_EnSI = m_EnterFuncMap.begin();
	m_EnEI = m_EnterFuncMap.end();

	for (; m_EnSI != m_EnEI; ++m_EnSI)
	{
		m_EnSI->second(this, _Right);
	}
}
void KCollision::CallStayList(KCollision* _Right) 
{
	m_SSI = m_StayFuncMap.begin();
	m_SEI = m_StayFuncMap.end();

	for (; m_SSI != m_SEI; ++m_SSI)
	{
		m_SSI->second(this, _Right);
	}
}

void KCollision::CallExitList(KCollision* _Right) {
	m_ExSI = m_ExitFuncMap.begin();
	m_ExEI = m_ExitFuncMap.end();

	for (; m_ExSI != m_ExEI; ++m_ExSI)
	{
		m_ExSI->second(this, _Right);
	}
}

void KCollision::DeathRelease() 
{
	std::set<KCollision*>::iterator StartIter = m_ColSet.begin();
	std::set<KCollision*>::iterator EndIter = m_ColSet.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		(*StartIter)->m_ColSet.erase(this);
	}

}