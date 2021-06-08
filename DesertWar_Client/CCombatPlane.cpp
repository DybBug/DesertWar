//***************************************************************************************
//
//	CCombatPlane.cpp : 전투기 관련 소스 파일.
//
//***************************************************************************************
#include "CGameEngine.h"
#include "CCombatPlane.h"
#include "FBXParser.h"
#include "CConsole.h"
#include "RenderState.h"




/////////////////////////////////////////////////////////////////////////////////////////////
// CCombatPlane()
CCombatPlane::CCombatPlane(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShader,
	CMeshNode* _pMesh, float _fMoveSpeed, float _fRotSpeed, D3DXVECTOR3 _vPos)
	:CObject(_pDev, _pDevC, _pShader)
{
	m_nType = OBJECT_TYPE_COMBATPLANE;

	m_fMoveSpeed = _fMoveSpeed;
	m_fRotSpeed = _fRotSpeed;

	m_fAngleX = 0.0f;
	m_fAngleY = 0.0f;
	m_fAngleZ = 0.0f;

	m_vPos = _vPos;
	D3DXMatrixTranslation(&m_mTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	m_mTM = m_mTrans;

	m_fHP = 50.f;

	// 머신건 관련 데이터(좌/우)
	m_vLMachinGunPos =  D3DXVECTOR3(-2.f, 0.f, 0.f);
	m_vLMachinGunOrgPos = m_vLMachinGunPos;

	m_vRMachinGunPos = D3DXVECTOR3(2.f, 0.f, 0.f);
	m_vRMachinGunOrgPos = m_vRMachinGunPos;

	// 머신건 탄 관련 데이터.
	m_nMachineGunBullets_MaxCnt = 1000;
	m_nMachineGunBullets_ReloadedCnt = 200;
	m_nMachineGunBullets_CurrCnt = 0;
	m_fMachinGunBullets_ReloadTime = 3.0f;
	m_fMachinGunBullets_ReloadCurrTime = 0.0f;
	m_fMachinGunRateofFire = 10.f;
	m_fMachinGunRateofFireTime = 0.0f;

	m_fBoombRateofFire = 1.0f;
	m_fBoombRateofFireTime = 0.0f;

	m_pMesh = _pMesh;
	

	// 충돌 박스 초기화.
	m_pCollisionBox = new CCubeMesh(8.f, 2.f, 12.f);
	m_pCollisionBox->CreateBuffer(m_pDev);

	m_pExplosion = ResourceManager->Explosion();
	m_fTime = 0.0f;
	m_iFrameX = 0;
	m_iFrameY = 0;
	m_pExplosion->s_vTexTransform = D3DXVECTOR4(m_iFrameX,
												m_iFrameY,
												1.f / (float)m_pExplosion->s_iFrameMaxX,
												1.f / (float)m_pExplosion->s_iFrameMaxY);

	m_fTurningRadius = 10.f;
	m_fTurningAngle = 0.f;
}





/////////////////////////////////////////////////////////////////////////////////////////////
// ~CCombatPlane()
CCombatPlane::~CCombatPlane()
{

}




/////////////////////////////////////////////////////////////////////////////////////////////
// _Draw()
void CCombatPlane::_Draw(CMeshNode* _pMesh)
{
	if (_pMesh->GetParent() != NULL){
		m_pShader->SetTexture(_pMesh->GetDiffuseMapSRV());
		m_pShader->SetWorld(&(_pMesh->GetTM() * m_mTM));
		m_pShader->SettingDataToGPU(PASS_LIGHTING_TEXTURE);
		_pMesh->Render(m_pDevC);
	}
	UINT ccount = _pMesh->GetChildCount();
	for (UINT i = 0; i < ccount; ++i){
		_Draw(_pMesh->GetChild(i));
	}
}




/////////////////////////////////////////////////////////////////////////////////////////////
// _ShadowDraw()
void CCombatPlane::_ShadowDraw(CMeshNode* _pMesh)
{
	if (_pMesh->GetParent() != NULL){
		m_pShadowShader->SetWorld(&(_pMesh->GetTM() * m_mTM));
		m_pShadowShader->SettingDataToGPU();
		_pMesh->Render(m_pDevC);
	}
	UINT ccount = _pMesh->GetChildCount();
	for (UINT i = 0; i < ccount; ++i){
		_ShadowDraw(_pMesh->GetChild(i));
	}
}




/////////////////////////////////////////////////////////////////////////////////////////////
// _UpdateByState()
void CCombatPlane::_UpdateByState(float _fDTime)
{
	switch (m_nState){
	case STATE_STOP:
	{
		break;
	}
	case STATE_ROTATE :
	case STATE_MOVE:
	{
		// 데이터 서버에 전송.
		int size = sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXMATRIX); // 4Byte + 12Byte + 64Byte
		//+sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3);
		BYTE pData[116] = { 0, };

		memcpy(pData, &GameEngine->ClientNet().m_nClientID, sizeof(UINT));
		memcpy(pData + sizeof(UINT), &m_vPos, sizeof(D3DXVECTOR3));
		memcpy(pData + sizeof(UINT) + sizeof(D3DXVECTOR3), &m_mTM, sizeof(D3DXMATRIX));
		//memcpy(pData + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXMATRIX), &m_vFront, sizeof(D3DXVECTOR3));
		//memcpy(pData + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXMATRIX) + sizeof(D3DXVECTOR3), &m_vHoriz, sizeof(D3DXVECTOR3));
		//memcpy(pData + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXMATRIX) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3), &m_vUp, sizeof(D3DXVECTOR3));


		CGameEngine::Instance()->ClientNet().WritePacket(CCOMBATPLANE_POS, pData, size);
		m_nState = STATE_STOP;
		break;
	}
	case STATE_FIRE:
	{
		//CConsole::Write("전투기 공격1 발사\n");
		// 서버에 데이터 전송.			
		int size = sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3);

		BYTE pData[40] = { 0, };

		*(UINT*)pData = GameEngine->ClientNet().m_nClientID;
		*(D3DXVECTOR3*)(pData + sizeof(UINT)) = m_vLMachinGunPos;
		*(D3DXVECTOR3*)(pData + sizeof(UINT) + sizeof(D3DXVECTOR3)) = m_vRMachinGunPos;
		*(D3DXVECTOR3*)(pData + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3)) = m_vFront;

		GameEngine->ClientNet().WritePacket(CCOMBATPLANE_MACHINGUN_CREATE, pData, size);

		m_nState = STATE_STOP;
		break;
	}
	case STATE_FIRE2:
	{
		//CConsole::Write("전투기 공격2 발사 \n");
		// 서버에 데이터 전송.
		int size = sizeof(UINT) + sizeof(D3DXVECTOR3);
		BYTE pData[16] = { 0, };

		*(UINT*)pData = GameEngine->ClientNet().m_nClientID;
		*(D3DXVECTOR3*)(pData + sizeof(UINT)) = m_vPos;

		GameEngine->ClientNet().WritePacket(CCOMBATPLANE_BOOMB_CREATE, pData, size);
		m_nState = STATE_STOP;
		break;
	}

	case STATE_EXPLOSION:
	{
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
				m_bLive = false;
				int cnt = GameEngine->LiveUnitCnt(m_nTeam);
				cnt -= 1;
				m_nState = STATE_STOP;
				break;
			}
			m_iFrameX++;
			m_fTime = 0.0f;
		}
		break;
	}

	}
}




