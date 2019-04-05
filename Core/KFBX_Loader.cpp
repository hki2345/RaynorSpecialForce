#include "KFBX.h"
#include <atlstr.h>
#include "KMacro.h"
#include "DXContainer_DE.h"
#include <algorithm>

#include "ResourceManager.h"
#include "KThread.h"



FBXLoader::FBXLoader()
{

}
FBXLoader::~FBXLoader()
{
	if (nullptr != m_pNewFbx)
	{
		delete m_pNewFbx;
	}
}

FbxAMatrix FBXLoader::MX_Reflect;

void FBXLoader::Init_FBXLoader()
{
	MX_Reflect[0] = FbxVector4{ 1, 0, 0, 0 };
	MX_Reflect[1] = FbxVector4{ 0, 0, 1, 0 };
	MX_Reflect[2] = FbxVector4{ 0, 1, 0, 0 };
	MX_Reflect[3] = FbxVector4{ 0, 0, 0, 1 };
}



// �ε� ���� ����
// [KFBX ����] - [�޴��� ����] - [�޴������� IOS ����] - [MAX Scene�� ����] - [�� Scene Import ����]
// [���õ� IOS�� ���� Import] - [���� Scene������] - [���Ͽ��� ���� ���� �˻�]
// [Bone�� ���� ���� �� ����] (�� ���� ������ �ִϸ��̼��� ������ �Ǻ����� ����) - [�ﰢȭ]
// [���� �� ���ִϸ��̼� �ҷ�����] - [�ִϸ��̼� Ȯ��(�ִϸ��̼� �̸��� ������ Ȯ��)]
// [�޽� �ҷ�����] - [�ִϸ��̼��� ���� ������ ����] - [���� �ҷ�����]
void FBXLoader::Load_FBX(const wchar_t* _Path)
{
	m_pNewFbx = new KFBX();
	m_pManager = FbxManager::Create();

	// �ҷ��� �����δ�
	// ���ؽ�(�÷�, �븻, ���̳븻, ź��Ʈ, �÷� ��) �ε��� 
	// ���̴�
	// ����
	// 1. �ؽ���
	// 2. �𽺽��ٽ�- ������ʹ� ������ ������ - FBX �� ���� - �׷��� ��������
	// 3. ����
	// 4. �����Ͷ�����

	// �÷��� �� �ҷ����� ������ �ٸ��� �װ� �������ش�.
	FbxIOSettings* pIos = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(pIos);



	// ���ο��� ���������� �Ű� �����͸� ����. ��ĳ���� �ڽ� 2D�� ���� ���
	m_pScene = FbxScene::Create(m_pManager, "");
	FbxImporter* pImp = FbxImporter::Create(m_pManager, "");

	// m_psz -> �ڵ� string ��ȯ -> wstring���� char* ������
	bool Temp = pImp->Initialize(CW2A(_Path, CP_UTF8).m_psz, -1, m_pManager->GetIOSettings());
	bool Temp2 = pImp->Import(m_pScene);


	// ��ǥ�� �谡 ��ǰ���� �Ǵ� �÷������� �ٸ� ���� �ֱ� ������ �����ִ� �۾�
	// ��, �ʹ� �ͽ����� ����.
	if (m_pScene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::eMax)
	{
		m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eMax);
	}

	Check_AllBoneCnt(m_pScene->GetRootNode());

	// �� ���� ��ŭ ���� ���߱�
	m_pNewFbx->Bone_Vec.reserve(m_BoneCnt);

	// �ﰢȭ
	Triangulate(m_pScene->GetRootNode());

	// ���� �ִ������� ������
	Load_Bone(m_pScene->GetRootNode(), 0, nullptr);
	Check_Animation();

	// �޽� ������
	Set_MeshData(m_pScene->GetRootNode());


	pImp->Destroy();
	m_pScene->Destroy();
	pIos->Destroy();
	m_pManager->Destroy();

	// �Ҹꤩ�ڿ��� �Ѵ�.
	// delete m_pNewFbx;
}

