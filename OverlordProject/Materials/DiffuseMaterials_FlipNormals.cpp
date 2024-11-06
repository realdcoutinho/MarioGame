#include "stdafx.h"
#include "DiffuseMaterials_FlipNormals.h"

DiffuseMaterials_FlipNormals::DiffuseMaterials_FlipNormals() :
	Material<DiffuseMaterials_FlipNormals>(L"Effects/PosNormTex3D_NegativeNormals.fx")
{}

void DiffuseMaterials_FlipNormals::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEffectVariables();
}

void DiffuseMaterials_FlipNormals::FlipNormals()
{
	SetVariable_Vector(L"gLightDirection", XMFLOAT4(0.577f, 0.577f, -0.577f, 0.0f));
}

void DiffuseMaterials_FlipNormals::InitializeEffectVariables()
{
	if (m_pDiffuseTexture)
	{
		SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture->GetShaderResourceView());
	}
}