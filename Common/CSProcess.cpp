//***************************************************************************************
//
//	CSProcess.cpp : ���� -> Ŭ�� ������ ���� �������� ���� �ҽ� ����.
//
//***************************************************************************************

#include "CGameEngine.h"
#include "CProcess.h"
#include "CConsole.h"

//#include "CCombatPlane.h"

namespace S_Protocol_Func{

	Func g_Protocol_Func[S_Protocol::SPROTOCOL_MAX] = {
		S_Protocol_Func::Connect_Complete_Func,
		S_Protocol_Func::DisCunnect,
		S_Protocol_Func::Enter_Waitroom_Func,
		S_Protocol_Func::Room_Leader_Func,
		S_Protocol_Func::Loading_Start_Func,
		S_Protocol_Func::Loading_Complete_Func,
		S_Protocol_Func::Ready_User_Func,
		S_Protocol_Func::Ready_Leader_Func,
		S_Protocol_Func::Object_Info_Func,
		S_Protocol_Func::Tank_Bomb_Create_Func,
		S_Protocol_Func::Tank_Bomb_TM_Func,
		S_Protocol_Func::Tank_SmokeBomb_Create_Func,
		S_Protocol_Func::Tank_SmokeBomb_TM_Func,

		S_Protocol_Func::CombatPlane_Machingun_Create_Func,
		S_Protocol_Func::CombatPlane_Machingun_TM_Func,

		S_Protocol_Func::CombatPlane_Boomb_Create_Func,
		S_Protocol_Func::CombatPlane_Boomb_TM_Func,

		S_Protocol_Func::ComInfo_Func,
		S_Protocol_Func::Char_Cnt_Func,

		S_Protocol_Func::Team_Cnt_Func,
		S_Protocol_Func::ComBatPlane_Serching_Func,
		S_Protocol_Func::ComBatPlane_Trace_Func,
		S_Protocol_Func::ComBatPlane_Return_Func,
		S_Protocol_Func::ComBatPlane_Machingun_Attack,
		S_Protocol_Func::ComBatPlane_Combatboomb_Attack,

		S_Protocol_Func::Tank_Move_Func,
		S_Protocol_Func::Tank_Body_Rot_Func,
		S_Protocol_Func::Tank_Attack_Func,
		
		S_Protocol_Func::Object_Death_Func,
		S_Protocol_Func::Bullet_Death_Func,

		S_Protocol_Func::Nexus_Info,

	};

	void Connect_Complete_Func(BYTE* _pPacket, DWORD& _Length)
	{
		//CConsole::Write("������ ������ �Ǿ����ϴ�.\n");
		//���� ���� �˷��ֱ�.
		BYTE Byte = 1;
		CGameEngine::Instance()->ClientNet().WritePacket(CENTER_WAITROOM, &Byte, 1);
		CWaitRoomScene* pWaitRoomScene = new CWaitRoomScene();
		CGameEngine::Instance()->SceneChange((CScene**)&pWaitRoomScene);

		return;
	}

	void DisCunnect(BYTE* _pPacket, DWORD& _Length)
	{
		//CConsole::Write("%d ������ ������ �����Ͽ����ϴ�.\n", *_pPacket);
		if (CGameEngine::Instance()->ClientNet().m_nClientID == 1)
			CGameEngine::Instance()->ClientNet().m_bLeader = true;
	}

	void Enter_Waitroom_Func(BYTE* _pPacket, DWORD& _Length)
	{
		if (CGameEngine::Instance()->ClientNet().m_nClientID != 0) return;

		//CConsole::Write("ID : %d Ŭ���̾�Ʈ �����߽��ϴ�.\n");
		if (CGameEngine::Instance()->ClientNet().m_nClientID == 0)
			CGameEngine::Instance()->ClientNet().m_nClientID = *_pPacket;

		CGameEngine::Instance()->ClientNet().m_nClientCnt = *(_pPacket + sizeof(UINT));
		//CConsole::Write("[ ���� �ο� ] : %d ��\n", CGameEngine::Instance()->ClientNet().m_nClientCnt);

		CGameEngine::Instance()->ClientNet().m_nClientTeam = *(_pPacket + sizeof(UINT) + sizeof(UINT));

		return;
	}
	 
