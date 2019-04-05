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
// ������ ����� �Լ� - ���ø��� ����� ������ �Ǵµ� �׷��⿣ ����� ���� �ʹ� ���� �������־
// �̷��� ���� �Լ� ���� ������ ����
bool SC2_ConManager<Force_Unit>::All_Load()
{
	return SC2_ConManager<Force_Unit>::All_LoadOrigin(L"KUD");
}



// ¥�� �ؽ��ĸ� ��� ���� ���̱� �빮��
// afx�� ����� �߰��Ǵ� �ְ� �̻�������. b.b
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


	// �����ϰ� dot�� double dot ���� �����Ѵ�. ������ �༮ ����
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

	// ���� handle�� ���� �� �ʸӱ��� �� ������ ���� üũ�Ԥ���	
	SC2_ConManager<KS>::All_LoadSub(Handle, FD, _Target);

	return true;
}


template<typename KS>
bool SC2_ConManager<KS>::All_LoadSub(const intptr_t& _Handle, _wfinddata_t& _FD, const wchar_t* _Target)
{
	do
	{
		// �����̴�.
		if (true == SC2_ConManager<KS>::IsDot(_FD.name))
		{
			std::wstring TempPath = PathManager::Find_ForderPath(_Target);

			TempPath += _FD.name;

			wchar_t ArrDrive[128] = { 0, };
			wchar_t ArrFolder[128] = { 0, };
			wchar_t ArrFile[128] = { 0, };
			wchar_t ArrExist[128] = { 0, };

			// ���� ��θ� ��´�.
			_wsplitpath_s(TempPath.c_str(), ArrDrive, ArrFolder, ArrFile, ArrExist);

			std::wstring Drive = ArrDrive;
			std::wstring Exi = ArrExist;
			std::wstring Name = ArrFile;
			std::wstring Folder = _Target;

			// ���� - Ȯ���� �߰�
			// Name += Exi;

			// ������ ����
			if (Exi == L".png" || Exi == L".PNG" || Exi == L".Png" ||
				Exi == L".jpg" || Exi == L".JPG" || Exi == L".Jpg" ||
				Exi == L".bmp" || Exi == L".dds" || Exi == L".DDS" ||
				Exi == L".mp3" || Exi == L".KM3" || Exi == L".km3" ||
				Exi == L".KCA" || Exi == L".KUD")
			{
				// �̰��ؾ� �������� �� �ҷ��� �� ���� �ʰ��;
				if (nullptr == SC2_ConManager<KS>::Find(Folder.c_str()))
				{
					SC2_ConManager<KS>::Load((Name).c_str());
				}

			}
		}

		// ������ - ����
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