#pragma once
#include "SmartPtr.h"
#include "Stl_AID.h"

#include <unordered_map>
#include "MultiCritical.h"

class KCore;
class PathManager 
{
	friend KCore;

private:
	// �� ������ ����Ǵ� ��ü���
	// ���ӿ� ���� �����͵��� ������ ���� ���ε�
	// �� �̸��� �����ϴ� ���̴�.
	static std::wstring m_PathForder;
	static std::unordered_map<std::wstring, std::wstring> m_PathMap;

private:
	PathManager() {}
	~PathManager() {}

public:
	static const void			Create_FilePath(const wchar_t* _Key, const wchar_t* _FullPath);
	static const void			Create_ForderPath(const wchar_t* _Key, const wchar_t* _Folder);
	static const void			Create_ForderPath(const wchar_t* _Key);
	static const bool			IsFile(const wchar_t* _Path);
	static const wchar_t*		PathForder() { return m_PathForder.c_str();	}
	// static std::vector<std::wstring> FolderAllFile(const wchar_t* _Path);

	// string ��ȯ�� �ʿ���
	static const wchar_t*	Find_ForderPath(const wchar_t* _Key);
	static std::wstring		Find_ForderPathStr(const wchar_t* _Key);
	static std::wstring		Add_FolderFile(const wchar_t* _FolderKey, const wchar_t* _FileName);
	static std::wstring		Chain_StringInt(const std::wstring& _Name, const int& _Value);
	static std::wstring		Split_FileName(const wchar_t* _Path);
	static std::wstring		Split_Path(const std::wstring& _ALL, const std::wstring& _FPath);


	static std::wstring		Convert_Int2Str(const int& _Value);
	static std::wstring		Convert_Float2Str(const float& _Value);


	static int				Convert_Str2Int(const std::wstring& _Path);
	static int				Convert_Str2Int(const wchar_t* _Path);
	static bool				Is_StrVSStr(const wchar_t* _Path1, const wchar_t* _Path2);

private:
	static void Init();

public:
};

template<typename Res>
class ResourceManager
{
private:
	ResourceManager() {}
	virtual ~ResourceManager() = 0 {}

private:
	static std::unordered_map<std::wstring, KPtr<Res>> m_RSMap;

public:
	static std::vector<KPtr<Res>> All_SingleVec() 
	{
		std::vector<KPtr<Res>> vec;

		std::unordered_map<std::wstring, KPtr<Res>>::iterator StartIter = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator EndIter = m_RSMap.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			vec.push_back(StartIter->second);
		}

		return vec;
	}

	static std::list<KPtr<Res>> All_SingleList()
	{
		std::list<KPtr<Res>> ReturnList;

		std::unordered_map<std::wstring, KPtr<Res>>::iterator StartMapIter = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator EndMapIter = m_RSMap.end();

		for (; StartMapIter != EndMapIter; ++StartMapIter)
		{
			ReturnList.push_back(StartMapIter->second);
		}

		return ReturnList;
	}

	// ��� ���ҽ� ����
	static size_t All_Count() { return m_RSMap.size();	}


	static size_t Name_Count(const std::wstring _Name)
	{
		int Cnt = 0;

		std::unordered_map<std::wstring, KPtr<Res>>::iterator S = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator E = m_RSMap.end();

		for (; S != E; ++S)
		{
			if (S->second->name() == _Name)
			{
				++Cnt;
			}
		}

		return Cnt;
	}

