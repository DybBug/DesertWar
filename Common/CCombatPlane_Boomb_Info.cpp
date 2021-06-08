#include "../DesertWar_Server//ServerIocp.h"
#include "CCombatPlane_Boomb_Info.h"
#include "CheckOBBCollision.h"
#include "../DesertWar_Server//St.h"

extern CServerIocp g_ServerIocp;

/////////////////////////////////////////////////////////////////////////////////////////
//  CCombatPlane_MachinGun_Info()
CCombatPlane_Boomb_Info::CCombatPlane_Boomb_Info()

{
	m_fPower = 10.0f;
	m_fSpeed = 10.f;

	m_fDeathTime = 0.0f;
}




/////////////////////////////////////////////////////////////////////////////////////////
// ~CCombatPlane_MachinGun_Info()
CCombatPlane_Boomb_Info::~CCombatPlane_Boomb_Info()
{

}



/////////////////////////////////////////////////////////////////////////////////////////
// Update()
void CCombatPlane_Boomb_Info::Update(float _fDTime)
{

	m_fDeathTime += _fDTime;	

	m_vPos.y -= m_fSpeed * _fDTime;
	if (m_vPos.y <= 0.0f) m_blive = false;

	D3DXMatrixTranslation(&m_mTM, m_vPos.x, m_vPos.y, m_vPos.z);

	if (m_blive){
		ST_OBB object, missile, nexus;

		//	int i = GetClientID();
		int cnt = St::Instance()->ClientCnt;

		int myTeam = g_ServerIocp.GetConnectUserManager()->GetConnectUser(9 - (GetClientID() - 1))->GetTeam(); // 총알의 속한 객체의 팀을 가져옴

		missile.vCenterPos.y = m_vPos.y;
		missile.vCenterPos.x = m_vPos.x;
		missile.vCenterPos.z = m_vPos.z;

		missile.vAxisDir[0] = m_vFront;
		missile.vAxisDir[1] = m_vHoriz;
		missile.vAxisDir[2] = m_vUp;

		missile.fAxisLen[0] = 5.f;
		missile.fAxisLen[1] = 5.f;
		missile.fAxisLen[2] = 5.f;

		missile.type = OBJ_BULLET;		// 자신의 타입을 총알(공격) 오브젝트로 설정

		for (int j = MAX_SESSION - 1; j >= 0; j--)
		{
			CConnectedUser* ConnectedUser = g_ServerIocp.GetConnectUserManager()->GetConnectUser(j);

			if ((*ConnectedUser->GetObject_InfoList(0)) == NULL)
				continue;
			if (myTeam == g_ServerIocp.GetConnectUserManager()->GetConnectUser(j)->GetTeam()) // 같은 팀이라면
				continue;

			object.vCenterPos.x = (*ConnectedUser->GetObject_InfoList(0))->GetPosition().x;
			object.vCenterPos.y = (*ConnectedUser->GetObject_InfoList(0))->GetPosition().y;
			object.vCenterPos.z = (*ConnectedUser->GetObject_InfoList(0))->GetPosition().z;

			object.vAxisDir[0] = (*ConnectedUser->GetObject_InfoList(0))->GetFront();
			object.vAxisDir[1] = (*ConnectedUser->GetObject_InfoList(0))->GetHoriz();
			object.vAxisDir[2] = (*ConnectedUser->GetObject_InfoList(0))->GetUp();

			(*ConnectedUser->GetObject_InfoList(0))->GetSize(object.fAxisLen[0], object.fAxisLen[1], object.fAxisLen[2]);


			if (CheckOBBCollision(&missile, &object))
			{
				//printf("충돌 \n");
				float fHP = (*ConnectedUser->GetObject_InfoList(0))->GetHP() - m_fPower;
				(*ConnectedUser->GetObject_InfoList(0))->SetHP(fHP);

				printf("아이디 : %d, 체력 : %f \n", m_nClientID, fHP);


				m_blive = false;
			}
		}

		// 넥서스와의 충돌

		if (myTeam == RED_TEAM)
		{
			nexus.vCenterPos.x = St::Instance()->BlueNexus->GetPosition().x;
			nexus.vCenterPos.y = St::Instance()->BlueNexus->GetPosition().y;
			nexus.vCenterPos.z = St::Instance()->BlueNexus->GetPosition().z;

			nexus.vAxisDir[0] = St::Instance()->BlueNexus->GetFront();
			nexus.vAxisDir[1] = St::Instance()->BlueNexus->GetHoriz();
			nexus.vAxisDir[2] = St::Instance()->BlueNexus->GetUp();

			nexus.fAxisLen[0] = 20.f;
			nexus.fAxisLen[1] = 90.f;
			nexus.fAxisLen[2] = 20.f;

			if (CheckOBBCollision(&missile, &nexus))
			{
				float fHP = St::Instance()->BlueNexus->GetNexusHP() - m_fPower;
				St::Instance()->BlueNexus->SetNexusHP(fHP);

				printf("BLUE : 아이디 : %d, 체력 : %f \n", St::Instance()->BlueNexus->GetNexusID(), fHP);

				m_blive = false;
			}
		}
		else if (myTeam == BLUE_TEAM)
		{
			nexus.vCenterPos.x = St::Instance()->RedNexus->GetPosition().x;
			nexus.vCenterPos.y = St::Instance()->RedNexus->GetPosition().y;
			nexus.vCenterPos.z = St::Instance()->RedNexus->GetPosition().z;

			nexus.vAxisDir[0] = St::Instance()->RedNexus->GetFront();
			nexus.vAxisDir[1] = St::Instance()->RedNexus->GetHoriz();
			nexus.vAxisDir[2] = St::Instance()->RedNexus->GetUp();

			nexus.fAxisLen[0] = 20.f;
			nexus.fAxisLen[1] = 90.f;
			nexus.fAxisLen[2] = 20.f;

			if (CheckOBBCollision(&missile, &nexus))
			{
				float fHP = St::Instance()->RedNexus->GetNexusHP() - m_fPower;
				St::Instance()->RedNexus->SetNexusHP(fHP);

				printf("RED : 아이디 : %d, 체력 : %f \n", St::Instance()->RedNexus->GetNexusID(), fHP);

				m_blive = false;
			}
		}
	}

	// 데이터 결합.(클라 아이디 + 객체 아이디 + 행렬)
	int size = sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX) + sizeof(bool);
	BYTE pData[73] = { 0, };
	memcpy(pData, &m_nClientID, sizeof(UINT));
	memcpy(pData + sizeof(UINT), &m_nID, sizeof(UINT));
	memcpy(pData + sizeof(UINT) + sizeof(UINT), &m_mTM, sizeof(D3DXMATRIX));
	memcpy(pData + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX), &m_blive, sizeof(bool));

	// 데이터 전송.
	g_ServerIocp.GetConnectUserManager()->WriteGameClientAll(SCOMBATPLANE_BOOMB_TM, pData, 73);

}

void CCombatPlane_Boomb_Info::ComUpdate(float _fDTime)
{

	Update(_fDTime);

}