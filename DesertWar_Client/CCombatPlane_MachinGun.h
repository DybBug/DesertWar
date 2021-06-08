//***************************************************************************************
//
//	CCombatPlane_MachinGun.h : 전투기 머신건 헤더 파일.
//
//***************************************************************************************
#pragma once
#include "Sprite.h"
#include "CObject.h"

class CCombatPlane_MachinGun : public CObject
{
public :
	CCombatPlane_MachinGun(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShader,
		SPRITE* _pSprite, D3DXVECTOR3 _vPos, D3DXVECTOR3 _vDir);
	virtual ~CCombatPlane_MachinGun();

protected :
	float m_fPower;
	float m_fSpeed;

	SPRITE m_Sprite;

	SPRITE* m_pExplosion;
	float m_fTime;
	UINT m_iFrameX;
	UINT m_iFrameY;

protected :
	virtual void _UpdateByState(){};

public :
	virtual void Update(float _fDTime);
	virtual void Render();


};




/****************************** End of File "CCombatPlane_MachinGun.h" ******************************/