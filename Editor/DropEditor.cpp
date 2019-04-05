#include "stdafx.h"
#include "DropEditor.h"
#include "KEditor.h"

BEGIN_MESSAGE_MAP(DropEditor, CEdit)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

DropEditor::DropEditor() : m_bStart(false)
{
}


DropEditor::~DropEditor()
{
}

afx_msg BOOL DropEditor::OnSetCursor(CWnd* _pWnd, UINT _nHitTest, UINT _Msg) 
{
	KEditorApp* App = (KEditorApp*)AfxGetApp();

	App->MyCurSor(LoadCursor(nullptr, IDC_HAND));
	return TRUE;
}


void DropEditor::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_bStart = true;

	CurX = point.x;

	CEdit::OnLButtonDown(nFlags, point);
}


void DropEditor::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_bStart = false;

	CEdit::OnLButtonUp(nFlags, point);
}


void DropEditor::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	Parent->UpdateData(TRUE);


	if (true == m_bStart)
	{
		int Value =  CurX - point.x;

		(*pValue) -= (float)Value;

		if (nullptr != m_ValueChangeFunc)
		{
			m_ValueChangeFunc();
		}

		CurX = point.x;
	}


	Parent->UpdateData(FALSE);


	CEdit::OnMouseMove(nFlags, point);
}
