#pragma once
// �̾ȿ� �ٸ� ����� ����.
// ������ �������ְ� �ϰ� ������.
// c#������ �̷��� �������̽���� �Ѵ�.
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

