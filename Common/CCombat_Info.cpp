#include "../DesertWar_Server//ServerIocp.h"
#include "CCombat_Info.h"
#include "CheckOBBCollision.h"
#include "CCombatPlane_MachinGun_Info.h"
#include "CCombatPlane_Boomb_Info.h"
#include "../DesertWar_Server//St.h"

extern CServerIocp g_ServerIocp;

void CCombat_Info::Update(float _fDTime)
{

	if (m_fHP <= 0)
		m_blive = FALSE;

	// ������ ����.

	int size = sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX) + sizeof(float); // 72

	BYTE pData[76] = {0,};

	memcpy(pData, &m_nClientID, sizeof(UINT));
	memcpy(pData + sizeof(UINT), &m_nType, sizeof(UINT));
	memcpy(pData + sizeof(UINT) + sizeof(UINT), &m_mTM, sizeof(D3DXMATRIX));
	memcpy(pData + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX), &m_fHP, sizeof(float));
			
	// ����.
	g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(SOBJECT_INFO, pData, size);
}


void CCombat_Info::SenseTarget()
{
	if (m_pTarget != NULL){
		float fDis = D3DXVec3Length(&(m_vPos - m_pTarget->GetPosition()));
		if (fDis > m_fSenseRange) m_pTarget = NULL;
	}

	int myTeam = g_ServerIocp.GetConnectUserManager()->GetConnectUser(9 - (m_nClientID - 1))->GetTeam(); // ���� ��
	int myChar = g_ServerIocp.GetConnectUserManager()->GetConnectUser(9 - (m_nClientID - 1))->GetCharacter();

	CObject_Info* pTempTarget = NULL;

	for (UINT i = 0; i < MAX_SESSION; i++){
		if ((*(g_ServerIocp.GetConnectUserManager()->GetConnectUser(i)->GetObject_InfoList(0))) == NULL) continue;
		pTempTarget = *g_ServerIocp.GetConnectUserManager()->GetConnectUser(i)->GetObject_InfoList(0);
		if (pTempTarget == NULL) continue;
		if (pTempTarget->GetClientID() == m_nClientID) continue;
		if (g_ServerIocp.GetConnectUserManager()->GetConnectUser(i)->GetTeam() == myTeam) continue;

		// �Ÿ� ���
		float NewTargetDis = D3DXVec3Length(&(m_vPos - pTempTarget->GetPosition()));	// ���� Ž���� Ÿ������ �Ÿ�.

		// ���� ���� ��
		if (NewTargetDis <= m_fSenseRange){
			if (m_pTarget != NULL){
				float OldTargetDis = D3DXVec3Length(&(m_vPos - m_pTarget->GetPosition()));	// ���� Ÿ�ٰ��� �Ÿ�.
				if (NewTargetDis < OldTargetDis) m_pTarget = pTempTarget;
			}
			else{
				m_pTarget = pTempTarget;
			}
		}

	}
}

