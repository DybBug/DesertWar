
//*****************************************************************************************
//	2015.01.02.
//	CPlayScene.cpp : �÷��̾� �ҽ� ����.
//	���� ������ �÷��̵Ǵ� ��.
//*****************************************************************************************
#include "CGameEngine.h"
#include "CPlayScene.h"
#include "RenderState.h"
#include "NetworkHeader.h"

///////////////////////////////////////////////////////////////////////////////////////////
// CPlayScene()
CPlayScene::CPlayScene()
	:CScene()
{
	m_pDev = GameEngine->Device().GetDevice();
	m_pDevC = GameEngine->Device().GetDeviceContext();
	m_pDepthStencilView = GameEngine->Device().GetDepthStencilView();
	m_pRenderTargetView = GameEngine->Device().GetRenderTargetView();
	m_pSwapChain = GameEngine->Device().GetSwapChain();

	GameEngine->Input().InitMouseScreenPt();

	m_Render[0] = &CPlayScene::_VictoryRender;
	m_Render[1] = &CPlayScene::_DefeatRender;
	m_bGameEnd = false;

	m_fChangeWaitroomTime = 0.0f;
	return;
}




/////////////////////////////////////////////////////////////////////////////////////////
//	~CPlayScene()
CPlayScene::~CPlayScene()
{
	for (int i = 0; i < 2; i++)
		SAFE_DELETE(m_pCamera[i]);
	
	SAFE_DELETE(m_pUICamera);
	SAFE_DELETE(m_pGameResultMesh);
	//SAFE_DELETE(m_pShadowMap);
}




/////////////////////////////////////////////////////////////////////////////////////////
// _ShowHelper()
void CPlayScene::_ShowHelper()
{
	//CConsole::Write("[����]\n");
	//CConsole::Write("F1 : ���� ���.\n");
	//CConsole::Write("C : �浹 ���� ���̱�.\n");
	//CConsole::Write("Z : ī�޶� ��ȯ.\n");
}




/////////////////////////////////////////////////////////////////////////////////////////
// _VictoryRender()
void CPlayScene::_VictoryRender()
{
	ID3D11ShaderResourceView* pTextureSRV = ResourceManager->VictoryTextureSRV();
	D3DXMATRIX World; D3DXMatrixIdentity(&World);
	
	m_pShader->EnableLight(0, false);	// �� ����.
	m_pShader->SetWorld(&World);
	m_pShader->SetTexture(&pTextureSRV);
	m_pShader->SetCamera(m_pUICamera);
	m_pShader->SettingDataToGPU(PASS_LIGHTING_TEXTURE);
	m_pGameResultMesh->Render(m_pDevC);
}




/////////////////////////////////////////////////////////////////////////////////////////
// _DefeatRender()
void CPlayScene::_DefeatRender()
{
	ID3D11ShaderResourceView* pTextureSRV = ResourceManager->DefeatTextureSRV();
	D3DXMATRIX World; D3DXMatrixIdentity(&World);

	m_pShader->EnableLight(0, false);	// �� ����.
	m_pShader->SetWorld(&World);
	m_pShader->SetTexture(&pTextureSRV);
	m_pShader->SetCamera(m_pUICamera);
	m_pShader->SettingDataToGPU(PASS_LIGHTING_TEXTURE);
	m_pGameResultMesh->Render(m_pDevC);
}




