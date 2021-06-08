//***************************************************************************************
//
//	CObject.cpp : 오브젝트 관련 소스 파일.
//
//***************************************************************************************
#include "CObject.h"
#include "CShaderManager.h"
#include "CConsole.h"

/////////////////////////////////////////////////////////////////////////////////////////
// CObject()
CObject::CObject(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShader)
	: m_pDev(_pDev), m_pDevC(_pDevC), m_pMesh(NULL), m_pCollisionBox(NULL), m_pShader(_pShader)
{
	m_nType = 0;
	m_fPrevHP = m_fHP = 100.f;
	m_bLive = true;
	m_nState = 0;
	D3DXMatrixIdentity(&m_mTM);
	D3DXMatrixIdentity(&m_mScale);
	D3DXMatrixIdentity(&m_mRot);
	D3DXMatrixIdentity(&m_mTrans);

	m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vFront = D3DXVECTOR3(0.f, 0.f, 1.f);
	m_vHoriz = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	m_fWidth = 1.f;
	m_fHeight = 1.f;
	m_fDepth = 1.f;

	m_pMesh = NULL;
	m_pDiffuseSRV = NULL;

	m_bPlayer = false;
	m_bComputer = false;

	m_bDestroy = false;
}




/////////////////////////////////////////////////////////////////////////////////////////
// ~CObject()
CObject::~CObject()
{
	SAFE_DELETE(m_pCollisionBox);
}




/////////////////////////////////////////////////////////////////////////////////////////
// _LAxisRotHoriz() 
// _fAngle : 누적된 회전 값이 아닌 회전 변화량.
// 리턴값 : X축으로 회전한 행렬값.
D3DXMATRIX CObject::_LAxisRotHoriz(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, float _fAngle)
{
	D3DXMATRIX mRot; 
	D3DXMatrixRotationAxis(&mRot, _pHoriz, _fAngle);

	// Front 축 교정
	D3DXVec3TransformNormal(_pFront, _pFront, &mRot);

	// Horiz 축 교정.
	// ...

	// Up 축 교정.
	D3DXVec3Cross(_pUp, _pFront, _pHoriz);
	D3DXVec3Normalize(_pUp, _pUp);

	return mRot;
}




/////////////////////////////////////////////////////////////////////////////////////////
// _LAxisRotUp()
// _fAngle : 누적된 회전 값이 아닌 회전 변화량.
// 리턴값 : Y축으로 회전한 행렬값.
D3DXMATRIX CObject::_LAxisRotUp(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, float _fAngle)
{
	D3DXMATRIX mRot;
	D3DXMatrixRotationAxis(&mRot, _pUp, _fAngle);

	// Front 축 교정
	D3DXVec3TransformNormal(_pFront, _pFront, &mRot);

	// Horiz 축 교정.
	D3DXVec3Cross(_pHoriz, _pUp, _pFront);
	D3DXVec3Normalize(_pHoriz, _pHoriz);

	// Up 축 교정.
	// ...
	return mRot;

}




/////////////////////////////////////////////////////////////////////////////////////////
// _LAxisRotFront()
// _fAngle : 누적된 회전 값이 아닌 회전 변화량.
// 리턴값 : Z축으로 회전한 행렬값.
D3DXMATRIX CObject::_LAxisRotFront(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, float _fAngle)
{
	D3DXMATRIX mRot;
	D3DXMatrixRotationAxis(&mRot, _pFront, _fAngle);

	// Front 축 교정
	// ...

	// Horiz 축 교정.
	D3DXVec3TransformNormal(_pHoriz, _pHoriz, &mRot);

	// Up 축 교정.
	D3DXVec3Cross(_pUp, _pFront, _pHoriz);
	D3DXVec3Normalize(_pUp, _pUp);

	return mRot;
}




/////////////////////////////////////////////////////////////////////////////////////////
// _LAxisRot(1, 2, 3)
// X * Y * Z 회전 행렬 곱
D3DXMATRIX CObject::_LAxisRot(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, D3DXVECTOR3 _fAngle)
{
	D3DXMATRIX RotX = _LAxisRotFront(_pFront, _pHoriz, _pUp, D3DXToRadian(_fAngle.x));
	D3DXMATRIX RotY = _LAxisRotUp(_pFront, _pHoriz, _pUp, D3DXToRadian(_fAngle.y));
	D3DXMATRIX RotZ = _LAxisRotHoriz(_pFront, _pHoriz, _pUp, D3DXToRadian(_fAngle.z));

	return (RotY * RotX * RotZ);
}




/////////////////////////////////////////////////////////////////////////////////////////
//  _AxisRotMatrix()
// 행렬로 각 축 회전.
void CObject::_AxisRotMatrix(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, D3DXMATRIX m)
{
	D3DXVec3TransformNormal(_pFront, _pFront, &m);
	D3DXVec3TransformNormal(_pHoriz, _pHoriz, &m);
	D3DXVec3TransformNormal(_pUp, _pUp, &m);
}


/****************************** End of File "CObject.cpp" ******************************/