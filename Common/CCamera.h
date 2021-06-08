//**********************************************************************************
//	2015.01.02.
//	CCamera.h : 카메라 관련 헤더 파일.		
//**********************************************************************************
#pragma once
#include "Common.h"


////////////////////////////////////////////////////////////////////////////////////
//
//	CCmaera : 카메라 기본 클래스.
//
////////////////////////////////////////////////////////////////////////////////////
class CCamera
{
public :
	CCamera(D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp,
		float _fFOV = D3DX_PI * 0.25f, float _fAspect = 1.0f,
		float _fNZ = 1.0f, float _fFZ = 1000.0f);
	virtual ~CCamera();
protected :
	bool m_bOrthoLens;
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vLookAt;
	D3DXVECTOR3 m_vUp;		// 업 벡터.
	D3DXVECTOR3 m_vFront;	// 전방 벡터.
	D3DXVECTOR3 m_vHoriz;	//	수평 벡터.

	float m_fNearZ;
	float m_fFarZ;
	float m_fAspect;
	float m_fFOV;

	UINT m_nScreenWidth;
	UINT m_nScreenHeight;

	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;	//	원근 투영 행렬.
	D3DXMATRIX m_mOrtho; // 직교 투영 행렬.

	float m_fDTime;

public :
	void LookAt(D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp);			// 뷰 행령.
	void SetLens(float _fFOV, float _fAspect, float _fNZ, float _fFZ);				// 원근 투영 행렬.
	void SetOrtho(UINT _nScreenWidth, UINT _nScreenHeight, float _fNZ, float _fFZ);	// 직교 투영 행렬.
	virtual void Update(float _fdTime) {};
	
	D3DXVECTOR3 GetEye()		{ return m_vEye; }
	D3DXVECTOR3 GetLookAt()		{ return m_vLookAt; }
	D3DXVECTOR3 GetUp()			{ return m_vUp; }
	D3DXVECTOR3 GetFront()		{ return m_vFront; }
	D3DXVECTOR3 GetHoriz()		{ return m_vHoriz; }

	float GetNearZ()			{ return m_fNearZ; }
	float GetFarZ()				{ return m_fFarZ; }
	float GetAspect()			{ return m_fAspect; }
	float GetFOV()				{ return m_fFOV; }

	UINT GetScreenWidth()		{ return m_nScreenWidth; }
	UINT GetScreenHeight()		{ return m_nScreenHeight; }

	D3DXMATRIX GetViewMatrix()	{ return m_mView; }
	D3DXMATRIX GetProjMatrix()	{ return m_mProj; }
	D3DXMATRIX GetOrthoMatrix() { return m_mOrtho; }

	void EnableOrthoLens(bool _bEnagle) { m_bOrthoLens = _bEnagle; }
	bool GetEnableOrthoLens() { return m_bOrthoLens; }
};


////////////////////////////////////////////////////////////////////////////////////
//
//	CFreeCamera : 자유 시점 카메라 클래스.
//
////////////////////////////////////////////////////////////////////////////////////
class CFreeCamera : public CCamera
{
public : 
	CFreeCamera(D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp,
		float _fFOV = D3DX_PI * 0.25f, float _fAspect = 1.0f,
		float _fNZ = 1.0f, float _fFZ = 1000.0f);
	virtual ~CFreeCamera();
protected :
	
public :
	virtual void Update(float _fDTime);

	void MoveFront(float _fDir, float _fSpeed);	// 카메라 전방(Z축) 벡터로 이동.
	void MoveHoriz(float _fDir, float _fSpeed); // 카메라 수평(X축) 벡터로 이동.
	void MoveUp(float _fDir, float _fSpeed);	// 카메라 업(Y축) 벡터로 이동.
};




////////////////////////////////////////////////////////////////////////////////////
//
//	CFixCamera : 고정 시점 카메라 클래스.
//
////////////////////////////////////////////////////////////////////////////////////

class CObject;

class CFixCamera : public CCamera
{
public :
	CFixCamera(CObject* _pTarget, D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp,
		float _fFOV = D3DX_PI * 0.25f, float _fAspect = 1.0f,
		float _fNZ = 1.0f, float _fFZ = 1000.0f);
	virtual ~CFixCamera();

protected :
	CObject* m_pTarget;

	D3DXVECTOR3 m_vOrgEye;
	D3DXVECTOR3 m_vOrgLookAt;
	D3DXVECTOR3 m_vOrgUp;
public :
	virtual void Update(float _fDTime);
};




//****************************** End of File "CCamera.h" ******************************//
