#pragma once
#include "CObject_Info.h"

class CCombat_Info : public CObject_Info
{
public :
	CCombat_Info() : CObject_Info()
	{
		m_vLMachinGunPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_vRMachinGunPos = D3DXVECTOR3(0.f, 0.f, 0.f);

		m_fHP = 50.f;

		D3DXMatrixIdentity(&m_mTM);

		m_fWidth = 8.f;
		m_fHeight = 2.f;
		m_fDepth = 12.f;

		m_blive = true;

		m_fSpeed = 70.0f;
		m_fRspeed = 20.0f;
		

		m_pTarget = NULL;
		m_nState = STATE_TARGET_SERCHING;
		m_fAngle = 0.0f;

		m_fTurningRadius = 20.f;

		m_enemyChar = 0;
		m_enemyPos = { 0, 0, 0 };
		m_enemy = 0;

		m_fSenseRange = 200.f;

		m_dUpVec = { 0, 1, 0 };
		DT = timeGetTime();
	};
	enum Combat_State{
	STATE_TARGET_SERCHING,
	STATE_TARGET_TRACE,
	STATE_MACHINGUN_ATTACK,
	STATE_COMBATBOOMB_ATTACK,
	STATE_RETURN
	};
	virtual ~CCombat_Info() {};

protected :
	D3DXVECTOR3 m_vLMachinGunPos;
	D3DXVECTOR3 m_vRMachinGunPos;

	CObject_Info* m_pTarget;
	int m_nState;

	float m_fAngle;
	float m_fTurningRadius;

	int m_enemyChar;
	D3DXVECTOR3 m_enemyPos;
	int m_enemy;


	float m_fSenseRange;

	D3DXVECTOR3 m_dUpVec;

	float DT;

public :
	virtual void Update(float _fDTime);
	virtual void ComUpdate(float _fDTime);
	
	// ¿ÞÂÊ MachinGun.
	void SetLMachinGunPos(D3DXVECTOR3* _v) { m_vLMachinGunPos = *_v; }
	D3DXVECTOR3 GetLMachinGunPos() { return m_vLMachinGunPos; }

	// ¿À¸¥ÂÊ MachinGun.
	void SetRMachinGunPos(D3DXVECTOR3* _v) { m_vRMachinGunPos = *_v; }
	D3DXVECTOR3 GetRMachinGunPos() { return m_vRMachinGunPos; }

	int SenseFunction(int*, int* );
	void SenseTarget();
};