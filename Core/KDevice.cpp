#include "KDevice.h"
#include "KWindow.h"
#include "Stl_AID.h"
#include "SmartPtr.h"
#include "KBlend.h"


KDevice::KDevice(KWindow* _Win) : Mof_KWindow(_Win),
m_pDevice(nullptr),
m_pContext(nullptr),
m_pTagetView(nullptr),
m_pDepthStencilTex(nullptr),
m_pDepthStencilView(nullptr),
m_pSwapChain(nullptr),
m_bInit(false),
m_Color(KColor::Zero)
{
}


KDevice::~KDevice()
{
	Release(); 
}

void KDevice::Release()
{
	if (nullptr != m_pDepthStencilView) { m_pDepthStencilView->Release(); }
	if (nullptr != m_pDepthStencilTex) { m_pDepthStencilTex->Release(); }
	if (nullptr != m_pBackBuffer) { m_pBackBuffer->Release(); }
	if (nullptr != m_pTagetView) { m_pTagetView->Release(); }
	if (nullptr != m_pSwapChain) { m_pSwapChain->Release(); }
	if (nullptr != m_pContext) { m_pContext->Release(); }
	if (nullptr != m_pDevice) { m_pDevice->Release(); }

	m_bInit = false;
}


bool KDevice::Init()
{

	m_bInit = false;

	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	// ����̽��� ���� ������ �˾ƺ���.
	// �����°�.
	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// D3D_DRIVER_TYPE_HARDWARE ��� �ߴٸ� 3D �ܺ� 3D��ġ�� ã�´�.
	// ����Ʈ

	if (S_OK != D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
		, nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &eLv, &m_pContext))
	{
		Release();
		return false;
	}


	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_iMSLv);

	// ��Ƽ ���ø� ������ üũ�Ѵ�.
	// �����°��̰�.

	// 0�̶�� ���� ��Ƽ���ø� ��ü�� ���� ���Ѵٴ� ���̱� ������
	// �׶��� ��Ʈ���ϴ�.
	//if (0 == m_iMSLv)
	//{
	//	return false;
	//}

	if (false == Create_SwapChain())
	{
		Release();
		return false;
	}


	if (false == Create_View())
	{
		Release();
		return false;
	}

	if (false == Create_ViewPort())
	{
		Release();
		return false;
	}


	m_bInit = true;
	return true;
}

bool KDevice::Create_SwapChain()
{
	DXGI_SWAP_CHAIN_DESC tDecs = {};

	// ������� ũ��
	tDecs.BufferDesc.Width = (UINT)kwindow()->width_st();
	tDecs.BufferDesc.Height = (UINT)kwindow()->height_st();

	// ���� ���� �ֱ�� ������ ����
	tDecs.BufferDesc.RefreshRate.Numerator = 60;
	tDecs.BufferDesc.RefreshRate.Denominator = 1;

	// ���� �Ȼ�
	tDecs.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// ��ĳ�� ���� ����
	// ��ĵ����
	tDecs.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDecs.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ������ �뵵�� ������.
	tDecs.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	tDecs.SampleDesc.Count = 1;
	tDecs.SampleDesc.Quality = 0;

	tDecs.OutputWindow = kwindow()->KHwnd();

	tDecs.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	tDecs.BufferCount = 1;

	// ��üȭ���� ������ ������?
	tDecs.Windowed = !kwindow()->IsFull();

	// �̳༮���� ���� �����ϰڽ��ϴ�.
	// ����ü���� ����� ���ؼ� ���� ��� �ϴ� �༮���̶��
	IDXGIDevice* pIdxDevice = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	m_pDevice->QueryInterface(__uuidof(pIdxDevice), (void**)&pIdxDevice);
	if (nullptr == pIdxDevice)
	{
		return false;
	}

	pIdxDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

	if (nullptr == pAdapter)
	{
		return false;
	}
	// ����� ����ü���� ����� �ִ� �ܰ��� �������̽����� �����´�.
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	if (nullptr != pFactory && S_OK != pFactory->CreateSwapChain(m_pDevice, &tDecs, &m_pSwapChain))
	{
		return false;
	}

	pFactory->Release();
	pAdapter->Release();
	pIdxDevice->Release();

	return true;

}

