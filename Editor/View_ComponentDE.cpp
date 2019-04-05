#include "stdafx.h"
#include "resource.h"
#include <Component.h>

#include "View_Component.h"
#include "Dlg_TransPosition.h"



ComDlg* View_Component::CreateDlg(const type_info* _Type)
{
	ComDlg* NewDlg = nullptr;

	if (_Type == &typeid(TransPosition))
	{
		NewDlg = new Dlg_TransPosition();
		NewDlg->Create(IDD_DLGTRANS, this);
	}

	return NewDlg;
}