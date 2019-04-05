#pragma once
#include "Renderer.h"
#include "KStreamBuffer.h"
#include <vector>


class Texture;
class Renderer_Effect : public Renderer
{
public:
	class Effect_Data
	{
	public:
		KVector Pos;
		KVector Value;
	};

	class Effect_Begin
	{
	public:
		KVector Dir;
		float LiveTime;
		float Speed;
	};

public:
	KUINT CurLive_Cnt;
	KUINT Live_Cnt;


private:
	float Time;
	
	// StreamBuffer
	std::vector<Effect_Data> m_EFDVec;
	std::vector<Effect_Data> m_EFLDVec;
	std::vector<Effect_Begin> m_EFBVec;

	KPtr<Texture> m_Tex;
	KPtr<KStreamBuffer> m_DataBuf;
	KPtr<KStreamBuffer> m_SOBuffer;


	KPtr<KMaterial> m_SMtl;
	KPtr<KMaterial> m_RMtl;

	KUINT m_MaxEFCnt;
	float m_UpdateTime;


public:
	KPtr<KMaterial> render_material();
	// void Check_Live();

public:
	bool Init(const KUINT& _DotCnt, const int& _Order = 0);
	void Update() override;
	void Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data);

public:
	Renderer_Effect();
	~Renderer_Effect();
};

