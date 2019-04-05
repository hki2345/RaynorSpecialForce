// Dlg_Editor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "Dlg_Editor.h"
#include "afxdialogex.h"
#include <Core_Class.h>
#include <State.h>
#include <TheOne.h>
#include <Renderer_Rect.h>
#include <Renderer_Grid.h>
#include <Renderer_Mesh.h>
#include <Renderer_Terrain.h>
#include <Renderer_Effect.h>
#include <Renderer_BonAni.h>
#include <Renderer_Draw.h>

#include <Effect_Gaussian.h>
#include <Effect_Bloom.h>
#include <Texture_Multi.h>

#include <DebugManager.h>
#include <TimeManager.h>
#include <KThread.h>

#include <ResourceManager.h>
#include <KImage.h>
#include <Light.h>

#include "Edit_Class.h"

#include <KSphere_Col.h>
#include <KPlane3D_Col.h>
#include <KRay3D.h>

#include <SC2_Camera.h>
#include <Renderer_AniEffect.h>
#include <Renderer_UI.h>


// Dlg_Editor 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_Editor, TabDlg)

Dlg_Editor::Dlg_Editor(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_TOOLDLG, pParent)
{

}

Dlg_Editor::~Dlg_Editor()
{
}

void Dlg_Editor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg_Editor, TabDlg)
END_MESSAGE_MAP()


// Dlg_Editor 메시지 처리기입니다.




BOOL Dlg_Editor::OnInitDialog()
{
	TabDlg::OnInitDialog();

	KPtr<State> TabScene = Core_Class::MainSceneMgr().Find_State(StateName.GetString());

	if (nullptr == TabScene) 
	{
		KASSERT(true);
	}



	//EXP1->Set_Mesh(L"RECT");
	// EXP1->Set_Material()


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
	SKYMESH1->ROpt.Defferd_orForward = 0;


	KPtr<TheOne> GRIDACTOR = TabScene->Create_One();
	GRIDACTOR->Trans()->rotate_world(KVector4(90.0f, 0.0f, 0.0f));
	GRIDACTOR->Trans()->scale_world(KVector4(10000.0f, 10000.0f, 10000.0f));
	KPtr<Renderer_Grid> GRIDRENDER = GRIDACTOR->Add_Component<Renderer_Grid>();





	TabScene->camera()->Create_EffectPost<Effect_Bloom>(0);
	// TabScene->camera()->Create_EffectPost<Effect_Gaussian>(1);
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



	CCreateContext         m_context;

	m_context.m_pNewViewClass = RUNTIME_CLASS(View_Component);
	View_Component* pView = (View_Component*)((CFrameWnd*)this)->CreateView(&m_context);

	RECT ViewSize = { 10, 10, 700, 530 };
	pView->ShowWindow(SW_SHOW);
	pView->MoveWindow(&ViewSize);

	Edit_Class::m_gVIewCom->Release_AllDlg();
	Edit_Class::m_gVIewCom->Set_One(Light2);

	// TabScene->camera()->Change_Mode();


	
	
	KPtr<TheOne> CUBEMIDDLE2 = TabScene->Create_One();
	CUBEMIDDLE2->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	CUBEMIDDLE2->Trans()->pos_local(KVector4(5.0f, 50.0f, 5.0f));
	KPtr<Renderer_Mesh> PTRMESH4 = CUBEMIDDLE2->Add_Component<Renderer_Mesh>();
	PTRMESH4->Set_Material(L"FORWARD3DMAT");
	PTRMESH4->Set_Mesh(L"CUBE");
	
	PTRMESH4->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"TILE_01.png");


	KPtr<TheOne> SPHERELEFT = TabScene->Create_One();
	SPHERELEFT->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	SPHERELEFT->Trans()->pos_local(KVector4(-15.0f, 0.0f, 0.0f));
	KPtr<Renderer_Mesh> PTRMESH1 = SPHERELEFT->Add_Component<Renderer_Mesh>();
	PTRMESH1->ROpt.IsLight = 1;
	PTRMESH1->Set_Material(L"DEFFERD3DMAT");
	PTRMESH1->Set_Mesh(L"SPHERE");

	PTRMESH1->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"MoonDiff.jpg");
	PTRMESH1->material()->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, L"MoonBump.jpg");


	
	KPtr<TheOne> SPHERERIGHT = TabScene->Create_One();
	SPHERERIGHT->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	SPHERERIGHT->Trans()->pos_local(KVector4(15.0f, 0.0f, 0.0f));
	KPtr<Renderer_Mesh> PTRMESH2 = SPHERERIGHT->Add_Component<Renderer_Mesh>();
	PTRMESH2->Set_Material(L"DEFFERD3DMAT");
	PTRMESH2->Set_Mesh(L"SPHERE");

	

	KPtr<TheOne> TERRAIN = TabScene->Create_One();
	TERRAIN->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	TERRAIN->Trans()->pos_local(KVector4(.0f, 0.0f, 0.0f));
	KPtr<Renderer_Terrain> TerMESH1 = TERRAIN->Add_Component<Renderer_Terrain>();

	// 순서를 지켜야 된다????? ㅇㅇ
	TerMESH1->Create_Terrain(100, 100, L"Cover.jpg", 1.0f);
	TerMESH1->base_texture(L"FB");
	TerMESH1->Insert_CoverTex(L"FC", L"Cover.jpg");
	TerMESH1->Set_RSState(L"SFRONT");
	TerMESH1->brush_size(10.0f);
	TerMESH1->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 7, L"cursor-target-allied.dds");





	KPtr<TheOne> OPARTI = TabScene->Create_One();
	KPtr<Renderer_Effect> RPARTI = OPARTI->Add_Component<Renderer_Effect>(2000);
	RPARTI->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"TILE_01.png");
	RPARTI->ROpt.Defferd_orForward = 0;


	// KPtr<TheOne> DrawOne = TabScene->Create_One();
	// DrawOne->Trans()->scale_local(KVector4(100.0f, 100.0f, .1f));
	// DrawOne->Trans()->pos_local(KVector4(10.0f, 10.0f, 0.0f));
	// DrawOne->Trans()->rotate_world(KVector4(90.0F, .0F, 0.0f));
	// 
	// KPtr<Renderer_Draw> DREN = DrawOne->Add_Component<Renderer_Draw>();
	// DREN->Set_Mesh(L"RECT");
	// DREN->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"TILE_01.png");
	// DREN->brush_size(10.0f);





	TabScene->This_Col3DManager.Link(101, 100);
	TabScene->This_Col3DManager.Link(100, 101);

	KPtr<KRay3D> RayCol = TabScene->camera()->Add_Component<KRay3D>(101);
	RayCol->EnterFunc(L"Edit Study", this, &Dlg_Editor::Collision_Test);

	// KPtr<KPlane3D_Col> Tescol = TERRAIN->Add_Component<KPlane3D_Col>(100);
	// KPtr<KPlane3D_Col> RendCol = DrawOne->Add_Component<KPlane3D_Col>(100);
	KPtr<KSphere_Col> RightCol = SPHERERIGHT->Add_Component<KSphere_Col>(100);

	KPtr<Renderer_AniEffect> EXP1 = TabScene->Create_One(L"TT")->Add_Component<Renderer_AniEffect>();
	EXP1->one()->Trans()->pos_local(KVector4(50.0f, 5.0f, 50.0f));
	EXP1->one()->Trans()->scale_local(KVector4::One * 100.0f);
	EXP1->EffectSetting(L"Explosion21.png", 4, 4, true, 0.02f);

	KPtr<Renderer_UI> m_uCMedic = TabScene->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uCMedic->one()->Trans()->scale_local(KVector(50.0f, 50.0f, 10.0f, .0f));
	m_uCMedic->one()->Trans()->pos_local(KVector(-225.0f, -250.0f, 1.1f, .0f));
	m_uCMedic->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"TemCover.png");
	m_uCMedic->cut_fade(1.0f);
	m_uCMedic->cut_value(1.f);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Dlg_Editor::Collision_Test(KCollision*, KCollision*)
{
	int a = 9;
}

