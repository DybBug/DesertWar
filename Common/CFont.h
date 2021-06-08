//**********************************************************************************
//	2015.01.28.
//	CFont.h : ��Ʈ ���� ��� ����
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
		float s_fLeft, s_fRight; // �ؽ�ó�� U ��ǥ.
		UINT s_nSize;			// ������ �ȼ� �ʺ�.
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

	// �߰�.
	CFontShader* m_pFontShader;

	ID3D11Buffer* m_pVB;
	ID3D11Buffer* m_pIB;

	D3DXMATRIX m_mWorld;

protected :
	// �ؽ�ó�� ���� ��ġ ������ ����ִ�.txt ���� �ε�.
	bool _LoadFontData(char*);
	void _ReleaseFontData();
	bool _LoadTexture(ID3D11Device* _pDev, WCHAR* _Filename);
	void _ReleaseTexture();

	// ���ڿ��� �Է¹޾� ���ڸ� �׸� �ﰢ������ ���� �迭�� ����� ��ȯ.
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