#include "SmartPtr.h"

SmartPtr::SmartPtr(const SmartPtr& _Otehr) : m_Count(0)
{

}

SmartPtr::SmartPtr() : m_Count(0)
{
}


SmartPtr::~SmartPtr()
{
}

void SmartPtr::Add_Cnt() {
	++m_Count;
}

void SmartPtr::Dec_Cnt() {
	--m_Count;

	if (0 >= m_Count)
	{
		delete this;
	}
}