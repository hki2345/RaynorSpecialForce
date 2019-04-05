#include "Container.h"
#include "Force_Unit.h"
#include <ResourceManager.h>



Force_Container::Force_Container()
{
}
Force_Container::Force_Container(const Force_Container& _Other) : Color(_Other.Color)
{
	memcpy_s(Name, NAMENUM, _Other.Name, NAMENUM);
}



/********************* All KResource Load **********************/
// 일종의 껍대기 함수 - 템플릿을 헤더에 묶으면 되는데 그러기엔 헤더가 지금 너무 많이 쓰여져있어서
// 이렇게 껍질 함수 만들어서 구현함 ㅇㅇ
bool SC2_ConManager<Force_Unit>::All_Load()
{
	return SC2_ConManager<Force_Unit>::All_LoadOrigin(L"KUD");
}



// 짜피 텍스쳐만 모두 받을 것이기 대문에
// afx가 헤더에 추가되니 애가 이상해진다. b.b
template<typename KS>
bool SC2_ConManager<KS>::All_LoadOrigin(const wchar_t* _Target)
{
	struct _wfinddata_t FD;
	intptr_t Handle;


	if (nullptr == PathManager::Find_ForderPath(_Target))
	{
		PathManager::Create_ForderPath(_Target);
	}


	std::wstring Path = PathManager::Find_ForderPath(_Target);


	Path += L"*.*";

	Handle = _wfindfirst(Path.c_str(), &FD);

	if (Handle == -1)
	{
		return false;
	}


	// 희한하게 dot과 double dot 까지 감지한다. 무서운 녀석 ㅋㅋ
	std::wstring FTN = FD.name;

	if (FTN == L".")
	{
		_wfindnext(Handle, &FD);
	}

	FTN = FD.name;

	if (FTN == L"..")
	{
		_wfindnext(Handle, &FD);
	}

	// 따라서 handle을 더블 닷 너머까지 한 곳에서 부터 체크함ㅇㅇ	
	SC2_ConManager<KS>::All_LoadSub(Handle, FD, _Target);

	return true;
}


template<typename KS>
bool SC2_ConManager<KS>::All_LoadSub(const intptr_t& _Handle, _wfinddata_t& _FD, const wchar_t* _Target)
{
	do
	{
		// 파일이다.
		if (true == SC2_ConManager<KS>::IsDot(_FD.name))
		{
			std::wstring TempPath = PathManager::Find_ForderPath(_Target);

			TempPath += _FD.name;

			wchar_t ArrDrive[128] = { 0, };
			wchar_t ArrFolder[128] = { 0, };
			wchar_t ArrFile[128] = { 0, };
			wchar_t ArrExist[128] = { 0, };

			// 파일 경로를 뜯는다.
			_wsplitpath_s(TempPath.c_str(), ArrDrive, ArrFolder, ArrFile, ArrExist);

			std::wstring Drive = ArrDrive;
			std::wstring Exi = ArrExist;
			std::wstring Name = ArrFile;
			std::wstring Folder = _Target;

			// 안함 - 확장자 추가
			// Name += Exi;

			// 사진과 음악
			if (Exi == L".png" || Exi == L".PNG" || Exi == L".Png" ||
				Exi == L".jpg" || Exi == L".JPG" || Exi == L".Jpg" ||
				Exi == L".bmp" || Exi == L".dds" || Exi == L".DDS" ||
				Exi == L".mp3" || Exi == L".KM3" || Exi == L".km3" ||
				Exi == L".KCA" || Exi == L".KUD")
			{
				// 이걸해야 연속으로 막 불러올 수 있지 않곗냐;
				if (nullptr == SC2_ConManager<KS>::Find(Folder.c_str()))
				{
					SC2_ConManager<KS>::Load((Name).c_str());
				}

			}
		}

		// 폴더다 - 제귀
		else
		{
			std::wstring Temp;
			Temp = _Target;
			Temp += L"\\";
			Temp += _FD.name;

			SC2_ConManager<KS>::All_LoadOrigin(Temp.c_str());
		}
	} while (_wfindnext(_Handle, &_FD) == 0);


	return true;
}

template<typename KS>
bool SC2_ConManager<KS>::IsDot(const std::wstring& _Value)
{
	for (size_t i = 0; i < _Value.size(); i++)
	{
		if (_Value[i] == L'.')
		{
			return true;
		}
	}

	return false;
}