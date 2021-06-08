//****************************************************************************************
//	2015.02.09
//	CTerrain.cpp 지형 관련 소스 파일.		
//****************************************************************************************
#include "CTerrain.h"





///////////////////////////////////////////////////////////////////////////////////////////
// CTerrain()
CTerrain::CTerrain(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShader, InitInfo _Info)
{
	m_pDev = _pDev;
	m_pDevC = _pDevC;
	m_pShader = _pShader;
	m_Info = _Info;
	m_pMesh = NULL;

	m_nPatch_CntX = 0;
	m_nPatch_CntZ = 0;
	m_nPatch_Offset = 1;

	m_nVtx_CntX = 0;
	m_nVtx_CntZ = 0;
	m_nOrgVtxCnt = 0;

	_InitHeightMap();
}




///////////////////////////////////////////////////////////////////////////////////////////
// ~CTerrain()
CTerrain::~CTerrain()
{
	SAFE_DELETE(m_pMesh);
	m_HeightMapDataList.clear();
}




/////////////////////////////////////////////////////////////////////////////////////////////
//// _InitHeightMap() : 사용X
//bool CTerrain::_InitHeightMap()
//{
//	//=======================================================================
//	// 텍스처의 정보 가져오기.
//	//=======================================================================
//	D3DX11_IMAGE_INFO info;
//	D3DX11GetImageInfoFromFile(m_Info.s_pHeightMapFilename, NULL, &info, NULL);
//
//	m_Info.s_nHeightMapWidth = info.Width;
//	m_Info.s_nHeightMapDepth = info.Height;
//
//	m_nPatch_CntX = m_Info.s_nHeightMapWidth;
//	m_nPatch_CntZ = m_Info.s_nHeightMapDepth;
//	m_nPatch_Offset = 1;
//
//	m_nVtx_CntX = m_Info.s_nHeightMapWidth + 1;
//	m_nVtx_CntZ = m_Info.s_nHeightMapDepth + 1;
//
//	m_nOrgVtxCnt = m_nVtx_CntX * m_nVtx_CntZ;
//
//	//========================================================================
//	// 텍스처 가져오기.
//	// 방법 1 : ShaderResourceView 에서 가져오는 방법.
//	// 방법 2 : Resource에서 가져오는 방법.
//	// 현재 '방법 2'를 이용하여 텍스쳐를 가져오고 있음.
//	//========================================================================
//	D3DX11_IMAGE_LOAD_INFO imageLoadinfo;
//	imageLoadinfo.Width = info.Width;
//	imageLoadinfo.Height = info.Height;
//	imageLoadinfo.Depth = info.Depth;
//	imageLoadinfo.FirstMipLevel = 0;
//	imageLoadinfo.MipLevels = info.MipLevels;
//	imageLoadinfo.Usage = D3D11_USAGE_STAGING;
//	imageLoadinfo.BindFlags = 0;
//	imageLoadinfo.CpuAccessFlags = D3D11_CPU_ACCESS_READ;
//	imageLoadinfo.MiscFlags = info.MiscFlags;
//	imageLoadinfo.Format = info.Format;
//	imageLoadinfo.Filter = D3DX11_FILTER_LINEAR;
//	imageLoadinfo.MipFilter = D3DX11_FILTER_LINEAR;
//	imageLoadinfo.pSrcInfo = NULL;
//
//	ID3D11Resource* pResource;
//	ID3D11Texture2D* pHeightMapTex;
//	D3DX11CreateTextureFromFile(m_pDev, m_Info.s_pHeightMapFilename, &imageLoadinfo, NULL, &pResource, NULL);
//
//	D3D11_RESOURCE_DIMENSION type;
//	pResource->GetType(&type);
//
//	if (type == D3D10_RESOURCE_DIMENSION_TEXTURE2D)
//		pHeightMapTex = (ID3D11Texture2D*)pResource;
//	else return false;
//	
//	//==========================================================================
//	// 그리드(메쉬) 생성.
//	//==========================================================================
//	m_pMesh = new CGridMesh();
//	m_pMesh->Create(m_Info.s_nHeightMapWidth, m_Info.s_nHeightMapDepth, m_nVtx_CntX, m_nVtx_CntZ);
//	vector<VERTEX> pvtx = m_pMesh->GetVertex();
//
//
//	//========================================================================
//	// Lock을 걸어 픽셀 정보 읽기.
//	//========================================================================
//	D3D11_MAPPED_SUBRESOURCE MappedResource;
//	m_pDevC->Map(pHeightMapTex,
//		//D3D11CalcSubresource(0, 0, 1),
//		0,
//		D3D11_MAP_WRITE_DISCARD,
//		0, &MappedResource);
//
//	char* pTex = (char*)MappedResource.pData;
//
//	//===========================================================================================
//	// 패치 구성.
//	//
//	// * DirectX9의 D3DLOCKED_RECT의 Pitch와 DirectX11의 D3D11_MAPPED_SUBRESOURCE의 RowPitcch 차이.
//	//
//	// ex) 2x2Pixel
//	//
//	// |-                               RowPitch                                   -|
//	// |-              Pitch              -|
//	//  ----------------------------------------------------------------------------------------
//	// |   R    |   G    |   B    |    A   ||    R    |    G    |    B    |    A    ||    R    | ...
//	//  ----------------------------------------------------------------------------------------
//	//  <-------------- 1열 1행 -----------> <--------------- 1열 2행 ---------------> <-------- 2열 2행...
//	//
//	//===========================================================================================
//
//	// 정점의 시작 위치 설정.
//	float sx = -((float)m_nPatch_CntX * m_nPatch_Offset / 2.0f);
//	float sz = ((float)m_nPatch_CntZ * m_nPatch_Offset / 2.0f);
//
//	for (UINT z = 0; z < m_nVtx_CntZ; z++){
//		for (UINT x = 0; x < m_nVtx_CntX; x++){
//			UINT CurrPixel = z * MappedResource.RowPitch + (x * 4);
//			//--------------------------------------------------------------
//			//UINT R = pTex[CurrPixel + 0]; // R
//			//UINT G = pTex[CurrPixel + 1]; // G
//			//UINT B = pTex[CurrPixel + 2]; // B
//			//UINT A = pTex[CurrPixel + 3]; // A
//			//--------------------------------------------------------------
//			float height = (pTex[CurrPixel + 0] / 255.f) * m_Info.s_fHeightScale;
//
//			//--------------------------------------------------------------
//			//마지막 요소는, 패치개수를 2n로 만들기 위해
//			//강제로 확장한 것이므로, 참조할 올바른 높이값이 존재하지 않습니다. 
//			//따라서, 각 행의 마지막 요소값을 가짐.
//			//--------------------------------------------------------------
//			if (x == m_nVtx_CntX - 1) //마지막 행..보정
//			{
//				DWORD addr = z * m_nVtx_CntX + (x - 1);
//				height = pvtx[addr].s_vPosition.y;
//			}
//			if (z == m_nVtx_CntZ - 1)//마지막열..보정
//			{
//				DWORD addr = (z - 1) * m_nVtx_CntX + (x);
//				height = pvtx[addr].s_vPosition.y;
//			}
//
//			//--------------------------------------------------------------
//			// 정점 버퍼 위치 계산.
//			//--------------------------------------------------------------
//			
//			// 그리드 높이 수정.
//			// 나중에 높이에 따른 노멀도 수정.(예정)
//			pvtx[z * m_nVtx_CntX + x].s_vPosition.y = height;
//		}
//	}
//
//	//========================================================================
//	// UnLock
//	//========================================================================
//	m_pDevC->Unmap(pResource, D3D11CalcSubresource(0, 0, 1));
//	
//	//========================================================================
//	// 정점 버퍼 및 인덱스 버퍼 생성.
//	//========================================================================
//	m_pMesh->CreateBuffer(m_pDev);
//
//	return true;
//}




