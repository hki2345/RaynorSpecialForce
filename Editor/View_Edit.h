#pragma once
#include "Dlg_MainMenu.h"

// View_Edit 뷰입니다.

class View_Edit : public CView
{
	DECLARE_DYNCREATE(View_Edit)

protected:
	View_Edit();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~View_Edit();

private:
	Dlg_MainMenu MDlg;

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};


