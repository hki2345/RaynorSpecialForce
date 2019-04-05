#pragma once
#include <Component.h>

class ComDlg : public CDialogEx
{
public:
	UINT Y_SIZE;

public:
	virtual void set_component(Component* _Com) {};

public:
	ComDlg(int _id, CWnd* _pWnd) : CDialogEx(_id, _pWnd) {}
	~ComDlg() {};

	virtual BOOL OnInitDialog();
};

