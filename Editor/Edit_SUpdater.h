#pragma once
#include <State.h>

class Edit_SUpdater : public State::State_Updater
{

public:
	// State_Updater��(��) ���� ��ӵ�
	virtual void Update_Edit() override;

	// State_Updater��(��) ���� ��ӵ�
	virtual void Start_State() override;

public:
	Edit_SUpdater();
	~Edit_SUpdater();
};

