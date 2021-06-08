//***************************************************************************************
//
//	CIntroScene.h : 인트로 씬 관련 헤더 파일
//	
//***************************************************************************************
#pragma once
#include "CScene.h"
#include "Common.h"
#include "CMesh.h"
class CIntroScene : public CScene
{
public :
	CIntroScene();
	virtual ~CIntroScene();
protected :
	bool m_bChange;
	CCubeMesh* m_pMesh;

	CShader* m_pShader;
	CCamera* m_pCamera;

	ID3D11ShaderResourceView* m_pBackgroundSRV;

	float m_fAngle;

protected :

public :
	virtual void Init();
	virtual void Update(float _fDtime);
	virtual void Render();

};


//****************************** End of File "CIntroScene.h" ******************************//