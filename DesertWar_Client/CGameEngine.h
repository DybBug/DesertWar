//**********************************************************************************
//	2015.01.02.
//	CGameEngine.h : ���� ���� ���� ��� ����.
//
//	'�̱���'���� ����.
//	���� ����ǰ� �ִ� ���� ���� �Ѵ�.
//**********************************************************************************
#pragma once
#include "CClientSession.h"
#include "CPlayScene.h"
#include "CIntroScene.h"
#include "CWaitRoomScene.h"
#include "CLoadingScene.h"
#include "Device.h"
#include "CTimer.h"
#include "CConsole.h"
#include "CShaderManager.h"
#include "CResourceManager.h"
#include "CObjectManager.h"
#include "CInput.h"
#include "CCamera.h"

#define GameEngine CGameEngine::Instance()

enum GAMERESULT{
	GAMERESULT_DEFAULT = 0,
	GAMERESULT_VICTORY,	// �¸�.
	GAMERESULT_DEFEAT,	// �й�.
};

class CGameEngine
{
private :
	static CGameEngine* m_pInstance;
protected :
	CGameEngine();
	~CGameEngine();
public:
	static CGameEngine* Instance(){
		if (m_pInstance == NULL)
			m_pInstance = new CGameEngine;
		return m_pInstance;
	}
private :	
	Device m_pDevice;
	CScene* m_pNowScene;
	CTimer m_Timer;
	CMouseInput m_Input;
	bool m_bNormalLine;
	bool m_bActivate;
	bool m_bCollisionBox;

	int m_nGameResult;

    //��Ʈ��ũ ����
	CClientSession pClientNet;
	TCHAR SvIPAddr[128];
	TCHAR PtNumber[128];

	CCamera* m_pCamera;
	LIGHT* m_pLight;

	int m_nBlueLiveUnitCnt;
	int m_nRedLiveUnitCnt;

public:
	Device& Device() { return m_pDevice; }
	CScene* Scene() { return m_pNowScene; }
	CTimer& Timer() { return m_Timer; }
	CMouseInput& Input() { return m_Input; }
	CCamera*& Camera() { return m_pCamera; }
	LIGHT*& Light() { return m_pLight; }

	CClientSession& ClientNet() { return pClientNet; }
	TCHAR*   ServerIPAddr() { return SvIPAddr; }
	TCHAR*   PortNumber() { return PtNumber; }
	bool& NormalLine() { return m_bNormalLine; }
	bool& Activate() { return m_bActivate; }
	bool& CollisionBox() { return m_bCollisionBox; }
	int& GameResult() { return m_nGameResult; }

	int& LiveUnitCnt(UINT Team) { return (Team == RED_TEAM) ? m_nRedLiveUnitCnt : m_nBlueLiveUnitCnt; }

	void SetNormalLineEnable(bool _bEnable = false) { m_bNormalLine = _bEnable; }

	void SceneChange(CScene** _ppScene);
	void SceneUpdate(){ m_pNowScene->Update(m_Timer.GetDeltaTime()); } // ���� '���' ������Ʈ.
	void SceneRender() { m_pNowScene->Render(); } // ���� '���' ����.
	void Release() { if(m_pInstance != NULL) delete m_pInstance; }
	//void Release() { if (m_pNowScene != NULL) SAFE_DELETE(m_pNowScene); }

	void PutFPS(int x, int y);
};


//****************************** End of File "CGameEngine.h" ******************************//