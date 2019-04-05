#pragma once
#include <State.h>
#include "Dlg_MainMenu.h"
#include "View_Component.h"

class Edit_Class
{
public:
	static KPtr<State> Select_State;
	static Dlg_MainMenu* MDlg;
	static View_Component* m_gVIewCom;

private:
	Edit_Class();
	~Edit_Class();
};

