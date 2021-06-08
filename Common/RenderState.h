//***************************************************************************************
//
//	RenderState.h : 랜더 스테이트관련 헤더 파일.
//
//***************************************************************************************


#pragma once
#include "Common.h"

class ResterRizerState
{
public :
	static void InitAll(ID3D11Device* _pDev);
	static void ReleaseAll();

	static ID3D11RasterizerState* m_pSolidRS;
	static ID3D11RasterizerState* m_pWireframeRS;
};


/****************************** End of File "RenderState.h" ******************************/