///////////////////////////////////////////////////////////////////////////////////////////
// _InitHeightMap()
bool CTerrain::_InitHeightMap()
{
	//=======================================================================
	// 텍스처의 정보 가져오기.
	//=======================================================================
		/*D3DX11_IMAGE_INFO info;
		D3DX11GetImageInfoFromFile(m_Info.s_pHeightMapFilename, NULL, &info, NULL);
	
		m_Info.s_nHeightMapWidth = info.Width;
		m_Info.s_nHeightMapDepth = info.Height;*/

		m_nPatch_CntX = m_Info.s_nHeightMapWidth;
		m_nPatch_CntZ = m_Info.s_nHeightMapDepth;
		m_nPatch_Offset = 1;
	
		m_nVtx_CntX = m_Info.s_nHeightMapWidth + 1;
		m_nVtx_CntZ = m_Info.s_nHeightMapDepth + 1;
	
		m_nOrgVtxCnt = m_nVtx_CntX * m_nVtx_CntZ;
		_LoadHeightMap();
		_HeightSmooth();

		m_pMesh = new CGridMesh();
		m_pMesh->Create((float)m_nPatch_CntX, (float)m_nPatch_CntZ, m_nPatch_CntX+1, m_nPatch_CntZ+1);
		vector<VERTEX> vtx = m_pMesh->GetVertex();
		for (UINT i = 0; i < m_pMesh->GetVertex().size(); ++i)
			vtx[i].s_vPosition.y = m_HeightMapDataList[i];
		m_pMesh->SetVertex(vtx);

		_NormalModified();
		m_pMesh->CreateBuffer(m_pDev);

		return true;
}