/////////////////////////////////////////////////////////////////////////////////////////////
// _UpdateByComState()
// 인공지능 업데이트.
void CCombatPlane::_UpdateByComState(float _fDTime)
{
	switch (m_nState){
	case STATE_COM_SEARCHING:
	{
		D3DXVECTOR3 vNexusPos = { 0.f, 0.f, 0.f };
		if (m_nTeam == RED_TEAM){
			vNexusPos = (ObjectManager->ObjectList()[11])->GetPosition();
		}
		else{
			vNexusPos = (ObjectManager->ObjectList()[10])->GetPosition();
		}

		// 2-1. 선회 중심(본진)에서 유닛까지의 벡터 구하기.
		D3DXVECTOR3 vDir = m_vPos - vNexusPos;
		// 2-2. 2-1에서 구한 벡터와 벡터(0.f, 1.f, 0.f)와 외적하여 유닛의 수정된 전방 벡터 계산.
		D3DXVECTOR3 vNewFront; 
		D3DXVec3Cross(&vNewFront, &vDir, &D3DXVECTOR3(0.f, 1.f,0.f));
		D3DXVec3Normalize(&vNewFront, &vNewFront);
		// 2-3. 이전 전방 벡터와 수정된 전방벡터의 각도를 구하여 그 각도만큼 회전한 회전 행렬 계산.
		float angle = 0.0f;
		angle = acos(D3DXVec3Dot(&D3DXVECTOR3(0.f, 0.f, 1.f), &vNewFront) / (D3DXVec3Length(&D3DXVECTOR3(0.f, 0.f, 1.f)) * D3DXVec3Length(&vNewFront)));
		
		D3DXVECTOR3 vTempVector;
		D3DXVec3Cross(&vTempVector, &D3DXVECTOR3(0.f, 0.f, 1.f), &vNewFront);

		if (vTempVector.y < 0.f) angle = -angle;

		D3DXMATRIX mRotY;
		D3DXMatrixRotationY(&mRotY, angle);
		// 2-4. 2-3에서 구한 회전 행렬을 이용하여 전방 유닛의 3축 회전.
		D3DXVec3TransformCoord(&m_vFront, &D3DXVECTOR3(0.f, 0.f, 1.f), &mRotY);
		//D3DXVec3TransformCoord(&m_vFront, &m_vFront, &mRotY);

		D3DXMATRIX mTrans; D3DXMatrixTranslation(&mTrans, m_vPos.x, m_vPos.y, m_vPos.z);

		_UpdateMachinGunPos();

		m_mTM = mRotY * mTrans;
		break;
	}
	case STATE_COM_ATTACK1:
	{
		bool LBullet = false;
		bool RBullet = false;
		for (int i = 0; i < BULLET_MAX; ++i){
			if (RBullet && LBullet) break;
			if (m_pBulletList[i] != NULL) continue;

			// 왼쪽.
			if (!LBullet){
				m_pBulletList[i] = new CCombatPlane_MachinGun(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext(), ShaderManager->GetBasicShader(),
					ResourceManager->TankBoomb(), m_vLMachinGunPos, m_vFront);
				LBullet = true;
				continue;
			}

			// 오른쪽.
			if (!RBullet){
				m_pBulletList[i] = new CCombatPlane_MachinGun(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext(), ShaderManager->GetBasicShader(),
					ResourceManager->TankBoomb(), m_vRMachinGunPos, m_vFront);
				RBullet = true;
				continue;
			}
		}
		break;
	}
	case STATE_COM_DEFAULT:
	{
		Rotate(&m_vFront);
		break;
	}
	}
}




