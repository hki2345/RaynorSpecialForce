#pragma once
// �̾ȿ� �ٸ� ����� ����.
// ������ �������ְ� �ϰ� ������.
// c#������ �̷��� �������̽���� �Ѵ�.
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