///////////////////////////////////////////////////////////////////////////////////////////
// _LoadHeightMap()
void CTerrain::_LoadHeightMap()
{
	vector<UCHAR> in(m_Info.s_nHeightMapWidth * m_Info.s_nHeightMapDepth);

	std::ifstream inFile;
	inFile.open(m_Info.s_pHeightMapFilename, ios_base::binary);

	if (inFile){
		inFile.read((char*)&in[0], (streamsize)in.size());
		inFile.close();
	}

	m_HeightMapDataList.resize(m_nOrgVtxCnt, 0);

	for (UINT z = 0; z < m_nVtx_CntZ; ++z){
		for (UINT x = 0; x < m_nVtx_CntX; ++x){
			if (x == m_nVtx_CntX - 1){
				UINT addr = z * m_nVtx_CntX + (x - 1);
				m_HeightMapDataList[z * m_nVtx_CntX + x] = m_HeightMapDataList[addr];
			}
			else if (z == m_nVtx_CntZ - 1){
				UINT addr = (z - 1) * m_nVtx_CntX + x;
				m_HeightMapDataList[z * m_nVtx_CntX + x] = m_HeightMapDataList[addr];
			}
			else{
				UINT addr = z * (m_nVtx_CntX-1) + x;
				m_HeightMapDataList[z * m_nVtx_CntX + x] = (in[addr] / 255.0f) * m_Info.s_fHeightScale;
			}
		}
	}
}




///////////////////////////////////////////////////////////////////////////////////////////
// _HeightSmooth()
void CTerrain::_HeightSmooth()
{
	vector<float> dest(m_HeightMapDataList.size());
	for (UINT i = 0; i < m_nVtx_CntZ; ++i){
		for (UINT j = 0; j < m_nVtx_CntX; ++j){
			dest[i * m_nVtx_CntX + j] = _Average(i, j);
		}
	}
	m_HeightMapDataList = dest;
}




///////////////////////////////////////////////////////////////////////////////////////////
// _Average()
float CTerrain::_Average(float _fX, float _fY)
{
	float avg = 0.0f;
	float num = 0.0f;

	for (int i = (int)_fX - 1; i <= (int)_fX + 1; ++i){
		for (int j = (int)_fY - 1; j <= (int)_fY + 1; ++j){
			if (i >= 0 && i < (int)m_nVtx_CntZ &&
				j >= 0 && j < (int)m_nVtx_CntX){
				avg += m_HeightMapDataList[i * m_nVtx_CntX + j];
				num += 1.0f;
			}
		}
	}
	return avg / num;
}




///////////////////////////////////////////////////////////////////////////////////////////
// _NormalModified
void CTerrain::_NormalModified()
{
	vector<VERTEX> Vtx = m_pMesh->GetVertex();
	vector<INDEX> Face = m_pMesh->GetIndex();

	UINT FaceCount = m_nPatch_CntX * m_nPatch_CntZ * 2;

	for (UINT i = 0; i < FaceCount; i++){
		D3DXVECTOR3 v0 = Vtx[Face[i].s_iIndex[0]].s_vPosition;
		D3DXVECTOR3 v1 = Vtx[Face[i].s_iIndex[1]].s_vPosition;
		D3DXVECTOR3 v2 = Vtx[Face[i].s_iIndex[2]].s_vPosition;

		D3DXVECTOR3 n0 = Vtx[Face[i].s_iIndex[0]].s_vNormal;;
		D3DXVECTOR3 n1 = Vtx[Face[i].s_iIndex[1]].s_vNormal;
		D3DXVECTOR3 n2 = Vtx[Face[i].s_iIndex[2]].s_vNormal;

		D3DXVECTOR3 fn; D3DXVec3Cross(&fn, &(v1 - v0), &(v2 - v0));
		D3DXVec3Normalize(&fn, &fn);

		D3DXVECTOR3 hn0 = fn + n0; D3DXVec3Normalize(&hn0, &hn0);
		D3DXVECTOR3 hn1 = fn + n1; D3DXVec3Normalize(&hn1, &hn1);
		D3DXVECTOR3 hn2 = fn + n2; D3DXVec3Normalize(&hn2, &hn2);

		Vtx[Face[i].s_iIndex[0]].s_vNormal = hn0;
		Vtx[Face[i].s_iIndex[1]].s_vNormal = hn1;
		Vtx[Face[i].s_iIndex[2]].s_vNormal = hn2;
	}
	m_pMesh->SetVertex(Vtx);
}




///////////////////////////////////////////////////////////////////////////////////////////
// Render()
void CTerrain::Render()
{
	D3DXMATRIX TM;
	D3DXMatrixIdentity(&TM);

	m_pShader->SetWorld(&TM);
	m_pShader->SettingDataToGPU();
	m_pMesh->Render(m_pDevC);
}






//****************************** End of File "CTerrain.cpp ******************************//