// ���� ������ ���� ��ü ������ �˾ƿ´�.
void FBXLoader::Check_AllBoneCnt(FbxNode* _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();
	if (nullptr != pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		++m_BoneCnt;
	}
	
	int Cnt = _pNode->GetChildCount();
	for (int i = 0; i < Cnt; i++)
	{
		Check_AllBoneCnt(_pNode->GetChild(i));
	}
}
void FBXLoader::Load_Bone(FbxNode* _pNode, KUINT _Depth, KBone* _pParent)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	KBone* NBone = nullptr;


	// ��忡 ���� �ִ��� ������ �˻���
	if (nullptr != pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		NBone = new KBone();

		std::wstring NName = CA2W(_pNode->GetName()).m_psz;
		memcpy_s(NBone->Name, sizeof(wchar_t) * PATHNUM, NName.c_str(), NName.size() * 2);
		NBone->Depth = _Depth++; // ���� ������ ������ -> Parent���� 1 �÷��ְ� ������ ����
		NBone->Index = (int)m_pNewFbx->Bone_Vec.size();
		NBone->m_pPBone = _pParent;
		m_pNewFbx->Bone_Vec.push_back(NBone);
		m_pNewFbx->Bone_Map.insert(std::multimap<std::wstring, KBone*>::value_type(NBone->Name, NBone));
	}
	else
	{
		_Depth++;
	}

	// Child Count - ����
	int CC = _pNode->GetChildCount();
	for (int i = 0; i < CC; i++)
	{
		Load_Bone(_pNode->GetChild(i), _Depth, NBone);
	}
}

void FBXLoader::Check_Animation()
{
	FbxArray<FbxString*> AniNameArr;
	m_pScene->FillAnimStackNameArray(AniNameArr);
	m_pNewFbx->Ani_Vec.resize(AniNameArr.GetCount());

	for (int i = 0; i < AniNameArr.GetCount(); i++)
	{
		FbxAnimStack* TempStack = m_pScene->FindMember<FbxAnimStack>(AniNameArr[i]->Buffer());

		// �ִϸ��̼��� ���� ���� �ִ� -> �ǹ� ������
		if (nullptr == TempStack)
		{
			continue;
		}

		Animation_Info NewInfo ;

		std::wstring NName = CA2W(TempStack->GetName()).m_psz;
		memcpy_s(NewInfo.Name, sizeof(wchar_t) * PATHNUM, NName.c_str(), NName.size() * 2);

		// �ִϸ��̼��� ���۰� ���� �ð��� �޾ƿ�
		FbxTakeInfo* TempInfo = m_pScene->GetTakeInfo(TempStack->GetName());
		NewInfo.Stime = TempInfo->mLocalTimeSpan.GetStart();
		NewInfo.Etime = TempInfo->mLocalTimeSpan.GetStop();

		NewInfo.eMode = m_pScene->GetGlobalSettings().GetTimeMode();
		NewInfo.Length_Time =
			NewInfo.Etime.GetFrameCount(NewInfo.eMode) - 
			NewInfo.Stime.GetFrameCount(NewInfo.eMode);

		// �ִϸ��̼ǿ� �ε����� �־� �����ϴµ� - ó������ ����ũ 1�̹Ƿ� 
		// �켱 1�� �� ��.. ������ �����߿� �߰��� �� �����Ƿ� ����
		NewInfo.Index = i;
		m_pNewFbx->Ani_Vec.push_back(NewInfo);
		m_pNewFbx->Ani_Map.insert(std::map<std::wstring, Animation_Info*>::value_type(NewInfo.Name, &NewInfo));
	}

	for (int i = 0; i < AniNameArr.GetCount(); i++)
	{
		delete AniNameArr[i];
	}
}
void FBXLoader::Triangulate(FbxNode* _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	// ��Ʈ���� ��Ʈ����Ʈ�� ã�� ������ ���� �� �ִ�.
	// �̰͵� �ݺ� ������ ������ �ڽĿ��� �� �ҷ���
	if (nullptr != pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		// �츮 �ʿ��� ���� ���� �������ִ� �۾�
		// ������ ����Ʈ �ϴ� �����̰� �޽������� ������ �� �ִ�.
		FbxGeometryConverter NewCon(m_pManager);

		// 2��° bool�� ���� ���ų� ���ųĸ� ���´�.
		NewCon.Triangulate(pAttr, true);
	}

	int CC = _pNode->GetChildCount();
	for (int i = 0; i < CC; i++)
	{
		Triangulate(_pNode->GetChild(i));
	}
}


