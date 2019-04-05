#pragma once
#include "Renderer.h"
#include "KImage.h"

class KBlend;
class HSpRenderer : public Renderer
{
private:
	KPtr<KImage> m_Img;
	KColor	     m_Color;
	size_t		 m_ImgIndex;
	
public:
	void ImageScaleSetting();

	void Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data) override;

	void Color(const KColor& _Color)
	{
		m_Color = _Color;
	}

	size_t ImageIndex()
	{
		return m_ImgIndex;
	}
	KPtr<KImage> Image() { return m_Img; }

	void ImageIndex(size_t _Index) 
	{
		m_ImgIndex = _Index;
	}

	KPtr<Texture> Texture() 
	{ 
		if (nullptr == m_Img)
		{
			return nullptr;
		}
		return m_Img->texture();
	}

	void Image(const wchar_t* _pImageName);

public:
	bool Init(int _Order = 0) override;
	bool Init(const wchar_t* _ImgName, int _Order = 0);

public:
	HSpRenderer();
	~HSpRenderer();
};

