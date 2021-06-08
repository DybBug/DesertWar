//**********************************************************************************
//	2015.01.02.
//	CShader.h : 쉐이더 관련 헤더 파일.
//**********************************************************************************
#pragma once
#include "Common.h"
#include "CCamera.h"
#include "CFrustum.h"

#define LIGHT_MAX 30
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CShader : 쉐이더 기본 클래스. : 다른 쉐이더 코드는 이 클래스에서 파생된다.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class CShader
{
public :
	CShader();
	virtual ~CShader();

protected :
	LPCWSTR m_pFileName;
	ID3D11Device* m_pDev;
	ID3D11DeviceContext* m_pDevC;
	ID3DX11Effect* m_pFX;
	ID3D11InputLayout* m_pInputLayout;
	ID3DX11EffectTechnique* m_pTech;
protected :
	void _BuildFX();
	void _BuildVertexLayout(D3D11_INPUT_ELEMENT_DESC* _pVtxDesc, int _nDescSize);

	//=====================================================================
	// 1. 입력 조립기 설정
	// 2. _BuildFX() : FX 생성.
	// 3. _BuildVertexLayout() : 입력 조립기 생성.
	// 4. 각 상수 버퍼 주소 가져오기.
	//=====================================================================
	virtual void _Init();
public :
	void Create(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC,
						LPCWSTR _pFileName);

	virtual void SettingDataToGPU(UINT _iPassNum = 0) = 0;

	virtual void SetWorld(D3DXMATRIX* _pMatrix){};
	virtual void SetCamera(CCamera* _pCamera){};
	virtual void SetLight(UINT _iIndex, LIGHT* _pLight){};
	virtual void SetMaterial(MATERIAL* _pMaterial){};
	virtual void SetTexture(ID3D11ShaderResourceView** _ppShaderResourceView, D3DXVECTOR4* _pTexTranform = NULL){};
	virtual void SetShadowTexture(ID3D11ShaderResourceView** _ppShaderResourceView){};
	virtual void SetColor(D3DXCOLOR* _col){};

	virtual void EnableLight(UINT _iIndex, bool _bEnable){};
	virtual void SetFrustum(CFrustum* _pFrustum) {};
	virtual void GetLight(UINT _iIndex, LIGHT* _pLight) {};

	virtual void SetView(D3DXMATRIX _mView) {};
	virtual void SetProj(D3DXMATRIX _mProj) {};
	virtual void SetShadow(D3DXMATRIX _mShadow) {};

};

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CBasicShader : 기본 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
enum{
	PASS_LIGHTING = 0,
	PASS_LIGHTING_TEXTURE,
	PASS_LIGHTING_TEXTURE_ALPHACLIP,
};

class CBasicShader : public CShader
{
public : 
	struct VERTEX
	{
		D3DXVECTOR3 s_vPosition;
		D3DXVECTOR3 s_vNormal;
		D3DXVECTOR2 s_vUV;

		VERTEX(){};
		VERTEX(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vNormal, D3DXVECTOR2 _vUV)
			: s_vPosition(_vPos), s_vNormal(_vNormal), s_vUV(_vUV){};
	};

public :
	CBasicShader();
	virtual ~CBasicShader();
protected :
	ID3DX11EffectMatrixVariable* m_pfxmWorld;
	ID3DX11EffectMatrixVariable* m_pfxmView;
	ID3DX11EffectMatrixVariable* m_pfxmProj;
	ID3DX11EffectMatrixVariable* m_pfxmVP;
	ID3DX11EffectMatrixVariable* m_pfxmWVP;
	ID3DX11EffectMatrixVariable* m_pfxmShadow;	// [추가] 2015.08.14. 그림자 행렬fx.
	ID3DX11EffectVectorVariable* m_pfxvEyeL;
	ID3DX11EffectVectorVariable* m_pfxvTexTranform;
	

	ID3DX11EffectVariable* m_pfxLight;
	ID3DX11EffectVariable* m_pfxMaterial;

	ID3DX11EffectShaderResourceVariable* m_pfxDiffuseMap;
	ID3DX11EffectShaderResourceVariable* m_pfxShadowMap;

	D3DXMATRIX m_mWorld;
	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;
	D3DXMATRIX m_mVP;
	D3DXMATRIX m_mWVP;
	D3DXMATRIX m_mShadow;	// [추가] 2015.08.14. 그림자 행렬.

	CCamera* m_pCamera;
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR4 m_vTexTransform;

	LIGHT m_Light[LIGHT_MAX];
	MATERIAL m_Material;
	ID3D11ShaderResourceView* m_pDiffusMapSRV;
	ID3D11ShaderResourceView* m_pShadowMapSRV;
protected :
	virtual void _Init();
	virtual void _TransformLight(D3DXMATRIX* _pInvWorld);	
public :
	// 설정한 데이터를 GPU에게 전달해주는 함수.
	virtual void SettingDataToGPU(UINT _iPassNum = 0);

