// Dlg_FbxLoad.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_FbxLoad.h"
#include "afxdialogex.h"


#include <Core_Class.h>
#include <Renderer_BonAni.h>


#include <KThread.h>

// Dlg_FbxLoad 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_FbxLoad, TabDlg)

Dlg_FbxLoad::Dlg_FbxLoad(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_FBXDLG, pParent)
{

}

Dlg_FbxLoad::~Dlg_FbxLoad()
{
}

void Dlg_FbxLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FBXLIST, m_RscList);

	for (int i = 0; i < 4; i++)
	{
		DDX_Control(pDX, IDC_DIFFBTN + i, m_TexBtn[i]);
		DDX_Control(pDX, IDC_DIFFPATH + i, m_TexPath[i]);
	}
}


BEGIN_MESSAGE_MAP(Dlg_FbxLoad, TabDlg)
	ON_LBN_SELCHANGE(IDC_FBXLIST, &Dlg_FbxLoad::OnLbnSelchangeFbxlist)
	ON_BN_CLICKED(IDC_FBXLOADBTN, &Dlg_FbxLoad::OnBnClickedFbxloadbtn)
	ON_BN_CLICKED(IDC_TOKM3, &Dlg_FbxLoad::OnBnClickedTokm2)
	
	
	ON_BN_CLICKED(IDC_DIFFBTN, &Dlg_FbxLoad::OnBnClickedDiffbtn)
	ON_BN_CLICKED(IDC_NORMBTN, &Dlg_FbxLoad::OnBnClickedNormbtn)
	ON_BN_CLICKED(IDC_SPECBTN, &Dlg_FbxLoad::OnBnClickedSpecbtn)
	ON_BN_CLICKED(IDC_EMISBTN, &Dlg_FbxLoad::OnBnClickedEmisbtn)
	ON_BN_CLICKED(IDC_LISTUPBTN, &Dlg_FbxLoad::OnBnClickedListupbtn)
END_MESSAGE_MAP()


void Dlg_FbxLoad::Init_Dlg()
{
	Update_RscTree();
}


void Dlg_FbxLoad::Hide_Dlg()
{
	if (nullptr != m_CurOne)
	{
		m_CurOne->Set_Death();
	}
}
// Dlg_FbxLoad 메시지 처리기입니다.


//-CFileDialog 정의
//CFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, 
//	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWITEPROMPT, 
//	OPCTSTR lpszFilter = NULL, CWnd *pParentWnd = NULL)

//-생성자 파라미터
//bOpenFileDialog : TRUE:열기, FALSE : 다른 이름으로저장
//	lpszDefExt : 기본 확장자. (ex. "*.jpg")
//	lpszFileName : 처음 대화상자가 출력 시 '파일 이름(N)'에 지정될 파일명
//	dwFlags :
//
//OFN_EXPLOPER: 윈도우 탐색기 스타일로 출력
//	OFN_ALLOWMULTISELECT : 다중 선택
//	OFN_CREATEPROMPT : 존재하지 않는 파일명 입력 시, 새로운 파일을 생성할 것인지 물음
//	OFN_FILEMUSTEXIST : 존재하지 않는 파일명 입력 불가
//	OFN_HIDEREADONLY : 읽기전용 파일은 보이지 않음
//	OFN_LONGNAMES : 긴 파일 이름 포맷 지원
//	OFN_OVERWRITEPROMPT : 이미 존재하는 파일 명일 경우 덮어쓸 것인가를 물음
//	OFN_PATHMUSTEXIST : 이미 존재하는 디렉터리명만을 입력
//	lpszFilter : 보여질 파일들에 대한 필터
//	pParentWnd : 대화상자의 부모 윈도우 지정
//
//
//	-멤버 함수
//	CString GetPathName() : 선택된 파일의 절대경로
//	CString GetFileName() : 선택된 파일의 이름과 확장자
//	CString GetFileExt() : 선택된 파일의 확장자
//	String GetFileTitle() : 선택된 파일의 파일명
//	BOOL GetReadOnlyPref() : 읽기전용 여부
//	POSITION GetStartPosition() : 다중 선택시 첫번째 파일의 위치
//	CString GetNextPathName() : 다중 선택시 다음 파일의 절대경로
//  다중 사용 문제점 
// https://m.blog.naver.com/PostView.nhn?blogId=jalaint&logNo=150073298476&proxyReferer=http%3A%2F%2Fwww.google.com%2Furl%3Fsa%3Dt%26rct%3Dj%26q%3D%26esrc%3Ds%26source%3Dweb%26cd%3D4%26ved%3D2ahUKEwij-v6otO_eAhVTQLwKHRBODwkQFjADegQIBhAB%26url%3Dhttp%253A%252F%252Fm.blog.naver.com%252Fjalaint%252F150073298476%26usg%3DAOvVaw2S-tLlWcpNhuAf2fJeXYXj

