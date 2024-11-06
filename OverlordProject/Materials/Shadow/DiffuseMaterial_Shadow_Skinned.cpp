#include "stdafx.h"
#include "DiffuseMaterial_Shadow_Skinned.h"

DiffuseMaterial_Shadow_Skinned::DiffuseMaterial_Shadow_Skinned():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow_Skinned.fx")
{}

void DiffuseMaterial_Shadow_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow_Skinned::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow_Skinned::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	
	 //* TODO_W8
	 //* Update The Shader Variables
	 //* 1. Update the LightWVP > Used to Transform a vertex into Light clipping space

	const auto pShadowMapRenderer = ShadowMapRenderer::Get();

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
	 //* 	(light_viewprojection [LightVP] can be acquired from the ShadowMapRenderer)
	SetVariable_Matrix(L"gWorldViewProj_Light", LightWVP);


	 //* 2. Update the ShadowMap texture
	SetVariable_Texture(L"gShadowMap", pShadowMapRenderer->GetShadowMap());

	 //* 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);

	 //* 4. Update Bones
	SetVariable_MatrixArray(L"gBones", &pModel->GetAnimator()->GetBoneTransforms()[0]._11, int(pModel->GetAnimator()->GetBoneTransforms().size()));









}
