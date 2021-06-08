//*************************************************************************************
//	
//	CAxis.h : 축 관련 헤더 파일.		
//
//*************************************************************************************
#pragma once
#include "Common.h"


class CAxis
{
public :
	struct VTX_AXIS{
		D3DXVECTOR3 vPos;
		D3DXCOLOR Color;
	};

public :
	CAxis(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, float Length);
	~CAxis(){};

protected :
	ID3D11Device* m_pDev;
	ID3D11DeviceContext* m_pDevC;

	float m_fLength;

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vFront;
	D3DXVECTOR3 m_vHoriz;
	D3DXVECTOR3 m_vUp;

public :
	void Render(){};
};


//****************************** End of File "CAxis.h" ******************************//
