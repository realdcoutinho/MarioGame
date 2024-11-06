#include "stdafx.h"
#include "DiffuseMaterial_Shadow_NegativeNormals.h"

#include "stdafx.h"
#include "DiffuseMaterial_Shadow.h"

DiffuseMaterial_Shadow_NegativeNormals::DiffuseMaterial_Shadow_NegativeNormals() :
	Material(L"Effects/Shadow/PosNormTex3D_Shadow_NegativeNormals.fx")
{}


void DiffuseMaterial_Shadow_NegativeNormals::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow_NegativeNormals::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow_NegativeNormals::FlipLight()
{
	//SetVariable_Vector(L"gLightDirection", XMFLOAT4(0.f, -1.f, 0.f, 0.0f));
}

void DiffuseMaterial_Shadow_NegativeNormals::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	/*
	 * TODO_W8
	 * Update The Shader Variables
	 * 1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	 * 	LightWVP = model_world * light_viewprojection
	 * 	(light_viewprojection [LightVP] can be acquired from the ShadowMapRenderer)
	 *
	 * 2. Update the ShadowMap texture
	 *
	 * 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	*/

	//Update Shadow Variables
	const auto pShadowMapRenderer = ShadowMapRenderer::Get();
	//...


	XMFLOAT4X4 world = pModel->GetTransform()->GetWorld();
	XMFLOAT4X4 lightVP = pShadowMapRenderer->GetLightVP();
	// Convert the XMFLOAT4X4 matrices to XMMATRIX
	XMMATRIX xmMat1 = XMLoadFloat4x4(&world);
	XMMATRIX xmMat2 = XMLoadFloat4x4(&lightVP);

	//* 	LightWVP = model_world * light_viewprojection

	// Multiply the two XMMATRIX matrices together
	XMMATRIX xmResult = xmMat1 * xmMat2;

	// Convert the result back to XMFLOAT4X4
	XMFLOAT4X4 LightWVP;
	XMStoreFloat4x4(&LightWVP, xmResult);

	SetVariable_Matrix(L"gWorldViewProj_Light", LightWVP);

	SetVariable_Texture(L"gShadowMap", ShadowMapRenderer::Get()->GetShadowMap());

	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
}