	virtual void SetWorld(D3DXMATRIX* _pMatrix);
	virtual void SetCamera(CCamera* _pCamera);
	virtual void SetLight(UINT _iIndex, LIGHT* _pLight);
	virtual void SetMaterial(MATERIAL* _pMaterial);
	virtual void SetTexture(ID3D11ShaderResourceView** _ppShaderResourceView, D3DXVECTOR4* _pTexTranform = NULL);
	virtual void SetShadowTexture(ID3D11ShaderResourceView** _ppShaderResourceView);

	virtual void EnableLight(UINT _iIndex, bool _bEnable);
	virtual void GetLight(UINT _iIndex, LIGHT* _pLight){ *_pLight = m_Light[_iIndex]; }
	
	virtual void SetView(D3DXMATRIX _mView){ m_mView = _mView; }
	virtual void SetProj(D3DXMATRIX _mProj) { m_mProj = _mProj; }
	virtual void SetShadow(D3DXMATRIX _mShadow) { m_mShadow = _mShadow; }
};




////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CFontShader : 폰트 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

class CFontShader : public CShader
{
public :
	struct VERTEX
	{
		D3DXVECTOR3 s_vPosition;
		D3DXVECTOR2 s_vUV;
	};

public :
	CFontShader();
	virtual ~CFontShader();
protected :
	ID3DX11EffectMatrixVariable* m_pfxmWorld;
	ID3DX11EffectMatrixVariable* m_pfxmView;
	ID3DX11EffectMatrixVariable* m_pfxmProj;
	ID3DX11EffectVectorVariable* m_pfxcFontColor;
	ID3DX11EffectShaderResourceVariable* m_pfxFontTexture;


	D3DXMATRIX m_mWorld;
	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;

	D3DXCOLOR m_cFontColor;

	ID3D11ShaderResourceView* m_pFontTextureSRV;

	CCamera* m_pCamera;
protected :
	virtual void _Init();
public :
	virtual void SettingDataToGPU(UINT _iPassNum = 0);

	virtual void SetWorld(D3DXMATRIX* _mWorld)						{ m_mWorld = *_mWorld; }
	virtual void SetColor(D3DXCOLOR* _cColor)						{ m_cFontColor = *_cColor; }
	virtual void SetCamera(CCamera* _pCamera)						{ m_pCamera = _pCamera; }
	virtual void SetTexture(ID3D11ShaderResourceView** _pTextureSRV) { m_pFontTextureSRV = *_pTextureSRV; }
	
	D3DXMATRIX GetWorld() { return m_mWorld; }
	D3DXMATRIX GetView() { return m_mView; }
	D3DXMATRIX GetProj() { return m_mProj; }
	D3DXCOLOR GetColor() { return m_cFontColor; }
	CCamera* GetCamera() { return m_pCamera; }
};




////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CNonLightingShader : 정점 색과 위치를 이용한 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class CNonLightingShader : public CShader
{
public : 
	struct VERTEX
	{
		D3DXVECTOR3 s_vPosition;
	};
public :
	CNonLightingShader();
	virtual ~CNonLightingShader();
protected :
	virtual void _Init();
protected :
	ID3DX11EffectMatrixVariable* m_pfxmWVP;
	ID3DX11EffectVectorVariable* m_pfxcColor;


	D3DXMATRIX m_mWVP;
	D3DXMATRIX m_mWorld;
	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;

	D3DXCOLOR m_cColor;
public :
	virtual void SettingDataToGPU(UINT _iPassNum = 0);

	virtual void SetWorld(D3DXMATRIX* _mWorld);
	virtual void SetCamera(CCamera* _pCamera);
	virtual void SetColor(D3DXCOLOR* _col);
	
};




////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CSkyShader : 스카이박스 관련 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class CSkyShader : public CShader
{
public :
	struct VERTEX
	{
		D3DXVECTOR3 s_vPosition;
	};
public :
	CSkyShader();
	virtual ~CSkyShader();

protected :
	ID3DX11EffectShaderResourceVariable* m_pfxCubeTextureSRV;
	ID3D11ShaderResourceView* m_pCubeTextureSRV;

	ID3DX11EffectMatrixVariable* m_pfxmWVP;

	D3DXMATRIX m_mWorld;
	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;
protected :
	virtual void _Init();
public :
	virtual void SettingDataToGPU(UINT _iPassNum = 0);

	void SetWorld(D3DXMATRIX* m);
	void SetCamera(CCamera* camera);
	void SetTexture(ID3D11ShaderResourceView** _ppShaderResourceView, D3DXVECTOR4* _pTexTranform = NULL);
};




