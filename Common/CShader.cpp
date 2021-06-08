//**********************************************************************************
//	2015.01.02.
//	CShader.cpp : 쉐이더 관련 소스 코드.
//**********************************************************************************
#include "CShader.h"

#pragma region CShader
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CShader : 다른 쉐이더 코드는 이 클래스에서 파생된다.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//	CShader()
CShader::CShader()
: m_pFileName(0), m_pDev(0), m_pDevC(0), m_pFX(0), m_pInputLayout(0), m_pTech(0)
{
	
}




////////////////////////////////////////////////////////////////////////////////////
// ~CShader()
CShader::~CShader()
{
	SAFE_RELEASE(m_pFX);
	SAFE_RELEASE(m_pInputLayout);
}




////////////////////////////////////////////////////////////////////////////////////
// _BuildFX()
void CShader::_BuildFX()
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(m_pFileName, 0, 0, 0, "fx_5_0", shaderFlags,
		0, 0, &compiledShader, &compilationMsgs, 0);

	if (compilationMsgs != 0){
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		SAFE_RELEASE(compilationMsgs);
	}
	/*if (FAILED(hr)){
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}*/
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), 
								 compiledShader->GetBufferSize(),
								 0, m_pDev, &m_pFX );
	SAFE_RELEASE(compiledShader);

	// Technique 가져오기.
	m_pTech = m_pFX->GetTechniqueByName("Tech");
}




////////////////////////////////////////////////////////////////////////////////////
//	_BuildVertexlayout() : 입력조립기 설정.
void CShader::_BuildVertexLayout(D3D11_INPUT_ELEMENT_DESC* _pVtxDesc, int _nDescSize)
{
	assert(m_pTech);

	UINT Count = _nDescSize / sizeof(D3D11_INPUT_ELEMENT_DESC);

	D3DX11_PASS_DESC passDesc;
	m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	m_pDev->CreateInputLayout(_pVtxDesc, Count, passDesc.pIAInputSignature,
	passDesc.IAInputSignatureSize, &m_pInputLayout);
}




////////////////////////////////////////////////////////////////////////////////////
// _Init()
void CShader::_Init()
{
//	_BuildFX();
//	_BuildVertexLayout(_pVtxDesc, _nDescSize);
}





////////////////////////////////////////////////////////////////////////////////////
// Create()
void CShader::Create(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, LPCWSTR _pFileName)
{
	m_pFileName = _pFileName;
	m_pDev = _pDev;
	m_pDevC = _pDevC;
	_Init();
}
#pragma endregion


#pragma region CBasicShader
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CBasicShader : 빛관련 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//	CBasicShader()
CBasicShader::CBasicShader()
: CShader()
{
 
}




////////////////////////////////////////////////////////////////////////////////////
// ~CBasicShader()
CBasicShader::~CBasicShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// _Init()
void CBasicShader::_Init()
{
	D3D11_INPUT_ELEMENT_DESC InputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	_BuildFX();
	_BuildVertexLayout(InputLayout, sizeof(InputLayout));

	m_pfxmWorld = m_pFX->GetVariableByName("g_mWorld")->AsMatrix();
	m_pfxmView = m_pFX->GetVariableByName("g_mView")->AsMatrix();
	m_pfxmProj = m_pFX->GetVariableByName("g_mProj")->AsMatrix();
	m_pfxmVP = m_pFX->GetVariableByName("g_mVP")->AsMatrix();
	m_pfxmWVP = m_pFX->GetVariableByName("g_mWVP")->AsMatrix();
	m_pfxmShadow = m_pFX->GetVariableByName("g_mShadowTransform")->AsMatrix();
	m_pfxvEyeL = m_pFX->GetVariableByName("g_vEyeL")->AsVector();
	m_pfxvTexTranform = m_pFX->GetVariableByName("g_vTexTransformL")->AsVector();

	m_pfxLight = m_pFX->GetVariableByName("g_Light");
	m_pfxMaterial = m_pFX->GetVariableByName("g_Material");
	m_pfxDiffuseMap = m_pFX->GetVariableByName("g_DiffuseMap")->AsShaderResource();
	m_pfxShadowMap = m_pFX->GetVariableByName("g_ShadowMap")->AsShaderResource();

	D3DXMatrixIdentity(&m_mWorld);
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProj);
	D3DXMatrixIdentity(&m_mVP);
	D3DXMatrixIdentity(&m_mWVP);

	m_pCamera = NULL;
	m_vEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vTexTransform = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	
	m_pDiffusMapSRV = NULL;
	m_pShadowMapSRV = NULL;
}




