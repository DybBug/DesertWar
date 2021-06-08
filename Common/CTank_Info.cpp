#include "../DesertWar_Server/ServerIocp.h"
#include "CTank_Info.h"
#include "../DesertWar_Server/CTerrain_Server.h"
#include "CTank_Boomb_Info.h"
#include "../DesertWar_Server/St.h"
#include <time.h>

extern CServerIocp g_ServerIocp;
extern CTerrain_Server* g_pTerrain;

#define TANK_COM_SPEED		10.f

typedef struct ENEMY_POS {
	//	int id;					// 일단 이건 보류
	D3DXVECTOR3 pos;
	float dis;

} ENEMY_POS;

//ENEMY_POS EPos[5];

ENEMY_POS EPos[3];		// 전투기를 빼고 탱크만 따라다니기 위해

void CTank_Info::Update(float _fDTime)
{
	if (m_fHP <= 0)
		m_blive = FALSE;

	// 데이터 조립.
	int size = sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX) + sizeof(D3DXMATRIX) + sizeof(D3DXMATRIX) + sizeof(float);
	BYTE pData[204] = { 0, };

	memcpy(pData, &m_nClientID, sizeof(UINT));
	memcpy(pData + sizeof(UINT), &m_nType, sizeof(UINT));
	memcpy(pData + sizeof(UINT) + sizeof(UINT), &m_mBodyTM, sizeof(D3DXMATRIX));
	memcpy(pData + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX), &m_mHoodTM, sizeof(D3DXMATRIX));
	memcpy(pData + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX) + sizeof(D3DXMATRIX), &m_mGunBarrelTM, sizeof(D3DXMATRIX));
	memcpy(pData + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX) + sizeof(D3DXMATRIX) + sizeof(D3DXMATRIX), &m_fHP, sizeof(float));
	// 전송.
	g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(SOBJECT_INFO, pData, size);

}

