//**********************************************************************************
//	2015.01.02.
//	Common.h : ...
//**********************************************************************************
#pragma once
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <windows.h>
#include <string>
#include <d3dx11.h>
#include <D3DX10math.h>
#include <vector>
#include <list>
#include <assert.h>
#include "d3dx11Effect.h"
#include <fstream>
#include <time.h>

#include <fbxsdk.h>

using namespace std;

#define SAFE_RELEASE(x) { if (x != NULL) x->Release(); x = NULL; }
#define SAFE_DELETE(x) { if(x != NULL) delete[] x; x = NULL; }

#define FPS 60.0f

//-------------------------------------------------------------
// 색상.
//-------------------------------------------------------------
namespace Colors{
	const D3DXCOLOR WHITE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	const D3DXCOLOR BLACK = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR RED = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR GREEN = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	const D3DXCOLOR BLUE = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	const D3DXCOLOR YELLOW = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	const D3DXCOLOR CYAN = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	const D3DXCOLOR MAGENTA = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

	const D3DXCOLOR SILVER = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);
	const D3DXCOLOR LIGHT_BLUE = D3DXCOLOR(0.69f, 0.77f, 0.87f, 1.0f);
}
//--------------------------------------------------------------------------------------
// 빛, 재질, 안개 구조체
//--------------------------------------------------------------------------------------
enum LIGHT_TYPE{
	LIGHT_TYPE_DIRECTION = 1,
	LIGHT_TYPE_POINT,
	LIGHT_TYPE_SPOT,
};

struct LIGHT
{
	D3DXCOLOR s_cAmbient;
	D3DXCOLOR s_cDiffuse;
	D3DXCOLOR s_cSpecular;

	D3DXVECTOR3 s_vDirection;
	float s_fRange;

	D3DXVECTOR3 s_vPosition;
	float s_fSpot;

	D3DXVECTOR3 s_vAttenuation;
	float s_fType;
	
	LIGHT() : s_cAmbient(1.0f, 1.0f, 1.0f, 1.0f), s_cDiffuse(1.0f, 1.0f, 1.0f, 1.0f),
		s_cSpecular(1.0f, 1.0f, 1.0f, 1.0f), s_vDirection(0.0f, 0.0f, 0.0f),
		s_fRange(0.0f), s_vPosition(0.0f, 0.0f, 0.0f), s_fSpot(0.0f),
		s_vAttenuation(0.0f, 0.0f, 0.0f), s_fType(0.0f){};
};

struct MATERIAL
{
	D3DXCOLOR s_cAmbient;
	D3DXCOLOR s_cDiffuse;
	D3DXCOLOR s_cSpecular;
	D3DXCOLOR s_cReflect;

	MATERIAL() : s_cAmbient(1.0f, 1.0f, 1.0f, 1.0f), s_cDiffuse(1.0f, 1.0f, 1.0f, 1.0f),
		s_cSpecular(1.0f, 1.0f, 1.0f, 1.0f), s_cReflect(1.0f, 1.0f, 1.0f, 1.0f){};
};

struct FOG
{
	D3DXCOLOR s_cColor;
	float s_fStart;
	float s_fRange;
	float s_fEnable;

	FOG() : s_cColor(1.0f, 1.0f, 1.0f, 1.0f), s_fStart(0.0f), s_fRange(0.0f), s_fEnable(false){};
};

//--------------------------------------------------------------------------------------
// 정점 및 인덱스 구조체.
//--------------------------------------------------------------------------------------
// [ 이동 ] : CShader.h 파일의 CBasicShader클래스로 이동.
//struct VERTEX
//{
//	D3DXVECTOR3 s_vPosition;
//	D3DXVECTOR3 s_vNormal;
//	D3DXVECTOR2 s_vUV;
//
//	VERTEX(){};
//	VERTEX(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vNormal, D3DXVECTOR2 _vUV)
//		: s_vPosition(_vPos), s_vNormal(_vNormal), s_vUV(_vUV){};
//};

struct INDEX
{
	UINT s_iIndex[3];
	INDEX(){};
	INDEX(UINT _0, UINT _1, UINT _2){ s_iIndex[0] = _0, s_iIndex[1] = _1; s_iIndex[2] = _2; }
};



//****************************** End of File "Common.h" ******************************//