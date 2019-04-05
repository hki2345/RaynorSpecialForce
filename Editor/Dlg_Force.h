#pragma once
#include "TabDlg.h"
#include "afxwin.h"
#include "afxcolorbutton.h"


// Dlg_Force 대화 상자입니다.
class SC2_Force;
class Dlg_Force : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Force)

public:
	Dlg_Force(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_Force();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORCEDLG };
#endif


private:
	CListBox m_KM3List;
	CListBox m_ForceList;
	CEdit m_ForceName;
	CMFCColorButton m_ForceColor;

	KPtr<TheOne> m_CurOne;
	KPtr<SC2_Force> m_CurForce;


	std::list<KPtr<TheOne>> m_OList;
	std::list<KPtr<TheOne>>::iterator m_SO;
	std::list<KPtr<TheOne>>::iterator m_EO;

private:
	void Init_Dlg();
	void Init_KM3List();
	void Init_ForceList();

	void Update_Force();
	

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedForcecolor();
	afx_msg void OnLbnSelchangeForcelist();
	afx_msg void OnEnChangeForcename();
	afx_msg void OnLbnSelchangeForkm3list();
	afx_msg void OnBnClickedKm3resetbtn();
};
