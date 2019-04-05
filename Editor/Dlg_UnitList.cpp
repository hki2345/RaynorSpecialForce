// Dlg_UnitList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Dlg_UnitList.h"
#include "Dlg_Terrain.h"

#include "afxdialogex.h"


#include <Renderer_BonAni.h>
#include <Container.h>
#include <Force_Unit.h>


// Dlg_UnitList ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Dlg_UnitList, TabDlg)

Dlg_UnitList::Dlg_UnitList(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_UNITLISTDLG, pParent)
{

}

Dlg_UnitList::~Dlg_UnitList()
{
	int a = 0;
}


void Dlg_UnitList::Init_Dlg(TabDlg* _Other) 
{
	m_Ulist.ResetContent();

	if (0 == ResourceManager<Changer_Animation>::All_Count())
	{
		ResourceManager<Changer_Animation>::All_Load();
		ResourceManager<MeshContainer>::All_Load();
	}

	if (0 == SC2_ConManager<Force_Unit>::All_Count())
	{
		SC2_ConManager<Force_Unit>::All_Load();
	}

	std::vector<KPtr<Force_Unit>> TT = SC2_ConManager<Force_Unit>::All_SingleVec();

	for (size_t i = 0; i < TT.size(); i++)
	{
		TT[i]->one()->Active_Off();
		m_Ulist.AddString(TT[i]->name());
	}

	m_MofUList = (Dlg_Terrain*)_Other;
}

void Dlg_UnitList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UNITLIST, m_Ulist);

}


BEGIN_MESSAGE_MAP(Dlg_UnitList, TabDlg)
	ON_LBN_DBLCLK(IDC_UNITLIST, &Dlg_UnitList::OnLbnDblclkUnitlist)
END_MESSAGE_MAP()


// Dlg_UnitList �޽��� ó�����Դϴ�.


void Dlg_UnitList::OnLbnDblclkUnitlist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	std::vector<KPtr<Force_Unit>> TT = SC2_ConManager<Force_Unit>::All_SingleVec();

	int TSel = m_Ulist.GetCurSel();
	m_MofUList->Create_Grab(TT[TSel]->name());

}