// ����ü�ο� ���� �ؽ�ó�� ���⼭ ����� �ش�.
bool KDevice::Create_View()
{
	m_pBackBuffer = nullptr;
	if (S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer))
	{
		return false;
	}

	// ���� �ý�ó�� ���� ������ �ϱ� ���� View�� �����.

	if (S_OK != m_pDevice->CreateRenderTargetView(m_pBackBuffer, 0, &m_pTagetView))
	{
		return false;
	}

	// pBackBufferTex->Release();

	// �ؽ�ó ���� ������ ���ټ� �ִ� �༮
	D3D11_TEXTURE2D_DESC tDecs = {};

	tDecs.ArraySize = 1;
	tDecs.Width = (UINT)kwindow()->width_st();
	tDecs.Height = (UINT)kwindow()->height_st();
	tDecs.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// ���� 24��Ʈ�� float�� ���ؼ� ���̸� üũ�ϰ�
	// ���� 8��Ʈ�� int������ ��Ʈ���� ������� ����
	// ���Ľǹ��۸� üũ�ϴ� �뵵�̴�.

	tDecs.SampleDesc.Count = 1;
	tDecs.SampleDesc.Quality = 0;

	tDecs.MipLevels = 1;

	// �⺻ �ؽ�ó �뵵�� �״�� �����ش޶�� ���ϴ°�./
	// ���Ľ� �۹���� ������ ���ڴ�.
	tDecs.Usage = D3D11_USAGE_DEFAULT;
	tDecs.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_pDevice->CreateTexture2D(&tDecs, 0, &m_pDepthStencilTex);

	if (nullptr == m_pDepthStencilTex)
	{
		return true;
	}


	if (S_OK != m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, 0, &m_pDepthStencilView))
	{
		return true;
	}

	// �̰� ������ ������ ���ΰ� ������ �ֽ��ϴ�.
	// RenderTargetView �� DepthStencilView �� ���� ��� OM �ܰ迡 �����Ѵ�.	
	// ������ɴܰ�

	// ���������͸� ��κ� ���� ������ �Ϸ��� �ϴ°�.
	m_pContext->OMSetRenderTargets(1, &m_pTagetView, m_pDepthStencilView);

	return true;
}

bool KDevice::Create_ViewPort()
{
	D3D11_VIEWPORT tDecs = {};
	tDecs.TopLeftX = 0;
	tDecs.TopLeftY = 0;

	tDecs.Width = (float)kwindow()->width_st();
	tDecs.Height = (float)kwindow()->height_st();

	tDecs.MinDepth = 0;
	tDecs.MaxDepth = 1;

	m_pContext->RSSetViewports(1, &tDecs);

	return true;
}



// ���� �׸��� ���� ������ ������.
void KDevice::Clear_Target()
{
	m_pContext->ClearRenderTargetView(m_pTagetView, m_Color.s);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
} 

// ����϶�
void KDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}

void KDevice::ResetContext() {
	m_pContext->VSSetShader(nullptr, nullptr, 0);
	m_pContext->HSSetShader(nullptr, nullptr, 0);
	m_pContext->DSSetShader(nullptr, nullptr, 0);
	m_pContext->GSSetShader(nullptr, nullptr, 0);
	m_pContext->CSSetShader(nullptr, nullptr, 0);
	m_pContext->PSSetShader(nullptr, nullptr, 0);
}

void KDevice::OMSet() 
{
	m_pContext->OMSetRenderTargets(1, &m_pTagetView, m_pDepthStencilView);
	// m_pContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
}

///////////////////////////////////////////// CB

bool KDevice::CreateCB(GCBUFFER* NewBuf) 
{
	if (S_OK != m_pDevice->CreateBuffer(&NewBuf->tDesc, nullptr, &NewBuf->pConstBuffer))
	{
		delete NewBuf;
		return false;
	}

	return true;
}

KPtr<KDevice::GCBUFFER> KDevice::FindCB(const wchar_t* _Name)
{
	return Map_Find<KPtr<KDevice::GCBUFFER>>(m_GCBMap, _Name);
}





/***************** Raster ***************/
KPtr<KDevice::RState> KDevice::Find_RS(const wchar_t* _Name)
{
	return Map_Find<KPtr<RState>>(m_RSMap, _Name);
}

