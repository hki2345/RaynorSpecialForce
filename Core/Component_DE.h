#pragma once
#include "TransPosition.h"


// 실상 모든 컴퍼넌트는 시간의 영향을 받는다.
#include "TimeManager.h"

// 컴퍼넌트 딜럭스 에디션은
// 기본적으로 트랜스를 가져야할 - 무궁무진하게 쓰임
// 컴퍼넌트들이 바로 사용할 수 있게 들고 있는 것이다.
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

