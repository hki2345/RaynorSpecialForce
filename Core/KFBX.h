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
	// ���������� �����ϱ� ���� - ��ü ������ �÷����� ����
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

// ���� ���� ����ġ�� ��� Ŭ���� 
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

	// ����ġ ���� - �ִϸ��̼� ��ȯ �� ���� Ȥ�� ���� �ִϸ��̼��� �󸶳� ���� ���̳�
	KVector m_Weights;
	// ��Ű�� ���� - 
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

	// ��Ű�� ������ ���� �ε����� ������ - ����
	// �޽��� ���Ը� �ּ� �̰� �󸶸�ŭ ������ ���̳Ŀ� ���� ����ġ��
	// ����ϰ� �ȴ�. - 2D �� �ִϸ��̼� �����ϸ� ����

	// ������ �ε����� �ش� �ε����� ����ġ�� ������ Ŭ������ �����Ѵ�.
	std::vector<std::vector<WI>> WIVec;


	// ������� �������� �ٸ�����.
	// ������� Ȯ���ϴ� �����̶��, ��������� ���� �ڴ� ��
	void Set_VertexCount(const KUINT& _Count)
	{
		VertVec.resize(_Count);
		WIVec.resize(_Count);
	}
};

class Animation_Info
{
public:
	// �ؿ� �ִϸ��̼��� �ð������� �ɰ��� �����ϰ� �Ǵµ�
	// �׶� ����� �ε��� - �������� ����
	int Index;
	wchar_t Name[PATHNUM] = { 0, };
	FbxTime Stime;
	FbxTime Etime;
	FbxLongLong Length_Time;

	// �׳� longlong ���� �ִ� �����̶� ¥�Ǥ� ������� ����� ���ǹ���
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
	// Bone - TRee ������ �Ǿ��ִ�.
	std::vector<KBone*> Bone_Vec;
	std::multimap<std::wstring, KBone*> Bone_Map;

	// Ani
	std::map<std::wstring, Animation_Info*> Ani_Map;
	
	// �ִϸ��̼� �����(�ʱⰪ)�� ���� ������ �����ϱ� ����
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

	// �ݻ� ��Ʈ���� - ������ �ִϸ��̼ǿ�
	// �� ����� ���ϰԵǸ� OFfset�� ���´�.
private:
	static FbxAMatrix MX_Reflect;

public:
	FbxManager* m_pManager;
	FbxScene* m_pScene;
	KFBX* m_pNewFbx;
	KUINT	m_BoneCnt;

public:
	// �⺻ ���� - �������� Ȯ�� ������ �������� ���� Ȥ�� ����
	void Load_FBX(const wchar_t* _Path);

private:
	void Check_AllBoneCnt(FbxNode* _pNode);

	// �� ������
	void Load_Bone(FbxNode* _pNode, KUINT _Depth, KBone* _pParent);
	
	// �ִϸ��̼� �� �ﰢȭ - �ﰢȭ�� �Ǿ�� �׷�������
	void Check_Animation();
	void Triangulate( FbxNode* _pNode);
	
	// ���� ����
	void Set_Material(Mesh_FbxData* _pMD, FbxSurfaceMaterial* _pNode);
	void Set_Mesh(Mesh_FbxData* _pMD, FbxMesh* _pNode);

	// �޽�
	void Set_MeshData(FbxNode* _pNode);
	void Set_Normal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_Tangent(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_BNormal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_Uv(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);

	// ������ ���� �ִϸ��̼� ���� ����
	void Set_AniData(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData);
	void Set_WeightIndices(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData);
	void Set_OffSetMatrix(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData);
	void Set_FrameMatrix(FbxNode* _pNode, FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData);
	void Check_WI(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData);

	// ������ ����� -> ���� ���� �ű⿡ � ���� ������ ����
	KVector Get_MaterialColor(FbxSurfaceMaterial* _pFbxMatData,
		const char* _pMtrlColorName,
		const char* _pMtrlFactorName);

	// ����ϰ� �ִ� �ؽ��ĸ� ������
	std::wstring Get_MaterialTexName(FbxSurfaceMaterial* _pFbxMatData,
		const char* _pMtrlTexName);


	// �̰� ���ܷ� ��Ʈ������ �ڵ����� ũ���� ���� �༭ ������ִ� �Լ�
	// ���⿡�� ���� ����� ������ ���� �ǰڴ�.
	FbxAMatrix Get_FbxMatrix(FbxNode* _pNode);
};