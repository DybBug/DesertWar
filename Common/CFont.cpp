//**********************************************************************************
//	2015.01.28.
//	CFont.cpp : 폰트 관련 소스 파일
//**********************************************************************************
#include "CFont.h"
#include <fstream>

////////////////////////////////////////////////////////////////////////////////////
// CFont()
CFont::CFont()
{
	m_pFont = NULL;
	m_pTextureSRV = NULL;

	// 추가.
	m_pFontShader = NULL;
	//ZeroMemory(&m_vVertices, sizeof(m_vVertices));
	//ZeroMemory(&m_vIndeices, sizeof(m_vIndeices));
	m_pVB = NULL;
	m_pIB = NULL;
}




////////////////////////////////////////////////////////////////////////////////////
// CFont()
CFont::CFont(const CFont& _other)
{

}




////////////////////////////////////////////////////////////////////////////////////
// ~CFont()
CFont::~CFont()
{
	SAFE_DELETE(m_pFontShader);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}




////////////////////////////////////////////////////////////////////////////////////
// _LoadFontData() : 텍스처의 글자 위치 정보를 담고있는 .txt 파일 로드.
bool CFont::_LoadFontData(char* _filename)
{
	ifstream fin;
	int i;
	char temp;

	m_pFont = new FONT[95];
	if (!m_pFont) return false;

	fin.open(_filename);
	if (fin.fail()) return false;

	for (i = 0; i < 95; i++){
		fin.get(temp);
		while (temp != ' '){
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' '){
			fin.get(temp);
		}
		fin >> m_pFont[i].s_fLeft;
		fin >> m_pFont[i].s_fRight;
		fin >> m_pFont[i].s_nSize;
	}
	fin.close();

	return true;
}




////////////////////////////////////////////////////////////////////////////////////
// _ReleaseFontData()
void CFont::_ReleaseFontData()
{
	if (m_pFont){
		delete[] m_pFont;
		m_pFont = NULL;
	}
	return;
}




////////////////////////////////////////////////////////////////////////////////////
// _LoadTexture()
bool CFont::_LoadTexture(ID3D11Device* _pDev, WCHAR* _pFilename)
{
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(_pDev, _pFilename, 0, 0, &m_pTextureSRV, 0);
	if (FAILED(result)){
		return false;
	}
	return true;
}




////////////////////////////////////////////////////////////////////////////////////
// _ReleaseTexture()
void CFont::_ReleaseTexture()
{
	if (m_pTextureSRV){
		m_pTextureSRV->Release();
		m_pTextureSRV = NULL;
	}
	return;
}




////////////////////////////////////////////////////////////////////////////////////
// _BuildVertexArray()
void CFont::_BuildVertexArray(void* _pVertices, char* _pText, float _fX, float _fY)
{
	CFontShader::VERTEX* pVertex;
	int numLetters, index, letter;

	pVertex = (CFontShader::VERTEX*)_pVertices;

	numLetters = (int)strlen(_pText);
	index = 0;

	for (UINT i = 0; i < (UINT)numLetters; ++i){
		letter = ((int)_pText[i]) - 32;

		if (letter == 0) _fX = _fX + 3.0f;
		else{
			pVertex[index].s_vPosition = D3DXVECTOR3(_fX, _fY, 0.f);
			pVertex[index].s_vUV = D3DXVECTOR2(m_pFont[letter].s_fLeft, 0.0f);
			index++;

			pVertex[index].s_vPosition = D3DXVECTOR3(_fX + m_pFont[letter].s_nSize, _fY - 16, 0.f);
			pVertex[index].s_vUV = D3DXVECTOR2(m_pFont[letter].s_fRight, 1.0f);
			index++;

			pVertex[index].s_vPosition = D3DXVECTOR3(_fX, _fY - 16, 0.f);
			pVertex[index].s_vUV = D3DXVECTOR2(m_pFont[letter].s_fLeft, 1.0f);
			index++;


			pVertex[index].s_vPosition = D3DXVECTOR3(_fX, _fY, 0);
			pVertex[index].s_vUV = D3DXVECTOR2(m_pFont[letter].s_fLeft, 0.0f);
			index++;

			pVertex[index].s_vPosition = D3DXVECTOR3(_fX + m_pFont[letter].s_nSize, _fY, 0);
			pVertex[index].s_vUV = D3DXVECTOR2(m_pFont[letter].s_fRight, 0.0f);
			index++;

			pVertex[index].s_vPosition = D3DXVECTOR3(_fX + m_pFont[letter].s_nSize, _fY - 16, 0);
			pVertex[index].s_vUV = D3DXVECTOR2(m_pFont[letter].s_fRight, 0.0f);
			index++;

			_fX = _fX + m_pFont[letter].s_nSize + 1.0f;
		}
	}
	return;
}




