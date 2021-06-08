//***************************************************************************************
//
//	RenderState.cpp : 랜더 스테이트관련 소스 파일.
//
//***************************************************************************************


#include "RenderState.h"

ID3D11RasterizerState* ResterRizerState::m_pSolidRS = 0;
ID3D11RasterizerState* ResterRizerState::m_pWireframeRS = 0;


void ResterRizerState::InitAll(ID3D11Device* _pDev)
{
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	_pDev->CreateRasterizerState(&solidDesc, &m_pSolidRS);

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	_pDev->CreateRasterizerState(&wireframeDesc, &m_pWireframeRS);

}




void ResterRizerState::ReleaseAll()
{
	if (m_pSolidRS) m_pSolidRS->Release();
	if (m_pWireframeRS) m_pWireframeRS->Release();
}




/****************************** End of File "RenderState.cpp" ******************************/