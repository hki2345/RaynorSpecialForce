#pragma once
#include "Renderer.h"
#include "KStreamBuffer.h"
#include <vector>


class Texture;
class KMaterial;
class Renderer_AniEffect : public Renderer
{
public:
	Renderer_AniEffect();
	~Renderer_AniEffect();

private:
	float m_InterTime;
	float m_EffectTime;
	bool m_Loop;
	bool m_Depth;
	int X;
	int Y;
	int CURX;
	int CURY;

	ANIEFFCB m_Data;

public:
	KPtr<KMaterial> m_Mtl;

public:
	void EffectSetting(const wchar_t* _TexName, int X, int Y, bool _Loop, float _InterTime = 1.0f);
	void EffectSetting(const wchar_t* _TexName, int X, int Y, bool _Loop, bool _Depth, float _InterTime = 1.0f);

public:
	bool Init(const int& _Order = 0);
	void Update_Trans(KPtr<Camera> _Camera) override;
	void Update() override;
	void Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data);

};