	void Room_Leader_Func(BYTE* _pPacket, DWORD& _Length)
	{
		//CConsole::Write("%d Ŭ���̾�Ʈ�� �����Դϴ�.\n", *_pPacket);
		CGameEngine::Instance()->ClientNet().m_bLeader = TRUE;

		return;
	}

	void Loading_Start_Func(BYTE* _pPacket, DWORD& _Length)
	{
		//CConsole::Write("�ε��� �����մϴ�.\n");
		//CGameEngine::Instance()->ClientNet().m_bStart = TRUE;

		PLAYER_INFO* ppList = ObjectManager->CharacterList();
		memset(ppList, 0, sizeof(PLAYER_INFO) * MAX_SESSION);
		for (UINT i = 0; i < MAX_SESSION; ++i) {
			ppList[i].ClientID = ((PLAYER_INFO*)_pPacket)[i].ClientID;
			ppList[i].Character = ((PLAYER_INFO*)_pPacket)[i].Character;
			ppList[i].Team = ((PLAYER_INFO*)_pPacket)[i].Team;
			ppList[i].Computer = ((PLAYER_INFO*)_pPacket)[i].Computer;
			//ppList[i].Pos = ((PLAYER_INFO*)_pPacket)[i].Pos;
		}

		CLoadingScene* pLoadingScene = new CLoadingScene();
		CGameEngine::Instance()->SceneChange((CScene**)&pLoadingScene);

		return;
	}

	void Loading_Complete_Func(BYTE* _pPacket, DWORD& _Length)
	{
		for (UINT i = 0; i < MAX_SESSION; ++i) {
			D3DXVECTOR3 pos = ((D3DXVECTOR3*)_pPacket)[i];
			//CConsole::Write("%d : %f, %f, %f\n", i, pos.x, pos.y, pos.z);
			(ObjectManager->ObjectList()[i])->SetPosition(&((D3DXVECTOR3*)_pPacket)[i]);
		}
		//CConsole::Write("������ �����մϴ�.\n");
		CPlayScene* pPlayScene = new CPlayScene();
		CGameEngine::Instance()->SceneChange((CScene**)&pPlayScene);

		return;
	}

	void Ready_User_Func(BYTE* _pPacket, DWORD& _Length)
	{
		//CConsole::Write("%d Ŭ���̾�Ʈ�� �غ� �Ϸ� �Ǿ����ϴ�.\n", *_pPacket);
	}

	void Ready_Leader_Func(BYTE* _pPacket, DWORD& _Length)
	{
		//CConsole::Write("��� Ŭ���̾�Ʈ�� �غ� �Ϸ� �Ǿ����ϴ�.\n");
		CGameEngine::Instance()->ClientNet().m_bAllReady = true;
	}

	void Object_Info_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ClientID = *(UINT*)_pPacket;
		UINT Type = *(UINT*)(_pPacket + sizeof(UINT));

		CObject* Obj = ObjectManager->ObjectList()[ClientID - 1];

		if (Obj == NULL) return;