void CTank_Info::ComUpdate(float _fDTime)
{
	if (m_fHP <= 0)
		m_blive = FALSE;

	m_nType = OBJ_TANK;

	// 컴퓨터 인공지능...
	int myTeam = g_ServerIocp.GetConnectUserManager()->GetConnectUser(9 - (m_nClientID - 1))->GetTeam();
	int otherTeam = 0;

	CObject_Info* pTempTarget;
	float newDis = 0.f;
	float minDis = 0.f;

	float bodyRot = 0.f;

	int j = 0;

	switch (m_iState) {
	case STATE_TARGET_SEARCHING:
	{
		for (int i = MAX_SESSION - 1; i >= 0; --i){
			if (g_ServerIocp.GetConnectUserManager()->GetConnectUser(i)->GetTeam() == myTeam) continue; // 같은팀이면 건너뛰기.
			if (i == 9 - (m_nClientID - 1)) continue; // 자기 자신이면 건너 뛰기.
			if ((*(g_ServerIocp.GetConnectUserManager()->GetConnectUser(i)->GetObject_InfoList(0))) == NULL) continue; //객체가 없으면 건너뛰기

			if (g_ServerIocp.GetConnectUserManager()->GetConnectUser(i)->GetCharacter() == TANK) 
				pTempTarget = *(g_ServerIocp.GetConnectUserManager()->GetConnectUser(i)->GetObject_InfoList(0));
			else
				continue;
			++j;

			// 타겟과의 거리.
			if (pTempTarget != NULL)
				newDis = D3DXVec3Length(&(pTempTarget->GetPosition() - m_vPos));
			if (j == 1) //첫 루프
			{
				minDis = newDis;
				m_pTarget = pTempTarget;
			}

			if (minDis >= newDis)
			{
				minDis = newDis;
				m_pTarget = pTempTarget;
			}
		}
		
		if (minDis < m_fSearchRange)
		{

			if (minDis < m_fAttackRange)
			{
				m_bAttackReady = true;
				m_iState = STATE_TARGET_SEARCH;
				break;
			}
			else
			{
				m_bAttackReady = false;
				m_iState = STATE_TARGET_SEARCH;
			}
		}
		else
			m_pTarget = NULL;
		
		// 넥서스 공격
		if (m_pTarget == NULL)
		{
			if (myTeam == RED_TEAM)
			{
				m_pTarget = St::Instance()->BlueNexus;
				float Dis = D3DXVec3Length(&(m_pTarget->GetPosition() - m_vPos));
				if (m_fNexusSearchRange > Dis)
				{
					if (m_bAttackReady > Dis)
					{
						m_bAttackReady = true;
					}
					else
					{
						m_bAttackReady = false;
					}
					m_iState = STATE_TARGET_SEARCH;
					break;
				}
				else
				{
					m_bAttackReady = false;
					m_pTarget = NULL;
					m_iState = STATE_TARGET_NONSEARCH;
					break;
				}
			}
			else if (myTeam == BLUE_TEAM)
			{
				m_pTarget = St::Instance()->RedNexus;
				float Dis = D3DXVec3Length(&(m_pTarget->GetPosition() - m_vPos));
				if (m_fNexusSearchRange > Dis)
				{
					if (m_bAttackReady > Dis)
					{
						m_bAttackReady = true;
					}
					else
					{
						m_bAttackReady = false;
					}
					m_iState = STATE_TARGET_SEARCH;
					break;
				}
				else
				{
					m_bAttackReady = false;
					m_pTarget = NULL;
					m_iState = STATE_TARGET_NONSEARCH;
					break;
				}
			}
		}
		break;
	}
	case STATE_TARGET_SEARCH:
	{
		m_vRandomPointPos = m_pTarget->GetPosition();
		m_vRandomPointDir = m_vRandomPointPos - m_vPos;
		D3DXVec3Normalize(&m_vRandomPointDir, &m_vRandomPointDir);

		m_vFront = m_vRandomPointDir;

		//		m_iState = STATE_BODY_ROTATE;
		m_iState = STATE_MOVE;
		break;
	}
	case STATE_TARGET_NONSEARCH:
	{
		// 랜덤값으로...이동;
		if (m_bTargetArrive == true) {

			D3DXVec3Normalize(&m_vNexusVector, &m_vNexusVector);
			if (myTeam == BLUE_TEAM) {
				m_vNexusVector = St::Instance()->RedNexus->GetPosition();
				D3DXVec3Normalize(&m_vNexusVector, &(m_vNexusVector - m_vPos));
			}
			else {
				m_vNexusVector = St::Instance()->BlueNexus->GetPosition();
				D3DXVec3Normalize(&m_vNexusVector, &(m_vNexusVector - m_vPos));
			}

			do
			{
				m_vRandomPointPos.x = m_vPos.x + (rand() % 50 - 15);
				m_vRandomPointPos.z = m_vPos.z + (rand() % 50 - 15);
				m_vRandomPointPos.y = 0.0f;

				D3DXVec3Normalize(&m_vRandomPointDir, &(m_vPos - m_vRandomPointPos));

			} while (D3DXVec3Dot(&m_vNexusVector, &m_vRandomPointDir) < 0.f);

			m_bTargetArrive = false;

		}

		m_vFront = m_vRandomPointDir;
		m_vFront.y = 0.0f;

		m_iState = STATE_MOVE;
		//m_iState = STATE_BODY_ROTATE;
		break;
	}
	case STATE_BODY_ROTATE:
	{
		bodyRot = acos(D3DXVec3Dot(&m_vFront, &m_vRandomPointDir) / ((D3DXVec3Length(&m_vFront) * D3DXVec3Length(&m_vRandomPointDir))));

		if (bodyRot > 0)
			m_fBodyAngle += m_fRspeed * _fDTime;
		else
			m_fBodyAngle -= m_fRspeed * _fDTime;

		// 회전 데이터 전송.(m_fBodyAngle)
		int size = sizeof(UINT) + sizeof(float) + sizeof(float);
		BYTE pData[12] = { 0, };

		memcpy(pData, &m_nClientID, sizeof(UINT));
		memcpy(pData + sizeof(UINT), &m_fBodyAngle, sizeof(float));
		memcpy(pData + sizeof(UINT) + sizeof(float), &m_fHP, sizeof(float));


		// 전송.
		g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(STANK_BODY_ROT, pData, size);

		if (m_fBodyAngle >= bodyRot)
			m_iState = STATE_MOVE;
	}
		break;
	case STATE_MOVE:
	{
		//공격 범위 검사
		if (m_pTarget != NULL)
		{
			//if (m_fAttackRange >= D3DXVec3Length(&(m_vPos - m_pTarget->GetPosition()))){
			if (m_bAttackReady == true)
			{
				DT += _fDTime;

				if (DT >= 0.5f){
					m_iState = STATE_ATTACK;
					DT = 0.0f;
					break;
				}
			}
		}
		D3DXVECTOR3 FrontData = m_vFront;

		if (m_bAttackReady == false)
		{
			float Width, Depth;
			Width = St::Instance()->TerrainWidth;
			Depth = St::Instance()->TerrainDepth;
			if (!(((m_vPos.x < Width / 2.f) && (m_vPos.x > -Width / 2.f)) && ((m_vPos.z < Depth / 2.f) && (m_vPos.z > -Depth / 2.f))))
			{
				if (myTeam == RED_TEAM)
				{
					m_pTarget = St::Instance()->BlueNexus;
					m_vFront = m_pTarget->GetPosition() - m_vPos;
					D3DXVec3Normalize(&m_vFront, &m_vFront);
				}
				else if (myTeam == BLUE_TEAM)
				{
					m_pTarget = St::Instance()->RedNexus;
					m_vFront = m_pTarget->GetPosition() - m_vPos;
					D3DXVec3Normalize(&m_vFront, &m_vFront);
				}

				m_iState = STATE_MOVE;
				break;
			}
			// 이전 위치 저장
			m_vPrevPos = m_vPos;

			m_vPos += m_vFront * m_fSpeed * _fDTime;
			m_vPos.y = g_pTerrain->GetHeight(m_vPos);

			// 높이에따른 전방벡터 계산
			if (m_vPrevPos.y != m_vPos.y){
				FrontData = m_vPos - m_vPrevPos;
				D3DXVec3Normalize(&FrontData, &FrontData);
			}
			
			float mylen = D3DXVec3Length(&m_vPos);
			float targetLen = D3DXVec3Length(&m_vRandomPointPos);

			if (mylen >= targetLen){
				m_bTargetArrive = true;
			}
			
		}

		// 서치냐 논서치냐에 따라 타겟에 대한 처리 해줘야함(

		int size = sizeof(UINT) + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3) + sizeof(float);
		BYTE pData[36] = { 0, };

		memcpy(pData, &m_nClientID, sizeof(UINT));
		memcpy(pData + sizeof(UINT), &m_nType, sizeof(UINT));
		memcpy(pData + sizeof(UINT) + sizeof(UINT), &m_vPos, sizeof(D3DXVECTOR3));
		memcpy(pData + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXVECTOR3), &FrontData, sizeof(D3DXVECTOR3));
		memcpy(pData + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3), &m_fHP, sizeof(float));

		// 전송.
		g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(STANK_MOVE, pData, size);

		m_iState = STATE_TARGET_SEARCHING;
	}
		break;
	case STATE_ATTACK:
	{
		if (m_pTarget == NULL)
		{
			m_iState = STATE_TARGET_SEARCHING;
			break;
		}

		else if (m_pTarget != NULL)
		{
			if (m_pTarget->GetHP() <= 0.f)
			{
				m_pTarget = NULL;
				break;
			}
			m_vGunBarrelDir = m_pTarget->GetPosition() - m_vPos; // 목표점 - 방향벡터
			/*m_vGunBarrelDir.x *= 1.5;
			m_vGunBarrelDir.z *= 1.5;*/
			D3DXVec3Normalize(&m_vGunBarrelDir, &m_vGunBarrelDir);

			m_vGunBarrelPos = D3DXVec3Length(&D3DXVECTOR3(0.f, 1.f, 6.f)) * m_vGunBarrelDir + m_vPos;

			CConnectedUser* pConnectedUser = reinterpret_cast<CConnectedUser*>(g_ServerIocp.GetConnectUserManager()->GetConnectUser(9 - (m_nClientID - 1)));
			if (*(pConnectedUser)->GetObject_InfoList(0) == NULL) break;

			for (UINT i = 1; i < BULLET_MAX + 1; ++i){
				if (*pConnectedUser->GetObject_InfoList(i) != NULL) continue;
				(*pConnectedUser->GetObject_InfoList(i)) = new CTank_Boomb_Info();
				((CTank_Boomb_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetClientID(m_nClientID);
				((CTank_Boomb_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetPosition(&m_vGunBarrelPos);
				((CTank_Boomb_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetFront(&m_vGunBarrelDir);
				((CTank_Boomb_Info*)(*pConnectedUser->GetObject_InfoList(i)))->SetID(i - 1);
				break;
			}
			
			int size = sizeof(UINT) + sizeof(D3DXVECTOR3);
			BYTE pData[16] = { 0, };
			memcpy(pData, &m_nClientID, sizeof(UINT));
			memcpy(pData + sizeof(UINT), &m_vFront, sizeof(D3DXVECTOR3));
			g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(STANK_ATTACK, pData, size);
		}

		m_iState = STATE_MOVE;
		break;
	}

	}
}

