#pragma once
#include "Begin.h"

class Begin_Updater : public Begin
{
private:
	bool m_bUpdate;
	bool m_bDeath;

public:
	virtual bool Is_Active()
	{
		return false == m_bDeath && true == m_bUpdate;
	}

	virtual bool Is_Death()
	{
		return true == m_bDeath;
	}


	// 데드를 시키면 실제로 사라지는 것이 아닌
	// nullptr로 되어 접근을 못하게 하는 형식 -> 
	// 스마트 포인터에서 더 이상 쓰이는 곳이 없게 될 때 삭제 시킴
	// 지워질 떄 모두 없앰
	virtual void Set_Death()
	{
		m_bDeath = true;
	}



	virtual void Active_On()
	{
		m_bUpdate = true;
	}

	virtual void Active_Off()
	{
		m_bUpdate = false;
	}

public:
	virtual bool Init();
	virtual void PrevUpdate();
	virtual void Update();
	virtual void NextUpdate();
	virtual void DebugUpdate();
	virtual void DebugRender();
	virtual void UIRender();

protected:
	Begin_Updater();
	~Begin_Updater();
};