////////////////////////////////////////////////////////////////////////////////////
// Init()
bool CFont::CreateFont(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC,
						char* _pFontFilename, WCHAR* _pTextureFilename,
						CCamera* _pCamera)
{
	m_pDev = _pDev;
	m_pDevC = _pDevC;

	bool result;

	result = _LoadFontData(_pFontFilename);
	if (!result){
		return false;
	}

	result = _LoadTexture(_pDev, _pTextureFilename);
	if (!result){
		return false;
	}

	//----------------------------------------------------------
	// 폰트 쉐이더 생성.
	//----------------------------------------------------------
	m_pFontShader = new CFontShader();
	m_pFontShader->Create(_pDev, _pDevC, L"../FX/FontShader.fx");

	m_pFontShader->SetCamera(_pCamera);

	//----------------------------------------------------------
	// 정점 및 버퍼 초기화.
	//----------------------------------------------------------
	CFontShader::VERTEX* vtx = new CFontShader::VERTEX[6 * MAX_LENGTH];
	memset(vtx, 0, sizeof(CFontShader::VERTEX) * 6 * MAX_LENGTH);

	D3D11_BUFFER_DESC vdesc;
	vdesc.Usage = D3D11_USAGE_DYNAMIC;
	vdesc.ByteWidth = sizeof(CFontShader::VERTEX) * 6 * MAX_LENGTH;
	vdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vdesc.MiscFlags = 0;
	vdesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = vtx;
	vdata.SysMemPitch = 0;
	vdata.SysMemSlicePitch = 0;

	_pDev->CreateBuffer(&vdesc, 0, &m_pVB);

	delete[] vtx;

	//----------------------------------------------------------
	// 인덱스 및 버퍼 초기화.
	//----------------------------------------------------------
	INDEX* index = new INDEX[6 * MAX_LENGTH];
	for (UINT i = 0; i < 6 * MAX_LENGTH; ++i){
		index->s_iIndex[0] = 0 + (3 * i);
		index->s_iIndex[1] = 1 + (3 * i);
		index->s_iIndex[2] = 2 + (3 * i);
	}


	D3D11_BUFFER_DESC idesc;
	idesc.Usage = D3D11_USAGE_DEFAULT;
	idesc.ByteWidth = sizeof(INDEX) * 6 * MAX_LENGTH;
	idesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idesc.CPUAccessFlags = 0;
	idesc.MiscFlags = 0;
	idesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = index;
	idata.SysMemPitch = 0;
	idata.SysMemSlicePitch = 0;
	_pDev->CreateBuffer(&idesc, &idata, &m_pIB);

	delete[] index;

	return true;
}




////////////////////////////////////////////////////////////////////////////////////
// Release()
void CFont::Release()
{
	_ReleaseTexture();
	_ReleaseFontData();

	return;
}




////////////////////////////////////////////////////////////////////////////////////
// DrawText
void CFont::DrawText(float _fX, float _fY, char* _pMsg, D3DXCOLOR _cColor)
{
	int MsgNum = (int)strlen(_pMsg);
	if ( MsgNum > MAX_LENGTH ) return;

	CFontShader::VERTEX* pVtx = new CFontShader::VERTEX[MsgNum * 6];
	if (!pVtx) return;
	memset(pVtx, 0, (sizeof(CFontShader::VERTEX) * MsgNum));

	int x, y;

	x = ((m_pFontShader->GetCamera()->GetScreenWidth() / 2) * -1) + _fX;
	y = (m_pFontShader->GetCamera()->GetScreenHeight() / 2) - _fY;
	D3DXMatrixTranslation(&m_mWorld, (float)x, (float)y, 1.0f);
	_BuildVertexArray((void*)pVtx, _pMsg, (float)x, (float)y);

	// 정점버퍼 재설정.
	D3D11_MAPPED_SUBRESOURCE mappedData;
	m_pDevC->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	CFontShader::VERTEX* v = (CFontShader::VERTEX*)mappedData.pData;
	memcpy(v, (void*)pVtx, (sizeof(CFontShader::VERTEX) * MsgNum));
	m_pDevC->Unmap(m_pVB, 0);

	delete[] pVtx;
	pVtx = NULL;

	m_pFontShader->SetWorld(&m_mWorld);
	m_pFontShader->SetColor(&_cColor);
	m_pFontShader->SetTexture(&m_pTextureSRV);

	m_pFontShader->SettingDataToGPU();

	UINT stride, offset;
	
	stride = sizeof(CFontShader::VERTEX);
	offset = 0;

	m_pDevC->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	m_pDevC->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	m_pDevC->DrawIndexed(MsgNum, 0, 0);




}





//****************************** End of File "CFont.cpp" ******************************//