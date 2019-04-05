#pragma once
#include "afxwin.h"
#include "TabDlg.h"


// Dlg_UnitList ��ȭ �����Դϴ�.
class Dlg_Terrain;
class Dlg_UnitList : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_UnitList)

public:
	Dlg_UnitList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_UnitList();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITLISTDLG };
#endif

private:
	Dlg_Terrain* m_MofUList;
	CListBox m_Ulist;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void Init_Dlg(TabDlg* _Other);
	afx_msg void OnLbnDblclkUnitlist();
};
