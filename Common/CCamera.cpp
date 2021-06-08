//**********************************************************************************
//	2015.01.02.
//	CCamera.cpp : 카메라 관련 소스 파일
//**********************************************************************************
#include "CCamera.h"

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
// CCamera
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// CCamera()
CCamera::CCamera(D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp,
				float _fFOV, float _fAspect, float _fNZ, float _fFZ)
: m_vEye(_vEye), m_vLookAt(_vLookAt), m_vUp(_vUp),
  m_fFOV(_fFOV), m_fAspect(_fAspect), m_fNearZ(_fNZ), m_fFarZ(_fFZ),
  m_nScreenWidth(0), m_nScreenHeight(0),
  m_fDTime(0.0f), m_bOrthoLens(false)
{
	LookAt(_vEye, _vLookAt, _vUp);
	SetLens(_fFOV, _fAspect, _fNZ, _fFZ);
	SetOrtho(m_nScreenWidth, m_nScreenHeight, _fNZ, _fFZ);
}




////////////////////////////////////////////////////////////////////////////////////
//	~CCamera()
CCamera::~CCamera()
{

}




////////////////////////////////////////////////////////////////////////////////////
//	LookAt() : 뷰 행령 변환.
void CCamera::LookAt(D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp)
{
	//	축계산.
	D3DXVECTOR3 Front = _vLookAt - _vEye;
	D3DXVec3Normalize(&m_vFront, &Front);

	D3DXVECTOR3 Horiz;
	D3DXVec3Cross(&Horiz, &_vUp, &Front);
	D3DXVec3Normalize(&m_vHoriz, &Horiz);

	D3DXVECTOR3 Up;
	D3DXVec3Cross(&Up, &Front, &Horiz);
	D3DXVec3Normalize(&m_vUp, &Up);

	m_vEye = _vEye;
//	m_vLookAt = m_vFront * 100.0f;

	D3DXMatrixLookAtLH(&m_mView, &m_vEye, &m_vLookAt, &m_vUp);
}




////////////////////////////////////////////////////////////////////////////////////
//	SetLens : 프로젝션 행렬 변환.
void CCamera::SetLens(float _fFOV, float _fAspect, float _fNZ, float _fFZ)
{
	m_fFOV = _fFOV;
	m_fAspect = _fAspect;
	m_fNearZ = _fNZ;
	m_fFarZ = _fFZ;

	D3DXMatrixPerspectiveFovLH(&m_mProj, m_fFOV, m_fAspect, m_fNearZ, m_fFarZ);
}




////////////////////////////////////////////////////////////////////////////////////
// SetOrtho
void CCamera::SetOrtho(UINT _nScreenWidth, UINT _nScreenHeight, float _fNZ, float _fFZ)
{
	m_nScreenWidth = _nScreenWidth;
	m_nScreenHeight = _nScreenHeight;
	m_fNearZ = _fNZ;
	m_fFarZ = _fFZ;

	D3DXMatrixOrthoOffCenterLH(&m_mOrtho, 0, (float)m_nScreenWidth, (float)m_nScreenHeight, 0.0f, m_fNearZ, m_fFarZ);
}





////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
// CFreeCamera
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// CFreeCamera()
CFreeCamera::CFreeCamera(D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp,
						float _fFOV, float _fAspect, float _fNZ, float _fFZ)
: CCamera(_vEye, _vLookAt, _vUp, _fFOV, _fAspect, _fNZ, _fFZ)
{

}




////////////////////////////////////////////////////////////////////////////////////
//	~CFreeCamera()
CFreeCamera::~CFreeCamera()
{

}




////////////////////////////////////////////////////////////////////////////////////
// MoveFront() : 카메라 전방(Z축) 벡터로 이동.
void CFreeCamera::MoveFront(float _fDir, float _fSpeed)
{
	m_vEye += m_vFront * _fDir * _fSpeed;
	m_vLookAt = m_vEye + m_vFront * 100.0f;

	LookAt(m_vEye, m_vLookAt, m_vUp);
}




