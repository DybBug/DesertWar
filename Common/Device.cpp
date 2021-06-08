//**********************************************************************************
//	2015.01.02.
//	Device.cpp : 디바이스 관련 소스 파일.
//**********************************************************************************
#include "Device.h"
#include "RenderState.h"
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////////
//	Device()
Device::Device()
:	m_pDev(0),
	m_pDevC(0),
	m_pSwapChain(0),
	m_pDepthStencilBuffer(0),
	m_pDepthStencilView(0),
	m_pRenderTargetView(0),

	m_DriverType(D3D_DRIVER_TYPE_HARDWARE),
	m_bEnable4xMSAA(false),
	m_n4xMSAAQuality(0),
	m_nClientWidth(0), m_nClientHeight(0)
{

}




////////////////////////////////////////////////////////////////////////////////////
//	~Device();
Device::~Device()
{
	ResterRizerState::ReleaseAll();

	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDepthStencilBuffer);
	if (m_pDevC) m_pDevC->ClearState();

	SAFE_RELEASE(m_pDevC);
	SAFE_RELEASE(m_pDev);
}





////////////////////////////////////////////////////////////////////////////////////
//	Init() : DirectX11 초기화
bool Device::Init(HWND _hWnd, int _nWidth, int _nHeight)
{
	m_nClientWidth = _nWidth;
	m_nClientHeight = _nHeight;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(0,
		m_DriverType,
		0,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&m_pDev,
		&featureLevel,
		&m_pDevC);
	/*if (FAILED(hr)){
		MessageBox(0, L"DirectX11 CreateDevice Failed.", 0, 0);
		return false;
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0){
		MessageBox(0, L"Direct3D feature Level 11 unsupported..", 0, 0);
		return false;
	}*/

	//-------------------------------------------------
	// 4xMSAA Quality Level Check
	//-------------------------------------------------
	m_pDev->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_n4xMSAAQuality);
	assert(m_n4xMSAAQuality > 0);

	//----------------------------------------------
	// SwapChain 설정.
	//----------------------------------------------
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_nClientWidth;
	sd.BufferDesc.Height = m_nClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (m_bEnable4xMSAA){
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_n4xMSAAQuality - 1;
	}
	else{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = _hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	//----------------------------------------
	//	SwapChain 생성.
	//----------------------------------------
	IDXGIDevice* pDXGIDevice = 0;
	m_pDev->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);

	IDXGIAdapter* pDXGIAdapter = 0;
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);

	IDXGIFactory* pDXGIFactory = 0;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);

	pDXGIFactory->CreateSwapChain(m_pDev, &sd, &m_pSwapChain);

	SAFE_RELEASE(pDXGIDevice);
	SAFE_RELEASE(pDXGIAdapter);
	SAFE_RELEASE(pDXGIFactory);

	Reset(_hWnd, _nWidth, _nHeight);

	ResterRizerState::InitAll(m_pDev);

	return true;


}




////////////////////////////////////////////////////////////////////////////////////
//	Reset() : DirectX11 재설정.
bool Device::Reset(HWND _hWnd, int _nWidth, int _nHeight)
{
	m_nClientWidth = _nWidth;
	m_nClientHeight = _nHeight;

	//-----------------------------------------------------------
	//
	// BackBuffer, Depth/Stencil Buffer, Viewport 재설정.
	//
	//-----------------------------------------------------------
	//assert(m_pDev);
	//assert(m_pDevC);
	//assert(m_pSwapChain);

	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilBuffer);

	//-----------------------------------------------------------
	//	BackBuffer(RenderTargetView)생성.
	//-----------------------------------------------------------
	m_pSwapChain->ResizeBuffers(1, m_nClientWidth, m_nClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	m_pDev->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	//-----------------------------------------------------------
	// Depth/Stencil Buffer, View 설정 및 생성.
	//-----------------------------------------------------------
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_nClientWidth;
	depthStencilDesc.Height = m_nClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//	4xMSAA 설정.
	if (m_bEnable4xMSAA){
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_n4xMSAAQuality - 1;
	}
	else{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;	// 텍스처의 용도.
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	m_pDev->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer);
	m_pDev->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView);

	//--------------------------------------------------------------
	// 백버퍼(RenderTargetView)와 깊이/스텐실(Depth/StencilView) 연결.
	//--------------------------------------------------------------
	m_pDevC->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//--------------------------------------------------------------
	// 뷰포트 설정 및 연결.
	//--------------------------------------------------------------
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width = static_cast<float>(m_nClientWidth);
	m_ScreenViewport.Height = static_cast<float>(m_nClientHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;

	m_pDevC->RSSetViewports(1, &m_ScreenViewport);

	return true;
}



//****************************** End of File "Device.cpp" ******************************//