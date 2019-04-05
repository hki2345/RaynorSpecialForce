#pragma once
#include "Dlg_MainMenu.h"

// View_Edit ���Դϴ�.

class View_Edit : public CView
{
	DECLARE_DYNCREATE(View_Edit)

protected:
	View_Edit();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~View_Edit();

private:
	Dlg_MainMenu MDlg;

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
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


