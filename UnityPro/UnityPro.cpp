
// UnityPro.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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


// UnityPro 생성

UnityPro::UnityPro()
{
	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UnityPro.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 UnityPro 개체입니다.

UnityPro theApp;


// UnityPro 초기화

BOOL UnityPro::InitInstance()
{
	//////////////// Client 면 0 ///////////////


	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

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
	//TODO: 추가한 추가 리소스를 처리합니다.
	return CWinApp::ExitInstance();
}

// UnityPro 메시지 처리기


void UnityPro::Product_Cpp(const wchar_t* _Path, const wchar_t* _Name, const PROD_TYPE& _Type)
{
	CFileFind FileFind;
	CString Find_ForderPath = _Path;
	BOOL bFile = FileFind.FindFile(Find_ForderPath + L"*.cpp");

	// 폴더 -> 내부에 cpp
	// 유니티 cpp -> cpp를 모두다 모아놓은 파일
	// 폴더의 수정시간 > 유니티 cpp 의 수정시간보다 크면
	// 그때만 cpp를 만든다.


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