void FBXLoader::Set_MeshData(FbxNode* _pNode)
{
	FbxNodeAttribute* pAtttr = _pNode->GetNodeAttribute();


	// �ش� �� �Լ��� �޽��� ������ �Ѵ�. ->
	// �� ���ϵȴ� �ش� Ŭ������ �����ε��� �� �Ǿ� �����Ƿ� �ڿ��� �����ϴ� ��
	if (nullptr != pAtttr && pAtttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pMesh = _pNode->GetMesh();

		if (nullptr != pMesh)
		{
			Mesh_FbxData* NewMD = new Mesh_FbxData();
			m_pNewFbx->MeshData_Vec.push_back(NewMD);


			std::wstring NName = CA2W(pMesh->GetName()).m_psz;
			memcpy_s(NewMD->Name, sizeof(wchar_t) * PATHNUM, NName.c_str(), NName.size() * 2);

			int MtrlCount = _pNode->GetMaterialCount();
			NewMD->IdxVec.resize(MtrlCount);
			NewMD->m_MtlVec.reserve(MtrlCount);

			Set_Mesh(NewMD, pMesh);
			
			
			// ������ ���ؽ� �ε������� �׷��� �޽� �ϳ��� ���� �ִϸ��̼� ������
			// �����վ��Ѵ�.
			if (false == m_pNewFbx->Ani_Map.empty())
			{
				Set_AniData(pMesh, NewMD);
			}
			// 
			else
			{
				NewMD->m_bAni = false;
			}

			for (int i = 0; i < MtrlCount; i++)
			{
				Set_Material(NewMD, _pNode->GetMaterial(i));
			}

		} // if(nullptr != pMesh)
	} // if (nullptr != pAtttr && pAtttr->GetAttributeType() == FbxNodeAttribute::eMesh)


	int CC = _pNode->GetChildCount();
	for (int i = 0; i < CC; i++)
	{
		Set_MeshData(_pNode->GetChild(i));
	}
}


// �޽� ����
void FBXLoader::Set_Mesh(Mesh_FbxData* _pMD, FbxMesh* _pMesh)
{
	// ������ ���� Ȯ�� - ��ġ
	int iVtxCnt = _pMesh->GetControlPointsCount();
	_pMD->Set_VertexCount(iVtxCnt);


	// ���� �޾ƿ���
	FbxVector4* pFbxPos = _pMesh->GetControlPoints();

	for (int i = 0; i < iVtxCnt; i++)
	{
		_pMD->VertVec[i].m_Pos.x = (float)pFbxPos[i].mData[0];
		_pMD->VertVec[i].m_Pos.y = (float)pFbxPos[i].mData[2];
		_pMD->VertVec[i].m_Pos.z = (float)pFbxPos[i].mData[1];
		_pMD->VertVec[i].m_Pos.w = 1.0f;
	}

	// �ε��� ����
	int TriCnt = _pMesh->GetPolygonCount();

	int IdxSize = _pMesh->GetPolygonSize(0);

	// �ε����� 3�� �ƴ� ��쵵 ������ �켱 �⺻�����δ� 3�̱� ������
	// �ϴ� ������ �Ÿ���.
	if (3 != IdxSize)
	{
		BBY;
	}

	IDX32 tempIDX32 = {};
	KUINT CurVtx = 0;

	FbxGeometryElementMaterial* pMtrl = _pMesh->GetElementMaterial();

	for (int i = 0; i < TriCnt; i++)
	{
		for (int j = 0; j < IdxSize; j++)
		{
			tempIDX32.p[j] = (DWORD)_pMesh->GetPolygonVertex(i, j);
			Set_Normal(_pMesh, _pMD, tempIDX32.p[j], CurVtx);
			Set_Tangent(_pMesh, _pMD, tempIDX32.p[j], CurVtx);
			Set_BNormal(_pMesh, _pMD, tempIDX32.p[j], CurVtx);
			Set_Uv(_pMesh, _pMD, tempIDX32.p[j], CurVtx);
			++CurVtx;
		}

		// i ���� ����� �ε��� ���ۿ� ���פ���
		// �ش� �����͸� ����. - �ش� ������ = ���� ��
		int SubIdx = pMtrl->GetIndexArray().GetAt(i);
		_pMD->IdxVec[SubIdx].push_back(tempIDX32.p[0]);
		_pMD->IdxVec[SubIdx].push_back(tempIDX32.p[2]);
		_pMD->IdxVec[SubIdx].push_back(tempIDX32.p[1]);
	}

}

// Semantic
// MSDN �ڷ��̰� HLSL���� �����ϴ� ���� -> ���۵� ���̴����� �ߴٸ� ���� �̸��� ���� �˰���.
// ���۵� Ȥ�� ������ ���̴� ���� ������ �ؽ��� �ռ��� �ϱ� ���ؼ� 
// (���⼭ �ռ��̶� ���� Diffuse�Ӹ� �ƴ϶� Spec�� Bumpȿ���� ����)
// �ؽ��İ� ���� �� �������� �� �ٽ��̴�. �̰� �̸� ���� ���� �����̴�.
// https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/dx-graphics-hlsl-semantics

