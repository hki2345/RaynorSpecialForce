#include "KM3.h"
#include "DXContainer_DE.h"
#include "WriteStream.h"
#include "ReadStream.h"

#include "KThread.h"

/******************** KM3 Data ********************/
void KM3Data::ConvertToKM3(KFBX* _Data)
{
	for (size_t MeshCnt = 0; MeshCnt < _Data->MeshData_Vec.size(); MeshCnt++)
	{
		KM3Mesh* pKMesh = new KM3Mesh();
		Mesh_FbxData* pMD = _Data->MeshData_Vec[MeshCnt];

		pKMesh->VxCnt = (KUINT)pMD->VertVec.size();
		pKMesh->VxSize = sizeof(Vertex_FbxData);
		pKMesh->Usage = D3D11_USAGE_DYNAMIC;
		pKMesh->m_VertBD = new char[pKMesh->VxCnt * pKMesh->VxSize];
		memcpy_s(pKMesh->m_VertBD, pKMesh->VxCnt * pKMesh->VxSize, 
			&pMD->VertVec[0], pKMesh->VxCnt * pKMesh->VxSize);

		pKMesh->IxVec.resize(pMD->IdxVec.size());

		for (size_t IdxCnt = 0; IdxCnt < pMD->IdxVec.size(); IdxCnt++)
		{
			pKMesh->IxVec[IdxCnt].IxCnt = (KUINT)pMD->IdxVec[IdxCnt].size();
			pKMesh->IxVec[IdxCnt].IxSize = sizeof(DWORD);
			pKMesh->IxVec[IdxCnt].Fm = IDX32::FM();
			pKMesh->IxVec[IdxCnt].Usage = D3D11_USAGE_DEFAULT;
			pKMesh->IxVec[IdxCnt].m_InxBD = new char[pKMesh->IxVec[IdxCnt].IxCnt * pKMesh->IxVec[IdxCnt].IxSize];

			memcpy_s(pKMesh->IxVec[IdxCnt].m_InxBD, pKMesh->IxVec[IdxCnt].IxCnt * pKMesh->IxVec[IdxCnt].IxSize
				, &pMD->IdxVec[IdxCnt][0], pKMesh->IxVec[IdxCnt].IxCnt * pKMesh->IxVec[IdxCnt].IxSize);
		}

		pKMesh->MtlVec.resize(pMD->m_MtlVec.size());

		memcpy_s(&pKMesh->MtlVec[0], sizeof(Material_FbxData) * (KUINT)pMD->m_MtlVec.size()
			, &pMD->m_MtlVec[0], sizeof(Material_FbxData) * (KUINT)pMD->m_MtlVec.size());

		MeshVec.push_back(pKMesh);
	}

	//// 애니메이션이 없는 경우를 위한 대비책
	if (0 < _Data->Ani_Vec.size())
	{
		AniVec.resize(_Data->Ani_Vec.size());
		memcpy_s(&AniVec[0], sizeof(Animation_Info) * (KUINT)_Data->Ani_Vec.size()
			, &_Data->Ani_Vec[0], sizeof(Animation_Info) * (KUINT)_Data->Ani_Vec.size());
	}

	if (0 < _Data->Bone_Vec.size())
	{
		BoneVec.resize(_Data->Bone_Vec.size());

		for (size_t i = 0; i < _Data->Bone_Vec.size(); i++)
		{
			BoneVec[i] = new KM3Bone();

			BoneVec[i]->BoneMX = _Data->Bone_Vec[i]->Bone_KMX;
			BoneVec[i]->OffsetMX = _Data->Bone_Vec[i]->Offset_KMX;
			BoneVec[i]->Depth = _Data->Bone_Vec[i]->Depth;
			BoneVec[i]->Index = _Data->Bone_Vec[i]->Index;

			memcpy_s(BoneVec[i]->Name, sizeof(wchar_t) * PATHNUM, _Data->Bone_Vec[i]->Name, sizeof(wchar_t) * PATHNUM);

			// 키프레임이 없는 경우를 위한 대비책
			if (0 == _Data->Bone_Vec[i]->KFVec.size())
			{
				continue;
			}

			BoneVec[i]->KFrameVec.resize(_Data->Bone_Vec[i]->KFVec.size());
			memcpy_s(&BoneVec[i]->KFrameVec[0], sizeof(KeyFrame) * (KUINT)BoneVec[i]->KFrameVec.size()
				, &_Data->Bone_Vec[i]->KFVec[0], sizeof(KeyFrame) * (KUINT)BoneVec[i]->KFrameVec.size());
		}
	}
}
	

