//**********************************************************************************
//	2015.01.02.
//	CMesh.h : 기본 메쉬 관련 헤더 파일. 
//**********************************************************************************
#pragma once
#include "Common.h"
#include "CShader.h"

////////////////////////////////////////////////////////////////////////////////////
//
// CMeshNode : 메쉬 노드
//
////////////////////////////////////////////////////////////////////////////////////
class CMeshNode
{
public:
	CMeshNode();
	~CMeshNode();
protected:
	string m_pMeshName;
	CMeshNode* m_pParentNode;
	vector<CMeshNode*> m_pChildNodeList;

	vector<CBasicShader::VERTEX> m_VertexList;
	ID3D11Buffer* m_pVB;

	vector<INDEX> m_IndexList;
	ID3D11Buffer* m_pIB;

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vScale;
	D3DXVECTOR3 m_vRot;

	D3DXMATRIX m_mTM;
	D3DXMATRIX m_mTrans;
	D3DXMATRIX m_mScale;
	D3DXMATRIX m_mRot;

	MATERIAL m_Material;

	WCHAR* m_pDiffuseMapname;
	WCHAR* m_pNormalMapname;
	WCHAR* m_pSpecularMapname;

	ID3D11ShaderResourceView* m_pDiffuseMapSRV;		// 디퓨즈 맵.
	ID3D11ShaderResourceView* m_pNormalMapRSV;		// 노멀 맵.
	ID3D11ShaderResourceView* m_pSpecularMapRSV;	// 스펙큘러 맵.
	
public:
	void Render(ID3D11DeviceContext* _pDevC);
	void Release();


	void SetName(string _pName)			{ m_pMeshName = _pName; }
	string GetName()						{ return m_pMeshName; }

	void SetParent(CMeshNode* _pNode)	{ m_pParentNode = _pNode; }
	CMeshNode* GetParent()				{ return m_pParentNode; }

	void SetChild(CMeshNode* _pNode){
		_pNode->SetParent(this);
		m_pChildNodeList.push_back(_pNode);
	}
	CMeshNode* GetChild(int _iIndex = -1)	{ return (_iIndex == -1 ? m_pChildNodeList.back() : m_pChildNodeList[_iIndex]); }

	void SetVertex(CBasicShader::VERTEX& _Vtx) { m_VertexList.push_back(_Vtx); }
	vector<CBasicShader::VERTEX>& GetVertexList() { return m_VertexList; }
	CBasicShader::VERTEX GetVertex(int _i) { return m_VertexList[_i]; }

	void SetIndex(INDEX& _Index) { m_IndexList.push_back(_Index); }
	vector<INDEX>& GetIndexList() { return m_IndexList; }
	INDEX GetIndex(int _i) { return m_IndexList[_i]; }

	void SetVertexBuffer(ID3D11Buffer* _pBuffer)	{ m_pVB = _pBuffer; }
	ID3D11Buffer** GetVertexBuffer()				{ return &m_pVB; }

	void SetIndexBuffer(ID3D11Buffer* _pBuffer)	{ m_pIB = _pBuffer; }
	ID3D11Buffer** GetIndexBuffer()				{ return &m_pIB; }


	void SetPos(D3DXVECTOR3 _vPos) { 
		m_vPos = _vPos;
		D3DXMatrixTranslation(&m_mTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	}
	D3DXVECTOR3 GetPos() { return m_vPos; }
	D3DXMATRIX GetTransMatrix() { return m_mTrans; }

	void SetScale(D3DXVECTOR3 _vScale) { 
		m_vScale = _vScale; 
		D3DXMatrixScaling(&m_mScale, m_vScale.x, m_vScale.y, m_vScale.z);
	}
	D3DXVECTOR3 GetScale() { return m_vScale; }
	D3DXMATRIX GetScaleMatrix() { return m_mScale; }


	void SetRot(D3DXVECTOR3 _vRot) { 
		m_vRot = _vRot; 
		D3DXMatrixRotationYawPitchRoll(&m_mRot, m_vRot.y, m_vRot.x, m_vRot.z);
	}
	D3DXVECTOR3 GetRot() { return m_vRot; }
	D3DXMATRIX GetRotMatrix() { return m_mRot; }

	void SetTM(D3DXMATRIX* _mTM) { m_mTM = *_mTM; }
	D3DXMATRIX GetTM() { return m_mTM; }

	// 디퓨즈맵 SRV의 주소값을 반환.
	ID3D11ShaderResourceView** GetDiffuseMapSRV() { return &m_pDiffuseMapSRV; } 
	// 노멀맵 SRV의 주소값을 반환.
	ID3D11ShaderResourceView** GetNormalMapSRV() { return &m_pNormalMapRSV; }
	// 스펙큘러맵 SRV의 주소값을 반환.
	ID3D11ShaderResourceView** GetSpecularMapSRV() { return &m_pSpecularMapRSV; }

	WCHAR* GetDiffuseMapname(){ return m_pDiffuseMapname; }
	void SetDiffuseMapname(WCHAR* _name) { m_pDiffuseMapname = _name; }

	WCHAR* GetNormalMapname(){ return m_pNormalMapname; }
	void SetNormalMapname(WCHAR* _name) { m_pNormalMapname = _name; }

	WCHAR* GetSpecularMapname() { return m_pSpecularMapname; }
	void SetSpecularMapname(WCHAR* _name) { m_pSpecularMapname = _name; }


	UINT GetChildCount() { return m_pChildNodeList.size(); }
};






////////////////////////////////////////////////////////////////////////////////////
//
//	CMesh : 메쉬 관련 기본 클래스.
//
////////////////////////////////////////////////////////////////////////////////////
class CMesh
{
public :
	CMesh();
	virtual ~CMesh();
protected :
	ID3D11Buffer* m_pVB;
	ID3D11Buffer* m_pIB;