public:
	static KPtr<Res> Find(KPtr<Res> _Ptr)
	{
		if (0 >= m_RSMap.size())
		{
			return nullptr;
		}

		std::unordered_map<std::wstring, KPtr<Res>>::iterator S = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator E = m_RSMap.end();

		for (; S != E; ++S)
		{
			if (_Ptr == S->second)
			{
				return S->second;
			}
		}

		return nullptr;
	}

	static KPtr<Res> Find(const wchar_t* _Name)
	{
		if (0 >= m_RSMap.size())
		{
			return nullptr;
		}
		KPtr<Res> pTemp = Map_Find<KPtr<Res>>(m_RSMap, _Name);

		return pTemp;
	}

	static KPtr<Res> Find_Name(const wchar_t* _Name)
	{
		if (0 >= m_RSMap.size())
		{
			return nullptr;
		}


		std::unordered_map<std::wstring, KPtr<Res>>::iterator S = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator E = m_RSMap.end();

		std::string Tmp = _Name;

		for (; S != E; ++S)
		{
			if (Tmp == S->second.ws_name())
			{
				return S->second;
			}
		}

		return nullptr;
	}


	static void Erase(const wchar_t* _Name)
	{
		m_RSMap.erase(_Name);
	}

	static void Erase(KPtr<Res> _Ptr)
	{
		std::unordered_map<std::wstring, KPtr<Res>>::iterator S = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator E = m_RSMap.end();

		for (; S != E; ++S)
		{
			if (_Ptr == S->second)
			{
				m_RSMap.erase(S);
				break;
			}
		}
	}


#pragma region CREATE
	static KPtr<Res> Create(const wchar_t* _Name)
	{
		std::unordered_map<std::wstring, KPtr<Res>>::iterator FI = m_RSMap.find(_Name);
		if (m_RSMap.end() != FI)
		{
			return FI->second;
		}

		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->name(_Name);

		// ���⼭ �����ε��� �ȵǰų� ������ ���� false�� �ްڴ�.
		if (false == NewRes->Create())
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1)
	{

		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->name(_Name);

		if (false == NewRes->Create(_1))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}


	// ���� 2�� -- ���� Ÿ���� ��Ƽ�� ���� �� �����ϴ� ���̴�.
	// ���� 2�� -- �ִ� Ŭ���� ���۰� ���� �����ϴ� ���̴�.
	template<typename V1, typename V2>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->name(_Name);

		if (false == NewRes->Create(_1, _2))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	// ���� 3�� -- �̰� ���÷��� �����ϴ� ���̴�.
	template<typename V1, typename V2, typename V3>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->name(_Name);

		if (false == NewRes->Create(_1, _2, _3))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	// ���� 4�� -- �̰� ����Ÿ���� �����ϴ� ���̴�.
	template<typename V1, typename V2, typename V3, typename V4>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	// ���� 5�� -- �׳� ���� ���߱�
	template<typename V1, typename V2, typename V3, typename V4, typename V5>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	// ���� 9�� -- �̰� �޽��� �����ϴ� ���̴�.
	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}



	// ���� 10�� -- �̰� �޽��� ���� ������ �����ϴ� ���̴�. - ������
	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9, typename V10>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9, V10 _10)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}
#pragma endregion

#pragma region LOAD
	static KPtr<Res> Load_NoneFind(const wchar_t* _Path)
	{
		Res* NewRes = new Res();
		NewRes->Split_Path(_Path);
		NewRes->name(NewRes->FileNameExt());

		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}


		std::wstring Tmp = NewRes->name();
		Tmp += PathManager::Convert_Int2Str((int)ResourceManager<Res>::Name_Count(Tmp));
		std::unordered_map<std::wstring, KPtr<Res>>::iterator FI = m_RSMap.find(Tmp);
		if (FI != m_RSMap.end())
		{
			delete NewRes;
			return FI->second;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(Tmp, NewRes));

		return NewRes;
	}

	static KPtr<Res> Load(const wchar_t* _Path)
	{
		Res* NewRes = new Res();
		NewRes->Split_Path(_Path);
		NewRes->name(NewRes->FileNameExt());		

		std::unordered_map<std::wstring, KPtr<Res>>::iterator FI = m_RSMap.find(NewRes->FileForder());
		if (FI != m_RSMap.end())
		{
			delete NewRes;
			return FI->second;
		}

		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(NewRes->FileForder(), NewRes));

		return NewRes;
	}


	template<typename V1>
	static KPtr<Res> Load(const wchar_t* _Path, V1 _1)
	{
		Res* NewRes = new Res();
		NewRes->Split_Path(_Path);
		NewRes->name(NewRes->FileNameExt());

		std::unordered_map<std::wstring, KPtr<Res>>::iterator FI = m_RSMap.find(NewRes->FileForder());
		if (FI != m_RSMap.end())
		{
			delete NewRes;
			return FI->second;
		}

		
		if (false == NewRes->Load(_1))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(NewRes->FileForder(), NewRes));

		return NewRes;
	}





	static KPtr<Res> Load(const wchar_t* _Path, const wchar_t* _Name)
	{
		std::wstring TempPath = PathManager::Find_ForderPath(_Path);
		TempPath += _Name;

		Res* NewRes = new Res();
		NewRes->name(_Name);
		NewRes->Split_Path(TempPath.c_str());
		NewRes->FileForder(_Path);
		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}


		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1>
	static KPtr<Res> Load(const wchar_t* _Path, const wchar_t* _Name, V1 _1)
	{
		std::wstring TempPath = PathManager::Find_ForderPath(_Path);
		TempPath += _Name;

		Res* NewRes = new Res();
		NewRes->name(_Name);
		NewRes->FileForder(_Path);
		NewRes->Split_Path(TempPath.c_str());
		if (false == NewRes->Load(_1))
		{
			delete NewRes;
			return nullptr;
		}


		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));

		return NewRes;
	}

	template<typename V1, typename V2>
	static KPtr<Res> Load(const wchar_t* _Path, const wchar_t* _Name, V1 _1, V2 _2)
	{
		std::wstring TempPath = PathManager::Find_ForderPath(_Path);
		TempPath += _Name;

		Res* NewRes = new Res();
		NewRes->name(_Name);
		NewRes->FileForder(_Path);
		NewRes->Split_Path(TempPath.c_str());
		if (false == NewRes->Load(_1, _2))
		{
			delete NewRes;
			return nullptr;
		}


			m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		
		return NewRes;
	}

