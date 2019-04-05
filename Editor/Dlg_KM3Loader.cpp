// Dlg_KM3Loader.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_KM3Loader.h"
#include "afxdialogex.h"


#include <KThread.h>
#include <Core_Class.h>
#include <Renderer_BonAni.h>
#include <ResourceManager.h>

// Dlg_KM3Loader 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_KM3Loader, TabDlg)

Dlg_KM3Loader::Dlg_KM3Loader(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_KM3DLG, pParent), m_PrevSel(-1), m_AniInx(0)
{
}

Dlg_KM3Loader::~Dlg_KM3Loader()
{
}

void Dlg_KM3Loader::DoDataExchange(CDataExchange* pDX)
{
	TabDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KM3LIST, m_KM3List);
	DDX_Control(pDX, IDC_ACFILENAME, m_CAName);
	DDX_Control(pDX, IDC_KM3ACCOMBO, m_ClipCombo);
	DDX_Control(pDX, IDC_KM3ACADDBTN, m_ClipAdd);
	DDX_Control(pDX, IDC_KM3ACDELBTN, m_ClipDelete);

	for (int i = 0; i < 3; i++)
	{
		DDX_Control(pDX, IDC_KM3ACSTART + i, m_ClipInfo[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		DDX_Control(pDX, IDC_DIFFBTNK + i, m_TexBtn[i]);
		DDX_Control(pDX, IDC_DIFFPATHK + i, m_TexPath[i]);
	}
}


BEGIN_MESSAGE_MAP(Dlg_KM3Loader, TabDlg)
	ON_BN_CLICKED(IDC_KM3LOAD, &Dlg_KM3Loader::OnBnClickedKm3load)
	ON_LBN_SELCHANGE(IDC_KM3LIST, &Dlg_KM3Loader::OnLbnSelchangeKm3list)
	ON_BN_CLICKED(IDC_DIFFBTNK, &Dlg_KM3Loader::OnBnClickedDiffbtnk)
	ON_BN_CLICKED(IDC_NORMBTNK, &Dlg_KM3Loader::OnBnClickedNormbtnk)
	ON_BN_CLICKED(IDC_SPECBTNK, &Dlg_KM3Loader::OnBnClickedSpecbtnk)
	ON_BN_CLICKED(IDC_EMISBTNK, &Dlg_KM3Loader::OnBnClickedEmisbtnk)
	ON_EN_CHANGE(IDC_KM3ACSTART, &Dlg_KM3Loader::OnEnChangeKm3acstart)
	ON_EN_CHANGE(IDC_KM3ACEND, &Dlg_KM3Loader::OnEnChangeKm3acend)
	ON_EN_CHANGE(IDC_KM3ACNAME, &Dlg_KM3Loader::OnEnChangeKm3acname)
	ON_BN_CLICKED(IDC_KM3ACADDBTN, &Dlg_KM3Loader::OnBnClickedKm3acaddbtn)
	ON_BN_CLICKED(IDC_KM3ACDELBTN, &Dlg_KM3Loader::OnBnClickedKm3acdelbtn)
	ON_CBN_SELCHANGE(IDC_KM3ACCOMBO, &Dlg_KM3Loader::OnCbnSelchangeKm3accombo)
	ON_BN_CLICKED(IDC_KM3ACSSAVE, &Dlg_KM3Loader::OnBnClickedKm3acssave)
	ON_BN_CLICKED(IDC_KM3ACASAVE, &Dlg_KM3Loader::OnBnClickedKm3acasave)
	ON_BN_CLICKED(IDC_KM3RESETBTN, &Dlg_KM3Loader::OnBnClickedKm3resetbtn)
	ON_BN_CLICKED(IDC_KM3PLAYBTN, &Dlg_KM3Loader::OnBnClickedKm3playbtn)
	ON_BN_CLICKED(IDC_KM3PAUBTN, &Dlg_KM3Loader::OnBnClickedKm3paubtn)
	ON_BN_CLICKED(IDC_KM3PREVBTN, &Dlg_KM3Loader::OnBnClickedKm3prevbtn)
	ON_BN_CLICKED(IDC_KM3NEXTBTN, &Dlg_KM3Loader::OnBnClickedKm3nextbtn)
END_MESSAGE_MAP()


void Dlg_KM3Loader::Init_Dlg()
{
	if (0 == ResourceManager<Changer_Animation>::All_Count())
	{
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// 만약 자체 메쉬에 애니메이션을 바로 적용시키려면
		// 애니메이션이 먼저 불러온 후 배급받는 식이다.
		ResourceManager<Changer_Animation>::All_Load();
		ResourceManager<MeshContainer>::All_Load();
	}

	Update_RscTree();
}

void Dlg_KM3Loader::Hide_Dlg()
{
	if (nullptr != m_CurOne)
	{
		m_CurOne->Set_Death();
	}
}

// Dlg_KM3Loader 메시지 처리기입니다.


void Dlg_KM3Loader::OnBnClickedKm3load()
{
	ResourceManager<MeshContainer>::Clear();
	ResourceManager<Changer_Animation>::Clear();

	ResourceManager<Changer_Animation>::All_Load();
	ResourceManager<MeshContainer>::All_Load();
	// 스래드 실행시 mfc 최신화가 안됀다.
	Update_RscTree();
}

unsigned int Dlg_KM3Loader::Load_KM3T(void* _args)
{
	ResourceManager<MeshContainer>::Clear();
	ResourceManager<Changer_Animation>::Clear();

	ResourceManager<Changer_Animation>::All_Load();
	ResourceManager<MeshContainer>::All_Load();
	// 스래드 실행시 mfc 최신화가 안됀다.
	Update_RscTree();

	return 0;
}

//



void Dlg_KM3Loader::Update_RscTree()
{
	UpdateData(TRUE);

	// 리스트 초기화
	m_KM3List.ResetContent();

	m_SO = m_OList.begin();
	m_EO = m_OList.end();

	for (; m_SO != m_EO; ++m_SO)
	{
		(*m_SO)->Set_Death();
	}

	m_OList.clear();



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


void Dlg_KM3Loader::OnLbnSelchangeKm3list()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Tint = m_KM3List.GetCurSel();

	// 같은 거 눌러도 안튕기게 한다
	if (Tint == m_PrevSel)
	{
		return;
	}
	else
	{
		m_PrevSel = Tint;
	}

	if (-1 == Tint)
	{
		return;
	}

	if (nullptr != m_CurOne)
	{
		m_CurOne->Active_Off();
	}


	m_SO = m_OList.begin();
	m_EO = m_OList.end();

	for (; m_SO != m_EO; ++m_SO)
	{
		CString tmpCstr;
		std::wstring TmpStr;
		m_KM3List.GetText(Tint, tmpCstr);

		TmpStr = tmpCstr.GetString();


		if (TmpStr == (*m_SO)->ws_name())
		{
			m_CurOne = (*m_SO);
			m_CurOne->Active_On();
		}
	}		
	
	if (nullptr == m_CurOne)
	{
		return;
	}
	
	Init_Path();
	Init_AniName();
	Init_AniClip();
}



/********************* Animation Changer ******************/
#pragma region Animation Changer
void Dlg_KM3Loader::Init_AniName()
{
	KPtr<Changer_Animation> TmpCA = m_CurOne->Get_Component<Renderer_BonAni>()->changer_animation();
	if (nullptr == TmpCA)
	{
		return;
	}

	UpdateData(TRUE);

	m_CAName.SetWindowTextW(TmpCA->name());

	UpdateData(FALSE);
}

void Dlg_KM3Loader::Init_AniClip()
{
	KPtr<Changer_Animation> TmpCA = m_CurOne->Get_Component<Renderer_BonAni>()->changer_animation();
	if (nullptr == TmpCA)
	{
		return;
	}
		
	// 과연 바로 만들자마자는 커클립이 0번째에 세팅되야한당
	Update_Combo(TmpCA);
	Update_Clip(TmpCA->cur_clip());
}


void Dlg_KM3Loader::Update_Combo(KPtr<Changer_Animation> _Value)
{
	UpdateData(TRUE);
	m_ClipCombo.ResetContent();

	std::list<KPtr<Changer_Animation::Ani_Clip>>* Tlist = _Value->clip_list();

	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator S = Tlist->begin();
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator E = Tlist->end();

	for (; S != E; ++S)
	{
		m_ClipCombo.AddString((*S)->Name);
	}


	m_ClipCombo.SetCurSel(_Value->cur_index());
	UpdateData(FALSE);
}

void Dlg_KM3Loader::Update_Clip(KPtr<Changer_Animation::Ani_Clip> _Value)
{
	if (nullptr == _Value)
	{
		return;
	}

	UpdateData(TRUE);
	wchar_t Tmp[10];

	_itow_s(_Value->Start, Tmp, 10);
	m_ClipInfo[0].SetWindowTextW(Tmp);

	_itow_s(_Value->End, Tmp, 10);
	m_ClipInfo[1].SetWindowTextW(Tmp);
	
	m_ClipInfo[2].SetWindowTextW(_Value->Name);
	UpdateData(FALSE);
}


// clip
void Dlg_KM3Loader::OnEnChangeKm3acstart()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// TabDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurOne)
	{
		return;
	}

	UpdateData(TRUE);
	CString TmpStr;
	m_ClipInfo[0].GetWindowTextW(TmpStr);
	UpdateData(FALSE);

	KPtr<Changer_Animation> TmpCA = m_CurOne->Get_Component<Renderer_BonAni>()->changer_animation();
	TmpCA->cur_clip()->Start = PathManager::Convert_Str2Int(TmpStr.GetString());
}


