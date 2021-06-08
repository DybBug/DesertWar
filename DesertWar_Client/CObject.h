//***************************************************************************************
//
//	CObject.h : 오브젝트 관련 헤더 파일.
//
//***************************************************************************************
#pragma once
#include "Common.h"
#include "CMesh.h"
#include "CShader.h"
#include "CNormalLine.h"

enum Type{
	OBJECT_TYPE_TANK = 0,
	OBJECT_TYPE_COMBATPLANE,
	
	OBJECT_TYPE_COMBATPLANE_MACHINGUN,
};

class CObject
{
public :
	CObject(ID3D11Device* _pDev, ID3D11DeviceContext* _DevC, CShader* _pShader);
	virtual ~CObject();
protected :
	ID3D11Device* m_pDev;
	ID3D11DeviceContext* m_pDevC;
	CShader* m_pShader;
	CBuildShadowMapShader* m_pShadowShader;

	UINT m_nType;
	UINT m_nState;

	float m_fPrevHP;
	float m_fHP;
	bool m_bLive;

	D3DXMATRIX m_mTM;
	D3DXMATRIX m_mScale;
	D3DXMATRIX m_mRot;
	D3DXMATRIX m_mTrans;

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vFront;	// 전방벡터(방향을 의미함).
	D3DXVECTOR3 m_vHoriz;	// 수평벡터.
	D3DXVECTOR3 m_vUp;		// 수직벡터.


	float m_fWidth;
	float m_fHeight;
	float m_fDepth;

	CMeshNode* m_pMesh;
	ID3D11ShaderResourceView* m_pDiffuseSRV;
	CCubeMesh* m_pCollisionBox; // 충돌 박스 메쉬.

	bool m_bPlayer;
	bool m_bComputer;

	UINT m_nTeam;

	bool m_bDestroy;
protected :
	D3DXMATRIX _LAxisRotUp(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz ,D3DXVECTOR3* _pUp, float _fAngle);
	D3DXMATRIX _LAxisRotFront(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz ,D3DXVECTOR3* _pUp, float _fAngle);
	D3DXMATRIX _LAxisRotHoriz(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz ,D3DXVECTOR3* _pUp, float _fAngle);
	D3DXMATRIX _LAxisRot(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, D3DXVECTOR3  _fAngle);
	void _AxisRotMatrix(D3DXVECTOR3* _pFront, D3DXVECTOR3* _pHoriz, D3DXVECTOR3* _pUp, D3DXMATRIX m);
	virtual void _UpdateByState() = 0;

public :
	virtual void Control(float _fDTime) {};
	virtual void Update(float _fDTime) = 0;
	virtual void Render() = 0;
	virtual void ShadowRender(CBuildShadowMapShader* _pShader){};

	void SetPosition(D3DXVECTOR3* _pPos) { 
		m_vPos = *_pPos; 
		D3DXMatrixTranslation(&m_mTrans, m_vPos.x, m_vPos.y, m_vPos.z); 
		m_mTM = m_mTrans;
	}
	void SetTM(D3DXMATRIX* _mTM) { m_mTM = *_mTM; }

	D3DXVECTOR3 GetPosition()	{ return m_vPos; }
	D3DXMATRIX GetTM()			{ return m_mTM; }
	D3DXMATRIX GetRot()			{ return m_mRot; }
	D3DXMATRIX GetTrans()		{ return m_mTrans; }

	bool GetLive() { return m_bLive; }
	void SetLive(bool _Live) { m_bLive = _Live; }

	void SetSize(float _fWidth, float _fHeight, float _fDepth){
		m_fWidth = _fWidth; m_fHeight = _fHeight; m_fDepth = _fDepth;
	}
	void GetSize(float& _fWidth, float& _fHeight, float& _fDepth){
		_fWidth = m_fWidth; _fHeight = m_fHeight; _fDepth = m_fDepth;
	}

	bool GetPlayer()		{ return m_bPlayer; }
	void SetPlayer(bool b)	{ m_bPlayer = b; }

	void SetHP(float _fHP)	{ m_fPrevHP = m_fHP;
							  m_fHP = _fHP; 
							}
	float GetHP()			{ return m_fHP; }

	void SetState(UINT _State)	{ m_nState = _State; }
	UINT GetState()				{ return m_nState; }

	bool GetComputer()			{ return m_bComputer; }
	void SetComputer(bool b)	{ m_bComputer = b; }

	void SetFront(D3DXVECTOR3* _Front) { m_vFront = *_Front; }

	UINT GetTeam()				{ return m_nTeam; }
	void SetTeam(UINT _nTeam)	{ m_nTeam = _nTeam; }

	UINT GetType()				{ return m_nType; }
	void SetType(UINT _nType)	{ m_nType = _nType; }

	bool GetDestory() { return m_bDestroy; }

	void SetShader(CShader* _pShader){ m_pShader = _pShader; }
	CShader* GetShader() { return m_pShader; }
};



/****************************** End of File "CObject.h" ******************************/