void CCombat_Info::ComUpdate(float _fDTime)
{

	// ������ �ΰ� ����...
	if (m_fHP <= 0){
		m_blive = FALSE;
		return;
	}

	switch (m_nState)
	{
	case STATE_TARGET_SERCHING:
	{
		D3DXVECTOR3 vNexusPos = { 0, 0, 0 };

		m_fAngle -= m_fRspeed * _fDTime;

		// ���� ��ġ(���� ����).
		int myTeam = g_ServerIocp.GetConnectUserManager()->GetConnectUser(9 - (m_nClientID - 1))->GetTeam();

		if (myTeam == RED_TEAM)
			vNexusPos = St::Instance()->RedNexus->GetPosition();
		else
			vNexusPos = St::Instance()->BlueNexus->GetPosition();


		// ĳ������ ��ġ ����.(������ ��ġ�κ��� ȸ�� ������ ��ŭ�� �Ÿ�).
		D3DXVECTOR3 Pos;
		m_vPos.x = m_fTurningRadius;
		m_vPos.y = m_fTurningRadius+10;
		m_vPos.z = m_fTurningRadius;

		D3DXMATRIX mRot; D3DXMatrixRotationY(&mRot, D3DXToRadian(m_fAngle));
		D3DXVec3TransformCoord(&m_vPos, &m_vPos, &mRot);

		D3DXMATRIX mTrans;
		D3DXMatrixTranslation(&mTrans, vNexusPos.x, vNexusPos.y, vNexusPos.z);
		D3DXVec3TransformCoord(&m_vPos, &m_vPos, &mTrans);


		// ������ ����(Ŭ�� ID, ü��, ��ġ)
		int size = sizeof(UINT) + sizeof(FLOAT) + sizeof(D3DXVECTOR3);
		BYTE pData[20] = { 0, };
		memcpy(pData, &m_nClientID, sizeof(UINT));
		memcpy(pData + sizeof(UINT), &m_fHP, sizeof(FLOAT));
		memcpy(pData + sizeof(UINT) + sizeof(FLOAT), &m_vPos, sizeof(D3DXVECTOR3));
		g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(SCOMBATPLANE_SERCHING, pData, size);

		// Ÿ��ã��.
		SenseTarget();
		if (m_pTarget != NULL)
			m_nState = STATE_TARGET_TRACE;
	}
		break; 
	
	case STATE_TARGET_TRACE:
	{
		DT += _fDTime;
		SenseTarget();
		if (m_pTarget == NULL){
			m_nState = STATE_RETURN;
			return;
		}

		m_enemyPos = m_pTarget->GetPosition();

		// Ÿ���� ��ũ�ϰ��.
		if (m_pTarget->GetType() == TANK)
		{
			m_vFront = m_enemyPos - m_vPos; // ��ǥ�� - ���⺤��
			m_vFront.y = 0.0f;
			D3DXVec3Normalize(&m_vFront, &m_vFront);

			//if (20.0f > D3DXVec3Length(&(m_vPos - m_pTarget->GetPosition())))
			if (abs(m_vPos.x - m_pTarget->GetPosition().x) < 5.0f && abs(m_vPos.z - m_pTarget->GetPosition().z) < 5.0f )
			{
				if (DT >= 1.5f){
					m_nState = STATE_COMBATBOOMB_ATTACK;
					DT = 0.0f;
				}
			}
			else{
				m_vPos += m_vFront * m_fSpeed * _fDTime;
			}
			if (m_vPos.y <= 40.f) m_vPos.y = 40.f;
		}
		// Ÿ���� �������� ���.
		else
		{
			m_vFront = m_enemyPos - m_vPos; // ��ǥ�� - ���⺤��
			D3DXVec3Normalize(&m_vFront, &m_vFront);

			if (100.0f > D3DXVec3Length(&(m_vPos - m_pTarget->GetPosition())))
			{
				if (DT >= 0.8f){
					m_nState = STATE_MACHINGUN_ATTACK;
					DT = 0.0f;
				}
			}
			else{
				m_vPos += m_vFront * m_fSpeed * _fDTime;
			}
		}

		
		int size = sizeof(UINT) + sizeof(FLOAT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3);
		BYTE pData[32] = { 0, };
		memcpy(pData, &m_nClientID, sizeof(UINT));
		memcpy(pData + sizeof(UINT), &m_fHP, sizeof(FLOAT));
		memcpy(pData + sizeof(UINT) + sizeof(FLOAT), &m_vPos, sizeof(D3DXVECTOR3));
		memcpy(pData + sizeof(UINT) + sizeof(FLOAT) + sizeof(D3DXVECTOR3), &m_vFront, sizeof(D3DXVECTOR3));
		g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(SCOMBATPLANE_TRACE, pData, size);

	}
		break;
	case STATE_MACHINGUN_ATTACK:
	{
		if (m_pTarget->Getlive() == TRUE)
		{
			CConnectedUser* pConnectedUser = reinterpret_cast<CConnectedUser*>(g_ServerIocp.GetConnectUserManager()->GetConnectUser(9 - (m_nClientID - 1)));
			if (*(pConnectedUser)->GetObject_InfoList(0) == NULL) break;

			m_vFront = m_enemyPos - m_vPos;
			D3DXVec3Normalize(&m_vFront, &m_vFront);

			bool bLMachinGun = false;
			bool bRMachinGun = false;

			D3DXVECTOR3 vHoriz; // ���� ����(�����Ϳ� ������ ����)
			D3DXVec3Cross(&vHoriz, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_vFront); //���� ���� ���ϱ�.
			m_vLMachinGunPos = (-2.f * vHoriz) + m_vPos;
			m_vRMachinGunPos = (+2.f * vHoriz) + m_vPos;


			int size = sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3);
			BYTE pData[28] = { 0, };
			memcpy(pData, &m_nClientID, sizeof(UINT));
			memcpy(pData + sizeof(UINT), &m_vLMachinGunPos, sizeof(D3DXVECTOR3));
			memcpy(pData + sizeof(UINT) + sizeof(D3DXVECTOR3), &m_vRMachinGunPos, sizeof(D3DXVECTOR3));

			for (UINT i = 1; i < BULLET_MAX + 1; ++i){
				if (*pConnectedUser->GetObject_InfoList(i) != NULL) continue;


				if ((bLMachinGun && bRMachinGun)){
					//==========================================================================================
					// ������ ����.
					//==========================================================================================
					printf("%d : ������ �ӽŰ� ���� \n", m_nClientID);
					g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(SCOMBATPLANE_MACHINGUN_ATTACK, pData, size);
					break;
				}

				//==========================================================================================
				//���� ���� �Ѿ�
				//==========================================================================================
				if (!bLMachinGun){
					(*pConnectedUser->GetObject_InfoList(i)) = new CCombatPlane_MachinGun_Info();
					((CCombatPlane_MachinGun_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetClientID(m_nClientID);
					((CCombatPlane_MachinGun_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetPosition(&m_vLMachinGunPos);
					((CCombatPlane_MachinGun_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetID(i - 1);
					((CCombatPlane_MachinGun_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetFront(&m_vFront);
					bLMachinGun = true;
					continue;
				}


				//==========================================================================================
				// ������ ���� �Ѿ�
				//==========================================================================================
				if (!bRMachinGun){
					(*pConnectedUser->GetObject_InfoList(i)) = new CCombatPlane_MachinGun_Info();
					((CCombatPlane_MachinGun_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetClientID(m_nClientID);
					((CCombatPlane_MachinGun_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetPosition(&m_vRMachinGunPos);
					((CCombatPlane_MachinGun_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetID(i - 1);
					((CCombatPlane_MachinGun_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetFront(&m_vFront);
					bRMachinGun = true;

					continue;
				}

			}
		}
	//	else if ((m_pTarget->Getlive() == FALSE) || 100.0f < D3DXVec3Length(&(m_vPos - m_pTarget->GetPosition())))
			m_nState = STATE_TARGET_TRACE;
	}
		break;
	case STATE_COMBATBOOMB_ATTACK:
	{
		DT += _fDTime;

		if (m_pTarget->Getlive() == TRUE)
		{
			CConnectedUser* pConnectedUser = reinterpret_cast<CConnectedUser*>(g_ServerIocp.GetConnectUserManager()->GetConnectUser(9 - (m_nClientID - 1)));
			if (*(pConnectedUser)->GetObject_InfoList(0) == NULL) break;

			//==========================================================================================
			// ����� ��ź
			//==========================================================================================
			for (UINT i = 1; i < BULLET_MAX + 1; ++i){
				if (*pConnectedUser->GetObject_InfoList(i) != NULL) continue;
				(*pConnectedUser->GetObject_InfoList(i)) = new CCombatPlane_Boomb_Info();
				((CCombatPlane_Boomb_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetClientID(m_nClientID);
				((CCombatPlane_Boomb_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetPosition(&m_vPos);
				((CCombatPlane_Boomb_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetID(i - 1);
				break;
			}
			//==========================================================================================
			// ������ ����.
			//==========================================================================================

			printf("������ ����2 �߻� \n");
			int size = sizeof(UINT) + sizeof(D3DXVECTOR3);
			BYTE pData[16] = { 0, };
			memcpy(pData, &m_nClientID, sizeof(UINT));
			memcpy(pData + sizeof(UINT), &m_vPos, sizeof(D3DXVECTOR3));

			g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(SCOMBATPLANE_MACHINGUN_ATTACK, pData, size);
			DT = 0;
		}
		//else if ((m_pTarget->Getlive() == FALSE) || (50.0f < D3DXVec3Length(&(m_vPos - m_pTarget->GetPosition()))))
		m_nState = STATE_TARGET_TRACE;
		break;
	}
		break;
	case STATE_RETURN:
	{
		D3DXVECTOR3 vNexusPos = { 0, 0, 0 };
		int myTeam = g_ServerIocp.GetConnectUserManager()->GetConnectUser(9 - (GetClientID() - 1))->GetTeam();

		if (myTeam == RED_TEAM)
			vNexusPos = St::Instance()->RedNexus->GetPosition();
		else
			vNexusPos = St::Instance()->BlueNexus->GetPosition();

		m_vFront = vNexusPos - m_vPos; // ��ǥ�� - ���⺤��
		D3DXVec3Normalize(&m_vFront, &m_vFront);
		m_vPos += m_vFront * m_fSpeed * _fDTime;
		//m_vPos.y = 25; // ���� 30���� ����

		int size = sizeof(UINT) + sizeof(FLOAT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3);
		BYTE pData[32] = { 0, };
		memcpy(pData, &m_nClientID, sizeof(UINT));
		memcpy(pData + sizeof(UINT), &m_fHP, sizeof(FLOAT));
		memcpy(pData + sizeof(UINT) + sizeof(FLOAT), &m_vPos, sizeof(D3DXVECTOR3));
		memcpy(pData + sizeof(UINT) + sizeof(FLOAT) + sizeof(D3DXVECTOR3), &m_vFront, sizeof(D3DXVECTOR3));
		g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(SCOMBATPLANE_RETRUN, pData, size);

		D3DXVECTOR3 Pos;
		Pos.x = m_vPos.x - vNexusPos.x;
		Pos.y = m_vPos.y - vNexusPos.y;
		Pos.z = m_vPos.z - vNexusPos.z;

		float Dis = D3DXVec3Length(&Pos);
		if (Dis <= m_fTurningRadius)
			m_nState = STATE_TARGET_SERCHING;
	}
		break;
	default:
		break;
	}

}