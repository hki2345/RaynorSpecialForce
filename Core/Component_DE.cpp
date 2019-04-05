#include "Component_DE.h"
#include "TransPosition.h"
#include "KMacro.h"


Component_DE::Component_DE()
{
}


Component_DE::~Component_DE()
{
}

void Component_DE::ComInit()
{
	m_Trans = Get_Component<TransPosition>();
}