#pragma once
#include "Component_DE.h"


// 이걸 상속받아 씀
class SomeOne : public Component_DE
{
public:
	SomeOne();
	virtual ~SomeOne() = 0;
};