void Dlg_Editor::Init_Dlg()
{

	ResourceManager<MeshContainer>::Clear();
	ResourceManager<Changer_Animation>::Clear();
	
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 만약 자체 메쉬에 애니메이션을 바로 적용시키려면
	// 애니메이션이 먼저 불러온 후 배급받는 식이다.
	ResourceManager<Changer_Animation>::All_Load();
	ResourceManager<MeshContainer>::All_Load();
	
	KPtr<State> TabScene = Core_Class::MainSceneMgr().Find_State(StateName.GetString());
	
	KPtr<TheOne> m_CurOne = TabScene->Create_One(L"");
	m_CurOne->Trans()->pos_local(KVector(5.0f));
	m_CurOne->Trans()->scale_local(KVector(1.f, 1.f, 1.f));
	
	KPtr<Renderer_BonAni> TRender = m_CurOne->Add_Component<Renderer_BonAni>();
	TRender->Set_Fbx(L"BattleCruiser.KM3");
	
	if (nullptr == TRender->changer_animation())
	{
		TRender->Create_Animation();
	}
	
	
	KPtr<TheOne> m_CurOne2 = TabScene->Create_One(L"");
	m_CurOne2->Trans()->pos_local(KVector(.0f));
	m_CurOne2->Trans()->scale_local(KVector(1.f, 1.f, 1.f));
	
	KPtr<Renderer_BonAni> TRender2 = m_CurOne2->Add_Component<Renderer_BonAni>();
	TRender2->Set_Fbx(L"BattleCruiser.KM3");
	
	
	if (nullptr == TRender2->changer_animation())
	{
		TRender2->Create_Animation();
	}

}