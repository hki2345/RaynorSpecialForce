#pragma once
#include <State.h>



class GBuilder : public State::State_Builder
{
public:
	GBuilder();
	~GBuilder();

public:
	void Build_State() override;
};

