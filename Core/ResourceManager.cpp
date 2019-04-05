#include "ResourceManager.h"


#include "KMacro.h"
#include "Sound.h"
#include "KImage.h"
#include "KVideo.h"
#include "KM3.h"
#include "Changer_Animation.h"

#include <io.h>
#include <iostream>

#include "MultiCritical.h"

std::unordered_map<std::wstring, std::wstring> PathManager::m_PathMap;
std::wstring PathManager::m_PathForder;

void PathManager::Init()
{
	wchar_t Arr[1000];
	GetCurrentDirectoryW(sizeof(wchar_t) * 1000, Arr);
	m_PathForder = Arr;

	size_t CutCount = m_PathForder.find_last_of('\\', m_PathForder.size());
	m_PathForder.replace(CutCount, m_PathForder.size(), L"\\bin\\");
}

const void PathManager::Create_FilePath(const wchar_t* _Forder, const wchar_t* _FullPath) {
	if (nullptr != Find_ForderPath(_Forder))
	{
		KASSERT(true);
	}

	m_PathMap.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Forder, _FullPath));
}

const void PathManager::Create_ForderPath(const wchar_t* _Key, const wchar_t* _Folder)
{
	if (nullptr != Find_ForderPath(_Key))
	{
		KASSERT(true);
	}

	std::wstring NewPath = m_PathForder + _Folder + L"\\";
	m_PathMap.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Key, NewPath));
}

const void PathManager::Create_ForderPath(const wchar_t* _Key) {
	Create_ForderPath(_Key, _Key);
}



const wchar_t* PathManager::Find_ForderPath(const wchar_t* _Key)
{
	std::unordered_map<std::wstring, std::wstring>::iterator PathIter = m_PathMap.find(_Key);

	if (PathIter == m_PathMap.end())
	{
		return nullptr;
	}

	return PathIter->second.c_str();
}

std::wstring PathManager::Find_ForderPathStr(const wchar_t* _Key)
{
	return Find_ForderPath(_Key);
}

// �ش� ������ ���� �̸��� ���� TempPath ��ü�� �����´�.
std::wstring PathManager::Add_FolderFile(const wchar_t* _FolderKey, const wchar_t* _FileName)
{
	std::wstring Temp = Find_ForderPath(_FolderKey);
	return Temp += _FileName;
}


// ���� �ִ��� Ȯ��
const bool PathManager::IsFile(const wchar_t* _Path)
{
	int R = _waccess_s(_Path, 0) + 1;

	if (0 == R)
	{
		return false;
	}

	return true;
}


std::wstring PathManager::Chain_StringInt(const std::wstring& _Name, const int& _Value)
{
	wchar_t Temp[16];

	_itow_s(_Value, Temp, 16, 10);

	return _Name + Temp;
}



std::wstring PathManager::Split_FileName(const wchar_t* _Path)
{
	std::wstring TempPath = _Path;

	wchar_t ArrDrive[128] = { 0, };
	wchar_t ArrFolder[128] = { 0, };
	wchar_t ArrFile[128] = { 0, };
	wchar_t ArrExist[128] = { 0, };

	// ���� ��θ� ��´�.
	_wsplitpath_s(TempPath.c_str(), ArrDrive, ArrFolder, ArrFile, ArrExist);

	std::wstring Exi = ArrExist;
	std::wstring Name = ArrFile;

	return Name + Exi;
}

std::wstring PathManager::Split_Path(const std::wstring& _ALL, const std::wstring& _FPath)
{
	if (L"" == _ALL)
	{
		return L"";
	}

	wchar_t Tmp[PATHNUM] = { 0, };

	size_t Start = 0;
	
	// ���ۺκ� ã�°���
	for (size_t i = 0; i < _ALL.size(); i++)
	{
		if (_ALL[i] == _FPath[i])
		{
			++Start;
			continue;
		}
		break;
	}

	// ���� ����
	for (size_t i = Start; i < _ALL.size(); i++)
	{
		for (size_t j = 0; j < _ALL.size() - Start; j++)
		{
			if (0 != Tmp[j])
			{
				continue;
			}

			Tmp[j] = _ALL[i];
			break;
		}
	}

	
	return Tmp;
}


