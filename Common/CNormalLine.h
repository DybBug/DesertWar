//*******************************************************************************************
//	CNormalLine.h : 노멀 관련 헤더 파일. 
//*******************************************************************************************
#pragma once
#include "Common.h"
#include "CShader.h"

class CNormalLine
{
public :

public :
	CNormalLine();
	~CNormalLine();
protected :
	CShader* m_pShader;
	UINT m_nVtxCnt;
	ID3D11Buffer* m_pVB;
public :
	bool Create(ID3D11Device* _pDev, CShader* _pShader, vector<::CBasicShader::VERTEX>* _pVtxList);
	bool Create(ID3D11Device* _pDev, CShader* _pShader, ::CBasicShader::VERTEX* _pVtxList, UINT _nCnt);
	void Draw(ID3D11DeviceContext* _pDevC, D3DXMATRIX* _mWorld);
	void Release();
};

//****************************** End of File "CNormalLine.h" ******************************//