/////////////////////////////////////////////////////////////////////////////////////////
// _BuildShadowTransform()
void CPlayScene::_BuildShadowTransform()
{
	// ī�޶� �� �� �������� ��ȯ.
	LIGHT light;	m_pShader->GetLight(0, &light);
	D3DXVECTOR3 LightDir = light.s_vDirection;
	D3DXVECTOR3 LightPos = -1.0f * m_SceneBounds.s_fRadius * light.s_vDirection;
	D3DXVECTOR3 TargetPos = m_SceneBounds.s_vCenter;
	D3DXVECTOR3 Up = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMATRIX mView; D3DXMatrixLookAtLH(&mView, &LightPos, &TargetPos, &Up);

	// �� ���� �� ��� ���� ��ȯ .
	D3DXVECTOR3 SphereCenterLS;
	D3DXVec3TransformCoord(&SphereCenterLS, &TargetPos, &mView);

	// ���� ���� ��� �����.
	float Left = SphereCenterLS.x - m_SceneBounds.s_fRadius;
	float Bottom = SphereCenterLS.y - m_SceneBounds.s_fRadius;
	float Near = SphereCenterLS.z - m_SceneBounds.s_fRadius;
	float Right = SphereCenterLS.x + m_SceneBounds.s_fRadius;
	float Top = SphereCenterLS.y + m_SceneBounds.s_fRadius;
	float Far = SphereCenterLS.z + m_SceneBounds.s_fRadius;
	D3DXMATRIX mOrtho;  D3DXMatrixOrthoOffCenterLH(&mOrtho, Left, Right, Bottom, Top, Near, Far);

	// ������ ���� ��ġ�� �ؽ�ó ��ǥ��� ��ȯ(��ũ�� ��ǥ��)
	// [-1, +1]^2  ->  [0,1]^2
	D3DXMATRIX mTrans(0.5f, 0.0f, 0.0f, 0.0f,
					  0.0f, -0.5f, 0.0f, 0.0f,
					  0.0f, 0.0f, 1.0f, 0.0f,
					  0.5f, 0.5f, 0.0f, 1.0f);

	// �׸��� ���.
	D3DXMATRIX mShadow = mView * mOrtho * mTrans;

	m_pShader->SetShadow(mShadow);
	CShader* TerrainShader = ShaderManager->GetTerrainShader();
	TerrainShader->SetShadow(mShadow);

	m_pShadowMap->SetLightView(mView);
	m_pShadowMap->SetLightProj(mOrtho);	
}




/////////////////////////////////////////////////////////////////////////////////////////
//	_DrawSceneToShadowMap()
void CPlayScene::_DrawSceneToShadowMap()
{
	D3DXMATRIX view		= m_pShadowMap->GetLightView();
	D3DXMATRIX proj		= m_pShadowMap->GetLightProj();
	D3DXMATRIX viewproj = view * proj;

	m_pShader = ShaderManager->GetBuildShadowMapShader();
	((CBuildShadowMapShader*)m_pShader)->SetEyePosW(GameEngine->Camera()->GetEye());
	((CBuildShadowMapShader*)m_pShader)->SetView(view);
	((CBuildShadowMapShader*)m_pShader)->SetProj(proj);

	ObjectManager->ShadowRender(m_pShader);

	m_pShader = ShaderManager->GetBasicShader();
	ID3D11ShaderResourceView* ShadowMapTex = m_pShadowMap->GetDepthMapSRV();
	m_pShader->SetShadowTexture(&ShadowMapTex);
	CShader* TerrainShader = ShaderManager->GetTerrainShader();
	TerrainShader->SetShadowTexture(&ShadowMapTex);
}




/////////////////////////////////////////////////////////////////////////////////////////
// _DrawUI()
void CPlayScene::_DrawUI()
{
	m_pShader->EnableLight(0, false);

	m_vHPBarPos = D3DXVECTOR3(0.f, 0.0f, 0.f);
	m_vHPBarPivot = D3DXVECTOR3(-0.6f, 0.6f, 0.0f);
	m_vHPBarPos.x = m_vHPBarPivot.x + (0.25f *(m_fCurrPlayerHP / m_fMaxPlayerHP));
	m_vHPBarPos.y = m_vHPBarPivot.y;
	m_vHPBarPos.z = m_vHPBarPivot.z;
	D3DXMATRIX Trans, Scale, World;
	D3DXMatrixTranslation(&Trans, m_vHPBarPos.x, m_vHPBarPos.y, m_vHPBarPos.z);
	m_fCurrPlayerHP = ObjectManager->Player()->GetHP();
	D3DXMatrixScaling(&Scale, m_fCurrPlayerHP / m_fMaxPlayerHP, 1.f, 1.f);
	World = Scale * Trans;
	m_pShader->SetWorld(&World);
	MATERIAL m;
	m.s_cDiffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_pShader->SetCamera(m_pUICamera);
	m_pShader->SetMaterial(&m);
	ID3D11ShaderResourceView* pTextuerSRV = ResourceManager->HPTextureSRV();
	m_pShader->SetTexture(&pTextuerSRV);
	m_pShader->SettingDataToGPU(PASS_LIGHTING_TEXTURE_ALPHACLIP);

	m_pHPBarMesh->Render(m_pDevC);

	m_pShader->EnableLight(0, true);
}