////////////////////////////////////////////////////////////////////////////////////
// _TransformLight() : 빛의 정보를 쉐이더에 넣기전에 알맞게 바꿔준다.
void CBasicShader::_TransformLight(D3DXMATRIX* _pInvWorld)
{
	LIGHT light[LIGHT_MAX];
	for (UINT i = 0; i < LIGHT_MAX; ++i){
		if (m_Light[i].s_fType <= 0.0f) continue;
		light[i] = m_Light[i];

		//	빛 위치, 방향 로컬로 변경
		D3DXVec3TransformCoord(&light[i].s_vPosition, &light[i].s_vPosition, _pInvWorld);
		D3DXVec3TransformNormal(&light[i].s_vDirection, &light[i].s_vDirection, _pInvWorld);

		//	빛 방향 바꾸기.
		light[i].s_vDirection = -light[i].s_vDirection;
		D3DXVec3Normalize(&light[i].s_vDirection, &light[i].s_vDirection);
	}
	//	빛정보 fx에 넘기기.
	m_pfxLight->SetRawValue(&light, 0, sizeof(LIGHT)*LIGHT_MAX);
}




////////////////////////////////////////////////////////////////////////////////////
// SetWorld() : 뷰행렬, 프로젝션 행렬은 '카메라'에서 가져온다.
void CBasicShader::SetWorld(D3DXMATRIX* _pMatrix)
{
	m_mWorld = *_pMatrix;
}




////////////////////////////////////////////////////////////////////////////////////
// SetCamera()
void CBasicShader::SetCamera(CCamera* _pCamera)
{
	m_pCamera = _pCamera;
	m_mView = m_pCamera->GetViewMatrix();

	m_pCamera->GetEnableOrthoLens() ? m_mProj = m_pCamera->GetOrthoMatrix() : m_mProj = m_pCamera->GetProjMatrix();

	m_vEye = m_pCamera->GetEye();
	m_mVP = m_mView * m_mProj;
}




////////////////////////////////////////////////////////////////////////////////////
// SetLight() : 라이팅의 Type이 0 또는 -일 경우 라이팅 사용 x.
void CBasicShader::SetLight(UINT _iIndex, LIGHT* _pLight)
{
	m_Light[_iIndex] = *_pLight;
	// 해당 라이팅 사용 안함.
	m_Light[_iIndex].s_fType = -m_Light[_iIndex].s_fType;
}





////////////////////////////////////////////////////////////////////////////////////
// SetMaterial()
void CBasicShader::SetMaterial(MATERIAL* _pMaterial)
{
	m_Material = *_pMaterial;
}




////////////////////////////////////////////////////////////////////////////////////
// SetTexture()
void CBasicShader::SetTexture(ID3D11ShaderResourceView** _ppShaderResourceView, D3DXVECTOR4* _pTransform)
{
	m_pDiffusMapSRV = *_ppShaderResourceView;

	if (_pTransform)
		m_vTexTransform = *_pTransform;
	else
		m_vTexTransform = D3DXVECTOR4(0.f, 0.f, 1.f, 1.f);
}




////////////////////////////////////////////////////////////////////////////////////
// SetShadowTexture()
void CBasicShader::SetShadowTexture(ID3D11ShaderResourceView** _ppShaderResourceView)
{
	m_pShadowMapSRV = *_ppShaderResourceView;
}