#pragma endregion

#pragma region SAVE

	static bool Save(const wchar_t* _Path)
	{
		std::unordered_map<std::wstring, KPtr<Res>>::iterator FI = m_RSMap.find(_Path);
		if (FI == m_RSMap.end())
		{
			return false;
		}

		if (false == FI->second->Save())
		{
			return false;
		}

		return true;
	}



#pragma endregion

	static bool Clear()
	{
		m_RSMap.clear();
		return true;
	}

#pragma region Load_FromKey
	static KPtr<Res> Load_FromKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName)
	{
		std::wstring TempPath = PathManager::Find_ForderPath(_PathKey);
		TempPath += _FileName;

		Res* NewRes = new Res();
		NewRes->name(_Key);
		NewRes->FileForder(_PathKey);
		NewRes->Split_Path(TempPath.c_str());
		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}
	template<typename V1>
	static KPtr<Res> Load_FromKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName, V1 _1)
	{
		std::wstring TempPath = PathManager::Find_ForderPath(_PathKey);
		TempPath += _FileName;

		Res* NewRes = new Res();
		NewRes->name(_Key);
		NewRes->FileForder(_PathKey);
		NewRes->Split_Path(TempPath.c_str());
		if (false == NewRes->Load(_1))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}


	// ������Ʈ�� ���̴��� �����ϴ� ���̴�.
	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6>
	static KPtr<Res> Load_FromKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6)
	{
		std::wstring TempPath = PathManager::Find_ForderPath(_PathKey);
		TempPath += _FileName;

		Res* NewRes = new Res();
		NewRes->name(_Key);
		NewRes->FileForder(_PathKey);
		NewRes->Split_Path(TempPath.c_str());
		if (false == NewRes->Load(_1, _2, _3, _4, _5, _6))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}
#pragma endregion

public:
	// ��� ���ҽ��� �� �����´�

	static bool All_Load();
	static bool All_Save()
	{
		std::unordered_map<std::wstring, KPtr<Res>>::iterator S = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator E = m_RSMap.end();

		for (; S != E; ++S)
		{
			S->second->Save();
		}

		return true;
	}

private:
	// ���� ���� ������ �ҷ������� �Լ�
	static bool All_LoadOrigin(const wchar_t* _Target);

	// ��� ���ҽ��� ���������� ���� �Լ�
	static bool All_LoadSub(const intptr_t& _Handle, _wfinddata_t& _FD, const wchar_t* _Target);
	static bool IsDot(const std::wstring& _Value);

};

template<typename Res>
std::unordered_map<std::wstring, KPtr<Res>> ResourceManager<Res>::m_RSMap;
