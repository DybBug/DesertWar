//**********************************************************************************
//	2015.01.02.
//	Device.h : 디바이스 관련 헤더 파일.
//	-> DirectX11 관련 클래스.
//**********************************************************************************
#pragma once
#include "Common.h"

class Device
{
public :
	Device();
	virtual ~Device();

protected :
	ID3D11Device*			m_pDev;
	ID3D11DeviceContext*	m_pDevC;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11Texture2D*		m_pDepthStencilBuffer;
	ID3D11RenderTargetView*	m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_VIEWPORT			m_ScreenViewport;

	D3D_DRIVER_TYPE m_DriverType;

	int m_nClientWidth;
	int m_nClientHeight;

	UINT m_n4xMSAAQuality;
	bool m_bEnable4xMSAA;
protected :
	//void _FontInit();

public :
	bool Init(HWND _hWnd, int _nWidth, int _nHeight);	// 디바이스(DirectX11)초기화 함수.
	bool Reset(HWND _hWnd, int _nWidth, int _nHeight);	// 디바이스(DirectX11)재설정 함수.
public :
	void SetClientSize(int _nWidth, int _nHeight) { m_nClientWidth = _nWidth,
													m_nClientHeight = _nHeight;
												}
	void GetClientSize(int* _pWidth, int* _pHeight) { *_pWidth = m_nClientWidth,
													  *_pHeight = m_nClientHeight;
												}
	ID3D11Device* GetDevice()						{ return m_pDev; }
	ID3D11DeviceContext* GetDeviceContext()			{ return m_pDevC; }
	ID3D11RenderTargetView* GetRenderTargetView()	{ return m_pRenderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView()   { return m_pDepthStencilView; }
	IDXGISwapChain* GetSwapChain()					{ return m_pSwapChain; }
	D3D11_VIEWPORT* GetViewPort()					{ return &m_ScreenViewport; }
};



//****************************** End of File "Device.h" ******************************//