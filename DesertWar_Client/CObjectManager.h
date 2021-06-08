//****************************************************************************************
//	
//	CObjectManager.h : 오브젝트 관리 헤더 파일.
//
//****************************************************************************************
#pragma once
#include "Common.h"
#include "CTerrain.h"
#include "CTank.h"
#include "CCombatPlane.h"
#include "CTank_Info.h"
#include "CCombat_Info.h"
#include "CSky.h"
#include "CNexus.h"

#define ObjectManager CObjectManager::Instance()


class CObjectManager
{
public :
	CObjectManager();
	~CObjectManager();
private :
	static CObjectManager* m_pInstance;
public :
	static CObjectManager* Instance(){
		if (m_pInstance == NULL)
			m_pInstance = new CObjectManager();
		return m_pInstance;
	}
private :
	PLAYER_INFO m_pCharacterList[MAX_SESSION + 2];
	CTerrain* m_pTerrain;
	CSky* m_pSky;
	CObject* m_pPlayer;
	vector<CObject*> m_pObjectList;
public :
	void Init();
	void Release();
	void Update(float _fDTime);
	void Render();
	void ShadowRender(CShader* _pShader);

	PLAYER_INFO* CharacterList(){ return m_pCharacterList; }
	CTerrain* Terrain() { return m_pTerrain; }
	CSky* Sky() { return m_pSky; }
	vector<CObject*>& ObjectList() { return m_pObjectList; }
	CObject* Player() { return m_pPlayer; }

};


//****************************** End of File "CObjectManager.h" ******************************//