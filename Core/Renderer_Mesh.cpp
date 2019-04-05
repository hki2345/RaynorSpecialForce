#include "Renderer_Mesh.h"
#include "KMacro.h"
#include "KWindow.h"
#include "TransPosition.h"
#include "ResourceManager.h"


Renderer_Mesh::Renderer_Mesh()
{
}

Renderer_Mesh::~Renderer_Mesh()
{
}

bool Renderer_Mesh::Init(int _Order /*= 0*/)
{
	Renderer::Init(_Order);

	Set_RSState(L"SBACK");
	return true;
}
 
