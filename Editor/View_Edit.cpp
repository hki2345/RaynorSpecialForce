// View_Edit.cpp : 구현 파일입니다.
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


// View_Edit 그리기입니다.

void View_Edit::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// View_Edit 진단입니다.

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


// View_Edit 메시지 처리기입니다.


BOOL View_Edit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	MDlg.Create(IDD_MENUDLG, this);
	MDlg.ShowWindow(SW_SHOW);

	return 0;
}
