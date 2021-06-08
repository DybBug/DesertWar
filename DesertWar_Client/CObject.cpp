//***************************************************************************************
//
//	CObject.cpp : ������Ʈ ���� �ҽ� ����.
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
// _fAngle : ������ ȸ�� ���� �ƴ� ȸ�� ��ȭ��.
// ���ϰ� : X������ ȸ���� ��İ�.
D3DXMATRIX CObject::_LAxisRotHoriz(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, float _fAngle)
{
	D3DXMATRIX mRot; 
	D3DXMatrixRotationAxis(&mRot, _pHoriz, _fAngle);

	// Front �� ����
	D3DXVec3TransformNormal(_pFront, _pFront, &mRot);

	// Horiz �� ����.
	// ...

	// Up �� ����.
	D3DXVec3Cross(_pUp, _pFront, _pHoriz);
	D3DXVec3Normalize(_pUp, _pUp);

	return mRot;
}




/////////////////////////////////////////////////////////////////////////////////////////
// _LAxisRotUp()
// _fAngle : ������ ȸ�� ���� �ƴ� ȸ�� ��ȭ��.
// ���ϰ� : Y������ ȸ���� ��İ�.
D3DXMATRIX CObject::_LAxisRotUp(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, float _fAngle)
{
	D3DXMATRIX mRot;
	D3DXMatrixRotationAxis(&mRot, _pUp, _fAngle);

	// Front �� ����
	D3DXVec3TransformNormal(_pFront, _pFront, &mRot);

	// Horiz �� ����.
	D3DXVec3Cross(_pHoriz, _pUp, _pFront);
	D3DXVec3Normalize(_pHoriz, _pHoriz);

	// Up �� ����.
	// ...
	return mRot;

}




/////////////////////////////////////////////////////////////////////////////////////////
// _LAxisRotFront()
// _fAngle : ������ ȸ�� ���� �ƴ� ȸ�� ��ȭ��.
// ���ϰ� : Z������ ȸ���� ��İ�.
D3DXMATRIX CObject::_LAxisRotFront(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, float _fAngle)
{
	D3DXMATRIX mRot;
	D3DXMatrixRotationAxis(&mRot, _pFront, _fAngle);

	// Front �� ����
	// ...

	// Horiz �� ����.
	D3DXVec3TransformNormal(_pHoriz, _pHoriz, &mRot);

	// Up �� ����.
	D3DXVec3Cross(_pUp, _pFront, _pHoriz);
	D3DXVec3Normalize(_pUp, _pUp);

	return mRot;
}




/////////////////////////////////////////////////////////////////////////////////////////
// _LAxisRot(1, 2, 3)
// X * Y * Z ȸ�� ��� ��
D3DXMATRIX CObject::_LAxisRot(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, D3DXVECTOR3 _fAngle)
{
	D3DXMATRIX RotX = _LAxisRotFront(_pFront, _pHoriz, _pUp, D3DXToRadian(_fAngle.x));
	D3DXMATRIX RotY = _LAxisRotUp(_pFront, _pHoriz, _pUp, D3DXToRadian(_fAngle.y));
	D3DXMATRIX RotZ = _LAxisRotHoriz(_pFront, _pHoriz, _pUp, D3DXToRadian(_fAngle.z));

	return (RotY * RotX * RotZ);
}




/////////////////////////////////////////////////////////////////////////////////////////
//  _AxisRotMatrix()
// ��ķ� �� �� ȸ��.
void CObject::_AxisRotMatrix(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, D3DXMATRIX m)
{
	D3DXVec3TransformNormal(_pFront, _pFront, &m);
	D3DXVec3TransformNormal(_pHoriz, _pHoriz, &m);
	D3DXVec3TransformNormal(_pUp, _pUp, &m);
}


/****************************** End of File "CObject.cpp" ******************************/