////////////////////////////////////////////////////////////////////////////////////
// EnableLight()
void CBasicShader::EnableLight(UINT _iIndex, bool _bEnable)
{
	if (_bEnable == TRUE){
		(m_Light[_iIndex].s_fType > 0) ? m_Light[_iIndex].s_fType *= 1.0f :
										 m_Light[_iIndex].s_fType *= -1.0f;
	}
	else if (_bEnable == FALSE){
		(m_Light[_iIndex].s_fType > 0) ? m_Light[_iIndex].s_fType *= -1.0f :
										 m_Light[_iIndex].s_fType *= 1.0f;
	}
}




////////////////////////////////////////////////////////////////////////////////////
// SettingDataToGPU() : 설정한 데이터를 GPU에게 세팅해주는 함수.
void CBasicShader::SettingDataToGPU(UINT _iPassNum)
{
	//--------------------------------------------------------------
	//	각종 데이터 처리.
	//--------------------------------------------------------------
	m_mWVP = m_mWorld * m_mVP;
	D3DXMATRIX imWorld;
	D3DXMatrixInverse(&imWorld, 0, &m_mWorld);
	_TransformLight(&imWorld);

	// 카메라 위치 로컬로 변환 후 GPU에게 전달.
	D3DXVECTOR3 vEyeL;
	D3DXVec3TransformCoord(&vEyeL, &m_vEye, &imWorld);

	//--------------------------------------------------------------
	// 상수버퍼 값 채우기.
	//--------------------------------------------------------------
	m_pfxmWorld->SetMatrix((float*)&m_mWorld);
	m_pfxmView->SetMatrix((float*)&m_mView);
	m_pfxmProj->SetMatrix((float*)&m_mProj);
	m_pfxmVP->SetMatrix((float*)&m_mVP);
	m_pfxmWVP->SetMatrix((float*)&m_mWVP);
	m_pfxmShadow->SetMatrix((float*)&(m_mWorld * m_mShadow));
	m_pfxvEyeL->SetRawValue(&vEyeL, 0, sizeof(D3DXVECTOR3));
	m_pfxMaterial->SetRawValue(&m_Material, 0, sizeof(MATERIAL));

	if (_iPassNum == PASS_LIGHTING_TEXTURE || _iPassNum == PASS_LIGHTING_TEXTURE_ALPHACLIP){
		m_pfxvTexTranform->SetRawValue(&m_vTexTransform, 0, sizeof(D3DXVECTOR4));
		m_pfxDiffuseMap->SetResource(m_pDiffusMapSRV);
		m_pfxShadowMap->SetResource(m_pShadowMapSRV);
	}

	//--------------------------------------------------------------
	// 입력조립기 설정.
	//--------------------------------------------------------------
	m_pDevC->IASetInputLayout(m_pInputLayout);
	m_pDevC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//--------------------------------------------------------------
	// GPU에게 데이터 전달.
	//--------------------------------------------------------------
	//D3DX11_TECHNIQUE_DESC techDesc;
	//m_pTech->GetDesc(&techDesc);
	m_pTech->GetPassByIndex(_iPassNum)->Apply(0, m_pDevC);

	//--------------------------------------------------------------
	// 값 초기화.
	//--------------------------------------------------------------
	// ... 나중에 문제 발생시 추가.!!
	m_vTexTransform = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_pDiffusMapSRV = NULL;

}
#pragma endregion


