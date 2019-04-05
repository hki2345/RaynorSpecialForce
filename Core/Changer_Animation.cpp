#include "Changer_Animation.h"

#include "ResourceManager.h"
#include "ReadStream.h"
#include "WriteStream.h"



Changer_Animation::Changer_Animation()
{
}


Changer_Animation::~Changer_Animation()
{
}



KPtr<Changer_Animation::Ani_Clip> Changer_Animation::Find_AniClip(const wchar_t* _Name)
{
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator S = m_ACList.begin();
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator E = m_ACList.end();

	std::wstring Tmp = _Name;

	for (; S != E; ++S)
	{
		if ((*S)->Name == Tmp)
		{
			return (*S);
		}
	}

	return nullptr;
}

KPtr<Changer_Animation::Ani_Clip> Changer_Animation::Find_AniClip(const int& _Num)
{
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator S = m_ACList.begin();
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator E = m_ACList.end();

	for (int Cnt = 0; S != E; ++S, ++Cnt)
	{
		if (Cnt == _Num)
		{
			return (*S);
		}
	}

	return nullptr;
}

KPtr<Changer_Animation::Ani_Clip> Changer_Animation::Create_AniClip(const wchar_t* _Name, const int& _Start, const int& _End)
{
	if (0 == _Name[0])
	{
		BBY;
		return nullptr;
	}

	KPtr<Changer_Animation::Ani_Clip> NC = new Ani_Clip(_Start, _End);

	std::wstring Tmp = _Name;
	memcpy_s(NC->Name, sizeof(wchar_t) * NAMENUM, _Name, sizeof(wchar_t) * (Tmp.size() + 1));


	m_ACList.push_back(NC);


	return NC;
}

bool Changer_Animation::Erase_AniClip(const wchar_t* _Name)
{
	// 사이즈가 최소면 삭제되지 않는다.
	if (1 == m_ACList.size())
	{
		return false;
	}

	if (0 == _Name[0])
	{
		BBY;
		return false;
	}


	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator S = m_ACList.begin();
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator E = m_ACList.end();


	std::wstring Tmp = _Name;

	for (; S != E; ++S)
	{
		if ((*S)->Name == Tmp)
		{
			m_ACList.erase(S);
			
			m_CurClip = (*m_ACList.begin());
			return true;
		}
	}

	BBY;
	return true;
}

bool Changer_Animation::Set_AniClip(KPtr<Changer_Animation::Ani_Clip> _Other)
{
	// 안에 있는 걸 해야 방어코드가 된다.
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator S = m_ACList.begin();
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator E = m_ACList.end();

	for (; S != E; ++S)
	{
		if ((*S) == _Other)
		{
			m_CurClip = (*S);
			return true;
		}
	}
	return false;
}

bool Changer_Animation::Set_AniClip(const wchar_t* _Name)
{
	if (0 == _Name[0])
	{
		BBY;
		return false;
	}


	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator S = m_ACList.begin();
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator E = m_ACList.end();

	for (; S != E; ++S)
	{
		std::wstring Tmp = _Name;
		if ((*S)->Name == Tmp)
		{
			m_CurClip = (*S);
			return true;
		}
	}

	m_CurClip = (*m_ACList.begin());

	return false;
}


bool Changer_Animation::Set_AniClip(const int& _Num)
{
	if (0 > _Num)
	{
		BBY;
		return false;
	}


	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator S = m_ACList.begin();
	std::list<KPtr<Changer_Animation::Ani_Clip>>::iterator E = m_ACList.end();

	for (int Cnt = 0; S != E; ++S, ++Cnt)
	{
		if (Cnt == _Num)
		{
			m_CurClip = (*S);
			return true;
		}
	}

	m_CurClip = (*m_ACList.begin());

	return false;
}


void Changer_Animation::Set_Name(KPtr<Changer_Animation::Ani_Clip> _Clip, const wchar_t* _Name)
{
	m_SAC = m_ACList.begin();
	m_EAC = m_ACList.end();

	for (; m_SAC != m_EAC; ++m_SAC)
	{
		if ((*m_SAC) == _Clip)
		{
			memcpy_s((*m_SAC)->Name, sizeof(wchar_t) * NAMENUM, _Name, sizeof(wchar_t) * NAMENUM);
		}
	}
}

int Changer_Animation::contain_name(const wchar_t* _name)
{
	int cnt = 0;

	m_SAC = m_ACList.begin();
	m_EAC = m_ACList.end();

	for (; m_SAC != m_EAC; ++m_SAC)
	{
		bool Check = false;
		for (size_t j = 0; 0 != _name[j]; j++)
		{
			if (_name[j] != (*m_SAC)->Name[j])
			{
				Check = false;
				break;
			}
			else
			{
				Check = true;
			}
		}

		if (true == Check)
		{
			++cnt;
		}
	}

	return cnt;
}

bool Changer_Animation::Load()
{
	ReadStream Stream = ReadStream(AllPath());


	int Cnt;
	Stream.Read(Cnt);

	for (int i = 0; i < Cnt; i++)
	{
		Changer_Animation::Ani_Clip* NewCA = new Changer_Animation::Ani_Clip();


		Stream.Read(NewCA->Name, sizeof(wchar_t) * NAMENUM);
		Stream.Read(NewCA->Start);
		Stream.Read(NewCA->End);
		Stream.Read(NewCA->Speed);

		m_ACList.push_back(NewCA);
	}

	return true;
}

bool Changer_Animation::Save()
{
	std::wstring Path = PathManager::Find_ForderPath(L"KCA");

	WriteStream Stream = WriteStream((Path + ws_name()).c_str());


	int Cnt = (int)(m_ACList.size());


	Stream.Write(Cnt);
	m_SAC = m_ACList.begin();
	m_EAC = m_ACList.end();
	
	for (; m_SAC != m_EAC; ++m_SAC)
	{
		Stream.Write((*m_SAC)->Name, sizeof(wchar_t) * NAMENUM);
		Stream.Write((*m_SAC)->Start);
		Stream.Write((*m_SAC)->End);
		Stream.Write((*m_SAC)->Speed);
	}

	return true;
}