	UINT m_nVtxCnt;
	UINT m_nIndexCnt;

	vector<CBasicShader::VERTEX> m_vVtx;
	vector<INDEX> m_vIndex;
protected :
	virtual void _InitVB() = 0;
	virtual void _InitIB() = 0;
public :
	void CreateBuffer(ID3D11Device* _pDev);
	void Render(ID3D11DeviceContext* _pDevC);
public :
	vector<CBasicShader::VERTEX> GetVertexList() { return m_vVtx; }
	void SetVertexList(vector<CBasicShader::VERTEX> vtx) { m_vVtx = vtx; }
	CBasicShader::VERTEX GetVertex(int _i) { return m_vVtx[_i]; }

	vector<INDEX> GetIndexList() { return m_vIndex; }
	void SetIndexList(vector<INDEX> Index) { m_vIndex = Index; }
	INDEX GetIndex(int _i) { return m_vIndex[_i]; }

	ID3D11Buffer* GetVB() { return m_pVB; }
	ID3D11Buffer* GetIB() { return m_pIB; }
};


////////////////////////////////////////////////////////////////////////////////////
//
//	CCubeMesh : 큐브 메쉬.
//
////////////////////////////////////////////////////////////////////////////////////
class CCubeMesh : public CMesh
{
public :
	CCubeMesh(float _fWidth = 1.0f, float _fheight = 1.0, float _fDepth = 1.0f);
	virtual ~CCubeMesh();
protected :
	float m_fWidth;
	float m_fHeight;
	float m_fDepth;
protected :
	virtual void _InitVB();
	virtual void _InitIB();
public :
	void Create(float _fWidth = 1.0f, float _fheight = 1.0, float _fDepth = 1.0f);
};




////////////////////////////////////////////////////////////////////////////////////
//
//	CGrid : 그리드 메쉬.
//
////////////////////////////////////////////////////////////////////////////////////
class CGridMesh : public CMesh
{
public: 
	CGridMesh();
	virtual ~CGridMesh();
protected :
	float m_fWidth;
	float m_fDepth;
	UINT m_nM;
	UINT m_nN;
protected :
	virtual void _InitVB();
	virtual void _InitIB();
public :
	void Create(float _fWidth, float _fDepth, UINT _nM, UINT _nN);
	
	void GetSize(float* _pW, float* _pD) { *_pW = m_fWidth, *_pD = m_fDepth; }
};




////////////////////////////////////////////////////////////////////////////////////
//
//	CPlaneMesh : 평면 메쉬.
//
////////////////////////////////////////////////////////////////////////////////////
class CPlaneMesh : public CMesh
{
public :
	CPlaneMesh();
	virtual ~CPlaneMesh();
protected : 
	float m_fWidth;
	float m_fHeight;
protected :
	virtual void _InitVB();
	virtual void _InitIB();
public :
	void Create(float _fWidth, float _fHeight);
	
	void GetSize(float* _pW, float* _pH) { *_pW = m_fWidth, *_pH = m_fHeight; }
};




////////////////////////////////////////////////////////////////////////////////////
//
//	CSphereMesh : 스페어 관련 기본 클래스.
//
////////////////////////////////////////////////////////////////////////////////////
class CSphereMesh : public CMesh
{
public :
	CSphereMesh();
	virtual ~CSphereMesh();
protected :
	float m_fRadius;
	UINT m_nSliceCount;
protected :
	virtual void _InitVB();
	virtual void _InitIB();
public :
	void Create(float radius, UINT sliceCount, UINT stackCount);
};



//****************************** End of File "CMesh.h" ******************************//