#pragma once
#include "TabDlg.h"

// Dlg_Editor ��ȭ �����Դϴ�.


class KCollision;
class Dlg_Editor : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Editor)

public:
	Dlg_Editor(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_Editor();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLDLG };
#endif

protected:
	void Init_Dlg();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void Collision_Test(KCollision*, KCollision*);
};
