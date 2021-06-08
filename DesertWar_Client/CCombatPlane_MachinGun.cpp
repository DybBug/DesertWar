//***************************************************************************************
//
//	CCombatPlane_MachinGun.cpp : 전투기 머신건 소스 파일.
//
//***************************************************************************************
#include "CCombatPlane_MachinGun.h"
#include "CResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CCombatPlane_MachinGun()
CCombatPlane_MachinGun::CCombatPlane_MachinGun(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShader,
	SPRITE* _pSprite, D3DXVECTOR3 _vPos, D3DXVECTOR3 _vDir)
:CObject(_pDev, _pDevC, _pShader)
{
	m_vPos = _vPos;
	m_vFront = _vDir;

	m_fPower = 10.0f;
	m_fSpeed = 10.0f;	
	m_Sprite = *_pSprite;

	m_pExplosion = ResourceManager->Explosion();
	m_fTime = 0.0f;
	m_iFrameX = 0;
	m_iFrameY = 0;
	m_pExplosion->s_fHeight = 10.f;
	m_pExplosion->s_fWidth = 10.f;
	m_pExplosion->s_vTexTransform = D3DXVECTOR4(m_iFrameX,
		m_iFrameY,
		1.f / (float)m_pExplosion->s_iFrameMaxX,
		1.f / (float)m_pExplosion->s_iFrameMaxY);
}




/////////////////////////////////////////////////////////////////////////////////
// ~CCombatPlane_MachinGun()
CCombatPlane_MachinGun::~CCombatPlane_MachinGun()
{
	
}




/////////////////////////////////////////////////////////////////////////////////
// Update()
void CCombatPlane_MachinGun::Update(float _fDTime)
{
	/*if (!m_bLive){
		m_fTime += _fDTime;
		m_pExplosion->s_vPosition = D3DXVECTOR3(m_mTM._41, m_mTM._42, m_mTM._43);
		int FrameMax = m_pExplosion->s_iFrameMaxX * m_pExplosion->s_iFrameMaxY;
		if (m_fTime >= (2.f / (float)FrameMax)){
			if (m_iFrameX == m_pExplosion->s_iFrameMaxX){
				m_iFrameY++;
				m_iFrameX = 0;
			}
			m_pExplosion->s_vTexTransform = D3DXVECTOR4(m_iFrameX,
				m_iFrameY,
				1.f / (float)m_pExplosion->s_iFrameMaxX,
				1.f / (float)m_pExplosion->s_iFrameMaxY);
			if ((m_iFrameX * m_iFrameY) == FrameMax){
				m_bDestroy = true;
				return;
			}
			m_iFrameX++;
			m_fTime = 0.0f;
		}
	}*/
}




/////////////////////////////////////////////////////////////////////////////////
void CCombatPlane_MachinGun::Render()
{
	/*if (!m_bLive){
		Sprite_Draw(m_pExplosion);
		return;
	}*/

	m_Sprite.s_vPosition = D3DXVECTOR3(m_mTM._41, m_mTM._42, m_mTM._43);
	m_Sprite.s_vTexTransform = D3DXVECTOR4(0.f, 0.f, 1.f, 1.f);
	Sprite_Draw(&m_Sprite);
}




/****************************** End of File "CCombatPlane_MachinGun.cpp" ******************************/