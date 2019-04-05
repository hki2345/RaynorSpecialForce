#pragma once
#include <State.h>

class KVideo;
class EUpdater : public State::State_Updater
{
public:
	EUpdater();
	~EUpdater();

private:
	KPtr<KVideo> TVideo;

	bool m_bVideo;
	float m_fUTime;

public:
	virtual void  Start_State() override;
	virtual void  Update_State() override;

};

