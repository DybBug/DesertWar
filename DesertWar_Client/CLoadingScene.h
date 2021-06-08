//***************************************************************************************
//
//	CLoadingScene.h : 로딩 씬 관련 헤더 파일
//	
//***************************************************************************************
#pragma once
#include "CScene.h"
#include "Common.h"
#include "CMesh.h"
class CLoadingScene : public CScene
{
public:
	CLoadingScene();
	virtual ~CLoadingScene();
protected:
	CCubeMesh* m_pMesh;

	CShader* m_pShader;
	CCamera* m_pCamera;

	ID3D11ShaderResourceView* m_pBackgroundSRV;
protected :

public:
	virtual void Init();
	virtual void Update(float _fDtime);
	virtual void Render();
};


//****************************** End of File "CLoadingScene.h" ******************************//