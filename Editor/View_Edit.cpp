// View_Edit.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEditor.h"
#include "View_Edit.h"


// View_Edit

IMPLEMENT_DYNCREATE(View_Edit, CView)

View_Edit::View_Edit()
{

}

View_Edit::~View_Edit()
{
}

BEGIN_MESSAGE_MAP(View_Edit, CView)
END_MESSAGE_MAP()


// View_Edit �׸����Դϴ�.

void View_Edit::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// View_Edit �����Դϴ�.

#ifdef _DEBUG
void View_Edit::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void View_Edit::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// View_Edit �޽��� ó�����Դϴ�.


BOOL View_Edit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	MDlg.Create(IDD_MENUDLG, this);
	MDlg.ShowWindow(SW_SHOW);

	return 0;
}