void KM3Data::SaveKM3(const wchar_t* _Path)
{
	WriteStream Stream = WriteStream(_Path);

	int MeshSize = (int)MeshVec.size();
	Stream.Write(MeshSize);

	for (size_t MeshCnt = 0; MeshCnt < MeshVec.size(); MeshCnt++)
	{
		KM3Mesh* pKM3 = MeshVec[MeshCnt];
		Stream.Write(pKM3->VxCnt);
		Stream.Write(pKM3->VxSize);
		Stream.Write(pKM3->Usage);
		Stream.Write(pKM3->m_VertBD, pKM3->VxCnt * pKM3->VxSize);
		
		int IdxSize = (int)MeshVec[MeshCnt]->IxVec.size();
		Stream.Write(IdxSize);

		for (size_t IdxCnt = 0; IdxCnt < MeshVec[MeshCnt]->IxVec.size(); IdxCnt++)
		{
			KM3Index* pNewID = &(MeshVec[MeshCnt]->IxVec[IdxCnt]);

			Stream.Write(pNewID->IxSize);
			Stream.Write(pNewID->IxCnt);
			Stream.Write(pNewID->Usage);
			Stream.Write(pNewID->Fm);
			Stream.Write(pNewID->m_InxBD, pNewID->IxSize * pNewID->IxCnt);
		}

		int MtlSize = (int)MeshVec[MeshCnt]->MtlVec.size();
		Stream.Write(MtlSize);

		if (0 != MtlSize)
		{
			Stream.Write(&MeshVec[MeshCnt]->MtlVec[0], sizeof(Material_FbxData) *  (KUINT)MeshVec[MeshCnt]->MtlVec.size());
		}
	}

	int AniSize = (int)AniVec.size();
	Stream.Write(AniSize);

	if (0 != AniSize)
	{
		Stream.Write(&AniVec[0], sizeof(Animation_Info)* (KUINT)AniSize);
	}

	int BoneSize = (int)BoneVec.size();
	Stream.Write(BoneSize);
	for (int i = 0; i < BoneSize; i++)
	{
		Stream.Write(BoneVec[i]->Name, sizeof(wchar_t) * PATHNUM);
		Stream.Write(BoneVec[i]->BoneMX);
		Stream.Write(BoneVec[i]->OffsetMX);
		Stream.Write(BoneVec[i]->Depth);
		Stream.Write(BoneVec[i]->Index);

		int FrameSize = (int)BoneVec[i]->KFrameVec.size();
		Stream.Write(FrameSize);
		if (0 < FrameSize)
		{
			Stream.Write(&(BoneVec[i]->KFrameVec[0]), sizeof(KeyFrame) * FrameSize);
		}
	}
}

