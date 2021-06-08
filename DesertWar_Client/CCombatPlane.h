//***************************************************************************************
//
//	CCombatPlane.h : ������ ���� ��� ����.
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

	// �ӽŰ� ���� ������(��/��)
	D3DXVECTOR3 m_vLMachinGunPos;
	D3DXVECTOR3 m_vLMachinGunOrgPos;

	D3DXVECTOR3 m_vRMachinGunPos;
	D3DXVECTOR3 m_vRMachinGunOrgPos;

	

	// �ӽŰ� ź ���� ������.
	UINT m_nMachineGunBullets_MaxCnt;		// �ӽŰ� ź ����.
	UINT m_nMachineGunBullets_ReloadedCnt;	// �ӽŰ� ź ������ ����.
	UINT m_nMachineGunBullets_CurrCnt;		// �ӽŰ� ź ���� ����.

	float m_fMachinGunBullets_ReloadTime;	// �ӽŰ� ź ������ �ð�.
	float m_fMachinGunBullets_ReloadCurrTime;	// �ӽŰ� ź ������ ���� �ð�.
	float m_fMachinGunRateofFire;			// �ʴ� �߻� �ӵ�.
	float m_fMachinGunRateofFireTime;		//	..

	//
	float m_fBoombRateofFire;				
	float m_fBoombRateofFireTime;			

	CObject* m_pBulletList[BULLET_MAX];

	SPRITE* m_pExplosion;
	float m_fTime;
	UINT m_iFrameX;
	UINT m_iFrameY;
	
	// �ΰ� ���� ���� ����.
	float m_fTurningRadius; // ��ȸ �ݰ�.
	float m_fTurningAngle; // ��ȸ ����.


protected:
	void _Draw(CMeshNode* _pMesh);
	void _ShadowDraw(CMeshNode* _pMesh);
	virtual void _UpdateByState() {};
	void _UpdateByState(float _fDTime);
	void _UpdateByComState(float _fDTime);

	void _UpdateMachinGunPos();

public:
	// ���ο� ������ �޾Ƽ� ȸ��.
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