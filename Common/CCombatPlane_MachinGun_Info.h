#pragma once
#include "CCombat_Info.h"

class CCombatPlane_MachinGun_Info : public CCombat_Info
{
public: 
	CCombatPlane_MachinGun_Info();
	virtual ~CCombatPlane_MachinGun_Info();

protected :
	float m_fPower;
	float m_fSpeed;

	float m_fDeathTime;

public :
	virtual void Update(float _fDTime);
	virtual void ComUpdate(float _fDTime);
};