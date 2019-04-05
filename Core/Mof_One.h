#pragma once
// 이안에 다른 헤더가 없다.
// 무조건 구현해주게 하고 싶을때.
// c#에서는 이런걸 인터페이스라고 한다.
class TheOne;
class Mof_One
{
public:
	friend class TheOne;
	friend class Component;

private:
	TheOne* m_pOne;

public:
	TheOne* one()
	{
		return m_pOne;
	}

private:
	void one(TheOne* _pWindow)
	{
		m_pOne = _pWindow;
	}

protected:
	Mof_One() : m_pOne(nullptr) {}
	~Mof_One() {}
};

