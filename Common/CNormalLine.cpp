//********************************************************************************************
//	CNormalLine.cpp : 노멀 관련 헤더 파일. 
//********************************************************************************************
#include "CNormalLine.h"



///////////////////////////////////////////////////////////////////////////////////////////////
// CNormalLine()
CNormalLine::CNormalLine()
{

}




///////////////////////////////////////////////////////////////////////////////////////////////
// ~CNormalLine()
CNormalLine::~CNormalLine()
{

}




///////////////////////////////////////////////////////////////////////////////////////////////
// Create()
bool CNormalLine::Create(ID3D11Device* _pDev, CShader* _pShader, vector<::CBasicShader::VERTEX>* _pVtxList)
{
	m_pShader = _pShader;
	UINT cnt = _pVtxList->size();
	m_nVtxCnt = cnt * 2;
	CNonLightingShader::VERTEX* vtx = new CNonLightingShader::VERTEX[m_nVtxCnt];
	for (UINT i = 0; i < cnt; ++i){
		vtx[2*i].s_vPosition		= (*_pVtxList)[i].s_vPosition;
		vtx[2 * i + 1].s_vPosition	=(*_pVtxList)[i].s_vPosition + (*_pVtxList)[i].s_vNormal;
	}

	D3D11_BUFFER_DESC vbdesc;
	vbdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbdesc.ByteWidth = sizeof(CNonLightingShader::VERTEX) * m_nVtxCnt;
	vbdesc.CPUAccessFlags = 0;
	vbdesc.MiscFlags = 0;
	vbdesc.StructureByteStride = 0;
	vbdesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = vtx;
	_pDev->CreateBuffer(&vbdesc, &vdata, &m_pVB);

	delete vtx;

	return true;
}




///////////////////////////////////////////////////////////////////////////////////////////////
// Create()
bool CNormalLine::Create(ID3D11Device* _pDev, CShader* _pShader, ::CBasicShader::VERTEX* _pVtxList, UINT _nCnt)
{
	m_pShader = _pShader;
	m_nVtxCnt = _nCnt * 2;
	CNonLightingShader::VERTEX* vtx = new CNonLightingShader::VERTEX[m_nVtxCnt];
	for (UINT i = 0; i < _nCnt; ++i){
		vtx[2 * i].s_vPosition = _pVtxList[i].s_vPosition;
		vtx[2 * i + 1].s_vPosition = _pVtxList[i].s_vPosition + _pVtxList[i].s_vNormal;
	}

	D3D11_BUFFER_DESC vbdesc;
	vbdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbdesc.ByteWidth = sizeof(CNonLightingShader::VERTEX) * m_nVtxCnt;
	vbdesc.CPUAccessFlags = 0;
	vbdesc.MiscFlags = 0;
	vbdesc.StructureByteStride = 0;
	vbdesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = vtx;
	_pDev->CreateBuffer(&vbdesc, &vdata, &m_pVB);

	delete vtx;

	return true;
}




///////////////////////////////////////////////////////////////////////////////////////////////
// Draw()
void CNormalLine::Draw(ID3D11DeviceContext* _pDevC, D3DXMATRIX* _mWorld)
{
	m_pShader->SetWorld(_mWorld);
	m_pShader->SetColor(&D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	m_pShader->SettingDataToGPU();

	UINT stride = sizeof(CNonLightingShader::VERTEX);
	UINT offset = 0;
	_pDevC->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	_pDevC->Draw(m_nVtxCnt, 0);
}




///////////////////////////////////////////////////////////////////////////////////////////////
// Release()
void CNormalLine::Release()
{
	SAFE_RELEASE(m_pVB);
}

//****************************** End of File "CNormalLine.cpp" ******************************//