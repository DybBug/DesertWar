//******************************************************************************************
//	
//	CFrustum.cpp : 프러스텀 관련 소스  파일.
//
//******************************************************************************************
#include "CFrustum.h"




/////////////////////////////////////////////////////////////////////////////////////////////
// CFrustum()
CFrustum::CFrustum()
{

}




/////////////////////////////////////////////////////////////////////////////////////////////
// ~CFrustum()
CFrustum::~CFrustum()
{

}




/////////////////////////////////////////////////////////////////////////////////////////////
// ExtractFrustum()
D3DXVECTOR4* CFrustum::ExtractFrustum(D3DXMATRIX& _M)
{
	D3DXPLANE Plane[MAX_PLANE];
	// 왼쪽 평면.
	Plane[LEFT_PLANE].a = _M(0, 3) + _M(0, 0);
	Plane[LEFT_PLANE].b = _M(1, 3) + _M(1, 0);
	Plane[LEFT_PLANE].c = _M(2, 3) + _M(2, 0);
	Plane[LEFT_PLANE].d = _M(3, 3) + _M(3, 0);

	// 오른쪽 평면.
	Plane[RIGHT_PLANE].a = _M(0, 3) - _M(0, 0);
	Plane[RIGHT_PLANE].b = _M(1, 3) - _M(1, 0);
	Plane[RIGHT_PLANE].c = _M(2, 3) - _M(2, 0);
	Plane[RIGHT_PLANE].d = _M(3, 3) - _M(3, 0);

	// 아래쪽 평면.
	Plane[BOTTOM_PLANE].a = _M(0, 3) + _M(0, 1);
	Plane[BOTTOM_PLANE].b = _M(1, 3) + _M(1, 1);
	Plane[BOTTOM_PLANE].c = _M(2, 3) + _M(2, 1);
	Plane[BOTTOM_PLANE].d = _M(3, 3) + _M(3, 1);

	// 위쪽 평면.
	Plane[TOP_PLANE].a = _M(0, 3) - _M(0, 1);
	Plane[TOP_PLANE].b = _M(1, 3) - _M(1, 1);
	Plane[TOP_PLANE].c = _M(2, 3) - _M(2, 1);
	Plane[TOP_PLANE].d = _M(3, 3) - _M(3, 1);

	// 근평면.
	Plane[NEAR_PLANE].a = _M(0, 2);
	Plane[NEAR_PLANE].b = _M(1, 2);
	Plane[NEAR_PLANE].c = _M(2, 2);
	Plane[NEAR_PLANE].d = _M(3, 2);

	// 원평면.
	Plane[FAR_PLANE].a = _M(0, 3) - _M(0, 2);
	Plane[FAR_PLANE].b = _M(1, 3) - _M(1, 2);
	Plane[FAR_PLANE].c = _M(2, 3) - _M(2, 2);
	Plane[FAR_PLANE].d = _M(3, 3) - _M(3, 2);


	for (int i = 0; i < MAX_PLANE; ++i){
		D3DXPlaneNormalize(&Plane[i], &Plane[i]);
		m_vPlane[i].x = Plane[i].a;
		m_vPlane[i].y = Plane[i].b;
		m_vPlane[i].z = Plane[i].c;
		m_vPlane[i].w = Plane[i].d;
	}

	return m_vPlane;
}



//****************************** End of File "CFrustum.cpp" ******************************//