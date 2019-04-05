#pragma once
#include "Renderer.h"


class Renderer_UI : public Renderer
{
public:
	Renderer_UI();
	~Renderer_UI();

private:
	KVector2 m_OriWinSize;
	KVector2 m_TmpWinSize;

	// 0 == 자르기
	// 1 == 페이드
	// 2 == 알파값
	float m_CutFade;
	float m_CutValue;
	float m_AlphaValue;

public:
	void Update();

	void Update_Trans(KPtr<Camera> _Camera) override;
	void Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data) override;

public:
	bool Init(int _Order = 0) override;

	void cut_fade(const float& _Value)
	{
		if (1.0f < _Value)
		{
			m_CutFade = 1.0f;
			return;
		}
		m_CutFade = _Value;
	}

	void cut_value(const float& _Value)
	{
		if (1.0f < _Value)
		{
			m_CutValue = 1.0f;
			return;
		}

		m_CutValue = _Value;
	}

	void alpha_value(const float& _Value)
	{
		if (1.0f < _Value)
		{
			m_AlphaValue = 1.0f;
			return;
		}

		m_AlphaValue = _Value;

	}


	bool Mouse_In();
};