#pragma region CFontShader
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CFontShader : 폰트 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// CFontShader()
CFontShader::CFontShader()
:CShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// ~CFontShader()
CFontShader::~CFontShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// _Init()
void CFontShader::_Init()
{
	D3D11_INPUT_ELEMENT_DESC InputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	_BuildFX();
	_BuildVertexLayout(InputLayout, sizeof(InputLayout));


	// 텍스처 샘플러 설정.
	//D3D11_SAMPLER_DESC samplerDesc;
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; 
	//samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
	//samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; 
	//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; 
	//samplerDesc.MipLODBias = 0.0f; 
	//samplerDesc.MaxAnisotropy = 1; 
	//samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; 
	//samplerDesc.BorderColor[0] = 0; 
	//samplerDesc.BorderColor[1] = 0;
	//samplerDesc.BorderColor[2] = 0; 
	//samplerDesc.BorderColor[3] = 0; 
	//samplerDesc.MinLOD = 0;
	//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; 
	//// Create the texture sampler state.
	//m_pDev->CreateSamplerState(&samplerDesc, &m_sampleState);

	m_pfxmWorld = m_pFX->GetVariableByName("g_mWorld")->AsMatrix();
	m_pfxmView = m_pFX->GetVariableByName("g_mView")->AsMatrix();
	m_pfxmProj = m_pFX->GetVariableByName("g_mProj")->AsMatrix();
	m_pfxcFontColor = m_pFX->GetVariableByName("g_cFontColor")->AsVector();
	m_pfxFontTexture = m_pFX->GetVariableByName("FontTex")->AsShaderResource();

	D3DXMatrixIdentity(&m_mWorld);
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProj);
	m_cFontColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_pFontTextureSRV = NULL;
	m_pCamera = NULL;
}




////////////////////////////////////////////////////////////////////////////////////
// SettingDataToGPU
void CFontShader::SettingDataToGPU(UINT _iPassNum)
{
	m_pfxmWorld->SetMatrix((float*)m_mWorld);
	m_pfxmView->SetMatrix((float*)m_pCamera->GetViewMatrix());
	m_pfxmProj->SetMatrix((float*)m_pCamera->GetOrthoMatrix());
	m_pfxcFontColor->SetRawValue(&m_cFontColor, 0, sizeof(m_cFontColor));
	m_pfxFontTexture->SetResource(m_pFontTextureSRV);

	//--------------------------------------------------------------
	// 입력조립기 설정.
	//--------------------------------------------------------------
	m_pDevC->IASetInputLayout(m_pInputLayout);
	m_pDevC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//--------------------------------------------------------------
	// GPU에게 데이터 전달.
	//--------------------------------------------------------------
	//D3DX11_TECHNIQUE_DESC techDesc;
	//m_pTech->GetDesc(&techDesc);
	m_pTech->GetPassByIndex(_iPassNum)->Apply(0, m_pDevC);	
}
#pragma endregion


#pragma region CNonLightingShader
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CNonLightingShader : 컬러 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
// CNonLightingShader()
CNonLightingShader::CNonLightingShader()
: CShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// ~CNonLightingShader()
CNonLightingShader::~CNonLightingShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// _Init()
void CNonLightingShader::_Init()
{
	D3D11_INPUT_ELEMENT_DESC InputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	_BuildFX();
	_BuildVertexLayout(InputLayout, sizeof(InputLayout));

	m_pfxmWVP = m_pFX->GetVariableByName("g_mWVP")->AsMatrix();
	m_pfxcColor = m_pFX->GetVariableByName("g_cColor")->AsVector();
}




////////////////////////////////////////////////////////////////////////////////////
// SetWorld()
void CNonLightingShader::SetWorld(D3DXMATRIX* _mWorld)
{
	m_mWorld = *_mWorld;
}




////////////////////////////////////////////////////////////////////////////////////
// SetCamera()
void CNonLightingShader::SetCamera(CCamera* _pCamera)
{
	m_mView = _pCamera->GetViewMatrix();
	m_mProj = _pCamera->GetProjMatrix();
}




////////////////////////////////////////////////////////////////////////////////////
// SetColor()
void CNonLightingShader::SetColor(D3DXCOLOR* _pCol)
{
	m_cColor = *_pCol;
}




