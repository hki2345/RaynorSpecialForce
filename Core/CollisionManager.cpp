#include "CollisionManager.h"
#include "KMacro.h"

CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::Progress() 
{
	StartColIter = m_Link.begin();
	EndColIter = m_Link.end();

	for (; StartColIter != EndColIter; ++StartColIter)
	{
		// 
		int_compare Link = (*StartColIter);

		LeftGIter = m_ColMap.find(Link.Left);
		RightGIter = m_ColMap.find(Link.Right);

		if (LeftGIter == m_ColMap.end() || RightGIter == m_ColMap.end())
		{
			continue;
		}

		if (0 >= LeftGIter->second.size() || 0 >= RightGIter->second.size())
		{
			continue;
		}

		LeftStartIter = LeftGIter->second.begin();
		LeftEndIter = LeftGIter->second.end();
		for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
		{
			(*LeftStartIter)->Update_Figure();
		}

		RightStartIter = RightGIter->second.begin();
		RightEndIter = RightGIter->second.end();
		for (; RightStartIter != RightEndIter; ++RightStartIter)
		{
			(*RightStartIter)->Update_Figure();
		}

		if (Link.Left == Link.Right)
		{
			LeftStartIter = LeftGIter->second.begin();
			LeftEndIter = LeftGIter->second.end();

			for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
			{
				if (false == (*LeftStartIter)->Is_Active())
				{
					continue;
				}

				RightStartIter = RightGIter->second.begin();
				RightEndIter = RightGIter->second.end();

				for (; RightStartIter != RightEndIter; ++RightStartIter)
				{
					if (false == (*RightStartIter)->Is_Active() ||
						LeftStartIter == RightStartIter)
					{
						continue;
					}

					(*LeftStartIter)->ColCheck((*RightStartIter));
				} // for (; RightStartIter != RightEndIter; ++RightStartIter)
			} // for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
		}
		else 
		{ // 
			LeftStartIter = LeftGIter->second.begin();
			LeftEndIter = LeftGIter->second.end();

			for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
			{
				if (false == (*LeftStartIter)->Is_Active())
				{
					continue;
				}

				RightStartIter = RightGIter->second.begin();
				RightEndIter = RightGIter->second.end();

				for (; RightStartIter != RightEndIter; ++RightStartIter)
				{
					if (false == (*RightStartIter)->Is_Active())
					{
						continue;
					}

					(*LeftStartIter)->ColCheck((*RightStartIter));
				} // for (; RightStartIter != RightEndIter; ++RightStartIter)
			} // for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
		}
	}

}
void CollisionManager::PushCol(KPtr<KCollision> _Col)
{
	if (nullptr == _Col)
	{
		return;
	}

	std::unordered_map<int, std::list<KPtr<KCollision>>>::iterator FindIter = m_ColMap.find(_Col->m_Order);

	if (FindIter == m_ColMap.end())
	{
		m_ColMap.insert(std::unordered_map<int, std::list<KPtr<KCollision>>>::value_type(_Col->m_Order, std::list<KPtr<KCollision>>()));
		FindIter = m_ColMap.find(_Col->m_Order);
	}

	FindIter->second.push_back(_Col);
}
void CollisionManager::Link(int _Value)
{
	Link(_Value, _Value);
}
void CollisionManager::Link(int _Left, int _Right) 
{
	int_compare Index = { _Left , _Right };

	std::unordered_map<int, std::list<KPtr<KCollision>>>::iterator FindIter;
	FindIter = m_ColMap.find(_Left);
	if (FindIter == m_ColMap.end())
	{
		m_ColMap.insert(std::unordered_map<int, std::list<KPtr<KCollision>>>::value_type(_Left, std::list<KPtr<KCollision>>()));
	}

	FindIter = m_ColMap.find(_Right);
	if (FindIter == m_ColMap.end())
	{
		m_ColMap.insert(std::unordered_map<int, std::list<KPtr<KCollision>>>::value_type(_Right, std::list<KPtr<KCollision>>()));
	}

	std::set<__int64>::iterator Iter = m_Link.find(Index);

	if (Iter != m_Link.end())
	{
		return;
	}

	m_Link.insert(Index);
}

void CollisionManager::PushOverCol(KPtr<KCollider2D_DE> _Collision) 
{
	KASSERT(nullptr == _Collision);

	std::unordered_map<int, std::list<KPtr<KCollision>>>::iterator FindGIter = m_ColMap.find(_Collision->m_Order);

	if (FindGIter == m_ColMap.end())
	{
		m_ColMap.insert(std::unordered_map<int, std::list<KPtr<KCollision>>>::value_type(_Collision->m_Order, std::list<KPtr<KCollision>>()));
		FindGIter = m_ColMap.find(_Collision->m_Order);
	}

	FindGIter->second.push_back(_Collision);
}

std::list<KPtr<KCollision>> CollisionManager::AllUpdateColCheck(int _Order, const Figure_Col* _pColFi)
{
	std::list<KPtr<KCollision>> ReturnList;

	std::unordered_map<int, std::list<KPtr<KCollision>>>::iterator UpdateCheckIter = m_ColMap.find(_Order);

	if (UpdateCheckIter == m_ColMap.end())
	{
		return ReturnList;
	}

	std::list<KPtr<KCollision>>::iterator ULCStartIter = UpdateCheckIter->second.begin();
	std::list<KPtr<KCollision>>::iterator ULCEndIter = UpdateCheckIter->second.end();

	for (; ULCStartIter != ULCEndIter; ++ULCStartIter)
	{
		if (true == (*ULCStartIter)->FiColCheck(_pColFi))
		{
			ReturnList.push_back((*ULCStartIter));
		}
		// (*LeftStartIter)->FiColCheck();
	} // for (; LeftStartIter != LeftEndIter; ++LeftStartIter)

	return ReturnList;
}

KPtr<KCollision> CollisionManager::UpdateColCheck(int _Order, const Figure_Col* _pColFi)
{
	std::unordered_map<int, std::list<KPtr<KCollision>>>::iterator UpdateCheckIter = m_ColMap.find(_Order);

	if (UpdateCheckIter == m_ColMap.end())
	{
		return nullptr;
	}

	std::list<KPtr<KCollision>>::iterator ULCStartIter = UpdateCheckIter->second.begin();
	std::list<KPtr<KCollision>>::iterator ULCEndIter = UpdateCheckIter->second.end();

	for (; ULCStartIter != ULCEndIter; ++ULCStartIter)
	{
		if (true == (*ULCStartIter)->FiColCheck(_pColFi))
		{
			return (*ULCStartIter);
		}
		// (*LeftStartIter)->FiColCheck();
	} // for (; LeftStartIter != LeftEndIter; ++LeftStartIter)

	return nullptr;
}

void CollisionManager::Release() 
{
	std::unordered_map<int, std::list<KPtr<KCollision>>>::iterator StartIter = m_ColMap.begin();
	std::unordered_map<int, std::list<KPtr<KCollision>>>::iterator EndIter = m_ColMap.end();

	std::list<KPtr<KCollision>>::iterator ListStartIter;
	std::list<KPtr<KCollision>>::iterator ListEndIter;

	for (; StartIter != EndIter; ++StartIter)
	{
		ListStartIter = StartIter->second.begin();
		ListEndIter = StartIter->second.end();

		for (; ListStartIter != ListEndIter;)
		{
			if (true == (*ListStartIter)->Is_Death())
			{
				(*ListStartIter)->DeathRelease();
				ListStartIter = StartIter->second.erase(ListStartIter);
			}
			else {
				++ListStartIter;
			}
		}
	}

}