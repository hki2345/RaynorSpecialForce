#pragma once
#include "KMacro.h"
#include "DXContainer.h"
#include <fbxsdk.h>
#include <vector>
#include <map>
#include <string>


#if X64
#if _DEBUG
#pragma comment (lib, "libfbxsdk-md_D64.lib")
#else if NDEBUG
#pragma comment (lib, "libfbxsdk-md_R64.lib")
#endif
#else if WIN32
#if _DEBUG
#pragma comment (lib, "libfbxsdk-md_D86.lib")
#else if NDEBUG
#pragma comment (lib, "libfbxsdk-md_R86.lib")
#endif
#endif // DEBUG


class KeyFrame
{
public:
	// 안정적으로 저장하기 위해 - 자체 프레임 플롯으로 저장
	// FbxAMatrix	MX_Frame;
	KVector Scale;
	KVector Rotate;
	KVector Pos;
	
	double		dTime;
};

class KBone
{
public:
	wchar_t Name[PATHNUM];
	KUINT Depth;
	KUINT Index;
	KBone* m_pPBone;
	KMatrix Offset_KMX;
	KMatrix Bone_KMX;
	FbxAMatrix Offset_FMX;
	FbxAMatrix Bone_FMX;
	std::vector<KeyFrame> KFVec;
};

class Matrial_Info
{
public:
	Matrial_Info() : Diff(KVector::One), Spec(KVector::One), Ambi(KVector::One), Emiv(KVector::One)
	{

	}

public:
	KVector Diff;
	KVector Spec;
	KVector Ambi;
	KVector Emiv;
};


class Material_FbxData
{
public:
	Matrial_Info Info;
	wchar_t Name[PATHNUM];
	wchar_t Diff[PATHNUM];
	wchar_t Bump[PATHNUM];
	wchar_t Spec[PATHNUM];
	wchar_t Emiv[PATHNUM];
};

// 본에 따른 가중치를 담는 클래스 
class WI
{
public:
	int BoneIdx;
	int Indices;
	double dWeight;
};

class Vertex_FbxData
{
public:
	KVector m_Pos;
	KVector2 m_UV;
	KVector m_Color;
	KVector m_Normal;
	KVector m_Tangent;
	KVector m_Binormal;

	// 가중치 정보 - 애니메이션 전환 간 이전 혹은 다음 애니메이션을 얼마나 섞을 것이냐
	KVector m_Weights;
	// 스키닝 정보 - 
	KVector m_Indices;

	Vertex_FbxData() : m_Color(KColor::White)
	{

	}
};

class Mesh_FbxData
{
public:
	~Mesh_FbxData()
	{
	}

public:
	wchar_t Name[PATHNUM];
	std::vector<Vertex_FbxData> VertVec;
	std::vector<std::vector<KUINT>> IdxVec;
	std::vector<Material_FbxData> m_MtlVec;

	bool m_bAni;

	// 스키닝 정보에 대한 인덱스의 복수형 - 지수
	// 메쉬도 무게를 둬서 이게 얼마만큼 움직일 것이냐에 대한 가중치를
	// 계산하게 된다. - 2D 본 애니메이션 생각하면 쉬움

	// 마지막 인덱스와 해당 인덱스의 가중치를 가지는 클래스도 관리한다.
	std::vector<std::vector<WI>> WIVec;


	// 리저브와 리사이즌 다르ㄴ다.
	// 리저브는 확보하는 개념이라면, 리사이즈는 때려 박는 식
	void Set_VertexCount(const KUINT& _Count)
	{
		VertVec.resize(_Count);
		WIVec.resize(_Count);
	}
};

class Animation_Info
{
public:
	// 밑에 애니메이션을 시간단위로 쪼개서 저장하게 되는데
	// 그때 저장될 인덱스 - 구분짓기 위함
	int Index;
	wchar_t Name[PATHNUM] = { 0, };
	FbxTime Stime;
	FbxTime Etime;
	FbxLongLong Length_Time;

	// 그냥 longlong 쓰고 있는 파일이라 짜피ㅏ 저장과의 관계는 무의미함
	FbxTime::EMode eMode;
};

class KFBX
{
public:
	~KFBX()
	{
		for (size_t i = 0; i < Bone_Vec.size(); i++)
		{
			delete Bone_Vec[i];
		}
		for (size_t i = 0; i < MeshData_Vec.size(); i++)
		{
			delete MeshData_Vec[i];
		}
	}

public:
	// Bone - TRee 구조로 되어있다.
	std::vector<KBone*> Bone_Vec;
	std::multimap<std::wstring, KBone*> Bone_Map;

	// Ani
	std::map<std::wstring, Animation_Info*> Ani_Map;
	
	// 애니메이션 서브셋(초기값)에 대한 정보도 저장하기 위함
	std::vector<Animation_Info> Ani_Vec;
	std::vector<Mesh_FbxData*> MeshData_Vec;

public:
	KBone* Find_Bone(const wchar_t* _Name);
};


class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

public:
	static void Init_FBXLoader();
	static KMatrix FMXtoKMX(const FbxMatrix& _Mat);
	static FbxMatrix KMXtoFMX(const KMatrix& _Mat);
	static KVector FVectoKVec(const FbxVector4& _Value);
	static KVector FQTtoKVec(const FbxQuaternion& _Value);

	// 반사 메트릭스 - 변형된 애니메이션에
	// 이 행렬을 곱하게되면 OFfset이 나온다.
private:
	static FbxAMatrix MX_Reflect;

public:
	FbxManager* m_pManager;
	FbxScene* m_pScene;
	KFBX* m_pNewFbx;
	KUINT	m_BoneCnt;

public:
	// 기본 세팅 - 데이터의 확인 유무와 정상적인 파일 혹은 보정
	void Load_FBX(const wchar_t* _Path);

private:
	void Check_AllBoneCnt(FbxNode* _pNode);

	// 본 얻어오기
	void Load_Bone(FbxNode* _pNode, KUINT _Depth, KBone* _pParent);
	
	// 애니메이션 및 삼각화 - 삼각화가 되어야 그려지겠지
	void Check_Animation();
	void Triangulate( FbxNode* _pNode);
	
	// 재질 정보
	void Set_Material(Mesh_FbxData* _pMD, FbxSurfaceMaterial* _pNode);
	void Set_Mesh(Mesh_FbxData* _pMD, FbxMesh* _pNode);

	// 메쉬
	void Set_MeshData(FbxNode* _pNode);
	void Set_Normal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_Tangent(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_BNormal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_Uv(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);

	// 정점에 대한 애니메이션 관련 정보
	void Set_AniData(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData);
	void Set_WeightIndices(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData);
	void Set_OffSetMatrix(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData);
	void Set_FrameMatrix(FbxNode* _pNode, FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData);
	void Check_WI(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData);

	// 재질의 색상과 -> 고유 색깔 거기에 어떤 색을 곱할지 결정
	KVector Get_MaterialColor(FbxSurfaceMaterial* _pFbxMatData,
		const char* _pMtrlColorName,
		const char* _pMtrlFactorName);

	// 사용하고 있는 텍스쳐를 가져옴
	std::wstring Get_MaterialTexName(FbxSurfaceMaterial* _pFbxMatData,
		const char* _pMtrlTexName);


	// 이건 번외로 메트릭스를 자동으로 크자이 곱해 줘서 계산해주는 함수
	// 여기에서 나온 행렬을 보정해 쓰면 되겠다.
	FbxAMatrix Get_FbxMatrix(FbxNode* _pNode);
};