////////////////////////////////////////////////////////////////////////////////////
//	MoveHoriz() : 카메라 수평(X축) 벡터로 이동.
void CFreeCamera::MoveHoriz(float _fDir, float _fSpeed)
{
	m_vEye += m_vHoriz * _fDir * _fSpeed;
	m_vLookAt = m_vEye + m_vFront * 100.0f;
	LookAt(m_vEye, m_vLookAt, m_vUp);
}




////////////////////////////////////////////////////////////////////////////////////
// MoveUp() : 카메라 업(Y축) 벡터로 이동.
void CFreeCamera::MoveUp(float _fDir, float _fSpeed)
{
	D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 Result = Up * _fDir * _fSpeed;
	m_vEye += Result;
	m_vLookAt += Result;
	LookAt(m_vEye, m_vLookAt, m_vUp);
}




////////////////////////////////////////////////////////////////////////////////////
//	Update()
void CFreeCamera::Update(float _fDTime)
{
	m_fDTime = _fDTime;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		MoveFront(1.0f, 100.f*_fDTime);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		MoveFront(-1.0f, 100.f*_fDTime);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		MoveHoriz(1.0f, 100.f*_fDTime);
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		MoveHoriz(-1.0f, 100.f*_fDTime);
	if ((GetAsyncKeyState(VK_UP) & 0x8000) && GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		MoveUp(1.0f, 100.f*_fDTime);
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		MoveUp(-1.0f, 100.f*_fDTime);
}




////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
// CFixCamera
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// CFixCamera()
CFixCamera::CFixCamera(CObject* _pTarget, D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp,
	float _fFOV, float _fAspect, float _fNZ, float _fFZ)
	:CCamera(_vEye, _vLookAt, _vUp, _fFOV, _fAspect, _fNZ, _fFZ)
{
	m_pTarget = _pTarget;

	m_vOrgEye = _vEye;
	m_vOrgLookAt = _vLookAt;
	m_vOrgUp = _vUp;
}




////////////////////////////////////////////////////////////////////////////////////
// ~CFixCamera()
CFixCamera::~CFixCamera()
{

}



#include "CObject.h"
#include "CTank.h"
////////////////////////////////////////////////////////////////////////////////////
// Update()
void CFixCamera::Update(float _fDTime)
{
	D3DXMATRIX mTM; D3DXMatrixIdentity(&mTM);

	// 타겟의 타입에 따라 변환 행렬 계산.
	if (m_pTarget->GetType() == OBJECT_TYPE_TANK){
		m_vOrgEye = D3DXVECTOR3(0.0f, 10.0f, -20.0f);
		m_vOrgLookAt = D3DXVECTOR3(0.f, 0.0f, 100.0f);
		m_vOrgUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		D3DXMATRIX mRot = m_pTarget->GetRot();
		D3DXMATRIX mHoodRot = ((CTank*)m_pTarget)->GetHoodRot();
		D3DXMATRIX mTrans = m_pTarget->GetTrans();

		D3DXVec3TransformCoord(&m_vEye, &m_vOrgEye, &(mHoodRot * mRot * mTrans));;

		D3DXVec3TransformCoord(&m_vLookAt, &m_vOrgLookAt, &(mHoodRot * mRot * mTrans));
		m_vUp = m_vOrgUp;
	}
	else if (m_pTarget->GetType() == OBJECT_TYPE_COMBATPLANE){
		m_vOrgEye = D3DXVECTOR3(0.0f, 10.0f, -50.0f);
		m_vOrgLookAt = D3DXVECTOR3(0.f, 0.0f, 0.0f);
		m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			
		mTM = m_pTarget->GetRot() * m_pTarget->GetTrans();

		D3DXVec3TransformCoord(&m_vEye, &m_vOrgEye, &mTM);
		D3DXVec3TransformCoord(&m_vLookAt, &m_vOrgLookAt, &mTM);

		D3DXVec3TransformNormal(&m_vUp, &m_vOrgUp, &mTM);
	}

	LookAt(m_vEye, m_vLookAt, m_vUp);
}





//****************************** End of File "CCamera.cpp" ******************************//