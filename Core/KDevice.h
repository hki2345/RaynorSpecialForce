#pragma once
#include "DXContainer.h"
#include "Mof_KWindow.h"
#include "SmartPtr.h"
#include "KMacro.h"

#include <string>
#include <unordered_map>



// ���������� �׷���ī���� ���� �Ϻθ� �������� ���̴�.
// ���⼭ �� ������ ��� ���� Window, fmod���� ������ �������� �Ͱ� �ϸƻ� ���Ѵ�.
// ������ ������ ������ ���� -> ������ -> �ٸ� ����� ������ ������ -> ����
// ����̽��� �ش� ������ �ҷ������� �����츦 �˾ƾ��Ѵ�.
class KDevice : public Mof_KWindow
{
public:
	KDevice(KWindow* _Win);
	~KDevice();


	// �������� �ش� ����̽��� ���ؽ�Ʈ�� �����Ѵ�.
public:
	friend class EUpdater;
	friend class Renderer;
	friend class RenderManager;

public:
	// ������� �������� �� ���̳�///
	// �ո鸸 �޸鸸
	// ���� ���̾� -> 3D MAX �����ϸ� ��
	// ������ Ÿ��
	enum RS_TYPE
	{
		RT_FRONT,
		RT_BACK,
		RT_NONE,
		RT_WIREFRAME,
		END,
	};


private:
	/***************** Rasterize ****************/
	// �����Ͷ����� ���� -> ���̾� ���������� �ָ������� ������
	// �������� �� �� �ְ� �Ѵ�.
	class RState : public SmartPtr
	{
	public:
		ID3D11DeviceContext* m_pContext;
		D3D11_RASTERIZER_DESC m_Desc;
		ID3D11RasterizerState* m_pRS;

	public:
		void Update();
		void Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode);

	public:
		~RState()
		{
			if (nullptr != m_pRS)
			{
				m_pRS->Release();
			}
		}
	};

private:
	KPtr<RState> m_RStateDef;
	std::unordered_map<std::wstring, KPtr<RState>> m_RSMap;

private:
	KPtr<RState> Find_RS(const wchar_t* _Name);

public:
	void Create_RS(const wchar_t* _Name, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode);
	void Set_RSDef(const wchar_t* _Name);
	void Set_RS(const wchar_t* _Name);
	void Reset_RS();

private:

	/***************** Depth Stencil ****************/

	// ���� ���ٽ� -> ���� ������ �������ش�.
	// + ���� ó�� �׷� ��
	class DSState : public SmartPtr
	{
	public:
		ID3D11DeviceContext* m_pContext;
		D3D11_DEPTH_STENCIL_DESC m_Desc;
		ID3D11DepthStencilState* m_pDS;

		std::wstring Name;

	public:
		void Update(unsigned int _Ref = 0);
		void Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, D3D11_DEPTH_STENCIL_DESC _Desc);

	public:
		~DSState()
		{
			if (nullptr != m_pDS)
			{
				m_pDS->Release();
			}
		}
	};

private:
	KPtr<DSState> m_DSStateDef;
	KPtr<DSState> m_DSStateCur;
	std::unordered_map<std::wstring, KPtr<DSState>> m_DSSMap;

private:
	KPtr<DSState> Find_DSS(const wchar_t* _Name);


public:
	void Create_DSS(const wchar_t* _Name, D3D11_DEPTH_STENCIL_DESC _Desc);
	void Reset_DSS();
	void Set_DSS(const wchar_t* _Name, unsigned int _Ref = 0);
	void Set_DSSDef(const wchar_t* _Name);
	std::wstring ds_name();

	
private:
	/********** Const Buffer ************/
	// ������ ����̱� ������ Window�� �׳� ������ ��� ���� ���̴�.
	class GCBUFFER : public SmartPtr
	{
	public:
		D3D11_BUFFER_DESC tDesc;
		ID3D11Buffer*		pConstBuffer; // ����� ��� ���� -> D11���� ���̴����� ���� ���۵�����
										   // ��� ��� �ڷ������� ����ȴ�
		KUINT			iReg;

		~GCBUFFER()
		{
			if (nullptr != pConstBuffer) { pConstBuffer->Release(); };
		}
	};

private:
	std::unordered_map<std::wstring, KPtr<GCBUFFER>> m_GCBMap;

