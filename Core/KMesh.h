#pragma once
#include "Resource.h"
#include "DXContainer.h"
#include <vector>

class Buffer_Vertex : public SmartPtr
{
public:
	ID3D11Buffer*				m_VertBuf; // ���ؽ� �����Դϴ�.
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
	ID3D11Buffer*				m_IdxBuf; // ���ؽ� �����Դϴ�.
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
	// �̷��� �޽��� ��Ÿ������ ���۵� �ڷ����� ���� ���ѹ��ȴ�. (KDevice ��� ���� ����)
	//ID3D11Buffer*				m_InxBuf; // �ε��� ����
	//DXGI_FORMAT					m_IdxFormat; // �ε��� ���� ����
	
	D3D11_PRIMITIVE_TOPOLOGY	m_eDrawMode; 
	
	// 18.10.25 ���� �ƿ� �������� ��� �ְ� �� �����Ѵ�
	std::vector<ID3D11Buffer*>			VertBuff_Vec;
	std::vector<KUINT>					VertSize_Vec;
	std::vector<KPtr<Buffer_Vertex>>	VertBuffInfo_Vec;
	std::vector<KPtr<Buffer_Index>>		IndexBuffInfo_Vec;

public:
	// ���� ��ΰ�? UINT _iVtxCount
	// ���ϳ��� ũ�Ⱑ ���ΰ�. _iVtxSize
	// ���� �뵵�� �����ΰ�? D3D11_USAGE _eVtxUsage 
	// -> ���� ĥ�� ������ -> �̹����� ���� ������ -> ���� �ݻ��ų�������� ���� ��
	// ���ؽ��� ���� ���� void* _VtxMem

	// �ε��� ���� ���� UINT _iIdxCount 
	// �ε��� ������ �뵵�� �����̳�? D3D11_USAGE _eIdxUsage
	// ���� ����
	bool Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem, 
		UINT _IdxCnt, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem, DXGI_FORMAT	_IdxFm
	, D3D11_PRIMITIVE_TOPOLOGY	_eDrawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	// -> �ϴ� �����Ѵ�.
public:
	void draw_mode(D3D11_PRIMITIVE_TOPOLOGY _eDrawMode) { m_eDrawMode = _eDrawMode; }
	bool Create_Vertex(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem);
	bool Create_Index(UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, DXGI_FORMAT _IdxFm, void* _IdxMem);

	void Update_Vertex(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem);
	void Update_Index(UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, DXGI_FORMAT _IdxFm, void* _IdxMem);

public:
	void Update(const KUINT& _Start = 0, const KUINT& _VtxCnt = 1, KUINT* _pOff = nullptr);
	void Render(const KUINT& _Start = 0, const KUINT& _EndIdx = 1, KUINT* _pOff = nullptr);

	// �ش� ���ؽ�, �ε����� ������ (����) �׸��� �� - �����ִ� ��
	void Update_Pick(const KUINT& _Vtx, const KUINT& _Sub);

	void Set_VtxData(const KUINT& _BufIdx, void* _VtxTemp, const KUINT& _VtxSize);

public:
	KMesh();
	~KMesh();
};

