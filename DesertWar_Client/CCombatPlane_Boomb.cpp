//************************************************************************************************
//
//	CCombatPlane_Boomb.cpp : ÀüÅõ±â ÆøÅº ¼Ò½º ÆÄÀÏ.
//
//************************************************************************************************
#include "CCombatPlane_Boomb.h"




////////////////////////////////////////////////////////////////////////////////////////////////////
// CCombatPlane_Boomb()
CCombatPlane_Boomb::CCombatPlane_Boomb(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShader,
		SPRITE* _pSprite, D3DXVECTOR3 _vPos)
:CObject(_pDev, _pDevC, _pShader)
{
	m_fPower = 50.0f;
	m_fSpeed = 20.0f;

	m_Sprite = *_pSprite;
}




////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CombatPlane_Boomb()
CCombatPlane_Boomb::~CCombatPlane_Boomb()
{
	

}




////////////////////////////////////////////////////////////////////////////////////////////////////
// Update()
void CCombatPlane_Boomb::Update(float _fDTime)
{

}




////////////////////////////////////////////////////////////////////////////////////////////////////
// Render()
void CCombatPlane_Boomb::Render()
{
	m_Sprite.s_vPosition = D3DXVECTOR3(m_mTM._41, m_mTM._42, m_mTM._43);
	Sprite_Draw(&m_Sprite);
}








/****************************** End of File "CCombatPlane_Boomb.cpp" ******************************/