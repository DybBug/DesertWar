#pragma once
#include "CTank_Info.h"
//#include "CheckOBBCollision.h"

class CTank_Boomb_Info : public CTank_Info
{
public :
	CTank_Boomb_Info();
	virtual ~CTank_Boomb_Info();
protected :
	float m_fPower;
	float m_fSpeed;

	float m_fDeathTime;

	D3DXVECTOR3 m_vGravity;		// �߷°� ���� ����
	D3DXVECTOR3 m_vVelocity;	// �߷°����� ���� ������ų ����

//	ST_OBB object, missile;

	int cnt, myTeam;

public :
	virtual void Update(float _fDTime);
	virtual void CTank_Boomb_Info::ComUpdate(float _fDTime);
};