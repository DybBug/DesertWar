//**********************************************************************************
//	2015.01.28.
//	CFont.h : 폰트 관련 헤더 파일
//**********************************************************************************
#pragma once
#include "Common.h"
#include "CShader.h"

class CFont
{
private :
#define MAX_LENGTH 16

	struct FONT
	{
		float s_fLeft, s_fRight; // 텍스처의 U 좌표.
		UINT s_nSize;			// 문자의 픽셀 너비.
	};
public :
	CFont();
	CFont(const CFont&);
	virtual ~CFont();
protected :
	ID3D11Device* m_pDev;
	ID3D11DeviceContext* m_pDevC;
	FONT* m_pFont;
	ID3D11ShaderResourceView* m_pTextureSRV;

	// 추가.
	CFontShader* m_pFontShader;

	ID3D11Buffer* m_pVB;
	ID3D11Buffer* m_pIB;

	D3DXMATRIX m_mWorld;

protected :
	// 텍스처의 글자 위치 정보를 담고있는.txt 파일 로드.
	bool _LoadFontData(char*);
	void _ReleaseFontData();
	bool _LoadTexture(ID3D11Device* _pDev, WCHAR* _Filename);
	void _ReleaseTexture();

	// 문자열을 입력받아 글자를 그릴 삼각형들의 정점 배열을 만들어 반환.
	void _BuildVertexArray(void* _pVertices, char* _pText, float _fX, float _fY);
public :
	bool CreateFont(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC,
		char* _pFontFilename, WCHAR* _pFilename, CCamera* _pCamera);
	void Release();

	void DrawText(float _fX, float _fY, char* _pMsg, D3DXCOLOR _cColor);
	
	ID3D11ShaderResourceView* GetTexture() { return m_pTextureSRV; }
	void SetCamera(CCamera* _pCamera) { m_pFontShader->SetCamera(_pCamera); }

};




//****************************** End of File "CFont.h" ******************************//