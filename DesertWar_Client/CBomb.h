//*************************************************************************************
//	CBomb.h : 포탄 관련 헤더 파일
//	: 전차의 일반 기술, 전투기 특수 기술에 적용.
//*************************************************************************************
#pragma once

#include "Common.h"
#include "CObject.h"


class CBomb : public CObject
{
public :
	CBomb(ID3D11Device* _pDev, ID3D11DeviceContext* m_pDevC, CShader* _pShader, 
			UINT _nType, float _fPower, float _fSpeed);
	virtual ~CBomb();

protected :
	float m_fPower;
	float m_fSpeed;
public :
	virtual void Control(float _fDTime) {};
	virtual void Update(float _fDTime);
	virtual void Render();
};


//****************************** End of File "CBomb.h" ******************************//