		if (Type == OBJ_TANK){
			D3DXMATRIX TM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT));
			D3DXMATRIX HoodTM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX));
			D3DXMATRIX GunBarrelTM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX) + sizeof(D3DXMATRIX));
			float HP = *(float*)(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX) + sizeof(D3DXMATRIX) + sizeof(D3DXMATRIX));

			((CTank*)Obj)->SetTM(&TM);
			((CTank*)Obj)->SetHoodTM(&HoodTM);
			((CTank*)Obj)->SetGunBarrelTM(&GunBarrelTM);
			((CTank*)Obj)->SetHP(HP);

		}
		else if (Type == OBJ_COMBATPLANE){
			// ������ ����.
			D3DXMATRIX TM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT));
			float HP = *(float*)(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX));


			((CCombatPlane*)Obj)->SetTM(&TM);
			((CCombatPlane*)Obj)->SetHP(HP);
		}
		return;
	}

	void Tank_Bomb_Create_Func(BYTE* _pPacket, DWORD& _Length)
	{
		//CConsole::Write("TANK_BOOMB_CREATE!!\n");
		UINT ID = *(UINT*)_pPacket;
		D3DXVECTOR3 Pos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT));
		D3DXVECTOR3 Dir = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(D3DXVECTOR3));

		CTank* Obj = ((CTank*)ObjectManager->ObjectList()[ID - 1]);

		if (Obj == NULL) return;

		for (UINT i = 0; i < BULLET_MAX; ++i){
			if ((*Obj->GetBulletList(i)) != NULL) continue;
			*(Obj->GetBulletList(i)) = new CTank_Bomb(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext(), ShaderManager->GetBasicShader(),
				ResourceManager->TankBoomb(), Pos, Dir);
			break;
		}

		return;
	}

	void Tank_Bomb_TM_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ID = *(UINT*)_pPacket;
		UINT ObjID = *(UINT*)(_pPacket + sizeof(UINT));
		D3DXMATRIX TM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT));
		bool Live = *(bool*)(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX));

		CTank* Obj = ((CTank*)ObjectManager->ObjectList()[ID - 1]);

		if (Obj == NULL) return;
		if (*Obj->GetBulletList(ObjID) == NULL) return;

		(*Obj->GetBulletList(ObjID))->SetTM(&TM);
		(*Obj->GetBulletList(ObjID))->SetLive(Live);
	

		return;
	}

	void Tank_SmokeBomb_Create_Func(BYTE* _pPacket, DWORD& _Length)
	{
		//CConsole::Write("TANK_SMOKEBOMB_CREATE!!\n");

		UINT ID = *(UINT*)_pPacket;
		D3DXVECTOR3 Pos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT));
		D3DXVECTOR3 Dir = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(D3DXVECTOR3));

		CTank* Obj = ((CTank*)ObjectManager->ObjectList()[ID - 1]);

		if (Obj == NULL) return;

		for (UINT i = 0; i < BULLET_MAX; ++i){
			if (*(Obj->GetBulletList(i)) != NULL) continue;
			*(Obj->GetBulletList(i)) = new CTank_SmokeBomb(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext(), ShaderManager->GetBasicShader(),
				ResourceManager->TankBoomb(), Pos, Dir);
			break;
		}

		return;
	}

	void Tank_SmokeBomb_TM_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ID = *(UINT*)_pPacket;
		UINT ObjID = *(UINT*)(_pPacket + sizeof(UINT));
		D3DXMATRIX TM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT));
		bool Live = *(bool*)(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX));

		CTank* Obj = ((CTank*)ObjectManager->ObjectList()[ID - 1]);

		if (Obj == NULL) return;

		if (*Obj->GetBulletList(ObjID) != NULL){
			(*Obj->GetBulletList(ObjID))->SetTM(&TM);
			(*Obj->GetBulletList(ObjID))->SetLive(Live);
		}

		return;
	}

	void CombatPlane_Machingun_Create_Func(BYTE* _pPacket, DWORD& _Length)
	{
		//CConsole::Write("�̻��� ����\n");
		// ������ ����.
		UINT ID = *(UINT*)_pPacket;
		D3DXVECTOR3 LPos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT));
		D3DXVECTOR3 LDir = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(D3DXVECTOR3));
		D3DXVECTOR3 RPos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3));
		D3DXVECTOR3 RDir = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3));

		CCombatPlane* Obj = ((CCombatPlane*)ObjectManager->ObjectList()[ID - 1]);

		if (Obj == NULL) return;

		bool LBullet = false;
		bool RBullet = false;
		for (int i = 0; i < BULLET_MAX; ++i){
			if (RBullet && LBullet) break;
			if (*Obj->GetBulletList(i) != NULL) continue;

			// ����.
			if (!LBullet){
				*Obj->GetBulletList(i) = new CCombatPlane_MachinGun(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext(), ShaderManager->GetBasicShader(),
					ResourceManager->TankBoomb(), LPos, LDir);
				LBullet = true;
				continue;
			}

			// ������.
			if (!RBullet){
				*Obj->GetBulletList(i) = new CCombatPlane_MachinGun(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext(), ShaderManager->GetBasicShader(),
					ResourceManager->TankBoomb(), RPos, RDir);
				RBullet = true;
				continue;
			}
			break;
		}

		return;
	}

	void CombatPlane_Machingun_TM_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ID = *(UINT*)_pPacket;
		UINT ObjID = *(UINT*)(_pPacket + sizeof(UINT));
		D3DXMATRIX TM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT));
		bool Live = *(bool*)(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX));

		CCombatPlane* Obj = ((CCombatPlane*)ObjectManager->ObjectList()[ID - 1]);

		if (Obj == NULL) return;

		if (*Obj->GetBulletList(ObjID) != NULL){
			(*Obj->GetBulletList(ObjID))->SetTM(&TM);
			(*Obj->GetBulletList(ObjID))->SetLive(Live);	
		}

		return;

	}

	void CombatPlane_Boomb_Create_Func(BYTE* _pPacket, DWORD& _Length)
	{
		// ������ ����.
		UINT ID = *(UINT*)_pPacket;
		D3DXVECTOR3 Pos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT));		

		CCombatPlane* Obj = ((CCombatPlane*)ObjectManager->ObjectList()[ID - 1]);

		if (Obj == NULL) return;

		for (int i = 0; i < BULLET_MAX; ++i){
			if (*Obj->GetBulletList(i) != NULL) continue;
			//CConsole::Write(" �Ѿ� 2 ����\n");
			*Obj->GetBulletList(i) = new CCombatPlane_Boomb(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext(), ShaderManager->GetBasicShader(),
				ResourceManager->TankBoomb(), Pos);
			break;
		}

		return;
	}

	void CombatPlane_Boomb_TM_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ID = *(UINT*)_pPacket;
		UINT ObjID = *(UINT*)(_pPacket + sizeof(UINT));
		D3DXMATRIX TM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT));
		bool Live = *(bool*)(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXMATRIX));

		CCombatPlane* Obj = ((CCombatPlane*)ObjectManager->ObjectList()[ID - 1]);

		if (Obj == NULL) return;

		if (*Obj->GetBulletList(ObjID) != NULL){
			(*Obj->GetBulletList(ObjID))->SetTM(&TM);
			(*Obj->GetBulletList(ObjID))->SetLive(Live);
		}

		return;
	}

	void ComInfo_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ID = *(UINT*)_pPacket;
		UINT Type = *(UINT*)(_pPacket + sizeof(UINT));

		CObject* Obj = ObjectManager->ObjectList()[ID - 1];

		if (Obj == NULL) return;

		D3DXVECTOR3 Pos = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXMATRIX tm;

		switch (Type) {
		case OBJ_TANK:
			Pos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(UINT));

			D3DXMatrixTranslation(&tm, Pos.x, Pos.y, Pos.z);
			((CTank*)ObjectManager->ObjectList()[ID - 1])->SetTM(&tm);
			//((CTank*)ObjectManager->ObjectList()[ID - 1])->SetHoodTM(&tm);
			//((CTank*)ObjectManager->ObjectList()[ID - 1])->SetGunBarrelTM(&tm);
			break;
		case OBJ_COMBATPLANE:
			Pos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(UINT));
			D3DXMatrixTranslation(&tm, Pos.x, Pos.y, Pos.z);
			((CCombatPlane*)ObjectManager->ObjectList()[ID - 1])->SetTM(&tm);	
			break;
		}
		return;
	}

	void Char_Cnt_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CGameEngine::Instance()->ClientNet().m_rTank = *_pPacket;
		CGameEngine::Instance()->ClientNet().m_rComb = *(_pPacket + sizeof(UINT));
		CGameEngine::Instance()->ClientNet().m_bTank = *(_pPacket + sizeof(UINT) + sizeof(UINT));
		CGameEngine::Instance()->ClientNet().m_bComb = *(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(UINT));
		CConsole::Write("* ���� ������� 2�� ��ũ�� 3�� �����Ҽ� �ֽ��ϴ� *\n");
		CConsole::Write("���� ���������� ������   ��ũ �� : %d \n", CGameEngine::Instance()->ClientNet().m_rTank);
		CConsole::Write("���� ���������� ������ ������ �� : %d \n", CGameEngine::Instance()->ClientNet().m_rComb);
		CConsole::Write("���� ��������� ������   ��ũ �� : %d \n", CGameEngine::Instance()->ClientNet().m_bTank);
		CConsole::Write("���� ��������� ������ ������ �� : %d \n", CGameEngine::Instance()->ClientNet().m_bComb);

		return;
	} 

	void Team_Cnt_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CGameEngine::Instance()->ClientNet().m_rTeam = *_pPacket;
		CGameEngine::Instance()->ClientNet().m_bTeam = *(_pPacket + sizeof(UINT));
		CConsole::Write("* ���� 5����� ���� �����մϴ� (���� ������ ¦���� ������ Ȧ���� ��������� �⺻ ����) *\n");
		CConsole::Write("���� ������ �ο� �� : %d \n", CGameEngine::Instance()->ClientNet().m_rTeam);
		CConsole::Write("���� ����� �ο� �� : %d \n", CGameEngine::Instance()->ClientNet().m_bTeam);
	
		return;
	}

	void ComBatPlane_Serching_Func(BYTE* _pPacket, DWORD& _Length)
	{
		// ������ ���� ����.
		UINT ClientID = *(UINT*)_pPacket;
		float HP = *(float*)(_pPacket + sizeof(UINT));
		D3DXVECTOR3 vPos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(float));

		// ������ �Է�.
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetHP(HP);
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetPosition(&vPos);
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetState(CCombatPlane::STATE_COM_SEARCHING);
		return;
	}

	void ComBatPlane_Trace_Func(BYTE* _pPacket, DWORD& _Length)
	{
		// ������ ���� ����.
		UINT ClientID = *(UINT*)_pPacket;
		float HP = *(float*)(_pPacket + sizeof(UINT));
		D3DXVECTOR3 vPos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(float));
		D3DXVECTOR3 vFront = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(float) + sizeof(D3DXVECTOR3));
		

		// ������ �Է�.
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetHP(HP);
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetPosition(&vPos);
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetFront(&vFront);
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetState(CCombatPlane::STATE_COM_DEFAULT);
		return;
	}

	void ComBatPlane_Return_Func(BYTE* _pPacket, DWORD& _Length)
	{
		// ������ ���� ����.
		UINT ClientID = *(UINT*)_pPacket;
		float HP = *(float*)(_pPacket + sizeof(UINT));
		D3DXVECTOR3 vPos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(float));
		D3DXVECTOR3 vFront = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(float) + sizeof(D3DXVECTOR3));

		// ������ �Է�.
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetHP(HP);
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetPosition(&vPos);
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetFront(&vFront);
		((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetState(CCombatPlane::STATE_COM_DEFAULT);
		return;
	}

	void ComBatPlane_Machingun_Attack(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ClientID = *(UINT*)_pPacket;
		CObject* pObj = (ObjectManager->ObjectList()[ClientID - 1]);
		((CCombatPlane*)pObj)->SetState(CCombatPlane::STATE_COM_ATTACK1);
	}

	void ComBatPlane_Combatboomb_Attack(BYTE* _pPacket, DWORD& _Length)
	{
	
	}

	void Tank_Move_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ID = *(UINT*)_pPacket;
		UINT Type = *(UINT*)(_pPacket + sizeof(UINT));
		D3DXVECTOR3 Pos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(UINT));
		D3DXVECTOR3 Front = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXVECTOR3));
		float HP = *(float*)(_pPacket + sizeof(UINT) + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3));

		CTank* Obj = (CTank*)ObjectManager->ObjectList()[ID - 1];
		if (Obj == NULL) return;

		Obj->SetPrevPos(Obj->GetPosition());
		Obj->SetPosition(&Pos);
		//CConsole::Write("COM Pos : %f, %f, %f \n", Pos.x, Pos.y, Pos.z);
		Obj->SetHP(HP);
		Obj->SetFront(&Front);
		Obj->SetState(CTank::STATE_COM_MOVE);

		return;
	}

	void Tank_Body_Rot_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ID = *(UINT*)_pPacket;
