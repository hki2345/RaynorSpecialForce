// Dlg_FbxLoad.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Dlg_FbxLoad.h"
#include "afxdialogex.h"


#include <Core_Class.h>
#include <Renderer_BonAni.h>


#include <KThread.h>

// Dlg_FbxLoad ��ȭ �����Դϴ�.

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
// Dlg_FbxLoad �޽��� ó�����Դϴ�.


//-CFileDialog ����
//CFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, 
//	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWITEPROMPT, 
//	OPCTSTR lpszFilter = NULL, CWnd *pParentWnd = NULL)

//-������ �Ķ����
//bOpenFileDialog : TRUE:����, FALSE : �ٸ� �̸���������
//	lpszDefExt : �⺻ Ȯ����. (ex. "*.jpg")
//	lpszFileName : ó�� ��ȭ���ڰ� ��� �� '���� �̸�(N)'�� ������ ���ϸ�
//	dwFlags :
//
//OFN_EXPLOPER: ������ Ž���� ��Ÿ�Ϸ� ���
//	OFN_ALLOWMULTISELECT : ���� ����
//	OFN_CREATEPROMPT : �������� �ʴ� ���ϸ� �Է� ��, ���ο� ������ ������ ������ ����
//	OFN_FILEMUSTEXIST : �������� �ʴ� ���ϸ� �Է� �Ұ�
//	OFN_HIDEREADONLY : �б����� ������ ������ ����
//	OFN_LONGNAMES : �� ���� �̸� ���� ����
//	OFN_OVERWRITEPROMPT : �̹� �����ϴ� ���� ���� ��� ��� ���ΰ��� ����
//	OFN_PATHMUSTEXIST : �̹� �����ϴ� ���͸����� �Է�
//	lpszFilter : ������ ���ϵ鿡 ���� ����
//	pParentWnd : ��ȭ������ �θ� ������ ����
//
//
//	-��� �Լ�
//	CString GetPathName() : ���õ� ������ ������
//	CString GetFileName() : ���õ� ������ �̸��� Ȯ����
//	CString GetFileExt() : ���õ� ������ Ȯ����
//	String GetFileTitle() : ���õ� ������ ���ϸ�
//	BOOL GetReadOnlyPref() : �б����� ����
//	POSITION GetStartPosition() : ���� ���ý� ù��° ������ ��ġ
//	CString GetNextPathName() : ���� ���ý� ���� ������ ������
//  ���� ��� ������ 
// https://m.blog.naver.com/PostView.nhn?blogId=jalaint&logNo=150073298476&proxyReferer=http%3A%2F%2Fwww.google.com%2Furl%3Fsa%3Dt%26rct%3Dj%26q%3D%26esrc%3Ds%26source%3Dweb%26cd%3D4%26ved%3D2ahUKEwij-v6otO_eAhVTQLwKHRBODwkQFjADegQIBhAB%26url%3Dhttp%253A%252F%252Fm.blog.naver.com%252Fjalaint%252F150073298476%26usg%3DAOvVaw2S-tLlWcpNhuAf2fJeXYXj

void Dlg_FbxLoad::OnBnClickedFbxloadbtn()
{
	// �̻��ϰ� ���� �װɷδ��ߵǴµ� �̰� â����  mfc�� ���̴ϱ� �ȵȴ�. �Ф�;;
	// KThread::Start_Thread<Dlg_FbxLoad>(L"TestThread", &Dlg_FbxLoad::Load_ByThread, this);
	static TCHAR BASED_CODE szFilter[] = _T("FBX ����(*.FBX) | *.FBX;*.fbx; |�������(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.FBX"), _T("*.fbx"), OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, szFilter, this);

	bool Check;

	if (IDOK == dlg.DoModal())
	{
		// �ѹ��� �ҷ������� ���� ->
		// �� ��ǥ ������ �ٷ� ���� ������ ������ �н��� ��Ÿ��
		POSITION pos = dlg.GetStartPosition();
		CString m_FileList = L"";

		while (pos)
		{
			pathName = dlg.GetNextPathName(pos);


			KPtr<MeshContainer> m_CurMesh = ResourceManager<MeshContainer>::Find(PathManager::Split_FileName(pathName).c_str());
			if (nullptr != m_CurMesh)
			{
				MessageBox(pathName + L"\n�̹� �ҷ��� �����Դϴ�.\n�ҷ����⸦ ��� �����մϴ�.");
				continue;
			}


			//KThread::Start_Thread<Dlg_FbxLoad>(L"Load_FBX", &Dlg_FbxLoad::Load_ByThread, this);
			m_CurMesh = ResourceManager<MeshContainer>::Load(pathName.GetBuffer(), MESH_LMODE::LM_FBX);
			if (nullptr == m_CurMesh)
			{
				Check = false;
				MessageBox(pathName + L"\n������ �ҷ����� ���߽��ϴ�.");
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
		MessageBox(L"���������� FBX�� �ҷ��Խ��ϴ�.");
	}

	Update_RscTree();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// �Ǵµ� �̰� �޽���â ���� ���������� mfc ����Ʈ ������Ʈ �Ϸ��ϱ� ������. �ФѤ�;;
// ��Ȯ���� ������ �� �ƴѵ� �ֽ�ȭ�� ���� - ���ҽ� �޴����� ����̾����� ������...
// UpdateData�ϸ� ������ ����
unsigned int Dlg_FbxLoad::Load_ByThread(void* _Test)
{
	ResourceManager<MeshContainer>::Load(pathName.GetBuffer(), MESH_LMODE::LM_FBX);
	return 0;
}


void Dlg_FbxLoad::Update_RscTree()
{
	UpdateData(TRUE);	
	
	// ����Ʈ �ʱ�ȭ
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	std::vector<KPtr<MeshContainer>> TVec = ResourceManager<MeshContainer>::All_SingleVec();

	if (0 == TVec.size())
	{
		MessageBox(L"��ȯ�� ������ �����ϴ�.");
		return;
	}

	for (size_t i = 0; i < TVec.size(); i++)
	{
		std::wstring Temp = PathManager::Find_ForderPath(L"KM3");
		std::wstring TN = TVec[i]->FileName();
		std::wstring TE = L".KM3";

		TVec[i]->Save((Temp + TN + TE).c_str());
	}


	MessageBox(L"���������� ��ȯ�Ǿ����ϴ�.");
}




void Dlg_FbxLoad::OnBnClickedDiffbtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Update_Tex(TEX_TYPE::TEX_COLOR, 0);
}


void Dlg_FbxLoad::OnBnClickedNormbtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Update_Tex(TEX_TYPE::TEX_BUMP, 1);
}


void Dlg_FbxLoad::OnBnClickedSpecbtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Update_Tex(TEX_TYPE::TEX_SPEC, 2);
}


void Dlg_FbxLoad::OnBnClickedEmisbtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Update_Tex(TEX_TYPE::TEX_EMIS, 3);
}



std::wstring Dlg_FbxLoad::Show_DDSWindow()
{
	static TCHAR BASED_CODE szFilter[] = _T("DDS ����(*.DDS) | *.DDS;*.dds; |�������(*.*)|*.*||");

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Update_RscTree();
}
