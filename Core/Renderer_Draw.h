#pragma once
#include "Renderer.h"




class KPlane3D_Col;
class RenderTarget_Multi;
class Renderer_Draw : public Renderer
{
private:
	DRAW_INFO	m_DI;
	KPtr<KPlane3D_Col> m_Col;
	KPtr<RenderTarget_Multi> m_DrawTarget;

public:
	bool Init(const int& _Order = 0);

	void Update_Trans(KPtr<Camera> _Camera) override;
	void Update();

	void RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx) override;


public:
	void brush_size(const float& _Value)
	{
		m_DI.BSize = _Value;
	}

public:
	Renderer_Draw();
	~Renderer_Draw();
};

