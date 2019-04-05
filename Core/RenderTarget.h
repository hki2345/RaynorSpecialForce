#pragma once
#include "Texture.h"
#include "KMacro.h"

// 랜더링의 속력은 무엇으로 결정되는가?

// 첫번째 출력 타겟횟수
// 두번째 출력 랜더횟수
// 세번째 출력 내부연산 

// 장점
// 1. 여러가지 필요한 값들을 모아놔서 그 타겟이 다른곳에 이용될수 있다.
// 단점
// 1. 기본적으로 연산이 늘기 때문에. 1~2개만 출력하려면 
// 2. 알파처리가 힘들다. 나중에 따로 처리 해줘야 한다.

// 빛연산에 있어서 디퍼드 랜더라는것은 타겟횟수를 늘리고
// 내부연산을 극적으로 줄이는것.

// 멀티랜더 타겟이.
class RenderTarget : public KResource
{
public:
	RenderTarget();
	~RenderTarget();

private:
	KPtr<Texture>	m_Tex;
	KVector4		m_Color;
	bool			m_bClear;

public:
	KPtr<Texture> target_tex() 
	{
		return m_Tex;
	}

	void Set_Clear()
	{
		m_bClear = true;
	}
	void Set_UnClear()
	{
		m_bClear = false;
	}

	// UINT 보단 unsigned int를 손에 익히자 -> UINT는 windows.h를 필요로하는데
	// 개발 환경에 따라 없을 수도 있으니까.
public:
	bool Create(const KUINT& _W, const KUINT& _H, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(const KUINT& _W, const KUINT& _H, KVector4 _Color, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag);

	void Clear();
};