void Dlg_KM3Loader::OnEnChangeKm3acend()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// TabDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurOne)
	{
		return;
	}

	UpdateData(TRUE);
	CString TmpStr;
	m_ClipInfo[1].GetWindowTextW(TmpStr);
	UpdateData(FALSE);

	KPtr<Changer_Animation> TmpCA = m_CurOne->Get_Component<Renderer_BonAni>()->changer_animation();
	TmpCA->cur_clip()->End = PathManager::Convert_Str2Int(TmpStr.GetBuffer());
}


void Dlg_KM3Loader::OnEnChangeKm3acname()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// TabDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurOne)
	{
		return;
	}
		
	UpdateData(TRUE);
	CString TmpStr;
	m_ClipInfo[2].GetWindowTextW(TmpStr);
	UpdateData(FALSE);

	KPtr<Changer_Animation> TmpCA = m_CurOne->Get_Component<Renderer_BonAni>()->changer_animation();
	TmpCA->Set_Name(TmpCA->cur_clip(), TmpStr.GetBuffer());

	Update_Combo(TmpCA);
}


// Ani Add Delete
void Dlg_KM3Loader::OnBnClickedKm3acaddbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurOne)
	{
		return;
	}


	KPtr<Renderer_BonAni> TmpRB = m_CurOne->Get_Component<Renderer_BonAni>();
	std::wstring TmpStr = L"NewClip";
	wchar_t TmpCnt[4];
	_itow_s(TmpRB->changer_animation()->contain_name(TmpStr.c_str()), TmpCnt, 10);


	TmpStr = TmpStr + TmpCnt;

	TmpRB->Create_Clip(TmpStr.c_str(), 0, 100000);
	TmpRB->Set_Clip(TmpStr.c_str());

	Update_Combo(TmpRB->changer_animation());
}


