#pragma once


// ã�� ���� ���ϰ��� ������ �װ��� �������ִ� Return�� ���� �ʿ���
// Return�� ���� �ʿ��ϴٴ� ����
template<typename RETURN, typename MAP, typename KEY>
RETURN Map_Find(MAP& _Map, const KEY& _Key)
{
	MAP::iterator FI = _Map.find(_Key);
	if (FI == _Map.end())
	{
		return nullptr;
	}
	return FI->second;
}

// ���� �ִ� ���ٸ� �Ǻ��� �ش�.
template<typename MAP, typename KEY>
bool Is_MapFind(MAP& _Map, const KEY& _Key)
{
	MAP::iterator FindIter = _Map.find(_Key);
	if (FindIter == _Map.end())
	{
		return false;
	}
	return true;
}

template<typename STL, typename KEY>
void Map_Erase(STL& _Map, const KEY& _Key)
{
	STL::iterator FindIter = _Map.find(_Key);
	if (FindIter == _Map.end())
	{
		return;
	}
	_Map.erase(FindIter);
}


#define UPDATE_AID(THIS_DATA, START, END, ACTION) \
START = THIS_DATA.begin(); \
END = THIS_DATA.end(); \
\
for (; START != END; ++START)\
{\
	if (false == (*START)->is_Active() && true == (*START)->is_Death())\
	{\
		continue;\
	}\
\
	(*START)->ACTION;\
}