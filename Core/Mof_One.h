#pragma once
// �̾ȿ� �ٸ� ����� ����.
// ������ �������ְ� �ϰ� ������.
// c#������ �̷��� �������̽���� �Ѵ�.
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

