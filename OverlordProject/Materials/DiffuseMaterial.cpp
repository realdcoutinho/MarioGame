#include "stdafx.h"
#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial() :
	Material<DiffuseMaterial>(L"Effects/PosNormTex3D.fx")
{}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEffectVariables();
}

void DiffuseMaterial::FlipNormals()
{
	SetVariable_Vector(L"gLightDirection", XMFLOAT4( 0.577f, 0.577f, -0.577f, 0.0f));
}

void DiffuseMaterial::InitializeEffectVariables()
{
	if (m_pDiffuseTexture)
	{
		SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture->GetShaderResourceView());
	}
}