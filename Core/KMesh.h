#pragma once
#include "Resource.h"
#include "DXContainer.h"
#include <vector>

class Buffer_Vertex : public SmartPtr
{
public:
	ID3D11Buffer*				m_VertBuf; // 버텍스 버퍼입니다.
	KUINT					    m_VtxSize;
	KUINT						m_VtxCnt;

	D3D11_BUFFER_DESC BufferDesc = D3D11_BUFFER_DESC();

	void Set_VtxData(void* _VtxTemp, const KUINT& _VtxSize);
	~Buffer_Vertex()
	{
		if (nullptr != m_VertBuf)
		{
			m_VertBuf->Release();
		}
	}
};


class Buffer_Index : public SmartPtr
{
public:
	ID3D11Buffer*				m_IdxBuf; // 버텍스 버퍼입니다.
	KUINT					    m_IdxSize;
	KUINT						m_IdxCnt;
	DXGI_FORMAT					m_IdxFm;

	D3D11_BUFFER_DESC BufferDesc = D3D11_BUFFER_DESC();

	Buffer_Index() :m_IdxBuf(nullptr) {}
	~Buffer_Index()
	{
		if (nullptr != m_IdxBuf)
		{
			m_IdxBuf->Release();
		}
	}
};


class KMesh : public KResource
{
private:
	// 이렇게 메쉬를 나타나내는 버퍼도 자료형을 통일 시켜버렸다. (KDevice 상수 버퍼 참고)
	//ID3D11Buffer*				m_InxBuf; // 인덱스 버퍼
	//DXGI_FORMAT					m_IdxFormat; // 인덱스 버퍼 포멧
	
	D3D11_PRIMITIVE_TOPOLOGY	m_eDrawMode; 
	
	// 18.10.25 이젠 아예 여러개를 들고 있게 해 관리한다
	std::vector<ID3D11Buffer*>			VertBuff_Vec;
	std::vector<KUINT>					VertSize_Vec;
	std::vector<KPtr<Buffer_Vertex>>	VertBuffInfo_Vec;
	std::vector<KPtr<Buffer_Index>>		IndexBuffInfo_Vec;

public:
	// 점이 몇개인가? UINT _iVtxCount
	// 점하나의 크기가 얼마인가. _iVtxSize
	// 점의 용도는 무엇인가? D3D11_USAGE _eVtxUsage 
	// -> 색을 칠할 것인지 -> 이미지를 입힐 것인지 -> 빛을 반사시킬것인지에 대한 값
	// 버텍스의 실제 내용 void* _VtxMem

	// 인덱스 버퍼 개수 UINT _iIdxCount 
	// 인덱스 버퍼의 용도가 무엇이냐? D3D11_USAGE _eIdxUsage
	// 이하 동문
	bool Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem, 
		UINT _IdxCnt, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem, DXGI_FORMAT	_IdxFm
	, D3D11_PRIMITIVE_TOPOLOGY	_eDrawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	// -> 일단 공개한다.
public:
	void draw_mode(D3D11_PRIMITIVE_TOPOLOGY _eDrawMode) { m_eDrawMode = _eDrawMode; }
	bool Create_Vertex(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem);
	bool Create_Index(UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, DXGI_FORMAT _IdxFm, void* _IdxMem);

	void Update_Vertex(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem);
	void Update_Index(UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, DXGI_FORMAT _IdxFm, void* _IdxMem);

public:
	void Update(const KUINT& _Start = 0, const KUINT& _VtxCnt = 1, KUINT* _pOff = nullptr);
	void Render(const KUINT& _Start = 0, const KUINT& _EndIdx = 1, KUINT* _pOff = nullptr);

	// 해당 버텍스, 인덱스의 정보를 (버퍼) 그리는 것 - 보내주는 것
	void Update_Pick(const KUINT& _Vtx, const KUINT& _Sub);

	void Set_VtxData(const KUINT& _BufIdx, void* _VtxTemp, const KUINT& _VtxSize);

public:
	KMesh();
	~KMesh();
};

