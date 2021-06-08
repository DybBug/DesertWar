//****************************************************************************************
//	2015.02.09
//	CTerrain.h ���� ���� ��� ����.		
//****************************************************************************************
#pragma once
#include "Common.h"
#include "CMesh.h"
#include "CShader.h"




///////////////////////////////////////////////////////////////////////////////////////////
//
// CTerrain
//
///////////////////////////////////////////////////////////////////////////////////////////
#define LAYERMAP_MAX 5
class CTerrain
{
public :
	struct InitInfo{
		WCHAR* s_pHeightMapFilename;

		WCHAR* s_pLayerMapFilenameList[LAYERMAP_MAX];
		WCHAR* s_pBlendMapFilename;

		UINT s_nHeightMapWidth;
		UINT s_nHeightMapDepth;

		// ���̸� ���Ͽ��� �о���� ���̰��鿡 ������ ��� ���.
		float s_fHeightScale;

		// �������� �� ĭ�� ũ��.
		float s_fCellSpacing;
	};

public :
	CTerrain(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShader, InitInfo _Info);
	virtual ~CTerrain();
protected :
	ID3D11Device* m_pDev;
	ID3D11DeviceContext* m_pDevC;
	CShader* m_pShader;

	InitInfo m_Info;
	CGridMesh* m_pMesh;

	UINT m_nPatch_CntX;	
	UINT m_nPatch_CntZ;	
	UINT m_nPatch_Offset;

	UINT m_nVtx_CntX;
	UINT m_nVtx_CntZ;
	UINT m_nOrgVtxCnt;

	vector<float> m_HeightMapDataList;

protected :
	bool _InitHeightMap();
	void _LoadHeightMap();
	void _HeightSmooth();
	float _Average(float _fX, float _fY);
	void _NormalModified();

public :
	void Render();
};

//****************************** End of File "CTerrain.h" ******************************//
