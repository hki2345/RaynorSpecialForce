#include "KMesh.h"
#include "Core_Class.h"


void Buffer_Vertex::Set_VtxData(void* _VtxTemp, const KUINT& _VtxSize)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	Core_Class::Context()->Map(m_VertBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	memcpy(tSub.pData, &_VtxTemp, _VtxSize);
	Core_Class::Context()->Unmap(m_VertBuf, 0);
}


KMesh::KMesh() : m_eDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

KMesh::~KMesh()
{
}


bool KMesh::Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem,
	UINT _IdxCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem,
	DXGI_FORMAT	_IdxFm, D3D11_PRIMITIVE_TOPOLOGY _eDrawMode)
{
	if (false == Create_Vertex(_iVtxCount, _iVtxSize, _eVtxUsage, _VtxMem))
	{
		return false;
	}

	if (false == Create_Index(_IdxCount, _iIdxSize, _eIdxUsage, _IdxFm, _IdxMem))
	{
		return false;
	}

	m_eDrawMode = _eDrawMode;

	return true;
}


void KMesh::Update_Vertex(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem)
{
	VertSize_Vec.clear();
	VertBuff_Vec.clear();
	VertBuffInfo_Vec.clear();

	Create_Vertex(_iVtxCount, _iVtxSize, _eVtxUsage, _VtxMem);
}

void KMesh::Update_Index(UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, DXGI_FORMAT _IdxFm, void* _IdxMem)
{
	IndexBuffInfo_Vec.clear();

	Create_Index(_iTriCount, _iIdxSize, _eIdxUsage, _IdxFm, _IdxMem);
}


bool KMesh::Create_Vertex(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem) 
{
	Buffer_Vertex* NewInfo = new Buffer_Vertex();

	NewInfo->m_VtxCnt = _iVtxCount;
	NewInfo->m_VtxSize = _iVtxSize;
	NewInfo->BufferDesc.ByteWidth = _iVtxCount * _iVtxSize;
	NewInfo->BufferDesc.Usage = _eVtxUsage;

	if (D3D11_USAGE_DYNAMIC == NewInfo->BufferDesc.Usage)
	{
		NewInfo->BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	NewInfo->BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSub = D3D11_SUBRESOURCE_DATA();
	tSub.pSysMem = _VtxMem;

	if (S_OK != Core_Class::PDevice()->CreateBuffer(&NewInfo->BufferDesc, &tSub, &NewInfo->m_VertBuf))
	{
		BBY;
		return false;
	}
	
	VertSize_Vec.push_back(NewInfo->m_VtxSize);
	VertBuff_Vec.push_back(NewInfo->m_VertBuf);
	VertBuffInfo_Vec.push_back(NewInfo);

	// 여러개를 하니 이제 만들면 여러개를 넣어준다.
	

	return true;
}



bool KMesh::Create_Index(UINT _iIdxCnt, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, DXGI_FORMAT _IdxFm, void* _IdxMem)
{
	Buffer_Index* NewInfo = new Buffer_Index();

	NewInfo->m_IdxFm = _IdxFm;
	NewInfo->m_IdxCnt = _iIdxCnt;
	NewInfo->m_IdxSize = _iIdxSize;
	NewInfo->BufferDesc.ByteWidth = _iIdxCnt * _iIdxSize;
	NewInfo->BufferDesc.Usage = _eIdxUsage;


	if (D3D11_USAGE_DYNAMIC == NewInfo->BufferDesc.Usage)
	{
		NewInfo->BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	NewInfo->BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSub = D3D11_SUBRESOURCE_DATA();
	tSub.pSysMem = _IdxMem;

	if (S_OK != Core_Class::PDevice()->CreateBuffer(&NewInfo->BufferDesc, &tSub, &NewInfo->m_IdxBuf))
	{
		BBY;
		return false;
	}

	IndexBuffInfo_Vec.push_back(NewInfo);

	return true;
}

void KMesh::Update(const KUINT& _StartVtx /*= 0*/, const KUINT& _VtxCnt /*= 1*/, KUINT* _pOff /*= nullptr*/)
{
	UINT Idx = 0;
	UINT* iOff = _pOff;
	
	if (nullptr == _pOff)
	{
		iOff = &Idx;
	}

	Core_Class::Context()->IASetVertexBuffers(_StartVtx, _VtxCnt, &VertBuff_Vec[0], &VertSize_Vec[0], iOff);
	Core_Class::Context()->IASetPrimitiveTopology(m_eDrawMode);
}
void KMesh::Render(const KUINT& _Start /*= 0*/, const KUINT& _EndIdx /*= 1*/, KUINT* _pOff /*= nullptr*/) 
{
	UINT* iOff = _pOff;
	UINT Idx = 0;

	if (nullptr == _pOff)
	{
		iOff = &Idx;
	}

	for (size_t i = _Start; i < _EndIdx; i++)
	{
		Core_Class::Context()->IASetIndexBuffer(
			IndexBuffInfo_Vec[i]->m_IdxBuf, IndexBuffInfo_Vec[i]->m_IdxFm, iOff[i]);
		Core_Class::Context()->DrawIndexed(IndexBuffInfo_Vec[i]->m_IdxCnt, 0, 0);
	}
}



void KMesh::Update_Pick(const KUINT& _Vtx, const KUINT& _Sub)
{
	KUINT tIdx = 0;

	Core_Class::Context()->IASetPrimitiveTopology(m_eDrawMode);
	Core_Class::Context()->IASetVertexBuffers(_Vtx, 1, &VertBuff_Vec[0], &VertSize_Vec[0], &tIdx);
	Core_Class::Context()->IASetIndexBuffer(IndexBuffInfo_Vec[_Sub]->m_IdxBuf, IndexBuffInfo_Vec[_Sub]->m_IdxFm, 0);
	Core_Class::Context()->DrawIndexed(IndexBuffInfo_Vec[_Sub]->m_IdxCnt, 0, 0);
}



void KMesh::Set_VtxData(const KUINT& _BufIdx, void* _VtxTemp, const KUINT& _VtxSize)
{
	if (VertBuffInfo_Vec.size() <= _BufIdx)
	{
		BBY;
	}

	VertBuffInfo_Vec[_BufIdx]->Set_VtxData(_VtxTemp, _VtxSize);
}