void Dlg_KM3Loader::OnBnClickedKm3acdelbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurOne)
	{
		return;
	}


	UpdateData(TRUE);
	CString TmpStr;
	m_ClipCombo.GetWindowTextW(TmpStr);
	UpdateData(FALSE);

	KPtr<Renderer_BonAni> TmpRB = m_CurOne->Get_Component<Renderer_BonAni>();

	TmpRB->Erase_Clip(TmpStr.GetBuffer());

	Update_Combo(TmpRB->changer_animation());
}





void Dlg_KM3Loader::OnCbnSelchangeKm3accombo()
{
	int cnt = m_ClipCombo.GetCurSel();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KPtr<Renderer_BonAni> TmpRB = m_CurOne->Get_Component<Renderer_BonAni>();

	std::list<KPtr<Changer_Animation::Ani_Clip>>* Tlist = TmpRB->changer_animation()->clip_list();

	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator S = Tlist->begin();
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator E = Tlist->end();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::wstring Tmp;
	for (size_t i = 0; S != E; ++S, ++i)
	{
		if (i == cnt)
		{
			TmpRB->changer_animation()->Set_AniClip((*S));
		}
	}
	Update_Clip(TmpRB->changer_animation()->cur_clip());
}

#pragma endregion


/********************* Texture ******************/
#pragma region Texture
void Dlg_KM3Loader::Init_Path()
{
	if (nullptr == m_CurOne)
	{
		return;
	}

	std::vector<KPtr<Texture>> TmpVec = m_CurOne->Get_Component<Renderer_BonAni>()->material()->texture_vec();


	UpdateData(TRUE);
	for (size_t i = 0; i < 4; i++)
	{
		if (i < TmpVec.size())
		{
			m_TexPath[i].SetWindowTextW((PathManager::Split_Path(TmpVec[i]->AllPath(), PathManager::Find_ForderPath(L"Mesh")).c_str()));
		}
		else
		{
			m_TexPath[i].SetWindowTextW(L"Unknown");
		}
	}
	UpdateData(FALSE);
}


