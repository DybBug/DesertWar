//************************************************************************************************
//	
//	CObjectManager.cpp : 오브젝트 관리 소스 파일.
//
//************************************************************************************************
#include "CGameEngine.h"
#include "CObjectManager.h"
#include "CResourceManager.h"

CObjectManager* CObjectManager::m_pInstance = NULL;
//////////////////////////////////////////////////////////////////////////////////////////////////
// CObjectManager()
CObjectManager::CObjectManager()
{
}




//////////////////////////////////////////////////////////////////////////////////////////////////
// ~CObjectManager()
CObjectManager::~CObjectManager()
{
	
}



//////////////////////////////////////////////////////////////////////////////////////////////////
// Init()
void CObjectManager::Init()
{
	ID3D11Device* pDev = GameEngine->Device().GetDevice();
	ID3D11DeviceContext* pDevC = GameEngine->Device().GetDeviceContext();

	

	//=================================================================================
	// 지형 초기화.
	//=================================================================================
	//
	//CConsole::Write("로딩씬...지형 로딩중...\n");

	CTerrain::InitInfo info;
	info.s_pHeightMapFilename = L"../resource/Terrain/terrain2_1024x1024.raw";
	info.s_pLayerMapFilenameList[0] = L"../resource/Terrain/SendGround00.jpg";
	info.s_fHeightScale = 100.f;
	info.s_nHeightMapWidth = 1024;
	info.s_nHeightMapDepth = 1024;
	info.s_nLV = 6;
	m_pTerrain = new CTerrain(pDev, pDevC, ShaderManager->GetTerrainShader(), info);





	//=================================================================================
	// 오브젝트 초기화
	//=================================================================================
	//CConsole::Write("로딩씬...오브젝트 로딩중...\n");

	

	// 전투기, 탱크 생성.
	for (UINT i = 0; i < MAX_SESSION; ++i){
		if (m_pCharacterList[i].Character == TANK){
			if (m_pCharacterList[i].Team == RED_TEAM){
				m_pObjectList.push_back(new CTank(pDev, pDevC, ShaderManager->GetBasicShader(),
					ResourceManager->P4F2(), NULL, 15.0f, 45.0f, m_pCharacterList[i].Pos));
			}
			else{
				m_pObjectList.push_back(new CTank(pDev, pDevC, ShaderManager->GetBasicShader(),
					ResourceManager->P4J(), NULL, 15.0f, 45.0f, m_pCharacterList[i].Pos));
			}
			m_pObjectList.back()->SetTeam(m_pCharacterList[i].Team);
	
			if (GameEngine->ClientNet().m_nClientID == i + 1){
				m_pPlayer = m_pObjectList.back();
				m_pTerrain->SetPlayer(m_pPlayer);
				m_pObjectList.back()->SetPlayer(true);
			}
		}
		else if (m_pCharacterList[i].Character == COMBATPLANE){
			CMeshNode* pMesh = NULL;
			if (m_pCharacterList[i].Team == RED_TEAM){
				m_pObjectList.push_back(new CCombatPlane(pDev, pDevC, ShaderManager->GetBasicShader(),
					ResourceManager->F18_Hornet(), 70.0f, 20.0f, m_pCharacterList[i].Pos));
			}
			else{
				m_pObjectList.push_back(new CCombatPlane(pDev, pDevC, ShaderManager->GetBasicShader(),
					ResourceManager->F86_Sabre(), 70.0f, 20.0f, m_pCharacterList[i].Pos));
			}
			m_pObjectList.back()->SetTeam(m_pCharacterList[i].Team);

			if (GameEngine->ClientNet().m_nClientID == i + 1){
				m_pPlayer = m_pObjectList.back();
				m_pObjectList.back()->SetPlayer(true);
			}
		}
		m_pObjectList[i]->SetComputer(m_pCharacterList[i].Computer);
	}

	// 넥서스 생성.
	m_pObjectList.push_back(new CNexus(pDev, pDevC, ShaderManager->GetBasicShader(),
		ResourceManager->Nexus(), D3DXVECTOR3(0.f, 0.f, 0.f), 100.f));
	m_pObjectList.push_back(new CNexus(pDev, pDevC, ShaderManager->GetBasicShader(),
		ResourceManager->Nexus(), D3DXVECTOR3(0.f, 0.f, 0.f), 100.f));

	//=================================================================================
	// 스카이 박스 초기화.
	//=================================================================================
	m_pSky = new CSky(pDev, pDevC, L"../resource/Texture/sky2.dds", 500.f);
}




//////////////////////////////////////////////////////////////////////////////////////////////////
// Release()
void CObjectManager::Release()
{
	SAFE_DELETE(m_pTerrain);
	for (UINT i = 0; i < m_pObjectList.size(); i++){
		SAFE_DELETE(m_pObjectList[i]);
	}
	m_pObjectList.clear();
	m_pPlayer = NULL;
}




//////////////////////////////////////////////////////////////////////////////////////////////////
// Update()
void CObjectManager::Update(float _fDTime)
{
	m_pTerrain->Update(_fDTime);

	if (GameEngine->Activate()){
		if (m_pPlayer->GetLive()) m_pPlayer->Control(_fDTime);
	}
	
		
	int cnt = m_pObjectList.size();
	for (UINT i = 0; i < cnt; ++i){
		if (m_pPlayer == m_pObjectList[i]) continue;
		if (m_pObjectList[i]->GetLive()) m_pObjectList[i]->Update(_fDTime);
	}

}




//////////////////////////////////////////////////////////////////////////////////////////////////
// Render()
void CObjectManager::Render()
{
	m_pTerrain->Render();

	UINT count = m_pObjectList.size();
	for (UINT i = 0; i < count; i++){
		if (m_pObjectList[i]->GetLive()) m_pObjectList[i]->Render();
	}

	m_pSky->Render(*GameEngine->Camera());
}




//////////////////////////////////////////////////////////////////////////////////////////////////
// ShaderRender()
void CObjectManager::ShadowRender(CShader* _pShader)
{
	m_pTerrain->ShadowRender((CBuildShadowMapShader*)_pShader);

	UINT count = m_pObjectList.size();
	for (UINT i = 0; i < count; i++){
		if (m_pObjectList[i]->GetHP() > 0.0f) m_pObjectList[i]->ShadowRender((CBuildShadowMapShader*)_pShader);
	}	


}


//****************************** End of File "CObjectManager.cpp" ******************************//