#pragma once
#include "TabDlg.h"
#include "afxcmn.h"



// Dlg_MeshConv 대화 상자입니다.

class Dlg_MeshConv : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_MeshConv)

public:
	Dlg_MeshConv(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_MeshConv();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHDLG };
#endif

private:
	std::vector<TabDlg*> m_DlgVec;
	int m_CurIdx;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void Show_Dlg(const int& _Index);
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeFbx2km3tab(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_MeshTab;
};
