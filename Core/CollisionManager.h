#pragma once
#include <set>
#include <unordered_map>
#include <list>
#include "DXContainer.h"
#include "KCollider2D_DE.h"

class State;
class CollisionManager
{
public:
	friend State;

private:
	std::set<__int64>::iterator StartColIter;
	std::set<__int64>::iterator EndColIter;
	std::set<__int64> m_Link;

	std::unordered_map<int, std::list<KPtr<KCollision>>>::iterator LeftGIter;
	std::unordered_map<int, std::list<KPtr<KCollision>>>::iterator RightGIter;
	std::unordered_map<int, std::list<KPtr<KCollision>>> m_ColMap;

	std::list<KPtr<KCollision>>::iterator LeftStartIter;
	std::list<KPtr<KCollision>>::iterator LeftEndIter;
	std::list<KPtr<KCollision>>::iterator RightStartIter;
	std::list<KPtr<KCollision>>::iterator RightEndIter;

public:
	void Link(int _Value);
	void Link(int _Left, int Right);
	void PushCol(KPtr<KCollision> _Col);
	std::list<KPtr<KCollision>> AllUpdateColCheck(int _Order, const Figure_Col* _pColFi);
	KPtr<KCollision> UpdateColCheck(int _Order, const Figure_Col* _pColFi);

private:
	void Progress();
	void Release();

private:
	void PushOverCol(KPtr<KCollider2D_DE> _Renderer);

public:
	CollisionManager();
	~CollisionManager();
};

