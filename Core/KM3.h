#pragma once
#include "KFBX.h"
#include "Resource.h"



enum MESH_LMODE
{
	LM_FBX,
	LM_KM3,
};

class KM3Bone
{
public:
	// 저장 불러오기가 쉽게 하려고 이렇게 크기를 지정한다.
	wchar_t Name[PATHNUM] = { 0, }; // 리터럴 초기화
	KUINT Depth;
	KUINT Index;
	KMatrix OffsetMX;
	KMatrix BoneMX;
	std::vector<KeyFrame> KFrameVec;
};

class KM3Index
{
public:
	// 크기를 불러오고 잘 저장하기 위함
	KUINT IxSize;
	KUINT IxCnt;
	D3D11_USAGE Usage;
	DXGI_FORMAT Fm;
	char* m_InxBD;

public:
	~KM3Index()
	{
		delete m_InxBD;
	}
};

class KM3Mesh
{
public:
	KUINT VxSize;
	KUINT VxCnt;

	D3D11_USAGE Usage;
	char* m_VertBD;

	std::vector<KM3Index> IxVec;
	std::vector<Material_FbxData> MtlVec;

public:
	~KM3Mesh()
	{
		delete m_VertBD;
	}
};


class KM3Data
{
public:
	std::vector<KM3Mesh*> MeshVec;
	std::vector<Animation_Info> AniVec;
	std::vector<KM3Bone*> BoneVec;

	std::map<std::wstring, KM3Bone*> BoneMap;

	
public:
	void ConvertToKM3(KFBX* _Data);
	void SaveKM3(const wchar_t* _Path);
	void LoadKM3(const wchar_t* _Path);

public:
	~KM3Data()
	{
		for (size_t i = 0; i < BoneVec.size(); i++)
		{
			delete BoneVec[i];
		}

		for (size_t i = 0; i < MeshVec.size(); i++)
		{
			delete MeshVec[i];
		}
	}
};

class MeshContainer : public KResource
{
public:
	KM3Data m_Data;

public:
	bool Load(MESH_LMODE _eMode = MESH_LMODE::LM_KM3);
	void Save(const wchar_t* _Path);
	KM3Bone* Find_Bone(const wchar_t* _Name);


public:
	MeshContainer();
	~MeshContainer();
};