void Dlg_KM3Loader::Update_Tex(const TEX_TYPE& _Value, const int& _Inx)
{
	if (nullptr == m_CurOne)
	{
		return;
	}

	UpdateData(TRUE);
	std::wstring Temp = PathManager::Split_Path(Show_DDSWindow(), PathManager::Find_ForderPath(L"Mesh"));
	m_TexPath[_Inx].SetWindowTextW(Temp.c_str());
	UpdateData(FALSE);

	KPtr<Texture> TTex = ResourceManager<Texture>::Load(Temp.c_str());

	KPtr<Renderer_BonAni> TAni = m_CurOne->Get_Component<Renderer_BonAni>();
	TAni->material()->Insert_TexData(_Value, _Inx, TTex->FileNameExt());
	TAni->Set_TexturePath(_Value, Temp.c_str());
}


std::wstring Dlg_KM3Loader::Show_DDSWindow()
{
	static TCHAR BASED_CODE szFilter[] = _T("DDS 파일(*.DDS) | *.DDS;*.dds; |모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.DDS"), _T("*.dds"), OFN_HIDEREADONLY, szFilter, this);


	if (IDOK == dlg.DoModal())
	{
		std::wstring Tmp = dlg.GetPathName();
		return Tmp;
	}

	return L"";
}

void Dlg_KM3Loader::OnBnClickedDiffbtnk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_Tex(TEX_TYPE::TEX_COLOR, 0);
}


void Dlg_KM3Loader::OnBnClickedNormbtnk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_Tex(TEX_TYPE::TEX_BUMP, 1);
}


void Dlg_KM3Loader::OnBnClickedSpecbtnk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_Tex(TEX_TYPE::TEX_SPEC, 2);
}


void Dlg_KM3Loader::OnBnClickedEmisbtnk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_Tex(TEX_TYPE::TEX_EMIS, 3);
}

#pragma endregion



// 싱글 세이브
void Dlg_KM3Loader::OnBnClickedKm3acssave()
{
	if (nullptr == m_CurOne)
	{
		return;
	}


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KPtr<Renderer_BonAni> TAni = m_CurOne->Get_Component<Renderer_BonAni>();

	ResourceManager<Changer_Animation>::Save(TAni->changer_animation()->name());


}


// 전체 세이브
void Dlg_KM3Loader::OnBnClickedKm3acasave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ResourceManager<Changer_Animation>::All_Save();
}


void Dlg_KM3Loader::OnBnClickedKm3resetbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_RscTree();
}



/******************* 단발애니메이션 *****************/
void Dlg_KM3Loader::OnBnClickedKm3playbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurOne)
	{
		return;
	}

	KPtr<Renderer_BonAni> TAni = m_CurOne->Get_Component<Renderer_BonAni>();

	TAni->pause_inx(-1);
}


void Dlg_KM3Loader::OnBnClickedKm3paubtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurOne)
	{
		return;
	}


	KPtr<Renderer_BonAni> TAni = m_CurOne->Get_Component<Renderer_BonAni>();
	m_AniInx = TAni->cur_frame();

	TAni->pause_inx(m_AniInx);
}


void Dlg_KM3Loader::OnBnClickedKm3prevbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurOne)
	{
		return;
	}
	
	KPtr<Changer_Animation> TmpCA = m_CurOne->Get_Component<Renderer_BonAni>()->changer_animation();
	KPtr<Renderer_BonAni> TAni = m_CurOne->Get_Component<Renderer_BonAni>();
	if (TmpCA->cur_clip()->Start > --m_AniInx)
	{
		m_AniInx = TmpCA->cur_clip()->Start;
	}

	TAni->pause_inx(m_AniInx);
}


void Dlg_KM3Loader::OnBnClickedKm3nextbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CurOne)
	{
		return;
	}
	KPtr<Changer_Animation> TmpCA = m_CurOne->Get_Component<Renderer_BonAni>()->changer_animation();
	KPtr<Renderer_BonAni> TAni = m_CurOne->Get_Component<Renderer_BonAni>();
	if (TmpCA->cur_clip()->End < ++m_AniInx)
	{
		m_AniInx = TmpCA->cur_clip()->End;
	}

	TAni->pause_inx(m_AniInx);
}
