// View_Component.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "View_Component.h"
#include "Edit_Class.h"



// View_Component

IMPLEMENT_DYNCREATE(View_Component, CScrollView)

View_Component::View_Component() : m_pActor(nullptr)
{
	Edit_Class::m_gVIewCom = this;
}

View_Component::~View_Component()
{
	Release_AllDlg();
}


BEGIN_MESSAGE_MAP(View_Component, CScrollView)
END_MESSAGE_MAP()


// View_Component 그리기입니다.

void View_Component::Release_AllDlg() 
{
	m_pActor = nullptr;
	StartDlgiter = m_ComDlgList.begin();
	EndDlgiter = m_ComDlgList.end();

	for (; StartDlgiter != EndDlgiter; ++StartDlgiter)
	{
		if (nullptr != (*StartDlgiter))
		{
			if (nullptr != (*StartDlgiter)->m_hWnd)
			{
				(*StartDlgiter)->ShowWindow(SW_HIDE);
			}
			delete (*StartDlgiter);
		}
	}

	m_ComDlgList.clear();
}

void View_Component::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);

}

void View_Component::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// View_Component 진단입니다.

#ifdef _DEBUG
void View_Component::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void View_Component::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// View_Component 메시지 처리기입니다.
void View_Component::Set_One(TheOne* _pActor)
{
	m_pActor = _pActor;

	// Reset();
}


void View_Component::Reset() 
{
	TheOne* m_TempActor = m_pActor;

	Release_AllDlg();

	m_pActor = m_TempActor;

	if (nullptr == m_pActor)
	{
		return;
	}

	std::list<KPtr<Component>> List = m_pActor->list_component();

	std::list<KPtr<Component>>::iterator StartIter = List.begin();
	std::list<KPtr<Component>>::iterator EndIter = List.end();

	POINT Pos = { 0, 0 };
	POINT Size = { 480, 0 };

	SIZE sizeTotal;

	sizeTotal.cx = Size.x;
	sizeTotal.cy = Pos.y;
	SetScrollSizes(MM_TEXT, sizeTotal);
}