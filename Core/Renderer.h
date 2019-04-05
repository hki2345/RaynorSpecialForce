#pragma once
#include "HSubTransCom.h"


// #include "Shader_Vertex.h"
#include "KMaterial.h"
#include "KMesh.h"
#include "KBlend.h"
#include "KDevice.h"


class DRAW_INFO
{
public:
	KVector2 MUv;
	KVector2 PPUV;
	float BSize;
	float OnClick;
	float Tmp1;
	float Tmp2;
};
class Render_Data
{
public:
	friend Renderer;

public:
	KUINT Mesh;
	KUINT Vertex;
	KUINT Sub_inx;
	KUINT Material;

private:
	Render_Data(const KUINT& _Mesh, const KUINT& _Vtx, const KUINT& _Sub, const KUINT& _Mtl)
		: Mesh(_Mesh), Vertex(_Vtx), Sub_inx(_Sub), Material(_Mtl)
	{

	}
};

enum RENDER_DATATYPE
{
	RDT_BASE,
	RDT_DATA,
	RDT_ONE,
};

class RenderOption
{
public:
	RenderOption() :
		IsLight(1),
		TexCnt(0),
		VTz_PXo(1),
		// 0 - 포워드 1 - 디퍼드
		Defferd_orForward(1),
		IsBoneAni(0), // 0 - 본 애니 아님
		LightOpacity(-1.0f),
		Alpha(0)
	{	}
	// ~RenderOption();


private:
	friend class Renderer;

public:
	int IsLight;
	int Defferd_orForward;
	int VTz_PXo;  // Vert -> 0, Pixel -> 1
	int TexCnt;
	int IsBoneAni; // 이것이 애니메이션이냐 -> 인트로 해야하는데 일단 박아넣음
	RENDER_DATATYPE Render_DT;

	// 이게 바로 인트 하나 추가하므로서 생긴 짜투리
	float LightOpacity;

	// 알파냐 아니냐 기본 0 - 알파 1
	int Alpha;


private:
	Tex_Data TexArr[8];

};


class Camera;
class KMesh;
class KMaterial;
class Renderer : public HSubTransCom
{
public:
	Renderer();
	~Renderer();


	friend class RenderManager;


protected:
	// 픽셀 버텍스에서 쓰이는
	// 아주 기본적인 메트릭스 배열같은 것들은 아예 랜더러가 가지고 있는것으로 한다.
	MatrixContainer		m_MD;
	int					m_bLight;
	int					m_Order;
	bool				m_Bill;

	std::vector<KPtr<KMesh>>		m_MeshVec;
	std::vector<KPtr<KMaterial>>	m_MtlVec;
	std::vector<Render_Data>		m_RDVec;


	KPtr<KDevice::RState>	m_RsState;

public:
	RenderOption ROpt;




public:
	size_t Count_Mesh() { return m_MeshVec.size(); }
	size_t Count_Material() { return m_MtlVec.size(); }

	// 이미지 설정을 이제 렌더러의 재질이 해준다.
	KPtr<KMaterial> material(const int& _Index = 0);
	KPtr<KMesh> mesh(const int& _Index = 0);

	void Set_RSState(const wchar_t* _Name);
	int order() { return m_Order; }


	// 직접 해당 메쉬를 넣어주는 방식
	bool Set_Mesh(KPtr<KMesh> _Mesh, const int& _Index = 0);

	// 이 둘은 이름을 리소스 메니저에서 찾아서 넣어주는 방식
	bool Set_Mesh(const wchar_t* _Res, const int& Index = 0);
	bool Set_Material(const wchar_t* _Res, const int& Index = 0);

	void Insert_RenderData(const KUINT& _Mesh, const KUINT& _Vtx, const KUINT& _Sub, const KUINT _Mat);

protected:
	void RenderUpdate();
	
	// 렌더러 통합 및 정리 -> 이렇게 가상으로 만드렁 놓으면
	// 직접 구현해야 하는 렌더러가 구현이 될수 있겠다.
	virtual void RenderBegin(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx);
	virtual void Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data);
	virtual void RenderFin();

protected:
	virtual void Update_Trans(KPtr<Camera> _Camera);
	virtual void Update_TransCB();
	virtual void Update_MtlCB(const KUINT _Index = 0);

	virtual void Update_TexSmp(const KUINT _Index = 0);
	virtual void Update_Material(const KUINT _Index = 0);
	virtual void Update_Mesh(const KUINT _Index = 0);

	// 해당 메쉬 따로 최신화
	virtual void Update_SelectMesh(const KUINT& _Mesh, const KUINT& _Vtx, const KUINT& _Idx);

public:
	virtual bool Init(int _Order = 0);
	

};