/////////////////////////////////////////////////////////////////////////////////////
// SettingDataToGPU()
void CNonLightingShader::SettingDataToGPU(UINT _iPassNum)
{
	m_mWVP = m_mWorld * m_mView * m_mProj;
	m_pfxmWVP->SetMatrix((float*)m_mWVP);
	m_pfxcColor->SetRawValue(&m_cColor, 0, sizeof(D3DXCOLOR));

	//--------------------------------------------------------------
	// 입력조립기 설정.
	//--------------------------------------------------------------
	m_pDevC->IASetInputLayout(m_pInputLayout);
	m_pDevC->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	//--------------------------------------------------------------
	// GPU에게 데이터 전달.
	//--------------------------------------------------------------
	//D3DX11_TECHNIQUE_DESC techDesc;
	//m_pTech->GetDesc(&techDesc);
	m_pTech->GetPassByIndex(_iPassNum)->Apply(0, m_pDevC);
}
#pragma endregion


#pragma region CSkyShader
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CSkyShader : 스카이박스 관련 쉐이더
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////
// CSkyShader()
CSkyShader::CSkyShader()
: CShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// ~CSkyShader
CSkyShader::~CSkyShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// _Init()
void CSkyShader::_Init()
{
	D3D11_INPUT_ELEMENT_DESC InputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	_BuildFX();
	_BuildVertexLayout(InputLayout, sizeof(InputLayout));

	m_pfxmWVP = m_pFX->GetVariableByName("g_mWVP")->AsMatrix();
	m_pfxCubeTextureSRV = m_pFX->GetVariableByName("g_CubeMap")->AsShaderResource();

	D3DXMatrixIdentity(&m_mWorld);
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProj);
	m_pCubeTextureSRV = NULL;
}




////////////////////////////////////////////////////////////////////////////////////
//
void CSkyShader::SetWorld(D3DXMATRIX* m)
{
	m_mWorld = *m;
}




////////////////////////////////////////////////////////////////////////////////////
//
void CSkyShader::SetCamera(CCamera* camera)
{
	m_mView = camera->GetViewMatrix();
	m_mProj = camera->GetProjMatrix();
}




////////////////////////////////////////////////////////////////////////////////////
// 
void CSkyShader::SetTexture(ID3D11ShaderResourceView** _ppShaderResourceView, D3DXVECTOR4* _pTexTranform )
{
	m_pCubeTextureSRV = *_ppShaderResourceView;
}




////////////////////////////////////////////////////////////////////////////////////
// SettingDataToGPU()
void CSkyShader::SettingDataToGPU(UINT _iPassNum)
{
	D3DXMATRIX  mWVP = m_mWorld * m_mView * m_mProj;
	m_pfxmWVP->SetMatrix((float*)&mWVP);
	m_pfxCubeTextureSRV->SetResource(m_pCubeTextureSRV);

	//--------------------------------------------------------------
	// 입력조립기 설정.
	//--------------------------------------------------------------
	m_pDevC->IASetInputLayout(m_pInputLayout);
	m_pDevC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//--------------------------------------------------------------
	// GPU에게 데이터 전달.
	//--------------------------------------------------------------
	//D3DX11_TECHNIQUE_DESC techDesc;
	//m_pTech->GetDesc(&techDesc);
	m_pTech->GetPassByIndex(_iPassNum)->Apply(0, m_pDevC);
}
#pragma endregion


