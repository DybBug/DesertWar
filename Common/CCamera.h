//**********************************************************************************
//	2015.01.02.
//	CCamera.h : ī�޶� ���� ��� ����.		
//**********************************************************************************
#pragma once
#include "Common.h"


////////////////////////////////////////////////////////////////////////////////////
//
//	CCmaera : ī�޶� �⺻ Ŭ����.
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
	D3DXVECTOR3 m_vUp;		// �� ����.
	D3DXVECTOR3 m_vFront;	// ���� ����.
	D3DXVECTOR3 m_vHoriz;	//	���� ����.

	float m_fNearZ;
	float m_fFarZ;
	float m_fAspect;
	float m_fFOV;

	UINT m_nScreenWidth;
	UINT m_nScreenHeight;

	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;	//	���� ���� ���.
	D3DXMATRIX m_mOrtho; // ���� ���� ���.

	float m_fDTime;

public :
	void LookAt(D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp);			// �� ���.
	void SetLens(float _fFOV, float _fAspect, float _fNZ, float _fFZ);				// ���� ���� ���.
	void SetOrtho(UINT _nScreenWidth, UINT _nScreenHeight, float _fNZ, float _fFZ);	// ���� ���� ���.
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
//	CFreeCamera : ���� ���� ī�޶� Ŭ����.
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

	void MoveFront(float _fDir, float _fSpeed);	// ī�޶� ����(Z��) ���ͷ� �̵�.
	void MoveHoriz(float _fDir, float _fSpeed); // ī�޶� ����(X��) ���ͷ� �̵�.
	void MoveUp(float _fDir, float _fSpeed);	// ī�޶� ��(Y��) ���ͷ� �̵�.
};




////////////////////////////////////////////////////////////////////////////////////
//
//	CFixCamera : ���� ���� ī�޶� Ŭ����.
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
