#include "stdafx.h"
#include "Splitter_Edit.h"


Splitter_Edit::Splitter_Edit() : m_bLock(false)
{
}


Splitter_Edit::~Splitter_Edit()
{
}

// �ش� Ŭ������ �� Ŭ���� �� �̷� �͵��� �� ���̴�.
BEGIN_MESSAGE_MAP(Splitter_Edit, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()

	// �������� �̰� �־��־� ������ } �߰�ȣ�� �ݰų� �Լ��� ������ ������
END_MESSAGE_MAP()

// �������̵� �Ͽ� ���ϴ� �Լ��� Ŀ���͸���¡ �ϴ� ���̴�.
void Splitter_Edit::OnLButtonDown(UINT nFlages, CPoint point) 
{
	if (true == m_bLock)
	{
		return;
	}

	CSplitterWnd::OnLButtonDown(nFlages, point);
}

void Splitter_Edit::OnMouseMove(UINT nFlages, CPoint point) 
{
	if (true == m_bLock)
	{
		return;
	}

	CSplitterWnd::OnMouseMove(nFlages, point);
}

BOOL Splitter_Edit::OnSetCursor(CWnd* _pWnd, UINT _nHitTest, UINT _Msg) 
{
	if (true == m_bLock)
	{
		return 0;
	}

	return CSplitterWnd::OnSetCursor(_pWnd, _nHitTest, _Msg);
}

BOOL Splitter_Edit::PreCreateWindow(CREATESTRUCT& cs) 
{
	// �θ��� �� -> ������ ���� �������̵� �ؼ�
	// â�� �ٲ�ų� �����ϴ� ���̴�.
	m_cxSplitter = 1;
	m_cySplitter = 1;

	//m_cxBorder = 5;
	//m_cyBorder = 5;

	// �� ���� 0 ���ϰ� �Ǹ� ���� -> ��� �ϴ� ���� 0 ���϶�� �Ҹ�
	m_cxSplitterGap = 1;
	m_cySplitterGap = 1;


	return CSplitterWnd::PreCreateWindow(cs);
}