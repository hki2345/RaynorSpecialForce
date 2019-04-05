#pragma once
#include "DXContainer.h"
#include "Mof_KWindow.h"
#include "SmartPtr.h"
#include "KMacro.h"

#include <string>
#include <unordered_map>



// 본격적으로 그래픽카드의 권한 일부를 가져오는 놈이다.
// 여기서 이 구조는 어떻게 보면 Window, fmod에서 권한을 가져오는 것과 일맥상 통한다.
// 권한을 가져올 포인터 마련 -> 가져옴 -> 다른 연결된 포인터 가져옴 -> 적용
// 디바이스는 해당 권한을 불러오려는 윈도우를 알아야한다.
class KDevice : public Mof_KWindow
{
public:
	KDevice(KWindow* _Win);
	~KDevice();


	// 렌더에서 해당 디바이스의 컨텍스트를 실행한다.
public:
	friend class EUpdater;
	friend class Renderer;
	friend class RenderManager;

public:
	// 어떤식으로 보여지게 할 것이냐///
	// 앞면만 뒷면만
	// 모든면 와이어 -> 3D MAX 생각하면 됌
	// 레스터 타입
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
	// 레스터라이즈 상태 -> 와이어 프레임인지 솔리드인지 결정은
	// 렌더러가 할 수 있게 한다.
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

	// 뎁스 스텐실 -> 렌더 순서를 결정해준다.
	// + 윤곽 처리 그런 거
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
	// 윈도우 기반이기 때문에 Window가 그냥 값으로 들고 있을 것이다.
	class GCBUFFER : public SmartPtr
	{
	public:
		D3D11_BUFFER_DESC tDesc;
		ID3D11Buffer*		pConstBuffer; // 요놈이 상수 버퍼 -> D11에선 쉐이더에서 쓰일 버퍼따위도
										   // 모두 요놈 자료형으로 저장된당
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

		// 큰 데이터를 한번에 넘기려고 하면 느리다.
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
	// 마소의 프로그래밍 방식중 하나.
	// 컴객체 방식에 대해서 공부해 보세요.
	// 리소스 관련 내용과 함수들에 대한 객체

	// 컴객체란 https://docs.microsoft.com/en-us/windows/desktop/com/the-component-object-model
	// 어떤 원하는 함수를 실행하기 위해서 다른 함수에서 포인터를 입력받아 그 포인터에서 실행하는 과정
	// 이 과정은 소위 "뇌가 있는" 과정이라 하여 해당 함수를 정상적으로 실행할 수 있는지에 대한
	// 판별을 자동으로 실시하는 것이다. 실시 할 수 없을 경우 해당 포인터가 nullptr로 대입되어
	// 어떤 함수도 실행할 수 없게 되는 것이다.
	ID3D11Device*				m_pDevice;				// 최종적으로 디바이스의 장치 권한을 얻어오는 것
														// 랜더링 관련
	ID3D11DeviceContext*		m_pContext;				// 컨텍스트

														// 스왑체인에서 빠져나온 텍스처로 만든 타겟은 최종 랜더타겟으로 이용된다.
	IDXGISwapChain*				m_pSwapChain;			// 출력을 위한 스왑체인을 정의하기 위한 객체

	ID3D11Texture2D*			m_pBackBuffer;		// 깊이 스텐실용 텍스처
	ID3D11RenderTargetView*		m_pTagetView;			// 출력될 타겟 뷰

	ID3D11Texture2D*			m_pDepthStencilTex;		// 깊이 스텐실용 텍스처
	ID3D11DepthStencilView*		m_pDepthStencilView;	// 깊이 스텐실 뷰



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



	// 다이렉트 초기화를 여러번 해야 하는 경우가 생긴다. -> 초기화가 됐는데
	// 전체화면 도중 튕기거나 alt +tab을 눌러 바탕화면으로 빠져나올 경우에도
	// 디바이스를 잃어버릴 수도 있기 때문에 이러한 설정을 한다.
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

