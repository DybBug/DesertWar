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

	D3DXVECTOR3 m_vGravity;		// 중력값 설정 벡터
	D3DXVECTOR3 m_vVelocity;	// 중력값에서 점점 누적시킬 벡터

//	ST_OBB object, missile;

	int cnt, myTeam;

public :
	virtual void Update(float _fDTime);
	virtual void CTank_Boomb_Info::ComUpdate(float _fDTime);
};