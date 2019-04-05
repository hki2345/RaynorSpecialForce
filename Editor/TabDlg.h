#pragma once
#include "afxdialogex.h"
#include <Core_Class.h>

class TabDlg : public CDialogEx
{
public:
	CString StateName;
	bool m_StateChange;

public:
	virtual void Init_Dlg() {};
	virtual void Hide_Dlg() {};
	virtual void Update_Dlg() {};

public:
	TabDlg(const int& _id, CWnd* _Wnd);
	~TabDlg();
};

