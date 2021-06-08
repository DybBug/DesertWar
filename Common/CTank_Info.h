#pragma once
#include "CObject_Info.h"

enum { STATE_TARGET_SEARCHING, STATE_TARGET_SEARCH, STATE_TARGET_NONSEARCH, STATE_BODY_ROTATE, STATE_HOOD_ROTATE, STATE_GUNBARREL_ROTATE, STATE_MOVE, STATE_ATTACK_READY, STATE_ATTACK };

class CTank_Info : public CObject_Info
{
public :
	CTank_Info() : CObject_Info()
	{
		m_vGunBarrelDir = D3DXVECTOR3(0.f, 0.f, 1.f);
		m_vGunBarrelPos = D3DXVECTOR3(0.f, 0.f, 0.f);

		m_fHP = 50.f;

		D3DXMatrixIdentity(&m_mBodyTM);
		D3DXMatrixIdentity(&m_mHoodTM);
		D3DXMatrixIdentity(&m_mGunBarrelTM);

		m_blive = true;
		m_iState = STATE_TARGET_SEARCHING;
		m_pTarget = NULL;
		m_fSearchRange = 100.f;
		m_fNexusSearchRange = 150.f;
		m_fAttackRange = 40.f;
		m_fBodyAngle = 0.f;
		m_fRspeed = 45.0f;
		m_fSpeed = 45.f;

		m_vPrevPos = { 0.f, 0.f, 0.f };

		m_bTargetArrive = true;
		m_bAttackReady = false;

		//srand((unsigned int)time(NULL));

		DT = 0.f;
	};
	virtual ~CTank_Info(){};

	

protected :
	D3DXVECTOR3 m_vGunBarrelDir;
	D3DXVECTOR3 m_vGunBarrelPos;

	D3DXMATRIX m_mBodyTM;
	D3DXMATRIX m_mHoodTM;
	D3DXMATRIX m_mGunBarrelTM;

	int m_iState;
	bool m_bPoint;					// 임의의 포인트를 찍을때 다시 도착전에 다시 찍힘을 방지
	bool m_bSeek;					// 적을 찾았는지 못찾았는지
	bool m_bTargetArrive;
	float m_fSearchRange;
	float m_fNexusSearchRange;
	float m_fAttackRange;

	bool m_bAttackReady;

	float m_fBodyAngle;

	D3DXVECTOR3 m_vRandomPointPos;
	D3DXVECTOR3 m_vRandomPointDir;

	D3DXVECTOR3 m_vNexusVector;

	CObject_Info* m_pTarget;

	D3DXVECTOR3 m_vPrevPos;

	D3DXMATRIX m_mHeightRot;

	float DT;

public :
	virtual void Update(float _fDTime);
	virtual void ComUpdate(float _fDTime);


	void SetGunBarrelDir(D3DXVECTOR3* _v) { m_vGunBarrelDir = *_v; }
	D3DXVECTOR3 GetGunBarrelDir() { return m_vGunBarrelDir; }

	void SetGunBarrelPos(D3DXVECTOR3* _v) { m_vGunBarrelPos = *_v; }
	D3DXVECTOR3 GetGunBarrelPos() { return m_vGunBarrelPos; }

	void SetBodyTM(D3DXMATRIX* _m){ m_mBodyTM = *_m; }
	void SetHoodTM(D3DXMATRIX* _m){ m_mHoodTM = *_m; }
	void SetGunBarrelTM(D3DXMATRIX* _m) { m_mGunBarrelTM = *_m; }

	D3DXMATRIX GetBodyTM() { return m_mBodyTM; }
	D3DXMATRIX GetHoodTM() { return m_mHoodTM; }
	D3DXMATRIX GetGunBarrelTM() { return m_mGunBarrelTM; }
};