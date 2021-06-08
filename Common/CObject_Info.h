#pragma once
#include "Common.h"


enum{
	OBJ_TANK = 1,
	OBJ_COMBATPLANE,

	OBJ_BULLET,

};

class CObject_Info
{
public :
	CObject_Info() {
		m_nClientID = 0;
		m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXMatrixIdentity(&m_mTM);
		m_vFront = D3DXVECTOR3(0.f, 0.f, 1.f);
		m_vHoriz = D3DXVECTOR3(1.f, 0.f, 0.f);
		m_vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

		m_fWidth = 1.f;
		m_fHeight = 1.f;
		m_fDepth = 1.f;

		m_nID = -1;

		m_blive = true;
		m_fRspeed = 10.0f;
	};
	virtual ~CObject_Info() {};
protected :
	UINT m_nClientID;

	UINT m_nType;

	D3DXVECTOR3 m_vPos;
	D3DXMATRIX m_mTM;

	D3DXVECTOR3 m_vFront;
	D3DXVECTOR3 m_vHoriz;
	D3DXVECTOR3 m_vUp;

	float m_fWidth;
	float m_fHeight;
	float m_fDepth;

	int m_nID; //ÃÑ¾Ë ¾ÆÀÌµð
	float m_fHP;
	float m_fSpeed;
	float m_fRspeed;
	bool m_blive;

public :
	virtual void Update(float _fDTime){};
	virtual void ComUpdate(float _fDTime){};
	virtual void NexusUpdate(float _fDTime){};

	void SetClientID(UINT _nID) { m_nClientID = _nID; }
	UINT GetClientID() { return m_nClientID; }

	void SetTM(D3DXMATRIX* _m) { m_mTM = *_m; }
	D3DXMATRIX GetTM() { return m_mTM; }

	void SetType(UINT i) { m_nType = i; }
	UINT GetType() { return m_nType; }

	void SetPosition(D3DXVECTOR3* _vPos) { m_vPos = *_vPos; }
	D3DXVECTOR3 GetPosition() { return m_vPos; }

	void SetFront(D3DXVECTOR3* _v){ m_vFront = *_v; }
	D3DXVECTOR3 GetFront() { return m_vFront; }

	void SetHoriz(D3DXVECTOR3* _v) { m_vHoriz = *_v; }
	D3DXVECTOR3 GetHoriz() { return m_vHoriz; }

	void SetUp(D3DXVECTOR3* _v) { m_vUp = *_v; }
	D3DXVECTOR3 GetUp() { return m_vUp; }

	void SetSize(float _fWidth, float _fHeight, float _fDepth){
		m_fWidth = _fWidth; m_fHeight = _fHeight; m_fDepth = _fDepth;
	}
	void GetSize(float& _fWidth, float& _fHeight, float& _fDepth){
		_fWidth = m_fWidth; _fHeight = m_fHeight; _fDepth = m_fDepth;
	}

	void SetID(UINT i) { m_nID = i; }
	UINT GetID() { return m_nID; }

	void SetHP(float i) { m_fHP = i; }
	float GetHP() { return m_fHP; }

	void Setlive(bool i) { m_blive = i; }
	bool Getlive() { return m_blive; }

};