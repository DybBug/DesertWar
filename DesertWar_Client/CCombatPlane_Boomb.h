//************************************************************************************************
//
//	CCombatPlane_Boomb.h : ÀüÅõ±â ÆøÅº Çì´õ ÆÄÀÏ.
//
//************************************************************************************************
#pragma once
#include "Sprite.h"
#include "CObject.h"

class CCombatPlane_Boomb : public CObject
{
public :
	CCombatPlane_Boomb(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShder,
		SPRITE* _pSprite, D3DXVECTOR3 _vPos);
	virtual ~CCombatPlane_Boomb();
	
protected :
	float m_fPower;
	float m_fSpeed;

	SPRITE m_Sprite;
protected :
	virtual void _UpdateByState(){};

public :
	virtual void Update(float _fDTime);
	virtual void Render();
};



/****************************** End of File "CCombatPlane_Boomb.h" ******************************/