#include "Renderer_Effect.h"
#include "Texture.h"
#include "Core_Class.h"


Renderer_Effect::Renderer_Effect() :
	m_UpdateTime(.0f),
	CurLive_Cnt(0),
	Live_Cnt(0),
	Time(.1f)
{
	ROpt.Render_DT = RENDER_DATATYPE::RDT_ONE;
}


Renderer_Effect::~Renderer_Effect()
{
}

void Renderer_Effect::Update()
{
	Time -= DELTATIME;

	if (Time <= .0f)
	{
		if (Live_Cnt < m_MaxEFCnt)
		{
			m_EFDVec[CurLive_Cnt].Value.x = KMath::random_f(1.0f, 10.0f);
			m_EFBVec[CurLive_Cnt].LiveTime = m_EFDVec[CurLive_Cnt].Value.x;

			++CurLive_Cnt;
			Time = .1f;

			if (CurLive_Cnt >= m_MaxEFCnt)
			{
				CurLive_Cnt = 0;
			}
		}
	}


	KUINT Cnt = 0;

	for (size_t i = 0; i < m_EFDVec.size(); i++)
	{
		if (0 < m_EFDVec[i].Value.x)
		{
			m_EFDVec[i].Value.x -= DELTATIME;
			m_EFDVec[i].Value.w -= DELTATIME / m_EFBVec[i].LiveTime;
			m_EFDVec[i].Pos += m_EFBVec[i].Dir * DELTATIME * m_EFBVec[i].Speed;

			m_EFLDVec[Cnt] = m_EFDVec[i];
			++Cnt;
		}
	}

	Live_Cnt = Cnt;
	m_DataBuf->Update_Data((KUINT)Live_Cnt * (KUINT)sizeof(Effect_Data), &m_EFLDVec[0]);
}


bool Renderer_Effect::Init(const KUINT& _DotCnt, const int& _Order/* = 0*/)
{
	m_MaxEFCnt = _DotCnt;

	// 스트림 버퍼 만드는 구간
	m_EFDVec.resize(_DotCnt);
	m_EFBVec.resize(_DotCnt);
	m_EFLDVec.resize(_DotCnt);


	for (size_t i = 0; i < _DotCnt; i++)
	{
		KVector RDir;

		m_EFDVec[i].Pos = KVector::Zero;
		m_EFDVec[i].Value = KVector(.0f, .0f, .0f, 1.0f);

		m_EFBVec[i].Dir = KVector(
			KMath::random_f(-.5f, .5f),
			KMath::random_f(.5f, 1.f), 
			.0f, .0f);
		m_EFBVec[i].Dir.NormalizeVec3();
		m_EFBVec[i].Speed = KMath::random_f(.0f, 10.0f);
	}


	m_DataBuf = new KStreamBuffer();
	if (false == m_DataBuf->Create(_DotCnt * sizeof(Effect_Data), &m_EFDVec[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE::D3D11_USAGE_DYNAMIC))
	{
		BBY;
		return false;
	}

	m_SOBuffer = new KStreamBuffer();
	if (false == m_SOBuffer->Create(_DotCnt * sizeof(Effect_Data), nullptr, D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT))
	{
		BBY;
		return false;
	}

	m_SMtl = ResourceManager<KMaterial>::Find(L"EFFECTFILTERMTL");
	m_RMtl = ResourceManager<KMaterial>::Find(L"EFFECTRENDERMTL");
	Renderer::Init();

	return true;
}

// 해당 효과 랜더는 일일이 직접 구현이 들어가는 부분이다.
void Renderer_Effect::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	std::wstring PrevDs = Core_Class::MainDevice().ds_name();

	Core_Class::MainDevice().Set_DSS(L"BASIC");
	Core_Class::Context()->RSSetState(nullptr);
	// 점들을 넘기는 거다 삼각형이 아니고
	Core_Class::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_SMtl->Update();


	KUINT TSize = sizeof(Effect_Data);
	KUINT TOffset = 0;
	ID3D11Buffer* pPosBuf = m_DataBuf->buffer();
	Core_Class::Context()->IASetVertexBuffers(0, 1, &pPosBuf, &TSize, &TOffset);



	// 만들어진 스트림 버퍼를 어떻게 최신화 하는 지에 대한 구간
	// 단 만들어진 버퍼는 우리가 인덱스와 버텍스를 일ㅇ일이 지정해 넣어주는 것이 아닌
	// 자동으로 넣우제 된다.
	// 였는데 데이터 초기화는 auto 안먹드라
	// Core_Class::Context()->DrawAuto();
	m_SOBuffer->Update();
	Core_Class::Context()->Draw((KUINT)Live_Cnt, 0);
	
	
	// 여기까지가 스트림에 그리는 과정
	m_SOBuffer->Reset();


	
	Core_Class::MainDevice().Set_RS(L"SNONE");
	// Core_Class::MainDevice().ResetContext();

	// 이건 이제 다시 그리는 과정

	ID3D11Buffer* pOutBuf = m_SOBuffer->buffer();
	Core_Class::Context()->IASetVertexBuffers(0, 1, &pOutBuf, &TSize, &TOffset);

	m_RMtl->Update_Tex();
	m_RMtl->Update_Sam();
	m_RMtl->Update();


	Core_Class::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Core_Class::Context()->DrawAuto();

	Core_Class::MainDevice().Set_RS(L"SBACK");
	Core_Class::MainDevice().Set_DSS(PrevDs.c_str());

	m_RMtl->Reset_Tex();
}


KPtr<KMaterial> Renderer_Effect::render_material()
{
	return m_RMtl;
}