int	PathManager::Convert_Str2Int(const std::wstring& _Path)
{
	// char - atoi()
	return _wtoi(_Path.c_str());
}

int	PathManager::Convert_Str2Int(const wchar_t* _Path)
{
	return _wtoi(_Path);
}


std::wstring PathManager::Convert_Int2Str(const int& _Value)
{
	wchar_t NUM[NAMENUM];
	_itow_s(_Value, NUM, 10);
	return NUM;
}

std::wstring PathManager::Convert_Float2Str(const float& _Value)
{
	wchar_t NUM[NAMENUM];

	return NUM;
}


// �н��� ������ �˻�
bool PathManager::Is_StrVSStr(const wchar_t* _Path1, const wchar_t* _Path2)
{
	if ((_Path1[0] == 0 && _Path2[0] != 0 )||
		(_Path1[0] != 0 && _Path2[0] == 0))
	{
		return false;
	}

	for (size_t i = 0; 0 != _Path1[i] && 0 != _Path2[i]; i++)
	{
		if (_Path1[i] != _Path2[i])
		{
			return false;
		}
	}

	return true;
}

/********************* All KResource Load **********************/
// ������ ����� �Լ� - ���ø��� ����� ������ �Ǵµ� �׷��⿣ ����� ���� �ʹ� ���� �������־
// �̷��� ���� �Լ� ���� ������ ����
bool ResourceManager<KImage>::All_Load()
{
	return ResourceManager<KImage>::All_LoadOrigin(L"Texture");
}
bool ResourceManager<Sound>::All_Load()
{
	return ResourceManager<Sound>::All_LoadOrigin(L"Sound");
}
bool ResourceManager<MeshContainer>::All_Load()
{
	return ResourceManager<MeshContainer>::All_LoadOrigin(L"KM3");
}
bool ResourceManager<Changer_Animation>::All_Load()
{
	return ResourceManager<Changer_Animation>::All_LoadOrigin(L"KCA");
}
bool ResourceManager<KVideo>::All_Load()
{
	return ResourceManager<KVideo>::All_LoadOrigin(L"Video");
}



// ¥�� �ؽ��ĸ� ��� ���� ���̱� �빮��
// afx�� ����� �߰��Ǵ� �ְ� �̻�������. b.b
template<typename KS>
bool ResourceManager<KS>::All_LoadOrigin(const wchar_t* _Target)
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
	ResourceManager<KS>::All_LoadSub(Handle, FD, _Target);

	return true;
}


template<typename KS>
bool ResourceManager<KS>::All_LoadSub(const intptr_t& _Handle, _wfinddata_t& _FD, const wchar_t* _Target)
{
	do
	{
		// �����̴�.
		if (true == ResourceManager<KS>::IsDot(_FD.name))
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

			Name += Exi;

			// ������ ����
			if (Exi == L".png" || Exi == L".PNG" || Exi == L".Png" ||
				Exi == L".jpg" || Exi == L".JPG" || Exi == L".Jpg" || 
				Exi == L".bmp" || Exi == L".dds" || Exi == L".DDS" ||
				Exi == L".mp3" || Exi == L".ogg" || Exi == L".wav" ||
				Exi == L".KM3" || Exi == L".km3" || Exi == L".KCA" ||
				Exi == L".AVI" || Exi == L".avi" )
			{
				// �̰��ؾ� �������� �� �ҷ��� �� ���� �ʰ��;
				if (nullptr == ResourceManager<KS>::Find(Folder.c_str()))
				{
					ResourceManager<KS>::Load(Folder.c_str(), (Name).c_str());
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

			ResourceManager<KS>::All_LoadOrigin(Temp.c_str());
		}
	} while (_wfindnext(_Handle, &_FD) == 0);


	return true;
}

template<typename KS>
bool ResourceManager<KS>::IsDot(const std::wstring& _Value)
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