#pragma region CTerrainShader
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CTerrainShader : 지형 관련 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// CTerrainShader()
CTerrainShader::CTerrainShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// ~CTerrainShader()
CTerrainShader::~CTerrainShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// _Init()
void CTerrainShader::_Init()
{
	D3D11_INPUT_ELEMENT_DESC InputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	_BuildFX();
	_BuildVertexLayout(InputLayout, sizeof(InputLayout));

	m_pfxmWorld = m_pFX->GetVariableByName("g_mWorld")->AsMatrix();
	m_pfxmView = m_pFX->GetVariableByName("g_mView")->AsMatrix();
	m_pfxmProj = m_pFX->GetVariableByName("g_mProj")->AsMatrix();
	m_pfxmVP = m_pFX->GetVariableByName("g_mVP")->AsMatrix();
	m_pfxmWVP = m_pFX->GetVariableByName("g_mWVP")->AsMatrix();
	m_pfxvEyeL = m_pFX->GetVariableByName("g_vEyeL")->AsVector();
	m_pfxvTexTranform = m_pFX->GetVariableByName("g_vTexTransformL")->AsVector();
	m_pfxmShadow = m_pFX->GetVariableByName("g_mShadowTransform")->AsMatrix();


	m_pfxLight = m_pFX->GetVariableByName("g_Light");
	m_pfxMaterial = m_pFX->GetVariableByName("g_Material");
	m_pfxDiffuseMap = m_pFX->GetVariableByName("g_DiffuseMap")->AsShaderResource();
	m_pfxShadowMap = m_pFX->GetVariableByName("g_ShadowMap")->AsShaderResource();
	m_pfxFrustum = m_pFX->GetVariableByName("g_WorldFrustumPlanes")->AsVector();


	D3DXMatrixIdentity(&m_mWorld);
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProj);
	D3DXMatrixIdentity(&m_mVP);
	D3DXMatrixIdentity(&m_mWVP);

	m_pCamera = NULL;
	m_vEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vTexTransform = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	m_pDiffusMapSRV = NULL;
}




////////////////////////////////////////////////////////////////////////////////////
// SetFrustum()
void CTerrainShader::SetFrustum(CFrustum* _pFrustum)
{
	m_Frustum = *_pFrustum;
	D3DXVECTOR4 vPlane[6];
	for (UINT i = 0; i < 6; ++i){
		vPlane[i] = m_Frustum.GetPlane(i);
	}

	m_pfxFrustum->SetFloatVectorArray((float*)vPlane, 0, 6);
	return;
}




////////////////////////////////////////////////////////////////////////////////////
// SettingDataToGPU() : 설정한 데이터를 GPU에게 세팅해주는 함수.
void CTerrainShader::SettingDataToGPU(UINT _iPassNum)
{
	//--------------------------------------------------------------
	//	각종 데이터 처리.
	//--------------------------------------------------------------
	m_mWVP = m_mWorld * m_mVP;
	D3DXMATRIX imWorld;
	D3DXMatrixInverse(&imWorld, 0, &m_mWorld);
	_TransformLight(&imWorld);

	// 카메라 위치 로컬로 변환 후 GPU에게 전달.
	D3DXVECTOR3 vEyeL;
	D3DXVec3TransformCoord(&vEyeL, &m_vEye, &imWorld);

	//--------------------------------------------------------------
	// 상수버퍼 값 채우기.
	//--------------------------------------------------------------
	m_pfxmWorld->SetMatrix((float*)&m_mWorld);
	m_pfxmShadow->SetMatrix((float*)&(m_mWorld * m_mShadow));
	m_pfxmView->SetMatrix((float*)&m_mView);
	m_pfxmProj->SetMatrix((float*)&m_mProj);
	m_pfxmVP->SetMatrix((float*)&m_mVP);
	m_pfxmWVP->SetMatrix((float*)&m_mWVP);
	m_pfxvEyeL->SetRawValue(&vEyeL, 0, sizeof(D3DXVECTOR3));
	m_pfxMaterial->SetRawValue(&m_Material, 0, sizeof(MATERIAL));
	

	if (_iPassNum == PASS_LIGHTING_TEXTURE || _iPassNum == PASS_LIGHTING_TEXTURE_ALPHACLIP){
		m_pfxvTexTranform->SetRawValue(&m_vTexTransform, 0, sizeof(D3DXVECTOR4));
		m_pfxDiffuseMap->SetResource(m_pDiffusMapSRV);
		m_pfxShadowMap->SetResource(m_pShadowMapSRV);
	}

	//--------------------------------------------------------------
	// 입력조립기 설정.
	//--------------------------------------------------------------
	m_pDevC->IASetInputLayout(m_pInputLayout);
	m_pDevC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//--------------------------------------------------------------
	// GPU에게 데이터 전달.
	//--------------------------------------------------------------
	//D3DX11_TECHNIQUE_DESC techDesc;
	//m_pTech->GetDesc(&techDesc);
	m_pTech->GetPassByIndex(_iPassNum)->Apply(0, m_pDevC);

	//--------------------------------------------------------------
	// 값 초기화.
	//--------------------------------------------------------------
	// ... 나중에 문제 발생시 추가.!!
	m_vTexTransform = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_pDiffusMapSRV = NULL;

}
#pragma endregion


