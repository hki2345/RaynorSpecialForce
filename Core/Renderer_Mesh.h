#pragma once
#include "Renderer.h"
#include "KImage.h"

class Renderer_Mesh : public Renderer
{

public:
	bool Init(int _Order = 0) override;

public:
	Renderer_Mesh();
	~Renderer_Mesh();
};

