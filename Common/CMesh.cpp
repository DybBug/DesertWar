//**********************************************************************************
//	2015.01.02.
//	CMesh.cpp : 기본 메쉬 관련 소스파일
//**********************************************************************************
#include "CMesh.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// CMeshNode
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// CMeshNode()
CMeshNode::CMeshNode()
: m_pParentNode(NULL), m_pVB(NULL), m_pIB(NULL)
{
	m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vScale = D3DXVECTOR3(1.f, 1.f, 1.f);
	m_vRot = D3DXVECTOR3(0.f, 0.f, 0.f);

	D3DXMatrixIdentity(&m_mTM);
	D3DXMatrixIdentity(&m_mTrans);
	D3DXMatrixIdentity(&m_mRot);
	D3DXMatrixIdentity(&m_mScale);

	ZeroMemory(&m_Material, sizeof(MATERIAL));

	m_pDiffuseMapSRV = NULL;
	m_pNormalMapRSV = NULL;

}



//////////////////////////////////////////////////////////////////////////////////////////
// ~CMeshNode()
CMeshNode::~CMeshNode()
{
	m_pParentNode = NULL;
	m_pChildNodeList.clear();
	m_VertexList.clear();
	m_IndexList.clear();
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}




//////////////////////////////////////////////////////////////////////////////////////////
// Draw()
void CMeshNode::Render(ID3D11DeviceContext* _pDevC)
{
	if (m_pParentNode == NULL) return;

	UINT stride = sizeof(CBasicShader::VERTEX);
	UINT offset = 0;

	_pDevC->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	_pDevC->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

//	_pDevC->Draw(m_VertexList.size(), 0);
	_pDevC->DrawIndexed(m_IndexList.size() * 3 , 0, 0);
}




//////////////////////////////////////////////////////////////////////////////////////////
// Release()
void CMeshNode::Release()
{
	for (UINT i = 0; i < m_pChildNodeList.size(); ++i){
		m_pChildNodeList[i]->Release();
	}
	delete this;
}



//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//	CMesh
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// CMesh()
CMesh::CMesh()
{
	m_pVB = NULL;
	m_pIB = NULL;

	m_nVtxCnt = 0;
	m_nIndexCnt = 0;
}




//////////////////////////////////////////////////////////////////////////////////////////
// ~CMesh()
CMesh::~CMesh()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}




//////////////////////////////////////////////////////////////////////////////////////////
// CreateBuffer()
void CMesh::CreateBuffer(ID3D11Device* _pDev)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(CBasicShader::VERTEX)* m_nVtxCnt;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	

	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &m_vVtx[0];
	_pDev->CreateBuffer(&vbd, &vdata, &m_pVB);



	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(INDEX)* m_nIndexCnt;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &m_vIndex[0];
	_pDev->CreateBuffer(&ibd, &idata, &m_pIB);
}




//////////////////////////////////////////////////////////////////////////////////////////
// Render()
void CMesh::Render(ID3D11DeviceContext* _pDevC)
{
	UINT stride = sizeof(CBasicShader::VERTEX);
	UINT offset = 0;
	_pDevC->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	_pDevC->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	_pDevC->DrawIndexed(m_nIndexCnt * 3, 0, 0);
}




//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//	CCubeMesh()
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// CCubeMesh()
CCubeMesh::CCubeMesh(float _fWidth, float _fheight, float _fDepth)
: CMesh()
{
	Create(_fWidth, _fheight, _fDepth);
}





//////////////////////////////////////////////////////////////////////////////////////////
// ~CCubeMesh()
CCubeMesh::~CCubeMesh()
{

}




