#pragma once
// 이안에 다른 헤더가 없다.
// 무조건 구현해주게 하고 싶을때.
// c#에서는 이런걸 인터페이스라고 한다.
class KWindow;
class Mof_KWindow
{
public:
	friend class StateManager;
	friend class TheOne;
	friend class State;

private:
	KWindow* m_pWindow;

public:
	KWindow* kwindow() 
	{
		return m_pWindow;
	}

private:
	void kwindow(KWindow* _pWindow)
	{
		m_pWindow = _pWindow;
	}

protected:
	Mof_KWindow(KWindow* _pWindow) : m_pWindow(_pWindow) {}
	Mof_KWindow() : m_pWindow(nullptr) {}
	~Mof_KWindow() {}
};