/////////////////////////////////////////////////////////////////////////////////////////
//	Init()
void CPlayScene::Init()
{
	//CConsole::Write("�÷��� ��!!\n");

	int bluecnt = GameEngine->LiveUnitCnt(BLUE_TEAM);
	bluecnt = 5;
	int redcnt = GameEngine->LiveUnitCnt(RED_TEAM);
	redcnt = 5;


	//----------------------------------------------------------------------------------
	// ���̴� �ʱ�ȭ
	//----------------------------------------------------------------------------------
	//ShaderManager()->ShaderChange(&CShaderManager::m_pBasicShader);
	m_pShader = ShaderManager->GetBasicShader();



	//----------------------------------------------------------------------------------
	// ������� �ʱ�ȭ
	//----------------------------------------------------------------------------------
	m_pShadowMap = new CShadowMap(m_pDev, 2048, 2048);
	m_SceneBounds.s_vCenter = D3DXVECTOR3(0.f, 0.f, 0.f);
	float Width, Height;
	ObjectManager->Terrain()->GetSize(&Width, &Height);
	Width /= 2.0f; Height /= 2.0f;
	m_SceneBounds.s_fRadius = sqrt((Width * Width) + (Height * Height));

	//----------------------------------------------------------------------------------
	// ������ ���� �ʱ�ȭ
	//----------------------------------------------------------------------------------
	m_bWireFrame = false;
	ID3D11RasterizerState* WireframeRS;
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_SOLID;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	m_pDev->CreateRasterizerState(&wireframeDesc, &WireframeRS);
	m_pDevC->RSSetState(WireframeRS);


	//----------------------------------------------------------------------------------
	// ī�޶� �ʱ�ȭ
	//----------------------------------------------------------------------------------
	int fWidth = 0, fHeight = 0;
	GameEngine->Device().GetClientSize(&fWidth, &fHeight);
	m_pCamera[0] = new CFreeCamera(D3DXVECTOR3(0.0f, 1.0f, -1.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		(float)D3DXToRadian(45.f), (float)fWidth / (float)fHeight, 1.0f, 1000.0f);
	m_pCamera[0]->SetOrtho(fWidth, fHeight, 1.0f, 10000.0f);

	m_pCamera[1] = new CFixCamera(ObjectManager->Player(),
		D3DXVECTOR3(0.0f, 10.0f, -50.0f),
		D3DXVECTOR3(0.f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		(float)D3DXToRadian(45.f), (float)(fWidth / fHeight), 1.0f, 1000.0f);

	GameEngine->Camera() = m_pCamera[1];
	m_bFixCamera = true;

	// UIī�޶�
	m_pUICamera = new CCamera(D3DXVECTOR3(0.f, 0.f, -1.6f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 1.f, 0.f));


	//----------------------------------------------------------------------------------
	// �� �ʱ�ȭ.
	//----------------------------------------------------------------------------------
	m_Light.s_fType = LIGHT_TYPE_DIRECTION;
	m_Light.s_vDirection = D3DXVECTOR3(-0.5f, -1.0f, -0.5f);
	D3DXVec3Normalize(&m_Light.s_vDirection, &m_Light.s_vDirection);
	m_Light.s_cDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_Light.s_cAmbient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	m_Light.s_cSpecular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	GameEngine->Light() = &m_Light;

	//----------------------------------------------------------------------------------
	// ��Ÿ ��ü �ʱ�ȭ.
	//----------------------------------------------------------------------------------
	m_pGameResultMesh = new CCubeMesh();
	m_pGameResultMesh->Create();
	m_pGameResultMesh->CreateBuffer(m_pDev);

	m_pHPBarMesh = new CPlaneMesh();
	m_pHPBarMesh->Create(0.5f, 0.05f);
	m_pHPBarMesh->CreateBuffer(m_pDev);

	m_fCurrPlayerHP = m_fMaxPlayerHP = ObjectManager->Player()->GetHP();
	
	return;
}





/////////////////////////////////////////////////////////////////////////////////////////
// Update()
void CPlayScene::Update(float _fdTime)
{

	if ((GameEngine->Activate() == true) ||
	(GameEngine->GameResult() == GAMERESULT_DEFAULT)){
		GameEngine->Camera()->Update(_fdTime);

		if (GetAsyncKeyState('N') & 0x0001)
			GameEngine->SetNormalLineEnable(!GameEngine->NormalLine());

		if (GetAsyncKeyState(VK_SPACE) & 0x0001){
			if (m_bWireFrame == false){

				m_pDevC->RSSetState(ResterRizerState::m_pSolidRS);
				m_bWireFrame = true;
			}
			else{
				m_pDevC->RSSetState(ResterRizerState::m_pWireframeRS);
 				m_bWireFrame = false;
			}
		}
		if (GetAsyncKeyState(VK_F1) & 0x0001){
			_ShowHelper();
		}

		if (GetAsyncKeyState('C') & 0x0001){
			GameEngine->CollisionBox() = !GameEngine->CollisionBox();
		}

		if (GetAsyncKeyState('Z') & 0x0001){
			if (m_bFixCamera){
				GameEngine->Camera() = m_pCamera[0];
				m_bFixCamera = false;
			}
			else{
				GameEngine->Camera() = m_pCamera[1];
				m_bFixCamera = true;
			}
		}


		m_pShader->SetCamera(GameEngine->Camera());
		//CShader* shader = ShaderManager->GetNonLightingShader();
		//shader->SetCamera(GameEngine->Camera());

		_BuildShadowTransform();

		ObjectManager->Update(_fdTime);
		
	}
	if (GameEngine->GameResult() != GAMERESULT_DEFAULT){
		// ���� ����� ������ ����
		if ((!m_bGameEnd) && GameEngine->ClientNet().m_bLeader){
			bool result = true;
			GameEngine->ClientNet().WritePacket(CGAME_END, (BYTE*)&result, sizeof(bool));
			m_bGameEnd = true;
		}

		m_fChangeWaitroomTime += _fdTime;

		if (m_fChangeWaitroomTime >= 5.0f){
			//if ((GetAsyncKeyState(VK_SPACE) & 0x0001))
				{
				GameEngine->Camera() = NULL;
				GameEngine->LiveUnitCnt(BLUE_TEAM) = 5;
				GameEngine->LiveUnitCnt(RED_TEAM) = 5;
				CGameEngine::Instance()->ClientNet().m_bAllReady = FALSE;
				GameEngine->GameResult() = GAMERESULT_DEFAULT;

				vector<CObject*> ObjList = ObjectManager->ObjectList();
				for (int i = 0; i < MAX_SESSION; ++i){
					delete ObjList[i];
				}
				ObjectManager->ObjectList().clear();

				CWaitRoomScene* Scene = new CWaitRoomScene();
				GameEngine->SceneChange((CScene**)&Scene);
				return;
			}
		}
	}	
}




/////////////////////////////////////////////////////////////////////////////////////////
//	Render()
void CPlayScene::Render()
{
	assert(m_pDevC);
	assert(m_pDev);

	// �׸��� �׸���.
	m_pShadowMap->BindDsvAndSetNullRenderTarget(m_pDevC);
	_DrawSceneToShadowMap();
	m_pDevC->RSSetState(0);

	// ������ �ĸ� ���ۿ� ���� ���۸� �ٽ� ��� ���Ա� �ܰ迡 ���´�.
	ID3D11RenderTargetView* renderTargets[1] = { m_pRenderTargetView };
	m_pDevC->OMSetRenderTargets(1, renderTargets, m_pDepthStencilView);
	m_pDevC->RSSetViewports(1, GameEngine->Device().GetViewPort());


	m_pDevC->ClearRenderTargetView(m_pRenderTargetView, (const float*)D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	m_pDevC->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	{
		int GameResult = GameEngine->GameResult();
		int Team = ObjectManager->Player()->GetTeam();
		int BlueCnt = GameEngine->LiveUnitCnt(BLUE_TEAM);
		int RedCnt = GameEngine->LiveUnitCnt(RED_TEAM);

		
		m_pShader->SetLight(0, &m_Light);
		m_pShader->EnableLight(0, true);



		ObjectManager->Render();

		_DrawUI();

		if (BlueCnt <= 0){
			if (Team == BLUE_TEAM)
				GameResult = GAMERESULT_DEFEAT;
			else
				GameResult = GAMERESULT_VICTORY;
		}
		if (RedCnt <= 0){
			if (Team == RED_TEAM)
				GameResult = GAMERESULT_DEFEAT;
			else
				GameResult = GAMERESULT_VICTORY;
		}
	
		if (GameResult != GAMERESULT_DEFAULT){
			(this->*m_Render[GameResult - 1])();
		}

	}
	m_pSwapChain->Present(0, 0);
}






//****************************** End of File "CPlayScene.cpp" ******************************//