////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CTerrainShader : 지형 관련 쉐이더 클래스.
//				지형을 프러스텀 컬링을하기 위해 사용 된다.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class CTerrainShader : public CBasicShader
{
public :
	struct VERTEX
	{
		D3DXVECTOR3 s_vPosition;
		D3DXVECTOR3 s_vNormal;
		D3DXVECTOR2 s_vUV;
	};
public :
	CTerrainShader();
	virtual ~CTerrainShader();
protected :
	// 부모클래스의 맴버 변수를 사용..
	// ..
	ID3DX11EffectVectorVariable* m_pfxFrustum;
	CFrustum m_Frustum;
protected:
	virtual void _Init();

public:
	// 설정한 데이터를 GPU에게 전달해주는 함수.
	virtual void SettingDataToGPU(UINT _iPassNum = 0);
	virtual void SetFrustum(CFrustum* _pFrustum);

};




////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CSpriteShader : 이펙트 관련 쉐이더 클래스.
//				기하 쉐이더에서 빌보딩을 이용하여 이펙트를 효과를 주는 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class CSpriteShader : public CShader
{
public:
	struct VERTEX
	{
		D3DXVECTOR3 s_vPosition;
		D3DXVECTOR2 s_vSize;
	};
public:
	CSpriteShader();
	virtual ~CSpriteShader();
protected:
	ID3DX11EffectMatrixVariable* m_pfxmVP;
	ID3DX11EffectVectorVariable* m_pfxvEyeW;
	ID3DX11EffectVariable* m_pfxMaterial;
	ID3DX11EffectShaderResourceVariable* m_pfxEffectTex;
	ID3DX11EffectVectorVariable* m_pfxvTexTransform;

	D3DXMATRIX m_mVP;
	D3DXVECTOR3 m_vEye;
	ID3D11ShaderResourceView* m_pEffectTexSRV;
	D3DXVECTOR4 m_vTexTransform;
	MATERIAL m_Material;
protected:
	virtual void _Init();
public:
	void SetCamera(CCamera* _pCamera);
	void SetMaterial(MATERIAL* _pMaterial);
	void SetTexture(ID3D11ShaderResourceView** _ppShaderResourceView, D3DXVECTOR4* _pTexTranform = NULL);

	virtual void SettingDataToGPU(UINT _iPassNum = 0);
};




////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CBuildShadowMapShader : 그림자맵 생성 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class CBuildShadowMapShader : public CShader
{
public :
	struct VERTEX
	{
		D3DXVECTOR3 s_vPosition;
		D3DXVECTOR3 s_vNormal;
		D3DXVECTOR2 s_vUV;

		VERTEX(){};
		VERTEX(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vNormal, D3DXVECTOR2 _vUV)
			: s_vPosition(_vPos), s_vNormal(_vNormal), s_vUV(_vUV){};
	};
public :
	CBuildShadowMapShader();
	~CBuildShadowMapShader();
protected :
	ID3DX11EffectVectorVariable* m_pfxvEyePosW;
	ID3DX11EffectScalarVariable* m_pfxfHeightScale;
	ID3DX11EffectScalarVariable* m_pfxfMaxTessDistance;
	ID3DX11EffectScalarVariable* m_pfxfMinTessDistance;
	ID3DX11EffectScalarVariable* m_pfxfMaxTessFactor;
	ID3DX11EffectScalarVariable* m_pfxfMinTessFactor;

	ID3DX11EffectMatrixVariable* m_pfxmWorld;
	ID3DX11EffectMatrixVariable* m_pfxmWorldInvTranspose;
	ID3DX11EffectMatrixVariable* m_pfxmViewProj;
	ID3DX11EffectMatrixVariable* m_pfxmWorldViewPorj;
	ID3DX11EffectMatrixVariable* m_pfxmTexTransform;

	D3DXVECTOR3 m_vEyePosW;
	float m_fHeightScale;
	float m_fMaxTessDistance;
	float m_fMinTessDistance;
	float m_fMaxTessFactor;
	float m_fMinTessFactor;

	D3DXMATRIX m_mWorld;
	D3DXMATRIX m_mWorldInvTranspose;
	D3DXMATRIX m_mViewProj;
	D3DXMATRIX m_mWorldViewProj;
	D3DXMATRIX m_mTexTransform;

	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;

protected :
	virtual void _Init();
public :
	virtual void SettingDataToGPU(UINT _iPassNum = 0);

	void SetEyePosW(D3DXVECTOR3 _vPos)				{ m_vEyePosW = _vPos; }
	void SetHeightScale(float _fScale)				{ m_fHeightScale = _fScale; }
	void SetTessDistance(float _fMin, float _fMax)	{ m_fMinTessDistance = _fMin; 
													  m_fMaxTessDistance = _fMax; }
	void SetTessFactor(float _fMin, float _fMax)	{ m_fMinTessFactor = _fMin;
													  m_fMaxTessFactor = _fMax; }
	void SetWorld(D3DXMATRIX* _m) { m_mWorld = *_m; }
	void SetView(D3DXMATRIX _m) { m_mView = _m; }
	void SetProj(D3DXMATRIX _m) { m_mProj = _m; }
};

//****************************** End of File "Shader.h" ******************************//