void FBXLoader::Set_Normal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// �ø�ƽ���� 
	// Normal 0,1 ~ �ɼ� ���� ���⼭�� ������ �븻 ����
	int Count = _pMesh->GetElementNormalCount();

	// �װ����� �ϳ��� �ƴϸ� �Ͷ߸� - �ϳ��� ������ �⺻���� ������
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementNormal* pE = _pMesh->GetElementNormal();
	KUINT Idx = 0;

	// ���⼭ �� ¤�� �Ѿ�� �� �� - ���ؽ� �� �ε��� �����ĵ� 
	// �ٸ��� ����� �� �ִ���
	// ũ�� �� ������ �ɼ����� �����µ� -> �Ƹ� Max �ɼ� ���δϱ� �� 2�� ���� �ƴ� �� ��û ���� ��
	// ���ؽ� ������ ����Ǵ� ��� Ȥ�� �ε��� ������ �װ���

	// ���ؽ� ����
	if (pE->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurVtx;
		}
		else
		{
			Idx = pE->GetIndexArray().GetAt(_CurVtx);
		}
	}
	// �ε��� ����
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// �ε���
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// �ƹ����� �� ���� �ڵ带 �ݺ��ؼ� ���⿡ �ϴ� �̸��� ���߾� ���´�.
	FbxVector4 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->VertVec[_CurIdx].m_Normal.x = (float)vData.mData[0];
	_pMeshData->VertVec[_CurIdx].m_Normal.y = (float)vData.mData[2];
	_pMeshData->VertVec[_CurIdx].m_Normal.z = (float)vData.mData[1];

	// �츮�� ���⼭ w�� ���� �ʴ´�.
	_pMeshData->VertVec[_CurIdx].m_Normal.w = .0f;
}


void FBXLoader::Set_Tangent(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// �ø�ƽ���� 
	// Normal 0,1 ~ �ɼ� ���� ���⼭�� ������ �븻 ����
	int Count = _pMesh->GetElementTangentCount();

	// �װ����� �ϳ��� �ƴϸ� �Ͷ߸� - �ϳ��� ������ �⺻���� ������
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementTangent* pE = _pMesh->GetElementTangent();
	KUINT Idx = 0;

	// ���⼭ �� ¤�� �Ѿ�� �� �� - ���ؽ� �� �ε��� �����ĵ� 
	// �ٸ��� ����� �� �ִ���
	// ũ�� �� ������ �ɼ����� �����µ� -> �Ƹ� Max �ɼ� ���δϱ� �� 2�� ���� �ƴ� �� ��û ���� ��
	// ���ؽ� ������ ����Ǵ� ��� Ȥ�� �ε��� ������ �װ���

	// ���ؽ� ����
	if (pE->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurVtx;
		}
		else
		{
			Idx = pE->GetIndexArray().GetAt(_CurVtx);
		}
	}
	// �ε��� ����
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// �ε���
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// �ƹ����� �� ���� �ڵ带 �ݺ��ؼ� ���⿡ �ϴ� �̸��� ���߾� ���´�.
	FbxVector4 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->VertVec[_CurIdx].m_Tangent.x = (float)vData.mData[0];
	_pMeshData->VertVec[_CurIdx].m_Tangent.y = (float)vData.mData[2];
	_pMeshData->VertVec[_CurIdx].m_Tangent.z = (float)vData.mData[1];

	// �츮�� ���⼭ w�� ���� �ʴ´�.
	_pMeshData->VertVec[_CurIdx].m_Tangent.w = .0f;
}


void FBXLoader::Set_BNormal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// �ø�ƽ���� 
	// Normal 0,1 ~ �ɼ� ���� ���⼭�� ������ �븻 ����
	int Count = _pMesh->GetElementBinormalCount();

	// �װ����� �ϳ��� �ƴϸ� �Ͷ߸� - �ϳ��� ������ �⺻���� ������
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementBinormal* pE = _pMesh->GetElementBinormal();
	KUINT Idx = 0;

	// ���⼭ �� ¤�� �Ѿ�� �� �� - ���ؽ� �� �ε��� �����ĵ� 
	// �ٸ��� ����� �� �ִ���
	// ũ�� �� ������ �ɼ����� �����µ� -> �Ƹ� Max �ɼ� ���δϱ� �� 2�� ���� �ƴ� �� ��û ���� ��
	// ���ؽ� ������ ����Ǵ� ��� Ȥ�� �ε��� ������ �װ���

	// ���ؽ� ����
	if (pE->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurVtx;
		}
		else
		{
			Idx = pE->GetIndexArray().GetAt(_CurVtx);
		}
	}
	// �ε��� ����
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// �ε���
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// �ƹ����� �� ���� �ڵ带 �ݺ��ؼ� ���⿡ �ϴ� �̸��� ���߾� ���´�.
	FbxVector4 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->VertVec[_CurIdx].m_Binormal.x = (float)vData.mData[0];
	_pMeshData->VertVec[_CurIdx].m_Binormal.y = (float)vData.mData[2];
	_pMeshData->VertVec[_CurIdx].m_Binormal.z = (float)vData.mData[1];

	// �츮�� ���⼭ w�� ���� �ʴ´�.
	_pMeshData->VertVec[_CurIdx].m_Binormal.w = .0f;
}

