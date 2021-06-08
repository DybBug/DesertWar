//******************************************************************************************
//	
//	CFrustum.h : �������� ���� ��� ����.
//
//******************************************************************************************
#pragma once
#include "Common.h"

class CFrustum
{
public :
	enum PLANE
	{
		LEFT_PLANE = 0,
		RIGHT_PLANE,
		BOTTOM_PLANE,
		TOP_PLANE,
		NEAR_PLANE,
		FAR_PLANE,

		MAX_PLANE,
	};
public :
	CFrustum();
	~CFrustum();

protected :
	D3DXVECTOR4 m_vPlane[MAX_PLANE];

public :
	// �������� ����
	D3DXVECTOR4* ExtractFrustum(D3DXMATRIX& _M);
	D3DXVECTOR4 GetPlane(UINT _index) { return m_vPlane[_index]; }
};


//****************************** End of File "CFrustum.cpp" ******************************//