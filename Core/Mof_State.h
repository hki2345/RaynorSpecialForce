#pragma once
// 이안에 다른 헤더가 없다.
// 무조건 구현해주게 하고 싶을때.
// c#에서는 이런걸 인터페이스라고 한다.
class State;
class Mof_State
{
public:
	friend class TheOne;
	friend class Component;
	friend class State;

private:
	State* m_pState;

public:
	State* state()
	{
		return m_pState;
	}

private:
	void state(State* _pScene)
	{
		m_pState = _pScene;
	}

protected:
	Mof_State() : m_pState(nullptr) {}
	~Mof_State() {}
};