void KM3Data::LoadKM3(const wchar_t* _Path)
{
	ReadStream Stream = ReadStream(_Path);

	int MeshSize;
	Stream.Read(MeshSize);

	// 메쉬가 0인데 뭘 불러오냐 ㅋㅋ
	if (0 == MeshSize)
	{
		BBY;
	}

	MeshVec.resize(MeshSize);

	for (size_t MeshCnt = 0; MeshCnt < MeshVec.size(); MeshCnt++)
	{
		MeshVec[MeshCnt] = new KM3Mesh();
		KM3Mesh* pKM3 = MeshVec[MeshCnt];
		Stream.Read(MeshVec[MeshCnt]->VxCnt);
		Stream.Read(MeshVec[MeshCnt]->VxSize);
		Stream.Read(MeshVec[MeshCnt]->Usage);

		MeshVec[MeshCnt]->m_VertBD = new char[MeshVec[MeshCnt]->VxCnt * MeshVec[MeshCnt]->VxSize];
		Stream.Read(MeshVec[MeshCnt]->m_VertBD, MeshVec[MeshCnt]->VxCnt * MeshVec[MeshCnt]->VxSize);

		int IdxSize;
		Stream.Read(IdxSize);
		MeshVec[MeshCnt]->IxVec.resize(IdxSize);
		for (size_t IdxCnt = 0; IdxCnt < MeshVec[MeshCnt]->IxVec.size(); IdxCnt++)
		{
			KM3Index* pNewID = &(MeshVec[MeshCnt]->IxVec[IdxCnt]);

			Stream.Read(pNewID->IxSize);
			Stream.Read(pNewID->IxCnt);
			Stream.Read(pNewID->Usage);
			Stream.Read(pNewID->Fm);
			pNewID->m_InxBD = new char[pNewID->IxSize * pNewID->IxCnt];
			Stream.Read(pNewID->m_InxBD, pNewID->IxSize * pNewID->IxCnt);
		}

		int MtlSize = (int)MeshVec[MeshCnt]->MtlVec.size();
		Stream.Read(MtlSize);

		if (0 != MtlSize)
		{
			MeshVec[MeshCnt]->MtlVec.resize(MtlSize);
			Stream.Read(&MeshVec[MeshCnt]->MtlVec[0], sizeof(Material_FbxData) *  (KUINT)MtlSize);
		}
	}

	int AniSize;
	Stream.Read(AniSize);

	if (0 != AniSize)
	{
		AniVec.resize(AniSize);
		Stream.Read(&AniVec[0], sizeof(Animation_Info)* (KUINT)AniSize);
	}


	int BoneSize;
	Stream.Read(BoneSize);	
	if (0 != BoneSize)
	{
		BoneVec.resize(BoneSize);
		for (int i = 0; i < BoneSize; i++)
		{
			BoneVec[i] = new KM3Bone();
			Stream.Read(BoneVec[i]->Name, sizeof(wchar_t) * PATHNUM);
			BoneMap.insert(std::map<std::wstring, KM3Bone*>::value_type(BoneVec[i]->Name, BoneVec[i]));
			Stream.Read(BoneVec[i]->BoneMX);
			Stream.Read(BoneVec[i]->OffsetMX);
			Stream.Read(BoneVec[i]->Depth);
			Stream.Read(BoneVec[i]->Index);

			int FrameSize;
			Stream.Read(FrameSize);
			if (0 < FrameSize)
			{
				BoneVec[i]->KFrameVec.resize(FrameSize);
				Stream.Read(&(BoneVec[i]->KFrameVec[0]), sizeof(KeyFrame) * FrameSize);
			}
		}
	}

}


/********************** MeshData ****************************/
MeshContainer::MeshContainer()
{
}
MeshContainer::~MeshContainer()
{
}

bool MeshContainer::Load(MESH_LMODE _eMode)
{
	if (MESH_LMODE::LM_FBX == _eMode)
	{
		FBXLoader pLoader;
		pLoader.Load_FBX(AllPath());
		m_Data.ConvertToKM3(pLoader.m_pNewFbx);
		return true;
	}

	else
	{
		m_Data.LoadKM3(AllPath());
		return true;
	}
}


void MeshContainer::Save(const wchar_t* _Path)
{
	m_Data.SaveKM3(_Path);
}

KM3Bone* MeshContainer::Find_Bone(const wchar_t* _Name)
{
	std::map<std::wstring, KM3Bone*>::iterator FI = m_Data.BoneMap.find(_Name);

	if (FI != m_Data.BoneMap.end())
	{
		return FI->second;
	}

	return nullptr;
}