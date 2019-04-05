#include "Renderer_Terrain.h"


#include "KMesh.h"

#include "KRay3D.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "Core_Class.h"
#include "InputManager.h"

#include "WriteStream.h"	
#include "ReadStream.h"	


Renderer_Terrain::Renderer_Terrain() :
	m_BSize(.0f),
	m_Edit(false),
	m_CalMPos(false),
	m_model(nullptr)
{
}


Renderer_Terrain::~Renderer_Terrain()
{
	if (nullptr != m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}
}


void Renderer_Terrain::TerrainTo_MPos(KPtr<Camera> _Camera)
{
	if (false == InputManager::Check_InScr() || false == m_CalMPos)
	{
		Reset_DI();
		return;
	}


	m_CRay = _Camera->Get_Component<KRay3D>();
	if (nullptr == m_CRay)
	{
		return;
	}


	
	bool Ter = Check_Plain();
	if (false == Ter)
	{
		Reset_DI();
		return;
	}


	for (int Z = 0; Z < m_TFD.SizeZ; Z++)
	{
		for (int X = 0; X < m_TFD.SizeX; X++)
		{
			// 버텍스 평행이동
			// 브러쉬 사이즈가 잘 안먹힘 ㅡ.ㅡ;;
			// 브러쉬 사이즈  수정시 UV자체가 바뀜
			// 하지만 마법의 숫자 10.0을 대입하니 정확한 크기에 마우스좌표가 찍힘
			// 지금은 아니지만 나중에 열어보면 아마 다시 써야하는게 맞겟다. 아니면 디버그하든가.
			// 아므ㅜ래도 디버그보단 그때 새마음 새뜻으로 다시 시작하는게 더 빠르겠다.
			// LU
			KVector V0 = m_TempVtx[((Z + 1) * (m_TFD.SizeX + 1)) + X].Pos * m_Trans->scale_world() + m_Trans->pos_world();

			// RU
			KVector V1 = m_TempVtx[((Z + 1) * (m_TFD.SizeX + 1)) + (X + 1)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

			// LD
			KVector V2 = m_TempVtx[((Z) * (m_TFD.SizeX + 1)) + (X)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

			// RD
			KVector V3 = m_TempVtx[((Z) * (m_TFD.SizeX + 1)) + (X + 1)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

			Ter = DirectX::TriangleTests::Intersects(m_CRay->ray_container()->Ori, m_CRay->ray_container()->Dir, V3, V1, V0, m_CRay->ray_container()->Dist);
			if (false == Ter)
			{
				Ter = DirectX::TriangleTests::Intersects(m_CRay->ray_container()->Ori, m_CRay->ray_container()->Dir, V2, V3, V1, m_CRay->ray_container()->Dist);
			}

			if (true == Ter)
			{
				m_MPos = KMath::Calc_ColPoint(m_CRay->ray_container()->Ori, m_CRay->ray_container()->Dir, m_CRay->ray_container()->Dist);
				m_DI.MUv = KMath::PostoUV2_XZ(m_MPos, m_Trans, KVector((float)m_TFD.SizeX, .0f, (float)m_TFD.SizeZ, .0f));
				m_DI.MUv.y = 1.0f - m_DI.MUv.y;

				return;
			}
		}
	}


	return;
}

void Renderer_Terrain::Reset_DI()
{
	m_MPos = .0f;
	m_DI.MUv = KVector2(-1.0f, -1.0f);
}

void Renderer_Terrain::Set_DI()
{
	m_DI.BSize = m_BSize;
	m_DI.PPUV = KVector2(
		1.0f / (m_Trans->scale_local().x * m_TFD.SizeX),
		1.0f / (m_Trans->scale_local().z * m_TFD.SizeZ)
	);
}


void Renderer_Terrain::Ascent_Normal(const float& _Value)
{
	if (false == m_Edit)
	{
		return;
	}

	int X = (int)(((m_MPos.x - m_Trans->scale_local().x * .5f - m_Trans->pos_local().x) / Trans()->scale_local().x));
	int Z = (int)(((m_MPos.z + m_Trans->scale_local().z * .5f - m_Trans->pos_local().z) / Trans()->scale_local().z));


	if (nullptr == mesh())
	{
		return;
	}

	if (X < 0 || Z < 0 || m_TFD.SizeX < X || m_TFD.SizeZ < Z)
	{
		return;
	}

	Cal_Face();
	m_TempVtx[(int)((Z) * (m_TFD.SizeZ + 1) + (X + 1))].Pos.y += 5.0f *DELTATIME * _Value;
	mesh()->Update_Vertex((KUINT)m_TempVtx.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &m_TempVtx[0]);
}

bool Renderer_Terrain::Check_Plain()
{
	bool m_OnTer = false;
	if (nullptr == m_CRay)
	{
		return m_OnTer;
	}


	// 1차 플레인 검열
	// LU
	KVector V0 = m_TempVtx[(m_TFD.SizeZ + 1) * (m_TFD.SizeX) - 1].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// RU
	KVector V1 = m_TempVtx[(m_TFD.SizeZ) * (m_TFD.SizeX) + m_TFD.SizeX].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// LD
	KVector V2 = m_TempVtx[0].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// RD
	KVector V3 = m_TempVtx[m_TFD.SizeX].Pos * m_Trans->scale_world() + m_Trans->pos_world();
	m_OnTer = DirectX::TriangleTests::Intersects(m_CRay->ray_container()->Ori, m_CRay->ray_container()->Dir, V3, V1, V0, m_CRay->ray_container()->Dist);
	if (false == m_OnTer)
	{
		m_OnTer = DirectX::TriangleTests::Intersects(m_CRay->ray_container()->Ori, m_CRay->ray_container()->Dir, V2, V3, V1, m_CRay->ray_container()->Dist);
	}

	return m_OnTer;
}


void Renderer_Terrain::Reset_Terrain(const KUINT& _X, const KUINT& _Z, const wchar_t* _NorMap /*= nullptr*/, const float& _HRatio /*= 1.0f*/)
{
	if (m_TFD.SizeX <= 0 || m_TFD.SizeZ <= 0)
	{
		return;
	}
	if (nullptr == mesh())
	{
		return;
	}

	m_TempVtx.clear();
	m_TempIdx.clear();

	VTX3D TempV;

	// 땅의 정보를 담은 노멀맵을 찾는다.
	KPtr<Texture> NTex = nullptr;
	if (nullptr != _NorMap)
	{
		NTex = ResourceManager<Texture>::Find(_NorMap);
		if (nullptr == NTex)
		{
			BBY;
		}
	}

	for (int z = 0; z < m_TFD.SizeZ + 1; z++)
	{
		for (int x = 0; x < m_TFD.SizeX + 1; x++)
		{
			// 지형을 실제로 높이는 단계
			if (nullptr != NTex)
			{
				int WX = (int)NTex->Width() / m_TFD.SizeX;
				int WZ = (int)NTex->Height() / m_TFD.SizeZ;

				KColor GCol = NTex->GetPixelF(x* WX, (int)NTex->Height() - (z * WZ));

				// 버퍼에 넘길 버텍스 정보 자체를 수정한다.
				TempV.Pos = KVector((float)x, GCol.x * _HRatio, (float)z, 1.0f);
			}
			else
			{
				TempV.Pos = KVector((float)x, .0f, (float)z, 1.0f);
			}

			TempV.Pos.w = 1.0f;
			TempV.Uv = KVector2((float)x, (float)(m_TFD.SizeZ - z));
			TempV.Color = KVector(1.0f, 1.0f, 1.0f, 1.0f);
			TempV.Normal = KVector(.0f, 1.0f, .0f, .0f);
			TempV.Tangent = KVector(1.0f, .0f, .0f, .0f);
			TempV.BNormal = KVector(.0f, .0f, -1.0f, .0f);

			m_TempVtx.push_back(TempV);
		}
	}

	for (int z = 0; z < m_TFD.SizeZ; z++)
	{
		for (int x = 0; x < m_TFD.SizeX; x++)
		{
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z)+(x + 1));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z)+(x));


			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x + 1));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z)+(x + 1));
		}
	}


	mesh()->Update_Vertex((KUINT)m_TempVtx.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &m_TempVtx[0]);
	mesh()->Update_Index((KUINT)m_TempIdx.size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT, IDX32::FM(), &m_TempIdx[0]);
}