public:
	template<typename BUFTYPE>
	bool CreateCB(const wchar_t* _Name, D3D11_USAGE _eUsage, UINT _iReg)
	{
		GCBUFFER* NewBuf = new GCBUFFER();

		NewBuf->tDesc.ByteWidth = sizeof(BUFTYPE);
		NewBuf->tDesc.Usage = _eUsage;
		NewBuf->tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (NewBuf->tDesc.Usage == D3D11_USAGE_DYNAMIC)
		{
			NewBuf->tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		if (false == CreateCB(NewBuf))
		{
			return false;
		}

		NewBuf->iReg = _iReg;

		m_GCBMap.insert(std::unordered_map<std::wstring, KPtr<GCBUFFER>>::value_type(_Name, NewBuf));

		return true;
	}

	template<typename BUFTYPE>
	void SettingCB(const wchar_t* _Name, const BUFTYPE& _Data, SHTYPE _eType)
	{
		KPtr<GCBUFFER> Buf = FindCB(_Name);

		if (nullptr == Buf || Buf->tDesc.ByteWidth != sizeof(BUFTYPE))
		{
			return;
		}

		// ū �����͸� �ѹ��� �ѱ���� �ϸ� ������.
		D3D11_MAPPED_SUBRESOURCE tSub = {};
		m_pContext->Map(Buf->pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
		memcpy(tSub.pData, &_Data, sizeof(BUFTYPE));
		m_pContext->Unmap(Buf->pConstBuffer, 0);

		m_pContext->VSSetConstantBuffers(Buf->iReg, 1, &Buf->pConstBuffer);
		m_pContext->PSSetConstantBuffers(Buf->iReg, 1, &Buf->pConstBuffer);
		m_pContext->DSSetConstantBuffers(Buf->iReg, 1, &Buf->pConstBuffer);
		m_pContext->HSSetConstantBuffers(Buf->iReg, 1, &Buf->pConstBuffer);
		m_pContext->GSSetConstantBuffers(Buf->iReg, 1, &Buf->pConstBuffer);
	}

	bool CreateCB(GCBUFFER* NewBuf);
	KPtr<GCBUFFER> FindCB(const wchar_t* _Name);



private:
	// ������ ���α׷��� ����� �ϳ�.
	// �İ�ü ��Ŀ� ���ؼ� ������ ������.
	// ���ҽ� ���� ����� �Լ��鿡 ���� ��ü

	// �İ�ü�� https://docs.microsoft.com/en-us/windows/desktop/com/the-component-object-model
	// � ���ϴ� �Լ��� �����ϱ� ���ؼ� �ٸ� �Լ����� �����͸� �Է¹޾� �� �����Ϳ��� �����ϴ� ����
	// �� ������ ���� "���� �ִ�" �����̶� �Ͽ� �ش� �Լ��� ���������� ������ �� �ִ����� ����
	// �Ǻ��� �ڵ����� �ǽ��ϴ� ���̴�. �ǽ� �� �� ���� ��� �ش� �����Ͱ� nullptr�� ���ԵǾ�
	// � �Լ��� ������ �� ���� �Ǵ� ���̴�.
	ID3D11Device*				m_pDevice;				// ���������� ����̽��� ��ġ ������ ������ ��
														// ������ ����
	ID3D11DeviceContext*		m_pContext;				// ���ؽ�Ʈ

														// ����ü�ο��� �������� �ؽ�ó�� ���� Ÿ���� ���� ����Ÿ������ �̿�ȴ�.
	IDXGISwapChain*				m_pSwapChain;			// ����� ���� ����ü���� �����ϱ� ���� ��ü

	ID3D11Texture2D*			m_pBackBuffer;		// ���� ���ٽǿ� �ؽ�ó
	ID3D11RenderTargetView*		m_pTagetView;			// ��µ� Ÿ�� ��

	ID3D11Texture2D*			m_pDepthStencilTex;		// ���� ���ٽǿ� �ؽ�ó
	ID3D11DepthStencilView*		m_pDepthStencilView;	// ���� ���ٽ� ��



	UINT						m_iMSLv;
	KColor						m_Color;
	bool						m_bInit;


public:
	ID3D11Device*				PDevice() { return m_pDevice; };
	ID3D11DeviceContext*		Context() { return m_pContext; };
	ID3D11DepthStencilView*		Depth() { return m_pDepthStencilView; };



public:
	void Set_BS(const wchar_t* _Name);

public:
	bool Create_SwapChain();
	bool Create_ViewPort();
	bool Create_View();



	// ���̷�Ʈ �ʱ�ȭ�� ������ �ؾ� �ϴ� ��찡 �����. -> �ʱ�ȭ�� �ƴµ�
	// ��üȭ�� ���� ƨ��ų� alt +tab�� ���� ����ȭ������ �������� ��쿡��
	// ����̽��� �Ҿ���� ���� �ֱ� ������ �̷��� ������ �Ѵ�.
	bool IsInit() {
		return m_bInit;
	}


	bool Init();
	bool Init_BasicFigure2D();
	bool Init_BasicFigure3D();
	
	bool DefRenderTaget();
	bool Def3DCreate();
	bool Mesh3DCreate();
	bool Mat3DCreate();
	bool Create_DefFilter();

public:
	void Release();
	void ResetContext();
	void OMSet();

private:
	void Clear_Target();
	void Present();
};