#pragma region CSpriteShader
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CSpriteShader : 쉐이더 관련 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// CSpriteShader()
CSpriteShader::CSpriteShader()
	:CShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// ~CSpriteShader()
CSpriteShader::~CSpriteShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// _Init()
void CSpriteShader::_Init()
{
	D3D11_INPUT_ELEMENT_DESC InputLayout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	_BuildFX();
	_BuildVertexLayout(InputLayout, sizeof(InputLayout));

	m_pfxmVP = m_pFX->GetVariableByName("g_mVP")->AsMatrix();
	m_pfxvEyeW = m_pFX->GetVariableByName("g_vEyePosW")->AsVector();
	m_pfxMaterial = m_pFX->GetVariableByName("g_Material");
	m_pfxEffectTex = m_pFX->GetVariableByName("g_EffectTexture")->AsShaderResource();
	m_pfxvTexTransform = m_pFX->GetVariableByName("g_vTexTransformL")->AsVector();

	D3DXMatrixIdentity(&m_mVP);
	m_vEye = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_pEffectTexSRV = NULL;
	m_vTexTransform = D3DXVECTOR4(0.f, 0.f, 1.f, 1.f);
}




////////////////////////////////////////////////////////////////////////////////////
//	SetCamera()
void CSpriteShader::SetCamera(CCamera* _pCamera)
{
	D3DXMATRIX mView = _pCamera->GetViewMatrix();
	D3DXMATRIX mProj = _pCamera->GetProjMatrix();

	m_mVP = mView * mProj;
	m_vEye = _pCamera->GetEye();
}




////////////////////////////////////////////////////////////////////////////////////
// SetMaterial()
void CSpriteShader::SetMaterial(MATERIAL* _pMaterial)
{
	m_Material = *_pMaterial;
}




////////////////////////////////////////////////////////////////////////////////////
// SetTexture()
void CSpriteShader::SetTexture(ID3D11ShaderResourceView** _ppShaderResourceView, D3DXVECTOR4* _pTexTranform)
{
	m_pEffectTexSRV = *_ppShaderResourceView;
	m_vTexTransform = *_pTexTranform;
}




////////////////////////////////////////////////////////////////////////////////////
// SettingDataToGPU()
void CSpriteShader::SettingDataToGPU(UINT _iPassNum)
{
	m_pfxmVP->SetMatrix((float*)&m_mVP);
	m_pfxvEyeW->SetRawValue(&m_vEye, 0, sizeof(D3DXVECTOR3));
	m_pfxMaterial->SetRawValue(&m_Material, 0, sizeof(MATERIAL));
	m_pfxEffectTex->SetResource(m_pEffectTexSRV);
	m_pfxvTexTransform->SetRawValue(&m_vTexTransform, 0, sizeof(D3DXVECTOR4));

	//--------------------------------------------------------------
	// 입력조립기 설정.
	//--------------------------------------------------------------
	m_pDevC->IASetInputLayout(m_pInputLayout);
	m_pDevC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//--------------------------------------------------------------
	// GPU에게 데이터 전달.
	//--------------------------------------------------------------
	//D3DX11_TECHNIQUE_DESC techDesc;
	//m_pTech->GetDesc(&techDesc);
	m_pTech->GetPassByIndex(_iPassNum)->Apply(0, m_pDevC);
}
#pragma endregion