/////////////////////////////////////////////////////////////////////////////////////////////
// _UpdateMachinGunPos()
void CCombatPlane::_UpdateMachinGunPos()
{
	D3DXVECTOR3 vHoriz; // 수평 벡터(업벡터에 수직인 벡터)
	D3DXVec3Cross(&vHoriz, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_vFront); //수평 벡터 구하기.
	m_vLMachinGunPos = (-2.f * vHoriz) + m_vPos;
	m_vRMachinGunPos = (+2.f * vHoriz) + m_vPos;
}




//////////////////////////////////////////////////////////////////////////////////////////
// RotateY()
void CCombatPlane::Rotate(D3DXVECTOR3* _pNewFront)
{
	m_vFront = *_pNewFront;

	// 회전 각도 계산.
	float angle = acos(D3DXVec3Dot(&D3DXVECTOR3(0.f, 0.f, 1.f), &m_vFront) / (D3DXVec3Length(&D3DXVECTOR3(0.f, 0.f, 1.f)) * D3DXVec3Length(&m_vFront)));
	D3DXVECTOR3 vTempVector;
	D3DXVec3Cross(&vTempVector, &D3DXVECTOR3(0.f, 0.f, 1.f), &m_vFront);
	//if (vTempVector.y < 0.f) angle = -angle;

	D3DXMatrixRotationAxis(&m_mRot, &vTempVector, angle);

	// 머신건 위치 업데이트.
	_UpdateMachinGunPos();

	m_mTM = m_mRot * m_mTrans;
}



