//***************************************************************************************
//
//	CLoadingScene.cpp : 로딩 씬 관련 헤더 파일
//	
//***************************************************************************************
#include "CGameEngine.h"
#include "CLoadingScene.h"
#include "CConsole.h"
#include "CResourceManager.h"


//////////////////////////////////////////////////////////////////////////////////////////////////
// CLoadingScene()
CLoadingScene::CLoadingScene()
	: CScene()
{
	m_pDev = CGameEngine::Instance()->Device().GetDevice();
	m_pDevC = CGameEngine::Instance()->Device().GetDeviceContext();
	m_pDepthStencilView = CGameEngine::Instance()->Device().GetDepthStencilView();
	m_pSwapChain = CGameEngine::Instance()->Device().GetSwapChain();
	m_pRenderTargetView = CGameEngine::Instance()->Device().GetRenderTargetView();

}




//////////////////////////////////////////////////////////////////////////////////////////////////
// ~CLoadingScene()
CLoadingScene::~CLoadingScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pMesh);

	m_pBackgroundSRV->Release();
}




//////////////////////////////////////////////////////////////////////////////////////////////////
// Init()
void CLoadingScene::Init()
{
	("로딩씬... \n 데이터 로딩중...\n");

	//--------------------------------------------------------------------------------
	// 객체 초기화.
	//--------------------------------------------------------------------------------
	D3DX11CreateShaderResourceViewFromFile(GameEngine->Device().GetDevice(), L"../Resource/Texture/LoadingScene_Background.png", 0, 0, &m_pBackgroundSRV, 0);
	m_pShader = ShaderManager->GetBasicShader();
	m_pCamera = new CCamera(D3DXVECTOR3(0.f, 0.f, -1.6f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 1.f, 0.f));

	m_pMesh = new CCubeMesh();
	m_pMesh->Create();
	m_pMesh->CreateBuffer(m_pDev);

	Render();

	("리소스 로딩중...\n");
	ResourceManager->Load(GameEngine->Device().GetDevice());

	("오브젝트 로딩중...\n");
	ObjectManager->Init();

	float X, Z;
	ObjectManager->Terrain()->GetSize(&X, &Z);
	BYTE Data[8];
	*((float*)Data) = X;
	*((float*)(Data + sizeof(float))) = Z;
	CGameEngine::Instance()->ClientNet().WritePacket(CLOADING_COMPLETE, Data, 8);
	return;
}




//////////////////////////////////////////////////////////////////////////////////////////////////
// Update()
void CLoadingScene::Update(float _fDTime)
{
	if (GameEngine->ClientNet().m_Protocol == SLOADING_COMPLETE){
		CPlayScene* pPlayScene = new CPlayScene();
		CGameEngine::Instance()->SceneChange((CScene**)&pPlayScene);
	}
}




//////////////////////////////////////////////////////////////////////////////////////////////////
// Render()
void CLoadingScene::Render()
{
	assert(m_pDevC);
	assert(m_pDev);

	m_pDevC->ClearRenderTargetView(m_pRenderTargetView, (float*)D3DXCOLOR(1.f, 1.f, 0.5f, 1.0f));
	m_pDevC->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	{
		int Width, Height;
		GameEngine->Device().GetClientSize(&Width, &Height);

		//m_pCamera->SetOrtho((UINT)Width, (UINT)Height, 1.0f, 1000.0f);
		//	m_pCamera->EnableOrthoLens(true);

		D3DXMATRIX World; D3DXMatrixIdentity(&World);

		m_pShader->SetWorld(&World);
		m_pShader->SetCamera(m_pCamera);
		m_pShader->SetTexture(&m_pBackgroundSRV);
		MATERIAL m;
		m.s_cDiffuse = m.s_cAmbient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		//m_pShader->SetMaterial(&m);

		m_pShader->SettingDataToGPU(PASS_LIGHTING_TEXTURE);
		m_pMesh->Render(GameEngine->Device().GetDeviceContext());
	}
	m_pSwapChain->Present(0, 0);
}


//****************************** End of File "CLoadingScene.cpp" ******************************//