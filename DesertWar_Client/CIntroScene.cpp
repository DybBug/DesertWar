//***************************************************************************************
//	
//	CIntroScene.cpp : 인트로 씬 관련 소스 파일.
//	
//***************************************************************************************
#include "CGameEngine.h"
#include "CIntroScene.h"
#include "CConsole.h"
#include "resource1.h"
#include "CInput.h"

BOOL CALLBACK ConnetProc(HWND _hWnd, UINT _nMsg, WPARAM _wParam, LPARAM _lParam);
extern HINSTANCE  g_hIns;
extern HWND		g_hWnd;
///////////////////////////////////////////////////////////////////////////////////////////////
//	CIntroScene()
CIntroScene::CIntroScene()
: CScene()
{
	m_pDev = CGameEngine::Instance()->Device().GetDevice();
	m_pDevC = CGameEngine::Instance()->Device().GetDeviceContext();
	m_pDepthStencilView = CGameEngine::Instance()->Device().GetDepthStencilView();
	m_pSwapChain = CGameEngine::Instance()->Device().GetSwapChain();
	m_pRenderTargetView = CGameEngine::Instance()->Device().GetRenderTargetView();

	m_pBackgroundSRV = NULL;
	m_fAngle = 0.0f;
	m_bChange = false;
}




///////////////////////////////////////////////////////////////////////////////////////////////
// ~CIntroScene()
CIntroScene::~CIntroScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pMesh);

	m_pBackgroundSRV->Release();
}





///////////////////////////////////////////////////////////////////////////////////////////////
// Init()
void CIntroScene::Init()
{
	ShaderManager->Load(GameEngine->Device().GetDevice(), GameEngine->Device().GetDeviceContext());
	D3DX11CreateShaderResourceViewFromFile(GameEngine->Device().GetDevice(), L"../Resource/Texture/IntroScene_Background.png", 0, 0, &m_pBackgroundSRV, 0);
	m_pShader = ShaderManager->GetBasicShader();

	m_pCamera = new CCamera(D3DXVECTOR3(0.f, 0.f, -1.6f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 1.f, 0.f));

	m_pMesh = new CCubeMesh();
	m_pMesh->CreateBuffer(m_pDev);

	("인트로 씬! \n");
}



///////////////////////////////////////////////////////////////////////////////////////////////
// Update()
void CIntroScene::Update(float _fDTime)
{

	if (GameEngine->Activate() == true){

		if (GetAsyncKeyState(VK_SPACE) & 0x0001){
			if (DialogBox(g_hIns, MAKEINTRESOURCE(IDD_DIALOG1), g_hWnd, ConnetProc) == IDOK){ //모달

				char IPstr[128];
				WideCharToMultiByte(CP_ACP, NULL, CGameEngine::Instance()->ServerIPAddr(), -1, IPstr, 128, NULL, FALSE);  // 유니코드 -> 멀티바이트

				if (!CGameEngine::Instance()->ClientNet().BeginTcp(IPstr, _wtoi(CGameEngine::Instance()->PortNumber())))
				{
					("네트워크 초기화 실패!!\n");
					return;
				}
			}
		 }
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////
// Render()
void CIntroScene::Render()
{
	assert(m_pDevC);
	assert(m_pDev);

	m_pDevC->ClearRenderTargetView(m_pRenderTargetView, (float*)D3DXCOLOR(1.f, 1.f, 1.f, 1.0f));
	m_pDevC->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	{
		int Width, Height;
		GameEngine->Device().GetClientSize(&Width, &Height);

	//	m_pCamera->SetOrtho((UINT)Width, (UINT)Height, 1.0f, 1000.0f);
	//	m_pCamera->EnableOrthoLens(true);

		D3DXMATRIX World; D3DXMatrixIdentity(&World);
		D3DXMatrixRotationY(&World, D3DXToRadian(90.f));
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

BOOL CALLBACK ConnetProc(HWND _hWnd, UINT _nMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_nMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(_hWnd, IDC_IPADDRESS1, _T("127.0.0.1"));
		SetDlgItemText(_hWnd, IDC_PORT, _T("9876"));
		break;
	case WM_COMMAND:
		switch (LOWORD(_wParam))
		{
		case IDOK:
			GetDlgItemText(_hWnd, IDC_IPADDRESS1, CGameEngine::Instance()->ServerIPAddr(), 128);
			GetDlgItemText(_hWnd, IDC_PORT, CGameEngine::Instance()->PortNumber(), 128);
			EndDialog(_hWnd, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(_hWnd, IDCANCEL);
			return FALSE;

		}
		break;
	}

	return FALSE;
}

//****************************** End of File "CIntroScene.cpp" ******************************//