
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "KEditor.h"

#include "MainFrm.h"

#include "View_Device.h"
#include "View_Edit.h"

#include <KCore.h>
#include <ResourceManager.h>
#include <KImage.h>
#include "Edit_Launcher.h"
#include "Edit_Class.h"
#include <InputManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ������ �̷��� ����
	//// �������� Ŭ���̾�Ʈ ������ �����ϴ� �並 ����ϴ�.
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
	//	CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("�� â�� ������ ���߽��ϴ�.\n");
	//	return -1;
	//}

	m_MainWnd.Lock();
	m_MainWnd.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE);

	SIZE ASize = { 1200, 600}; 
	SIZE TSize = { 800, 600 };
	SIZE MSize = { ASize.cx - TSize.cx, 1080 };


	// ���⼭ �ڵ� �Ѱܼ� ����̽��� �ʱ�ȭ�ϴ� ���
	m_MainWnd.CreateView(0, 0, RUNTIME_CLASS(View_Device), TSize, nullptr);
		
	CWnd* pWnd = m_MainWnd.GetPane(0, 0);
	KCore::Start<Edit_Launcher>(AfxGetInstanceHandle(), L"MainWindow", false, pWnd->m_hWnd);

	m_MainWnd.CreateView(0, 1, RUNTIME_CLASS(View_Edit), MSize, nullptr);

	RECT Rc = { 0, 0, ASize.cx, ASize.cy };

	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(nullptr, 0, 0, Rc.right, Rc.bottom - Rc.top + m_MainWnd.GetBoderSizeY() * 2, SWP_NOMOVE | SWP_NOZORDER);

	// Edit_Class::MDlg->DlgInit();
	// Edit_Class::SpDlg->SpriteInit();
	// Edit_Class::SceneDlg->Init();


	// Edit_Class::ADlg->ResetObjectTreeItem();


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.hMenu = nullptr;
	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// �� â���� ��Ŀ���� �̵��մϴ�.
	// m_wndView.SetFocus();
	m_MainWnd.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// �信�� ù° ũ���� �ش� ��ɿ� ��Ÿ������ �մϴ�.
	//if (m_MainWnd.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;
	// ShowCursor(TRUE);
	// �׷��� ������ �⺻ ó���մϴ�.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

