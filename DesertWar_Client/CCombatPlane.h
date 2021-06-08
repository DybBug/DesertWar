//***************************************************************************************
//
//	CCombatPlane.h : 전투기 관련 헤더 파일.
//
//***************************************************************************************
#pragma once
#include "Common.h"
#include "CObject.h"
#include "CCombatPlane_MachinGun.h"
#include "CCombatPlane_Boomb.h"
#define BULLET_MAX 200



class CCombatPlane : public CObject
{
public :
	enum {
		STATE_STOP = 0,
		STATE_ROTATE,
		STATE_MOVE,
		STATE_FIRE,
		STATE_FIRE2,
		STATE_EXPLOSION,
	};

	enum{
		STATE_COM_SEARCHING = 0,
		STATE_COM_ATTACK1,
		STATE_COM_DEFAULT,

	};
public:
	CCombatPlane(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShader,
		CMeshNode* _pMesh, float _fMoveSpeed, float _fRotSpeed, D3DXVECTOR3 _vPos);
	virtual ~CCombatPlane();
protected:
	float m_fMoveSpeed;
	float m_fRotSpeed;
	float m_fAngleX;
	float m_fAngleY;
	float m_fAngleZ;

	// 머신건 관련 데이터(좌/우)
	D3DXVECTOR3 m_vLMachinGunPos;
	D3DXVECTOR3 m_vLMachinGunOrgPos;

	D3DXVECTOR3 m_vRMachinGunPos;
	D3DXVECTOR3 m_vRMachinGunOrgPos;

	

	// 머신건 탄 관련 데이터.
	UINT m_nMachineGunBullets_MaxCnt;		// 머신건 탄 갯수.
	UINT m_nMachineGunBullets_ReloadedCnt;	// 머신건 탄 장전된 갯수.
	UINT m_nMachineGunBullets_CurrCnt;		// 머신건 탄 현재 갯수.

	float m_fMachinGunBullets_ReloadTime;	// 머신건 탄 재장전 시간.
	float m_fMachinGunBullets_ReloadCurrTime;	// 머신건 탄 재장전 현재 시간.
	float m_fMachinGunRateofFire;			// 초당 발사 속도.
	float m_fMachinGunRateofFireTime;		//	..

	//
	float m_fBoombRateofFire;				
	float m_fBoombRateofFireTime;			

	CObject* m_pBulletList[BULLET_MAX];

	SPRITE* m_pExplosion;
	float m_fTime;
	UINT m_iFrameX;
	UINT m_iFrameY;
	
	// 인공 지능 관련 변수.
	float m_fTurningRadius; // 선회 반경.
	float m_fTurningAngle; // 선회 각도.


protected:
	void _Draw(CMeshNode* _pMesh);
	void _ShadowDraw(CMeshNode* _pMesh);
	virtual void _UpdateByState() {};
	void _UpdateByState(float _fDTime);
	void _UpdateByComState(float _fDTime);

	void _UpdateMachinGunPos();

public:
	// 새로운 방향을 받아서 회전.
	void Rotate(D3DXVECTOR3* _pNewFront);

	void Control(float _fDTime);
	virtual void Update(float _fDTime);
	virtual void Render();
	virtual void ShadowRender(CBuildShadowMapShader* _pShader);

	CObject** GetBulletList(int i) { return &m_pBulletList[i]; }

	void SetTurningRadius(float _fRadius) { m_fTurningRadius = _fRadius; }
	float GetTurningRadius() { return m_fTurningRadius; }

	void SetTurningAngle(float _fAngle) { m_fTurningAngle = _fAngle; }
	float GetTurningAngle() { return m_fTurningAngle; }


};


/****************************** End of File "CCombatPlane.h" ******************************/