void Renderer_Terrain::Create_Terrain(const KUINT& _X, const KUINT& _Z, const wchar_t* _NorMap /*= nullptr*/, const float& _HRatio /*= 1.0f*/)
{
	m_TFD.TexCnt = 0;
	m_TFD.SizeX = _X;
	m_TFD.SizeZ = _Z;

	if (m_TFD.SizeX <= 0 || m_TFD.SizeZ <= 0)
	{
		return;
	}

	KPtr<KMesh> NMesh = new KMesh();

	VTX3D TempV;
	

	// 땅의 정보를 담은 노멀맵을 찾는다.
	KPtr<Texture> NTex = nullptr;
	if (nullptr != _NorMap)
	{
		NTex = ResourceManager<Texture>::Find(_NorMap);
		if (nullptr == NTex)
		{
			BBY;
		}
	}




	for (int z = 0; z < m_TFD.SizeZ + 1; z++)
	{
		for (int x = 0; x < m_TFD.SizeX + 1; x++)
		{
			;
			// 지형을 실제로 높이는 단계
			if (nullptr != NTex)
			{
				// 텍스쳐가 64 X 64 식으롤 반복 적으로 덮혀있을 떄를 대비해
				// 텍스쳐의 전체 버텍스를 맞춰주기 위함이다.
				int WX = (int)NTex->Width() / m_TFD.SizeX;
				int WZ = (int)NTex->Height() / m_TFD.SizeZ;
				

				// Z를 전체 크기에서 빼주는 이유는
				// 현재 반복해서 텍스쳐를 입히는 방식이 인덱스가 증가할 수록
				// 텍스쳐를 입히기 때문
				// 하지만 좌표로 치면 안으로 들어갈수록 - 이기 때문에
				// 부호보정이 필요하다.ㄴ
				KColor GCol = NTex->GetPixelF(x* WX, (int)NTex->Height() - (z * WZ));
				
				// 버퍼에 넘길 버텍스 정보 자체를 수정한다.
				TempV.Pos = KVector((float)x, GCol.x * _HRatio, (float)z, 1.0f);
			}
			else
			{
				TempV.Pos = KVector((float)x, .0f, (float)z, 1.0f);
			}

			TempV.Pos.w = 1.0f;
			TempV.Uv = KVector2((float)x, (float)(m_TFD.SizeZ - z));
			TempV.Color = KVector(1.0f, 1.0f, 1.0f, 1.0f);




			TempV.Normal = KVector(-1.0f, -1.0f, .0f, .0f);
			TempV.Tangent = KVector(1.0f, .0f, .0f, .0f);
			TempV.BNormal = KVector(.0f, .0f, 1.0f, .0f);
			TempV.Normal = KVector(.0f, TempV.Pos.y, .0f, .0f);
			
			m_TempVtx.push_back(TempV);
		}
	}



	for (int z = 0; z < m_TFD.SizeZ; z++)
	{
		for (int x = 0; x < m_TFD.SizeX; x++)
		{
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z) + (x + 1));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z) + (x));


			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x + 1));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z) + (x + 1));
		}
	}

	Cal_Face();
	NMesh->Create_Vertex((KUINT)m_TempVtx.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &m_TempVtx[0]);
	NMesh->Create_Index((KUINT)m_TempIdx.size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT, IDX32::FM(), &m_TempIdx[0]);
	Set_Mesh(NMesh);


	// 기본 세팅 같이 풀어주고 해야함
	Set_Material(L"DEFFERDTERRAINMAT");
	NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	

	// NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	// Set_Material(L"DTESSLEMAT");
}



