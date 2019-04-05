#pragma once
#include "Camera.h"

class Sampler;
class Effect_Bloom : public Effect_Post
{
public:
	Effect_Bloom();
	~Effect_Bloom();


private:
	KPtr<KMesh> m_Mesh;
	KPtr<KMaterial> m_ExMat;
	KPtr<KMaterial> m_SmallMat;
	KPtr<KMaterial> m_MergeMat;
	KPtr<Sampler> m_Smp;
	KPtr<RenderTarget_Multi> m_LightTarget;
	KPtr<RenderTarget_Multi> m_ExTarget;

	KVector ArrScreenSize[4];
	KVector ArrTargetSize[4];
public:
	bool Create() override;
	void Update() override;

};

