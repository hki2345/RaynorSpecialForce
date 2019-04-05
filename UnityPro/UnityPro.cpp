
// UnityPro.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "UnityPro.h"
#include <string>
#include <unordered_map>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif




class PathPro
{
public:
	static std::wstring RPath;
	static std::wstring ClientPath;
	static std::wstring EditorPath;

	static void Init()
	{
		wchar_t Temp[1000];
		GetCurrentDirectoryW(sizeof(wchar_t) * 1000, Temp);

		RPath = Temp;
		ClientPath = Temp;
		EditorPath = Temp;

		size_t Cnt = RPath.find_last_of('\\', RPath.size());
		RPath.replace(Cnt, RPath.size(), L"\\");
		ClientPath.replace(Cnt, RPath.size(), L"\\UClient\\");
		EditorPath.replace(Cnt, RPath.size(), L"\\UEditor\\");
	}
};


std::wstring PathPro::RPath;
std::wstring PathPro::ClientPath;
std::wstring PathPro::EditorPath;


// UnityPro

BEGIN_MESSAGE_MAP(UnityPro, CWinApp)
END_MESSAGE_MAP()


// UnityPro ����

UnityPro::UnityPro()
{
	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UnityPro.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ UnityPro ��ü�Դϴ�.

UnityPro theApp;


// UnityPro �ʱ�ȭ

BOOL UnityPro::InitInstance()
{
	//////////////// Client �� 0 ///////////////


	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	PathPro::Init();



	Product_Cpp((PathPro::RPath + L"Core\\").c_str(), L"UCore.cpp", PROD_TYPE::PT_EDITOR);
	Product_Cpp((PathPro::RPath + L"Container\\").c_str(), L"UContainer.cpp", PROD_TYPE::PT_EDITOR);
	Product_Cpp((PathPro::RPath + L"Editor\\").c_str(), L"UEditor.cpp", PROD_TYPE::PT_EDITOR);


	Product_Cpp((PathPro::RPath + L"Core\\").c_str(), L"UCore.cpp", PROD_TYPE::PT_CLIENT);
	Product_Cpp((PathPro::RPath + L"Container\\").c_str(), L"UContainer.cpp", PROD_TYPE::PT_CLIENT);
	Product_Cpp((PathPro::RPath + L"Client\\").c_str(), L"UClient.cpp", PROD_TYPE::PT_CLIENT);
	
	return TRUE;
}

int UnityPro::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	return CWinApp::ExitInstance();
}

// UnityPro �޽��� ó����


void UnityPro::Product_Cpp(const wchar_t* _Path, const wchar_t* _Name, const PROD_TYPE& _Type)
{
	CFileFind FileFind;
	CString Find_ForderPath = _Path;
	BOOL bFile = FileFind.FindFile(Find_ForderPath + L"*.cpp");

	// ���� -> ���ο� cpp
	// ����Ƽ cpp -> cpp�� ��δ� ��Ƴ��� ����
	// ������ �����ð� > ����Ƽ cpp �� �����ð����� ũ��
	// �׶��� cpp�� �����.


	CString SavePath = L"";

	switch (_Type)
	{
	case PROD_TYPE::PT_CLIENT:
		SavePath = PathPro::ClientPath.c_str();
		break;

	case PROD_TYPE::PT_EDITOR:
		SavePath = PathPro::EditorPath.c_str();
		break;

	default:
		break;
	}



	SavePath += _Name;

	_WIN32_FILE_ATTRIBUTE_DATA FolderAttData;
	_WIN32_FILE_ATTRIBUTE_DATA FileAttData;
	LARGE_INTEGER FolderTime;
	LARGE_INTEGER FileTime;


	GetFileAttributesEx(_Path, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &FolderAttData);
	GetFileAttributesEx(SavePath.GetString(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &FileAttData);
	memcpy_s(&FolderTime, sizeof(LARGE_INTEGER), &(FolderAttData.ftLastWriteTime), sizeof(LARGE_INTEGER));
	memcpy_s(&FileTime, sizeof(LARGE_INTEGER), &(FileAttData.ftLastWriteTime), sizeof(LARGE_INTEGER));

	if (FolderTime.QuadPart < FileTime.QuadPart)
	{
		return;
	}

	FILE* P;
	_wfopen_s(&P, SavePath.GetString(), L"wt");
/*
	CString SavePath2 = PathPro::RPath.c_str();
	SavePath2 += L"AR14TOOLUNITY\\";
	SavePath2 += _Name;

	FILE* P1;
	_wfopen_s(&P1, SavePath2.GetString(), L"wt");
*/
	while (bFile)
	{
		bFile = FileFind.FindNextFileW();

		// "."
		// ".."

		if (FileFind.IsDots())
		{
			continue;
		}
		else if (FileFind.IsDirectory())
		{
			continue;
		}
		else
		{
			// CString PathName = FileFind.GetFilePath();
			CString FileName = FileFind.GetFileName();

			std::string TextName = "#include \"";
			TextName += CW2A(FileName);
			TextName += "\"\n";

			fwrite(TextName.c_str(), TextName.size(), 1, P);
			//fwrite(TextName.c_str(), TextName.size(), 1, P1);
		}
	}

	fclose(P);
	// fclose(P1);

	FileFind.Close();
}


