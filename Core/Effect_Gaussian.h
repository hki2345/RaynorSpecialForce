#pragma once
#include "Camera.h"


class Effect_Gaussian : public Effect_Post
{
public:
	Effect_Gaussian();
	~Effect_Gaussian();

private:
	KPtr<KMesh>			m_Mesh;
	KPtr<KMaterial>		m_Mtl;
	KPtr<Sampler>		m_Spl;


public:
	bool Create() override;
	void Update() override;

};

