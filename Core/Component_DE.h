#pragma once
#include "TransPosition.h"


// �ǻ� ��� ���۳�Ʈ�� �ð��� ������ �޴´�.
#include "TimeManager.h"

// ���۳�Ʈ ������ �������
// �⺻������ Ʈ������ �������� - ���ù����ϰ� ����
// ���۳�Ʈ���� �ٷ� ����� �� �ְ� ��� �ִ� ���̴�.
class Component_DE : public Component
{
protected:
	TransPosition* m_Trans;

public:
	TransPosition* Trans()
	{
		return m_Trans;
	}

public:
	virtual void ComInit() override;

public:
	Component_DE();
	~Component_DE();
};

