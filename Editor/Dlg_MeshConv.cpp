// Dlg_MeshConv.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_MeshConv.h"
#include "afxdialogex.h"

#include <Renderer_Grid.h>
#include <Light.h>
#include <SC2_Camera.h>

#include "Dlg_FbxLoad.h"
#include "Dlg_KM3Loader.h"
#include <Renderer_Mesh.h>

// Dlg_MeshConv 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_MeshConv, TabDlg)

Dlg_MeshConv::Dlg_MeshConv(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_MESHDLG, pParent)
{

}

Dlg_MeshConv::~Dlg_MeshConv()
{
	for (size_t i = 0; i < m_DlgVec.size(); i++)
	{
		delete m_DlgVec[i];
	}
}

void Dlg_MeshConv::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FBX2KM3TAB, m_MeshTab);
}


BEGIN_MESSAGE_MAP(Dlg_MeshConv, TabDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FBX2KM3TAB, &Dlg_MeshConv::OnTcnSelchangeFbx2km3tab)
END_MESSAGE_MAP()


// Dlg_MeshConv 메시지 처리기입니다.


BOOL Dlg_MeshConv::OnInitDialog()
{
	TabDlg::OnInitDialog();

	KPtr<State> TabScene = Core_Class::MainSceneMgr().Find_State(StateName.GetString());

	if (nullptr == TabScene)
	{
		KASSERT(true);
	}


	// 투명한 걸 그린다면 만약에 -> 스카이랑 섞이기 위해선 당연히 ... 스카이가 디퍼드로 그려져야 한다.
	KPtr<TheOne> SKYMESH = TabScene->Create_One();
	SKYMESH->Trans()->scale_local(KVector4(10000.0f, 10000.0f, 10000.0f));
	SKYMESH->Trans()->pos_local(KVector4(.0f, 0.0f, 0.0f));
	KPtr<Renderer_Mesh> SKYMESH1 = SKYMESH->Add_Component<Renderer_Mesh>();
	SKYMESH1->Set_Material(L"SKY3DMATFORWARD");
	SKYMESH1->Set_RSState(L"SNONE");
	SKYMESH1->ROpt.LightOpacity = 1.0f;
	SKYMESH1->Set_Mesh(L"SPHERE");
	SKYMESH1->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"Space.jpg");
	SKYMESH1->ROpt.Defferd_orForward = 0;

	KPtr<TheOne> GRIDACTOR = TabScene->Create_One();
	GRIDACTOR->Trans()->rotate_world(KVector4(90.0f, 0.0f, 0.0f));
	GRIDACTOR->Trans()->scale_world(KVector4(10000.0f, 10000.0f, 10000.0f));
	KPtr<Renderer_Grid> GRIDRENDER = GRIDACTOR->Add_Component<Renderer_Grid>();


	TabScene->camera()->Add_Component<SC2_Camera>();
	TabScene->camera()->Far(10000.0f);
	TabScene->camera()->one()->Trans()->pos_local(KVector4(0.0f, 10.0f, -20.0f));



	KPtr<TheOne> Light2 = TabScene->Create_One();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<Light> pLight2 = Light2->Add_Component<Light>();
	pLight2->Trans()->rotate_world(KVector4(45.0F, 0.0F, 0.0f));
	pLight2->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight2->PushLightLayer(0);


	KPtr<TheOne> Light3 = TabScene->Create_One();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<Light> pLight3 = Light3->Add_Component<Light>();
	pLight3->Trans()->rotate_world(KVector4(-45.0F, 0.0F, 0.0f));
	pLight3->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight3->PushLightLayer(1);


	KPtr<TheOne> Light4 = TabScene->Create_One();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<Light> pLight4 = Light4->Add_Component<Light>();
	pLight4->Trans()->rotate_world(KVector4(.0F, 45.0F, 0.0f));
	pLight4->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight4->PushLightLayer(0);



	UpdateData(TRUE);

	m_MeshTab.InsertItem(0, L"FBX");
	m_MeshTab.InsertItem(1, L"KM3");

	RECT Rc;
	m_MeshTab.GetClientRect(&Rc);

	Dlg_FbxLoad* LDlg = new Dlg_FbxLoad();
	LDlg->StateName = L"FBX Loader";
	LDlg->Create(IDD_FBXDLG, this);
	LDlg->SetBackgroundColor(RGB(255, 255, 255), TRUE);
	LDlg->SetWindowPos(nullptr, 0, 46, Rc.right - 0, Rc.bottom - 46, SWP_NOZORDER);
	LDlg->ShowWindow(SW_HIDE);
	LDlg->m_StateChange = false;
	m_DlgVec.push_back(LDlg);


	Dlg_KM3Loader* KDlg = new Dlg_KM3Loader();
	KDlg->StateName = L"FBX Loader";
	KDlg->Create(IDD_KM3DLG, this);
	KDlg->SetBackgroundColor(RGB(255, 255, 255), TRUE);
	KDlg->SetWindowPos(nullptr, 0, 46, Rc.right - 0, Rc.bottom - 46, SWP_NOZORDER);
	KDlg->ShowWindow(SW_HIDE);
	KDlg->m_StateChange = false;
	m_DlgVec.push_back(KDlg);

	Show_Dlg(0);

	UpdateData(FALSE);


	return TRUE;
}

void Dlg_MeshConv::Show_Dlg(const int& _Index)
{
	m_DlgVec[m_CurIdx]->ShowWindow(SW_HIDE);
	m_DlgVec[m_CurIdx]->Hide_Dlg();

	m_DlgVec[_Index]->ShowWindow(SW_SHOW);
	m_DlgVec[_Index]->Init_Dlg();
	m_MeshTab.SetCurSel((int)_Index);

	m_CurIdx = _Index;
}

void Dlg_MeshConv::OnTcnSelchangeFbx2km3tab(NMHDR *pNMHDR, LRESULT *pResult)
{
	Show_Dlg(m_MeshTab.GetCurSel());

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
