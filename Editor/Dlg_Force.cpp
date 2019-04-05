// Dlg_Force.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_Force.h"
#include "afxdialogex.h"


#include <Renderer_Grid.h>
#include <Light.h>
#include <SC2_Camera.h>

#include <Renderer_BonAni.h>
#include <ResourceManager.h>
#include <Renderer_Mesh.h>
#include <Effect_Bloom.h>

#include <Con_Class.h>
#include <SC2_Force.h>

// Dlg_Force 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_Force, TabDlg)

Dlg_Force::Dlg_Force(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_FORCEDLG, pParent)
{

}

Dlg_Force::~Dlg_Force()
{
}

void Dlg_Force::Init_Dlg()
{
	Init_KM3List();
}

BOOL Dlg_Force::OnInitDialog()
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
	SKYMESH1->ROpt.Defferd_orForward = 0;
	SKYMESH1->ROpt.LightOpacity = 1.0f;
	SKYMESH1->Set_Mesh(L"SPHERE");
	SKYMESH1->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"Space.jpg");

	KPtr<TheOne> GRIDACTOR = TabScene->Create_One();
	GRIDACTOR->Trans()->rotate_world(KVector4(90.0f, 0.0f, 0.0f));
	GRIDACTOR->Trans()->scale_world(KVector4(10000.0f, 10000.0f, 10000.0f));
	KPtr<Renderer_Grid> GRIDRENDER = GRIDACTOR->Add_Component<Renderer_Grid>();



	TabScene->camera()->Add_Component<SC2_Camera>();
	TabScene->camera()->Far(10000.0f);
	TabScene->camera()->one()->Trans()->pos_local(KVector4(0.0f, 10.0f, -20.0f));
	// TabScene->camera()->Create_EffectPost<Effect_Bloom>(0);



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




	Init_ForceList();

	return TRUE;
}


void Dlg_Force::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FORKM3LIST, m_KM3List);
	DDX_Control(pDX, IDC_FORCELIST, m_ForceList);
	DDX_Control(pDX, IDC_FORCENAME, m_ForceName);
	DDX_Control(pDX, IDC_FORCECOLOR, m_ForceColor);
}


BEGIN_MESSAGE_MAP(Dlg_Force, CDialogEx)
	ON_BN_CLICKED(IDC_FORCECOLOR, &Dlg_Force::OnBnClickedForcecolor)
	ON_LBN_SELCHANGE(IDC_FORCELIST, &Dlg_Force::OnLbnSelchangeForcelist)
	ON_EN_CHANGE(IDC_FORCENAME, &Dlg_Force::OnEnChangeForcename)
	ON_LBN_SELCHANGE(IDC_FORKM3LIST, &Dlg_Force::OnLbnSelchangeForkm3list)
	ON_BN_CLICKED(IDC_KM3RESETBTN, &Dlg_Force::OnBnClickedKm3resetbtn)
END_MESSAGE_MAP()





void Dlg_Force::Init_KM3List()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 만약 자체 메쉬에 애니메이션을 바로 적용시키려면
	// 애니메이션이 먼저 불러온 후 배급받는 식이다.
	if (0 == ResourceManager<Changer_Animation>::All_Count())
	{
		ResourceManager<Changer_Animation>::All_Load();
		ResourceManager<MeshContainer>::All_Load();
	}

	// 리스트 초기화
	m_KM3List.ResetContent();


	m_SO = m_OList.begin();
	m_EO = m_OList.end();

	for (; m_SO != m_EO; ++m_SO)
	{
		(*m_SO)->Set_Death();
	}

	m_OList.clear();



	UpdateData(TRUE);


	std::vector<KPtr<MeshContainer>> TVec = ResourceManager<MeshContainer>::All_SingleVec();

	for (size_t i = 0; i < TVec.size(); i++)
	{
		std::wstring Temp = TVec[i]->FileExt();
		if (L".KM3" == Temp)
		{
			m_CurOne = Core_Class::MainScene()->Create_One(TVec[i]->FileNameExt());
			m_CurOne->Trans()->pos_local(KVector(.0f));
			m_CurOne->Trans()->scale_local(KVector(1.f, 1.f, 1.f));

			KPtr<Renderer_BonAni> TRender = m_CurOne->Add_Component<Renderer_BonAni>();
			TRender->Set_Fbx(TVec[i]->FileNameExt());

			if (nullptr == TRender->changer_animation())
			{
				TRender->Create_Animation();
			}

			m_OList.push_back(m_CurOne);
			m_CurOne->Active_Off();
			m_KM3List.AddString(TVec[i]->FileNameExt());
		}
	}

	m_CurOne = nullptr;

	UpdateData(FALSE);
}


