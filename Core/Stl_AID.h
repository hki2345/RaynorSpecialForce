#pragma once


// 찾는 것은 리턴값이 잇으니 그것을 리턴해주는 Return이 따로 필요함
// Return의 형이 필요하다는 말임
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

// 단지 있다 없다만 판별해 준다.
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