void FBXLoader::Set_Uv(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// �ø�ƽ���� 
	// Normal 0,1 ~ �ɼ� ���� ���⼭�� ������ �븻 ����
	int Count = _pMesh->GetElementUVCount();

	// �װ����� �ϳ��� �ƴϸ� �Ͷ߸� - �ϳ��� ������ �⺻���� ������
	if (1 > Count)
	{
		BBY;
	}

	FbxGeometryElementUV* pE = _pMesh->GetElementUV();
	KUINT Idx = 0;

	// ���⼭ �� ¤�� �Ѿ�� �� �� - ���ؽ� �� �ε��� �����ĵ� 
	// �ٸ��� ����� �� �ִ���
	// ũ�� �� ������ �ɼ����� �����µ� -> �Ƹ� Max �ɼ� ���δϱ� �� 2�� ���� �ƴ� �� ��û ���� ��
	// ���ؽ� ������ ����Ǵ� ��� Ȥ�� �ε��� ������ �װ���

	// ���ؽ� ����
	if (pE->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurVtx;
		}
		else
		{
			Idx = pE->GetIndexArray().GetAt(_CurVtx);
		}
	}
	// �ε��� ����
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// �ε���
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// �ƹ����� �� ���� �ڵ带 �ݺ��ؼ� ���⿡ �ϴ� �̸��� ���߾� ���´�.

	// UV �� �翩������ Vector2�� �����Ѵ�.
	FbxVector2 vData = pE->GetDirectArray().GetAt(Idx);

	// �츮�� ���⼭ w�� ���� �ʴ´�.
	_pMeshData->VertVec[_CurIdx].m_UV.x = (float)vData.mData[0];
	// �׸��� max ��ǥ��� ���� �Ʒ����� ���. ���� ������ ���� ������ �ʿ���
	_pMeshData->VertVec[_CurIdx].m_UV.y = 1.0f - (float)(vData.mData[1]);
}

void FBXLoader::Set_AniData(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData)
{
	// ��Ų -> ��ġ�� ����
	// Deformer -> ��ȯ��
	// BlendShape -> Zbrush ó�� �޽��� �� ��׷����� ����
	// Skin -> �츮�� �ƴ� �� �ִϸ��̼� - ���պΰ��ְ� ���� �־� ���°� ����
	// VertexCacheDeformer -> ���� ���� �𸥴� �� �ѹ��� �� ���� �����
	// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=cpp_ref/class_fbx_deformer.
	// html,topicNumber=cpp_ref_class_fbx_deformer_htmle6d8427a-4060-4b8a-9a26-4ece8baf83c4
	int Count = _pMesh->GetDeformerCount(FbxDeformer::eSkin);


	if (0 >= Count)
	{
		_pMeshData->m_bAni = false;
		return;
	}

	_pMeshData->m_bAni = true;

	for (int i = 0; i < Count; i++)
	{
		FbxSkin* pSkin = (FbxSkin*)_pMesh->GetDeformer(i, FbxDeformer::eSkin);

		if (nullptr == pSkin)
		{
			continue;
		}

		FbxSkin::EType Dtype = pSkin->GetSkinningType();

		// ������ �ִ� - ���� �ִ�
		// ������ �ִ϶� ������ġ ĳ�� �׾��� ��ó�� ������Ÿ��� �� ���ϴ� ����
		// �װ� ������ �ƴϰ� (���� ������ �ְ�����) ���� ��ġ�� �ϳ��� ����� ���ϴ� ��
		if (FbxSkin::EType::eRigid == Dtype || Dtype == FbxSkin::EType::eLinear)
		{
			// Ŭ������ -> ���������� ��Ī -> ����ü
			int iClusterCnt = pSkin->GetClusterCount();

			for (int j = 0; j < iClusterCnt; j++)
			{
				// ���պ����� ������ Cluster��� �ϰ�
				// Cluster�� �𿩼� joint == ���� �̷�ٰ� �����Ѵ�.
				// Cluster (joint == ����)�� ���´�.

				// 
				FbxCluster* pCluster = pSkin->GetCluster(j);

				if (nullptr == pCluster || nullptr == pCluster->GetLink())
				{
					continue;
				}

				// �׶��� ���� �̸�
				KBone* pBone = m_pNewFbx->Find_Bone(CA2W(pCluster->GetLink()->GetName(), CP_UTF8).m_psz);

				// �ƴ� ��Ų�� �ִµ� ���� ���� �� ���ʿ� ���� �ȵ��ݾ�?
				// �� ���� �Ǵ� ��찡 �ִ�.
				// ���� ���� �ִϸ��̼��� ������ ��찡 �ִ� - ������ �Ϲ����� ���� �ƴ�
				// �Ϲ������θ� ���� ����ϱ⿡ �Ͷ߸��� ����
				if (nullptr == pBone)
				{
					BBY;
				}

				pBone->Bone_FMX = Get_FbxMatrix(_pMesh->GetNode());
				pBone->Bone_KMX = FMXtoKMX(pBone->Bone_FMX);

				Set_WeightIndices(pCluster, pBone, _pMeshData);
				Set_OffSetMatrix(pCluster, pBone, _pMeshData);
				Set_FrameMatrix(_pMesh->GetNode(), pCluster, pBone, _pMeshData);
			} // for (int j = 0; j < iClusterCnt; j++)
		} // if (FbxSkin::EType::eRigid == Dtype || Dtype == FbxSkin::EType::eLinear)
	} // for (int i = 0; i < Count; i++)

	Check_WI(_pMesh, _pMeshData);
}

