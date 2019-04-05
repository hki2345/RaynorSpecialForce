// Dlg_MainMenu.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEditor.h"
#include "afxdialogex.h"

#include "Dlg_MainMenu.h"
#include "Dlg_MeshConv.h"

#include "Dlg_Force.h"
#include "Dlg_Unit.h"
#include "Dlg_Terrain.h"
#include "Dlg_Editor.h"
#include "Dlg_UnitList.h"

#include "Edit_Class.h"


// Dlg_MainMenu ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Dlg_MainMenu, CDialogEx)

Dlg_MainMenu::Dlg_MainMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MENUDLG, pParent), m_CurIndex(0)
{
	Edit_Class::MDlg = this;
}

Dlg_MainMenu::~Dlg_MainMenu()
{
	for (size_t i = 0; i < m_VecDlg.size(); i++)
	{
		delete m_VecDlg[i];
	}
}

void Dlg_MainMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENUTAB, TabMenu);
}


BEGIN_MESSAGE_MAP(Dlg_MainMenu, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENUTAB, &Dlg_MainMenu::OnTcnSelchangeMenutab)
END_MESSAGE_MAP()


// Dlg_MainMenu �޽��� ó�����Դϴ�.

BOOL Dlg_MainMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255, 255, 255), TRUE);

	CreateDlg<Dlg_MeshConv>(IDD_MESHDLG, L"Mesh");
	CreateDlg<Dlg_Force>(IDD_FORCEDLG, L"Force");
	CreateDlg<Dlg_Unit>(IDD_UNITDLG, L"Unit");
	CreateDlg<Dlg_Terrain>(IDD_TERRAINDLG, L"Terrain");
	CreateDlg<Dlg_Editor>(IDD_TOOLDLG, L"Study");


	// ���� �ߴ� â - �ϳ��� �׳� �̷��� �ٹ�
	{
		Dlg_UnitList* m_pDlg = new Dlg_UnitList();

		m_pDlg->StateName = L"Unit List";
		m_pDlg->Create(IDD_UNITLISTDLG, this);
		m_pDlg->SetBackgroundColor(RGB(255, 255, 255), TRUE);
		m_pDlg->ShowWindow(SW_HIDE);
		m_pDlg->m_StateChange = false;
		m_VecDlg.push_back(m_pDlg);
	}


	Show_Dlg(0);
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



void Dlg_MainMenu::OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// GetcurSel -> ���� ���� ����Ŵ -> ��ȭ�Ѵٸ� �װ��� �� ���� ����Ŵ
	Show_Dlg(TabMenu.GetCurSel());

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}



void Dlg_MainMenu::Show_Dlg(size_t _Index)
{

	if (true == m_VecDlg[_Index]->m_StateChange)
	{
		Core_Class::MainSceneMgr().Change_State(m_VecDlg[_Index]->StateName.GetString());
	}
	else {
		Core_Class::MainSceneMgr().Change_State(nullptr);
	}

	m_VecDlg[m_CurIndex]->ShowWindow(SW_HIDE);
	m_VecDlg[_Index]->ShowWindow(SW_SHOW);
	m_VecDlg[_Index]->Init_Dlg();
	TabMenu.SetCurSel((int)_Index);

	m_CurIndex = _Index;
}


TabDlg* Dlg_MainMenu::Find_Dlg(const wchar_t* _Name)
{
	if (nullptr == _Name)
	{
		return nullptr;
	}

	for (size_t i = 0; i < m_VecDlg.size(); i++)
	{
		if (m_VecDlg[i]->StateName == _Name)
		{
			return m_VecDlg[i];
		}
	}

	return nullptr;
}

void Dlg_MainMenu::Update_Dlg()
{
	m_VecDlg[m_CurIndex]->Update_Dlg();
}