void KDevice::Create_RS(const wchar_t* _Name, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode)
{
	RState* _Ptr = new RState();
	_Ptr->Create(m_pDevice, m_pContext, _FillMode, _CullMode);
	m_RSMap.insert(std::unordered_map<std::wstring, KPtr<RState>>::value_type(_Name, _Ptr));
}


void KDevice::Set_RS(const wchar_t* _Name) 
{
	KPtr<RState> RSS = Map_Find<KPtr<RState>>(m_RSMap, _Name);
	if (nullptr == RSS)
	{
		KASSERT(true);
		return;
	}
	RSS->Update();
}
void KDevice::Set_RSDef(const wchar_t* _Name)
{
	KPtr<RState> RSS = Map_Find<KPtr<RState>>(m_RSMap, _Name);
	if (nullptr == RSS)
	{
		KASSERT(true);
		return;
	}

	if (nullptr == RSS->m_pRS)
	{
		KASSERT(true);
		return;
	}
	m_RStateDef = RSS;
	m_RStateDef->Update();
	return;
}

void KDevice::Reset_RS()
{
	m_RStateDef->Update();
}

void KDevice::RState::Update()
{
	m_pContext->RSSetState(m_pRS);
}


void KDevice::RState::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode)
{
	if (nullptr == _pContext)
	{
		KASSERT(nullptr == _pContext);
		return;
	}

	m_Desc.FillMode = _FillMode;
	m_Desc.CullMode = _CullMode;
	m_pContext = _pContext;

	if (S_OK != _pDevice->CreateRasterizerState(&m_Desc, &m_pRS))
	{
		return;
	}
}


/******************** Depth Stencil ********************/
KPtr<KDevice::DSState> KDevice::Find_DSS(const wchar_t* _Name)
{
	return Map_Find<KPtr<DSState>>(m_DSSMap, _Name);
}

void KDevice::Create_DSS(const wchar_t* _Name, D3D11_DEPTH_STENCIL_DESC _Desc)
{
	DSState* TPtr = new DSState();
	TPtr->Name = _Name;
	TPtr->Create(m_pDevice, m_pContext, _Desc);
	m_DSSMap.insert(std::unordered_map<std::wstring, KPtr<DSState>>::value_type(_Name, TPtr));
}

void KDevice::Set_DSSDef(const wchar_t* _Name)
{
	KPtr<DSState> DSS = Map_Find<KPtr<DSState>>(m_DSSMap, _Name);
	if (nullptr == DSS)
	{
		KASSERT(true);
		return;
	}

	if (nullptr == DSS->m_pDS)
	{
		KASSERT(true);
		return;
	}
	m_DSStateDef = DSS;
	m_DSStateDef->Update();
	return;
}

std::wstring KDevice::ds_name()
{
	if (nullptr != m_DSStateCur)
	{
		return m_DSStateCur->Name;
	}

	return L"";
}


void KDevice::Reset_DSS()
{
	if (nullptr == m_DSStateDef)
	{
		m_DSStateDef->Update();
	}
}

void KDevice::DSState::Update(unsigned int _Ref)
{
	// �̰� ���� ���Ľ� ��
	m_pContext->OMSetDepthStencilState(m_pDS, _Ref);
}

void KDevice::DSState::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, D3D11_DEPTH_STENCIL_DESC _Desc)
{
	if (nullptr == _pContext)
	{
		KASSERT(nullptr == _pContext);
		return;
	}

	m_pContext = _pContext;

	if (S_OK != _pDevice->CreateDepthStencilState(&_Desc, &m_pDS))
	{
		return;
	}
}

void KDevice::Set_DSS(const wchar_t* _Name, unsigned int _Ref)
{
	KPtr<DSState> DSS = Map_Find<KPtr<DSState>>(m_DSSMap, _Name);
	if (nullptr == DSS)
	{
		KASSERT(true);
		return;
	}
	DSS->Update(_Ref);
	m_DSStateCur = DSS;
}

void KDevice::Set_BS(const wchar_t* _Name) 
{
	KPtr<KBlend> Ptr = ResourceManager<KBlend>::Find(_Name);

	if (nullptr == Ptr)
	{
		BBY;
	}
	Ptr->Update();
}

bool KDevice::Create_DefFilter()
{
	return true;
}