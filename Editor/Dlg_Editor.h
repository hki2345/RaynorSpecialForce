#pragma once
#include "TabDlg.h"

// Dlg_Editor 대화 상자입니다.


class KCollision;
class Dlg_Editor : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Editor)

public:
	Dlg_Editor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_Editor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLDLG };
#endif

protected:
	void Init_Dlg();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void Collision_Test(KCollision*, KCollision*);
};
