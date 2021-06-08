//***********************************************************************************
//***********************************************************************************
//
//	CNexus.h : 넥서스 관련 헤더 파일.
//
//***********************************************************************************
//***********************************************************************************
#pragma once
#include "CObject.h"
#include "CShader.h"

class CNexus : public  CObject
{
public :
	CNexus(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShader, 
		CMeshNode* _pMesh, D3DXVECTOR3 _vPos, float _fHP);
	virtual ~CNexus();

protected :
	float m_fAngle;
	float m_fRotateSpeed;


protected :
	void _Draw(CMeshNode* _pMesh);
	void _ShadowDraw(CMeshNode* _pMesh);
	virtual void _UpdateByState() {};

public :
	virtual void Control(float _fDTime) {};
	virtual void Update(float _fDTime);
	virtual void Render();
	virtual void ShadowRender(CBuildShadowMapShader* _pShader);


};













/****************************** End of File "Nexus.h" ******************************/