// ���� ����
void FBXLoader::Set_Material(Mesh_FbxData* _pMD, FbxSurfaceMaterial* _pSur)
{
	Material_FbxData NewMtl = {};
	
	NewMtl.Info.Diff = Get_MaterialColor(_pSur, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	NewMtl.Info.Ambi = Get_MaterialColor(_pSur, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	NewMtl.Info.Spec = Get_MaterialColor(_pSur, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	NewMtl.Info.Emiv = Get_MaterialColor(_pSur, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);

	std::wstring tMesh = PathManager::Find_ForderPath(L"Mesh");

	std::wstring NName = Get_MaterialTexName(_pSur, FbxSurfaceMaterial::sDiffuse);
	NName = PathManager::Split_Path(NName, tMesh);
	memcpy_s(NewMtl.Diff, sizeof(wchar_t) * PATHNUM, NName.c_str(), NName.size() * 2);

	NName = Get_MaterialTexName(_pSur, FbxSurfaceMaterial::sNormalMap);
	NName = PathManager::Split_Path(NName, tMesh);
	memcpy_s(NewMtl.Bump, sizeof(wchar_t) * PATHNUM, NName.c_str(), NName.size() * 2);

	NName = Get_MaterialTexName(_pSur, FbxSurfaceMaterial::sSpecular);
	NName = PathManager::Split_Path(NName, tMesh);
	memcpy_s(NewMtl.Spec, sizeof(wchar_t) * PATHNUM, NName.c_str(), NName.size() * 2);
	
	NName = Get_MaterialTexName(_pSur, FbxSurfaceMaterial::sEmissive);
	NName = PathManager::Split_Path(NName, tMesh);
	memcpy_s(NewMtl.Emiv, sizeof(wchar_t) * PATHNUM, NName.c_str(), NName.size() * 2);

	_pMD->m_MtlVec.push_back(NewMtl);
}






FbxAMatrix FBXLoader::Get_FbxMatrix(FbxNode* _pNode)
{
	const FbxVector4 vT = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 vR = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 vS = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);


	// �̰� �ڵ����� ��� �����ϸ��ؼ� �����ִ� ����
	return FbxAMatrix(vT, vR, vS);
}


// ����ġ ���� - �Լ��� �� ����
void FBXLoader::Set_WeightIndices(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData)
{
	int IndexCnt = _pC->GetControlPointIndicesCount();

	// �� ����ġ�� �츮�� ����ϴ� �� �ƴϰ� Ŭ�����͸� ���� ����� �������ش�.
	WI tempWi;
	for (int i = 0; i < IndexCnt; i++)
	{
		tempWi.BoneIdx = _pBone->Index;
		tempWi.dWeight = _pC->GetControlPointWeights()[i];
		tempWi.Indices = _pC->GetControlPointIndices()[i];
		_pMeshData->WIVec[tempWi.Indices].push_back(tempWi);
	}
}



// Offset - �ִϸ��̼� ���� �⺻ �ڼ� ����
void FBXLoader::Set_OffSetMatrix(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData)
{
	FbxAMatrix MX_ClusterTrans;
	FbxAMatrix MX_ClusterLinkTrans;

	// ������ �� ������ �մ� Ʈ����
	_pC->GetTransformMatrix(MX_ClusterTrans);
	_pC->GetTransformLinkMatrix(MX_ClusterLinkTrans);

	FbxAMatrix MX_Offset;


	// �������? (1/2) * (2/1)
	// �� Ŭ�����ʹ����� �⺻���� ������ ����� ���ϴ� ������ �ȴ�.
	// ���� ��ũ�� �ٸ� ����� ����� * ���� ��� * ���� �⺻���
	// ��, ��ȭ �Ǿ��մ� ���¿��� ������ ����� ���� ��ķ� ���� �ʱ�ȭ �� �⺻����� ���� ����
	MX_Offset = MX_ClusterLinkTrans.Inverse() * MX_ClusterTrans * _pBone->Bone_FMX;

	// �̰� ��� ��ǥ���� y, z �� �ٲ�� �ֱ� ������ �ٲ��ִ� �� ->
	// �ι� ���ϴ� ������ ���� ���� �ٲٰ� ���� �ٱ��� ������
	MX_Offset = MX_Reflect * MX_Offset * MX_Reflect;

	_pBone->Offset_FMX = MX_Offset;
	_pBone->Offset_KMX = FMXtoKMX(MX_Offset);
}


//  �ð� �� �ִϸ��̼� ����
void FBXLoader::Set_FrameMatrix(FbxNode* _pNode, FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData)
{
	// Ÿ�Ӹ�带 ������ - �� Ÿ���� ��� �ִϸ��̼��� �ϴ� �����´�.
	FbxTime::EMode eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	for (size_t i = 0; i < m_pNewFbx->Ani_Vec.size(); i++)
	{
		FbxLongLong SFrame = m_pNewFbx->Ani_Vec[i].Stime.GetFrameCount(eTimeMode);
		FbxLongLong EFrame = m_pNewFbx->Ani_Vec[i].Etime.GetFrameCount(eTimeMode);

		for (FbxLongLong i = SFrame; i < EFrame; i++)
		{
			KeyFrame tFrame = {};
			FbxTime tTime = 0;

			// SetFrame -> Ÿ�� ���� �ð��� �־��ָ� �׿� �ش��ϴ�
			// FBX�� �ִϸ��̼ǿ��� ����� ���ִ� �ð���� ��ȯ����
			tTime.SetFrame(i, eTimeMode);

			// �ش� Ŭ���㽺�Ϳ� ��ũ�� Ŭ�����͸� ��� �ð������� ��ȯ����
			FbxAMatrix MX_FromTrans = _pNode->EvaluateGlobalTransform(tTime) * _pBone->Bone_FMX;
			FbxAMatrix MX_CurTrans = MX_FromTrans.Inverse() * _pC->GetLink()->EvaluateGlobalTransform(tTime);


			// �� �����Ӥ� ��ũ�� ���� ��ȯ
			MX_CurTrans = MX_Reflect * MX_CurTrans * MX_Reflect;

			tFrame.dTime = tTime.GetSecondDouble();
			
			tFrame.Scale = FBXLoader::FVectoKVec(MX_CurTrans.GetS());
			tFrame.Rotate = FBXLoader::FQTtoKVec(MX_CurTrans.GetQ());
			tFrame.Pos = FBXLoader::FVectoKVec(MX_CurTrans.GetT());

			m_pNewFbx->Bone_Vec[_pBone->Index]->KFVec.push_back(tFrame);
		}
	}
}



void FBXLoader::Check_WI(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData)
{
	// ����ġ ���� ��� -> ����ġ�� ���� �Ǻ��� �츮�� �����Ѵ�.
	std::vector<std::vector<WI>>::iterator Iter = _pMeshData->WIVec.begin();


	int iVtxInx = 0;
	for (iVtxInx = 0; Iter != _pMeshData->WIVec.end(); ++Iter, ++iVtxInx)
	{
		if ((*Iter).size() > 1)
		{
			// ���� ���� -> �̰� ������ Algoritm ����� �ʿ��ϴ�.
			// �׸��� �̰� �����Լ� Ư�� sort�� �ι�° ���ڰ��� �ٷ� ������ ���� �κ��ε�
			// �װ� �����Լ��� �ع���
			std::sort((*Iter).begin(), (*Iter).end(), 
				[](const WI& _left, const WI& _Right)
			{
				return _left.dWeight > _Right.dWeight;
			}
			);

			double dWeight = .0f;
			for (int i = 0; i < (int)(*Iter).size(); i++)
			{
				dWeight += (*Iter)[i].dWeight;
			}

			// ����ġ�� ���� 1���� Ŭ�� ������ - 0�� 1������ ��
			// ����ġ ��ü�� �ǹ̸� ������ ������ ó���κп� �����ش�.
			// �����ϳ� �Ѿ�� ���� �ִٸ� ���� Ŭ�����Ϳ� ������
			double revision = .0f;
			if (dWeight > 1.0)
			{
				revision = 1.0 - dWeight;
				(*Iter)[0].dWeight += revision;
			}

			if ((*Iter).size() >= 4)
			{
				(*Iter).erase((*Iter).begin() + 4, (*Iter).end());
			}
		} // if (_pMeshData->WIVec[i].size() > 1)

		for (int i = 0; i < (int)(*Iter).size(); i++)
		{
			_pMeshData->VertVec[iVtxInx].m_Weights.s[i] = (float)(*Iter)[i].dWeight;
			_pMeshData->VertVec[iVtxInx].m_Indices.s[i] = (float)(*Iter)[i].BoneIdx;
		}
	} // for (size_t i = 0, iVtxInx = 0; i < _pMeshData->WIVec.size(); i++)
}



KVector FBXLoader::Get_MaterialColor(FbxSurfaceMaterial* _pFbxMatData,
	const char* _pMtrlColorName,
	const char* _pMtrlFactorName)
{
	FbxDouble3 FbxColor;
	FbxDouble dFactor = 0;

	FbxProperty tMtrlColorPro = _pFbxMatData->FindProperty(_pMtrlColorName);
	FbxProperty tMtrlFactorPro = _pFbxMatData->FindProperty(_pMtrlFactorName);

	if (tMtrlColorPro.IsValid())
	{
		FbxColor = tMtrlColorPro.Get<FbxDouble3>();
	}

	if (tMtrlFactorPro.IsValid())
	{
		dFactor = tMtrlFactorPro.Get<FbxDouble>();
	}

	// ������ �� rgb�� ���������� �����´�.
	return KVector(
		(float)(FbxColor.mData[0] * dFactor),
		(float)(FbxColor.mData[1] * dFactor),
		(float)(FbxColor.mData[2] * dFactor),
		(float)dFactor
	);
}

std::wstring FBXLoader::Get_MaterialTexName(FbxSurfaceMaterial* _pFbxMatData,
	const char* _pMtrlTexName)
{
	// �ؽ��� �̸��� �׳� string�ε�
	// ������ �ؽ��� �̸��� ��ٴ� ���̴�. - ��θ� ������
	std::string Name = "";

	FbxProperty tMtrlTexNamePro = _pFbxMatData->FindProperty(_pMtrlTexName);

	if (tMtrlTexNamePro.IsValid())
	{
		KUINT iCnt = tMtrlTexNamePro.GetSrcObjectCount();

		if (0 != iCnt)
		{
			FbxFileTexture* pFbxTex = tMtrlTexNamePro.GetSrcObject<FbxFileTexture>(0);
			if (nullptr != pFbxTex)
			{
				Name = pFbxTex->GetFileName();
			}
		}
	}

	return CA2W(Name.c_str(), CP_UTF8).m_psz;
}


FbxMatrix FBXLoader::KMXtoFMX(const KMatrix& _Mat)
{
	FbxMatrix TMX;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			TMX.Set(y, x, _Mat.m[y][x]);
		}
	}

	return TMX;
}

KMatrix FBXLoader::FMXtoKMX(const FbxMatrix& _Mat)
{
	KMatrix TMX;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; ++x)
		{
			TMX.m[y][x] = (float)_Mat.Get(y, x);
		}
	}

	return TMX;
}
KVector FBXLoader::FVectoKVec(const FbxVector4& _Value)
{
	KVector TVec;

	for (int i = 0; i < 4; ++i)
	{
		TVec.s[i] = (float)_Value.mData[i];
	}

	return TVec;
}


KVector FBXLoader::FQTtoKVec(const FbxQuaternion& _Value)
{
	KVector TVec;
	
	
	for (int i = 0; i < 4; ++i)
	{
		TVec.s[i] = (float)_Value.mData[i];
	}

	return TVec;
}