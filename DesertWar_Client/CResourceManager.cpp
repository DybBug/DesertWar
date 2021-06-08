//****************************************************************************************
//	2015.01.02.
//	CResourceManager.cpp : 텍스처 관리 소스 파일.
//
//****************************************************************************************
#include "CResourceManager.h"
#include "FBXParser.h"


CResourceManager* CResourceManager::m_pInstance = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////
// CResourceManager()
CResourceManager::CResourceManager()
{
	
	m_pP4F2 = NULL;
	m_pP4J = NULL;
	m_pTigerDiffuseMapRSV = NULL;
	m_pF18_Hornet = NULL;	
	m_pF86_Sabre = NULL;
	m_pF18_HornetDiffuseMapRSV = NULL;
	m_pNexus = NULL;
	m_pVictoryTextureSRV = NULL;
	m_pDefeatTextureSRV = NULL;
	m_pHPBarTextureSRV = NULL;
	m_pBlue_Tank = NULL;
	m_pBlue_CombatPlane = NULL;
	m_pRed_Tank = NULL;
	m_pRed_CombatPlane = NULL;
}




////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CResourceManager()
CResourceManager::~CResourceManager()
{
	
}




////////////////////////////////////////////////////////////////////////////////////////////////////
// Load()
void CResourceManager::Load(ID3D11Device* _pDev)
{
	FBXParser parser;
	// 전차 리소스.
	if (m_pP4F2 == NULL)			parser.FileLoad(_pDev, "../Resource/Tank/P4F2/P4F2_model_track.fbx", &m_pP4F2);
	if (m_pP4J == NULL)				parser.FileLoad(_pDev, "../Resource/Tank/P4J/P4J_model_track.fbx", &m_pP4J);

	// 전투기 리소스.
	if (m_pF18_Hornet == NULL)	parser.FileLoad(_pDev, "../Resource/CombatPlane/F18_Hornet/F18_Hornet.fbx", &m_pF18_Hornet);
	if (m_pF86_Sabre == NULL)	parser.FileLoad(_pDev, "../Resource/CombatPlane/F86_Sabre/F86_Sabre.fbx", &m_pF86_Sabre);

	//D3DX11CreateShaderResourceViewFromFile(_pDev, L"../Resource/CombatPlane/F18_Hornet/F_18.tga", 0, 0, &m_pF18_HornetDiffuseMapRSV, 0);


	// 넥서스 리소스
	if(m_pNexus == NULL) parser.FileLoad(_pDev, "../Resource/Nexus/SciFi-Tower_Rocket Launcher.fbx", &m_pNexus);

	parser.Release();

	// 스프라이트 로드.
	Sprite_Init();
	Sprite_DataLoad(&m_TankBoobSprite, L"../Resource/Effect/Explosion_Missile01.png", 1.f, 1.f);
	Sprite_DataLoad(&m_Explosion, L"../Resource/Effect/Explosion.dds", 30.f, 30.f, 8, 4);
	Sprite_DataLoad(&m_Smoke, L"../Resource/Effect/Smoke.png", 100.f, 100.f);

	// 기타 텍스처.
	if (m_pVictoryTextureSRV == NULL)	D3DX11CreateShaderResourceViewFromFile(_pDev, L"../Resource/Texture/Victory.png", 0, 0, &m_pVictoryTextureSRV, 0);
	if (m_pDefeatTextureSRV == NULL)	D3DX11CreateShaderResourceViewFromFile(_pDev, L"../Resource/Texture/Defeat.png", 0, 0, &m_pDefeatTextureSRV, 0);
	if (m_pHPBarTextureSRV == NULL)		D3DX11CreateShaderResourceViewFromFile(_pDev, L"../Resource/Texture/HP_Bar.png", 0, 0, &m_pHPBarTextureSRV, 0);

	// 팀 메쉬
	m_pBlue_Tank = m_pP4J;
	m_pBlue_CombatPlane = m_pF86_Sabre;
	m_pRed_Tank = m_pP4F2;
	m_pRed_CombatPlane = m_pF18_Hornet;

	if (FAILED(D3DX11CreateShaderResourceViewFromFile(_pDev, L"../Resource/Texture/sky2.dds", 0, 0, &m_pSkyCubeMapSRV, 0))){
		MessageBox(NULL, L"../resource/texture/Victory.png", L"Texture Error!!", MB_OK);
	}

}




////////////////////////////////////////////////////////////////////////////////////////////////////
// Release()
void CResourceManager::Release()
{

	if (m_pP4F2) m_pP4F2->Release();
	if (m_pP4J) m_pP4J->Release();


	if (m_pTigerDiffuseMapRSV) m_pTigerDiffuseMapRSV->Release();

	if (m_pF18_Hornet) m_pF18_Hornet->Release();
	if (m_pF86_Sabre) m_pF86_Sabre->Release();

	if (m_pNexus) m_pNexus->Release();

	// Sprite 제거.
	Sprite_Release(&m_TankBoobSprite);
	Sprite_Release(&m_Explosion);
	Sprite_Release(&m_Smoke);
	Sprite_Release();

	// 기타 텍스처.
	SAFE_RELEASE(m_pVictoryTextureSRV);
	SAFE_RELEASE(m_pDefeatTextureSRV);
	SAFE_RELEASE(m_pHPBarTextureSRV);
}



//****************************** End of File "CResourceManager.cpp" ******************************//