//***************************************************************************************
//	2015.01.02.
//	CGameEngine.cpp : ���� ���� ���� �ҽ� ����.
//	'�̱���'���� ����.
//***************************************************************************************
#include "CGameEngine.h"

CGameEngine* CGameEngine::m_pInstance = NULL;
///////////////////////////////////////////////////////////////////////////////////////
// CGameEngine()
CGameEngine::CGameEngine()
	: m_pNowScene(NULL), m_bNormalLine(false), m_bCollisionBox(false)
{
	m_nGameResult = GAMERESULT_DEFAULT;
	m_nBlueLiveUnitCnt = 5;
	m_nRedLiveUnitCnt = 5;
}




///////////////////////////////////////////////////////////////////////////////////////
// ~CGameEngine()
CGameEngine::~CGameEngine()
{
	if (m_pNowScene != NULL) SAFE_DELETE(m_pNowScene);
	
}




///////////////////////////////////////////////////////////////////////////////////////
//	SceneChange()
void CGameEngine::SceneChange(CScene** _ppScene)
{
	if (m_pNowScene != NULL) SAFE_DELETE(m_pNowScene);
	// ���� ������� ��ü.
	m_pNowScene = *_ppScene;

	//	����� ��� �ʱ�ȭ.
	m_pNowScene->Init();

	*_ppScene = NULL;
}




///////////////////////////////////////////////////////////////////////////////////////
// PutFPS()
void CGameEngine::PutFPS(int x, int y)
{
	static int oldtime = (int)m_Timer.GetDeltaTime();
	int nowtime = (int)m_Timer.GetDeltaTime();

	static int frmcnt = 0;
	static float fps = 0.0f;

	++frmcnt;

	int time = nowtime - oldtime;
	if (time >= 999){
		oldtime = nowtime;

		fps = (float)frmcnt * 1000 / (float)time;
		frmcnt = 0;
	}

	char msg[40];
	sprintf_s(msg, "FPS : %.1f/%d", fps, frmcnt);

}





//****************************** End of File "CGameEngine.cpp" ******************************//