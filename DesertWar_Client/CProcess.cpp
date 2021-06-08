#include "CGameEngine.h"
#include "CProcess.h"
#include "CConsole.h"

namespace S_Protocol_Func{
	void Connect_Complete_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CConsole::Write("서버와 연결이 되었습니다.\n");
		//대기실 입장 알려주기.
		BYTE Byte = 1;
		CGameEngine::Instance()->ClientNet().WritePacket(CENTER_WAITROOM, &Byte, 1);
		CWaitRoomScene* pWaitRoomScene = new CWaitRoomScene();
		CGameEngine::Instance()->SceneChange((CScene**)&pWaitRoomScene);

		return;
	}

	void Enter_Waitroom_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CConsole::Write("ID : %d 클라이언트 입장했습니다.\n");
		if (CGameEngine::Instance()->ClientNet().m_nClientID == 0)
			CGameEngine::Instance()->ClientNet().m_nClientID = *_pPacket;

		CGameEngine::Instance()->ClientNet().m_nClientCnt = *(_pPacket + sizeof(UINT));
		CConsole::Write("[ 접속 인원 ] : %d 명\n", CGameEngine::Instance()->ClientNet().m_nClientCnt);

		return;
	}

	void Room_Leader_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CConsole::Write("%d 클라이언트가 방장입니다.\n", *_pPacket);
		CGameEngine::Instance()->ClientNet().m_bLeader = TRUE;

		return;
	}

	void Loading_Start_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CConsole::Write("로딩을 시작합니다.\n", *_pPacket);
		//CGameEngine::Instance()->ClientNet().m_bStart = TRUE;
		UINT** ppList = ObjectManager->CharacterList();
		*ppList = new UINT;
		memmove(*ppList, _pPacket, _Length);
		CLoadingScene* pLoadingScene = new CLoadingScene();
		CGameEngine::Instance()->SceneChange((CScene**)&pLoadingScene);

		return;
	}

	void Loading_Complete_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CConsole::Write("게임을 시작합니다.\n", *_pPacket);
		CPlayScene* pPlayScene = new CPlayScene();
		CGameEngine::Instance()->SceneChange((CScene**)&pPlayScene);

		return;
	}

	void Ready_User_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CConsole::Write("%d 클라이언트가 준비 완료 되었습니다.\n", *_pPacket);
	}

	void Ready_Leader_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CConsole::Write("모든 클라이언트가 준비 완료 되었습니다.\n");
		CGameEngine::Instance()->ClientNet().m_bAllReady = true;
	}

	void Object_Info_Func(BYTE* _pPacket, DWORD& _Length)
	{
		UINT ClientID = *(UINT*)_pPacket;
		UINT Type = *(UINT*)(_pPacket + sizeof(UINT));
		if (Type == OBJ_TANK){
			D3DXMATRIX TM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT));
			D3DXMATRIX HoodTM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT));

			((CTank*)ObjectManager->ObjectList()[ClientID - 1])->SetTM(&TM);
			((CTank*)ObjectManager->ObjectList()[ClientID - 1])->SetHoodTM(&HoodTM);
		}
		else if (Type == OBJ_COMBATPLANE){
			// 데이터 분해.
			D3DXMATRIX TM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT));

			((CCombatPlane*)ObjectManager->ObjectList()[ClientID - 1])->SetTM(&TM);
		}
		return;
	}

	void Tank_Boomb_Create_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CConsole::Write("TANK_BOOMB_CREATE!!\n");
		UINT ID = *(UINT*)_pPacket;
		D3DXVECTOR3 Pos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT));
		D3DXVECTOR3 Dir = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(D3DXVECTOR3));

		((CTank*)ObjectManager->ObjectList()[ID - 1])->
			GetTank_Boomb_List().push_back(new CTank_Bomb(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext(), ShaderManager->GetBasicShader(),
			ResourceManager->Tank_BombMesh(),
			Pos, Dir));
	}

	void CombatPlane_Machingun_Create_Func(BYTE* _pPacket, DWORD& _Length)
	{
		
		// 데이터 분할.
		UINT ID = *(UINT*)_pPacket;
		D3DXVECTOR3 LPos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT));
		D3DXVECTOR3 LDir = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(D3DXVECTOR3));
		D3DXVECTOR3 RPos = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3));
		D3DXVECTOR3 RDir = *(D3DXVECTOR3*)(_pPacket + sizeof(UINT) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3));

		((CCombatPlane*)ObjectManager->ObjectList()[ID - 1])->
			GetCombatPlane_MuchinGunBullet_List().push_back(new CCombatPlane_MachinGun(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext(), ShaderManager->GetBasicShader(),
			ResourceManager->Tank_BombMesh(),
			LPos, LDir));

		((CCombatPlane*)ObjectManager->ObjectList()[ID - 1])->
			GetCombatPlane_MuchinGunBullet_List().push_back(new CCombatPlane_MachinGun(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext(), ShaderManager->GetBasicShader(),
			ResourceManager->Tank_BombMesh(),
			RPos, RDir));

		return;
	}

	void CombatPlane_Machingun_TM_Func(BYTE* _pPacket, DWORD& _Length)
	{
		CConsole::Write("COMBATPLANE_MACHINGUN_MOVE!!\n");

		UINT ID = *(UINT*)_pPacket;
		UINT ObjID = *(UINT*)(_pPacket + sizeof(UINT));
		D3DXMATRIX TM = *(D3DXMATRIX*)(_pPacket + sizeof(UINT) + sizeof(UINT));
		((CCombatPlane*)ObjectManager->ObjectList()[ID - 1])->
			GetCombatPlane_MuchinGunBullet_List()[ObjID]->SetTM(&TM);

	}
}