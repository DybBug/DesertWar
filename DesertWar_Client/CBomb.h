//*************************************************************************************
//	CBomb.h : ��ź ���� ��� ����
//	: ������ �Ϲ� ���, ������ Ư�� ����� ����.
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