//////////////////////////////////////////////////////////////////////////////////////////
// Control()
void CCombatPlane::Control(float _fDTime)
{
	if (m_fHP <= 0.f ){
		m_nState = STATE_EXPLOSION;
		_UpdateByState(_fDTime);
		return;
	}
	D3DXVECTOR3 Angle = D3DXVECTOR3(0.f, 0.f, 0.f);
	POINT temp;
	GameEngine->Input().GetMouseRelativePt(&temp.x, &temp.y);
	
//	if (temp.x != 0 && temp.y != 0){
	Angle.x = (-temp.x * m_fRotSpeed * _fDTime);
	Angle.z = (-temp.y * m_fRotSpeed * _fDTime);
//	m_nState = STATE_ROTATE;
//	}

	if (GetAsyncKeyState('W') & 0x8000){
		m_vPos += m_vFront * m_fMoveSpeed * _fDTime;
		m_nState = STATE_MOVE;
	}
	//if (GetAsyncKeyState('S') & 0x8000){
	//	m_vPos -= m_vFront * m_fMoveSpeed * _fDTime;
	//	m_nState = STATE_MOVE;

	//}
	if (GetAsyncKeyState('A') & 0x8000){
		Angle.y = -m_fRotSpeed * _fDTime * 5;
		m_nState = STATE_MOVE;
	}
	if (GetAsyncKeyState('D') & 0x8000){
		Angle.y= +m_fRotSpeed * _fDTime * 5;
		m_nState = STATE_MOVE;
	}

	m_fMachinGunRateofFireTime += _fDTime;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000){
		if (m_fMachinGunRateofFireTime >= (1.0f / m_fMachinGunRateofFire)){
			m_nState = STATE_FIRE;
			m_fMachinGunRateofFireTime = 0.0f;
		}
	}

	m_fBoombRateofFireTime += _fDTime;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000){
		if (m_fBoombRateofFireTime >= m_fBoombRateofFire){
			m_nState = STATE_FIRE2;
			m_fBoombRateofFireTime = 0.0f;
		}

	}
	m_vPos.y -= 2.5f * _fDTime;
	if (m_vPos.y < 0.0f)
		m_vPos.y = 0.0f;



	m_mRot *= _LAxisRot(&m_vFront, &m_vHoriz, &m_vUp, Angle);

	D3DXMatrixTranslation(&m_mTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	m_mTM = m_mRot * m_mTrans;

	// 머신건 관련 데이터 업데이트.
	D3DXVec3TransformCoord(&m_vLMachinGunPos, &m_vLMachinGunOrgPos, &m_mTM);
	D3DXVec3TransformCoord(&m_vRMachinGunPos, &m_vRMachinGunOrgPos, &m_mTM);

	_UpdateByState(_fDTime);

	for (UINT i = 0; i < BULLET_MAX; ++i){
		if (m_pBulletList[i] == NULL) continue;
		//if (m_pBulletList[i]->GetDestory()){
		if (!m_pBulletList[i]->GetLive()){
			SAFE_DELETE(m_pBulletList[i]);
			//CConsole::Write("총알 삭제!!\n");
			continue;
		}
		m_pBulletList[i]->Update(_fDTime);
	}
}




/////////////////////////////////////////////////////////////////////////////////////////////
// Update()
void CCombatPlane::Update(float _fDTime)
{
	if (m_fHP <= 0.f){
		m_nState = STATE_EXPLOSION;
		_UpdateByState(_fDTime);
	}

	if (m_bComputer)
		_UpdateByComState(_fDTime);

	for (UINT i = 0; i < BULLET_MAX; ++i){
		if (m_pBulletList[i] == NULL) continue;
		//if (m_pBulletList[i]->GetDestory()){
			if (!m_pBulletList[i]->GetLive()){
			SAFE_DELETE(m_pBulletList[i]);
			//CConsole::Write("총알 삭제!!\n");
			continue;
		}
		m_pBulletList[i]->Update(_fDTime);
	}
}




//////////////////////////////////////////////////////////////////////////////////////////////
// Render()
void CCombatPlane::Render()
{
	if (m_nState == STATE_EXPLOSION){
		Sprite_Draw(m_pExplosion);
		return;
	}
	MATERIAL m;
	if (m_fPrevHP == m_fHP)		
		m.s_cDiffuse = m.s_cAmbient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	else
		m.s_cDiffuse = m.s_cAmbient = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	 m_fPrevHP = m_fHP;

	m_pShader->SetMaterial(&m);
	_Draw(m_pMesh);

	for (UINT i = 0; i < BULLET_MAX; ++i){
		if (m_pBulletList[i] == NULL) continue;
		m_pBulletList[i]->Render();
	}


	if(GameEngine->CollisionBox() == true){
		m_pDevC->RSSetState(ResterRizerState::m_pWireframeRS);

		m.s_cDiffuse = m.s_cAmbient = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		m_pShader->SetMaterial(&m);
		m_pShader->SetWorld(&m_mTM);
		m_pShader->SettingDataToGPU(PASS_LIGHTING);

		m_pCollisionBox->Render(m_pDevC);
		m_pDevC->RSSetState(ResterRizerState::m_pSolidRS);

	}
}




//////////////////////////////////////////////////////////////////////////////////////////////
//	ShadowRender()
void CCombatPlane::ShadowRender(CBuildShadowMapShader* _pShader)
{
	m_pShadowShader = _pShader;

	_ShadowDraw(m_pMesh);
}

/****************************** End of File "CCombatPlane.cpp" ******************************/