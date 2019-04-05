#pragma once
#include <list>
#include <vector>
#include "Resource.h"

class Changer_Animation : public KResource
{
public:
	// �̰� �´� �̰� ���ҽ��� �Ǹ� �̸� �ߺ� �� ���ɼ� ����
	// �ִϸ��̼��� ��ĥ���� ��� Ŭ���� ��ĥ���� �ʹ� ����.
	class Ani_Clip : public SmartPtr
	{
	public:
		// �̸��� Begin -> string �Լ� ���� ������
		// ¥�� �°� �� ������ �ִµ�
		wchar_t Name[NAMENUM];
		int Start;
		int End;
		float Speed;


	public:
		Ani_Clip(const int& _Start, const int& _End)
			: Start(_Start), End(_End)
		{
		}

	public:
		Ani_Clip() {};
		Ani_Clip(const Ani_Clip& _Value) {};
	};

public:
	Changer_Animation();
	~Changer_Animation();


private:
	std::list<KPtr<Ani_Clip>> m_ACList;
	std::list<KPtr<Ani_Clip>>::iterator m_SAC;
	std::list<KPtr<Ani_Clip>>::iterator m_EAC;
	KPtr<Ani_Clip> m_CurClip;

public:
	bool Load();
	bool Save();


	KPtr<Ani_Clip> Create_AniClip(const wchar_t* _Name, const int& _Start, const int& _End);
	KPtr<Ani_Clip> Find_AniClip(const wchar_t* _Name);
	KPtr<Ani_Clip> Find_AniClip(const int& _Num);
	
	bool Create() { return true; }
	void Set_Name(KPtr<Ani_Clip> _Clip, const wchar_t* _Name);
	bool Erase_AniClip(const wchar_t* _Name);
	bool Set_AniClip(KPtr<Changer_Animation::Ani_Clip> _Other);
	bool Set_AniClip(const wchar_t* _Name);
	bool Set_AniClip(const int& _Num);

	
	int contain_name(const wchar_t* _name);


	std::list<KPtr<Ani_Clip>>* clip_list()
	{
		return &m_ACList;
	}

	std::vector<KPtr<Ani_Clip>> clip_vector()
	{
		std::vector<KPtr<Ani_Clip>> Tmp;

		m_SAC = m_ACList.begin();
		m_EAC = m_ACList.end();

		for (; m_SAC != m_EAC; ++m_SAC)
		{
			Tmp.push_back((*m_SAC));
		}

		return Tmp;
	}


	KPtr<Ani_Clip> cur_clip()
	{
		return m_CurClip;
	}

	int cur_index()
	{
		m_SAC = m_ACList.begin();
		m_EAC = m_ACList.end();

		int Cnt = 0;
		for (; m_SAC != m_EAC; ++m_SAC)
		{
			if ((*m_SAC) == m_CurClip)
			{
				return Cnt;
			}

			++Cnt;
		}

		return 0;
	}

	
};

