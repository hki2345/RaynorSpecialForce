#pragma once
#include "SC2Manager.h"
#include "SC2_Force.h"


class Con_Class
{
private:
	static KPtr<SC2Manager> m_S2Manager;
	static KPtr<SC2_Force>	m_fPlayer;
	static KPtr<SC2_Force>	m_fEnemy;

public:
	static void s2_manager(KPtr<SC2Manager> _Manager)
	{
		m_S2Manager = _Manager;
	}

	static KPtr<SC2Manager> s2_manager()
	{
		if (nullptr == m_S2Manager)
		{
			m_S2Manager = new SC2Manager();
		}

		return m_S2Manager;
	}



	static void force_player(KPtr<SC2_Force> _Other)
	{
		m_fPlayer = _Other;
	}
	static void force_player(const wchar_t* _Name)
	{
		m_fPlayer = m_S2Manager->Find_Force(_Name);
	}

	static KPtr<SC2_Force> force_player()
	{
		return m_fPlayer;
	}



	static void force_enemy(KPtr<SC2_Force> _Other)
	{
		m_fEnemy = _Other;
	}

	static KPtr<SC2_Force> force_enemy()
	{
		return m_fEnemy;
	}


public:
	Con_Class();
	~Con_Class();
};