//////////////////////////////////////////////////////////////////////////////////////////
// _InitVB()
void CCubeMesh::_InitVB()
{
	float w = 0.5f * m_fWidth;
	float h = 0.5f * m_fHeight;
	float d = 0.5f * m_fDepth;

	m_vVtx = {
			{ D3DXVECTOR3(-w, -h, -d), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 1.0f) },
			{ D3DXVECTOR3(-w, +h, -d), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) },
			{ D3DXVECTOR3(+w, +h, -d), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 0.0f) },
			{ D3DXVECTOR3(+w, -h, -d), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 1.0f) },

			{ D3DXVECTOR3(-w, -h, +d), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f) },
			{ D3DXVECTOR3(+w, -h, +d), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) },
			{ D3DXVECTOR3(+w, +h, +d), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f) },
			{ D3DXVECTOR3(-w, +h, +d), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f) },

			{ D3DXVECTOR3(-w, +h, -d), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
			{ D3DXVECTOR3(-w, +h, +d), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
			{ D3DXVECTOR3(+w, +h, +d), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
			{ D3DXVECTOR3(+w, +h, -d), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },

			{ D3DXVECTOR3(-w, -h, -d), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
			{ D3DXVECTOR3(+w, -h, -d), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
			{ D3DXVECTOR3(+w, -h, +d), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
			{ D3DXVECTOR3(-w, -h, +d), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },

			{ D3DXVECTOR3(-w, -h, +d), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
			{ D3DXVECTOR3(-w, +h, +d), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
			{ D3DXVECTOR3(-w, +h, -d), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
			{ D3DXVECTOR3(-w, -h, -d), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },

			{ D3DXVECTOR3(+w, -h, -d), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
			{ D3DXVECTOR3(+w, +h, -d), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
			{ D3DXVECTOR3(+w, +h, +d), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
			{ D3DXVECTOR3(+w, -h, +d), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
	};
	m_nVtxCnt = m_vVtx.size();
}



//////////////////////////////////////////////////////////////////////////////////////////
// _InitIB()
void CCubeMesh::_InitIB()
{
	m_vIndex = {
			// fornt face
			{ 0, 1, 2 },
			{ 0, 2, 3 },

			//	back face
			{ 4, 5, 6 },
			{ 4, 6, 7 },

			//	top face
			{ 8, 9, 10 },
			{ 8, 10, 11 },

			//	bottom face
			{ 12, 13, 14 },
			{ 12, 14, 15 },

			//	left face
			{ 16, 17, 18 },
			{ 16, 18, 19 },

			//	right face
			{ 20, 21, 22 },
			{ 20, 22, 23 },
	};
	m_nIndexCnt = m_vIndex.size();
}




//////////////////////////////////////////////////////////////////////////////////////////
// Create()
void CCubeMesh::Create(float _fWidth, float _fHeight, float _fDepth)
{
	m_fWidth = _fWidth, m_fHeight = _fHeight, m_fDepth = _fDepth;

	_InitVB();
	_InitIB();
}




//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// CGridMesh
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// CGridMesh()
CGridMesh::CGridMesh()
: CMesh()
{
	m_fWidth = 0.0f;
	m_fDepth = 0.0f;
	m_nM = 0;
	m_nN = 0;
}




//////////////////////////////////////////////////////////////////////////////////////////
// ~CGricMesh()
CGridMesh::~CGridMesh()
{

}




//////////////////////////////////////////////////////////////////////////////////////////
// _InitVB()
void CGridMesh::_InitVB()
{
	m_nVtxCnt = m_nM * m_nN;

	float halfWidth = 0.5f * m_fWidth;
	float halfDepth = 0.5f * m_fDepth;

	float dx = m_fWidth / (m_nN - 1);
	float dz = m_fDepth / (m_nM - 1);

	float du = 1.0f / (m_nN - 1);
	float dv = 1.0f / (m_nM - 1);

	m_vVtx.resize(m_nVtxCnt);

	for (UINT i = 0; i < m_nM; ++i){
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < m_nN; ++j){
			float x = -halfWidth + j*dx;

			m_vVtx[i * m_nN + j].s_vPosition = D3DXVECTOR3(x, 0.0f, z);
			m_vVtx[i * m_nN + j].s_vNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_vVtx[i * m_nN + j].s_vUV = D3DXVECTOR2(j * du, i * dv);
		}
	}
}




//////////////////////////////////////////////////////////////////////////////////////////
// _InitIB()
void CGridMesh::_InitIB()
{
	m_nIndexCnt = (m_nM - 1) * (m_nN - 1) * 2;

	m_vIndex.resize(m_nIndexCnt);

	UINT k = 0;
	for (UINT i = 0; i < m_nM - 1; ++i){
		for (UINT j = 0; j < m_nN - 1; ++j){
			m_vIndex[k].s_iIndex[0] = i * m_nN + j;
			m_vIndex[k].s_iIndex[1] = i* m_nN + j + 1;
			m_vIndex[k].s_iIndex[2] = (i + 1)*m_nN + j;

			m_vIndex[k + 1].s_iIndex[0] = (i + 1) * m_nN + j;
			m_vIndex[k + 1].s_iIndex[1] = i*m_nN + j + 1;
			m_vIndex[k + 1].s_iIndex[2] = (i + 1)*m_nN + j + 1;

			k += 2;
		}
	}
}




//////////////////////////////////////////////////////////////////////////////////////////
// Create()
void CGridMesh::Create(float _fWidth, float _fDepth, UINT _nM, UINT _nN)
{
	m_fWidth = _fWidth;
	m_fDepth = _fDepth;
	m_nM = _nM;
	m_nN = _nN;

	_InitVB();
	_InitIB();
}




//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//	CPlaneMesh
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// CPlaneMesh()
CPlaneMesh::CPlaneMesh()
:CMesh()
{

}




//////////////////////////////////////////////////////////////////////////////////////////
// ~CPlaneMesh()
CPlaneMesh::~CPlaneMesh()
{

}




//////////////////////////////////////////////////////////////////////////////////////////
// _InitVB()
void CPlaneMesh::_InitVB()
{
	float w = m_fWidth * 0.5f;
	float h = m_fHeight * 0.5f;
	m_vVtx = {
			{ D3DXVECTOR3(-w, -h, 0.f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 1.0f) },
			{ D3DXVECTOR3(-w, +h, 0.f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) },
			{ D3DXVECTOR3(+w, +h, 0.f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 0.0f) },
			{ D3DXVECTOR3(+w, -h, 0.f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 1.0f) },
	};
	m_nVtxCnt = m_vVtx.size();
}




//////////////////////////////////////////////////////////////////////////////////////////
// _InitIB()
void CPlaneMesh::_InitIB()
{
	m_vIndex = {
			{0,1,2},
			{0,2,3}
	};
	m_nIndexCnt = m_vIndex.size();
}




//////////////////////////////////////////////////////////////////////////////////////////
// Create()
void CPlaneMesh::Create(float _fWidth, float _fHeight)
{
	m_fWidth = _fWidth;
	m_fHeight = _fHeight;

	_InitVB();
	_InitIB();
}




//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// CSphereMesh
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
// CSphereMesh()
CSphereMesh::CSphereMesh()
	:CMesh()
{
	m_fRadius = 0.f;
	m_nSliceCount = 0;
}




//////////////////////////////////////////////////////////////////////////////////////////
// ~CSphereMesh()
CSphereMesh::~CSphereMesh()
{

}




//////////////////////////////////////////////////////////////////////////////////////////
// _InitVB()
void CSphereMesh::_InitVB()
{
	CBasicShader::VERTEX TopVtx = { D3DXVECTOR3(0.f, +m_fRadius, 0.f), D3DXVECTOR3(0.f, 1.f, 0.f), D3DXVECTOR2(0.f, 0.f) };
	CBasicShader::VERTEX BottomVtx = { D3DXVECTOR3(0.f, -m_fRadius, 0.f), D3DXVECTOR3(0.f, -1.f, 0.f), D3DXVECTOR2(0.f, 1.f) };

	m_vVtx.push_back(TopVtx);

	float PhiStep = D3DX_PI / m_nSliceCount;
	float ThetaStep = 2.f * D3DX_PI / m_nSliceCount;

	for (UINT i = 1; i < m_nSliceCount - 1; ++i){
		float phi = i * PhiStep;

		for (UINT j = 0; j <= m_nSliceCount; ++j){
			float theta = j * ThetaStep;
			CBasicShader::VERTEX v;

			// position.
			v.s_vPosition.x = m_fRadius * sinf(phi) * cosf(theta);
			v.s_vPosition.y = m_fRadius*cosf(phi);
			v.s_vPosition.z = m_fRadius*sinf(phi)*sinf(theta);

			// Normal.
			D3DXVec3Normalize(&v.s_vNormal, &v.s_vPosition);

			// UV.
			v.s_vUV.x = theta / D3DX_PI;
			v.s_vUV.y = phi / D3DX_PI;

			m_vVtx.push_back(v);
	
		}
	}
	m_vVtx.push_back(BottomVtx);

	m_nVtxCnt = m_vVtx.size();
	return;
}




//////////////////////////////////////////////////////////////////////////////////////////
// _InitIB()
void CSphereMesh::_InitIB()
{
	for (UINT i = 1; i <= m_nSliceCount; ++i)
		m_vIndex.push_back(INDEX(0, i + 1, i));

	UINT baseIndex = 1;
	UINT ringVertexCount = m_nSliceCount + 1;
	for (UINT i = 0; i < m_nSliceCount - 2; ++i){
		for (UINT j = 0; j < m_nSliceCount; ++j){
			m_vIndex.push_back(INDEX(baseIndex + i*ringVertexCount + j, 
									 baseIndex + i*ringVertexCount + j + 1, 
									 baseIndex + (i + 1)*ringVertexCount + j));

			m_vIndex.push_back(INDEX(baseIndex + (i + 1)*ringVertexCount + j,
									 baseIndex + i*ringVertexCount + j + 1,
									 baseIndex + (i + 1)*ringVertexCount + j + 1));
			
		}
	}

	UINT southPoleIndex = (UINT)m_vVtx.size() - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < m_nSliceCount; ++i){
		m_vIndex.push_back(INDEX(southPoleIndex, baseIndex + i, baseIndex + i + 1));
	}

	m_nIndexCnt = m_vIndex.size();

	return;
}




//////////////////////////////////////////////////////////////////////////////////////////
// Create()
void CSphereMesh::Create(float radius, UINT sliceCount, UINT stackCount)
{
	m_fRadius = radius;
	m_nSliceCount = sliceCount;

	_InitVB();
	_InitIB();
}






//****************************** End of File "CMesh.cpp" ******************************//