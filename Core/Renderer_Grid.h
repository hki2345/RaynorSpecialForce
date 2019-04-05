#pragma once
#include "Renderer.h"

class Renderer_Grid : public Renderer
{
public:
	Renderer_Grid();
	~Renderer_Grid();


public:
	// KVector4.x�� BlockSize;
	// KVector4.y�� ����;
	// KVector4.z�� ����;
	// KVector4.w�� ����;

	float m_BlockSizeCur;
	float m_BlockSizeInter;
	// float m_BlockSizeMax;
	KVector4 m_GD;

public:
	void Update_Trans(KPtr<Camera> _Camera) override;
	void RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx) override;
	void Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data) override;

public:
	bool Init(int _Order = 0) override;

private:
	void Set_Data(KPtr<Camera> _Camera);

	void DebugRender() override;

};

