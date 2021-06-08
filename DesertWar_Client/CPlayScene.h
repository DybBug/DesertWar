
//*****************************************************************************************
//	2015.01.02.
//	CPlayScene.h : 플레이씬 헤더 파일.
//	실제 게임이 실행되는 씬.
//*****************************************************************************************
#pragma once
#include "Common.h"
#include "CScene.h"
#include "CShader.h"
#include "CMesh.h"

#include "CFont.h"
#include "CTerrain.h"
#include "CTank.h"
#include "CShadowMap.h"



class CPlayScene : public CScene
{

public :
	CPlayScene();
	virtual ~CPlayScene();

	typedef void(CPlayScene::*GameResultRender)();
protected :
	CShader* m_pShader;
	CCamera* m_pCamera[2];
	LIGHT m_Light;

	bool m_bWireFrame;
	bool m_bFixCamera;
	bool m_bGameEnd;

	GameResultRender m_Render[2];
	CCubeMesh* m_pGameResultMesh;
	CCamera* m_pUICamera;

	CShadowMap* m_pShadowMap;
	BoundingSphere m_SceneBounds;

	float m_fChangeWaitroomTime;

	// UI관련 맴버 변수들..
	D3DXVECTOR3 m_vHPBarPivot;
	D3DXVECTOR3 m_vHPBarPos;
	CPlaneMesh* m_pHPBarMesh;

	float m_fCurrPlayerHP;
	float m_fMaxPlayerHP;

protected :
	void _ShowHelper();

	void _VictoryRender();
	void _DefeatRender();
	void _BuildShadowTransform();
	void _DrawSceneToShadowMap();
	void _DrawUI();
public :
	virtual void Init();
	virtual void Update(float _fDTime);
	virtual void Render();

	void Draw(CMeshNode* _Mesh);
};








//****************************** End of File "CPlayScene.h" ******************************//