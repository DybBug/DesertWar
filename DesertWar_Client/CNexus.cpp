//*************************************************************************************
//*************************************************************************************
//
//	CNexus.cpp : 넥서스 관련 소스 파일.
//
//*************************************************************************************
//*************************************************************************************
#include "CGameEngine.h"
#include "CNexus.h"
#include "RenderState.h"

///////////////////////////////////////////////////////////////////////////////////////
// CNexus()
CNexus::CNexus(ID3D11Device* _pDev, ID3D11DeviceContext* _pDevC, CShader* _pShader,
				CMeshNode*_pMesh, D3DXVECTOR3 _vPos, float _fHP)
: CObject(_pDev, _pDevC, _pShader)
{
	m_fPrevHP = m_fHP = _fHP;
	m_vPos = _vPos;
	m_pMesh = _pMesh;

	m_fAngle = 0.0f;
	m_fRotateSpeed = 45.0f;

	// 충돌 박스 초기화.
	m_pCollisionBox = new CCubeMesh(30.f, 90.f, 30.f);
	m_pCollisionBox->CreateBuffer(m_pDev);
}




///////////////////////////////////////////////////////////////////////////////////////
// ~CNexus()
CNexus::~CNexus()
{

}




///////////////////////////////////////////////////////////////////////////////////////
// _Draw();
void CNexus::_Draw(CMeshNode* _pMesh)
{
	MATERIAL m;
	if (m_fPrevHP != m_fHP){
		m.s_cDiffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	}
	else m.s_cDiffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);


	m_pShader->SetMaterial(&m);

	if (_pMesh->GetParent() != NULL){
		m_pShader->SetTexture(_pMesh->GetDiffuseMapSRV());
		if (_pMesh->GetName() == "Tower_Rocket Launcher-R"){
			m_pShader->SetWorld(&(m_mRot * _pMesh->GetTM() * m_mTrans));
		}
		else{
			m_pShader->SetWorld(&(_pMesh->GetTM() * m_mTrans));
		}
		m_pShader->SettingDataToGPU(PASS_LIGHTING_TEXTURE);
		_pMesh->Render(m_pDevC);
	}
	UINT ccount = _pMesh->GetChildCount();
	for (UINT i = 0; i < ccount; ++i){
		_Draw(_pMesh->GetChild(i));
	}

}




/////////////////////////////////////////////////////////////////////////////////////////////
// _ShadowDraw()
void CNexus::_ShadowDraw(CMeshNode* _pMesh)
{

	if (_pMesh->GetParent() != NULL){
		if (_pMesh->GetName() == "Tower_Rocket Launcher-R"){
			m_pShadowShader->SetWorld(&(m_mRot * _pMesh->GetTM() * m_mTrans));
		}
		else{
			m_pShadowShader->SetWorld(&(_pMesh->GetTM() * m_mTrans));
		}
		m_pShadowShader->SettingDataToGPU();
		_pMesh->Render(m_pDevC);
	}
	UINT ccount = _pMesh->GetChildCount();
	for (UINT i = 0; i < ccount; ++i){
		_ShadowDraw(_pMesh->GetChild(i));
	}
}



///////////////////////////////////////////////////////////////////////////////////////
// Update()
void CNexus::Update(float _fDTime)
{
	if (m_fHP <= 0.f){
		m_bLive = false;
		return;
	}
	m_fAngle += m_fRotateSpeed * _fDTime;
	D3DXMatrixRotationY(&m_mRot, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&m_mTrans, m_vPos.x, m_vPos.y, m_vPos.z);
}




///////////////////////////////////////////////////////////////////////////////////////
// Draw()
void CNexus::Render()
{
	_Draw(m_pMesh);

	m_fPrevHP = m_fHP;

	if (GameEngine->CollisionBox() == true){
		m_pDevC->RSSetState(ResterRizerState::m_pWireframeRS);
		MATERIAL m;
		m.s_cDiffuse = m.s_cAmbient = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		m_pShader->SetMaterial(&m);
		m_pShader->SetWorld(&m_mTM);
		m_pShader->SettingDataToGPU(PASS_LIGHTING);

		m_pCollisionBox->Render(m_pDevC);
		m_pDevC->RSSetState(ResterRizerState::m_pSolidRS);
	}
}




//////////////////////////////////////////////////////////////////////////////////////////////
//	ShadowRender()
void CNexus::ShadowRender(CBuildShadowMapShader* _pShader)
{
	m_pShadowShader = _pShader;

	_ShadowDraw(m_pMesh);
}


/****************************** End of File "Nexus.cpp" ******************************/