void Dlg_Force::Init_ForceList()
{
	if (nullptr == Con_Class::s2_manager())
	{
		BBY;
	}

	std::map<std::wstring, KPtr<SC2_Force>>* Map = Con_Class::s2_manager()->force_map();
	std::map<std::wstring, KPtr<SC2_Force>>::iterator m_SFI = Map->begin();
	std::map<std::wstring, KPtr<SC2_Force>>::iterator m_EFI = Map->end();



	UpdateData(TRUE);	

	for (; m_SFI != m_EFI; ++m_SFI)
	{
		m_ForceList.AddString(m_SFI->first.c_str());
	}

	UpdateData(FALSE);
}

void Dlg_Force::Update_Force()
{
	if (nullptr == m_CurForce)
	{
		return;
	}

	UpdateData(TRUE);

	m_ForceName.SetWindowTextW(m_CurForce->force_name());


	KColor TC = m_CurForce->force_color();
	TC.a = 0;
	COLORREF T = TC.color_to_reverse255();
	m_ForceColor.SetColor(T);


	if (nullptr != m_CurOne)
	{
		KPtr<Renderer_BonAni> TRender = m_CurOne->Get_Component<Renderer_BonAni>();
		TRender->force_color(m_CurForce->force_color());
	}

	
	UpdateData(FALSE);
}



// Dlg_Force 메시지 처리기입니다.
void Dlg_Force::OnLbnSelchangeForcelist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Tint = m_ForceList.GetCurSel();
	if (-1 == Tint)
	{
		return;
	}

	CString TempStr;
	m_ForceList.GetText(Tint, TempStr);

	m_CurForce = Con_Class::s2_manager()->Find_Force(TempStr.GetBuffer());
	Update_Force();
}

void Dlg_Force::OnEnChangeForcename()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// TabDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurForce)
	{
		return;
	}
}

void Dlg_Force::OnBnClickedForcecolor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurForce)
	{
		return;
	}


	DWORD T = m_ForceColor.GetColor();

	m_CurForce->force_color(T);
	KColor* EXID = &m_CurForce->force_color();
	EXID->a = 1.0f;

	Update_Force();
}



void Dlg_Force::OnLbnSelchangeForkm3list()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Tint = m_KM3List.GetCurSel();
	if (-1 == Tint)
	{
		return;
	}

	if (nullptr != m_CurOne)
	{
		m_CurOne->Set_Death();
	}

	CString TempStr;
	m_KM3List.GetText(Tint, TempStr);

	m_CurOne = Core_Class::MainScene()->Create_One(L"FBX_LOAD");
	m_CurOne->Trans()->pos_local(KVector(.0f));
	m_CurOne->Trans()->scale_local(KVector(1.f, 1.f, 1.f));
	KPtr<Renderer_BonAni> TRender = m_CurOne->Add_Component<Renderer_BonAni>();

	TRender->Set_Fbx(TempStr);
	TRender->Create_Clip(L"ALLAni", 0, 100000);
	TRender->Set_Clip(L"ALLAni");
}


void Dlg_Force::OnBnClickedKm3resetbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Init_KM3List();
}
