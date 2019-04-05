#pragma once
#include "afxwin.h"
#include "TabDlg.h"


// Dlg_UnitList 대화 상자입니다.
class Dlg_Terrain;
class Dlg_UnitList : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_UnitList)

public:
	Dlg_UnitList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_UnitList();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITLISTDLG };
#endif

private:
	Dlg_Terrain* m_MofUList;
	CListBox m_Ulist;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void Init_Dlg(TabDlg* _Other);
	afx_msg void OnLbnDblclkUnitlist();
};