void Dlg_FbxLoad::OnBnClickedFbxloadbtn()
{
	// 이상하게 단일 그걸로는잘되는데 이게 창띄우고  mfc랑 엮이니까 안된다. ㅠㅠ;;
	// KThread::Start_Thread<Dlg_FbxLoad>(L"TestThread", &Dlg_FbxLoad::Load_ByThread, this);
	static TCHAR BASED_CODE szFilter[] = _T("FBX 파일(*.FBX) | *.FBX;*.fbx; |모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.FBX"), _T("*.fbx"), OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, szFilter, this);

	bool Check;

	if (IDOK == dlg.DoModal())
	{
		// 한번에 불러오려는 포스 ->
		// 이 좌표 다음이 바로 다음 선택한 파일의 패스를 나타냄
		POSITION pos = dlg.GetStartPosition();
		CString m_FileList = L"";

		while (pos)
		{
			pathName = dlg.GetNextPathName(pos);


			KPtr<MeshContainer> m_CurMesh = ResourceManager<MeshContainer>::Find(PathManager::Split_FileName(pathName).c_str());
			if (nullptr != m_CurMesh)
			{
				MessageBox(pathName + L"\n이미 불러온 파일입니다.\n불러오기를 계속 진행합니다.");
				continue;
			}


			//KThread::Start_Thread<Dlg_FbxLoad>(L"Load_FBX", &Dlg_FbxLoad::Load_ByThread, this);
			m_CurMesh = ResourceManager<MeshContainer>::Load(pathName.GetBuffer(), MESH_LMODE::LM_FBX);
			if (nullptr == m_CurMesh)
			{
				Check = false;
				MessageBox(pathName + L"\n파일을 불러오지 못했습니다.");
				return;
			}


			pathName.Delete(0, pathName.ReverseFind('\\' + 1));
			m_FileList += (pathName + _T("\r\n"));
		}

		Check = true;
	}
	else
	{
		return;
	}

	if (true == Check)
	{
		MessageBox(L"성공적으로 FBX를 불러왔습니다.");
	}

	Update_RscTree();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 되는데 이게 메시지창 띄우고 결정적으로 mfc 리스트 업데이트 하려니까 터진다. ㅠㅡㅠ;;
// 정확히는 터지는 건 아닌데 최신화를 못함 - 리소스 메니저엔 들어이쓴ㄴ데 말이지...
// UpdateData하면 터진다 ㅅㅂ
unsigned int Dlg_FbxLoad::Load_ByThread(void* _Test)
{
	ResourceManager<MeshContainer>::Load(pathName.GetBuffer(), MESH_LMODE::LM_FBX);
	return 0;
}


void Dlg_FbxLoad::Update_RscTree()
{
	UpdateData(TRUE);	
	
	// 리스트 초기화
	m_RscList.ResetContent();

	std::vector<KPtr<MeshContainer>> TVec = ResourceManager<MeshContainer>::All_SingleVec();

	for (size_t i = 0; i < TVec.size(); i++)
	{
		if (L".FBX" == TVec[i]->ws_fileext() || L".fbx" == TVec[i]->ws_fileext())
		{
			m_RscList.AddString(TVec[i]->FileNameExt());
		}
	}

	UpdateData(FALSE);
}

void Dlg_FbxLoad::Update_Path()
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


void Dlg_FbxLoad::Update_Tex(const TEX_TYPE& _Value, const int& _Inx)
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


void Dlg_FbxLoad::OnLbnSelchangeFbxlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Tint = m_RscList.GetCurSel();
	if (-1 == Tint)
	{
		return;
	}

	if (nullptr != m_CurOne)
	{
		m_CurOne->Set_Death();
	}

	CString TempStr;
	m_RscList.GetText(Tint, TempStr);

	m_CurOne = Core_Class::MainScene()->Create_One(L"FBX_LOAD");
	m_CurOne->Trans()->pos_local(KVector(.0f));
	m_CurOne->Trans()->scale_local(KVector(1.f, 1.f, 1.f));
	KPtr<Renderer_BonAni> TRender = m_CurOne->Add_Component<Renderer_BonAni>();

	TRender->Set_Fbx(TempStr);
	TRender->Create_Clip(L"ALLAni", 0, 100000);
	TRender->Set_Clip(L"ALLAni");


	Update_Path();
}


void Dlg_FbxLoad::OnBnClickedTokm2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::vector<KPtr<MeshContainer>> TVec = ResourceManager<MeshContainer>::All_SingleVec();

	if (0 == TVec.size())
	{
		MessageBox(L"변환할 파일이 없습니다.");
		return;
	}

	for (size_t i = 0; i < TVec.size(); i++)
	{
		std::wstring Temp = PathManager::Find_ForderPath(L"KM3");
		std::wstring TN = TVec[i]->FileName();
		std::wstring TE = L".KM3";

		TVec[i]->Save((Temp + TN + TE).c_str());
	}


	MessageBox(L"성공적으로 변환되었습니다.");
}




void Dlg_FbxLoad::OnBnClickedDiffbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_Tex(TEX_TYPE::TEX_COLOR, 0);
}


void Dlg_FbxLoad::OnBnClickedNormbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_Tex(TEX_TYPE::TEX_BUMP, 1);
}


void Dlg_FbxLoad::OnBnClickedSpecbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_Tex(TEX_TYPE::TEX_SPEC, 2);
}


void Dlg_FbxLoad::OnBnClickedEmisbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_Tex(TEX_TYPE::TEX_EMIS, 3);
}



std::wstring Dlg_FbxLoad::Show_DDSWindow()
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

void Dlg_FbxLoad::OnBnClickedListupbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_RscTree();
}
