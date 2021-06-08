//***************************************************************************************
//	Sprite.cpp : 스프라이트 관련 소스 파일.
//***************************************************************************************
#include "CGameEngine.h"
#include "CShaderManager.h"
#include "Sprite.h"


class CSpriteShader;

////////////////////////////////////////////////////////////////////////////////////////
// 전역 변수.
ID3D11Buffer* g_pSprite = NULL;

namespace SPRITE_
{
	ID3D11Device* g_pDevice = NULL;
	ID3D11DeviceContext* g_pDeviceContext = NULL;
	CShader* g_pShader = NULL;
}
using namespace SPRITE_;


////////////////////////////////////////////////////////////////////////////////////////
// Sprite_Init()
// 스프라이트의 정점 버퍼를 생성 하는 함수.
// 스프라이트를 만들기 전에 호출 해야함..
void Sprite_Init()
{
	g_pDevice = GameEngine->Device().GetDevice();
	g_pDeviceContext = GameEngine->Device().GetDeviceContext();
	g_pShader = ShaderManager->GetSpriteShader();

	// DESC 초기화.
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(CSpriteShader::VERTEX);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	// 빈 버퍼 생성.
	if (FAILED(g_pDevice->CreateBuffer(&desc, 0, &g_pSprite))){
		("버텍스버퍼 생성 실패\n");
	}
}




////////////////////////////////////////////////////////////////////////////////////////
// Sprite_DataLoad()
// 스프라이트의 텍스처, 크기 정보 로드.
void Sprite_DataLoad(SPRITE* _pSprite, wchar_t* _pFilename, float _fWidth, float _fHeight)
{
	_pSprite->s_fWidth = _fWidth;
	_pSprite->s_fHeight = _fHeight;
	_pSprite->s_pFilename = _pFilename;
	_pSprite->s_vTexTransform = D3DXVECTOR4(0.f, 0.f, 1.f, 1.f);
	_pSprite->s_iFrameMaxX = 0;
	_pSprite->s_iFrameMaxY = 0;

	D3DX11CreateShaderResourceViewFromFile(g_pDevice,
											_pSprite->s_pFilename,
											0, 
											0, 
											&(_pSprite->s_pTexSRV), 
											0);

}



void Sprite_DataLoad(SPRITE* _pSprite, wchar_t* _pfilename, float _fWidth, float _fHeight, UINT _iFrameMaxX, UINT _iFrameMaxY)
{
	_pSprite->s_fWidth = _fWidth;
	_pSprite->s_fHeight = _fHeight;
	_pSprite->s_pFilename = _pfilename;
	_pSprite->s_vTexTransform = D3DXVECTOR4(0.f, 0.f, 1.f, 1.f);
	_pSprite->s_iFrameMaxX = _iFrameMaxX;
	_pSprite->s_iFrameMaxY = _iFrameMaxY;

	D3DX11CreateShaderResourceViewFromFile(g_pDevice,
		_pSprite->s_pFilename,
		0,
		0,
		&(_pSprite->s_pTexSRV),
		0);
}





////////////////////////////////////////////////////////////////////////////////////////
// Sprite_Draw()
// 스프라이트를 그리는 함수.
void Sprite_Draw(SPRITE* _pSprite)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	g_pDeviceContext->Map(g_pSprite, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	CSpriteShader::VERTEX* vtx = (CSpriteShader::VERTEX*)mappedData.pData;
	vtx->s_vPosition = _pSprite->s_vPosition;
	vtx->s_vSize.x = _pSprite->s_fWidth;
	vtx->s_vSize.y = _pSprite->s_fHeight;
	g_pDeviceContext->Unmap(g_pSprite, 0);

	g_pShader->SetCamera(GameEngine->Camera());
	MATERIAL m;
	m.s_cDiffuse = D3DXCOLOR(1.f, 1.f, 1.f, 0.1f);
	g_pShader->SetMaterial(&m);
	g_pShader->SetTexture(&(_pSprite->s_pTexSRV), &(_pSprite->s_vTexTransform));

	g_pShader->SettingDataToGPU();

	UINT stride = sizeof(CSpriteShader::VERTEX);
	UINT offset = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &g_pSprite, &stride, &offset);
	g_pDeviceContext->Draw(1, 0);
}




////////////////////////////////////////////////////////////////////////////////////////
// Sprite_Release()
void Sprite_Release(SPRITE* _pSprite)
{
	if (_pSprite == NULL){
		SAFE_RELEASE(g_pSprite);
	}
	else{
		SAFE_RELEASE(_pSprite->s_pTexSRV);
	}
}


//****************************** End of File "Sprite.cpp" ******************************//