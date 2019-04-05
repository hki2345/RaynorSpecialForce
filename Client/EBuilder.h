#pragma once
#include <State.h>


class EBuilder : public State::State_Builder
{
public:
	EBuilder();
	~EBuilder();


public:
	void Build_State() override;
};