bool Renderer_Terrain::Cal_Normal()
{
	int index1 = 0;
	int index2 = 0;
	int index3 = 0;
	int index = 0;
	int count = 0;
	float vertex1[3] = { 0.f, 0.f, 0.f };
	float vertex2[3] = { 0.f, 0.f, 0.f };
	float vertex3[3] = { 0.f, 0.f, 0.f };
	float vector1[3] = { 0.f, 0.f, 0.f };
	float vector2[3] = { 0.f, 0.f, 0.f };
	float sum[3] = { 0.f, 0.f, 0.f };
	float length = 0.0f;


	// 정규화되지 않은 법선 벡터를 저장할 임시 배열을 만듭니다.
	KVector* normals = new KVector[(m_TFD.SizeX - 1) * (m_TFD.SizeZ - 1)];
	if (!normals)
	{
		return false;
	}

	// 메쉬의 모든면을 살펴보고 법선을 계산합니다.
	for (int j = 0; j<(m_TFD.SizeZ - 1); j++)
	{
		for (int i = 0; i<(m_TFD.SizeX - 1); i++)
		{
			index1 = (j * m_TFD.SizeX) + i;
			index2 = (j * m_TFD.SizeX) + (i + 1);
			index3 = ((j + 1) * m_TFD.SizeX) + i + 1;

			// 표면에서 세 개의 꼭지점을 가져옵니다.
			vertex1[0] = m_TempVtx[index1].Pos.x;
			vertex1[1] = m_TempVtx[index1].Pos.y;
			vertex1[2] = m_TempVtx[index1].Pos.z;

			vertex2[0] = m_TempVtx[index2].Pos.x;
			vertex2[1] = m_TempVtx[index2].Pos.y;
			vertex2[2] = m_TempVtx[index2].Pos.z;

			vertex3[0] = m_TempVtx[index3].Pos.x;
			vertex3[1] = m_TempVtx[index3].Pos.y;
			vertex3[2] = m_TempVtx[index3].Pos.z;

			// 표면의 두 벡터를 계산합니다.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_TFD.SizeX - 1)) + i;

			// 이 두 법선에 대한 정규화되지 않은 값을 얻기 위해 두 벡터의 외적을 계산합니다.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// 이제 모든 정점을 살펴보고 각면의 평균을 취합니다.     
	// 정점이 닿아 그 정점에 대한 평균 평균값을 얻는다.
	for (int j = 0; j<m_TFD.SizeZ; j++)
	{
		for (int i = 0; i<m_TFD.SizeX; i++)
		{
			// 합계를 초기화합니다.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// 카운트를 초기화합니다.
			count = 0;

			// 왼쪽 아래면.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_TFD.SizeX - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// 오른쪽 아래 면.
			if ((i < (m_TFD.SizeX - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_TFD.SizeX - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// 왼쪽 위 면.
			if (((i - 1) >= 0) && (j < (m_TFD.SizeZ - 1)))
			{
				index = (j * (m_TFD.SizeX - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// 오른쪽 위 면.
			if ((i < (m_TFD.SizeX - 1)) && (j < (m_TFD.SizeZ - 1)))
			{
				index = (j * (m_TFD.SizeX - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// 이 정점에 닿는면의 평균을 취합니다.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// 이 법선의 길이를 계산합니다.
			length = (float)sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// 높이 맵 배열의 정점 위치에 대한 인덱스를 가져옵니다.
			index = (j * m_TFD.SizeX) + i;

			// 이 정점의 최종 공유 법선을 표준화하여 높이 맵 배열에 저장합니다.
			m_TempVtx[index].Normal.x = (sum[0] / length);
			m_TempVtx[index].Normal.y = (sum[1] / length);
			m_TempVtx[index].Normal.z = (sum[2] / length);
		}
	}

	// 임시 법선을 해제합니다.
	delete[] normals;
	normals = 0;

	return true;
}

void Renderer_Terrain::Cal_Face()
{
	Cal_Normal();
	Cal_Model();

	VTX3D vertex1, vertex2, vertex3;
	KVector tangent, binormal;


	// 지형 모델에서면의 수를 계산합니다.
	int faceCount = (int)m_TempVtx.size() / 3;

	// 모델 데이터에 대한 인덱스를 초기화합니다.
	int index = 0;

	// 모든면을 살펴보고 접선, 비공식 및 법선 벡터를 계산합니다.
	for (int i = 0; i<faceCount; i++)
	{
		// 지형 모델에서이면에 대한 세 개의 정점을 가져옵니다.
		vertex1.Pos.x = m_model[index].x;
		vertex1.Pos.y = m_model[index].y;
		vertex1.Pos.z = m_model[index].z;
		vertex1.Uv.x = m_model[index].tu;
		vertex1.Uv.y = m_model[index].tv;
		vertex1.Normal.x = m_model[index].nx;
		vertex1.Normal.y = m_model[index].ny;
		vertex1.Normal.z = m_model[index].nz;
		index++;

		vertex2.Pos.x = m_model[index].x;
		vertex2.Pos.y = m_model[index].y;
		vertex2.Pos.z = m_model[index].z;
		vertex2.Uv.x = m_model[index].tu;
		vertex2.Uv.y = m_model[index].tv;
		vertex2.Normal.x = m_model[index].nx;
		vertex2.Normal.y = m_model[index].ny;
		vertex2.Normal.z = m_model[index].nz;
		index++;

		vertex3.Pos.x = m_model[index].x;
		vertex3.Pos.y = m_model[index].y;
		vertex3.Pos.z = m_model[index].z;
		vertex3.Uv.x = m_model[index].tu;
		vertex3.Uv.y = m_model[index].tv;
		vertex3.Normal.x = m_model[index].nx;
		vertex3.Normal.y = m_model[index].ny;
		vertex3.Normal.z = m_model[index].nz;
		index++;

		// 그 얼굴의 탄젠트와 바이 노멀을 계산합니다.
		Cal_Tangent(vertex1, vertex2, vertex3, tangent, binormal);

		// 이면에 대한 접선과 binormal을 모델 구조에 다시 저장하십시오.
		m_TempVtx[index - 1].Tangent.x = tangent.x;
		m_TempVtx[index - 1].Tangent.y = tangent.y;
		m_TempVtx[index - 1].Tangent.z = tangent.z;
		m_TempVtx[index - 1].BNormal.x = binormal.x;
		m_TempVtx[index - 1].BNormal.y = binormal.y;
		m_TempVtx[index - 1].BNormal.z = binormal.z;

		m_TempVtx[index - 2].Tangent.x = tangent.x;
		m_TempVtx[index - 2].Tangent.y = tangent.y;
		m_TempVtx[index - 2].Tangent.z = tangent.z;
		m_TempVtx[index - 2].BNormal.x = binormal.x;
		m_TempVtx[index - 2].BNormal.y = binormal.y;
		m_TempVtx[index - 2].BNormal.z = binormal.z;

		m_TempVtx[index - 3].Tangent.x = tangent.x;
		m_TempVtx[index - 3].Tangent.y = tangent.y;
		m_TempVtx[index - 3].Tangent.z = tangent.z;
		m_TempVtx[index - 3].BNormal.x = binormal.x;
		m_TempVtx[index - 3].BNormal.y = binormal.y;
		m_TempVtx[index - 3].BNormal.z = binormal.z;
	}
}

bool Renderer_Terrain::Cal_Model()
{
	if (nullptr != m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}

	int index1 = 0;
	int index2 = 0;
	int index3 = 0;
	int index4 = 0;

	// 모델의 정점 수를 설정합니다.
	int m_vertexCount = (m_TFD.SizeX - 1) * (m_TFD.SizeZ - 1) * 6;

	// 지형 모델 배열을 만듭니다.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// 텍스처의 두 번째 세트 (알파 맵)의 증가 크기를 설정합니다.
	float incrementSize = 1.0f / 31.0f;

	// 텍스처 증가를 초기화합니다.
	float tu2Left = 0.0f;
	float tu2Right = incrementSize;
	float tv2Bottom = 1.0f;
	float tv2Top = 1.0f - incrementSize;

	// 높이 맵 지형 데이터로 지형 모델을 로드합니다.
	int index = 0;

	for (int j = 0; j<(m_TFD.SizeZ - 1); j++)
	{
		for (int i = 0; i<(m_TFD.SizeX - 1); i++)
		{
			index1 = (m_TFD.SizeZ * j) + i;          // 왼쪽 아래.
			index2 = (m_TFD.SizeZ * j) + (i + 1);      // 오른쪽 아래.
			index3 = (m_TFD.SizeZ * (j + 1)) + i;      // 왼쪽 위.
			index4 = (m_TFD.SizeZ * (j + 1)) + (i + 1);  // 오른쪽 위.

			// 왼쪽 위.
			m_model[index].x = m_TempVtx[index3].Pos.x;
			m_model[index].y = m_TempVtx[index3].Pos.y;
			m_model[index].z = m_TempVtx[index3].Pos.z;
			m_model[index].nx = m_TempVtx[index3].Normal.x;
			m_model[index].ny = m_TempVtx[index3].Normal.y;
			m_model[index].nz = m_TempVtx[index3].Normal.z;
			m_model[index].tu = 0.0f;
			m_model[index].tv = 0.0f;
			m_model[index].tu2 = tu2Left;
			m_model[index].tv2 = tv2Top;
			index++;

			// 오른쪽 위.					
			m_model[index].x = m_TempVtx[index4].Pos.x;
			m_model[index].y = m_TempVtx[index4].Pos.y;
			m_model[index].z = m_TempVtx[index4].Pos.z;
			m_model[index].nx = m_TempVtx[index4].Normal.x;
			m_model[index].ny = m_TempVtx[index4].Normal.y;
			m_model[index].nz = m_TempVtx[index4].Normal.z;
			m_model[index].tu = 1.0f;
			m_model[index].tv = 0.0f;
			m_model[index].tu2 = tu2Left;
			m_model[index].tv2 = tv2Top;
			index++;

			// 왼쪽 아래.
			m_model[index].x = m_TempVtx[index1].Pos.x;
			m_model[index].y = m_TempVtx[index1].Pos.y;
			m_model[index].z = m_TempVtx[index1].Pos.z;
			m_model[index].nx = m_TempVtx[index1].Normal.x;
			m_model[index].ny = m_TempVtx[index1].Normal.y;
			m_model[index].nz = m_TempVtx[index1].Normal.z;
			m_model[index].tu = 0.0f;
			m_model[index].tv = 1.0f;
			m_model[index].tu2 = tu2Left;
			m_model[index].tv2 = tv2Top;
			index++;

			// 왼쪽 아래.
			m_model[index].x = m_TempVtx[index1].Pos.x;
			m_model[index].y = m_TempVtx[index1].Pos.y;
			m_model[index].z = m_TempVtx[index1].Pos.z;
			m_model[index].nx = m_TempVtx[index1].Normal.x;
			m_model[index].ny = m_TempVtx[index1].Normal.y;
			m_model[index].nz = m_TempVtx[index1].Normal.z;
			m_model[index].tu = 0.0f;
			m_model[index].tv = 1.0f;
			m_model[index].tu2 = tu2Left;
			m_model[index].tv2 = tv2Top;
			index++;

			// 오른쪽 위.
			m_model[index].x = m_TempVtx[index4].Pos.x;
			m_model[index].y = m_TempVtx[index4].Pos.y;
			m_model[index].z = m_TempVtx[index4].Pos.z;
			m_model[index].nx = m_TempVtx[index4].Normal.x;
			m_model[index].ny = m_TempVtx[index4].Normal.y;
			m_model[index].nz = m_TempVtx[index4].Normal.z;
			m_model[index].tu = 1.0f;
			m_model[index].tv = 0.0f;
			m_model[index].tu2 = tu2Left;
			m_model[index].tv2 = tv2Top;
			index++;

			// 오른쪽 아래.
			m_model[index].x = m_TempVtx[index2].Pos.x;
			m_model[index].y = m_TempVtx[index2].Pos.y;
			m_model[index].z = m_TempVtx[index2].Pos.z;
			m_model[index].nx = m_TempVtx[index2].Normal.x;
			m_model[index].ny = m_TempVtx[index2].Normal.y;
			m_model[index].nz = m_TempVtx[index2].Normal.z;
			m_model[index].tu = 1.0f;
			m_model[index].tv = 1.0f;
			m_model[index].tu2 = tu2Left;
			m_model[index].tv2 = tv2Top;
			index++;

			// 알파 맵에 대해 tu 텍스처 좌표를 증가시킵니다.
			tu2Left += incrementSize;
			tu2Right += incrementSize;
		}

		// 알파 맵에 대해 tu 텍스처 좌표를 재설정합니다.
		tu2Left = 0.0f;
		tu2Right = incrementSize;

		// 알파 맵의 tv 텍스처 좌표를 증가시킵니다.
		tv2Top -= incrementSize;
		tv2Bottom -= incrementSize;
	}

	return true;
}

bool Renderer_Terrain::Cal_Tangent(
	const VTX3D& vertex1
	, const VTX3D& vertex2
	, const VTX3D& vertex3
	, KVector& tangent
	, KVector& binormal)
{
	float vector1[3] = { 0.0f, 0.0f, 0.0f };
	float vector2[3] = { 0.0f, 0.0f, 0.0f };
	float tuVector[2] = { 0.0f, 0.0f };
	float tvVector[2] = { 0.0f, 0.0f };


	// 이면의 두 벡터를 계산합니다.
	vector1[0] = vertex2.Pos.x - vertex1.Pos.x;
	vector1[1] = vertex2.Pos.y - vertex1.Pos.y;
	vector1[2] = vertex2.Pos.z - vertex1.Pos.z;

	vector2[0] = vertex3.Pos.x - vertex1.Pos.x;
	vector2[1] = vertex3.Pos.y - vertex1.Pos.y;
	vector2[2] = vertex3.Pos.z - vertex1.Pos.z;

	// tu 및 tv 텍스처 공간 벡터를 계산합니다.
	tuVector[0] = (vertex2.Uv.x - vertex1.Uv.x);
	tvVector[0] = (vertex2.Uv.y - vertex1.Uv.y);

	tuVector[1] = (vertex3.Uv.x - vertex1.Uv.x);
	tvVector[1] = (vertex3.Uv.y - vertex1.Uv.y);

	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// 교차 곱을 계산하고 계수로 곱하여 접선과 비 구식을 얻습니다.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// 이 법선의 길이를 계산합니다.
	float length = (float)sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// 법선을 표준화 한 다음 저장합니다.
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// 이 법선의 길이를 계산합니다.
	length = (float)sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// 법선을 표준화 한 다음 저장합니다.
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return true;
}

void Renderer_Terrain::RenderBegin(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx)
{
	TerrainTo_MPos(_Camera);
	Set_DI();

	m_DI.OnClick = .0f;

	if (true == InputManager::Check_InScr())
	{
		if (true == KEY_PRESS(L"LB"))
		{
			Ascent_Normal(1.0f);
			m_DI.OnClick = 1.0f;
		}

		if (true == KEY_PRESS(L"RB"))
		{
			Ascent_Normal(-1.0f);
			m_DI.OnClick = -1.0f;
		}
	}

	material()->PShader()->SettingCB<TERRAIN_FD>(L"TERRAIN_FD", m_TFD);
	material()->PShader()->SettingCB<DRAW_INFO>(L"DRAW_INFO", m_DI);
}

void Renderer_Terrain::base_texture(const wchar_t* _MTex)
{
	// 섞여질 텍스트 어레이 타입 -> 멀티텍스트, 텍스트 배열들은,,,
	// 8번째 이후 부터 입력된다.
	material()->Insert_TexData(TEX_TYPE::TEX_MULTI, m_TFD.TexCnt + 8, _MTex, m_TFD.TexCnt + 8, L"TerrainSmp");
	++m_TFD.TexCnt;
}


void Renderer_Terrain::Insert_CoverTex(const wchar_t* _MTex, const wchar_t* _Cover)
{
	if (m_TFD.TexCnt <= 0)
	{
		BBY;
	}

	// 기본형은 컬러로 맨처음 쫙 깔아주는 과정
	material()->Insert_TexData(TEX_TYPE::TEX_COLOR, m_TFD.FloorCnt, _Cover, m_TFD.FloorCnt);
	material()->Insert_TexData(TEX_TYPE::TEX_MULTI, m_TFD.TexCnt + 8, _MTex, m_TFD.TexCnt + 8, L"TerrainSmp");

	++m_TFD.FloorCnt;
	++m_TFD.TexCnt;
}



// 여기서 산출된 좌표로 캐릭ㅌ가 움직일 것이다.
// 따라서 모든 유닛은 이 지형을 알아야하며
// 매번 해당 좌표에 위치했을 시 그 높이를 산출해야한다.
float Renderer_Terrain::Y_Terrain(const KVector& _Pos)
{
	int X = (int)((_Pos.x - m_Trans->pos_local().x) / Trans()->scale_local().x);
	int Z = (int)((_Pos.z - m_Trans->pos_local().z) / Trans()->scale_local().z);
	
	if (X < 0 || Z < 0 || m_TFD.SizeX < X || m_TFD.SizeZ < Z)
	{
		return .0f;
	}
	
	

	// 좌상단
	KVector V0 = m_TempVtx[((Z + 1) * (m_TFD.SizeX + 1)) + X].Pos  * m_Trans->scale_world() + m_Trans->pos_world();

	// 우상단
	KVector V1 = m_TempVtx[((Z + 1) * (m_TFD.SizeX + 1)) + (X + 1)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// 좌하단
	KVector V2 = m_TempVtx[((Z) * (m_TFD.SizeX + 1)) + (X)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// 우하단
	KVector V3 = m_TempVtx[((Z) * (m_TFD.SizeX + 1)) + (X + 1)].Pos * m_Trans->scale_world() + m_Trans->pos_world();
	
	// 검출될 y좌표
	float F0 = .0f;
	float F1 = .0f;

	KVector ObjPos = KVector(
		_Pos.x 
		, .0f
		, _Pos.z );

	DirectX::TriangleTests::Intersects(ObjPos, KVector::Up, V0, V2, V3, F0);
	DirectX::TriangleTests::Intersects(ObjPos, KVector::Up, V0, V3, V1, F1);
	
	if (0 != F0|| 0 != F1)
	{
		if (0 != F0)
		{
			return F0 * Trans()->scale_local().y * .5f;
		}
		else if (0 != F1)
		{
			return F1 * Trans()->scale_local().y * .5f;
		}
	}

	return 0.0f;
}

void Renderer_Terrain::Clear()
{
	for (size_t i = 0; i < m_TempVtx.size(); i++)
	{
		m_TempVtx[i].Pos.y = .0f;
		m_TempVtx[i].Normal.y = .0f;
		m_TempVtx[i].Tangent.y = .0f;
	}
	mesh()->Update_Vertex((KUINT)m_TempVtx.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &m_TempVtx[0]);
}

void Renderer_Terrain::Save()
{
	std::wstring Path = PathManager::Find_ForderPath(L"KTD");
	Path += name();
	Path += L".KTD";

	WriteStream WS = WriteStream(Path.c_str());


	int VT = (int)m_TempVtx.size();
	WS.Write(VT);
	for (int i = 0; i < VT; i++)
	{
		WS.Write(m_TempVtx[i].BNormal);
		WS.Write(m_TempVtx[i].Color);
		WS.Write(m_TempVtx[i].Normal);
		WS.Write(m_TempVtx[i].Pos);
		WS.Write(m_TempVtx[i].Tangent);
		WS.Write(m_TempVtx[i].Uv);
	}

	int IT = (int)m_TempIdx.size();
	WS.Write(IT);
	for (int i = 0; i < IT; i++)
	{
		WS.Write(m_TempIdx[i]);
	}
}

void Renderer_Terrain::Load(const wchar_t* _Name)
{
	if (nullptr == mesh())
	{
		Set_Mesh(new KMesh());
	}

	m_TempVtx.clear();
	m_TempIdx.clear();


	std::wstring Path = PathManager::Find_ForderPath(L"KTD");
	Path += _Name;
	Path += L".KTD";

	ReadStream RS = ReadStream(Path.c_str());


	int VT;
	RS.Read(VT);
	for (int i = 0; i < VT; i++)
	{
		VTX3D TT;

		RS.Read(TT.BNormal);
		RS.Read(TT.Color);
		RS.Read(TT.Normal);
		RS.Read(TT.Pos);
		RS.Read(TT.Tangent);
		RS.Read(TT.Uv);

		m_TempVtx.push_back(TT);
	}

	int IT = (int)m_TempIdx.size();
	RS.Read(IT);
	for (int i = 0; i < IT; i++)
	{
		UINT TT;
		RS.Read(TT);
		m_TempIdx.push_back(TT);
	}
/*
	for (size_t i = 0; i < m_TempIdx.size(); i++)
	{
		if (i % 3 == 0)
		{
			int Temp = m_TempIdx[i + 1];
			m_TempIdx[i + 1] = m_TempIdx[i];
			m_TempIdx[i] = Temp;
		}
	}
*/
	mesh()->Update_Vertex((KUINT)m_TempVtx.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &m_TempVtx[0]);
	mesh()->Update_Index((KUINT)m_TempIdx.size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT, IDX32::FM(), &m_TempIdx[0]);
}



float Renderer_Terrain::TER_Left()
{
	return one()->Trans()->pos_world().x;
}

float Renderer_Terrain::TER_Right()
{
	return one()->Trans()->pos_world().x + m_TFD.SizeX * one()->Trans()->scale_world().x;
}

float Renderer_Terrain::TER_Up()
{
	return one()->Trans()->pos_world().z + m_TFD.SizeZ * one()->Trans()->scale_world().z;
}

float Renderer_Terrain::TER_Down()
{
	return one()->Trans()->pos_world().z;
}