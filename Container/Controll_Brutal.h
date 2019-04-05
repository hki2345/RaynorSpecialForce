#pragma once
#include "Controll_AI.h"


class Controll_Brutal : public Controll_AI
{
public:
	Controll_Brutal();
	~Controll_Brutal();

private:
	KPtr<Renderer_BonAni> DeathRender;



	void Init_Value() override;
	void Update_MIDLE() override;
	void Update_MOVE() override;

	void Update_AIDLE() override;

	void Update_BURROW() override;
	void Update_HIDE() override;
	void Update_UNBURROW() override;

	void Update_ATTACK01() override;
	void Update_ATTACK02() override;
	void Update_ATTACK03() override;
	void Update_DEATH() override;
};

