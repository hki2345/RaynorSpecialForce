#pragma once
#include <State.h>


class SBuilder : public State::State_Builder
{
public:
	SBuilder();
	~SBuilder();

public:
	void Build_State() override;
};

