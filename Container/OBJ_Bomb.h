#pragma once
#include "Controll_AI.h"


class KCollision;
class OBJ_Bomb : public Controll_AI
{
public:
	OBJ_Bomb();
	~OBJ_Bomb();


private:
	KVector m_Dir;
	float m_Grav;
	float m_Spd;
	std::list<KPtr<Force_Unit>> m_TUnitList;

private:
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


	void Update_StayCol(KCollision* _Left, KCollision* _Right);	
};

