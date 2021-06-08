
//*****************************************************************************************
//	2015.01.02.
//	Scene.h : 씬관련 헤더파일.
//	모든 Scene(장면)들은 이 클래스를상속받음.
//*****************************************************************************************
#pragma once
#include "Common.h"



class CScene
{
public :
	CScene();
	virtual ~CScene();
protected :
	ID3D11Device* m_pDev;
	ID3D11DeviceContext* m_pDevC;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	IDXGISwapChain* m_pSwapChain;

protected :
public :
	virtual void Init() = 0;
	virtual void Update(float _fDTime) = 0;
	virtual void Render() = 0;

	void SetRenderTargetView(ID3D11RenderTargetView* _pRenderTargetView) {
		m_pRenderTargetView = _pRenderTargetView;
	}
	void SetDepthStencilView(ID3D11DepthStencilView* _pDepthStencilView){
		m_pDepthStencilView = _pDepthStencilView;
	}
};








//****************************** End of File "CScene.h" ******************************//