#pragma region CBuildShaderMapShader
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//	CBuildShadowMapShader : 그림자맵 생성 쉐이더.
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// CBuildShadowMapShader()
CBuildShadowMapShader::CBuildShadowMapShader()
: CShader()
{

}




////////////////////////////////////////////////////////////////////////////////////
// ~CBuildShadowMapShader()
CBuildShadowMapShader::~CBuildShadowMapShader()
{

}




/////////////////////////////////////////////////////////////////////////////////////////////////
// _Init()
void CBuildShadowMapShader::_Init()
{
	D3D11_INPUT_ELEMENT_DESC InputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	_BuildFX();
	_BuildVertexLayout(InputLayout, sizeof(InputLayout));

	m_pfxvEyePosW			= m_pFX->GetVariableByName("g_vEyePosW")->AsVector();
	m_pfxfHeightScale		= m_pFX->GetVariableByName("g_fHeightScale")->AsScalar();
	m_pfxfMaxTessDistance	= m_pFX->GetVariableByName("g_fMaxTessDistance")->AsScalar();
	m_pfxfMinTessDistance	= m_pFX->GetVariableByName("g_fMinTessDistance")->AsScalar();
	m_pfxfMaxTessFactor		= m_pFX->GetVariableByName("g_fMaxTessFactor")->AsScalar();
	m_pfxfMinTessFactor		= m_pFX->GetVariableByName("g_fMinTessFactor")->AsScalar();

	m_pfxmWorld				 = m_pFX->GetVariableByName("g_mWorld")->AsMatrix();
	m_pfxmWorldInvTranspose	 = m_pFX->GetVariableByName("g_mWorldInvTranspose")->AsMatrix();
	m_pfxmViewProj			 = m_pFX->GetVariableByName("g_mViewProj")->AsMatrix();
	m_pfxmWorldViewPorj		 = m_pFX->GetVariableByName("g_mWorldViewProj")->AsMatrix();
	m_pfxmTexTransform		 = m_pFX->GetVariableByName("g_mTexTransform")->AsMatrix();

	return;
}




/////////////////////////////////////////////////////////////////////////////////////////////////
// SettingDataToGPU()
void CBuildShadowMapShader::SettingDataToGPU(UINT _iPassNum)
{
	D3DXMatrixInverse(&m_mWorldInvTranspose, 0, &m_mWorld);
	D3DXMatrixTranspose(&m_mWorldInvTranspose, &m_mWorldInvTranspose);
	m_mViewProj = m_mView * m_mProj;
	m_mWorldViewProj = m_mWorld * m_mView * m_mProj;
	D3DXMatrixIdentity(&m_mTexTransform);

	m_pfxvEyePosW->SetRawValue(&m_vEyePosW, 0, sizeof(D3DXVECTOR3));
	m_pfxmWorld->SetMatrix((float*)m_mWorld);
	m_pfxmWorldInvTranspose->SetMatrix((float*)m_mWorldInvTranspose);
	m_pfxmViewProj->SetMatrix((float*)m_mViewProj);
	m_pfxmWorldViewPorj->SetMatrix((float*)m_mWorldViewProj);
	m_pfxmTexTransform->SetMatrix((float*)m_mTexTransform);

	//--------------------------------------------------------------
	// 입력조립기 설정.
	//--------------------------------------------------------------
	m_pDevC->IASetInputLayout(m_pInputLayout);
	m_pDevC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//--------------------------------------------------------------
	// GPU에게 데이터 전달.
	//--------------------------------------------------------------
	//D3DX11_TECHNIQUE_DESC techDesc;
	//m_pTech->GetDesc(&techDesc);
	m_pTech->GetPassByIndex(_iPassNum)->Apply(0, m_pDevC);

	return;
}




#pragma endregion


//****************************** End of File "CShader.cpp" ******************************//