//		float TankRot = *(float*)(_pPacket + sizeof(UINT));
		D3DXVECTOR3 Dir = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT));
		float HP = *(float*)(_pPacket + sizeof(UINT) + sizeof(D3DXVECTOR3));
		CTank* obj = (CTank*)ObjectManager->ObjectList()[ID - 1];

		// ������ ����
		obj->SetHP(HP);
//		obj->SetAngle(TankRot);
		obj->SetState(CTank::STATE_COM_BODYROT);
		
		return;
	}

	void Tank_Attack_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ID = *(UINT*)_pPacket;
		D3DXVECTOR3 Front = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT));

		CTank* obj = (CTank*)ObjectManager->ObjectList()[ID - 1];
		obj->SetGunBarrelFront(&Front);
		obj->SetState(CTank::STATE_COM_ATTACK);
	}

	void Object_Death_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ID = *(UINT*)_pPacket;
		CObject* Obj = ObjectManager->ObjectList()[ID - 1];
		Obj->SetHP(0.0f);
	}

	void Bullet_Death_Func(BYTE* _pPacket, DWORD& _Length)
	{
		int ClientID = ((int*)_pPacket)[0];
		int BulletID = ((int*)_pPacket)[1];
		int Type = ((int*)_pPacket)[2];


		CObject* Obj = ObjectManager->ObjectList()[ClientID - 1];
		if (Type == TANK)
			(*((CTank*)Obj)->GetBulletList(BulletID))->SetHP(0.0f);
		else if (Type == COMBATPLANE)
			(*((CCombatPlane*)Obj)->GetBulletList(BulletID))->SetHP(0.0f);
	}

	void Nexus_Info(BYTE* _pPacket, DWORD& _Length)
	{
		D3DXVECTOR3 Pos = *(D3DXVECTOR3*)_pPacket;
		float HP = *(float*)(_pPacket + sizeof(D3DXVECTOR3));
		UINT ID = *(UINT*)(_pPacket + sizeof(D3DXVECTOR3) + sizeof(float));
		UINT Team = *(UINT*)(_pPacket + sizeof(D3DXVECTOR3) + sizeof(float) + sizeof(UINT));

		CNexus* pObj = (CNexus*)ObjectManager->ObjectList()[ID - 1];
		if (pObj != NULL){
			pObj->SetPosition(&Pos);
			pObj->SetHP(HP);
			pObj->SetTeam(Team);
		}
		if (HP <= 0.0f){
			if (Team == GameEngine->ClientNet().m_nClientTeam){
				GameEngine->GameResult() = GAMERESULT_DEFEAT;
			}
			else{
				GameEngine->GameResult() = GAMERESULT_VICTORY;
			}
		}

		return;		
	}
}





/****************************** End of File "CSProcess.cpp" ******************************/