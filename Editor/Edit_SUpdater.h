#pragma once
#include <State.h>

class Edit_SUpdater : public State::State_Updater
{

public:
	// State_Updater을(를) 통해 상속됨
	virtual void Update_Edit() override;

	// State_Updater을(를) 통해 상속됨
	virtual void Start_State() override;

public:
	Edit_SUpdater();
	~Edit_SUpdater();
};

